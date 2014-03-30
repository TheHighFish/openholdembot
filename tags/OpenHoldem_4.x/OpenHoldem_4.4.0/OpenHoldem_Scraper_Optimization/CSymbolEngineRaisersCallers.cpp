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
#include "CSymbolEngineUserchair.h"

CSymbolEngineRaisersCallers *p_symbol_engine_raisers_callers = NULL;

CSymbolEngineRaisersCallers::CSymbolEngineRaisersCallers()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	assert(p_symbol_engine_active_dealt_playing != NULL);
	assert(p_symbol_engine_chip_amounts != NULL);
	assert(p_symbol_engine_dealerchair != NULL);
	assert(p_symbol_engine_userchair != NULL);
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
	_raischair_previous_frame = k_undefined;
	
	ResetOnHandreset();
}

void CSymbolEngineRaisersCallers::ResetOnHandreset()
{
	// callbits, raisbits, etc.
	for (int i=k_betround_preflop; i<=k_betround_river; i++)
	{
		_callbits[i] = 0;
		_raisbits[i] = 0;
		_foldbits[i] = 0;
	}
	_raischair = k_undefined;
}

void CSymbolEngineRaisersCallers::ResetOnNewRound()
{}

void CSymbolEngineRaisersCallers::ResetOnMyTurn()
{
	CalculateRaisers();
	CalculateCallers();
}

void CSymbolEngineRaisersCallers::ResetOnHeartbeat()
{
	CalculateNOpponentsCheckingBettingFolded();
	CalculateFoldBits();
}

void CSymbolEngineRaisersCallers::CalculateRaisers()
{
	_raisbits[BETROUND] = 0;

	if (p_symbol_engine_chip_amounts->call() <= 0.0)
	{
		// There are no bets and raises.
		// Skip the calculations to keep the raischair of the previous round.
		// http://www.maxinmontreal.com/forums/viewtopic.php?f=156&t=16806
		return;
	}
	// Raischair, nopponentsraising, raisbits
	//
	int first_possible_raiser = FirstPossibleRaiser();
	int last_possible_raiser  = LastPossibleRaiser();
	double last_bet = p_scraper->player_bet(_raischair_previous_frame);

	for (int i=first_possible_raiser; i<=last_possible_raiser; i++)
	{
		double current_players_bet = p_symbol_engine_chip_amounts->currentbet(i%p_tablemap->nchairs());

		if (current_players_bet > last_bet)
		{
			last_bet = current_players_bet;
			_raischair = i % p_tablemap->nchairs();
			int new_raisbits = _raisbits[BETROUND] | k_exponents[i%p_tablemap->nchairs()];
			_raisbits[BETROUND] = new_raisbits;
		}
	}
	AssertRange(_raischair, k_undefined, k_last_chair);
}

void CSymbolEngineRaisersCallers::CalculateCallers()
{
	// nopponentscalling
	//
	// Take the first player with the smallest non-zero balance 
	// after the aggressor as the first bettor.
	// Then start a circular search for callers.

	_callbits[BETROUND] = 0;
	_nopponentscalling = 0;

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
	for (int i=FirstBettor+1; 
		i<=FirstBettor+p_tablemap->nchairs()-1; 
		i++)
	{
		// Exact match required. Players being allin don't count as callers.
		if ((p_scraper->player_bet(i%p_tablemap->nchairs()) == CurrentBet) && (CurrentBet > 0))
		{
			int new_callbits = _callbits[BETROUND] | k_exponents[i%p_tablemap->nchairs()];
			_callbits[BETROUND] = new_callbits;
			_nopponentscalling++;
		}
		else if (p_scraper->player_bet(i%p_tablemap->nchairs()) > CurrentBet)
		{
			CurrentBet = p_scraper->player_bet(i%p_tablemap->nchairs());
		}
	}

	AssertRange(_callbits[BETROUND], 0, k_bits_all_ten_players_1_111_111_111);
	AssertRange(_nopponentscalling,   0, k_max_number_of_players);
}

int CSymbolEngineRaisersCallers::FirstPossibleRaiser()
{
	int first_possible_raiser;
	// Don't start searching for the highest bet at the button.
	// This method will fail, if a player in late raises and a player in early coldcalls.
	// Start searching at the last known raiser; and only at the button when we have a new betting-round.
	if (p_symbol_engine_history->DidAct())
	{
		// Counting raises behind me
		first_possible_raiser = (USER_CHAIR + 1) % p_tablemap->nchairs();
	}
	else
	{
		// Start with the player after last known raiser
		first_possible_raiser = (DEALER_CHAIR + 1) % p_tablemap->nchairs();
	}
	return first_possible_raiser;
}

int CSymbolEngineRaisersCallers::LastPossibleRaiser()
{
	// For technical reasons (for-loop) we handle the modulo-operation 
	// inside the loop and not here.
	return (FirstPossibleRaiser() + p_tablemap->nchairs() - 1);
}

void CSymbolEngineRaisersCallers::CalculateNOpponentsCheckingBettingFolded()
{
	_nplayerscallshort  = 0;
	_nopponentsbetting  = 0;
	_nopponentsfolded   = 0;
	_nopponentschecking = 0;

	for (int i=0; i<p_tablemap->nchairs(); i++)
	{
		double current_players_bet = p_symbol_engine_chip_amounts->currentbet(i);
		if (current_players_bet < RaisersBet()
			&& p_scraper_access->PlayerHasCards(i))
		{
			_nplayerscallshort++;
		}
		if (i == USER_CHAIR)
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
		if ((p_symbol_engine_active_dealt_playing->playersdealtbits() & (1<<(i)))
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
	AssertRange(_nplayerscallshort,  0, k_max_number_of_opponents_at_full_ring_table);
	// Using k_max_number_of_players instead of k_max_number_of_opponents_at_full_ring_table below,
	// as it might be that the user is not seated or user-chair not recognized
	AssertRange(_nopponentsbetting,  0, k_max_number_of_players);
	AssertRange(_nopponentsfolded,   0, k_max_number_of_players);
	AssertRange(_nopponentschecking, 0, k_max_number_of_players);
}

double CSymbolEngineRaisersCallers::RaisersBet()
{
	// The raisers bet is simply the largest bet at the table.
	// So we don't have to know the raisers chair for that.
	double result = 0;
	for (int i=0; i<p_tablemap->nchairs(); i++)
	{
		double current_players_bet = p_symbol_engine_chip_amounts->currentbet(i);
		if (current_players_bet > result
			&& p_scraper_access->PlayerHasCards(i))
		{
			result = current_players_bet;
		}
	}
	return result;
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
	new_foldbits &= p_symbol_engine_active_dealt_playing->playersdealtbits();

	// remove players, who folded in earlier betting-rounds.
	if (BETROUND >= k_betround_flop)
	{
		new_foldbits &= (~_foldbits[k_betround_preflop]);
	}
	if (BETROUND >= k_betround_turn)
	{
		new_foldbits &= (~_foldbits[k_betround_flop]);
	}
	if (BETROUND >= k_betround_river)   
	{
		new_foldbits &= (~_foldbits[k_betround_turn]);
	}
	_foldbits[BETROUND] = new_foldbits;
}

int CSymbolEngineRaisersCallers::nopponentsraising()		
{ 
	// Be careful: nopponents does not include the user, 
	// whereas raisbits counts all chairs. 
	// We have to remove the userchair here.
	int opponents_raisbits = _raisbits[BETROUND] 
		& ~p_symbol_engine_userchair->userchairbit();
	return bitcount(opponents_raisbits); 

}

