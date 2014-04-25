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

#ifndef INC_CENGINECONTAINER_H
#define INC_CENGINECONTAINER_H

#include "CVirtualSymbolEngine.h"

const int k_max_number_of_symbol_engines = 50;

class CEngineContainer {
 public:
  CEngineContainer();
  ~CEngineContainer();
 public:
  // both to be called by the auto-connector
  void ResetOnConnection();
  void ResetOnDisconnection();
 public:
  void CallSymbolEnginesToUpdateSymbolsIfNecessary();
  bool EvaluateSymbol(const char *name, double *result);
  CString SymbolsProvided()       { return _list_of_symbols; }
 private:
  void BuildListOfSymbolsProvided();
 private:
  void CreateSpecialSymbolEngines();
  void AddSymbolEngine(CVirtualSymbolEngine *new_symbol_engine);
  void CreateSymbolEngines();
  void DestroyAllSymbolEngines();
  void DestroyAllSpecialSymbolEngines();
 private:
  void ResetOnHandreset();
  void ResetOnNewRound();
  void ResetOnMyTurn();
  void ResetOnHeartbeat();
 private:
  CVirtualSymbolEngine *_symbol_engines[k_max_number_of_symbol_engines]; 
  int _number_of_symbol_engines_loaded;
  bool _reset_on_connection_executed;
  CString _list_of_symbols;
};

extern CEngineContainer *p_engine_container;

#endif INC_CENGINECONTAINER_H