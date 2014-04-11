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

#include "stdafx.h"
#include "CFunction.h"

#include "CParseTreeNode.h"

CFunction::CFunction(CString *new_name, CString *new_function_text)
{
	_name = *new_name;
	_function_text = *new_function_text;
}

CFunction::~CFunction()
{}

void CFunction::SetParseTree(TPParseTreeNode _new_parse_tree)
{
	_parse_tree_node = _new_parse_tree;
}

double CFunction::Evaluate()
{
	write_log(true, "[CFunction] Evaluating function %s\n", _name); //!!
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
