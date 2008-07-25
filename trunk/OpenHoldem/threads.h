

extern	CWinThread			*h_heartbeat_thread;
UINT __cdecl heartbeat_thread(LPVOID pParam);

extern	CWinThread			*h_prwin_thread;
UINT __cdecl prwin_thread(LPVOID pParam);

extern	bool				heartbeat_thread_alive;
extern double				heartbeat_cycle_time;

extern	bool				prwin_thread_alive;
extern	bool				prwin_thread_complete;
extern	unsigned int		prwin_iterator_progress;
extern	double				prwin_run_with_nit;
extern	double				prwin_run_with_f$p;
extern	double				prwin_run_with_br;
extern	unsigned int		prwin_run_with_playercards[2];
extern	unsigned int		prwin_run_with_commoncards[5];

extern CRITICAL_SECTION		cs_heartbeat;
extern CRITICAL_SECTION		cs_prwin;
extern CRITICAL_SECTION		cs_updater;
extern CRITICAL_SECTION		cs_calc_f$symbol;
extern CRITICAL_SECTION		cs_parse;


