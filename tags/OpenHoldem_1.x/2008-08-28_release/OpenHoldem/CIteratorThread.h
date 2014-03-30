#ifndef INC_CITERATORTHREAD_H
#define INC_CITERATORTHREAD_H

#include "inlines/eval.h"

// Critical section used in shared variable writes
extern CRITICAL_SECTION	cs_iterator;

extern struct SIterParams
{
	double			nit;
	double			f$p;
	double			br;
	unsigned int	pcard[2];
	unsigned int	ccard[5];
} _iter_params;

extern struct SIterVars
{
	double			prwin, prtie, prlos;
	bool			iterator_thread_running;
	bool			iterator_thread_complete;
	unsigned int	iterator_thread_progress;
} _iter_vars;

extern class CIteratorThread
{
public:
	// public functions
	CIteratorThread();
	~CIteratorThread();

private:
	// private functions and variables - not available via accessors or mutators
	static UINT IteratorThreadFunction(LPVOID pParam);
	int InRange(int card1, int card2);
	void InitIteratorLoop(void);

	// variables for iterator loop
	CardMask		_plCards, _comCards;
	int				_nplCards, _ncomCards;
	double			_win, _tie, _los;
	int				_willplay, _wontplay, _mustplay, _topclip;

	HANDLE			_m_stop_thread;
	HANDLE			_m_wait_thread;

} *p_iterator_thread;


#endif //INC_CITERATORTHREAD_H
