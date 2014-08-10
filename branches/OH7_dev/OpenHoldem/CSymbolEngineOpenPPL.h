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

#ifndef INC_CSYMBOLENGINEOPENPPL_H
#define INC_CSYMBOLENGINEOPENPPL_H

class CSymbolEngineOpenPPL {
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
  CString SymbolsProvided();
 public:
  double Decision();
 private:
  double EvaluateOpenPPLMainFunctionForCurrentBetround();
  bool IsOpenPPLProfile();
 private:
  double _decision; 
};

extern CSymbolEngineOpenPPL *p_symbol_engine_openppl;

#endif INC_CSYMBOLENGINEOPENPPL_H