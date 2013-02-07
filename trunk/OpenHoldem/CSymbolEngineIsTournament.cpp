#include "stdafx.h"
#include "CSymbolEngineIsTournament.h"

#include <assert.h>
#include "CGameState.h"
#include "CScraper.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineRaisersCallers.h"
#include "CTableLimits.h"
#include "..\CTablemap\CTablemap.h"
#include "MagicNumbers.h"

CSymbolEngineIsTournament *p_symbol_engine_istournament = NULL;

const int k_lowest_bigblind_ever_seen_in_tournament = 10;

const int k_number_of_tournament_identifiers = 13;
const CString k_tournament_identifiers[k_number_of_tournament_identifiers] =
{	"tournament ",	// PokerStars
	" ante ",		// PokerStars
	" ante:",	
	"buy-in:",		// Winamax.fr
	"buyin:"
	"buy-in ",
	"buyin ",
	"super turbo",
	"superturbo",
	"super-turbo",
	"double up",
	"double-up",
	" sng",
	"sng ",
};

CSymbolEngineIsTournament::CSymbolEngineIsTournament()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	assert(p_symbol_engine_active_dealt_playing != NULL);
	assert(p_symbol_engine_chip_amounts != NULL);
	assert(p_symbol_engine_raisers_callers != NULL);
}

CSymbolEngineIsTournament::~CSymbolEngineIsTournament()
{}

void CSymbolEngineIsTournament::InitOnStartup()
{
	ResetOnConnection();
}

void CSymbolEngineIsTournament::ResetOnConnection()
{
	_is_tournament   = false;
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
		if ((players_bet > 0) && (players_bet < p_tablelimits->sblind()))
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
	// Don't do anything if we are already sure.
	// Don't mix things up.
	if (_decision_locked)
	{
		return;
	}
	// If we have more than 2 hands played we should be sure
	// and stick to our decision (probably cash-game).
	if (p_game_state->hands_played() > 2)
	{
		_decision_locked = true;
		return;
	}
	// If the blinds are "too low" then we play a cash-game.
	double bigblind = p_tablelimits->sblind();
	if ((bigblind > 0) && (bigblind < k_lowest_bigblind_ever_seen_in_tournament))
	{
		_is_tournament   = false;
		_decision_locked = true;
		return;
	}
	// If the title-string looks like a tournament then it is a tournament.
	if (TitleStringLooksLikeTournament())
	{
		_is_tournament   = true;
		_decision_locked = true;
		return;
	}
	// If there are antes then it is a tournament.
	if (AntesPresent())
	{
		_is_tournament   = true;
		_decision_locked = true;
		return;
	}
	// If bets and balances are "tournament-like", then it is a tournament
	if (BetsAndBalancesAreTournamentLike())
	{
		_is_tournament   = true;
		_decision_locked = true;
		return;
	}
	// Everything else is a cash-game until we know better.
}

