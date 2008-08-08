
extern class CIteratorThread
{
	public:
		CIteratorThread();
		~CIteratorThread();

		HANDLE		m_StopThread;
		HANDLE		m_WaitThread;
		HANDLE		m_NormalExitThread;

	private:
		static UINT iterator_thread_function(LPVOID pParam);
		static int inrange(int card1, int card2);

} *p_iterator_thread;

extern struct SIteratorStatus
{
	bool				iterator_thread_running;
	bool				iterator_thread_complete;
	unsigned int		iterator_thread_progress;
	double				iterator_run_with_nit;
	double				iterator_run_with_f$p;
	double				iterator_run_with_br;
	unsigned int		iterator_run_with_playercards[2];
	unsigned int		iterator_run_with_commoncards[5];
} iterator_status;

// This critical section controls access to any variables in the CIteratorThread class or the related SIteratorStatus global structure
// Any code needing access to these variables must grab and release this critical section appropriately
extern CRITICAL_SECTION		cs_iterator;


