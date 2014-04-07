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

class CParseTreeNode
{
public:
	CParseTreeNode();
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
public:
	double Evaluate();
private:
	double EvaluateIdentifier(CString name);
	double EvaluateUnaryExpression();
	double EvaluateBinaryExpression();
	double EvaluateTernaryExpression();
	double EvaluateSibbling(TPParseTreeNode first_second_or_third_sibbling);
private:
	int _node_type;
	// In case of terminal node (identifier)
	CString _terminal_name;
	// In case of terminal node (number)
	double _constant_value;
private:
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
};

#endif INC_CPARSETREENODE_H