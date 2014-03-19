#ifndef INC_CFORMULAFILESPLITTER_H
#define INC_CFORMULAFILESPLITTER_H

#define _AFXDLL 

#include "afx.h"
#include "atlstr.h"

class CFormulaFileSplitter
{
public:
	CFormulaFileSplitter();
	~CFormulaFileSplitter();
public:
	CString GetNextFunctionOrList(CArchive &formula_file);
private:
	bool IsFunctionHeader(CString line_of_code);
private:
	CString _next_formula_content;
	CString _next_line;
};

#endif INC_CFORMULAFILESPLITTER_H