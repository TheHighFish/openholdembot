//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#include "StdAfx.h"
#include "CMyMutex.h"
#include "CPreferences.h"


CMyMutex::CMyMutex() : _mutex(false, preferences.mutex_name())
{
	if (_mutex.Lock(500))
	  _locked = TRUE;
	else
	  _locked = FALSE;
}


CMyMutex::~CMyMutex(void) 
{
	if (_locked == TRUE)
		_mutex.Unlock();
}

bool 
CMyMutex::IsLocked()
{
	return _locked == TRUE;
}
