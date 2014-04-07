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

#ifndef INC_COHSCRIPTLIST_H
#define INC_COHSCRIPTLIST_H

#include "COHScriptObject.h"

class COHScriptList: COHScriptObject
{
public:
	COHScriptList(CString *new_name, CString *new_function_text);
	~COHScriptList();
public:
	void Set(CString list_member);
	// Actually Evaluate() returns true/false if our hand is in the list or not
	// but the function is inheritzed from the base-class.
	double Evaluate();
};

#endif INC_COHSCRIPTLIST_H