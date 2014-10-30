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

/*
##f$OpenPPL_IsFinalTable##
##f$OpenPPL_Others##
##f$OpenPPL_FloatingPointRandom_0_100##
##f$OpenPPL_IntegerRandom_0_100##
##f$OpenPPL_Random##*/

/* technical stuff, probably to be done elsewhere
##f$OpenPPL_Decision##
##f$RaiseByBigBlinds##
##f$RaiseByPercentagedPotsize##
##f$OpenPPL_DecisionIsPercentagedPotsizedBet##
##f$OpenPPL_DecisionIsFixedBetInBigBlinds##
##f$OpenPPL_DecisionIsBetPotOrBetHalfPotOrBetMax##
##f$OpenPPL_DecisionIsFixedAction##
##f$OpenPPL_RememberToSitoutIfNecessary##

##f$OpenPPL_TimeToInitMemorySymbols##
##f$OpenPPL_HandResetHappened##
##f$OpenPPL_ResetUserVariablesOnHandReset##
##f$OpenPPL_GameStateChanged##
##f$OpenPPL_RememberCurrentGameState##
##f$OpenPPL_InitMemorySymbols##
##f$OpenPPL_HaveBackdoorStraightDraw##
*/

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

int CSymbolEngineOpenPPLVariousOtherSymbols::IntegerRandom_0_100()
{
	// Random does not get pre-calculated and stored
	// as it should get evaluated each time new.
	return Random(100);
}

bool CSymbolEngineOpenPPLVariousOtherSymbols::EvaluateSymbol(const char *name, double *result)
{
	if (memcmp(name, "others", 6)==0 && strlen(name)==6)	
	{
		// Always true
		*result = true;
		return true;
	}
	else if (memcmp(name, "random", 6)==0 && strlen(name)==6)	
	{
		*result = IntegerRandom_0_100();
		return true;
	}
	// There was a third OpenPPL-symbol tagged as "various others": isfinaltable
	// but that already is an OpenHoldem symbol, so nothing to do
	//
	// Symbol of a different symbol-engine
	return false;
}