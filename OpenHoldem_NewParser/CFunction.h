#ifndef INC_CFUNCTION_H
#define INC_CFUNCTION_H

#include "CParseTreeNode.h"

class CFunction
{
public:
	CFunction(CString _new_name, CString _new_function_text);
	~CFunction();
public:
	void SetParseTree(TPParseTreeNode _new_parse_tree);
	double Evaluate();
	void ClearCache();
private:
	CString _name;
	CString _function_text;
	TPParseTreeNode _parse_tree_node;
	double _cached_result;
	bool _is_result_cached;
};

#endif INC_CFUNCTION_H