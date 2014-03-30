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


#ifndef INC_CSYMBOLTABLE_H
#define INC_CSYMBOLTABLE_H

#include <map>
using namespace std;

extern class CSymbolTable
{
public:
	CSymbolTable();
	~CSymbolTable();
public:
	void AddSymbolsFromFile(CString filename);
	bool IsOpenPPLSymbol(CString symbol);
	CString GetSymbolNameWithcorrectCases(CString symbol);
	bool GenerationOfSymboltableInProgress();
private:
	void ClearSymbolTable();
	void AddSymbol(CString new_symbol);
	CString GetSymbolNameWithCorrectPrefix(CString symbol);
	CString GetLookupKey(CString symbol);
private:
	// http://www.cplusplus.com/reference/stl/map/
	typedef map<CString, CString> CSMap;
	CSMap known_symbols;
	bool _generation_of_symboltable_in_progress;
} *p_symbol_table;

#endif INC_CSYMBOLTABLE_H