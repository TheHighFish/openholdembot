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

#ifndef INC_CSYMBOLENGINETIME_H
#define INC_CSYMBOLENGINETIME_H

#include "CVirtualSymbolEngine.h"

extern class CSymbolEngineTime: public CVirtualSymbolEngine
{
public:
	CSymbolEngineTime();
	~CSymbolEngineTime();
public:
	// Mandatory reset-functions
	void InitOnStartup();
	void ResetOnConnection();
	void ResetOnHandreset();
	void ResetOnNewRound();
	void ResetOnMyTurn();
	void ResetOnHeartbeat();
public:
	void ResetOnAutoPlayerAction();
public:
	// Public accessors
	double elapsed()		{ return _elapsed; }
	double elapsedhand()	{ return _elapsedhand; }
	double elapsedauto()	{ return _elapsedauto; }
	double elapsedtoday()	{ return _elapsedtoday; }
private:
	double _elapsed;
	double _elapsedhand;
	double _elapsedauto;
	double _elapsedtoday;
private:
	time_t _elapsedautohold;	// The last time autoplayer acted
	time_t _elapsedhold;		// The time we "sat down"
	time_t _elapsedhandhold;	// The time since start of last hand
} *p_symbol_engine_time;

#endif INC_CSYMBOLENGINETIME_H