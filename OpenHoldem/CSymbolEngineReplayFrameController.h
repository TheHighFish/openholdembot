//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
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
	void UpdateOnConnection();
	void UpdateOnHandreset();
	void UpdateOnNewRound();
	void UpdateOnMyTurn();
	void UpdateOnHeartbeat();
  void UpdateAfterAutoplayerAction(int autoplayer_action_code);
 public:
	// Public accessors
	void ShootReplayFrameIfNotYetDone();
 private:
  int _heartbeat_of_last_replay_frame;
};

extern CSymbolEngineReplayFrameController *p_symbol_engine_replayframe_controller;

#endif INC_CSYMBOLENGINEREPLAYFRAMECONTROLLER_H
