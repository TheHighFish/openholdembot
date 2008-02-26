
void do_autoplayer(void);

void heartbeat_thread(void *);
extern	HANDLE				h_heartbeat_thread;
extern	bool				heartbeat_thread_alive;
extern CRITICAL_SECTION		cs_heartbeat;
extern double				heartbeat_cycle_time;

void prwin_thread(void *);
extern	HANDLE				h_prwin_thread;
extern	bool				prwin_thread_alive;
extern	bool				prwin_thread_complete;
extern CRITICAL_SECTION		cs_prwin;
extern	unsigned int		prwin_iterator_progress;
extern	double				prwin_run_with_nit;
extern	double				prwin_run_with_f$p;
extern	double				prwin_run_with_br;
extern	unsigned int		prwin_run_with_playercards[2];
extern	unsigned int		prwin_run_with_commoncards[5];

extern CRITICAL_SECTION		cs_updater;
extern CRITICAL_SECTION		cs_calc_f$symbol;

