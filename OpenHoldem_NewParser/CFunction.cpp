#include "stdafx.h"
#include "CFunction.h"

#include "CParseTreeNode.h"

CFunction::CFunction(CString _new_name, CString _new_function_text)
{
	_name = _new_name;
	_function_text = _new_function_text;
	_parse_tree_node = NULL;
	ClearCache();
}

CFunction::~CFunction()
{}

void CFunction::SetParseTree(TPParseTreeNode _new_parse_tree)
{
	_parse_tree_node = _new_parse_tree;
}

double CFunction::Evaluate()
{
	if (!_is_result_cached)
	{
		if (_parse_tree_node != NULL)
		{
			_cached_result = _parse_tree_node->Evaluate();
			_is_result_cached = true;
		}
		// Else: keep _cached_result as 0.0
	}
	return _cached_result;
}

void CFunction::ClearCache()
{
	_cached_result = 0.0;
	_is_result_cached = false;
}
