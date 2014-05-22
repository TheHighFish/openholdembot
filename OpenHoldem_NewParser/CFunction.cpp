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

CFunction::CFunction(CString *new_name, CString *new_function_text) {
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

double CFunction::Evaluate(bool log /* = false */) {
  write_log(preferences.debug_formula(), 
    "[CFunction] Evaluating function %s\n", _name); 
  if (_is_result_cached) {
    p_autoplayer_trace->Add(_name, _cached_result);  
  } else {
    if (_parse_tree_node != NULL)
    {
      int log_line = p_autoplayer_trace->Add(_name);
      p_autoplayer_trace->Indent(true);
      _cached_result = _parse_tree_node->Evaluate();
      _is_result_cached = true;
      p_autoplayer_trace->BackPatchValue(log_line, _cached_result);
      p_autoplayer_trace->Indent(false);
    }
    // Else: keep _cached_result as 0.0
  }
  //p_autoplayer_trace->Add(LogResult());
  CString log_message;
  log_message.Format("[CFunction] Function %s -> %6.3f\n", 
    _name, _cached_result);
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
  //MessageBox(0, result, _name, 0);
  return result;
}


