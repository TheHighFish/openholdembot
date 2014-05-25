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

#ifndef INC_CDEBUGTAB_H
#define INC_CDEBUGTAB_H

#include "COHScriptObject.h"
#include "CParseTreeNode.h"

const int kMaxSizeOfDebugTab = 50;

class CDebugTab: public COHScriptObject{
  friend class CFormulaParser;
 public:
  CDebugTab();
  ~CDebugTab();
 public:
  // f$debug is not a normal function
  // We always return zero when evaluated.
  double Evaluate()   { return k_undefined_zero; }
  // To be used by the formula-editor
  CString EvaluateAll(); 
  CString function_text();
 protected:
  void Clear();
  void AddExpression(CString expression_text, TPParseTreeNode expression);
  int _number_of_expressions;
};

extern CDebugTab *p_debug_tab;

#endif INC_CDEBUGTAB_H

