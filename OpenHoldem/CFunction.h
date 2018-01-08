//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
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
    CString new_name,
    CString new_function_text);
  CFunction(
    CString new_name, 
    CString new_function_text,
    CString file_path,
    int absolute_line);
  ~CFunction();
 public:
  double Evaluate(bool log = false);
  bool EvaluatesToBinaryNumber();
  void ClearCache();
 public:
  virtual void Parse();
  void SetParseTree(TPParseTreeNode _new_parse_tree);
  virtual void MarkAsImportedFromShankyPPL() { _imported_from_shanky_ppl = true; }
  virtual bool ImportedFromShankyPPL() { return _imported_from_shanky_ppl; }
 public:
  // For debugging output
  CString Serialize();
  void Dump();
  static CFunction* CurrentlyEvaluatedFunction() {
    return _currently_evaluated_function;
  }
 protected:
  // For OpenPPL, which evaluates f$preflop, ...
  // instead of f$beep, f$alli, ...
  void SetValue(double value);
 protected:
  // To be used by CFunctionCollection::parseAll()
   virtual bool EmptyParseTree() {
     return _parse_tree_node == NULL;
   }
 protected:
  // Used by the parse-tree-rotator
  TPParseTreeNode _parse_tree_node;
 private:
  double _cached_result;
  bool _is_result_cached;
  bool _imported_from_shanky_ppl;
 private:
  static CFunction* _currently_evaluated_function;
  CFunction* _previously_evaluated_function;
};

#endif INC_CFUNCTION_H