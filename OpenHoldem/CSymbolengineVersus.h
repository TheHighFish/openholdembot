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

#ifndef INC_CSYMBOLENGINEVERSUS_H
#define INC_CSYMBOLENGINEVERSUS_H

#include "CVirtualSymbolEngine.h"

class CSymbolEngineVersus: public CVirtualSymbolEngine {
 public:
  CSymbolEngineVersus();
  ~CSymbolEngineVersus();
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
};

extern CSymbolEngineVersus *p_symbol_engine_versus;

#endif INC_CSYMBOLENGINEVERSUS_H