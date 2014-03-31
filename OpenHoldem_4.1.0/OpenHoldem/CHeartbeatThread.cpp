#include "stdafx.h"
#include "CHeartbeatThread.h"

#include <process.h>
#include "CAutoconnector.h"
#include "CAutoplayer.h"
#include "CAutoplayerFunctions.h"
#include "CBetroundCalculator.h"
#include "CSymbolEngineChipAmounts.h"
#include "CEngineContainer.h"
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
#include "CScraperAccess.h"
#include "ScraperOverride.h"
#include "CStableFramesCounter.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineUserchair.h"
#include "CSymbols.h"
#include "..\CTablemap\CTablemap.h"
#include "CTableMapLoader.h"
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

void CHeartbeatThread::SetOpenHoldemWindowTitle()
{
	char title[MAX_WINDOW_TITLE];
	CString	*messageTitle = new CString;

	GetWindowText(p_autoconnector->attached_hwnd(), title, MAX_WINDOW_TITLE);
	messageTitle->Format("%s - %s (%s)", p_formula->formula_name(), p_tablemap->sitename(), title);
	theApp.m_pMainWnd->PostMessage(WMA_SETWINDOWTEXT, 0, (LPARAM)messageTitle);
}

UINT CHeartbeatThread::HeartbeatThreadFunction(LPVOID pParam)
{
	CHeartbeatThread	*pParent = static_cast<CHeartbeatThread*>(pParam);

	LARGE_INTEGER		cycle_start = {0}, cycle_end = {0}, lFrequency = {0};
	unsigned int		new_scrape = NOTHING_CHANGED;
	bool				iswait = false;
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

		p_tablemap_loader->ReloadAllTablemapsIfChanged();

		// This critical section lets other threads know that the internal state is being updated
		EnterCriticalSection(&pParent->cs_update_in_progress);

		////////////////////////////////////////////////////////////////////////////////////////////
		// Scrape window

		write_log(prefs.debug_heartbeat(), "[HeartBeatThread] Calling DoScrape.\n");
		new_scrape = !NOTHING_CHANGED;
		p_lazyscraper->DoScrape();

		OverriderScraperStateByScraperPreprocessorDLLIfNecessary();

		// Necessary to pre-compute some info 
		// which is needed by the symbol-engines.
		p_scraper_access->GetNeccessaryTablemapObjects();
		////////////////////////////////////////////////////////////////////////////////////////////
		// Caclulate symbols

		// mark symbol result cache as stale
		p_formula->MarkCacheStale();

		if (new_scrape!=NOTHING_CHANGED)
		{
			p_engine_container->CallSymbolEnginesToUpdateSymbolsIfNecessary();
		}

		LeaveCriticalSection(&pParent->cs_update_in_progress);

		SetOpenHoldemWindowTitle();
		
		////////////////////////////////////////////////////////////////////////////////////////////
		// If we've folded, stop iterator thread and set prwin/tie/los to zero
		if (!p_scraper_access->UserHasCards())
		{
			p_iterator_thread->StopIteratorThread();
		}

		////////////////////////////////////////////////////////////////////////////////////////////
		// Create replay frame

		static double last_br = 0; 
		static double last_ncallbets = -1; 

		if (p_handreset_detector->IsHandreset() ||
			p_betround_calculator->betround() != last_br ||
			p_symbol_engine_chip_amounts->ncallbets() != last_ncallbets)
		{
			last_br = p_betround_calculator->betround();
			last_ncallbets = p_symbol_engine_chip_amounts->ncallbets();

			p_heartbeat_thread->set_replay_recorded_this_turn(false);
		}

		 // If it's my turn, and we have enough stable frames
		if ((prefs.replay_record() 			
			&& p_symbol_engine_autoplayer->ismyturn() 
			&& p_stableframescounter->NumberOfStableFrames() >= prefs.frame_delay()) 
			&& !p_heartbeat_thread->replay_recorded_this_turn()
			 
			 ||

			 // Every change
			 (prefs.replay_record_every_change() && new_scrape!=NOTHING_CHANGED) 
			 
			 ||

			 // Every change and I'm playing
			 (prefs.replay_record_every_change_playing() && (p_symbol_engine_userchair->userchair_confirmed() &&
			   !(p_scraper->card_player(p_symbol_engine_userchair->userchair(), 0)==CARD_NOCARD ||
			     p_scraper->card_player(p_symbol_engine_userchair->userchair(), 0)==CARD_BACK ||
			     p_scraper->card_player(p_symbol_engine_userchair->userchair(), 1)==CARD_NOCARD ||
			     p_scraper->card_player(p_symbol_engine_userchair->userchair(), 1)==CARD_BACK))&& new_scrape != NOTHING_CHANGED)
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
		{
			m_ScraperOutputDlg->UpdateDisplay();
		}

		// Measure cycle time
		QueryPerformanceCounter(&cycle_end);
		QueryPerformanceFrequency(&lFrequency);

		////////////////////////////////////////////////////////////////////////////////////////////
		// Save state
		write_log(prefs.debug_heartbeat(), "[HeartBeatThread] Calling CaptureState.\n");
		p_game_state->CaptureState();

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
		if (p_dll_extension->IsDllLoaded() && p_symbol_engine_autoplayer->ismyturn())
		{
			iswait = (p_dll_extension->process_message()) ("query", "dll$iswait");
		}
		else
		{
			iswait = false;
		}

		write_log(prefs.debug_heartbeat(), "[HeartBeatThread] autoplayer_engaged(): %i\n", p_autoplayer->autoplayer_engaged());
		write_log(prefs.debug_heartbeat(), "[HeartBeatThread] user_chair_confirmed(): %i\n", p_symbol_engine_userchair->userchair_confirmed());
		write_log(prefs.debug_heartbeat(), "[HeartBeatThread] iswait: %i\n", iswait);
		// If autoplayer is engaged, we know our chair, and the DLL hasn't told us to wait, then go do it!
		if (p_autoplayer->autoplayer_engaged() && p_symbol_engine_userchair->userchair_confirmed() && !iswait)
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
