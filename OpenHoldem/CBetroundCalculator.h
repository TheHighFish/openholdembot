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
	int betround()			{ return _betround; }
	bool IsNewBetround()	{ return (_betround != _betround_previous_heartbeat); }
private:
	int _betround;
	int _betround_previous_heartbeat;
} *p_betround_calculator;

#endif