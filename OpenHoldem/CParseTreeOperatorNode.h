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

#ifndef INC_CPARSETREEOPERATORNODE_H
#define INC_CPARSETREEOPERATORNODE_H

#include "CParseTreeNode.h"

class CParseTreeOperatorNode: public CParseTreeNode {
  friend class CFormulaParser;
  friend class CParseTreeRotator;
 public:
  CParseTreeOperatorNode(int relative_line_number);
  virtual ~CParseTreeOperatorNode();
 public:
  void MakeUnaryOperator(int node_type, TPParseTreeNode first_sibbling);
  void MakeBinaryOperator(int node_type, TPParseTreeNode first_sibbling,
    TPParseTreeNode second_sibbling);
  void MakeTernaryOperator(int node_type, TPParseTreeNode first_sibbling,
    TPParseTreeNode second_sibbling, TPParseTreeNode third_sibbling);
 public:
  void MakeWhenCondition(TPParseTreeNode condition);
 public:
  bool EvaluatesToBinaryNumber();
 public:
  // For debugging output
  CString Serialize();
 public:
  double Evaluate(bool log /* = false */);
  CString EvaluateToString(bool log /* = false */);
 private:
  double EvaluateUnaryExpression(bool log_symbol);
  double EvaluateBinaryExpression(bool log);
  double EvaluateTernaryExpression(bool log);
  double EvaluateSibbling(TPParseTreeNode first_second_or_third_sibbling, bool log);
 private:
  void ErrorDivisionByZero(int relative_line, TPParseTreeNode second_sibbling);
 private:
  bool SecondSibblingIsUserVariableToBeSet();
};

typedef CParseTreeOperatorNode *TPParseTreeOperatorNode;

#endif INC_CPARSETREEOPERATORNODE_H