//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose:
//
//*******************************************************************************

#include "StdAfx.h"
#include "CMyMutex.h"
#include "CPreferences.h"

CMyMutex::CMyMutex() : _mutex(false, preferences.mutex_name()) {
  // We want a long timeout to let OHs instances act in FIFO order.
	// But we can't wait forever because events can happen. Popups, table timeout... 
  // FIFO order is actually not guaranteed, but seems to work bz chance in Win XP:
  // http://www.maxinmontreal.com/forums/viewtopic.php?f=114&t=18860
	if (_mutex.Lock(5000)) {
	  _locked = TRUE;
  }	else {
	  _locked = FALSE;
  }
}


CMyMutex::~CMyMutex(void) {
	if (_locked == TRUE) {                                                                                                                                                                                                                                                                                                                                      
		_mutex.Unlock();
  }
}

bool CMyMutex::IsLocked() {
	return _locked == TRUE;
}
