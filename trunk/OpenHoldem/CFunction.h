//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#ifndef INC_CFUNCTION_H
#define INC_CFUNCTION_H

#include "COHScriptObject.h"
#include "CParseTreeNode.h"

class CFunction: public COHScriptObject{
  friend class CParseTreeRotator;
  friend class CFunctionCollection;
public:
  CFunction(
      CString *new_name, 
      CString *new_function_text,
      int absolute_line);
  ~CFunction();
 public:
  void SetParseTree(TPParseTreeNode _new_parse_tree);
  void ClearCache();
 public:
  double Evaluate(bool log = false);
  bool EvaluatesToBinaryNumber();
  // For debugging output
  CString Serialize();
  void Dump();
 protected:
  // For OpenPPL, which evaluates f$preflop, ...
  // instead of f$beep, f$alli, ...
  void SetValue(double value);
 protected:
  // Used by the parse-tree-rotator
  TPParseTreeNode _parse_tree_node;
 private:
  //CString _name;
  //CString _function_text;
  double _cached_result;
  bool _is_result_cached;
};

#endif INC_CFUNCTION_H