#include "stdafx.h"
#include <process.h>

#include "debug.h"
#include "scraper.h"
#include "global.h"
#include "GameState.h"
#include "PokerTrackerThread.h"
#include "symbols.h"
#include "OpenHoldem.h"
#include "autoplayer.h"
#include "DialogScraperOutput.h"
#include "PokerPro.h"
#include "HeartbeatThread.h"
#include "IteratorThread.h"

CRITICAL_SECTION	cs_heartbeat;

CHeartbeatThread	*p_heartbeat_thread = NULL;

CHeartbeatThread::CHeartbeatThread()
{
    __SEH_SET_EXCEPTION_HANDLER(MyUnHandledExceptionFilter);

    __SEH_HEADER

	// Create events
	m_StopThread = CreateEvent(0, TRUE, FALSE, 0);
	m_WaitThread = CreateEvent(0, TRUE, FALSE, 0);

	// Initialize variables
	heartbeat_cycle_time = 0;

	// Start thread
	AfxBeginThread(heartbeat_thread_function, this);

    __SEH_LOGFATAL("CHeartbeatThread::Constructor :\n");
}

CHeartbeatThread::~CHeartbeatThread()
{
    __SEH_HEADER

	// Trigger thread to stop
	::SetEvent(m_StopThread);

	// Wait until thread finished
	::WaitForSingleObject(m_WaitThread, INFINITE);

	// Close handles
	::CloseHandle(m_StopThread);
	::CloseHandle(m_WaitThread);

    __SEH_LOGFATAL("CHeartbeatThread::Destructor :\n");
}

