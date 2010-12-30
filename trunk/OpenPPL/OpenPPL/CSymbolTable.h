#ifndef INC_CSYMBOLTABLE_H
#define INC_CSYMBOLTABLE_H

extern class CSymbolTable
{
public:
	CSymbolTable();
	~CSymbolTable();
public:
	void AddSymbolsFromFile(CString filename);
	bool IsOpenPPLSymbol(CString symbol);
private:
	void ClearSymbolTable();
	void AddSymbol(CString symbol);
} *p_symbol_table;

#endif INC_CSYMBOLTABLE_H