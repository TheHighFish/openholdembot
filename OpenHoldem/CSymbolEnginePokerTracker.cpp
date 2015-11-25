//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose:
//
//*******************************************************************************

#include "stdafx.h"
#include "CSymbolEnginePokerTracker.h"
#include "CSymbolEngineActiveDealtPlaying.h"

#include "CFormulaParser.h"
#include "..\PokerTracker_Query_Definitions\pokertracker_query_definitions.h"
#include "CPokerTrackerThread.h"
#include "CPreferences.h"
#include "CSymbolEngineCallers.h"
#include "CSymbolEngineChairs.h"
#include "CSymbolengineDealerchair.h"
#include "CSymbolEngineIsRush.h"
#include "CSymbolEngineRaisers.h"
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
	assert(p_symbol_engine_isrush != NULL); 
	assert(p_symbol_engine_raisers != NULL);
	assert(p_symbol_engine_userchair != NULL);
	assert(p_symbol_engine_active_dealt_playing != NULL);
}

CSymbolEnginePokerTracker::~CSymbolEnginePokerTracker() {
}

void CSymbolEnginePokerTracker::InitOnStartup() {
	check_for_identity_of_players_executed_this_heartbeat = false;
	ClearAllStats();
}

void CSymbolEnginePokerTracker::ResetOnConnection() {
	ClearAllStats();
	p_pokertracker_thread->StartThread();
}

void CSymbolEnginePokerTracker::ResetOnHandreset() {
  if (p_symbol_engine_isrush->isrush())
    ClearAllStats();
}

void CSymbolEnginePokerTracker::ResetOnNewRound()
{}

void CSymbolEnginePokerTracker::ResetOnMyTurn()
{}

void CSymbolEnginePokerTracker::ResetOnHeartbeat()
{
	check_for_identity_of_players_executed_this_heartbeat = false;
}

void CSymbolEnginePokerTracker::WarnAboutInvalidPTSymbol(CString s) {
	CString error_message;
  error_message.Format("Invalid PT-symbol: %s\n", s);
	OH_MessageBox_Formula_Error(error_message, "Error");
}

bool CSymbolEnginePokerTracker::IsOldStylePTSymbol(CString s) {
	return ((s.Left(2) == "pt") 
		&& ((s.Left(3) != "pt_") || (s.Left(5) == "pt_r_")));
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
	assert(chair >= kFirstChair); 
	assert(chair <= kLastChair);
	write_log(preferences.debug_pokertracker(), "[CSymbolEnginePokerTracker] ClearSeatStats() for chair %i\n", chair);
	PT_DLL_ClearPlayerStats(chair);
	if (clearNameAndFound) {
		_player_data[chair].found = false;
		memset(_player_data[chair].pt_name, 0, kMaxLengthOfPlayername);
		memset(_player_data[chair].scraped_name, 0, kMaxLengthOfPlayername);
	}
	_player_data[chair].skipped_updates = k_advanced_stat_update_every;
}

void CSymbolEnginePokerTracker::ClearAllStatsOfChangedPlayers() {
	write_log(preferences.debug_pokertracker(), "[CSymbolEnginePokerTracker] Executing ClearAllStatsOfChangedPlayers()\n");
	for (int i=0; i<kMaxNumberOfPlayers; i++)
	{
		if (p_pokertracker_thread->CheckIfNameHasChanged(i))
		{
			ClearSeatStats(i, true);
		}
	}
}

void CSymbolEnginePokerTracker::ClearAllStats()
{
	for (int i=0; i<kMaxNumberOfPlayers; i++)
	{
		ClearSeatStats(i, true);
	}
}

