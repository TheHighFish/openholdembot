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

#include "stdafx.h"
#include "CSymbolEngineOpenPPLBoard.h"

#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CStringMatch.h"
#include "CSymbolEnginePokerval.h"
#include "MagicNumbers.h"
#include "NumericalFunctions.h"

CSymbolEngineOpenPPLBoard *p_symbol_engine_open_ppl_board = NULL;

CSymbolEngineOpenPPLBoard::CSymbolEngineOpenPPLBoard()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	assert(p_symbol_engine_pokerval != NULL);
}

CSymbolEngineOpenPPLBoard::~CSymbolEngineOpenPPLBoard()
{}

void CSymbolEngineOpenPPLBoard::InitOnStartup()
{
	ResetOnConnection();
}

void CSymbolEngineOpenPPLBoard::ResetOnConnection()
{}

void CSymbolEngineOpenPPLBoard::ResetOnHandreset()
{
	_wheel_possible = false;
}

void CSymbolEngineOpenPPLBoard::ResetOnNewRound()
{}

void CSymbolEngineOpenPPLBoard::ResetOnMyTurn()
{
	_wheel_possible = WheelPossible();
}

void CSymbolEngineOpenPPLBoard::ResetOnHeartbeat()
{}

bool CSymbolEngineOpenPPLBoard::WheelPossible()
{
	const int kRankbits_A2345 = 0x1F; // 0b111110
	return (bitcount(
		p_symbol_engine_pokerval->rankbitscommon() & kRankbits_A2345) 
			>= 3);
}

bool CSymbolEngineOpenPPLBoard::EvaluateSymbol(const char *name, double *result)
{
	if (memcmp(name, "wheelpossible", 13)==0 && strlen(name)==13)	
	{
		*result = _wheel_possible;
		return true;
	}
	// Symbol of a different symbol-engine
	return false;
}