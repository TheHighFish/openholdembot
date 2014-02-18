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

#ifndef INC_CSYMBOLENGINELISTS_H
#define INC_CSYMBOLENGINELISTS_H

#include "CVirtualSymbolEngine.h"

extern class CSymbolEngineLists: public CVirtualSymbolEngine
{
public:
	CSymbolEngineLists();
	~CSymbolEngineLists();
public:
	// Mandatory reset-functions
	void InitOnStartup();
	void ResetOnConnection();
	void ResetOnHandreset();
	void ResetOnNewRound();
	void ResetOnMyTurn();
	void ResetOnHeartbeat();
public:
	// Public accessors
	bool EvaluateSymbol(const char *name, double *result);
	CString IdentifiersProvided();
public:
	bool IsList(const int list) 
	{ 
		if ((list >= 0) && (list < MAX_HAND_LISTS))
		{
			return _is_list[list];
		}
		ErrorListNumberOutOfRange(list);
		return false;
	}
private:
	void CalculateLists();
	// list tests
	void SetIsList(const int list, bool is_list) 
	{ 
		if ((list >= 0) && (list < MAX_HAND_LISTS))
		{
			_is_list[list] = is_list;
		}
		else
		{
			ErrorListNumberOutOfRange(list);
		}
	}
	void ErrorListNumberOutOfRange(int list);
private:
	bool _is_list[MAX_HAND_LISTS];
} *p_symbol_engine_lists;

#endif INC_CSYMBOLENGINELISTS_H