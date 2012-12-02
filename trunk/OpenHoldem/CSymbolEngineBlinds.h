#ifndef INC_CSYMBOLENGINEBLINDS_H
#define INC_CSYMBOLENGINEBLINDS_H

#include "CVirtualSymbolEngine.h"
#include "..\CTransform\CTransform.h"

class CSymbolEngineBlinds: public CVirtualSymbolEngine
{
public:
	CSymbolEngineBlinds();
	~CSymbolEngineBlinds();
public:
	// Mandatory reset-functions
	void InitOnStartup();
	void ResetOnConnection();
	void ResetOnHandreset();
	void ResetOnNewRound();
	void ResetOnMyTurn();
	void ResetOnHeartbeat();
public:
	// Public accessors
	double bblindbits()			{ return _bblindbits; }
	double opponentsblindbits()	{ return _opponentsblindbits; }
	double playersblindbits()	{ return _playersblindbits; }
	double nplayersblind()		{ return bitcount(_playersblindbits); }
	double nopponentsblind()	{ return bitcount(_opponentsblindbits); }
private:
	bool BlindsAreUnknown();
	void CalculateBlinds();
private:
	double _playersblindbits;
	double _opponentsblindbits;
	double _bblindbits;
};

#endif INC_CSYMBOLENGINEBLINDS_H