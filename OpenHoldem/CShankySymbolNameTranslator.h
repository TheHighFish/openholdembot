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

#pragma once

#include <map>

class CShankySymbolNameTranslator {
 public:
  CShankySymbolNameTranslator();
  ~CShankySymbolNameTranslator();
 public:
  CString Translate(CString symbol);
 private:
  void InitSymbolTable();
 private:
  std::map<CString, CString> _shanky_symbol_table;
};
