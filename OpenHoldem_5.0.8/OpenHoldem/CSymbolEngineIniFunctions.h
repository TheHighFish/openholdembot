//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*****************************************************************************
//
// Purpose: Providing functions (f$init_once_per_hand, etc.)
//   to deal with memory-symbols and other initializations (DLL maybe)
//   and get rid of the old advice "put it into f$alli".
//
// Not really a symbol-engine, but easy to implement with this concept.
//
//*****************************************************************************

#ifndef INC_CSYMBOLENGINEINIFUNCTIONS_H
#define INC_CSYMBOLENGINEINIFUNCTIONS_H

#include "CVirtualSymbolEngine.h"

class CSymbolEngineIniFunctions: public CVirtualSymbolEngine {
public:
  CSymbolEngineIniFunctions();
  ~CSymbolEngineIniFunctions();
 public:
  // Mandatory reset-functions
  void InitOnStartup();
  void ResetOnConnection();
  void ResetOnHandreset();
  void ResetOnNewRound();
  void ResetOnMyTurn();
  void ResetOnHeartbeat();
 public:
  CString SymbolsProvided();
};

extern CSymbolEngineIniFunctions *p_symbol_engine_ini_functions;

#endif INC_CSYMBOLENGINEINIFUNCTIONS_H
