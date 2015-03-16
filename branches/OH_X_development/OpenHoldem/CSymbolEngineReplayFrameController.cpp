//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Shooting replay-frames 
//   (at the right time and only once per turn).
//   Not really a symbol-engine, but easy to implement with this concept 
//
//******************************************************************************

#include "stdafx.h"
#include "CSymbolEngineReplayFrameController.h"

#include <assert.h>
#include "CAutoPlayer.h"
#include "CHeartbeatThread.h"
#include "CLazyScraper.h"
#include "CPreferences.h"
#include "CReplayFrame.h"
#include "CStableFramesCounter.h"
#include "CSymbolEngineAutoplayer.h"
#include "CTableState.h"
#include "MagicNumbers.h"

CSymbolEngineReplayFrameController *p_symbol_engine_replayframe_controller = NULL;

CSymbolEngineReplayFrameController::CSymbolEngineReplayFrameController() {
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
  assert(SYM->p_symbol_engine_autoplayer() != NULL);
}

CSymbolEngineReplayFrameController::~CSymbolEngineReplayFrameController() {
}

void CSymbolEngineReplayFrameController::InitOnStartup() {
	ResetOnConnection();
}

void CSymbolEngineReplayFrameController::ResetOnConnection() {
	_replay_recored_this_turn = false;
}

void CSymbolEngineReplayFrameController::ResetOnHandreset() {
}

void CSymbolEngineReplayFrameController::ResetOnNewRound() {
}

void CSymbolEngineReplayFrameController::ResetOnMyTurn() {
}

void CSymbolEngineReplayFrameController::ResetOnHeartbeat() {
  if (HBT->p_lazyscraper()->IsIdenticalScrape()) {
    // There is no benefit in duplicate frames, so we abort
    write_log(theApp.p_preferences()->debug_replayframes(), "[CSymbolEngineReplayFrameController] No replay required, as identical scrape\n");
    return;
  }
  if ((theApp.p_preferences()->replay_record() == kShootReplyFramesOnEveryChangeWhilePlaying)	
      && p_table_state->User()->HasKnownCards()) {
        write_log(theApp.p_preferences()->debug_replayframes(), "[CSymbolEngineReplayFrameController] Replay required (on change while in hand)\n");
    ShootReplayFrameIfNotYetDone();
    return;
  }
  if (theApp.p_preferences()->replay_record() == kShootReplyFramesOnEveryChange) {
    write_log(theApp.p_preferences()->debug_replayframes(), "[CSymbolEngineReplayFrameController] Replay required (on every change in table-state)\n");
    ShootReplayFrameIfNotYetDone();
    return;
  }
	if ((theApp.p_preferences()->replay_record() == kShootReplyFramesOnMyTurn)			
		  && SYM->p_symbol_engine_autoplayer()->ismyturn() 
      && p_autoplayer->autoplayer_engaged()
		  && p_stableframescounter->NumberOfStableFrames() == theApp.p_preferences()->frame_delay()) {
    // If it's my turn and we have enough stable frames
    // then we will usually act and shoot a replay-frame on this heartbeat.
    // Shooting exactly once in case of no action is ensured by
    // "p_stableframescounter->NumberOfStableFrames() == theApp.p_preferences()->frame_delay()".
    // As NumberOfStableFrames() updates OnHeartbeat we have to put that code here
    // and can~t make use of OnMyTurn();
    write_log(theApp.p_preferences()->debug_replayframes(), "[CSymbolEngineReplayFrameController] Replay required (on my turn and time to act)\n");
		ShootReplayFrameIfNotYetDone();
    return;
	}
  // ResetOnHeartbeat() is the last function to be called,
  // whereas the first one depends on circumstances.
  // therefore we reset _replay_recored_this_turn here at the very end. 
	_replay_recored_this_turn = false;
}

void CSymbolEngineReplayFrameController::ShootReplayFrameIfNotYetDone() {
	// Don't shoot replay-frames twice per heartbeat
	if (_replay_recored_this_turn)	{
		write_log(theApp.p_preferences()->debug_replayframes(), "[CSymbolEngineReplayFrameController] Not shooting a replay-frame, because we already shot one this heartbeat\n");
		return;
	}
	write_log(theApp.p_preferences()->debug_replayframes(), "[CSymbolEngineReplayFrameController] Going to shooting a replay-frame\n");
	CReplayFrame crf;
	crf.CreateReplayFrame();
}