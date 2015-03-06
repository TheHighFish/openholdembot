//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Detecting if we play a holdem or omaha,
//   needed e.g. for automatic adaption of PT-queries.
//
//******************************************************************************

#ifndef INC_CSYMBOLENGINEISOMAHA_H
#define INC_CSYMBOLENGINEISOMAHA_H

#include "CVirtualSymbolEngine.h"

class CSymbolEngineIsOmaha: public CVirtualSymbolEngine
{
public:
	CSymbolEngineIsOmaha();
	~CSymbolEngineIsOmaha();
public:
  bool EvaluateSymbol(const char *name, double *result, bool log = false);
  CString SymbolsProvided();
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
	bool isomaha()		{ return false; }
};

#endif INC_CSYMBOLENGINEISOMAHA_H
