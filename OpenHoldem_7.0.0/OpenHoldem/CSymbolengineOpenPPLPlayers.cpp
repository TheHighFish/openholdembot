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
#include "CSymbolEngineOpenPPLPlayers.h"

#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CStringMatch.h"
#include "MagicNumbers.h"

/*
##f$OpenPPL_PlayersAtStartOfThisRound##
##f$OpenPPL_Opponents##
##f$OpenPPL_OpponentsAtTable##
##f$OpenPPL_NumberOfOpponentsAllin##
##f$OpenPPL_Casino_Moves_Allinbets_Immediatelly_To_The_Mainpot##
##f$OpenPPL_OpponentIsAllin##
##f$OpenPPL_Opponents_AllinBits##
##f$OpenPPL_OpponentsLeft##
##f$OpenPPL_OpponentsOnFlop##
##f$OpenPPL_OpponentsOnTurn##
##f$OpenPPL_OpponentsOnRiver##
##f$OpenPPL_OpponentsWithLowerStack##*/

CSymbolEngineOpenPPLPlayers *p_symbol_engine_open_ppl_players = NULL;

CSymbolEngineOpenPPLPlayers::CSymbolEngineOpenPPLPlayers()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	//
	// This engine does not use any other engines.
}

CSymbolEngineOpenPPLPlayers::~CSymbolEngineOpenPPLPlayers()
{}

void CSymbolEngineOpenPPLPlayers::InitOnStartup()
{
	ResetOnConnection();
}

void CSymbolEngineOpenPPLPlayers::ResetOnConnection()
{}

void CSymbolEngineOpenPPLPlayers::ResetOnHandreset()
{}

void CSymbolEngineOpenPPLPlayers::ResetOnNewRound()
{}

void CSymbolEngineOpenPPLPlayers::ResetOnMyTurn()
{}

void CSymbolEngineOpenPPLPlayers::ResetOnHeartbeat()
{}

bool CSymbolEngineOpenPPLPlayers::EvaluateSymbol(const char *name, double *result)
{
	// Symbol of a different symbol-engine
	return false;
}