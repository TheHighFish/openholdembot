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

#include "stdafx.h"
#include "CBetroundCalculator.h"

#include "CPreferences.h"
#include "CScraper.h"
#include "CTableState.h"
#include "MagicNumbers.h"
#include "StringFunctions.h"

CBetroundCalculator *p_betround_calculator = NULL;

CBetroundCalculator::CBetroundCalculator()
{
	// Betround will be indirectly used 
	// when a formula gets loaded and verified.
	// (it gets used for symbol lookups).
	// So it should have a meaningful value,
	// even before we connect to a table.
	_betround = k_betround_preflop;
	_betround_previous_heartbeat = k_undefined;
}

CBetroundCalculator::~CBetroundCalculator()
{}

void CBetroundCalculator::OnNewHeartbeat()
{
	// Save old value first before before changing _betround
	// Otherwise the values will be identical all the time.
	_betround_previous_heartbeat = _betround;
	write_log(preferences.debug_alltherest(), "[CBetroundCalculator] _betround_previous_heartbeat = %i\n",
		_betround_previous_heartbeat);
	// Betround is a very important prerequisite
	// to determine what symbols shall be calculated.
	// So we can hardly do it with a symbol-engine and do it here
	if (!p_scraper->IsCommonAnimation()) {
    if (p_table_state->_common_cards[4].IsKnownCard()) {
			_betround = k_betround_river;
		}
		else if (p_table_state->_common_cards[3].IsKnownCard()) {
			_betround = k_betround_turn;
		}
		else if (p_table_state->_common_cards[2].IsKnownCard() 
			  && p_table_state->_common_cards[1].IsKnownCard() 
			  && p_table_state->_common_cards[0].IsKnownCard()) {
			_betround = k_betround_flop;
		}	else {
			_betround = k_betround_preflop;
		}
	}
	else {
		// There is a common card animation going on currently
		// so lets not try to determine the betround,
		// but if it's a new hand then lets default to pre-flop
		write_log(preferences.debug_alltherest(), "[CBetroundCalculator] Animation going on\n");
		if (_betround_previous_heartbeat == k_undefined)
		{
			_betround = k_betround_preflop;
		}
	}
	write_log(preferences.debug_alltherest(), "[CBetroundCalculator] _betround = %i\n",
		_betround);
}


bool CBetroundCalculator::IsNewBetround()
{ 
	bool result = (_betround != _betround_previous_heartbeat);
	write_log(preferences.debug_alltherest(), "[CBetroundCalculator] IsNewBetround() = %s\n",
		Bool2CString(result));
	return result; 
}