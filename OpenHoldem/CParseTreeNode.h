//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose: Base class for Operators and terminal nodes 
//   (identifiers, numbers)
//
//*******************************************************************************

#ifndef INC_CPARSETREENODE_H
#define INC_CPARSETREENODE_H

class CParseTreeNode;

typedef CParseTreeNode *TPParseTreeNode;

class CParseTreeNode {
  friend class CParseTreeOperatorNode;
  friend class CParseTreeTerminalNode;
  friend class CFormulaParser;
  friend class CParseTreeRotator;
 public:
  CParseTreeNode(int relative_line_number);
  ~CParseTreeNode();
 public:
  virtual double Evaluate(bool log = false);
  virtual CString EvaluateToString(bool log = false);
  virtual bool EvaluatesToBinaryNumber();
 public:
  // For debugging output
  virtual CString Serialize();
 protected:
  int _node_type;
  // In case of terminal node (identifier)
 private:
  // Line number relative to current function
  double _relative_line_number;
};

#endif INC_CPARSETREENODE_H