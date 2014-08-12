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

#include "CAutoplayer.h"
#include "CAutoplayerTrace.h"
#include "CParseTreeNode.h"
#include "CPreferences.h"
#include "OH_MessageBox.h"

// Global counter to detect potentially endless recursion_depth
int recursion_depth = 0;
const int kMaxRecursionDepth = 100;

CFunction::CFunction(
    CString *new_name, 
    CString *new_function_text,
    int absolute_line) {
  _name = ((new_name != NULL) ? *new_name : "");
  _function_text = ((new_function_text != NULL) ? *new_function_text : "");
  _starting_line_of_function = absolute_line;
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
  // Check recursion depth of DoCalcF$symbol 
  // to detect a recursive formula.
  // Decrease recursion_depth on every function exit!
  recursion_depth++;
  if (recursion_depth > kMaxRecursionDepth) {
    CString error_message = CString(
      "Recursion to deep.\n"
      "Probably endless.\n"
      "Stopping autoplayer.\n"
      "\n"
      "Last function: ") + _name;
	OH_MessageBox_Error_Warning(error_message);
	p_autoplayer->EngageAutoplayer(false);
	++recursion_depth;
	return k_undefined_zero;
  }
  // Result already cached
  if (_is_result_cached) {
    if (log) {
      write_log(preferences.debug_auto_trace(),
        "[CFunction] %s -> %.3f [cached]\n", _name, _cached_result);
      p_autoplayer_trace->Add(_name, _cached_result);  
    }
    // Keep cached result: do nothing
  } else {
    // Evaluate symbol
    if (_parse_tree_node != NULL)
    {
      int log_line;
      if (log) {
        // Reserve a line in the log, without result ATM
        log_line = p_autoplayer_trace->Add(_name);
      }
      p_autoplayer_trace->Indent(true);
      _cached_result = _parse_tree_node->Evaluate(log);
      _is_result_cached = true;
      if (log) {
        p_autoplayer_trace->BackPatchValueAndLine(
          log_line, _cached_result, _starting_line_of_function);
      }
      p_autoplayer_trace->Indent(false);
    }
    // Else: keep _cached_result as 0.0
  }
  --recursion_depth;
  return _cached_result;
}

bool CFunction::EvaluatesToBinaryNumber() {
  if (_parse_tree_node == NULL) return false;
  return _parse_tree_node->EvaluatesToBinaryNumber();
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

void CFunction::Dump() {
  CString data;
  data.Format("%s %s %.3f %i\n",
    _name,
    (_is_result_cached ? "[cached]" : "[not chached]"),
    _cached_result,
    _parse_tree_node);
  write_log(preferences.debug_auto_trace(), (char*)(LPCTSTR)data);
}


