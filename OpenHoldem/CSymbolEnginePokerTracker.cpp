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
#include "CSymbolEnginePokerTracker.h"

#include "CFormulaParser.h"
#include "..\PokerTracker_Query_Definitions\pokertracker_query_definitions.h"
#include "CPokerTrackerThread.h"
#include "CPreferences.h"
#include "CSymbolEngineRaisersCallers.h"
#include "CSymbolEngineUserchair.h"
#include "debug.h"
#include "OH_MessageBox.h"
#include "StringFunctions.h"


CSymbolEnginePokerTracker *p_symbol_engine_pokertracker = NULL;


CSymbolEnginePokerTracker::CSymbolEnginePokerTracker()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	assert(p_symbol_engine_raisers_callers != NULL);
	assert(p_symbol_engine_userchair != NULL);
}

CSymbolEnginePokerTracker::~CSymbolEnginePokerTracker()
{}

void CSymbolEnginePokerTracker::InitOnStartup()
{
	check_for_identity_of_players_executed_this_heartbeat = false;
	ClearAllStats();
}

void CSymbolEnginePokerTracker::ResetOnConnection()
{
	ClearAllStats();
	p_pokertracker_thread->StartThread();
}

void CSymbolEnginePokerTracker::ResetOnHandreset()
{}

void CSymbolEnginePokerTracker::ResetOnNewRound()
{}

void CSymbolEnginePokerTracker::ResetOnMyTurn()
{}

void CSymbolEnginePokerTracker::ResetOnHeartbeat()
{
	check_for_identity_of_players_executed_this_heartbeat = false;
}

void CSymbolEnginePokerTracker::WarnAboutInvalidPTSymbol(CString s) {
	CString error_message = "Error: Invalid PT-symbol: " + s;
	OH_MessageBox_Formula_Error(error_message, "Error");
}

bool CSymbolEnginePokerTracker::IsOldStylePTSymbol(CString s) {
	return ((s.Left(2) == "pt") 
		&& (s.Left(3) != "pt_")
		&& (s.Left(5) != "pt_r_"));
}

void CSymbolEnginePokerTracker::CheckForChangedPlayersOncePerHeartbeatAndSymbolLookup() {
	if (check_for_identity_of_players_executed_this_heartbeat)
	{
		return;
	}
	check_for_identity_of_players_executed_this_heartbeat = true;
	ClearAllStatsOfChangedPlayers();
}

void CSymbolEnginePokerTracker::ClearSeatStats(int chair, bool clearNameAndFound) {
	assert(chair >= k_first_chair); 
	assert(chair <= k_last_chair);
	write_log(preferences.debug_pokertracker(), "[CSymbolEnginePokerTracker] ClearSeatStats() for chair %i\n", chair);
	PT_DLL_ClearPlayerStats(chair);
	if (clearNameAndFound) {
		_player_data[chair].found = false;
		memset(_player_data[chair].pt_name, 0, k_max_length_of_playername);
		memset(_player_data[chair].scraped_name, 0, k_max_length_of_playername);
	}
	_player_data[chair].skipped_updates = k_advanced_stat_update_every;
}

void CSymbolEnginePokerTracker::ClearAllStatsOfChangedPlayers() {
	write_log(preferences.debug_pokertracker(), "[CSymbolEnginePokerTracker] Executing ClearAllStatsOfChangedPlayers()\n");
	for (int i=0; i<k_max_number_of_players; i++)
	{
		if (p_pokertracker_thread->CheckIfNameHasChanged(i))
		{
			ClearSeatStats(i, true);
		}
	}
}

void CSymbolEnginePokerTracker::ClearAllStats()
{
	for (int i=0; i<k_max_number_of_players; i++)
	{
		ClearSeatStats(i, true);
	}
}

bool CSymbolEnginePokerTracker::EvaluateSymbol(const char *name, double *result, bool log /* = false */)
{
  FAST_EXIT_ON_OPENPPL_SYMBOLS(name);
	if (memcmp(name,"pt_",3)!=0)
	{
		// Symbol of a different symbol-engine
		return false;
	}
	CString s = name;
	CheckForChangedPlayersOncePerHeartbeatAndSymbolLookup();
	if (IsOldStylePTSymbol(s))
	{
		CString error_message;
		error_message.Format(
			"Old style PokerTracker symbol detected: %s.\n"
			"\n"
			"PokerTracker symbol either start with \"pt_\" or \"pt_r_\".\n", s);
		OH_MessageBox_Formula_Error(
			error_message,			 
			"ERROR: Invalid PokerTracker Symbol");
		*result = k_undefined;
		return true;
	}
	if (!PT_DLL_IsValidSymbol(CString(s)))
	{
		// Invalid PokerTracker symbol
		WarnAboutInvalidPTSymbol(s);
		*result = k_undefined;
		return true;
	}
	int chair = 0;

	if (!p_pokertracker_thread->IsConnected())
	{
		if (!p_symbol_engine_userchair->userchair_confirmed() || p_formula_parser->IsParsing())
		{
			// We are not yet seated or formula is getting parsed.
			// Symbol-lookup happens, because of Formula-validation.
			// Not a problem, if we do not yet have a DB-connection.
			// Don't throw a warning here.
		}
		else
		{
			// We are seated and playing.
			// Serious problem, if we do not have a DB-connection.
			OH_MessageBox_Error_Warning("Not connected to PokerTracker database.\n"
				"Can't use PokerTracker symbols.", "ERROR");
		}
		*result = k_undefined;
		return true;
	}

	CString standard_symbol_name;
	// PokerTracker ymbols for the raise-chair
	if (StringAIsPrefixOfStringB("pt_r_", s))
	{
		chair = p_symbol_engine_raisers_callers->raischair();
	}
	// PokerTracker symbols for chair X
	else 
	{
		assert(StringAIsPrefixOfStringB("pt_", s));
		CString symbol = s;
		CString last_character = symbol.Right(1);
		chair = atoi(last_character);
	}
	AssertRange(chair, k_first_chair, k_last_chair);
	*result = PT_DLL_GetStat(s, chair); 
	return true;
}

CString CSymbolEnginePokerTracker::SymbolsProvided() {
  CString list;
  for (int i=0; i<PT_DLL_GetNumberOfStats(); ++i) {
    CString basic_symbol_name = PT_DLL_GetBasicSymbolNameWithoutPTPrefix(i);
    // Add symbol for raise-chair
    CString new_symbol = "pt_r_" + basic_symbol_name;
    list.AppendFormat(" %s", new_symbol);
    // Add symbols for all chairs, indexed by trailing numbers
    for (int j=0; j<k_max_number_of_players; j++) {
	    new_symbol.Format("pt_%s%i", basic_symbol_name, j); 
	    list.AppendFormat(" %s", new_symbol);
    }
  }
  return list;
}