#include "stdafx.h"
#include <process.h>

#include "OpenHoldem.h"

#include "CScraper.h"
#include "CSymbols.h"
#include "CAutoplayer.h"
#include "CIteratorThread.h"
#include "CHeartbeatThread.h"
#include "CPokerTrackerThread.h"
#include "CGlobal.h"

#include "GameState.h"
#include "DialogScraperOutput.h"
#include "PokerPro.h"

CHeartbeatThread	*p_heartbeat_thread = NULL;
CRITICAL_SECTION	CHeartbeatThread::cs_update_in_progress;

CHeartbeatThread::CHeartbeatThread()
{
	__SEH_SET_EXCEPTION_HANDLER

	__SEH_HEADER

	InitializeCriticalSectionAndSpinCount(&cs_update_in_progress, 4000);

	// Create events
	_m_stop_thread = CreateEvent(0, TRUE, FALSE, 0);
	_m_wait_thread = CreateEvent(0, TRUE, FALSE, 0);

	// Start thread
	AfxBeginThread(HeartbeatThreadFunction, this);

	__SEH_LOGFATAL("CHeartbeatThread::Constructor :\n");
}

CHeartbeatThread::~CHeartbeatThread()
{
	__SEH_HEADER

	// Trigger thread to stop
	::SetEvent(_m_stop_thread);

	// Wait until thread finished
	::WaitForSingleObject(_m_wait_thread, INFINITE);

	// Close handles
	::CloseHandle(_m_stop_thread);
	::CloseHandle(_m_wait_thread);

	DeleteCriticalSection(&cs_update_in_progress);

	p_heartbeat_thread = NULL;

	__SEH_LOGFATAL("CHeartbeatThread::Destructor :\n");
}

