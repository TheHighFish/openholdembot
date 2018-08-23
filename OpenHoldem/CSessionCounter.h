#pragma once
//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#include "CSpaceOptimizedGlobalObject.h"

class CSessionCounter {
 public:
	// public functions
	CSessionCounter();
	~CSessionCounter();
 public:
	// public accessors
	// session_id() returns a value in the range 0..(MAX_SESSION_IDS - 1)
	int session_id() { return _session_id; }
 private:
	// private variables - use public accessors and public mutators to address these
	int _session_id;
 private:
	// private functions and variables - not available via accessors or mutators		
	HANDLE hMutex;
};

extern CSessionCounter *p_sessioncounter;
