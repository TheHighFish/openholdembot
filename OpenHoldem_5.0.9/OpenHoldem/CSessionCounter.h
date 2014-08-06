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

#ifndef INC_CSESSIONCOUNTER_H
#define INC_CSESSIONCOUNTER_H

#include "MagicNumbers.h"

class CSessionCounter
{
public:
	// public functions
	CSessionCounter();
	~CSessionCounter();

public:
	// public accessors
	// session_id() returns a value in the range 0..(MAX_SESSION_IDS - 1)
	const unsigned int session_id() { return _session_id; }

private:
	// private variables - use public accessors and public mutators to address these
	unsigned int _session_id;

private:
	// private functions and variables - not available via accessors or mutators		
	HANDLE hMutex;

};

extern CSessionCounter *p_sessioncounter;

#endif //INC_CSESSIONCOUNTER_H