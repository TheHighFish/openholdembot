//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*****************************************************************************
//
// Purpose: Detecting if we play a tournament, especially
//   to enable / disable automatic blind-locking (stability) 
//
//*****************************************************************************

#include "stdafx.h"
#include "CSymbolEngineIsTournament.h"

#include <assert.h>
#include "CGameState.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineRaisersCallers.h"
#include "CSymbolEngineTime.h"
#include "CSymbolEngineTableLimits.h"
#include "..\CTablemap\CTablemap.h"
#include "MagicNumbers.h"
#include "StringFunctions.h"

CSymbolEngineIsTournament *p_symbol_engine_istournament = NULL;

const double k_lowest_bigblind_ever_seen_in_tournament           = 10.0;
const double k_large_bigblind_probably_later_table_in_tournament = 500.0;

const int k_number_of_tournament_identifiers = 45;
// Partial tournament strings of various casinos
// Sources: PokerStars, and lots of unnamed casinos (by PM)
// These strings have to be lower-cases for comparison
// http://www.maxinmontreal.com/forums/viewtopic.php?f=117&t=16104
const CString k_tournament_identifiers[k_number_of_tournament_identifiers] =
{	" ante ",		
	" ante:",
	"(ante ",		
	"(ante:",
	"buy-in:",		
	"buyin:"
	"buy-in ",
	"buyin ",
	"double ",
	"double-",
	"free $",
	"freeroll",
	"garantis",			// french for "guaranteed"
	"gratuit ",			// french for "free"
	"guaranteed",
	"miniroll",
	"mise initiale"		// french for "ante"
	" mtt",
	"mtt ",
	"(mtt",
	"multitable",
	"multi-table",
	" nothing",
	"-nothing",
	"qualif ",			// french abbreviation
	"qualificatif",		// french for "qualifier"
	"qualification",
	"qualifier",
	"rebuy",
	"shootout ",
	"sit and go",
	"sit&go",
	"sit & go",
	"sit 'n go",
	"sit'n go",
	" sng",
	"sng ",
	"(sng",
	"sup turbo",
	"super turbo",
	"superturbo",
	"super-turbo",
	"ticket ",
	"tour ",
	"tournament ",		
};

CSymbolEngineIsTournament::CSymbolEngineIsTournament()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	assert(p_symbol_engine_active_dealt_playing != NULL);
	assert(p_symbol_engine_chip_amounts != NULL);
	assert(p_symbol_engine_raisers_callers != NULL);
	assert(p_symbol_engine_tablelimits != NULL);
	assert(p_symbol_engine_time != NULL);
}

CSymbolEngineIsTournament::~CSymbolEngineIsTournament()
{}

void CSymbolEngineIsTournament::InitOnStartup()
{
	ResetOnConnection();
}

void CSymbolEngineIsTournament::ResetOnConnection()
{
	_istournament    = k_undefined;
	_decision_locked = false;
}

void CSymbolEngineIsTournament::ResetOnHandreset()
{}

void CSymbolEngineIsTournament::ResetOnNewRound()
{}

void CSymbolEngineIsTournament::ResetOnMyTurn()
{
	TryToDetectTournament();
}

void CSymbolEngineIsTournament::ResetOnHeartbeat()
{}

bool CSymbolEngineIsTournament::BetsAndBalancesAreTournamentLike()
{
	// "Beautiful" numbers => tournament
	// This condition does unfortunatelly only work for the first and final table in an MTT,
	// not necessarily for other late tables (fractional bets, uneven sums).
	double sum_of_all_cips = 0.0;
	for (int i=0; i<p_tablemap->nchairs(); i++)
	{
		sum_of_all_cips += p_symbol_engine_chip_amounts->balance(i);
		sum_of_all_cips += p_symbol_engine_chip_amounts->currentbet(i);
	}
	if (sum_of_all_cips != int(sum_of_all_cips))
	{
		// Franctional number.
		// Looks like a cash-game.
		return false;
	}
	if ((int(sum_of_all_cips) % 100) != 0)
	{
		// Not a multiplicity of 100.
		// Probably not a tournament.
		return false;
	}
	if ((int(sum_of_all_cips) % p_tablemap->nchairs()) != 0)
	{
		// Not a multiplicity of the players originally seated (nchairs).
		// Probably not a tournament.
		return false;
	}
	return true;
}

