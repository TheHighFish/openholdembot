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

#ifndef INC_CPARSERSYMBOLTABLE_H
#define INC_CPARSERSYMBOLTABLE_H

#include <map>

class CParserSymbolTable {
 public:
  CParserSymbolTable();
  ~CParserSymbolTable();
 public:
  void Clear();
  void VerifySymbol(CString name);
  void VeryfyAllUsedFunctionsAtEndOfParse();
 private:
  std::map<CString, bool> _known_symbols;
  std::map<CString, bool> _used_functions;
};

#endif INC_CPARSERSYMBOLTABLE_H