UINT CHeartbeatThread::heartbeat_thread_function(LPVOID pParam)
{
    __SEH_SET_EXCEPTION_HANDLER(MyUnHandledExceptionFilter);

    __SEH_HEADER

	CHeartbeatThread *pParent = static_cast<CHeartbeatThread*>(pParam);

	LARGE_INTEGER		cycle_start, cycle_end, lFrequency;
    Autoplayer			ap;
    unsigned int		new_scrape;
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
		if(::WaitForSingleObject(pParent->m_StopThread, 0) == WAIT_OBJECT_0)
		{
			// Set event
			::SetEvent(pParent->m_WaitThread);
			AfxEndThread(0);
		}

        // Measure cycle time
        QueryPerformanceCounter(&cycle_start);

        ////////////////////////////////////////////////////////////////////////////////////////////
        // Scrape window
		EnterCriticalSection(&cs_scraper);
		scraper.scraper_update_in_progress = true;
		LeaveCriticalSection(&cs_scraper);

		if (!global.ppro_is_connected)
        {
            new_scrape = scraper.DoScrape();
            QueryPerformanceCounter(&scraper.clockshold);
        }
        else
        {
            if (ppro.data.m_tinf.m_tid != 0)
            {
                ppro.DoScrape();
            }
        }

		EnterCriticalSection(&cs_scraper);
		scraper.scraper_update_in_progress = false;
		LeaveCriticalSection(&cs_scraper);

		////////////////////////////////////////////////////////////////////////////////////////////
        // Set Title of window
		CString *messageTitle = new CString();
        if (!global.ppro_is_connected)
        {
            GetWindowText(global.attached_hwnd, title, 512);
			messageTitle->Format("%s - %s (%s)", global.formula_name, global.trans.map.sitename, title);
        }
        else
        {
            if (ppro.data.m_tinf.m_tid == 0)
                _snprintf_s(title, _countof(title), _TRUNCATE, "%s", ppro.data.m_site_name);

            else if (ppro.data.m_userchair!=-1)
                _snprintf_s(title, _countof(title), _TRUNCATE, "%s - %s - %s", ppro.data.m_site_name, ppro.data.m_tinf.m_name, ppro.data.m_pinf[ppro.data.m_userchair].m_name);

            else
                _snprintf_s(title, _countof(title), _TRUNCATE, "%s - %s", ppro.data.m_site_name, ppro.data.m_tinf.m_name);
			messageTitle->Format("%s - %s (%s)", global.formula_name, global.trans.map.sitename, title);
        }
		theApp.m_pMainWnd->PostMessage(WMA_SETWINDOWTEXT, 0, (LPARAM)messageTitle);


        ////////////////////////////////////////////////////////////////////////////////////////////
        // Caclulate symbols

        // mark symbol result cache as stale
        N = (int) global.formula.mFunction.GetSize();
        for (i=0; i<N; i++)
        {
            global.formula.mFunction[i].fresh = false;
        }

		EnterCriticalSection(&cs_symbols);	
		symbols.symbols_update_in_progress = true;
		LeaveCriticalSection(&cs_symbols);

		if (new_scrape!=NOTHING_CHANGED || (global.ppro_is_connected && ppro.data.m_tinf.m_tid != 0))
        {
            symbols.CalcSymbols();
        }
        else
        {
            symbols.calc_time();
            symbols.calc_probabilities();
        }

		EnterCriticalSection(&cs_symbols);	
		symbols.symbols_update_in_progress = false;
		LeaveCriticalSection(&cs_symbols);

		////////////////////////////////////////////////////////////////////////////////////////////
        // Now that the scraper and symbols calc is done, save some variables from CSraper and CSymbols
		// for use below
        EnterCriticalSection(&cs_symbols);
		double	sym_issittingin = symbols.sym.issittingin;
		bool	user_chair_confirmed = symbols.user_chair_confirmed;
		int		sym_chair = (int) symbols.sym.chair;
		bool	sym_ismyturn = (bool) symbols.sym.ismyturn;
		LeaveCriticalSection(&cs_symbols);

        EnterCriticalSection(&cs_scraper);
		double	card_player0 = scraper.card_player[sym_chair][0];
		double	card_player1 = scraper.card_player[sym_chair][1];
		LeaveCriticalSection(&cs_scraper);


		////////////////////////////////////////////////////////////////////////////////////////////
        // If we've folded, stop iterator thread and set prwin/tie/los to zero
		if (user_chair_confirmed &&
			(card_player0==CARD_NOCARD || card_player0==CARD_BACK ||
			 card_player1==CARD_NOCARD || card_player1==CARD_BACK))
        {

			// stop iterator thread
			if (p_iterator_thread)
			{
				delete p_iterator_thread;
				write_log("Stopped iterator thread: %08x\n", p_iterator_thread);
				p_iterator_thread = NULL;
			}
			
			EnterCriticalSection(&cs_symbols);
            symbols.sym.prwin = 0;
            symbols.sym.prlos = 0;
            symbols.sym.prtie = 0;
			LeaveCriticalSection(&cs_symbols);

			EnterCriticalSection(&cs_iterator);
            iterator_status.iterator_thread_progress = 0;
            iterator_status.iterator_run_with_nit = 0;
            iterator_status.iterator_run_with_f$p = 0;
            iterator_status.iterator_run_with_br = 0;
            LeaveCriticalSection(&cs_iterator);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // Create replay frame
        if (global.preferences.replay_record)
        {
            if (sym_ismyturn && !global.replay_recorded_this_turn ||
                    global.preferences.replay_record_every_change && new_scrape != NOTHING_CHANGED)
            {
                global.create_replay_frame();
                global.replay_recorded_this_turn = true;
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // Start/stop PokerTracker thread as needed

		// Start Poker Tracker Thread if needed and not already running
        if (sym_issittingin && !p_pokertracker_thread && !PT.disable)
        {
            p_pokertracker_thread = new CPokerTrackerThread;
			write_log("Started poker tracker thread: %08x\n", p_pokertracker_thread);
        }

        // Stop Poker Tracker Thread if not needed any longer
        if (!sym_issittingin && p_pokertracker_thread)
        {
			delete p_pokertracker_thread;
			write_log("Stopped poker tracker thread: %08x\n", p_pokertracker_thread);
			p_pokertracker_thread = NULL;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // Update scraper output dialog if it is present
        if (m_ScraperOutputDlg)
            m_ScraperOutputDlg->do_update_display();

        // Measure cycle time
        QueryPerformanceCounter(&cycle_end);
        QueryPerformanceFrequency(&lFrequency);

        EnterCriticalSection(&cs_heartbeat);
        p_heartbeat_thread->heartbeat_cycle_time = (double) (cycle_end.LowPart - cycle_start.LowPart)/(double) lFrequency.LowPart;
        LeaveCriticalSection(&cs_heartbeat);

        ////////////////////////////////////////////////////////////////////////////////////////////
        // Save state
        global.capture_state(title);

        ////////////////////////////////////////////////////////////////////////////////////////////
        // Game state engine
        game_state.process_game_state(&global.state[(global.state_index-1)&0xff]);
        game_state.process_ftr(&global.state[(global.state_index-1)&0xff]);

        ////////////////////////////////////////////////////////////////////////////////////////////
        // DLL - send state
        if ((card_player0 != CARD_NOCARD && card_player1 != CARD_NOCARD && user_chair_confirmed) ||
            global.preferences.dll_always_send_state)
        {
            cdll.pass_state_to_dll(&global.state[(global.state_index-1)&0xff]);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // Autoplayer
        if (cdll.hMod_dll!=NULL && sym_ismyturn)
        {
            iswait = (cdll.process_message) ("query", "dll$iswait");
        }
        else
        {
            iswait = false;
        }

        if (user_chair_confirmed && global.autoplay && !iswait)
        {
            if (!global.ppro_is_connected)
                ap.do_autoplayer();

            else if (ppro.data.m_tinf.m_tid != 0)
                ppro.DoAutoplayer();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // scrape_delay, or wait for next ppro message
        if (global.ppro_is_connected)
        {
            // If there is anything on the socket, process it
            FD_ZERO(&fd);
            FD_SET(ppro.m_socket, &fd);
            tv.tv_usec = 50;
            tv.tv_sec = 0;

            result = 1;
            while (select(0, &fd, NULL, NULL, &tv)>0 && ::WaitForSingleObject(pParent->m_StopThread, 0)!=WAIT_OBJECT_0)
            {
                pbytes = NULL;
                nbytes = 0;
                result = ppro.recv_message( &pbytes, &nbytes );
                if (result == 0)
                {
					// Trigger thread to stop
					::SetEvent(pParent->m_StopThread);
                }
                else
                {
                    ppro.handle_message(pbytes, nbytes);
                }
            }
        }
        global.ppro_isppro = ppro.m_socket!=INVALID_SOCKET ? 1 : 0;
        global.ppro_tid = ppro.data.m_tinf.m_tid;

        Sleep(global.preferences.scrape_delay);
    }

    __SEH_LOGFATAL("CHeartbeatThread::heartbeat_thread_function :\n");
}
