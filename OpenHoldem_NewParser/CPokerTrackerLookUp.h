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

#ifndef INC_CPOKERTRACKERLOOKUP_H
#define INC_CPOKERTRACKERLOOKUP_H

#include <map>

class CPokerTrackerLookup
{
public:
	// public functions
	CPokerTrackerLookup();
	~CPokerTrackerLookup();
	const int GetSiteId();

private:
	// private functions and variables - not available via accessors or mutators
	std::map<CString, int>	_pt3_siteid;
};

extern CPokerTrackerLookup pt_lookup;

#endif INC_CPOKERTRACKERLOOKUP_H