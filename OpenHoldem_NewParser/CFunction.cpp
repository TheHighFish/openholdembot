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

#include "CAutoplayerTrace.h"
#include "CParseTreeNode.h"
#include "CPreferences.h"

CFunction::CFunction(CString *new_name, CString *new_function_text)
{
  _name = ((new_name != NULL) ? *new_name : "");
  _function_text = ((new_function_text != NULL) ? *new_function_text : "");
  _is_result_cached = false;
  _cached_result = k_undefined_zero;
  _parse_tree_node = NULL;
}

CFunction::~CFunction()
{}

void CFunction::SetParseTree(TPParseTreeNode _new_parse_tree)
{
	_parse_tree_node = _new_parse_tree;
}

double CFunction::Evaluate() {
  write_log(preferences.debug_formula(), 
    "[CFunction] Evaluating function %s\n", _name); 
  CString log_message;
  if (_is_result_cached) {
    log_message.Format("[CFunction] Function %s -> %6.3f [cached]\n", 
      _name, _cached_result);
  } else {
    if (_parse_tree_node != NULL)
    {
      p_autoplayer_trace->Indent(true);
      _cached_result = _parse_tree_node->Evaluate();
      p_autoplayer_trace->Indent(false);
      _is_result_cached = true;
      log_message.Format("[CFunction] Function %s -> %6.3f\n", 
        _name, _cached_result);
    }
    // Else: keep _cached_result as 0.0
  }
  p_autoplayer_trace->Add(LogResult());
  write_log(preferences.debug_formula(), (char*)(LPCSTR)log_message); 
  return _cached_result;
}

void CFunction::ClearCache()
{
	_cached_result = 0.0;
	_is_result_cached = false;
}

CString CFunction::Serialize() {
  CString result;
  if (_parse_tree_node != NULL) {
    result = _parse_tree_node->Serialize();
  }
  MessageBox(0, result, _name, 0);
  return result;
}

CString CFunction:: LogResult() {
  CString message;
  message.Format("%s = %8.3f %s", _name, _cached_result,
    _is_result_cached ? "[cached]" : "");
  return message;
}

bool CFunction::IsStandardFunction() {
  for (int i=0; i<k_number_of_standard_functions; i++) {
    if (strcmp(_name, k_standard_function_names[i]) == 0) {
      return true;
    }
  }
  return false;
}