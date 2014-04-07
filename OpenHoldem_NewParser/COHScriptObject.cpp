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
#include "COHScriptObject.h"

COHScriptObject::COHScriptObject()
{
	_name = "";
	_function_text = "";
}

COHScriptObject::COHScriptObject(CString *new_name, CString *new_function_text)
{
	_name = *new_name;
	_function_text = *new_function_text;
}

COHScriptObject::~COHScriptObject()
{}

double COHScriptObject::Evaluate()
{
	bool this_method_should_always_get_overwritten_and_never_called = false;
	assert(this_method_should_always_get_overwritten_and_never_called);
	return 0.0;
}

