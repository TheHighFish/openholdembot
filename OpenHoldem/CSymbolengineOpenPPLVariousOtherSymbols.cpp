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
#include "CSymbolEngineOpenPPLVariousOtherSymbols.h"

#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CStringMatch.h"
#include "MagicNumbers.h"

CSymbolEngineOpenPPLVariousOtherSymbols *p_symbol_engine_open_ppl_various_other_symbols = NULL;

CSymbolEngineOpenPPLVariousOtherSymbols::CSymbolEngineOpenPPLVariousOtherSymbols()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	//
	// This engine does not use any other engines.
}

CSymbolEngineOpenPPLVariousOtherSymbols::~CSymbolEngineOpenPPLVariousOtherSymbols()
{}

void CSymbolEngineOpenPPLVariousOtherSymbols::InitOnStartup()
{
	ResetOnConnection();
}

void CSymbolEngineOpenPPLVariousOtherSymbols::ResetOnConnection()
{}

void CSymbolEngineOpenPPLVariousOtherSymbols::ResetOnHandreset()
{}

void CSymbolEngineOpenPPLVariousOtherSymbols::ResetOnNewRound()
{}

void CSymbolEngineOpenPPLVariousOtherSymbols::ResetOnMyTurn()
{}

void CSymbolEngineOpenPPLVariousOtherSymbols::ResetOnHeartbeat()
{}

bool CSymbolEngineOpenPPLVariousOtherSymbols::EvaluateSymbol(const char *name, double *result)
{
	// Symbol of a different symbol-engine
	return false;
}