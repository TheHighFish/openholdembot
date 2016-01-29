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

#ifndef INC_CPARSETREEOPERATORNODE_H
#define INC_CPARSETREEOPERATORNODE_H

#include "CParseTreeNode.h"

class CParseTreeOperatorNode: public CParseTreeNode {
  friend class CFormulaParser;
  friend class CParseTreeRotator;
 public:
  CParseTreeOperatorNode(int relative_line_number);
  ~CParseTreeOperatorNode();
 public:
  void MakeUnaryOperator(int node_type, TPParseTreeNode first_sibbling);
  void MakeBinaryOperator(int node_type, TPParseTreeNode first_sibbling,
    TPParseTreeNode second_sibbling);
  void MakeTernaryOperator(int node_type, TPParseTreeNode first_sibbling,
    TPParseTreeNode second_sibbling, TPParseTreeNode third_sibbling);
 public:
  void MakeRaiseToAction(TPParseTreeNode raise_by_amount_in_big_blinds);
  void MakeRaiseByAction(TPParseTreeNode raise_by_amount_in_big_blinds);
  // Values to be expected in the range (0..100] (or more), not (0..1]
  void MakeRaiseByPercentagedPotsizeAction(TPParseTreeNode raise_by_amount_percentage);
  void MakeWhenCondition(TPParseTreeNode condition);
 public:
  //double Evaluate(bool log = false);
  //CString EvaluateToString(bool log = false);
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
  //bool IsBinaryIdentifier();
 private:
  bool SecondSibblingIsUserVariableToBeSet();
  void SetUserVariable(CString name); //?????
 protected:
  int _node_type;
 private:
  //CString _terminal_name;
  // In case of terminal node (number)
  //double _constant_value;
  // Line number relative to current function
  //double _relative_line_number;
};

typedef CParseTreeOperatorNode *TPParseTreeOperatorNode;

#endif INC_CPARSETREEOPERATORNODE_H