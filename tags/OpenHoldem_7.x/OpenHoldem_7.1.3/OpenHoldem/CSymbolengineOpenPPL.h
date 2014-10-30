	//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose: This symbol-engine does not provide any symbols,
//   but triggers only the calculation of history/symbols, etc.
//   All OpenPPL-symbols are functions that get handled by the function-collection.
//
//***************************************************************************** 

#ifndef INC_CSYMBOLENGINE_OPENPPL_HANDSTRENGTH_H
#define INC_CSYMBOLENGINE_OPENPPL_HANDSTRENGTH_H

#include "CVirtualSymbolEngine.h"

class CSymbolEngineOpenPPL: public CVirtualSymbolEngine
{
 public:
	CSymbolEngineOpenPPL();
	~CSymbolEngineOpenPPL();
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
  void InitMemorySymbols();
};

extern CSymbolEngineOpenPPL *p_symbol_engine_open_ppl;

#endif INC_CSYMBOLENGINE_OPENPPL_HANDSTRENGTH_H