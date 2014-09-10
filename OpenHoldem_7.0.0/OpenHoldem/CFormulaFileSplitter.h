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

#ifndef INC_CFORMULAFILESPLITTER_H
#define INC_CFORMULAFILESPLITTER_H

#define _AFXDLL 

#include "afx.h"
#include "atlstr.h"

class CFormulaFileSplitter {
 public:
  CFormulaFileSplitter();
  ~CFormulaFileSplitter();
 public:
  void ScanForNextFunctionOrList(CArchive &formula_file);
  // For debug-tab, which has to parse line by line
  void SetInput(CString line_of_debug_tab);
  void InitNewParse();
 public:
  CString GetFunctionHeader()   { return _function_header; }
  CString GetFunctionText()     { return _function_text; }
 public:
   int starting_line_of_current_function() { return _starting_line_of_current_function; }
 private:
  bool IsFunctionHeader(CString line_of_code);
 private:
  CString _function_header;
  CString _function_text;
  CString _next_line;
 private:
  bool _first_function_processed;
  int _total_line_processed;
  int _starting_line_of_current_function;
};

#endif INC_CFORMULAFILESPLITTER_H