UINT CHeartbeatThread::HeartbeatThreadFunction(LPVOID pParam)
{
	__SEH_SET_EXCEPTION_HANDLER

	__SEH_HEADER

	CHeartbeatThread *pParent = static_cast<CHeartbeatThread*>(pParam);

	LARGE_INTEGER		cycle_start, cycle_end, lFrequency;
	unsigned int		new_scrape = NOTHING_CHANGED;
	bool				iswait;
	char				title[512];
	int					N, i;

	// PokerPro valiables only
	const char			*pbytes;
	int					nbytes, result;
	fd_set				fd;
	timeval				tv;

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

			if (!p_global->ppro_is_connected)
			{
				new_scrape = p_scraper->DoScrape();

				LARGE_INTEGER PerformanceCount;
				QueryPerformanceCounter(&PerformanceCount);
				p_scraper->set_clocks_hold(PerformanceCount);
			}
			else
			{
				if (ppro.data.m_tinf.m_tid != 0)
				{
					ppro.DoScrape();
				}
			}

			////////////////////////////////////////////////////////////////////////////////////////////
			// Caclulate symbols

			// mark symbol result cache as stale
			N = (int) p_global->formula.mFunction.GetSize();
			for (i=0; i<N; i++)
			{
				p_global->formula.mFunction[i].fresh = false;
			}

			if (new_scrape!=NOTHING_CHANGED || (p_global->ppro_is_connected && ppro.data.m_tinf.m_tid != 0))
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
		if (!p_global->ppro_is_connected)
		{
			GetWindowText(p_global->attached_hwnd(), title, 512);
			messageTitle->Format("%s - %s (%s)", p_global->formula_name, p_global->trans.map.sitename, title);
		}
		else
		{
			if (ppro.data.m_tinf.m_tid == 0)
				_snprintf_s(title, _countof(title), _TRUNCATE, "%s", ppro.data.m_site_name);

			else if (ppro.data.m_userchair!=-1)
				_snprintf_s(title, _countof(title), _TRUNCATE, "%s - %s - %s", ppro.data.m_site_name, ppro.data.m_tinf.m_name, ppro.data.m_pinf[ppro.data.m_userchair].m_name);

			else
				_snprintf_s(title, _countof(title), _TRUNCATE, "%s - %s", ppro.data.m_site_name, ppro.data.m_tinf.m_name);
			messageTitle->Format("%s - %s (%s)", p_global->formula_name, p_global->trans.map.sitename, title);
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

		if (p_global->preferences.replay_record)
		{
			if ( (p_symbols->sym()->ismyturn && !p_global->replay_recorded_this_turn()) ||
				 (p_global->preferences.replay_record_every_change && new_scrape != NOTHING_CHANGED) )
			{
				p_global->CreateReplayFrame();
				p_global->set_replay_recorded_this_turn(true);
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////
		// Start/stop PokerTracker thread as needed

		// Start Poker Tracker Thread if needed and not already running
		if (p_symbols->sym()->issittingin && !p_global->preferences.pt_prefs.disable)
			if (!p_pokertracker_thread)  
				p_pokertracker_thread = new CPokerTrackerThread;

		// Stop Poker Tracker Thread if not needed any longer
		if (!p_symbols->sym()->issittingin)
			if (p_pokertracker_thread)  
			{
				delete p_pokertracker_thread;
				p_pokertracker_thread = NULL;
			}

		////////////////////////////////////////////////////////////////////////////////////////////
		// Update scraper output dialog if it is present
		if (m_ScraperOutputDlg)
			m_ScraperOutputDlg->do_update_display();

		// Measure cycle time
		QueryPerformanceCounter(&cycle_end);
		QueryPerformanceFrequency(&lFrequency);

		////////////////////////////////////////////////////////////////////////////////////////////
		// Save state
		p_global->CaptureState(title);

		////////////////////////////////////////////////////////////////////////////////////////////
		// Game state engine
		game_state.process_game_state(&p_global->state[(p_global->state_index-1)&0xff]);
		game_state.process_ftr(&p_global->state[(p_global->state_index-1)&0xff]);

		////////////////////////////////////////////////////////////////////////////////////////////
		// DLL - send state
		if ((p_scraper->card_player(p_symbols->sym()->userchair, 0) != CARD_NOCARD && 
			 p_scraper->card_player(p_symbols->sym()->userchair, 1) != CARD_NOCARD && 
			 p_symbols->user_chair_confirmed()) ||
			p_global->preferences.dll_always_send_state)
		{
			cdll.pass_state_to_dll(&p_global->state[(p_global->state_index-1)&0xff]);
		}

		////////////////////////////////////////////////////////////////////////////////////////////
		// Autoplayer
		if (cdll.hMod_dll!=NULL && p_symbols->sym()->ismyturn)
		{
			iswait = (cdll.process_message) ("query", "dll$iswait");
		}
		else
		{
			iswait = false;
		}

		// If autoplayer is engaged, we know our chair, and the DLL hasn't told us to wait, then go do it!
		if (p_global->autoplay && p_symbols->user_chair_confirmed() && !iswait)
		{
			if (!p_global->ppro_is_connected)
				p_autoplayer->DoAutoplayer();

			else if (ppro.data.m_tinf.m_tid != 0)
				ppro.DoAutoplayer();
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
		if (p_global->ppro_is_connected)
		{
			// If there is anything on the socket, process it
			FD_ZERO(&fd);
			FD_SET(ppro.m_socket, &fd);
			tv.tv_usec = 50;
			tv.tv_sec = 0;

			result = 1;
			while (select(0, &fd, NULL, NULL, &tv)>0 && ::WaitForSingleObject(pParent->_m_stop_thread, 0)!=WAIT_OBJECT_0)
			{
				pbytes = NULL;
				nbytes = 0;
				result = ppro.recv_message( &pbytes, &nbytes );
				if (result == 0)
				{
					// Trigger thread to stop
					::SetEvent(pParent->_m_stop_thread);
				}
				else
				{
					ppro.handle_message(pbytes, nbytes);
				}
			}
		}
		p_global->ppro_isppro = ppro.m_socket!=INVALID_SOCKET ? 1 : 0;
		p_global->ppro_tid = ppro.data.m_tinf.m_tid;

		Sleep(p_global->preferences.scrape_delay);
	}

	__SEH_LOGFATAL("CHeartbeatThread::HeartbeatThreadFunction :\n");
}
