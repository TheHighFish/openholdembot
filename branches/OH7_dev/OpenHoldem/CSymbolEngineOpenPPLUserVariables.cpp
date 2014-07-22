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

#include "stdafx.h"
#include "CSymbolEngineOpenPPLUserVariables.h"

#include "CPreferences.h"
#include "CStringHashtableTemplate.h"

CSymbolEngineOpenPPLUserVariables *p_symbol_engine_openppl_user_variables = NULL;

CSymbolEngineOpenPPLUserVariables::CSymbolEngineOpenPPLUserVariables() {
}

CSymbolEngineOpenPPLUserVariables::~CSymbolEngineOpenPPLUserVariables() {
}

void CSymbolEngineOpenPPLUserVariables::InitOnStartup() {
}

void CSymbolEngineOpenPPLUserVariables::ResetOnConnection() {
}

void CSymbolEngineOpenPPLUserVariables::ResetOnHandreset() {
  // All user-variables are for the current hand only 
  // and get deleted on hand-reset.
  write_log(preferences.debug_symbolengine_open_ppl(),
    "[CSymbolEngineOpenPPLUserVariables] Deleting all user variables on hand-reset\n");
  _user_variables.clear();
}

void CSymbolEngineOpenPPLUserVariables::ResetOnNewRound() {
}

void CSymbolEngineOpenPPLUserVariables::ResetOnMyTurn() {
}

void CSymbolEngineOpenPPLUserVariables::ResetOnHeartbeat() {
}

void CSymbolEngineOpenPPLUserVariables::Set(CString symbol) {
  write_log(preferences.debug_symbolengine_open_ppl(),
    "[CSymbolEngineOpenPPLUserVariables] Setting user-variable %s\n", symbol);
  _user_variables[symbol] = true;
}

bool CSymbolEngineOpenPPLUserVariables::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
  FAST_EXIT_ON_OPENPPL_SYMBOLS(name);
  if (memcmp(name, "user", 4) != 0) {
    // Not a user-variable
    return false;
  }
  if (memcmp(name+4, "chair", 5) == 0) {
    // Symbol "userchair", not a user-variable
    return false;
  }
  // Try to look it up
  if (_user_variables[name]) {
    write_log(preferences.debug_symbolengine_open_ppl(),
      "[CSymbolEngineOpenPPLUserVariables] user-variable exists: %s\n", name);
    *result = double(true);
    return true;
  }
  write_log(preferences.debug_symbolengine_open_ppl(),
    "[CSymbolEngineOpenPPLUserVariables] user-variable does not exist: %s\n", name);
  *result = double(false);
  return true;
}

// SymbolsProvided() does not make much sense here
// as we only know the prefixes

