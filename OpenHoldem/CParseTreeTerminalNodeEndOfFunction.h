//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#pragma once

#include "CParsetreeTerminalNodeIdentifier.h"

class CParseTreeTerminalNodeEndOfFunction: public CParseTreeTerminalNodeIdentifier {
  friend class CFormulaParser;
  friend class CParseTreeRotator;
 public:
  CParseTreeTerminalNodeEndOfFunction(int relative_line_number);
  virtual ~CParseTreeTerminalNodeEndOfFunction();
 public:
  virtual double Evaluate(bool log = false);
  static bool evaluating_defailt_logic() {
    return _evaluating_defailt_logic; 
  }
 private:
  static bool _evaluating_defailt_logic;
};

typedef CParseTreeTerminalNodeEndOfFunction *TPParseTreeTerminalNodeEndOfFunction;

