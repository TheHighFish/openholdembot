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

#ifndef INC_CHEARTBEATTHREAD_H
#define INC_CHEARTBEATTHREAD_H

#include "..\CCritSec\CCritSec.h"

class CLazyScraper;

class CHeartbeatThread {
 public:
	// public functions
	CHeartbeatThread();
	~CHeartbeatThread();
 public:
	// public accessors
	void StartThread();
	long int heartbeat_counter()    { return _heartbeat_counter; }
  CLazyScraper* p_lazyscraper()   { return _p_lazyscraper; }
 public:
	// This critical section does not control access to any variables/members, but is used as 
	// a flag to indicate when the scraper/symbol classes are in an update cycle
	static CRITICAL_SECTION	cs_update_in_progress;
 private:
	// private functions and variables - not available via accessors or mutators
	static UINT HeartbeatThreadFunction(LPVOID pParam);
	static void ScrapeEvaluateAct();
	static void AutoConnect();
	static void FlexibleHeartbeatSleeping();
 private:
	// private variables - use public accessors and public mutators to address these	
	CCritSec	m_critsec;
	static		CHeartbeatThread *pParent;
	static		long int _heartbeat_counter;
 private:
	HANDLE		_m_stop_thread;
	HANDLE		_m_wait_thread;
 private:
  /*static*/ CLazyScraper *_p_lazyscraper;
};

extern CHeartbeatThread *p_heartbeat_thread;

#define HBT p_heartbeat_thread

#endif //INC_CHEARTBEATTHREAD_H