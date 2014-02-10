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

#ifndef INC_CSYMBOLENGINEPRWIN_H
#define INC_CSYMBOLENGINEPRWIN_H

#include "CVirtualSymbolEngine.h"
#include "assert.h"
#include "CSymbolEngineUserChair.h"
#include "CSymbolEngineTableLimits.h"
#include "MagicNumbers.h"

extern class CSymbolEnginePrwin: public CVirtualSymbolEngine
{
public:
	CSymbolEnginePrwin();
	~CSymbolEnginePrwin();
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
	bool EvaluateSymbol(const char *name, double *result);
	int nhands()		{ return (_nhandshi + _nhandslo + _nhandsti); }
	int nhandshi()		{ return _nhandshi; }
	int nhandslo()		{ return _nhandslo; }
	int nhandsti()		{ return _nhandsti; }
	double prwinnow()	{ return _prwinnow; }
	double prlosnow()	{ return _prlosnow; }
public:
	int nopponents_for_prwin()	{ return _nopponents_for_prwin; }
private:
	void CalculateNOpponents();
	void CalculateNhands();
private:
	int _nhandshi;
	int _nhandslo;
	int _nhandsti;
	double _prwinnow;
	double _prlosnow;
private:
	int _nopponents_for_prwin;
} *p_symbol_engine_prwin;

#endif INC_CSYMBOLENGINEPRWIN_H
