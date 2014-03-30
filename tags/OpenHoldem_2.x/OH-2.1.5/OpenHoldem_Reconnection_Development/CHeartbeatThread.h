#ifndef INC_CHEARTBEATTHREAD_H
#define INC_CHEARTBEATTHREAD_H

#include "..\CCritSec\CCritSec.h"

extern class CHeartbeatThread
{
public:
	// This critical section does not control access to any variables/members, but is used as 
	// a flag to indicate when the scraper/symbol classes are in an update cycle
	static CRITICAL_SECTION	cs_update_in_progress;

public:
	// public functions
		CHeartbeatThread();
		~CHeartbeatThread();

public:
	// public accessors
	const bool				replay_recorded_this_turn() { return _replay_recorded_this_turn; }

public:
#define ENT CSLock lock(m_critsec);
	// public mutators 
	void set_replay_recorded_this_turn(const bool b) { ENT _replay_recorded_this_turn = b; }
#undef ENT

private:
	// private variables - use public accessors and public mutators to address these	
	bool		_replay_recorded_this_turn;

private:
	// private functions and variables - not available via accessors or mutators
	static UINT HeartbeatThreadFunction(LPVOID pParam);

	HANDLE		_m_stop_thread;
	HANDLE		_m_wait_thread;

	CCritSec	m_critsec;

} *p_heartbeat_thread;

#endif //INC_CHEARTBEATTHREAD_H