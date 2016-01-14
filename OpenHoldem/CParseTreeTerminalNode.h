//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose:
//
//*******************************************************************************

#ifndef INC_CPARSETREETERMINALNODE_H
#define INC_CPARSETREETERMINALNODE_H

#include "CParsetreeNode.h"

class CParseTreeTerminalNode: public CParseTreeNode {
  friend class CFormulaParser;
  friend class CParseTreeRotator;
 public:
  CParseTreeTerminalNode(int relative_line_number);
  ~CParseTreeTerminalNode();
 public:
  void MakeConstant(double value);
  void MakeIdentifier(CString name);
  void MakeAction(int action_constant);
  void MakeUserVariableDefinition(CString uservariable);
 public:
  double Evaluate(bool log = false);
  CString EvaluateToString(bool log = false);
  bool EvaluatesToBinaryNumber();
 public:
  // For debugging output
  CString Serialize();
 public:
  // Might be called by CSymbolEngineMemorySymbols
  // to evaluate right-hand-side expressions
  static double EvaluateIdentifier(CString name, bool log);
 private:
  bool IsBinaryIdentifier();
 private:
  void SetUserVariable(CString name);
 protected:
  int _node_type;
  // In case of terminal node (identifier)
 private:
  CString _terminal_name;
  // In case of terminal node (number)
  double _constant_value;
  // Line number relative to current function
  double _relative_line_number;
};

typedef CParseTreeTerminalNode *TPParseTreeTerminalNode;

#endif INC_CPARSETREETERMINALNODE_H