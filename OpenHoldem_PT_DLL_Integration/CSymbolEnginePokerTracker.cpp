#include "stdafx.h"
#include "CSymbolEnginePokerTracker.h"

#include "CPokerTrackerDLLInterface.h"
#include "CPokerTrackerThread.h"
#include "CSymbolEngineRaisersCallers.h"
#include "OH_MessageBox.h"
#include "StringFunctions.h"


CSymbolEnginePokerTracker *p_symbol_engine_pokertracker = NULL;


CSymbolEnginePokerTracker::CSymbolEnginePokerTracker()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	assert(p_symbol_engine_raisers_callers != NULL);
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
	for (int i=0; i<=pt_max; i++)
	{
		p_pokertracker_dll_interface->SetStat(chair, i, -1.0);
	}
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
	for (int i=0; i<k_max_number_of_players; i++)
	{
//!!!		if (CheckIfNameHasChanged(i))//!!!
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

int GetIndex(char *symbol)
{
	return 0; //!!!
}

double CSymbolEnginePokerTracker::ProcessQuery(const char * s)
{
	CheckForChangedPlayersOncePerHeartbeatAndSymbolLookup();
	int chair = 0;

//!!!	if (!_connected || PQstatus(_pgconn) != CONNECTION_OK)
	{
		//!!!if (!p_symbol_engine_userchair->userchair_confirmed())
		{
			// We are not yet seated.
			// Symbol-lookup happens, because of Formula-validation.
			// Not a problem, if we do not yet have a DB-connection.
			// Don't throw a warning here.
		}
		//!!!else
		{
			// We are seated and playing.
			// Serious problem, if we do not have a DB-connection.
			OH_MessageBox_Error_Warning("Not connected to PokerTracker database.\n"
				"Can't use PokerTracker symbols.", "ERROR");
		}
		return k_undefined;
	}

	// ATTENTION!
	//  
	// Be very careful, if a string is a prefix of multiple symbols,
	// e.g. "pt_vpip" is a prefix of both "pt_vpipX" and "pt_vpipsbX".
	// Take care to handle those cases correctly!

	// PokerTracker ring game symbols for the raise-chair
	if (StringAIsPrefixOfStringB("pt_r_", s) //!!!!!
		&& !StringAIsPrefixOfStringB("pt_riverpct", s))
	{
		chair = p_symbol_engine_raisers_callers->raischair();
		{
			// Invalid ring game symbol
			WarnAboutInvalidPTSymbol(s);
			return -1.0;
		}
	}
	// PokerTracker ring game symbols for chair X
	else if (StringAIsPrefixOfStringB("pt_", s))
	{
		//!!! chair ==
		{
			// Invalid ring game symbol
			WarnAboutInvalidPTSymbol(s);
			return -1.0;
		}
	}
	int index = GetIndex("symbol"); //!!!
	//!!!AssertRange(index, -1, (k_max_number_of_supported_pokertracker_stats - 1));
	AssertRange(chair, k_first_chair, k_last_chair);
	if (index == 1)
	{
		WarnAboutInvalidPTSymbol(s);
		return -1.0;
	}
	return p_pokertracker_dll_interface->GetStat(chair, index);
}




