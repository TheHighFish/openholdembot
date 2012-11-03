#include "stdafx.h"
#include "CHeartbeatThread.h"

#include <process.h>
#include "CAutoconnector.h"
#include "CAutoplayer.h"
#include "CAutoplayerFunctions.h"
#include "CFormula.h"
#include "CGameState.h"
#include "CHandhistory.h"
#include "CHandresetDetector.h"
#include "CIteratorThread.h"
#include "CLazyScraper.h"
#include "CopenHoldemHopperCommunication.h"
#include "CopenHoldemStatusbar.h"
#include "CPreferences.h"
#include "CReplayFrame.h"
#include "CScraper.h"
#include "..\..\Reference Scraper DLL\scraperdll.h"
#include "CStableFramesCounter.h"
#include "CSymbols.h"
#include "..\CTablemap\CTablemap.h"
#include "CValidator.h"
#include "DialogScraperOutput.h"
#include "MainFrm.h"
#include "OpenHoldem.h"
#include "CHandHistory.h"

CHeartbeatThread	*p_heartbeat_thread = NULL;
CRITICAL_SECTION	CHeartbeatThread::cs_update_in_progress;
long int			_heartbeat_counter = 0;

CHeartbeatThread::CHeartbeatThread()
{
	InitializeCriticalSectionAndSpinCount(&cs_update_in_progress, 4000);

	_heartbeat_counter = 0;

	// Create events
	_m_stop_thread = CreateEvent(0, TRUE, FALSE, 0);
	_m_wait_thread = CreateEvent(0, TRUE, FALSE, 0);

	_replay_recorded_this_turn = false;

	// Start thread
	AfxBeginThread(HeartbeatThreadFunction, this);
}

CHeartbeatThread::~CHeartbeatThread()
{
	// Trigger thread to stop
	::SetEvent(_m_stop_thread);

	// Wait until thread finished
	::WaitForSingleObject(_m_wait_thread, INFINITE);

	// Close handles
	::CloseHandle(_m_stop_thread);
	::CloseHandle(_m_wait_thread);

	DeleteCriticalSection(&cs_update_in_progress);

	p_heartbeat_thread = NULL;
}

