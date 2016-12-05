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

#ifndef INC_CSYMBOLENGINEREPLAYFRAMECONTROLLER_H
#define INC_CSYMBOLENGINEREPLAYFRAMECONTROLLER_H

#include "CVirtualSymbolEngine.h"

class CSymbolEngineReplayFrameController: public CVirtualSymbolEngine {
 public:
	CSymbolEngineReplayFrameController();
	~CSymbolEngineReplayFrameController();
 public:
	// Mandatory reset-functions
	void InitOnStartup();
	void ResetOnConnection();
	void ResetOnHandreset();
	void ResetOnNewRound();
	void ResetOnMyTurn();
	void ResetOnHeartbeat();
 public:
	// Public accessors
	void ShootReplayFrameIfNotYetDone();
 private:
	bool __replay_recored_this_turn;
private:
	void set_replay_recored_this_turn(bool p_b);
	bool is_replay_recored_this_turn() const;
};

extern CSymbolEngineReplayFrameController *p_symbol_engine_replayframe_controller;

#endif INC_CSYMBOLENGINEREPLAYFRAMECONTROLLER_H
