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

#include "CParseTreeTerminalNode.h"
#include "CParseTreeTerminalNodeIdentifier.h"
#include "CPreferences.h"
#include "OH_MessageBox.h"

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

void CSymbolEngineMemorySymbols::ErrorInvalidMemoryStoreCommand(CString command) {
  // Handles wrong me_st_ commands.
  CString message;
  message.Format("Invalid memory-symbol: %s\n"
    "Memory-store-commands must contain\n"
    "  * the prefix me_st_\n"
    "  * the name of the variable\n"
    "  * another underscore\n"
    "  * a value\n"
    " Example: me_st_Pi_3_141592653\n",
    command);
  OH_MessageBox_Formula_Error(message, "Error");
}

void CSymbolEngineMemorySymbols::ErrorUnnamedMemorySymbol(CString command) {
  // Handles wrong me_re_ and me_inc_ commands
  // but not me_st_.
  CString message;
  message.Format("Invalid memory-symbol: %s\n"
    "Missing variable name.\n"
    "Memory-recall and memory-increment commands must contain\n"
    "  * the prefix me_re_ or me_inc_\n"
    "  * the name of the variable\n"
    " Example: me_inc_ContiBetsRaised\n",
    command);
  OH_MessageBox_Formula_Error(message, "Error");
}

void CSymbolEngineMemorySymbols::Store(CString command) {
  assert(command.Left(6) == "me_st_");
  CString command_without_prefix = command.Mid(6); 
  write_log(preferences.debug_memorysymbols(), 
    "[CSymbolEngineMemorySymbols] command without prefix> %s\n", command_without_prefix);
  // Get the name of the symbol, up to the next underscore
  int position_of_first_underscore = command_without_prefix.Find('_');
  write_log(preferences.debug_memorysymbols(), 
    "[CSymbolEngineMemorySymbols] position of under-score: %i\n", 
    position_of_first_underscore);
  if (position_of_first_underscore <= 0) {
    // Missing right hand side expression, including under-score
    ErrorInvalidMemoryStoreCommand(command);
    return;
  }
  CString symbol_name = command_without_prefix.Left(position_of_first_underscore);
  write_log(preferences.debug_memorysymbols(), 
    "[CSymbolEngineMemorySymbols] symbol name: %s\n", symbol_name);
  // Get the right hand value after the underscore
  CString right_hand_side = command_without_prefix.Mid(position_of_first_underscore + 1);
  write_log(preferences.debug_memorysymbols(), 
    "[CSymbolEngineMemorySymbols] right hand side: %s\n", right_hand_side);
  if (right_hand_side == "") {
    // Empty right hand side expression after under-score
    ErrorInvalidMemoryStoreCommand(command);
    return;
  }
  double result = EvaluateRightHandExpression(right_hand_side);
  _memory_symbols[LowerCaseKey(symbol_name)] = result;
}

double CSymbolEngineMemorySymbols::EvaluateRightHandExpression(CString right_hand_value) {
  // Possible use-case
  //   * constants        me_st_x_3_141
  //   * functions        me_st_x_f$myfunc
  //   * symbols          me_st_x_userchair
  //   * memory-symbols   me_st_x_re_re_y
  // Already removed: "me_st_x_"
  // All we have is the oure right-hand-side
  double result = kUndefinedZero;
  if (isdigit(right_hand_value[0])) {
    // Constant
    // Remove possible underscore by decimal point
    right_hand_value.Replace('_', '.');
    result = atof(right_hand_value);
  } else {
    result = CParseTreeTerminalNodeIdentifier::EvaluateIdentifier(
      right_hand_value, true); // !!! Needs function parameter
  }
  write_log(preferences.debug_memorysymbols(), 
    "[CSymbolEngineMemorySymbols] Evaluating %s -> %.3f\n",
    right_hand_value, result);
  return result;
}

void CSymbolEngineMemorySymbols::Increment(CString command) {
  assert(command.Left(7) == "me_inc_");
  CString symbol_name = command.Mid(7);
  if (symbol_name == "") {
    ErrorUnnamedMemorySymbol(command);
    return;
  }
  ++_memory_symbols[LowerCaseKey(symbol_name)];
}

double CSymbolEngineMemorySymbols::Recall(CString command) {
  assert(command.Left(6) == "me_re_");
  CString symbol_name = command.Mid(6);
  if (symbol_name == "") {
    ErrorUnnamedMemorySymbol(command);
    return kUndefinedZero;
  }
  return _memory_symbols[LowerCaseKey(symbol_name)];
}

bool CSymbolEngineMemorySymbols::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
  // memory-commands
  // "name" = query
  FAST_EXIT_ON_OPENPPL_SYMBOLS(name);
  if (memcmp(name, "me_", 3) == 0) {
    write_log(preferences.debug_memorysymbols(), 
      "[CSymbolEngineMemorySymbols] EvaluateSymbol(%s)\n", name);
    if (memcmp(name, "me_st_", 6) == 0) {  
      Store(name);
      *result = kUndefinedZero;
      return true;
    } else if (memcmp(name, "me_re_", 6) == 0) {
      *result = Recall(name);
      return true;
    } else if (memcmp(name, "me_inc_", 7) == 0) {
      Increment(name);
      *result = kUndefinedZero;
      return true;
    } else {
    // Looks like a memory-command, but is invalid
    return false;
    }
  }
  // Not a memory symbol
  return false;
}

CString CSymbolEngineMemorySymbols::LowerCaseKey(CString symbol) {
  CString new_key = symbol.MakeLower();
  return new_key;
}

// SymbolsProvided() does not make much sense here
// as we only know the prefixes
