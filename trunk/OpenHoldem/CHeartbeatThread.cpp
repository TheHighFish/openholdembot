//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose:
//
//***************************************************************************** 

#include "stdafx.h"
#include "CHeartbeatThread.h"

#include <process.h>
#include "CAutoconnector.h"
#include "CAutoplayer.h"
#include "CAutoplayerFunctions.h"
#include "CBetroundCalculator.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineReplayFrameController.h"
#include "CEngineContainer.h"
#include "CFormula.h"
#include "CGameState.h"
#include "CHandhistory.h"
#include "CHandresetDetector.h"
#include "CIteratorThread.h"
#include "CLazyScraper.h"
#include "CopenHoldemHopperCommunication.h"
#include "CopenHoldemStatusbar.h"
#include "CopenHoldemTitle.h"
#include "CPreferences.h"
#include "CReplayFrame.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CStableFramesCounter.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineUserchair.h"
#include "..\CTablemap\CTablemap.h"
#include "CTableMapLoader.h"
#include "CTablePositioner.h"
#include "CValidator.h"
#include "DialogScraperOutput.h"
#include "MainFrm.h"
#include "OpenHoldem.h"
#include "CHandHistory.h"

CHeartbeatThread	*p_heartbeat_thread = NULL;
CRITICAL_SECTION	CHeartbeatThread::cs_update_in_progress;
long int			CHeartbeatThread::_heartbeat_counter = 0;
CHeartbeatThread    *CHeartbeatThread::pParent = NULL;

CHeartbeatThread::CHeartbeatThread()
{
	InitializeCriticalSectionAndSpinCount(&cs_update_in_progress, 4000);

	_heartbeat_counter = 0;

	// Create events
	_m_stop_thread = CreateEvent(0, TRUE, FALSE, 0);
	_m_wait_thread = CreateEvent(0, TRUE, FALSE, 0);
}

CHeartbeatThread::~CHeartbeatThread()
{
	// Trigger thread to stop
	::SetEvent(_m_stop_thread);

	// Wait until thread finished
	::WaitForSingleObject(_m_wait_thread, k_max_time_to_wait_for_thread_to_shutdown);

	// Close handles
	::CloseHandle(_m_stop_thread);
	::CloseHandle(_m_wait_thread);

	DeleteCriticalSection(&cs_update_in_progress);

	p_heartbeat_thread = NULL;
}

void CHeartbeatThread::StartThread()
{
	// Start thread
	write_log(preferences.debug_heartbeat(), "[HeartBeatThread] Starting heartbeat thread\n");
	AfxBeginThread(HeartbeatThreadFunction, this);
}

void CHeartbeatThread::FlexibleHeartbeatSleeping()
{
	int scrape_delay = preferences.scrape_delay();
	if (p_scraper_access->UserHasCards() 
		&& (p_scraper_access->NumberOfVisibleButtons() > 0))
	{
		// My turn
		// Stable frames expected
		// Shorter reaction times desired
		scrape_delay /= 2;
	}
	write_log(preferences.debug_heartbeat(), "[HeartBeatThread] Sleeping %d ms.\n", scrape_delay);
	Sleep(scrape_delay);
}

UINT CHeartbeatThread::HeartbeatThreadFunction(LPVOID pParam)
{
	pParent = static_cast<CHeartbeatThread*>(pParam);
	// Seed the RNG
	srand((unsigned)GetTickCount());

	while (true)
	{
		_heartbeat_counter++;
		write_log(preferences.debug_heartbeat(), "[HeartBeatThread] Starting next cycle\n");
		// Check event for stop thread
		if(::WaitForSingleObject(pParent->_m_stop_thread, 0) == WAIT_OBJECT_0)
		{
			// Set event
			::SetEvent(pParent->_m_wait_thread);
			AfxEndThread(0);
		}
		p_tablemap_loader->ReloadAllTablemapsIfChanged();
		if (!p_autoconnector->IsConnected())
		{
			AutoConnect();			
		}
		else
		{
			ScrapeEvaluateAct();
		}
		FlexibleHeartbeatSleeping();

		

		FlexibleHeartbeatSleeping();
		write_log(preferences.debug_heartbeat(), "[HeartBeatThread] Heartbeat cycle ended\n");
	}
}

