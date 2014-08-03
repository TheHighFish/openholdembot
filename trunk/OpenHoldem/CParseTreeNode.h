//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*****************************************************************************
//
// Purpose:
//
//*****************************************************************************

#ifndef INC_CPARSETREENODE_H
#define INC_CPARSETREENODE_H

class CParseTreeNode;

typedef CParseTreeNode *TPParseTreeNode;

class CParseTreeNode {
  friend class CFormulaParser;
  friend class CParseTreeRotator;
 public:
  CParseTreeNode(int relative_line_number);
  ~CParseTreeNode();
 public:
  void MakeConstant(double value);
  void MakeIdentifier(CString name);
  void MakeUnaryOperator(int node_type, TPParseTreeNode first_sibbling);
  void MakeBinaryOperator(int node_type, TPParseTreeNode first_sibbling,
    TPParseTreeNode second_sibbling);
  void MakeTernaryOperator(int node_type, TPParseTreeNode first_sibbling,
    TPParseTreeNode second_sibbling, TPParseTreeNode third_sibbling);
 public:
  void MakeAction(int action_constant);
  void MakeRaiseByAction(TPParseTreeNode raise_by_amount_in_big_blinds);
  // Values to be expected in the range (0..100] (or more), not (0..1]
  void MakeRaiseByPercentagedPotsizeAction(TPParseTreeNode raise_by_amount_percentage);
  void MakeUserVariableDefinition(CString uservariable);
  void MakeWhenCondition(TPParseTreeNode condition);
 public:
  double Evaluate(bool log = false);
  CString EvaluateToString(bool log = false);
  bool EvaluatesToBinaryNumber();
 public:
  // For debugging output
  CString Serialize();
 public:
  TPParseTreeNode GetRightMostSibbling();
  TPParseTreeNode GetLeftMostSibbling();
  void SetRightMostSibbling(TPParseTreeNode sibbling);
  void SetLeftMostSibbling(TPParseTreeNode sibbling);
 public:
  // Might be called by CSymbolEngineMemorySymbols
  // to evaluate right-hand-side expressions
  static double EvaluateIdentifier(CString name, bool log);
 private:
  double EvaluateUnaryExpression(bool log_symbol);
  double EvaluateBinaryExpression(bool log);
  double EvaluateTernaryExpression(bool log);
  double EvaluateSibbling(TPParseTreeNode first_second_or_third_sibbling, bool log);
 private:
  bool IsAnyKindOfWhenCondition();
  bool IsWhenConditionWithAction();
  bool IsOpenEndedWhenCondition();
 private:
  bool IsBinaryIdentifier();
 protected:
  int _node_type;
  // In case of terminal node (identifier)
 protected:
  // Sibblings: pointers to the operands of expressions
  // First: always present, as long as non-terminal-node
  TPParseTreeNode _first_sibbling;	
  // Second: for binary and ternary operators
  // Holds the 2nd operand for binary expressions
  // Holds the "then"-part of ternary-expressions
  // Holds the "then"-part of WHEN-conditions
  // Holds the continuation to the next when-condition after user-vartiables
  TPParseTreeNode _second_sibbling;	
  // Third: for ternary operators only
  // Holds the "else"-part of ternary-expressions
  // Holds the next when-condition in when-condition-sequences
  TPParseTreeNode _third_sibbling;	
 private:
  CString _terminal_name;
  // In case of terminal node (number)
  double _constant_value;
  // Line number relative to current function
  double _relative_line_number;
};

#endif INC_CPARSETREENODE_H