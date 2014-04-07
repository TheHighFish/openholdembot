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

#ifndef INC_CFUNCTION_H
#define INC_CFUNCTION_H

#include "COHScriptObject.h"
#include "CParseTreeNode.h"

class CFunction: public COHScriptObject
{
public:
	CFunction(CString *new_name, CString *new_function_text);
	~CFunction();
public:
	void SetParseTree(TPParseTreeNode _new_parse_tree);
	double Evaluate();
	void ClearCache();
private:
	//CString _name;
	//CString _function_text;
	TPParseTreeNode _parse_tree_node;
	double _cached_result;
	bool _is_result_cached;
};

#endif INC_CFUNCTION_H