void CHeartbeatThread::ScrapeEvaluateAct()
{
	unsigned int		new_scrape = NOTHING_CHANGED; /// not necessary?
	bool				iswait = false;

	p_table_positioner->AlwaysKeepPositionIfEnabled();
	// This critical section lets other threads know that the internal state is being updated
	EnterCriticalSection(&pParent->cs_update_in_progress);

	////////////////////////////////////////////////////////////////////////////////////////////
	// Scrape window

	write_log(preferences.debug_heartbeat(), "[HeartBeatThread] Calling DoScrape.\n");
	new_scrape = !NOTHING_CHANGED;
	p_lazyscraper->DoScrape();

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
		// Shoot replay-frame if desired
		// a) on every change
		if (preferences.replay_record_every_change() 
			// b) on every change when in hand
			|| (preferences.replay_record_every_change_playing()
				&& p_scraper_access->UserHasCards()))
		{
			p_symbol_engine_replayframe_controller->ShootReplayFrameIfNotYetDone();
		}
	}

	LeaveCriticalSection(&pParent->cs_update_in_progress);
	p_openholdem_title->UpdateTitle();
	
	////////////////////////////////////////////////////////////////////////////////////////////
	// Update scraper output dialog if it is present
	if (m_ScraperOutputDlg)
	{
		m_ScraperOutputDlg->UpdateDisplay();
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	// Save state
	write_log(preferences.debug_heartbeat(), "[HeartBeatThread] Calling CaptureState.\n");
	p_game_state->CaptureState();

	////////////////////////////////////////////////////////////////////////////////////////////
	// Game state engine
	// TODO: create a symbol-engine
	write_log(preferences.debug_heartbeat(), "[HeartBeatThread] Calling ProcessGameState.\n");
	p_game_state->ProcessGameState(p_game_state->state((p_game_state->state_index()-1)&0xff));
	write_log(preferences.debug_heartbeat(), "[HeartBeatThread] Calling ProcessFtr.\n");
	p_game_state->ProcessFtr(p_game_state->state((p_game_state->state_index()-1)&0xff));

	////////////////////////////////////////////////////////////////////////////////////////////
	// OH-Validator
	write_log(preferences.debug_heartbeat(), "[HeartBeatThread] Calling ValidateGameState.\n");
	p_validator->ValidateGameState();

	////////////////////////////////////////////////////////////////////////////////////////////
	// DLL - always send state
	write_log(preferences.debug_heartbeat(), "[HeartBeatThread] Calling PassStateToDll.\n");
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

	write_log(preferences.debug_heartbeat(), "[HeartBeatThread] autoplayer_engaged(): %s\n", 
		Bool2CString(p_autoplayer->autoplayer_engaged()));
	write_log(preferences.debug_heartbeat(), "[HeartBeatThread] user_chair_confirmed(): %s\n", 
		Bool2CString(p_symbol_engine_userchair->userchair_confirmed()));
	write_log(preferences.debug_heartbeat(), "[HeartBeatThread] iswait: %s\n", 
		Bool2CString(iswait));
	// If autoplayer is engaged, we know our chair, and the DLL hasn't told us to wait, then go do it!
	if (p_autoplayer->autoplayer_engaged() && !iswait)
	{
		write_log(preferences.debug_heartbeat(), "[HeartBeatThread] Calling DoAutoplayer.\n");
		p_autoplayer->DoAutoplayer();
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////
	// Hand history generator

	if (preferences.handhistory_generator_enable())
	{
		p_handhistory->MakeHistory();
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	// Switching from basic to advanced statusbar
	// with detailed info instead of hint for beginners
	p_openholdem_statusbar->SwitchToAdvancedStatusbarAfterFirstHand();
}

void CHeartbeatThread::AutoConnect()
{
	assert(!p_autoconnector->IsConnected());
	if (preferences.autoconnector_when_to_connect() == k_AutoConnector_Connect_Permanent) 
	{
		if (p_autoconnector->TimeSincelast_failed_attempt_to_connect() > 1 /* seconds */)
		{
			write_log(preferences.debug_autoconnector(), "[CHeartbeatThread] going to call Connect()");
			p_autoconnector->Connect(NULL);
		}
		else
		{
			write_log(preferences.debug_autoconnector(), "[CHeartbeatThread] Reconnection blocked. Other instance failed previously.\n");
		}
	}
}