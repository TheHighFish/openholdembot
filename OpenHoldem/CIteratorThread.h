#ifndef INC_CITERATORTHREAD_H
#define INC_CITERATORTHREAD_H

#include "inlines/eval.h"
#include "..\CCritSec\CCritSec.h"

// Global class to hold variables that are used by the CIteratorThread class but need to
// persist between instantiations of the CIteratorThread class
extern class CIteratorVars
{
public:
	// public functions
	CIteratorVars();
	~CIteratorVars();
	void ResetVars();

public:
	// public accessors
	const double		prwin() { return _prwin; }
	const double		prtie() { return _prtie; }
	const double		prlos() { return _prlos; }
	const bool			iterator_thread_running() { return _iterator_thread_running; }
	const bool			iterator_thread_complete() { return _iterator_thread_complete; }
	const unsigned int	iterator_thread_progress() { return _iterator_thread_progress; }
	const unsigned int	nit() { return _nit; }
	const unsigned int	f$p() { return _f$p; }
	const unsigned int	br() { return _br; }
	const unsigned int	pcard(const int i) { if (i>=0 && i<=1) return _pcard[i]; else return CARD_NOCARD; }
	const unsigned int	ccard(const int i) { if (i>=0 && i<=4) return _ccard[i]; else return CARD_NOCARD; }

public:
#define ENT CSLock lock(m_critsec);
	// public mutators 
	void set_prwin(const double d) { ENT _prwin = d; }
	void set_prtie(const double d) { ENT _prtie = d; }
	void set_prlos(const double d) { ENT _prlos = d; }
	void set_iterator_thread_running(const bool b) { ENT _iterator_thread_running = b; }
	void set_iterator_thread_complete(const bool b) { ENT _iterator_thread_complete = b; }
	void set_iterator_thread_progress(const unsigned int i) { ENT _iterator_thread_progress = i; }
	void set_nit(const unsigned int i) { ENT _nit = i; }
	void set_f$p(const unsigned int i) { ENT _f$p = i; }
	void set_br(const unsigned int i) { ENT _br = i; }
	void set_pcard(const unsigned int i, const unsigned int c) { ENT if (i>=0 && i<=1) _pcard[i] = c; }
	void set_ccard(const unsigned int i, const unsigned int c) { ENT if (i>=0 && i<=4) _ccard[i] = c; }
#undef ENT

private:
	// private variables - use public accessors and public mutators to address these
	unsigned int	_nit;
	unsigned int	_f$p;
	unsigned int	_br;
	unsigned int	_pcard[2];
	unsigned int	_ccard[5];
	double			_prwin, _prtie, _prlos;
	bool			_iterator_thread_running;
	bool			_iterator_thread_complete;
	unsigned int	_iterator_thread_progress;

private:
	// private functions and variables - not available via accessors or mutators
	CCritSec	m_critsec;

} iter_vars;


extern class CIteratorThread
{
public:
	// public functions
	CIteratorThread();
	~CIteratorThread();

private:
	// private functions and variables - not available via accessors or mutators
	static UINT IteratorThreadFunction(LPVOID pParam);
	int InRange(const int card1, const int card2, const int willplay, 
				const int wontplay, const int topclip, const int mustplay);
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
