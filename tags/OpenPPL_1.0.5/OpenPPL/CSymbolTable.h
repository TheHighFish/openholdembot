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