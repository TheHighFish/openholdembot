//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*****************************************************************************
//
// Purpose: Shooting replay-frames 
//   (at the right time and only once per turn).
//   Not really a symbol-engine, but easy to implement with this concept 
//
//*****************************************************************************

#include "stdafx.h"
#include "CSymbolEngineReplayFrameController.h"

#include <assert.h>
#include "CPreferences.h"
#include "CStableFramesCounter.h"
#include "CSymbolEngineAutoplayer.h"
#include "MagicNumbers.h"
#include "CReplayFrame.h"

CSymbolEngineReplayFrameController *p_symbol_engine_replayframe_controller = NULL;

CSymbolEngineReplayFrameController::CSymbolEngineReplayFrameController()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
}

CSymbolEngineReplayFrameController::~CSymbolEngineReplayFrameController()
{}

void CSymbolEngineReplayFrameController::InitOnStartup()
{
	ResetOnConnection();
}

void CSymbolEngineReplayFrameController::ResetOnConnection()
{
	_replay_recored_this_turn = false;
}

void CSymbolEngineReplayFrameController::ResetOnHandreset()
{}

void CSymbolEngineReplayFrameController::ResetOnNewRound()
{}

void CSymbolEngineReplayFrameController::ResetOnMyTurn()
{
	// If it's my turn, and we have enough stable frames
	if (preferences.replay_record() 			
		&& p_symbol_engine_autoplayer->ismyturn() 
		&& p_stableframescounter->NumberOfStableFrames() >= preferences.frame_delay())
	{
		ShootReplayFrameIfNotYetDone();
	}
}

void CSymbolEngineReplayFrameController::ResetOnHeartbeat()
{
	_replay_recored_this_turn = false;
}

void CSymbolEngineReplayFrameController::ShootReplayFrameIfNotYetDone()
{
	// Don't shoot replay-frames twice per heartbeat
	if (_replay_recored_this_turn)
	{
		write_log(preferences.debug_replayframes(), "[CSymbolEngineReplayFrameController] Not shooting a replay-frame, because we already shot one this heartbeat\n");
		return;
	}
	write_log(preferences.debug_replayframes(), "[CSymbolEngineReplayFrameController] Going to shooting a replay-frame\n");
	CReplayFrame crf;
	crf.CreateReplayFrame();
}

