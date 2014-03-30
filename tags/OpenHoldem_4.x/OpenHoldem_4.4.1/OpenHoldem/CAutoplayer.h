//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose:
//
//***************************************************************************** 

#ifndef INC_CAUTOPLAYER_H
#define INC_CAUTOPLAYER_H

#include "../CCritSec/CCritSec.h"
#include "MainFrm.h"
#include "../CTablemap/CTablemap.h"
#include "../CTablemap/CTableMapAccess.h"
#include "OpenHoldem.h"

extern class CAutoplayer 
{
public:
	// public functions
	CAutoplayer();
	~CAutoplayer(void);
public:
	void EngageAutoPlayerUponConnectionIfNeeded();
	void DoAutoplayer(void);

public:
	// public accessors
	const bool autoplayer_engaged() { return _autoplayer_engaged; }

public:
	// public mutators
	void EngageAutoplayer(bool to_be_enabled_or_not);

private:
	// private functions and variables - not available via accessors or mutators
	void DoRebuyIfNeccessary(void);
	bool ExecutePrimaryFormulasIfNecessary();
	bool ExecuteSecondaryFormulasIfNecessary();
	bool ExecuteRaiseCallCheckFold();
	bool ExecuteBeep();
	bool AnyPrimaryFormulaTrue();
	bool AnySecondaryFormulaTrue();
	bool DoAllin(void);
	bool DoBetPot(void);
	bool HandleInterfacebuttonsI86(void); 
	void PrepareActionSequence();
	void FinishActionSequenceIfNecessary();
	bool DoSwag(void);
	bool DoPrefold(void);
	bool TimeToHandleSecondaryFormulas();
	bool DoChat(void);

private:
	// private variables - use public accessors and public mutators to address these
	bool	_autoplayer_engaged;

private:
	RMapCI	_alli_but, _rais_but, _call_but, _chec_but, _fold_but;
	RMapCI	_autopost_but, _sitin_but, _sitout_but, _leave_but, _pre_fold_but;
	bool	_autopost_state, _sitin_state, _sitout_state;
	POINT	cursor_position;
	HWND	window_with_focus;
	bool	action_sequence_needs_to_be_finished;

	CCritSec	m_critsec;
} *p_autoplayer;


#endif //INC_CAUTOPLAYER_H
