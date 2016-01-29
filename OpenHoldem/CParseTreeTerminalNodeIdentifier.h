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

#ifndef INC_CPARSETREETERMINALNODEIDENTIFIER_H
#define INC_CPARSETREETERMINALNODEIDENTIFIER_H

#include "CParsetreeTerminalNode.h"

class CParseTreeTerminalNodeIdentifier: public CParseTreeTerminalNode {
  friend class CFormulaParser;
  friend class CParseTreeRotator;
 public:
  CParseTreeTerminalNodeIdentifier(int relative_line_number, CString name);
  ~CParseTreeTerminalNodeIdentifier();
 public:
  virtual double Evaluate(bool log = false);
  virtual CString EvaluateToString(bool log = false);
  virtual bool EvaluatesToBinaryNumber();
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
 protected:
  CString _terminal_name;
  // Line number relative to current function
  double _relative_line_number; //!!!!!???? double wrong, not only here
};

typedef CParseTreeTerminalNodeIdentifier *TPParseTreeTerminalNodeIdentifier;

#endif INC_CPARSETREETERMINALNODEIDENTIFIER_H