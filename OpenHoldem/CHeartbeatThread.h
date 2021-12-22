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

#ifndef INC_CHEARTBEATTHREAD_H
#define INC_CHEARTBEATTHREAD_H

#include "CHeartbeatDelay.h"
#include "COpenHoldemStarter.h"
#include "CSpaceOptimizedGlobalObject.h"
#include "CWatchdog.h"

class CHeartbeatThread /*: public CSpaceOptimizedGlobalObject */{
 public:
	// public functions
	CHeartbeatThread();
	~CHeartbeatThread();
 public:
	// public accessors
	void StartThread();
	long int heartbeat_counter() { return _heartbeat_counter; }
 public:
	// This critical section does not control access to any variables/members, but is used as 
	// a flag to indicate when the scraper/symbol classes are in an update cycle
	static CRITICAL_SECTION	cs_update_in_progress;
 private:
	// private functions and variables - not available via accessors or mutators
	static UINT HeartbeatThreadFunction(LPVOID pParam);
	static void ScrapeEvaluateAct();
	static void AutoConnect();
 private:
	// private variables - use public accessors and public mutators to address these	
	CCritSec	m_critsec;
	static		CHeartbeatThread *pParent;
	static		long int _heartbeat_counter;
 private:
  static CHeartbeatDelay _heartbeat_delay;
  static COpenHoldemStarter _openholdem_starter;
 private:
	HANDLE		_m_stop_thread;
	HANDLE		_m_wait_thread;
};

extern CHeartbeatThread *p_heartbeat_thread;

#endif //INC_CHEARTBEATTHREAD_H