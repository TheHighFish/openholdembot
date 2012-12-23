#include "stdafx.h"
#include "CSymbolEngineRaisersCallers.h"

#include <assert.h>
#include "CBetroundCalculator.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CStringMatch.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineDealerchair.h"
#include "CSymbolEngineHistory.h"

CSymbolEngineRaisersCallers *p_symbol_engine_raisers_callers = NULL;

CSymbolEngineRaisersCallers::CSymbolEngineRaisersCallers()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	assert(p_symbol_engine_active_dealt_playing != NULL);
	assert(p_symbol_engine_chip_amounts != NULL);
	assert(p_symbol_engine_dealerchair != NULL);
	// Also using p_symbol_engine_history one time,
	// but because we use "old" information here
	// there is no dependency on this cycle.
}

CSymbolEngineRaisersCallers::~CSymbolEngineRaisersCallers()
{}

void CSymbolEngineRaisersCallers::InitOnStartup()
{}

void CSymbolEngineRaisersCallers::ResetOnConnection()
{
	_nchairs = p_tablemap->nchairs();
	_raischair_previous_frame = k_undefined;
}

void CSymbolEngineRaisersCallers::ResetOnHandreset()
{
	_userchair = p_symbol_engine_userchair->userchair();

	// callbits, raisbits, etc.
	for (int i=k_betround_preflop; i<=k_betround_river; i++)
	{
		_callbits[i] = 0;
		_raisbits[i] = 0;
		_foldbits[i] = 0;
	}
}

void CSymbolEngineRaisersCallers::ResetOnNewRound()
{}

void CSymbolEngineRaisersCallers::ResetOnMyTurn()
{
	_dealerchair      = p_symbol_engine_dealerchair->dealerchair();
	_betround         = p_betround_calculator->betround();
	_playersdealtbits = p_symbol_engine_active_dealt_playing->playersdealtbits();

	CalculateRaisers();
	CalculateCallers();
}

void CSymbolEngineRaisersCallers::ResetOnHeartbeat()
{}

void CSymbolEngineRaisersCallers::CalculateRaisers()
{
	int FirstPossibleRaiser;
	int LastPossibleRaiser;
	int last_bet;
	// Raischair, nopponentsraising, raisbits
	//
	// Don't start searching for the highest bet at the button.
	// This method will fail, if a player in late raises and a player in early coldcalls.
	// Start searching at the last known raiser; and only at the button when we have a new betting-round.
	if (p_symbol_engine_history->DidAct())
	{
		// Counting raises behind me
		FirstPossibleRaiser = _userchair + 1;
	}
	else
	{
		// Start with the player after last known raiser
		FirstPossibleRaiser = _dealerchair + 1;
		last_bet = p_scraper->player_bet(_raischair_previous_frame);
	}
	// For technical reasons (for-loop) we handle the modulo-operation inside the loop
	LastPossibleRaiser = FirstPossibleRaiser + p_tablemap->nchairs() - 1;

	for (int i=FirstPossibleRaiser; i<=LastPossibleRaiser; i++)
	{
		double current_players_bet = p_symbol_engine_chip_amounts->currentbet(i%p_tablemap->nchairs());

		if (current_players_bet > last_bet)
		{
			last_bet = current_players_bet;
			_raischair = i % p_tablemap->nchairs();
			int new_raisbits = _raisbits[_betround] | k_exponents[i%p_tablemap->nchairs()];
			_raisbits[_betround] = new_raisbits;
		}
	}
}

void CSymbolEngineRaisersCallers::CalculateCallers()
{
	// nopponentscalling
	//
	// Take the first player with the smallest non-zero balance 
	// after the aggressor as the first bettor.
	// Then start a circular search for callers.
	int FirstBettor = _raischair;
	double SmallestBet = p_scraper->player_bet(FirstBettor);
	for (int i=_raischair+1; 
		i<=_raischair+p_tablemap->nchairs()-1; 
		i++)
	{
		double CurrentBet = p_scraper->player_bet(i%p_tablemap->nchairs());
		if ((CurrentBet < SmallestBet) && (CurrentBet > 0))
		{
			FirstBettor = i%p_tablemap->nchairs();
			SmallestBet = CurrentBet;
		}
	}

	double CurrentBet = p_scraper->player_bet(FirstBettor);
	for (int i=FirstBettor+1; i<=FirstBettor+p_tablemap->nchairs()-1; i++)
	{
		// Exact match required. Players being allin don't count as callers.
		if ((p_scraper->player_bet(i%p_tablemap->nchairs()) == CurrentBet) && (CurrentBet > 0))
		{
			int new_callbits = _callbits[_betround] | k_exponents[i%p_tablemap->nchairs()];
			_callbits[_betround] = new_callbits;
			_nopponentscalling++;
		}
		else if (p_scraper->player_bet(i%p_tablemap->nchairs()) > CurrentBet)
		{
			CurrentBet = p_scraper->player_bet(i%p_tablemap->nchairs());
		}
	}
}

void CSymbolEngineRaisersCallers::CalculateNOpponentsCheckingBettingFolded()
{
	for (int i=0; i<_nchairs; i++)
	{
		double current_players_bet = p_symbol_engine_chip_amounts->currentbet(i);
		if (current_players_bet < p_symbol_engine_chip_amounts->currentbet(_raischair)
			&& p_scraper_access->PlayerHasCards(i))
		{
			_nplayerscallshort++;
		}
		if (i == _userchair)
		{
			// No opponent;
			// Nothing more to do
			continue;
		}
		if (current_players_bet > 0) 
		{
			_nopponentsbetting++;
		}
		// Players might have been betting, but folded, so no else for the if
		if ((_playersdealtbits & (1<<(i)))
			&& ((p_scraper->card_player(i, 0) == CARD_NOCARD)
				|| (p_scraper->card_player(i, 1) == CARD_NOCARD)))
		{
			_nopponentsfolded++;					
		}
		if (p_scraper->card_player(i, 0) != CARD_NOCARD 
			&& p_scraper->card_player(i, 1) != CARD_NOCARD 
			&& current_players_bet == 0)
		{
			_nopponentschecking++;
		}
	}
}

void CSymbolEngineRaisersCallers::CalculateFoldBits()
{
	// foldbits (very late, as they depend on the dealt symbols)
	int new_foldbits = 0;
	for (int i=0; i<p_tablemap->nchairs(); i++)
	{
		if (p_scraper->card_player(i, 0) == CARD_NOCARD &&
			p_scraper->card_player(i, 1) == CARD_NOCARD)
		{
			new_foldbits |= k_exponents[i];
		}
	}
	// remove players, who didn't get dealt.
	new_foldbits &= int(_playersdealtbits);

	// remove players, who folded in earlier betting-rounds.
	if (_betround >= k_betround_flop)
	{
		new_foldbits &= (~_foldbits[k_betround_preflop]);
	}
	if (_betround >= k_betround_turn)
	{
		new_foldbits &= (~_foldbits[k_betround_flop]);
	}
	if (_betround >= k_betround_river)   
	{
		new_foldbits &= (~_foldbits[k_betround_turn]);
	}
	_foldbits[_betround] = new_foldbits;
}

