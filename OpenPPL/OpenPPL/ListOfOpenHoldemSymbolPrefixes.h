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