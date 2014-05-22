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

#ifndef INC_CSYMBOLENGINEBLINDS_H
#define INC_CSYMBOLENGINEBLINDS_H

#include "CVirtualSymbolEngine.h"

#include "CSymbolengineUserChair.h"
#include "NumericalFunctions.h"

class CSymbolEngineBlinds: public CVirtualSymbolEngine
{
public:
	CSymbolEngineBlinds();
	~CSymbolEngineBlinds();
public:
	bool EvaluateSymbol(const char *name, double *result, bool log = false);
	// Mandatory reset-functions
	void InitOnStartup();
	void ResetOnConnection();
	void ResetOnHandreset();
	void ResetOnNewRound();
	void ResetOnMyTurn();
	void ResetOnHeartbeat();
public:
	// Public accessors
	int bblindbits()			{ return _bblindbits; }
	int opponentsblindbits()	{ return _playersblindbits & ~p_symbol_engine_userchair->userchairbit(); }
	int playersblindbits()	{ return _playersblindbits; }
	int nplayersblind()		{ return bitcount(_playersblindbits); }
	int nopponentsblind()	{ return bitcount(opponentsblindbits()); }
private:
	bool BlindsAreUnknown();
	void CalculateBlinds();
private:
	int _playersblindbits;
	int _bblindbits;
};

extern CSymbolEngineBlinds *p_symbol_engine_blinds;

#endif INC_CSYMBOLENGINEBLINDS_H