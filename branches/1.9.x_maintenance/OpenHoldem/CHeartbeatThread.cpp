#include "stdafx.h"
#include <process.h>

#include "OpenHoldem.h"
#include "MainFrm.h"

#include "CScraper.h"
#include "CSymbols.h"
#include "CFormula.h"
#include "CAutoplayer.h"
#include "CIteratorThread.h"
#include "CHeartbeatThread.h"
#include "CPokerTrackerThread.h"
#include "CPreferences.h"
#include "CGameState.h"
#include "..\CTablemap\CTablemap.h"
#include "CReplayFrame.h"

#include "DialogScraperOutput.h"
#include "CPokerPro.h"

CHeartbeatThread	*p_heartbeat_thread = NULL;
CRITICAL_SECTION	CHeartbeatThread::cs_update_in_progress;

CHeartbeatThread::CHeartbeatThread()
{
	__SEH_SET_EXCEPTION_HANDLER

	InitializeCriticalSectionAndSpinCount(&cs_update_in_progress, 4000);

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
	__SEH_SET_EXCEPTION_HANDLER

	CHeartbeatThread	*pParent = static_cast<CHeartbeatThread*>(pParam);
	CMainFrame			*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

	LARGE_INTEGER		cycle_start = {0}, cycle_end = {0}, lFrequency = {0};
	unsigned int		new_scrape = NOTHING_CHANGED;
	bool				iswait = false;
	char				title[512] = {0};
	int					N = 0, i = 0;

	// PokerPro variables only
	const char			*pbytes = NULL;
	int					nbytes = 0, result = 0;
	fd_set				fd;
	timeval				tv;

	// Seed the RNG
	srand((unsigned)GetTickCount());

	while (true)
	{
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

			if (!p_pokerpro->IsConnected())
			{
				new_scrape = p_scraper->DoScrape();

				LARGE_INTEGER PerformanceCount;
				QueryPerformanceCounter(&PerformanceCount);
				p_scraper->set_clocks_hold(PerformanceCount);
			}
			else
			{
				if (p_pokerpro->ppdata()->m_tinf.m_tid != 0)
				{
					p_pokerpro->DoScrape();
				}
			}

			////////////////////////////////////////////////////////////////////////////////////////////
			// Caclulate symbols

			// mark symbol result cache as stale
			p_formula->MarkCacheStale();

			if (new_scrape!=NOTHING_CHANGED || (p_pokerpro->IsConnected() && p_pokerpro->ppdata()->m_tinf.m_tid != 0))
			{
				p_symbols->CalcSymbols();
			}
			else
			{
				p_symbols->CalcTime();
				p_symbols->CalcProbabilities();
			}

		LeaveCriticalSection(&pParent->cs_update_in_progress);

		////////////////////////////////////////////////////////////////////////////////////////////
		// Set Title of window
		CString *messageTitle = new CString();
		if (!p_pokerpro->IsConnected())
		{
			GetWindowText(pMyMainWnd->attached_hwnd(), title, 512);
			messageTitle->Format("%s - %s (%s)", p_formula->formula_name(), p_tablemap->s$items()->sitename, title);
		}
		else
		{
			if (p_pokerpro->ppdata()->m_tinf.m_tid == 0)
				_snprintf_s(title, _countof(title), _TRUNCATE, "%s", p_pokerpro->ppdata()->m_site_name);

			else if (p_pokerpro->ppdata()->m_userchair!=-1)
				_snprintf_s(title, _countof(title), _TRUNCATE, "%s - %s - %s", p_pokerpro->ppdata()->m_site_name, p_pokerpro->ppdata()->m_tinf.m_name, p_pokerpro->ppdata()->m_pinf[p_pokerpro->ppdata()->m_userchair].m_name);

			else
				_snprintf_s(title, _countof(title), _TRUNCATE, "%s - %s", p_pokerpro->ppdata()->m_site_name, p_pokerpro->ppdata()->m_tinf.m_name);
			messageTitle->Format("%s - %s (%s)", p_formula->formula_name(), p_tablemap->s$items()->sitename, title);
		}
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
				delete p_iterator_thread;
				p_iterator_thread = NULL;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////
		// Create replay frame

		if (prefs.replay_record())
		{
			if ( (p_symbols->sym()->ismyturn && !p_heartbeat_thread->replay_recorded_this_turn()) ||
				 (prefs.replay_record_every_change() && new_scrape != NOTHING_CHANGED) )
			{
				CReplayFrame	crf;
				crf.CreateReplayFrame();
				p_heartbeat_thread->set_replay_recorded_this_turn(true);
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////
		// Start/stop PokerTracker thread as needed

		// Start Poker Tracker Thread if needed and not already running

		if (p_symbols->sym()->issittingin && !prefs.pt_disable() && p_pokertracker_thread)
			p_pokertracker_thread->StartThread();

		// Stop Poker Tracker Thread if not needed any longer
		if (!p_symbols->sym()->issittingin && p_pokertracker_thread)
			p_pokertracker_thread->StopThread();

		////////////////////////////////////////////////////////////////////////////////////////////
		// Update scraper output dialog if it is present
		if (m_ScraperOutputDlg)
			m_ScraperOutputDlg->UpdateDisplay();

		// Measure cycle time
		QueryPerformanceCounter(&cycle_end);
		QueryPerformanceFrequency(&lFrequency);

		////////////////////////////////////////////////////////////////////////////////////////////
		// Save state
		p_game_state->CaptureState(title);

		////////////////////////////////////////////////////////////////////////////////////////////
		// Game state engine
		p_game_state->ProcessGameState(p_game_state->state((p_game_state->state_index()-1)&0xff));
		p_game_state->ProcessFtr(p_game_state->state((p_game_state->state_index()-1)&0xff));

		////////////////////////////////////////////////////////////////////////////////////////////
		// DLL - send state
		if ((p_scraper->card_player(p_symbols->sym()->userchair, 0) != CARD_NOCARD && 
			 p_scraper->card_player(p_symbols->sym()->userchair, 1) != CARD_NOCARD && 
			 p_symbols->user_chair_confirmed()) ||
			prefs.dll_always_send_state())
		{
			p_dll_extension->PassStateToDll(p_game_state->state((p_game_state->state_index()-1)&0xff));
		}

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

		// If autoplayer is engaged, we know our chair, and the DLL hasn't told us to wait, then go do it!
		if (p_autoplayer->autoplayer_engaged() && p_symbols->user_chair_confirmed() && !iswait)
		{
			if (!p_pokerpro->IsConnected())
				p_autoplayer->DoAutoplayer();

			else if (p_pokerpro->ppdata()->m_tinf.m_tid != 0)
				p_pokerpro->DoAutoplayer();
		}
		else
		{
			// Calc primary formulas anyway, so main window can display its information correctly
			// but set final_answer param to false, since we are not actually using this info to
			// take action
			p_symbols->CalcPrimaryFormulas(false);
		}

		////////////////////////////////////////////////////////////////////////////////////////////
		// scrape_delay, or wait for next ppro message
		if (p_pokerpro->IsConnected())
		{
			// If there is anything on the socket, process it
			FD_ZERO(&fd);
			FD_SET(p_pokerpro->socket(), &fd);

			tv.tv_usec = 50;
			tv.tv_sec = 0;

			result = 1;
			while (select(0, &fd, NULL, NULL, &tv)>0 && ::WaitForSingleObject(pParent->_m_stop_thread, 0)!=WAIT_OBJECT_0)
			{
				pbytes = NULL;
				nbytes = 0;
				result = p_pokerpro->ReceivePPMessage( &pbytes, &nbytes );
				if (result == 0)
				{
					// Trigger thread to stop
					::SetEvent(pParent->_m_stop_thread);
				}
				else
				{
					p_pokerpro->HandlePPMessage(pbytes, nbytes);
				}
			}
		}
		Sleep(prefs.scrape_delay());
	}
}
