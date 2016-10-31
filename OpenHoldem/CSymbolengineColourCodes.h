//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#ifndef INC_CSYMBOLENGINECOLOURCODES_H
#define INC_CSYMBOLENGINECOLOURCODES_H

#include "CVirtualSymbolEngine.h"

class CSymbolEngineColourCodes: public CVirtualSymbolEngine {
 public:
	CSymbolEngineColourCodes();
	~CSymbolEngineColourCodes();
 public:
	// Mandatory reset-functions
	void InitOnStartup();
	void UpdateOnConnection();
	void UpdateOnHandreset();
	void UpdateOnNewRound();
	void UpdateOnMyTurn();
	void UpdateOnHeartbeat();
 public:
	// Public accessors
	bool EvaluateSymbol(const char *name, double *result, bool log = false);
	CString SymbolsProvided();;
 public:
  // Pokertracker icon (if meaningful sample-size),
  // otherwise scraped colour-code.
  int ColourCodeToDisplay(const int chair);
};

extern CSymbolEngineColourCodes *p_symbol_engine_colourcodes;

#endif INC_CSYMBOLENGINECOLOURCODES_H