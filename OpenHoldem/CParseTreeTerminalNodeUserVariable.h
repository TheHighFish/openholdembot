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

#ifndef INC_CPARSETREETERMINALNODEUSERVARIABLE_H
#define INC_CPARSETREETERMINALNODEUSERVARIABLE_H

#include "CParsetreeTerminalNodeIdentifier.h"

class CParseTreeTerminalNodeUserVariable: public CParseTreeTerminalNodeIdentifier {
  friend class CFormulaParser;
  friend class CParseTreeRotator;
 public:
  CParseTreeTerminalNodeUserVariable(int relative_line_number, CString name);
  virtual ~CParseTreeTerminalNodeUserVariable();
 public:
  virtual double Evaluate(bool log = false);
  virtual CString EvaluateToString(bool log = false);
 public:
  // For debugging output
  CString Serialize();
};

typedef CParseTreeTerminalNodeUserVariable *TPParseTreeTerminalNodeUserVariable;

#endif IINC_CPARSETREETERMINALNODEUSERVARIABLE_H