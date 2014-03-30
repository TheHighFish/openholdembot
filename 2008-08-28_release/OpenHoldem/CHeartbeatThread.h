#ifndef INC_CHEARTBEATTHREAD_H
#define INC_CHEARTBEATTHREAD_H
	
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

private:
	// private functions and variables - not available via accessors or mutators
	static UINT HeartbeatThreadFunction(LPVOID pParam);

	HANDLE		_m_stop_thread;
	HANDLE		_m_wait_thread;

} *p_heartbeat_thread;

#endif //INC_CHEARTBEATTHREAD_H