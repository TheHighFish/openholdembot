//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*****************************************************************************
//
// Purpose: Global class to hold variables that are used by the 
//   CIteratorThread class but need to persist between instantiations 
//   of the CIteratorThread class
//
//*****************************************************************************

#ifndef INC_CITERATORVARS_H
#define INC_CITERATORVARS_H

extern class CIteratorVars
{
public:
	// public functions
	CIteratorVars();
	~CIteratorVars();
public:
	void ResetVars();

public:
	// public accessors
	const double		prwin() { return _prwin; }
	const double		prtie() { return _prtie; }
	const double		prlos() { return _prlos; }
	const bool			iterator_thread_running() { return _iterator_thread_running; }
	const bool			iterator_thread_complete() { return _iterator_thread_complete; }
	const unsigned int	iterator_thread_progress() { return _iterator_thread_progress; }
	const unsigned int	nit() { return _nit; } // iterations
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
	void set_pcard(const unsigned int i, const unsigned int c) { ENT if (i>=0 && i<=1) _pcard[i] = c; }
	void set_ccard(const unsigned int i, const unsigned int c) { ENT if (i>=0 && i<=4) _ccard[i] = c; }
#undef ENT

private:
	// private variables - use public accessors and public mutators to address these
	unsigned int	_nit;
	unsigned int	_pcard[k_number_of_cards_per_player];
	unsigned int	_ccard[k_number_of_community_cards];
	double			_prwin, _prtie, _prlos;
	bool			_iterator_thread_running;
	bool			_iterator_thread_complete;
	unsigned int	_iterator_thread_progress;

private:
	// private functions and variables - not available via accessors or mutators
	CCritSec	m_critsec;

} iter_vars;

#endif INC_CITERATORVARS_H