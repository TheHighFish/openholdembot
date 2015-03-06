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

#ifndef INC_CSYMBOLENGINEICM
#define INC_CSYMBOLENGINEICM

#include "CVirtualSymbolEngine.h"

class CSymbolEngineICM: public CVirtualSymbolEngine {
 public:
  CSymbolEngineICM();
  ~CSymbolEngineICM();
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
	bool EvaluateSymbol(const char *name, double *result, bool log = false);
	CString SymbolsProvided();
 private:
  int GetChairFromDealPos(const char* pquery);
  double EquityICM(double *stacks, double *prizes, int playerNB, int player);
};

#endif INC_CSYMBOLENGINEICM