int CSymbolEnginePokerTracker::PlayerIcon(const int chair) {
  assert(chair >= 0);
  assert(chair <= kLastChair);
  return PT_DLL_GetStat("icon", chair);
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
			"PokerTracker symbol start with \"pt_\".\n"
      "Possible postfixes:\n"
      "  * chair number (0..9)\n"
      "  * _raischair\n"
      "  * _headsup\n"
      "  * _smallblind\n"
      "  * _bigblind\n"
      "  * _dealer\n"
      "  * _cutoff\n"
      "  * _user\n"
      "  * _firstraiser\n"
      "  * _firstcaller\n"
      "  * _lastcaller\n", s);
		OH_MessageBox_Formula_Error(
			error_message,			 
			"ERROR: Invalid PokerTracker Symbol");
		*result = kUndefined;
		return true;
	}
	if (!PT_DLL_IsValidSymbol(CString(s)))
	{
		// Invalid PokerTracker symbol
		WarnAboutInvalidPTSymbol(s);
		*result = kUndefined;
		return true;
	}
	int chair = 0;

	if (!p_pokertracker_thread->IsConnected()) 	{
		if (!p_symbol_engine_userchair->userchair_confirmed() || p_formula_parser->IsParsing()) {
			// We are not yet seated or formula is getting parsed.
			// Symbol-lookup happens, because of Formula-validation.
			// Not a problem, if we do not yet have a DB-connection.
			// Don't throw a warning here.
      write_log(preferences.debug_pokertracker(), "[PokerTracker] Not yet seated or formula parsing.\n");
		} else {
			// We are seated and playing.
			// Serious problem, if we do not have a DB-connection.
			OH_MessageBox_Error_Warning("Not connected to PokerTracker database.\n"
				"Can't use PokerTracker symbols.");
		}
		*result = kUndefined;
		return true;
	}

	CString standard_symbol_name;
	assert(StringAIsPrefixOfStringB("pt_", s));
	// PokerTracker symbols for the raise-chair
	if (s.Right(10) == "_raischair") {
		chair = p_symbol_engine_raisers->raischair();
	}
	// PokerTracker symbols for the opponent headsup chair
	else if (s.Right(8) == "_headsup") {
    chair = p_symbol_engine_chairs->opponent_headsup_chair();
	}
  // PokerTracker symbols for the smallblind chair
	else if (s.Right(11) == "_smallblind") {
    chair = p_symbol_engine_chairs->smallblind_chair();
	}
  // PokerTracker symbols for the bigblind chair
	else if (s.Right(9) == "_bigblind") {
    chair = p_symbol_engine_chairs->bigblind_chair();
	}
  // PokerTracker symbols for the cutoff chair
	else if (s.Right(7) == "_cutoff") {
    chair = p_symbol_engine_chairs->cutoff_chair();
	}
  // PokerTracker symbols for the firstcaller chair
	else if (s.Right(12) == "_firstcaller") {
    chair = p_symbol_engine_callers->firstcaller_chair();
	}
  // PokerTracker symbols for the lastcaller chair
	else if (s.Right(11) == "_lastcaller") {
    chair = p_symbol_engine_callers->lastcaller_chair();
	}
  // PokerTracker symbols for the firstraiser chair
	else if (s.Right(12) == "_firstraiser") {
		chair = p_symbol_engine_raisers->firstraiser_chair();
	}
  // PokerTracker symbols for the dealerchair chair
	else if (s.Right(7) == "_dealer") {
    chair = p_symbol_engine_dealerchair->dealerchair();
	}
  // PokerTracker symbols for the  chair
	else if (s.Right(5) == "_user") {
    chair = p_symbol_engine_userchair->userchair();
	}
  // PokerTracker symbols for chair X
	else {
		CString symbol = s;
		CString last_character = symbol.Right(1);
    if (!isdigit(last_character[0])) {
      CString error_message;
      error_message.Format("Invalid PokerTracker Symbol: &s",
        symbol);
      OH_MessageBox_Formula_Error(error_message, "ERROR");
		  *result = kUndefined;
      return false;
    }
		chair = atoi(last_character);
	}
  // Catch undefined chair (e.g. pt_r_-symbol without raisee)
  if (chair < 0) {
    *result = kUndefined;
    return true;
  }
	AssertRange(chair, kFirstChair, kLastChair);
	*result = PT_DLL_GetStat(s, chair); 
	return true;
}

CString CSymbolEnginePokerTracker::SymbolsProvided() {
  CString list;
  for (int i=0; i<PT_DLL_GetNumberOfStats(); ++i) {
    CString basic_symbol_name = PT_DLL_GetBasicSymbolNameWithoutPTPrefix(i);
    // Add symbol for raise-chair
    CString new_symbol = "pt_" + basic_symbol_name + "_raischair";
    list.AppendFormat(" %s", new_symbol);
    // Add symbol for headsup-chair...
    new_symbol = "pt_" + basic_symbol_name + "_headsup";
	  list.AppendFormat(" %s", new_symbol);
    // ... and all similar symbols
    new_symbol = "pt_" + basic_symbol_name + "_smallblind";
	  list.AppendFormat(" %s", new_symbol);
    new_symbol = "pt_" + basic_symbol_name + "_bigblind";
	  list.AppendFormat(" %s", new_symbol);
    new_symbol = "pt_" + basic_symbol_name + "_cutoff";
	  list.AppendFormat(" %s", new_symbol);
    new_symbol = "pt_" + basic_symbol_name + "_firstcaller";
	  list.AppendFormat(" %s", new_symbol);
    new_symbol = "pt_" + basic_symbol_name + "_lastcaller";
	  list.AppendFormat(" %s", new_symbol);
    new_symbol = "pt_" + basic_symbol_name + "_firstraiser";
	  list.AppendFormat(" %s", new_symbol);
    new_symbol = "pt_" + basic_symbol_name + "_dealer";
	  list.AppendFormat(" %s", new_symbol);
    new_symbol = "pt_" + basic_symbol_name + "_user";
	  list.AppendFormat(" %s", new_symbol);

    // Add symbols for all chairs, indexed by trailing numbers
    for (int j=0; j<kMaxNumberOfPlayers; j++) {
	    new_symbol.Format("pt_%s%i", basic_symbol_name, j); 
	    list.AppendFormat(" %s", new_symbol);
    }
  }
  return list;
}
