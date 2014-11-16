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

#ifndef INC_CSYMBOLENGINE_TABLESTATS_H
#define INC_CSYMBOLENGINE_TABLESTATS_H

#include "CVirtualSymbolEngine.h"

class CSymbolEngineTableStats: public CVirtualSymbolEngine {
 public:
	CSymbolEngineTableStats();
	~CSymbolEngineTableStats();
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
	CString SymbolsProvided();
 public:
 private:
  double FlopTurnRiverPct(int betround);
  double AvgBetsPf();
	double TablePfr();
};

extern CSymbolEngineTableStats *p_symbol_engine_table_stats;

#endif INC_CSYMBOLENGINE_TABLESTATS_H