//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Providing functions (f$init_once_per_hand, etc.)
//   to deal with memory-symbols and other initializations (DLL maybe)
//   and get rid of the old advice "put it into f$alli".
//
// Not really a symbol-engine, but easy to implement with this concept.
//
//******************************************************************************

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
  void UpdateOnConnection();
  void UpdateOnHandreset();
  void UpdateOnNewRound();
  void UpdateOnMyTurn();
  void UpdateOnHeartbeat();
 public:
  CString SymbolsProvided();
};

#endif INC_CSYMBOLENGINEINIFUNCTIONS_H
