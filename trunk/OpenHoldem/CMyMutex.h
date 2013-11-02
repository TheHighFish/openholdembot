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

#ifndef INC_MYMUTEX_H
#define INC_MYMUTEX_H

// Wrapper around CMutex objects to provide for automatic
// acquire and release of the mutex.


class CMyMutex : public CMutex
{
public:
	CMyMutex();
	~CMyMutex();
	bool IsLocked();

private:
	CMutex _mutex;
	bool _locked;
};

#endif INC_MYMUTEX_H
