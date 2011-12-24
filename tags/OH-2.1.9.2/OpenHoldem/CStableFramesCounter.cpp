#include "stdafx.h"

#include "CPreferences.h"
#include "CScraper.h"
#include "CStableFramesCounter.h"
#include "CSymbols.h"
#include "MagicNumbers.h"

// We create s single global instance,
// as the counter is needed for both autoplayer and PPro.
CStableFramesCounter *p_stableframescounter = NULL;


CStableFramesCounter::CStableFramesCounter()
{
	Reset();
}

CStableFramesCounter::~CStableFramesCounter()
{}

void CStableFramesCounter::Reset()
{
	write_log(prefs.debug_stableframescounter(), "[CStableFramesCounter] Reset\n");
	// Reset it to -1, so we get 0 identical frames, 
	// when we increment it the next time.
	_NumberOfStableFrames = -1;
}

void CStableFramesCounter::ResetOnAutoplayerAction()
{
	write_log(prefs.debug_stableframescounter(), "[CStableFramesCounter] Reset on autoplayer-action\n");
	Reset();
}

int CStableFramesCounter::UpdateNumberOfStableFrames()
{
	// Static variables to keep the last game-state.
	static unsigned int	card_common_last[k_number_of_community_cards] = {0};
	static unsigned int	card_player_last[k_max_number_of_players][k_number_of_cards_per_player] = {0};
	static bool			dealer_last[k_max_number_of_players] = {0};
	static double		playerbalance_last[k_max_number_of_players] = {0};
	static double		playerbet_last[k_max_number_of_players] = {0};
	static double		myturnbitslast = 0;

	write_log(prefs.debug_stableframescounter(), "[CStableFramesCounter] Update number of stable frames\n");
	if (_NumberOfStableFrames < 0)
	{
		write_log(prefs.debug_stableframescounter(), "[CStableFramesCounter] Counter got reset: no stable frames yet\n");
		// Counter got reset, e.g. after an autoplayer-action.
		// Nothing to compare - now we have 0 identical frames.
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
	for (int i=0; i<k_number_of_community_cards; i++)
		if (p_scraper->card_common(i) != card_common_last[i])  
		{
			same_scrape = false;
			write_log(prefs.debug_stableframescounter(), "[CStableFramesCounter] Community-cards don't match\n");
		}

	for (int i=0; i<k_max_number_of_players; i++)
	{
		write_log(prefs.debug_stableframescounter(), "[CStableFramesCounter] Checking player: %d\n", i);
		if (p_scraper->card_player(i, 0) != card_player_last[i][0])	
		{
			same_scrape = false;
			write_log(prefs.debug_stableframescounter(), "[CStableFramesCounter] Player-cards don't match\n");
		}
		else if (p_scraper->card_player(i, 1) != card_player_last[i][1])	
		{
			same_scrape = false;
			write_log(prefs.debug_stableframescounter(), "[CStableFramesCounter] Player-cards don't match\n");
		}
		else if (p_scraper->dealer(i)		 != dealer_last[i])			
		{
			same_scrape = false;
			write_log(prefs.debug_stableframescounter(), "[CStableFramesCounter] Dealer-position does not match\n");
		}
		else if (p_scraper->player_balance(i) != playerbalance_last[i])	
		{
			same_scrape = false;
			write_log(prefs.debug_stableframescounter(), "[CStableFramesCounter] Balance does not match\n");
		}
		else if (p_scraper->player_bet(i)	 != playerbet_last[i])		
		{
			same_scrape = false;
			write_log(prefs.debug_stableframescounter(), "[CStableFramesCounter] bet does not match\n");
		}
		if (!same_scrape)
		{
			// Quick exit
			break;
		}
	}

	int e = SUCCESS;
	int sym_myturnbits = (int) p_symbols->sym()->myturnbits;

	if (sym_myturnbits != myturnbitslast)  same_scrape = false;

	if (same_scrape)
	{
		write_log(prefs.debug_stableframescounter(), "[CStableFramesCounter] Increasing number of stable frames\n");
		_NumberOfStableFrames++;
	}
	else
	{
		// Unstable frame
		// Remember current values as last known ones.
		write_log(prefs.debug_stableframescounter(), "[CStableFramesCounter] Gamestate unstable: resetting counter\n");
		for (int i=0; i<k_number_of_community_cards; i++)
		{
			card_common_last[i] = p_scraper->card_common(i);
		}
		for (int i=0; i<k_max_number_of_players; i++)
		{
			card_player_last[i][0]	= p_scraper->card_player(i, 0);
			card_player_last[i][1]	= p_scraper->card_player(i, 1);
			dealer_last[i]			= p_scraper->dealer(i);
			playerbalance_last[i]	= p_scraper->player_balance(i);
			playerbet_last[i]		= p_scraper->player_bet(i);
		}
		myturnbitslast = sym_myturnbits;
		_NumberOfStableFrames = 0;
	}
	write_log(prefs.debug_stableframescounter(), "[CStableFramesCounter] Number of stable frames: %d\n", _NumberOfStableFrames);
	return _NumberOfStableFrames;
}