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
#include "CDebugTab.h"

CDebugTab *p_debug_tab = NULL; 

// These arrays are global, 
// because -- when private members --
// all entries of the 2nd array (except the first element)
// magically disappeared after function_text()
// and before EvaluateAll.
// No matter what order, 
// no matter if CStrings or CString* or char*,
// no matter if CArray or normal arrays.
// But now it works. ;-)
CString _expression_texts[kMaxSizeOfDebugTab];
TPParseTreeNode _expressions[kMaxSizeOfDebugTab];

CDebugTab::CDebugTab() {
  // Name is alwayss the same and required for function lookup
  _name = "f$debug";
  _number_of_expressions = 0;
  Clear();
}

CDebugTab::~CDebugTab() {
  Clear();
}

CString CDebugTab::EvaluateAll() {
  CString all_results;
  for (int i=0; i<_number_of_expressions; ++i) {
    double result = _expressions[i]->Evaluate();
    CString current_line;
    assert(_expression_texts[i] != "");
    current_line.Format("%10.3f = %s\n", result,
      _expression_texts[i]);
    all_results += current_line;
  }
  return all_results;
}

void CDebugTab::Clear() {
  assert(_number_of_expressions >= 0);
  assert(_number_of_expressions <= kMaxSizeOfDebugTab);
  for (int i=0; i<kMaxSizeOfDebugTab; ++i) {
    _expression_texts[i] = "";
  }
  _number_of_expressions = 0;
}

void CDebugTab::AddExpression(CString expression_text, TPParseTreeNode expression) {
  CString text = expression_text.TrimLeft().TrimRight();
  if (text == "") return;
  if (expression == NULL) return;
  if (_number_of_expressions >= kMaxSizeOfDebugTab) {
    bool too_many_expressions_for_debug_tab = false;
    assert(too_many_expressions_for_debug_tab);
    return;
  }
  _expression_texts[_number_of_expressions] = CString(text); 
  _expressions[_number_of_expressions] = expression;
  ++_number_of_expressions;
}

CString CDebugTab::function_text() {
  return _function_text; // !!! because we need it for parsing
  /*
  // Clean function text,
  // nicely formatted without results
  CString function_text;
  for (int i=0; i<_number_of_expressions; ++i) {
    function_text += " = ";
    if (_expression_texts[i] != "") {
      function_text += _expression_texts[i];
    }
    function_text += "\n";
  }
  return function_text;
  */
}