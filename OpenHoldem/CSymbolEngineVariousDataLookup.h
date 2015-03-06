//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Symbol lookup for various symbols 
//   that are not part of a regular symbol-engine
//
//******************************************************************************

#ifndef INC_CSYMBOLS_H
#define INC_CSYMBOLS_H

#include "CVirtualSymbolEngine.h"

class CSymbolEngineVariousDataLookup: public CVirtualSymbolEngine
{
public:
	// public functions
	CSymbolEngineVariousDataLookup();
	~CSymbolEngineVariousDataLookup();
public:
	// Mandatory reset-functions
	void InitOnStartup();
	void ResetOnConnection();
	void ResetOnHandreset();
	void ResetOnNewRound();
	void ResetOnMyTurn();
	void ResetOnHeartbeat();
public:
	bool EvaluateSymbol(const char *name, double *result, bool log = false);
	CString SymbolsProvided();
private:
};

#endif /* INC_CSYMBOLS_H */
