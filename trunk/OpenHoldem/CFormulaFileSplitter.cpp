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

#include "stdafx.h"
#include "CFormulaFileSplitter.h"

#include "OH_MessageBox.h"

// Format of a formula file:
// * Date and time (OH-script only)
//     ##2014-02-09 23:16:55##
// * Notes (OH-script only)
//     ##Notes##
//     Comments below
// * DLL
//     ##dll##
//     Filename
// * Lists
//     ##list123##
//     AA KK ...  
// * formulas
//     ##f$alli##
//     C-style expressions or Shanky-style WHEN-conditions
//     Somewhat special cases: f$test and f$debug

#undef DEBUG_FORMULA_FILESPLITTER

CFormulaFileSplitter::CFormulaFileSplitter() {
  InitNewParse();
}

CFormulaFileSplitter::~CFormulaFileSplitter() {
}

void CFormulaFileSplitter::InitNewParse() {
  _first_function_processed = false; 
  _total_line_processed = 0;
  _starting_line_of_current_function = 0;
}

// Returns the next function (including header),
// i.e. everything up to the second-next-function or end of file.
void CFormulaFileSplitter::ScanForNextFunctionOrList(CArchive &formula_file) {
  // Function-header is the first line, 
  // (usually the last line of last scan)
  // rest is content
  _function_header = _next_line;
  _function_text = ""; 
  while (true) {
    try {
      if (!formula_file.ReadString(_next_line)) {
	      break;
      }
    }
    catch (CException *e) {
	    // break;
    }
    ++_total_line_processed;
    // Avoid problems with "empty" lines before first function header
    // that contain spaces.
    _next_line.TrimRight();
#ifdef DEBUG_FORMULA_FILESPLITTER
    //printf("[CFormulaFileSplitter] next line: %s\n", _next_line);
#endif
    if (IsFunctionHeader(_next_line)
        && _first_function_processed) {
      // Next function found
      // Only continue, if we found the first one
      //
      // In case of break: keep that function-header for the next query
      _starting_line_of_current_function = _total_line_processed;
      break;
	  }
    if (_function_header.IsEmpty() || (_function_header.Find('#') < 0)) {
      // Escpecially meant to catch OpenGeeks newlines 
      // (which are not empty) at the beginning of the file.
      // Other cases can't happen, as we search for ## 
      // when looking for the next function-header. 
      _function_header = _next_line;
    } else {
      _first_function_processed = true;
      // Add non-function-header (content) to the functions body
      _function_text += _next_line;
      _function_text += "\n";
    }
  } 
#ifdef DEBUG_FORMULA_FILESPLITTER
  //printf("[CFormulaFileSplitter] next function: %s\n", _formula_content);
  OH_MessageBox_Interactive(_function_text, "Function", 0);
#endif
}

// A function header (or list header) starts with ##
bool CFormulaFileSplitter::IsFunctionHeader(CString line_of_code)
{
	return ((line_of_code.GetAt(0) == '#')
		&& (line_of_code.GetAt(1) == '#'));
}

void CFormulaFileSplitter::SetInput(CString line_of_debug_tab) {
  _function_text = line_of_debug_tab;
}