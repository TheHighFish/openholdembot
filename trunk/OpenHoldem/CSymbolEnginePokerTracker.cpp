#include "stdafx.h"
#include "CSymbolEnginePokerTracker.h"

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

void CSymbolEnginePokerTracker::WarnAboutInvalidPTSymbol(CString s)
{
	CString error_message = "Error: Invalid PT-symbol: " + s;
	OH_MessageBox_Error_Warning(error_message, "Error");
}

bool CSymbolEnginePokerTracker::IsOldStylePTSymbol(CString s)
{
	return ((s.Left(2) == "pt") 
		&& (s.Left(3) != "pt_")
		&& (s.Left(5) != "pt_r_"));
}

void CSymbolEnginePokerTracker::CheckForChangedPlayersOncePerHeartbeatAndSymbolLookup()
{
	if (check_for_identity_of_players_executed_this_heartbeat)
	{
		return;
	}
	check_for_identity_of_players_executed_this_heartbeat = true;
	ClearAllStatsOfChangedPlayers();
}

void CSymbolEnginePokerTracker::ClearSeatStats(int chair, bool clearNameAndFound)
{
	assert(chair >= k_first_chair); 
	assert(chair <= k_last_chair);
	write_log(prefs.debug_pokertracker(), "[CSymbolEnginePokerTracker] ClearSeatStats() for chair %i\n", chair);
	PT_DLL_ClearPlayerStats(chair);
	if (clearNameAndFound)
	{
		_player_data[chair].found = false;
		memset(_player_data[chair].pt_name, 0, k_max_length_of_playername);
		memset(_player_data[chair].scraped_name, 0, k_max_length_of_playername);
	}
	_player_data[chair].skipped_updates = k_advanced_stat_update_every;
}

void CSymbolEnginePokerTracker::ClearAllStatsOfChangedPlayers()
{
	write_log(prefs.debug_pokertracker(), "[CSymbolEnginePokerTracker] Executing ClearAllStatsOfChangedPlayers()\n");
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

double CSymbolEnginePokerTracker::ProcessQuery(const char *s)
{
	CheckForChangedPlayersOncePerHeartbeatAndSymbolLookup();
	if (IsOldStylePTSymbol(s))
	{
		CString error_message;
		error_message.Format(
			"Old style PokerTracker symbol detected: %s.\n"
			"\n"
			"PokerTracker symbol either start with \"pt_\" or \"pt_r_\".\n", s);
		OH_MessageBox_Error_Warning(
			error_message,			 
			"ERROR: Invalid PokerTracker Symbol");
		return k_undefined;
	}
	if (!PT_DLL_IsValidSymbol(CString(*s)))
	{
		// Invalid PokerTracker symbol
		WarnAboutInvalidPTSymbol(s);
		return k_undefined;
	}
	int chair = 0;

	if (!p_pokertracker_thread->IsConnected())
	{
		if (!p_symbol_engine_userchair->userchair_confirmed())
		{
			// We are not yet seated.
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
		return k_undefined;
	}

	CString pure_symbol_name;
	// PokerTracker ymbols for the raise-chair
	if (StringAIsPrefixOfStringB("pt_r_", s))
	{
		chair = p_symbol_engine_raisers_callers->raischair();
		CString symbol = s;
		pure_symbol_name = symbol.Right(symbol.GetLength() - 5);
	}
	// PokerTracker symbols for chair X
	else 
	{
		assert(StringAIsPrefixOfStringB("pt_", s));
		CString symbol = s;
		CString last_character = symbol.Right(1);
		chair = atoi(last_character);
		pure_symbol_name = symbol.Right(symbol.GetLength() - 3);
	}
	AssertRange(chair, k_first_chair, k_last_chair);
	return PT_DLL_GetStat(pure_symbol_name, chair); 
}




