#ifndef INC_CBETROUNDCALCULATOR_H
#define INC_CBETROUNDCALCULATOR_H

// The betround-calculator is not a normal symbol-engine,
// because changes in the betround need to be known
// by the symbol-engine-container / all other symbol-engines
// in advance (just like hand-resets).

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
	bool IsNewBetround()	{ return (_betround != _betround_previous_heartbeat); }
private:
	int _betround;
	int _betround_previous_heartbeat;
} *p_betround_calculator;

#endif