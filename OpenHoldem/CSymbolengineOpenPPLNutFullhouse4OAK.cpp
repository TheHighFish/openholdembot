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
#include "CSymbolEngineOpenPPLNutFullHouse4OAK.h"

#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CStringMatch.h"
#include "MagicNumbers.h"

CSymbolEngineOpenPPLNutFullHouse4OAK *p_symbol_engine_open_ppl_nut_fullhouse_4oak = NULL;

CSymbolEngineOpenPPLNutFullHouse4OAK::CSymbolEngineOpenPPLNutFullHouse4OAK()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	//
	// This engine does not use any other engines.
}

CSymbolEngineOpenPPLNutFullHouse4OAK::~CSymbolEngineOpenPPLNutFullHouse4OAK()
{}

void CSymbolEngineOpenPPLNutFullHouse4OAK::InitOnStartup()
{
	ResetOnConnection();
}

void CSymbolEngineOpenPPLNutFullHouse4OAK::ResetOnConnection()
{}

void CSymbolEngineOpenPPLNutFullHouse4OAK::ResetOnHandreset()
{}

void CSymbolEngineOpenPPLNutFullHouse4OAK::ResetOnNewRound()
{}

void CSymbolEngineOpenPPLNutFullHouse4OAK::ResetOnMyTurn()
{}

void CSymbolEngineOpenPPLNutFullHouse4OAK::ResetOnHeartbeat()
{}

bool CSymbolEngineOpenPPLNutFullHouse4OAK::EvaluateSymbol(const char *name, double *result)
{
	// Symbol of a different symbol-engine
	return false;
}