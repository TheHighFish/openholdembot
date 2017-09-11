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

#ifndef INC_CPARSETREETERMINALNODEBETSIZEACTION_H
#define INC_CPARSETREETERMINALNODEBETSIZEACTION_H

#include "CParseTreeNode.h"

class CParseTreeTerminalNodeBetsizeAction: public CParseTreeNode {
  friend class CParseTreeRotator;
 public:
   CParseTreeTerminalNodeBetsizeAction(int relative_line_number);
  virtual ~CParseTreeTerminalNodeBetsizeAction();
 public:
  void MakeRaiseToAction(TPParseTreeNode raise_by_amount_in_big_blinds);
  void MakeRaiseByAction(TPParseTreeNode raise_by_amount_in_big_blinds);
  // Values to be expected in the range (0..100] (or more), not (0..1]
  void MakeRaiseByPercentagedPotsizeAction(TPParseTreeNode raise_by_amount_percentage);
 public:
  bool EvaluatesToBinaryNumber();
 public:
  // For debugging output
  CString Serialize();
 public:
  double Evaluate(bool log /* = false */);
  CString EvaluateToString(bool log /* = false */);
};

typedef CParseTreeTerminalNodeBetsizeAction *TPParseTreeTerminalNodeBetsizeAction;

#endif INC_CPARSETREETERMINALNODEBETSIZEACTION_H