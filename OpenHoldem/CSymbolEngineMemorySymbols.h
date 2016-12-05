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

#ifndef INC_C_SYMBOLENGINEMEMORYSYMBOLS_H
#define INC_C_SYMBOLENGINEMEMORYSYMBOLS_H

#include <map>
#include "CVirtualSymbolEngine.h"

class CSymbolEngineMemorySymbols: public CVirtualSymbolEngine {
 public:
  CSymbolEngineMemorySymbols();
  ~CSymbolEngineMemorySymbols();
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
 private:
  void Store(CString command);
  void Increment(CString command);
  double Recall(CString command);
 private:
  double EvaluateRightHandExpression(CString right_hand_value);
 private:
  void ErrorInvalidMemoryStoreCommand(CString command);
  void ErrorUnnamedMemorySymbol(CString command);
 private:
  // The keys for storage and lookup are lower-cases
  // To make memory-symbols case insensitive, 
  // i.e. less error-prone as there are no compile-time-checks.
  // http://www.maxinmontreal.com/forums/viewtopic.php?f=156&t=19224
  CString LowerCaseKey(CString symbol);
 private:
  std::map<CString, double> _memory_symbols;
};

extern CSymbolEngineMemorySymbols *p_symbol_engine_memory_symbols;

#endif INC_C_SYMBOLENGINEMEMORYSYMBOLS_H