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

#ifndef INC_CSYMBOLENGINE_OPENPPL_BOARD_H
#define INC_CSYMBOLENGINE_OPENPPL_BOARD_H

#include "CVirtualSymbolEngine.h"

class CSymbolEngineOpenPPLBoard: public CVirtualSymbolEngine
{
public:
	CSymbolEngineOpenPPLBoard();
	~CSymbolEngineOpenPPLBoard();
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
	CString IdentifiersProvided();;
public:
private:
};

extern CSymbolEngineOpenPPLBoard *p_symbol_engine_open_ppl_board;

#endif INC_CSYMBOLENGINE_OPENPPL_BOARD_H