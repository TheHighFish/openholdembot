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

#ifndef INC_CSYMBOLENGINE_OPENPPL_STACKSIZES_H
#define INC_CSYMBOLENGINE_OPENPPL_STACKSIZES_H

#include "CVirtualSymbolEngine.h"

class CSymbolEngineOpenPPLStacksizes: public CVirtualSymbolEngine
{
public:
	CSymbolEngineOpenPPLStacksizes();
	~CSymbolEngineOpenPPLStacksizes();
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
	CString IdentifiersProvided();;
public:
private:
};

extern CSymbolEngineOpenPPLStacksizes *p_symbol_engine_open_ppl_stacksizes;

#endif INC_CSYMBOLENGINE_OPENPPL_STACKSIZES_H