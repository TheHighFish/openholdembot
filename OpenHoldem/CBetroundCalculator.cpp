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

#include "CScraper.h"
#include "MagicNumbers.h"

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
	// Betround is a very important prerequisite
	// to determine what symbols shall be calculated.
	// So we can hardly do it with a symbol-engine and do it here
	if (!p_scraper->IsCommonAnimation())
	{
		if (p_scraper->card_common(4) != CARD_NOCARD)
		{
			_betround = k_betround_river;
		}
		else if (p_scraper->card_common(3) != CARD_NOCARD)
		{
			_betround = k_betround_turn;
		}
		else if (p_scraper->card_common(2) != CARD_NOCARD 
			&& p_scraper->card_common(1) != CARD_NOCARD 
			&& p_scraper->card_common(0) != CARD_NOCARD)
		{
			_betround = k_betround_flop;
		}
		else
		{
			_betround = k_betround_preflop;
		}
	}
	else
	{
		// There is a common card animation going on currently
		// so lets not try to determine the betround,
		// but if it's a new hand then lets default to pre-flop
		if (_betround_previous_heartbeat == k_undefined)
		{
			_betround = k_betround_preflop;
		}
	}
	_betround_previous_heartbeat = _betround;
}

