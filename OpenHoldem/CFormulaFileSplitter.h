//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Extract functions from an input-file
//   and put them into the function-collection (not yet parsed)
//
//******************************************************************************

#ifndef INC_CFORMULAFILESPLITTER_H
#define INC_CFORMULAFILESPLITTER_H

#include "afx.h"
#include "atlstr.h"
#include "COHScriptObject.h"

class CFormulaFileSplitter {
 public:
  CFormulaFileSplitter();
  ~CFormulaFileSplitter();
 public:
  void SplitFile(CArchive &formula_file);
 private:
  void SkipShankyOptionSettings(CArchive &formula_file);
 private:
  COHScriptObject* GetNextObject(CArchive &formula_file);
  void ScanForNextFunctionOrList(CArchive &formula_file);
  CString ExtractFunctionName(const CString function_header);
 private:
  inline bool IsFunctionHeader(CString line_of_code);
  inline bool IsShankyFunction(CString line_of_code);
  void SanityChecksForWrongFileTypes();
 private:
  CString _function_name;
  CString _function_text;
  CString _next_line;
 private:
  bool _first_function_processed;
  int _total_lines_processed;
  int _starting_line_of_next_function;
  bool _splitting_shanky_ppl;
};

#endif INC_CFORMULAFILESPLITTER_H