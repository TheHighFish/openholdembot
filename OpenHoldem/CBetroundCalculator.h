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

#ifndef INC_CBETROUNDCALCULATOR_H
#define INC_CBETROUNDCALCULATOR_H

// The betround-calculator is not a normal symbol-engine,
// because changes in the betround need to be known
// by the symbol-engine-container / all other symbol-engines
// in advance (just like hand-resets).

#define BETROUND p_betround_calculator->betround()

extern class CBetroundCalculator
{
public:
	CBetroundCalculator();
	~CBetroundCalculator();
public:
	void OnNewHeartbeat();
public:
	int betround()			
	{ 
		// Betround is very important and gets used as an index at various places
		// so we always make sure that it is not out of range.
		assert(_betround >= k_betround_preflop);
		assert(_betround <= k_betround_river);
		return _betround; 
	}
public:
	bool IsNewBetround();
private:
	int _betround;
	int _betround_previous_heartbeat;
} *p_betround_calculator;

#endif