bool CSymbolEngineIsTournament::AntesPresent()
{
	// Antes are present, if all players are betting 
	// and at least 3 have a bet smaller than SB 
	// (remember: this is for the first few hands only).
	if ((p_symbol_engine_raisers_callers->nopponentsbetting() + 1)
		< p_symbol_engine_active_dealt_playing->nplayersseated())
	{
		return false;
	}
	int players_with_antes = 0;
	for (int i=0; i<p_tablemap->nchairs(); i++)
	{
		double players_bet = p_symbol_engine_chip_amounts->currentbet(i);
		if ((players_bet > 0) && (players_bet < p_symbol_engine_tablelimits->sblind()))
		{
			players_with_antes++;
		}
	}
	return (players_with_antes >= 3);
}

bool CSymbolEngineIsTournament::TitleStringLooksLikeTournament()
{
	CString title = p_scraper->title();
	title = title.MakeLower();
	for (int i=0; i<k_number_of_tournament_identifiers; i++)
	{
		if (title.Find(k_tournament_identifiers[i]))
		{
			return true;
		}
	}
	return false;
}

void CSymbolEngineIsTournament::TryToDetectTournament()
{
	if (_istournament != k_undefined)
		write_log(preferences.debug_istournament(), "[CSymbolEngineIsTournament] Currently istournament = %s\n", Bool2CString(_istournament));
	else
		write_log(preferences.debug_istournament(), "[CSymbolEngineIsTournament] Currently istournament = unknown\n");
	// Don't do anything if we are already sure.
	// Don't mix things up.
	if (_decision_locked)
	{
		write_log(preferences.debug_istournament(), "[CSymbolEngineIsTournament] decision already locked\n");
		return;
	}
	// If we have more than 2 hands played we should be sure
	// and stick to our decision, whatever it is (probably cash-game).
	// Also checking for (elapsedauto < elapsed). i.e. at least one action
	// since connection, as handsplayed does not reset if we play multiple games.
	if ((_istournament != k_undefined)
		&& (p_game_state->hands_played() > 2)
		&& (p_symbol_engine_time->elapsedauto() < p_symbol_engine_time->elapsed()))
	{
		write_log(preferences.debug_istournament(), "[CSymbolEngineIsTournament] Enough hands played; locking current value\n");
		_decision_locked = true;
		return;
	}
	// If the blinds are "too low" then we play a cash-game.
	double bigblind = p_symbol_engine_tablelimits->bblind();
	if ((bigblind > 0) && (bigblind < k_lowest_bigblind_ever_seen_in_tournament))
	{
		write_log(preferences.debug_istournament(), "[CSymbolEngineIsTournament] Blinds \"too low\"; this is a cash-game\n");
		_istournament    = false;
		_decision_locked = true;
		return;
	}
	// If the title-string looks like a tournament then it is a tournament.
	if (TitleStringLooksLikeTournament())
	{
		write_log(preferences.debug_istournament(), "[CSymbolEngineIsTournament] Table title looks like a tournament\n");
		_istournament    = true;
		_decision_locked = true;
		return;
	}
	// If there are antes then it is a tournament.
	if (AntesPresent())
	{
		write_log(preferences.debug_istournament(), "[CSymbolEngineIsTournament] Game with antes; therefore tournament\n");
		_istournament    = true;
		_decision_locked = true;
		return;
	}
	// If bets and balances are "tournament-like", then it is a tournament
	if (BetsAndBalancesAreTournamentLike())
	{
		write_log(preferences.debug_istournament(), "[CSymbolEngineIsTournament] Bets and balances look like tournament\n");
		_istournament    = true;
		_decision_locked = true;
		return;
	}
	// Not yet sure, but we have handled so far:
	// * all lower stakes cash-games
	// * all SNGs / MTTs with good title-string
	// * all SNGs and starting tables of MTTs (by bets and balances)
	// * some later tables of MTTs (by antes)
	// * some more later tables of MTTs (if no reconnection is necessary)
	// This leaves us with only 2 cases
	// * medium and high-stakes cash-games ($5/$10 and above)
	// * some (very few) later tables of MTTs
	if (bigblind > k_large_bigblind_probably_later_table_in_tournament)
	{
		// Probably tournament, but not really sure yet,
		// so don't lock the decision.
		write_log(preferences.debug_istournament(), "[CSymbolEngineIsTournament] bigblind very large; probably tournament\n");
		_istournament = true;
		return;
	}
	// Smaller bigblinds (< $500) will be considered cash-games, 
	// because they are too small for later tables of tournaments
	_istournament    = false;
	_decision_locked = true;
	write_log(preferences.debug_istournament(), "[CSymbolEngineIsTournament] high-stakes cash-game up to $200\\$400.n");
	// The only case that can go wrong now:
	// High-stakes cash-games ($250/$500 and above) can be recognized 
	// as tournaments. 
	// Main consequence: blinds won't be locked for the entire session,
	// but only for the current hand. Does this hurt much?
}
