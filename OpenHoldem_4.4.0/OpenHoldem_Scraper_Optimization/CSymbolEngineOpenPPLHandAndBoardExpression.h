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

#ifndef INC_CSYMBOLENGINE_HAND_AND_BOARD_EXPRESSION_H
#define INC_CSYMBOLENGINE_HAND_AND_BOARD_EXPRESSION_H

#include "CVirtualSymbolEngine.h"

extern class CSymbolEngineOpenPPLHandAndBoardExpression: public CVirtualSymbolEngine
{
public:
	CSymbolEngineOpenPPLHandAndBoardExpression();
	~CSymbolEngineOpenPPLHandAndBoardExpression();
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
	bool ProcessQuery(CString hand_expression);
} *p_symbol_engine_open_ppl_hand_and_board_expression;

#endif INC_CSYMBOLENGINE_HAND_AND_BOARD_EXPRESSION_H