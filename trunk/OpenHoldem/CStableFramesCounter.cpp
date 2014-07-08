//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#include "stdafx.h"
#include "CStableFramesCounter.h"

#include "CPreferences.h"
#include "CScraper.h"
#include "CSymbolEngineAutoplayer.h"
#include "CTableState.h"
#include "MagicNumbers.h"

CStableFramesCounter *p_stableframescounter = NULL;


CStableFramesCounter::CStableFramesCounter()
{
	// Initialize private variables
	memset(&_card_common_last[0],		0, sizeof(_card_common_last[0])*k_number_of_community_cards);
	memset(&_card_player_last[0][0],	0, sizeof(_card_player_last[0][0])*k_number_of_cards_per_player*k_max_number_of_players);
	memset(&_dealer_last[0],			0, sizeof(_dealer_last[0])*k_max_number_of_players);
	memset(&_playerbalance_last[0],		0, sizeof(_playerbalance_last[0])*k_max_number_of_players);
	memset(&_playerbet_last[0],			0, sizeof(_playerbet_last[0])*k_max_number_of_players);
	_myturnbitslast = 0;

	Reset();
}

CStableFramesCounter::~CStableFramesCounter()
{}

void CStableFramesCounter::Reset()
{
	write_log(preferences.debug_stableframescounter(), "[CStableFramesCounter] Reset\n");
	_NumberOfStableFrames = 0;
	_isReset = true;
}

void CStableFramesCounter::SaveCurrentState()
{
	/* 
		Saves the current state for future reference
	*/

	_myturnbitslast = p_symbol_engine_autoplayer->myturnbits();

	for (int i=0; i<k_number_of_community_cards; i++)
    _card_common_last[i] = p_table_state->_common_cards[i].GetValue();

	for (int i=0; i<k_max_number_of_players; i++)
	{
    _card_player_last[i][0]	= p_table_state->_players[i]._hole_cards[0].GetValue();
		_card_player_last[i][1]	= p_table_state->_players[i]._hole_cards[1].GetValue();
		_dealer_last[i]         = p_scraper->dealer(i);
		_playerbalance_last[i]  = p_table_state->_players[i]._balance;
		_playerbet_last[i]      = p_scraper->player_bet(i);
	}
}

// This function does automatically detect a changed game-state,
// so there is no longer any need to do a "Reset()" after 
// every autoplayer-action.
unsigned int CStableFramesCounter::UpdateNumberOfStableFrames()
{
	write_log(preferences.debug_stableframescounter(), "[CStableFramesCounter] Update number of stable frames\n");

	if (_isReset)
	{
		// Counter got reset, e.g. after an autoplayer-action.
		write_log(preferences.debug_stableframescounter(), "[CStableFramesCounter] Counter got reset: no stable frames yet\n");

		// Remember current values as last known ones.
		SaveCurrentState();
		
		// Nothing to compare - now we have 0 identical frames.
		_isReset = false;
		_NumberOfStableFrames = 0;

		return _NumberOfStableFrames; 
	}
	
	// These items need to be the same to count as a identical frame:
	// - up and down cards
	// - button position
	// - playerbets
	// - playerbalances
	// - button states

	bool same_scrape = true;

	if (_myturnbitslast != p_symbol_engine_autoplayer->myturnbits())
		same_scrape = false;

	for (int i=0; i<k_number_of_community_cards; i++)
	{
		if(!same_scrape)
			break;

    if (p_table_state->_common_cards[i].GetValue() != _card_common_last[i])  
		{
			same_scrape = false;
			write_log(preferences.debug_stableframescounter(), "[CStableFramesCounter] Community-cards don't match\n");
		}
	}

	for (int i=0; i<k_max_number_of_players; i++)
	{
		if(!same_scrape)
			break;

		write_log(preferences.debug_stableframescounter(), "[CStableFramesCounter] Checking player: %d\n", i);

    if (p_table_state->_players[i]._hole_cards[0].GetValue() != _card_player_last[i][0])	
		{
			same_scrape = false;
			write_log(preferences.debug_stableframescounter(), "[CStableFramesCounter] Player%d-cards don't match\n", i);
		}
		else if (p_table_state->_players[i]._hole_cards[1].GetValue() != _card_player_last[i][1])	
		{
			same_scrape = false;
			write_log(preferences.debug_stableframescounter(), "[CStableFramesCounter] Player%d-cards don't match\n", i);
		}
		else if (p_scraper->dealer(i)		 != _dealer_last[i])			
		{
			same_scrape = false;
			write_log(preferences.debug_stableframescounter(), "[CStableFramesCounter] Dealer%d-position does not match\n", i);
		}
		else if (p_table_state->_players[i]._balance != _playerbalance_last[i])	
		{
			same_scrape = false;
			write_log(preferences.debug_stableframescounter(), "[CStableFramesCounter] Player%d-balance does not match\n", i);
		}
		else if (p_scraper->player_bet(i)	 != _playerbet_last[i])		
		{
			same_scrape = false;
			write_log(preferences.debug_stableframescounter(), "[CStableFramesCounter] Player%d-bet does not match\n", i);
		}
	}

	if (same_scrape)
	{
		write_log(preferences.debug_stableframescounter(), "[CStableFramesCounter] Increasing number of stable frames\n");
		_NumberOfStableFrames++;
	}
	else
	{
		// Unstable frame
		write_log(preferences.debug_stableframescounter(), "[CStableFramesCounter] Gamestate unstable: resetting counter\n");
		Reset();
	}

	write_log(preferences.debug_stableframescounter(), "[CStableFramesCounter] Number of stable frames: %d\n", _NumberOfStableFrames);

	return _NumberOfStableFrames;
}