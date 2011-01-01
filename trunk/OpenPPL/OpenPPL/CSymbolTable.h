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
	CString GetStandardizedSymbolName(CString symbol);
private:
	void ClearSymbolTable();
	void AddSymbol(CString new_symbol);
private:
	// http://www.cplusplus.com/reference/stl/map/
	map<CString, bool> known_symbols;
} *p_symbol_table;

#endif INC_CSYMBOLTABLE_H