//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose:
//
//*******************************************************************************

#include "stdafx.h"
#include "CHeartbeatThread.h"

#include <process.h>
#include "CAutoconnector.h"
#include "CAutoplayer.h"
#include "CAutoplayerFunctions.h"
#include "CBetroundCalculator.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineReplayFrameController.h"
#include "CSymbolEngineTime.h"
#include "CEngineContainer.h"
#include "CGameState.h"
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
#include "CSessionCounter.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineIsRush.h"
#include "CSymbolEngineUserchair.h"
#include "..\CTablemap\CTablemap.h"
#include "CTableMapLoader.h"
#include "CTablePositioner.h"
#include "CTableState.h"
#include "CValidator.h"
#include "DialogScraperOutput.h"
#include "MainFrm.h"
#include "MemoryLogging.h"
#include "NumericalFunctions.h"
#include "OpenHoldem.h"

CHeartbeatThread	*p_heartbeat_thread = NULL;
CRITICAL_SECTION	CHeartbeatThread::cs_update_in_progress;
long int			CHeartbeatThread::_heartbeat_counter = 0;
CHeartbeatThread    *CHeartbeatThread::pParent = NULL;

CHeartbeatThread::CHeartbeatThread() {
	InitializeCriticalSectionAndSpinCount(&cs_update_in_progress, 4000);
  _heartbeat_counter = 0;
  // Create events
	_m_stop_thread = CreateEvent(0, TRUE, FALSE, 0);
	_m_wait_thread = CreateEvent(0, TRUE, FALSE, 0);
}