UINT CHeartbeatThread::HeartbeatThreadFunction(LPVOID pParam)
{
	CHeartbeatThread	*pParent = static_cast<CHeartbeatThread*>(pParam);

	LARGE_INTEGER		cycle_start = {0}, cycle_end = {0}, lFrequency = {0};
	unsigned int		new_scrape = NOTHING_CHANGED;
	bool				iswait = false;
	char				title[MAX_WINDOW_TITLE] = {0};
	int					N = 0;

	_heartbeat_counter++;

	// Seed the RNG
	srand((unsigned)GetTickCount());

	while (true)
	{
		write_log(prefs.debug_heartbeat(), "[HeartBeatThread] Heartbeat start\n");

		// Check event for stop thread
		if(::WaitForSingleObject(pParent->_m_stop_thread, 0) == WAIT_OBJECT_0)
		{
			// Set event
			::SetEvent(pParent->_m_wait_thread);
			AfxEndThread(0);
		}

		// Measure cycle time
		QueryPerformanceCounter(&cycle_start);


		// This critical section lets other threads know that the internal state is being updated
		EnterCriticalSection(&pParent->cs_update_in_progress);

			////////////////////////////////////////////////////////////////////////////////////////////
			// Scrape window

			write_log(prefs.debug_heartbeat(), "[HeartBeatThread] Calling DoScrape.\n");
			new_scrape = true; //!!!
			p_lazyscraper->DoScrape();

			////////////////////////////////////////////////////////////////////////////////////////////
			// Give scraper.dll a chance to override scraper results

			if (theApp._scraper_dll)
			{
				SScraperState  ss;

				// populate structure that gets passed to dll
				strncpy_s(ss.title, MAX_WINDOW_TITLE, p_scraper->title(), MAX_WINDOW_TITLE);

				for (int i=0; i<k_number_of_community_cards; i++)
				{
					ss.card_common[i] = p_scraper->card_common(i);
				}

				for (int i=0; i<k_max_number_of_players; i++)
				{
					ss.card_player[i][0] = p_scraper->card_player(i, 0);
					ss.card_player[i][1] = p_scraper->card_player(i, 1);
					ss.dealer[i] = p_scraper->dealer(i);
					ss.sitting_out[i] = p_scraper->sitting_out(i);
					ss.seated[i] = p_scraper->seated(i);
					ss.active[i] = p_scraper->active(i);
					ss.name[i] = p_scraper->player_name(i);
					ss.balance[i] = p_scraper->player_balance(i);
					ss.name_good_scrape[i] = p_scraper->name_good_scrape(i);
					ss.balance_good_scrape[i] = p_scraper->balance_good_scrape(i);
					ss.bet[i] = p_scraper->player_bet(i);
				}

				ss.card_player_for_display[0] = p_scraper->card_player_for_display(0);
				ss.card_player_for_display[1] = p_scraper->card_player_for_display(1);

				for (int i=0; i<k_max_number_of_pots; i++)
				{
					ss.pot[i] = p_scraper->pot(i);
				}

				for (int i=0; i<k_max_number_of_buttons; i++)
				{
					ss.button_state[i] = p_scraper->button_state(i);
					ss.i86X_button_state[i] = p_scraper->i86X_button_state(i);
					ss.button_label[i] = p_scraper->button_label(i);
				}

				ss.i86_button_state = p_scraper->i86_button_state();

				ss.sblind = p_scraper->s_limit_info()->sblind;
				ss.found_sblind = p_scraper->s_limit_info()->found_sblind;
				ss.bblind = p_scraper->s_limit_info()->bblind;
				ss.found_bblind = p_scraper->s_limit_info()->found_bblind;
				ss.bbet = p_scraper->s_limit_info()->bbet;
				ss.found_bbet = p_scraper->s_limit_info()->found_bbet;
				ss.ante = p_scraper->s_limit_info()->ante;
				ss.found_ante = p_scraper->s_limit_info()->found_ante;
				ss.limit = (LimitType) p_scraper->s_limit_info()->limit;
				ss.found_limit = p_scraper->s_limit_info()->found_limit;
				ss.handnumber = p_scraper->s_limit_info()->handnumber;
				ss.found_handnumber = p_scraper->s_limit_info()->found_handnumber;
				ss.istournament = p_scraper->s_limit_info()->istournament;

				// Call the scraper override
				write_log(prefs.debug_heartbeat(), "[HeartBeatThread] Sending scraper data to override DLL.\n");
				(theApp._dll_scraper_override) (&ss);

				// Replace values in p_scraper with those provided by scraper dll
				p_scraper->set_title(ss.title);

				for (int i=0; i<k_number_of_community_cards; i++)
				{
					p_scraper->set_card_common(i, ss.card_common[i]);
				}

				for (int i=0; i<k_max_number_of_players; i++)
				{
					p_scraper->set_card_player(i, 0, ss.card_player[i][0]);
					p_scraper->set_card_player(i, 1, ss.card_player[i][1]);
					p_scraper->set_dealer(i, ss.dealer[i]);
					p_scraper->set_sitting_out(i, ss.sitting_out[i]);
					p_scraper->set_seated(i, ss.seated[i]);
					p_scraper->set_active(i, ss.active[i]);
					p_scraper->set_player_name(i, ss.name[i]);
					p_scraper->set_player_balance(i, ss.balance[i]);
					p_scraper->set_name_good_scrape(i, ss.name_good_scrape[i]);
					p_scraper->set_balance_good_scrape(i, ss.balance_good_scrape[i]);
					p_scraper->set_player_bet(i, ss.bet[i]);
				}

				p_scraper->set_card_player_for_display(0, ss.card_player_for_display[0]);
				p_scraper->set_card_player_for_display(1, ss.card_player_for_display[1]);

				for (int i=0; i<k_max_number_of_pots; i++)
				{
					p_scraper->set_pot(i, ss.pot[i]);
				}

				for (int i=0; i<k_max_number_of_buttons; i++)
				{
					p_scraper->set_button_state(i, ss.button_state[i]);
					p_scraper->set_i86X_button_state(i, ss.i86X_button_state[i]);
					p_scraper->set_button_label(i, ss.button_label[i]);
				}

				p_scraper->set_i86_button_state(ss.i86_button_state);

				p_scraper->set_sblind(ss.sblind);
				p_scraper->set_found_sblind(ss.found_sblind);
				p_scraper->set_bblind(ss.bblind);
				p_scraper->set_found_bblind(ss.found_bblind);
				p_scraper->set_bbet(ss.bbet);
				p_scraper->set_found_bbet(ss.found_bbet);
				p_scraper->set_ante(ss.ante);
				p_scraper->set_found_ante(ss.found_ante);
				p_scraper->set_limit(ss.limit);
				p_scraper->set_found_limit(ss.found_limit);
				p_scraper->set_handnumber(ss.handnumber);
				p_scraper->set_found_handnumber(ss.found_handnumber);
				p_scraper->set_istournament(ss.istournament);

			}


			////////////////////////////////////////////////////////////////////////////////////////////
			// Caclulate symbols

			p_handreset_detector->OnNewHeartbeat();

			// mark symbol result cache as stale
			p_formula->MarkCacheStale();

			if (new_scrape!=NOTHING_CHANGED)
			{
				write_log(prefs.debug_heartbeat(), "[HeartBeatThread] Calling CalcSymbols.\n");
				p_symbols->CalcSymbols();
			}
			else
			{
				write_log(prefs.debug_heartbeat(), "[HeartBeatThread] Calling CalcTime.\n");
				p_symbols->CalcTime();
				write_log(prefs.debug_heartbeat(), "[HeartBeatThread] Calling CalcProbabilities.\n");
				p_symbols->CalcProbabilities();
			}

		LeaveCriticalSection(&pParent->cs_update_in_progress);

		////////////////////////////////////////////////////////////////////////////////////////////
		// Set Title of window
		CString *messageTitle = new CString();
		GetWindowText(p_autoconnector->attached_hwnd(), title, MAX_WINDOW_TITLE);
		messageTitle->Format("%s - %s (%s)", p_formula->formula_name(), p_tablemap->sitename(), title);
		theApp.m_pMainWnd->PostMessage(WMA_SETWINDOWTEXT, 0, (LPARAM)messageTitle);


		////////////////////////////////////////////////////////////////////////////////////////////
		// If we've folded, stop iterator thread and set prwin/tie/los to zero
		if (p_symbols->user_chair_confirmed() &&
			(p_scraper->card_player(p_symbols->sym()->chair, 0)==CARD_NOCARD || 
			 p_scraper->card_player(p_symbols->sym()->chair, 0)==CARD_BACK ||
			 p_scraper->card_player(p_symbols->sym()->chair, 1)==CARD_NOCARD || 
			 p_scraper->card_player(p_symbols->sym()->chair, 1)==CARD_BACK))
		{
			if (p_iterator_thread)
			{
				write_log(prefs.debug_heartbeat(), "[HeartBeatThread] Stopping iterator thread.\n");
				delete p_iterator_thread;
				p_iterator_thread = NULL;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////
		// Create replay frame

		static double last_br = 0; 
		static double last_ncallbets = -1; 

		last_br = p_symbols->sym()->betround;
		last_ncallbets = p_symbols->sym()->ncallbets;

		if (p_handreset_detector->IsHandreset() ||
			p_symbols->sym()->betround != last_br ||
			p_symbols->sym()->ncallbets != last_ncallbets)
		{
			p_heartbeat_thread->set_replay_recorded_this_turn(false);
		}

		 // If it's my turn, and we have enough stable frames
		if ((prefs.replay_record() 			
			&& p_symbols->sym()->ismyturn 
			&& p_stableframescounter->NumberOfStableFrames() >= prefs.frame_delay()) 
			&& !p_heartbeat_thread->replay_recorded_this_turn()
			 
			 ||

			 // Every change
			 (prefs.replay_record_every_change() && new_scrape!=NOTHING_CHANGED) 
			 
			 ||

			 // Every change and I'm playing
			 (prefs.replay_record_every_change_playing() && (p_symbols->user_chair_confirmed() &&
			   !(p_scraper->card_player(p_symbols->sym()->chair, 0)==CARD_NOCARD ||
			     p_scraper->card_player(p_symbols->sym()->chair, 0)==CARD_BACK ||
			     p_scraper->card_player(p_symbols->sym()->chair, 1)==CARD_NOCARD ||
			     p_scraper->card_player(p_symbols->sym()->chair, 1)==CARD_BACK))&& new_scrape != NOTHING_CHANGED)
		   )
		{
			write_log(prefs.debug_heartbeat(), "[HeartBeatThread] Calling CreateReplayFrame.\n");
			CReplayFrame   crf;
			crf.CreateReplayFrame();
			p_heartbeat_thread->set_replay_recorded_this_turn(p_autoplayer->autoplayer_engaged());
		}

		////////////////////////////////////////////////////////////////////////////////////////////
		// Update scraper output dialog if it is present
		if (m_ScraperOutputDlg)
			m_ScraperOutputDlg->UpdateDisplay();

		// Measure cycle time
		QueryPerformanceCounter(&cycle_end);
		QueryPerformanceFrequency(&lFrequency);

		////////////////////////////////////////////////////////////////////////////////////////////
		// Save state
		write_log(prefs.debug_heartbeat(), "[HeartBeatThread] Calling CaptureState.\n");
		p_game_state->CaptureState(title);

		////////////////////////////////////////////////////////////////////////////////////////////
		// Game state engine
		write_log(prefs.debug_heartbeat(), "[HeartBeatThread] Calling ProcessGameState.\n");
		p_game_state->ProcessGameState(p_game_state->state((p_game_state->state_index()-1)&0xff));
		write_log(prefs.debug_heartbeat(), "[HeartBeatThread] Calling ProcessFtr.\n");
		p_game_state->ProcessFtr(p_game_state->state((p_game_state->state_index()-1)&0xff));

		////////////////////////////////////////////////////////////////////////////////////////////
		// OH-Validator
		write_log(prefs.debug_heartbeat(), "[HeartBeatThread] Calling ValidateGameState.\n");
		p_validator->ValidateGameState();

		////////////////////////////////////////////////////////////////////////////////////////////
		// DLL - always send state
		write_log(prefs.debug_heartbeat(), "[HeartBeatThread] Calling PassStateToDll.\n");
		p_dll_extension->PassStateToDll(p_game_state->state((p_game_state->state_index()-1)&0xff));

		////////////////////////////////////////////////////////////////////////////////////////////
		// Autoplayer
		if (p_dll_extension->IsDllLoaded() && p_symbols->sym()->ismyturn)
		{
			iswait = (p_dll_extension->process_message()) ("query", "dll$iswait");
		}
		else
		{
			iswait = false;
		}

		write_log(prefs.debug_heartbeat(), "[HeartBeatThread] autoplayer_engaged(): %i\n", p_autoplayer->autoplayer_engaged());
		write_log(prefs.debug_heartbeat(), "[HeartBeatThread] user_chair_confirmed(): %i\n", p_symbols->user_chair_confirmed());
		write_log(prefs.debug_heartbeat(), "[HeartBeatThread] iswait: %i\n", iswait);
		// If autoplayer is engaged, we know our chair, and the DLL hasn't told us to wait, then go do it!
		if (p_autoplayer->autoplayer_engaged() && p_symbols->user_chair_confirmed() && !iswait)
		{
			write_log(prefs.debug_heartbeat(), "[HeartBeatThread] Calling DoAutoplayer.\n");
			p_autoplayer->DoAutoplayer();
		}
		
		////////////////////////////////////////////////////////////////////////////////////////////
		// Hand history generator

		if (prefs.handhistory_generator_enable())
		{
			p_handhistory->MakeHistory();
		}

		////////////////////////////////////////////////////////////////////////////////////////////
		// Switching from basic to advanced statusbar
		// with detailed info instead of hint for beginners
		p_openholdem_statusbar->SwitchToAdvancedStatusbarAfterFirstHand();

		////////////////////////////////////////////////////////////////////////////////////////////
		// Finally sleeping
		write_log(prefs.debug_heartbeat(), "[HeartBeatThread] Sleeping %d ms.\n", prefs.scrape_delay());
		Sleep(prefs.scrape_delay());

		write_log(prefs.debug_heartbeat(), "[HeartBeatThread] Heartbeat end.\n");
	}
}
