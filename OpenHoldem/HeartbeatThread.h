
extern class CHeartbeatThread
{
	public:
		CHeartbeatThread();
		~CHeartbeatThread();

		HANDLE		m_StopThread;
		HANDLE		m_WaitThread;
		double		heartbeat_cycle_time;

	private:
		static UINT heartbeat_thread_function(LPVOID pParam);

} *p_heartbeat_thread;

// This critical section controls access to any variables in the CHeartbeatThread class
// Any code needing access to these variables must grab and release this critical section appropriately
extern CRITICAL_SECTION		cs_heartbeat;


