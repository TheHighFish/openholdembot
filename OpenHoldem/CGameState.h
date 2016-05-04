//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: This module provides:
//     * a series of 256 gamestates for the DLL
//   In the past it also was responsible for
//     * history and action symbols
//     * table statistics
//     * creating a summary for the log (hand-history) 
//   But after refactoring this module creates just gamestates
//   and nothing else.
//
//******************************************************************************

#ifndef INC_CGAMESTATE_H
#define INC_CGAMESTATE_H

#include "CDllExtension.h"
#include "..\CCritSec\CCritSec.h"
#include "MagicNumbers.h"

class CGameState {
 public:
	// public functions
	CGameState();
	~CGameState();
	void CaptureState();
 private:
	// private functions and variables - not available via accessors or mutators
	void DumpState(void);
private:
};

extern CGameState *p_game_state;

#endif //INC_CGAMESTATE_H