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

#ifndef INC_CSYMBOLENGINEPOKERACTION_H
#define INC_CSYMBOLENGINEPOKERACTION_H

#include "CVirtualSymbolEngine.h"

class CSymbolEnginePokerAction: public CVirtualSymbolEngine {
 public:
  CSymbolEnginePokerAction();
  ~CSymbolEnginePokerAction();
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
 public:
  const int		DealPosition(const int chairnum);
 private:
  // private functions and variables - not available via accessors or mutators
	const int		PreflopPos();
	const int		PreflopRaisPos();
	const int		PostflopPos();
	const bool	FirstIntoPot();	
	const int		BetPosition(const int chairnum);
	const bool	AgchairAfter();
};

#endif INC_CSYMBOLENGINEPOKERACTION_H