
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

extern	bool				iterator_thread_running;
extern	bool				iterator_thread_complete;
extern	unsigned int		iterator_thread_progress;
extern	double				iterator_run_with_nit;
extern	double				iterator_run_with_f$p;
extern	double				iterator_run_with_br;
extern	unsigned int		iterator_run_with_playercards[2];
extern	unsigned int		iterator_run_with_commoncards[5];

// This critical section controls access to any variables in the CIteratorThread class or the related global variables
// Any code needing access to these variables must grab and release this critical section appropriately
extern CRITICAL_SECTION		cs_iterator;