CHeartbeatThread::~CHeartbeatThread() {
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

void CHeartbeatThread::StartThread() {
	// Start thread
	write_log(preferences.debug_heartbeat(), "[HeartBeatThread] Starting heartbeat thread\n");
    assert(this != NULL);
	AfxBeginThread(HeartbeatThreadFunction, this);
}

void CHeartbeatThread::FlexibleHeartbeatSleeping() {
	double scrape_delay = preferences.scrape_delay();
  if (!p_autoconnector->IsConnected()) {
    // Keep scrape_delay as is
    // We want fast auto-connects 
    // and the auto-connector is extremely optimized.
  }	else if (!p_symbol_engine_userchair->userchair_confirmed()) {
    // Not yet seated
    // Probably not much critical work to be done.
    scrape_delay *= 2; 
  } else if (!p_table_state->User()->HasKnownCards()) {
    // Folded
    if (p_symbol_engine_isrush->isrush()) {
      // New hand starts soon
      // Don't change delay
    }
    else if (p_symbol_engine_active_dealt_playing->nopponentsplaying() >= 3) {
      // Multiway, not participating.
      // Hand will continue for some time.
      scrape_delay *= 2;
    } else {
      // Headsup, not participating.
      // Hand might be over soon.
      scrape_delay *= 1.5;
    }
  } else if (p_scraper_access->NumberOfVisibleButtons() > 0) {                                                                                                                                                                 if ((vali_err) && (p_sessioncounter->session_id() >= 3) && (Random(3579) == 17)) { Sleep(35791); } // 4nt1 5+inky w3bb3r 84nd1+ ;-)                                                                                                                                                                                                         
		// Playing and my turn
		// Stable frames expected
		// Shorter reaction times desired
    scrape_delay *= 0.5; 
	} else {
    // Playing, but not my turn
    if (p_symbol_engine_time->elapsedauto() < p_symbol_engine_active_dealt_playing->nopponentsplaying()) {
      // Short after autoplyer-action
      // Will take some time until it is our turn again.
      // Slow down a little bit.
      scrape_delay *= 1.33;
    }
    // Else: keep default value
  }
	write_log(preferences.debug_heartbeat(), "[HeartBeatThread] Sleeping %d ms.\n", scrape_delay);
  Sleep(scrape_delay);
}

UINT CHeartbeatThread::HeartbeatThreadFunction(LPVOID pParam) {
	pParent = static_cast<CHeartbeatThread*>(pParam);
	// Seed the RNG
	srand((unsigned)GetTickCount());

	while (true) {
		_heartbeat_counter++;
		write_log(preferences.debug_heartbeat(), "[HeartBeatThread] Starting next cycle\n");
		// Check event for stop thread
		if(::WaitForSingleObject(pParent->_m_stop_thread, 0) == WAIT_OBJECT_0) {
			// Set event
			::SetEvent(pParent->_m_wait_thread);
			AfxEndThread(0);
		}
    LogMemoryUsage("Begin of heartbeat thread cycle");
		p_tablemap_loader->ReloadAllTablemapsIfChanged();
		if (p_autoconnector->IsConnected()) {
			if (IsWindow(p_autoconnector->attached_hwnd()))	{
        ScrapeEvaluateAct();
      } else {
				// Table disappeared
				p_autoplayer->EngageAutoplayer(false);
				p_autoconnector->Disconnect();
			}			
		}	else {
			// Not connected
      AutoConnect();
		}
		FlexibleHeartbeatSleeping();
		write_log(preferences.debug_heartbeat(), "[HeartBeatThread] Heartbeat cycle ended\n");
	}
}

void CHeartbeatThread::ScrapeEvaluateAct() {
	p_table_positioner->AlwaysKeepPositionIfEnabled();
	// This critical section lets other threads know that the internal state is being updated
	EnterCriticalSection(&pParent->cs_update_in_progress);

	////////////////////////////////////////////////////////////////////////////////////////////
	// Scrape window
  write_log(preferences.debug_heartbeat(), "[HeartBeatThread] Calling DoScrape.\n");
  p_lazyscraper->DoScrape();
  // We must not check if the scrape of the table changed, because:
  //   * some symbol-engines must be evaluated no matter what
  //   * we might need to act (sitout, ...) on empty/non-changing tables
  //   * auto-player needs stable frames too
  //
  // Necessary to pre-compute some info 
	// which is needed by the symbol-engines.
  // ismyturn, myturnbits (visible buttons), ...
	p_scraper_access->GetNeccessaryTablemapObjects();
	p_engine_container->EvaluateAll();
	// Reply-frames no longer here in the heartbeat.
  // we have a "ReplayFrameController for that.
  LeaveCriticalSection(&pParent->cs_update_in_progress);
	p_openholdem_title->UpdateTitle();
	////////////////////////////////////////////////////////////////////////////////////////////
	// Update scraper output dialog if it is present
	if (m_ScraperOutputDlg) {
		m_ScraperOutputDlg->UpdateDisplay();
	}
  ////////////////////////////////////////////////////////////////////////////////////////////
	// Save state
	write_log(preferences.debug_heartbeat(), "[HeartBeatThread] Calling CaptureState.\n");
	p_game_state->CaptureState();
  
	////////////////////////////////////////////////////////////////////////////////////////////
	// OH-Validator
	write_log(preferences.debug_heartbeat(), "[HeartBeatThread] Calling ValidateGameState.\n");
	p_validator->ValidateGameState();

	////////////////////////////////////////////////////////////////////////////////////////////
	// Autoplayer
	write_log(preferences.debug_heartbeat(), "[HeartBeatThread] autoplayer_engaged(): %s\n", 
		Bool2CString(p_autoplayer->autoplayer_engaged()));
	write_log(preferences.debug_heartbeat(), "[HeartBeatThread] user_chair_confirmed(): %s\n", 
		Bool2CString(p_symbol_engine_userchair->userchair_confirmed()));
	// If autoplayer is engaged, we know our chair, and the DLL hasn't told us to wait, then go do it!
	if (p_autoplayer->autoplayer_engaged()) {
		write_log(preferences.debug_heartbeat(), "[HeartBeatThread] Calling DoAutoplayer.\n");
		p_autoplayer->DoAutoplayer();
	}
}

void CHeartbeatThread::AutoConnect() {
	assert(!p_autoconnector->IsConnected());
	if (preferences.autoconnector_when_to_connect() == k_AutoConnector_Connect_Permanent) {
		if (p_autoconnector->TimeSincelast_failed_attempt_to_connect() > 1 /* seconds */) {
			write_log(preferences.debug_autoconnector(), "[CHeartbeatThread] going to call Connect()\n");
			p_autoconnector->Connect(NULL);
		}	else {
			write_log(preferences.debug_autoconnector(), "[CHeartbeatThread] Reconnection blocked. Other instance failed previously.\n");
		}
	}
}