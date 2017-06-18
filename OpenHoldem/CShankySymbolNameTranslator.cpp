//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Adapting symbol names when importing Shanky PPL,
//   also caring about proper cases, because Shanky PPL is case-insensitive.
//
//******************************************************************************

#include "stdafx.h"
#include "CShankySymbolNameTranslator.h"

CShankySymbolNameTranslator::CShankySymbolNameTranslator() {
  InitSymbolTable();
}

CShankySymbolNameTranslator::~CShankySymbolNameTranslator() {
}

void CShankySymbolNameTranslator::InitSymbolTable() {
  _shanky_symbol_table.clear();
  _shanky_symbol_table["acepresentonflop"] = "AcePresentOnFlop";
  _shanky_symbol_table["botcalledbeforeflop"] = "BotCalledBeforeFlop";
  /*_shanky_symbol_table[""] = "";
  _shanky_symbol_table[""] = "";
  _shanky_symbol_table[""] = "";
  _shanky_symbol_table[""] = "";
  _shanky_symbol_table[""] = "";
  _shanky_symbol_table[""] = "";
  _shanky_symbol_table[""] = "";
  _shanky_symbol_table[""] = "";
  _shanky_symbol_table[""] = "";
  _shanky_symbol_table[""] = "";
  _shanky_symbol_table[""] = "";
  _shanky_symbol_table[""] = "";
  _shanky_symbol_table[""] = "";
  _shanky_symbol_table[""] = "";
  _shanky_symbol_table[""] = "";
  _shanky_symbol_table[""] = "";
  _shanky_symbol_table[""] = "";
  _shanky_symbol_table[""] = "";
  _shanky_symbol_table[""] = "";
  _shanky_symbol_table[""] = "";
  _shanky_symbol_table[""] = "";
  _shanky_symbol_table[""] = "";
  _shanky_symbol_table[""] = "";
  _shanky_symbol_table[""] = "";
  _shanky_symbol_table[""] = "";
  _shanky_symbol_table[""] = "";
  _shanky_symbol_table[""] = "";
  _shanky_symbol_table[""] = "";*/
}

CString CShankySymbolNameTranslator::Translate(CString symbol) {
  CString key = symbol;
  CString lookup_result = _shanky_symbol_table[key.MakeLower()];
  if (lookup_result != "") {
    return lookup_result;
  }
  // return symbol without modifications
  return symbol;
}