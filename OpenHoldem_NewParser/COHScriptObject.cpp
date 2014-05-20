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

#include "CFormulaParser.h"

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

bool COHScriptObject::IsStandardFunction() {
  for (int i=0; i<k_number_of_standard_functions; ++i) {
    if (_name == k_standard_function_names[i]) return true;
  }
  return false;
}

bool COHScriptObject::IsSpecialFunction() {
  if (_name == "f$debug") return true;
  if (_name == "f$test")  return true;
  if (_name == "notes")   return true;
  if (_name == "DLL")     return true;
  return false;
}

void COHScriptObject::Parse() {
  p_formula_parser->ParseSingleFormula(_name, function_text());
};