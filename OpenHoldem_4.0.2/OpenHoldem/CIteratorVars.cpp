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

#include "stdafx.h"
#include "CIteratorVars.h"

#include "CPreferences.h"

CIteratorVars iter_vars; // !!! Bad: global object! Undefined equence of initialization!


CIteratorVars::CIteratorVars()
{
	ResetVars();
}

CIteratorVars::~CIteratorVars()
{}

void CIteratorVars::ResetVars()
{
	write_log(prefs.debug_prwin(), "[CiteratorVars] Resetting PrWin variables\n");
	_nit = 0;
	_f$p = 0;

	for (int i=0; i<k_number_of_cards_per_player; i++)
		_pcard[i] = CARD_NOCARD;

	for (int i=0; i<k_number_of_community_cards; i++)
		_ccard[i] = CARD_NOCARD;

	_prwin = 0;
	_prtie = 0;
	_prlos = 0;
	_iterator_thread_running = false;
	_iterator_thread_complete = true;
	_iterator_thread_progress = 0;
}


