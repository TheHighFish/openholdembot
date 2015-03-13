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

#include "stdafx.h"
#include "CSymbolEngineMemorySymbols.h"
#include "CParseTreeNode.h"
#include "CPreferences.h"

CSymbolEngineMemorySymbols *p_symbol_engine_memory_symbols = NULL;

CSymbolEngineMemorySymbols::CSymbolEngineMemorySymbols() {
}

CSymbolEngineMemorySymbols::~CSymbolEngineMemorySymbols() {
  _memory_symbols.clear();
}

void CSymbolEngineMemorySymbols::InitOnStartup() {
}

void CSymbolEngineMemorySymbols::ResetOnConnection() {
  _memory_symbols.clear();
}

void CSymbolEngineMemorySymbols::ResetOnHandreset() {
}

void CSymbolEngineMemorySymbols::ResetOnNewRound() {
}

void CSymbolEngineMemorySymbols::ResetOnMyTurn() {
}

void CSymbolEngineMemorySymbols::ResetOnHeartbeat() {
}

void CSymbolEngineMemorySymbols::Store(CString command) {
  assert(command.Left(6) == "me_st_");
  CString command_without_prefix = command.Mid(6); 
  // Get the name of the symbol, up to the next underscore
  int position_of_first_underscore = command_without_prefix.Find('_');
  CString symbol_name = command_without_prefix.Left(position_of_first_underscore);
  // Get the right hand value after the underscore
  CString right_hand_value = command_without_prefix.Mid(position_of_first_underscore + 1);
  double result = EvaluateRightHandExpression(right_hand_value);
  _memory_symbols[symbol_name] = result;
}

double CSymbolEngineMemorySymbols::EvaluateRightHandExpression(CString right_hand_value) {
  // Possible use-case
  //   * constants        me_st_x_3_141
  //   * functions        me_st_x_f$myfunc
  //   * symbols          me_st_x_userchair
  //   * memory-symbols   me_st_x_re_re_y
  // Already removed: "me_st_x_"
  // All we have is the oure right-hand-side
  if (isdigit(right_hand_value[0])) {
    // Constant
    // Remove possible underscore by decimal point
    right_hand_value.Replace('_', '.');
    return atof(right_hand_value);
  }
  return CParseTreeNode::EvaluateIdentifier(right_hand_value, 
    MAIN->p_preferences()->trace_enabled());
}

void CSymbolEngineMemorySymbols::Increment(CString command) {
  assert(command.Left(7) == "me_inc_");
  CString symbol_name = command.Mid(7);
  ++_memory_symbols[symbol_name];
}

double CSymbolEngineMemorySymbols::Recall(CString command) {
  assert(command.Left(6) == "me_re_");
  CString symbol_name = command.Mid(6);
  return _memory_symbols[symbol_name];
}

bool CSymbolEngineMemorySymbols::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
  // memory-commands
  // "name" = query
  FAST_EXIT_ON_OPENPPL_SYMBOLS(name);
  if (memcmp(name, "me_", 3) == 0) {
    if (memcmp(name, "me_st_", 6) == 0) {  
      Store(name);
      *result = k_undefined_zero;
      return true;
    } else if (memcmp(name, "me_re_", 6) == 0) {
      *result = Recall(name);
      return true;
    } else if (memcmp(name, "me_inc_", 7) == 0) {
      Increment(name);
      *result = k_undefined_zero;
      return true;
    } else {
    // Looks like a memory-command, but is invalid
    return false;
    }
  }
  // Not a memory symbol
  return false;
}

// SymbolsProvided() does not make much sense here
// as we only know the prefixes
