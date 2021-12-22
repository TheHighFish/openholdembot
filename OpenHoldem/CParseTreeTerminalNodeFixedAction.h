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

#ifndef INC_CPARSETREETERMINALNODEFIXEDACTION_H
#define INC_CPARSETREETERMINALNODEFIXEDACTION_H

#include "CParsetreeTerminalNodeIdentifier.h"

class CParseTreeTerminalNodeFixedAction: public CParseTreeTerminalNodeIdentifier {
  friend class CFormulaParser;
  friend class CParseTreeRotator;
 public:
  CParseTreeTerminalNodeFixedAction(int relative_line_number, CString name);
  virtual ~CParseTreeTerminalNodeFixedAction();
 public:
  virtual CString EvaluateToString(bool log = false);
 public:
  // For debugging output
  CString Serialize();
};

typedef CParseTreeTerminalNodeFixedAction *TPParseTreeTerminalNodeFixedAction;

#endif INC_CPARSETREETERMINALNODEFIXEDACTION_H