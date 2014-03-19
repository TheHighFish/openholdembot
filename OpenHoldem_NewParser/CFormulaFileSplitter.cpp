#include "stdafx.h"
#include "CFormulaFileSplitter.h"

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

CFormulaFileSplitter::CFormulaFileSplitter()
{
	_next_line = "";
}

CFormulaFileSplitter::~CFormulaFileSplitter()
{}

// Returns the next function (including header),
// i.e. everything up to the second-next-function or end of file.
CString CFormulaFileSplitter::GetNextFunctionOrList(CArchive &formula_file)
{
#ifdef _DEBUG
	/*printf("[CFormulaFileSplitter] %s\n", 
		formula_file.IsLoading() ? "Loading\n" :
		formula_file.IsStoring() ? "Storing\n" :
		"Error");*/
#endif
	// Init new formula as empty + old _next_line (function-header)
	_next_formula_content = _next_line;
	if (_next_line != "")
	{	
		_next_formula_content += "\n";
		// Otherwise: start of file: keep _next_formula_content empty.
	}
	while (true)
	{
		try
		{
			if (!formula_file.ReadString(_next_line))
			{
				break;
			}
		}
		catch (CException *e)
		{
			//break;
		}
#ifdef _DEBUG
		//printf("[CFormulaFileSplitter] next line: %s\n", _next_line);
#endif
		if (IsFunctionHeader(_next_line) 
			&& (_next_formula_content != "")
			&& (_next_formula_content != "\n"))
		{
			// Next function found
			// Only continue, if the "next" one is the first one
			//
			// In case of break: keep that function-header for the next query
			break;
		}
		// Add non-function-header (content) to the functions body
		_next_formula_content += _next_line;
		_next_formula_content += "\n";

	} 
#ifdef _DEBUG
		//printf("[CFormulaFileSplitter] next function: %s\n", _next_formula_content);
#endif
	return _next_formula_content;
}

// A function header (or list header) starts with ##
bool CFormulaFileSplitter::IsFunctionHeader(CString line_of_code)
{
	return ((line_of_code.GetAt(0) == '#')
		&& (line_of_code.GetAt(1) == '#'));
}
