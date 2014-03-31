#ifndef INC_CSYMBOLENGINEBLINDS_H
#define INC_CSYMBOLENGINEBLINDS_H

#include "CVirtualSymbolEngine.h"

#include "CSymbolengineUserChair.h"
#include "NumericalFunctions.h"

extern class CSymbolEngineBlinds: public CVirtualSymbolEngine
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
private:
	int _nplayersdealt;
	int _playersplayingbits;
	int _dealerchair;
	int _userchair;
} *p_symbol_engine_blinds;

#endif INC_CSYMBOLENGINEBLINDS_H