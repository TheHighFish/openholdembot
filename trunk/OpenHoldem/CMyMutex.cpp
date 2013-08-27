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
