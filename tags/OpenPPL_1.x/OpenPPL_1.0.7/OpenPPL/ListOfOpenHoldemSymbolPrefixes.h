#ifndef INC_LISTOFOPENHOLDEMSYMBOLPREFIXES_H
#define INC_LISTOFOPENHOLDEMSYMBOLPREFIXES_H

#include <afx.h>

extern class ListOfOpenHoldemSymbolPrefixes
{
public:
	ListOfOpenHoldemSymbolPrefixes();
	~ListOfOpenHoldemSymbolPrefixes();
public:
	bool LooksLikeOpenHoldemSymbol(CString symbol);
} *p_list_of_openholdem_symbol_prefixes;

#endif INC_LISTOFOPENHOLDEMSYMBOLPREFIXES_H