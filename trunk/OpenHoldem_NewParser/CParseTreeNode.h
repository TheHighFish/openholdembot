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
	void MakebinaryOperator(int node_type, TPParseTreeNode first_sibbling,
		TPParseTreeNode second_sibbling);
	void MakeTernaryOperator(int node_type, TPParseTreeNode first_sibbling,
		TPParseTreeNode second_sibbling, TPParseTreeNode third_sibbling);
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
	// In case of non-terminal-node
	TPParseTreeNode _first_sibbling;	// always present
	TPParseTreeNode _second_sibbling;	// for binary and ternary operators
	TPParseTreeNode _third_sibbling;	// for ternary operators only
};

#endif INC_CPARSETREENODE_H