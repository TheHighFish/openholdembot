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
#include "CSymbolEngineTableLimits.h"

#include <assert.h>
#include "CBetroundCalculator.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CSymbolEngineDealerchair.h"
#include "CSymbolEngineIsTournament.h"
#include "debug.h"
#include "FloatingPoint_Comparisions.h"
#include "MagicNumbers.h"
#include "Median.h"
#include "Numericalfunctions.h"


CSymbolEngineTableLimits	*p_symbol_engine_tablelimits = NULL;


CSymbolEngineTableLimits::CSymbolEngineTableLimits()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	assert(p_symbol_engine_dealerchair != NULL);
	//
	// Actually this symbol-engine also depends on 
	// p_symbol_engine_is_tournament.
	// This is name circular dependency, but this does not really matter,
	// as the value of istournament() should be constant for the
	// entire session, so it does not matter, if we use old values
	// from the last heartbeat.
}

CSymbolEngineTableLimits::~CSymbolEngineTableLimits()
{
}


void CSymbolEngineTableLimits::InitOnStartup()
{
	ResetOnConnection();
}

void CSymbolEngineTableLimits::ResetOnConnection()
{
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] ResetOnConnection()\n");
	blinds_locked_for_complete_session = false;
	number_of_saved_tablelimits = 0;
	for (int i=0; i<k_number_of_hands_to_autolock_blinds_for_cashgames; i++)
	{
		tablelimits_first_N_hands_sblind[i] = 0;
		tablelimits_first_N_hands_bblind[i] = 0;
		tablelimits_first_N_hands_bbet[i]   = 0;
	}
	_ante = 0;
	_gametype = k_gametype_unknown;
	ResetOnHandreset();
}

void CSymbolEngineTableLimits::ResetOnHandreset()
{
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] ResetOnHandreset()\n");
	blinds_locked_for_current_hand = false;
	tablelimit_locked_for_current_hand.sblind = 0;
	tablelimit_locked_for_current_hand.bblind = 0;
	tablelimit_locked_for_current_hand.bbet   = 0;
	// We have to reset the known good values also,
	// * as the blinds could change (tournament)
	// * as they could be wrong and we would assume,
	//   that we know the blinds and don't calculate anything.
	tablelimit_last_known_good_value.sblind = 0;
	tablelimit_last_known_good_value.bblind = 0;
	tablelimit_last_known_good_value.bbet   = 0;
	ResetBets();
}

void CSymbolEngineTableLimits::ResetOnNewRound()
{}

void CSymbolEngineTableLimits::ResetOnMyTurn()
{}

void CSymbolEngineTableLimits::ResetOnHeartbeat()
{
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] ResetOnHeartbeat()\n");
	tablelimit_unreliable_input.sblind = 0;
	tablelimit_unreliable_input.bblind = 0;
	tablelimit_unreliable_input.bbet = 0;
}

void CSymbolEngineTableLimits::ResetBets()
{
	for (int i=k_betround_preflop; i<=k_betround_river; i++)
	{
		SetBet(i, 0);
	}
}


void CSymbolEngineTableLimits::AutoLockBlindsForCashgamesAfterNHands()
{
	if (p_symbol_engine_istournament == NULL)
	{
		// Initialization phase
		// Null pointer possible due to circular dependency
		// Simply leave because it is too early to lock the blinds
		return;
	}
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] AutoLockBlindsForCashgamesAfterNHands()\n");
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] blinds_locked_for_complete_session: %d\n", blinds_locked_for_complete_session);
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] istournament: %d\n", IsTrue(p_symbol_engine_istournament->istournament()));
	if (blinds_locked_for_complete_session || p_symbol_engine_istournament->istournament())
	{
		write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] blinds_locked_for_complete_session or istournament\n");
		write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] Leaving CSymbolEngineTableLimits::AutoLockBlindsForCashgamesAfterNHands() early\n");
		return;
	}
	if (number_of_saved_tablelimits == k_number_of_hands_to_autolock_blinds_for_cashgames)
	{
		// We simply take the median as the "correct" value.
		// This works, as long as less than half of the values are too small
		// and less than half of the values are too high.
		// Rasonable assumption, otherwise we have name serious problem anyway.
		tablelimit_locked_for_complete_session.sblind = median(tablelimits_first_N_hands_sblind, k_number_of_hands_to_autolock_blinds_for_cashgames);
		tablelimit_locked_for_complete_session.bblind = median(tablelimits_first_N_hands_bblind, k_number_of_hands_to_autolock_blinds_for_cashgames);
		tablelimit_locked_for_complete_session.bbet   = median(tablelimits_first_N_hands_bbet,   k_number_of_hands_to_autolock_blinds_for_cashgames);
		blinds_locked_for_complete_session = true;
		write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] Blinds locked at %f / %f / %f\n", tablelimit_locked_for_complete_session.sblind, 
			tablelimit_locked_for_complete_session.bblind, tablelimit_locked_for_complete_session.bbet);
	}
	else
	{
		write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] Not yet enough hands to lock blinds permanent.\n");
	}
}

bool CSymbolEngineTableLimits::ReasonableBlindsForCurrentHand()
{
	// The blinds are "reasonable" after the calculation (with non-zero-input),
	// so we check only for non-zero blinds.
	// We do also wait until it is our turn, as here the frames are more stable
	// and our turn is the decisive point of time.
	// (We don't wait for isfinalanswer, as that seems to be calculated afterwards;
	// at least it takes dll$iswait into consideration, which is calculated afterwards.)
	// (ismyturn doesn't work either)
	// Pros/cons: more reliability, but no auto-locking before it is our turn.
	// Affects maybe DLL- and Perl-people negative at the beginning of the first hands.
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] ismyturn: %d\n", p_scraper_access->IsMyTurn());
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] sblind: %f\n", tablelimit_unreliable_input.sblind);
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] bblind: %f\n", tablelimit_unreliable_input.bblind);
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] bbet: %f\n", tablelimit_unreliable_input.bbet); 
	if (/*p_symbol_engine_autoplayer->ismyturn() && */ (tablelimit_unreliable_input.sblind >= 0.01) 
		&& (tablelimit_unreliable_input.bblind >= tablelimit_unreliable_input.sblind) 
		&& (tablelimit_unreliable_input.bbet   >= tablelimit_unreliable_input.bblind))
	{
		write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] ReasonableBlindsForCurrentHand(): true\n");
		return true;
	}
	else
	{
		write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] ReasonableBlindsForCurrentHand(): false\n");
		return false;
	}
}

void CSymbolEngineTableLimits::RememberBlindsForCashgames()
{
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] RememberBlindsForCashgames()\n");
	if (number_of_saved_tablelimits < k_number_of_hands_to_autolock_blinds_for_cashgames)
	{
		write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] Saving blinds for hand %d\n", number_of_saved_tablelimits);
		tablelimits_first_N_hands_sblind[number_of_saved_tablelimits] = tablelimit_locked_for_current_hand.sblind;
		tablelimits_first_N_hands_bblind[number_of_saved_tablelimits] = tablelimit_locked_for_current_hand.bblind;
		tablelimits_first_N_hands_bbet[number_of_saved_tablelimits]   = tablelimit_locked_for_current_hand.bbet;
		number_of_saved_tablelimits++;
	}
	else
	{
		write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] Already enough saved blinds.\n");
	}
}

void CSymbolEngineTableLimits::AutoLockBlindsForCurrentHand()
{
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] AutoLockBlindsForCurrentHand()\n");
	blinds_locked_for_current_hand = true;
	tablelimit_locked_for_current_hand.sblind = tablelimit_last_known_good_value.sblind;
	tablelimit_locked_for_current_hand.bblind = tablelimit_last_known_good_value.bblind;
	tablelimit_locked_for_current_hand.bbet	  = tablelimit_last_known_good_value.bbet;
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] Locked blinds at %f / %f / %f\n", tablelimit_locked_for_current_hand.sblind,
		tablelimit_locked_for_current_hand.bblind, tablelimit_locked_for_current_hand.bbet);
	RememberBlindsForCashgames();
}

void CSymbolEngineTableLimits::SetSmallBlind(double small_blind)
{
	assert(small_blind >= 0);
	tablelimit_unreliable_input.sblind = small_blind; 
}

void CSymbolEngineTableLimits::SetBigBlind(double big_blind)
{
	assert(big_blind >= 0);
	tablelimit_unreliable_input.bblind = big_blind;
}

void CSymbolEngineTableLimits::SetBigBet(double big_bet)
{
	assert(big_bet >= 0);
	tablelimit_unreliable_input.bbet = big_bet;
}

void CSymbolEngineTableLimits::SetAnte(double ante)
{
	assert(ante >= 0);
	_ante = ante;
}

void CSymbolEngineTableLimits::SetGametype(int gametype)
{
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] SetGametype() new gametype: %d\n", gametype);
	assert(gametype >= k_gametype_unknown);
	assert(gametype <= k_gametype_FL);
	_gametype = gametype;
}

void CSymbolEngineTableLimits::SetBet(int betround, double bet)
{
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] SetBet()\n");
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] New bet for betround[%d]: %f\n", betround, bet);
	assert(betround >= k_betround_preflop);
	assert(betround <= k_betround_river);
	assert(bet >= 0);
	_betsizes_for_all_bettingrounds[betround] = bet;
}

void CSymbolEngineTableLimits::AutoLockBlinds()
{
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] AutoLockBlinds()\n");
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] blinds_locked_for_current_hand: %d\n", blinds_locked_for_current_hand);
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] ReasonableBlindsForCurrentHand(): %d\n", ReasonableBlindsForCurrentHand());
	if (!blinds_locked_for_current_hand && ReasonableBlindsForCurrentHand())
	{
		AutoLockBlindsForCurrentHand();
		AutoLockBlindsForCashgamesAfterNHands();
	}
}

void CSymbolEngineTableLimits::CalcTableLimits_NL_PL()
{
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] CalcTableLimits_NL_PL()\n");
	if (tablelimit_unreliable_input.sblind==0)
	{
		if (p_scraper->s_limit_info()->found_sb_bb)
			SetSmallBlind(p_scraper->s_limit_info()->sb_bb);
	}
	if (tablelimit_unreliable_input.bblind==0)
	{
		if (p_scraper->s_limit_info()->found_bb_BB)
			SetBigBlind(p_scraper->s_limit_info()->bb_BB);
	}
	if (tablelimit_unreliable_input.bbet==0)
	{
		if (p_scraper->s_limit_info()->found_bb_BB)
			SetBigBet(p_scraper->s_limit_info()->bb_BB);
		else if (p_scraper->s_limit_info()->found_sb_bb)
			SetBigBet(p_scraper->s_limit_info()->sb_bb*2);
		else if (tablelimit_unreliable_input.bblind!=0)
			SetBigBet(tablelimit_unreliable_input.bblind);
		else if (tablelimit_unreliable_input.sblind!=0)
			SetBigBet(tablelimit_unreliable_input.sblind*2);
	}
}

bool CSymbolEngineTableLimits::IsCalculationNeccessary()
{
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] IsCalculationNeccessary()\n");
	if (tablelimit_unreliable_input.sblind <= 0)
	{
		write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] sblind=0, calculation neccessary.\n");
		return true;
	}
	if (tablelimit_unreliable_input.bblind <= 0)
	{
		write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] bblind=0, calculation neccessary.\n");
		return true;
	}
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] no calculation neccessary.\n");
	return false;
}

void CSymbolEngineTableLimits::CalcTableLimits_FL_AndUnknownGametype()
{
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] CalcTableLimits_FL_AndUnknownGametype()\n");
	if (tablelimit_unreliable_input.sblind==0)
	{
		if (p_scraper->s_limit_info()->found_sb_bb)
			SetSmallBlind(p_scraper->s_limit_info()->sb_bb);
	}
	if (tablelimit_unreliable_input.bblind==0)
	{
		if (p_scraper->s_limit_info()->found_bb_BB)
			SetBigBlind(p_scraper->s_limit_info()->bb_BB);
	}
	if (tablelimit_unreliable_input.bbet==0)
	{
		if (p_scraper->s_limit_info()->found_bb_BB)
			SetBigBet(p_scraper->s_limit_info()->bb_BB*2);
		else if (p_scraper->s_limit_info()->found_sb_bb)
			SetBigBet(p_scraper->s_limit_info()->sb_bb*4);
		else if (tablelimit_unreliable_input.bblind!=0)
			SetBigBet(tablelimit_unreliable_input.bblind*2);
		else if (tablelimit_unreliable_input.sblind!=0)
			SetBigBet(tablelimit_unreliable_input.sblind*4);
	}
}

void CSymbolEngineTableLimits::SwapBlindsIfSbGreaterThanBBAndNotZero()
{
	// If SB is greater than BB that is usually name mistake and we should swap.
	// But we don't swap, if BB is zero, as that does mean, 
	// that we just got the amount wrong (BB got cards, i.e. dealt, but no bet).
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] SwapBlindsIfSbGreaterThanBBAndNotZero()\n");
	if ((tablelimit_unreliable_input.sblind > tablelimit_unreliable_input.bblind)
		&& (tablelimit_unreliable_input.bblind >= 0.01))
	{
		write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] SwapBlindsIfSbGreaterThanBBAndNotZero swap neccessary\n");
		SwapDoubles(&tablelimit_unreliable_input.sblind, &tablelimit_unreliable_input.bblind);
	}
}

void CSymbolEngineTableLimits::SearchTableForSbAndBbValue()
{
	bool	found_inferred_sb = false, found_inferred_bb = false;

	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] SearchTableForSbAndBbValue()\n");
	for (int i=1; i<=p_tablemap->nchairs(); i++)
	{
		int next_chair = (p_symbol_engine_dealerchair->dealerchair() + i) % p_tablemap->nchairs();
		// We do no longer require cards for the players,
		// as this mixed things up, when e.g. SB folds.
		// We try to look directly at the bets.
		//
		// But not looking for cards is bad either, 
		// as sometimes we don't get the bet correctly 
		// and then end up with the bet of another user.
		// If we find cards without name necessary blind
		// then we will try it as unknown (zero).
		if (p_scraper->player_bet(next_chair) >= 0.01)
		{
			write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] SearchTableForSbAndBbValue found bet for chair %d\n", next_chair);
			// Bet found
			if (!found_inferred_sb)
			{
				// Not yet SB known
				if (tablelimit_unreliable_input.sblind==0)
				{
					SetSmallBlind(p_scraper->player_bet(next_chair));
					write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] found inferred SB at chair %d: %f\n", next_chair, p_scraper->player_bet(next_chair));
					found_inferred_sb = true;
				}
			}
			else if (!found_inferred_bb)
			{
				// Not yet BB known
				if (tablelimit_unreliable_input.bblind==0)
				{
					// Not heads up - normal blinds
					if (next_chair != p_symbol_engine_dealerchair->dealerchair())
					{
						SetBigBlind(p_scraper->player_bet(next_chair));
						write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] found inferred BB at chair %d: %f\n", next_chair, p_scraper->player_bet(next_chair));
					}
					// heads up - reversed blinds
					else
					{
						SetBigBlind(tablelimit_unreliable_input.sblind);
						SetSmallBlind(p_scraper->player_bet(next_chair));
						write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] found inferred BB headsup at chair %d: %f\n", next_chair, p_scraper->player_bet(next_chair));
						write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] Swapping blinds.\n");
					}
					found_inferred_bb = true;
				}
				break;
			}
		}
		else if ((p_scraper->card_player(next_chair, 0) != CARD_NOCARD)  
			&& (p_scraper->card_player(next_chair, 0) != CARD_NOCARD))
		{
			write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] SearchTableForSbAndBbValue found no bet for chair %d\n", next_chair);
			write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] SearchTableForSbAndBbValue but found cards for chair %d\n", next_chair);
			// Awful. Found cards, but not the expected blind.
			// Either name misread, or name blind is missing.
			// Read it as zero and try to guess it later correctly.
			if (!found_inferred_sb)
			{
				found_inferred_sb = true;
				write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] expected SB, but not found\n");
				// Don't set any value.
				// Who knows, we might even have correct data 
				// from name previous round!? 
			}
			else if (!found_inferred_bb)
			{
				found_inferred_bb = true;
				write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] expected BB, but not found\n");
				// Don't set any value.
				// Who knows, we might even have correct data 
				// from name previous round!? 
				break;
			}
		}
		else
		{
			write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] SearchTableForSbAndBbValue found neither bet nor cards for chair %d\n", next_chair);
		}
	}
	SwapBlindsIfSbGreaterThanBBAndNotZero();
}

double CSymbolEngineTableLimits::GuessSmallBlindFromBigBlind()
{
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] GuessSmallBlindFromBigBlind()\n");
	// Special case: 0.02/0.05/0.10
	if (IsEqual(tablelimit_unreliable_input.bblind, 0.05))
	{
		return 0.02;
	}
	// Special case: 0.05/0.15/0.30
	if (IsEqual(tablelimit_unreliable_input.bblind, 0.15))
	{
		return 0.05;
	}
	// Special case: 0.10/0.25/0.50
	if (IsEqual(tablelimit_unreliable_input.bblind, 0.25))
	{
		return 0.10;
	}
	// Special case: 0.02/0.05
	if (IsEqual(tablelimit_unreliable_input.bblind, 0.05))
	{
		return 0.02;
	}
	return (tablelimit_unreliable_input.bblind / 2);
}

double CSymbolEngineTableLimits::GuessBigBlindFromSmallBlind()
{	// Special cases: NL 0.10/0.25.
	// Some casinos do also provide NL 0.10/0.20,
	// but that is rare, we do assume 0.10/0.25 here.
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] GuessBigBlindFromSmallBlind()\n");
	if (gametype() == k_gametype_NL && IsEqual(tablelimit_unreliable_input.sblind, 0.10))
	{
		return 0.25;
	}
	return (tablelimit_unreliable_input.sblind * 2);
}

double CSymbolEngineTableLimits::GuessBigBetFromBigBlind()
{
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] GuessBigBetFromBigBlind()\n");
	if ((gametype() == k_gametype_NL) || (gametype() == k_gametype_PL))
	{
		write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] GuessBigBetFromBigBlind() BB = bb\n");
		return (tablelimit_unreliable_input.bblind);
	}
	else
	{
		write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] GuessBigBetFromBigBlind() BB = 2*bb\n");
		return (tablelimit_unreliable_input.bblind*2);
	}
}

void CSymbolEngineTableLimits::AdjustForReasonableness()
{
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] CheckForReasonableness()\n");
	// SB unknown?
	if (IsSmallerOrEqual(tablelimit_unreliable_input.sblind, 0))
	{
		SetSmallBlind(GuessSmallBlindFromBigBlind());
			write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] adjusting SB\n");
	}
	// BB unknown?
	if (IsSmallerOrEqual(tablelimit_unreliable_input.bblind, 0))
	{
		SetBigBlind(GuessBigBlindFromSmallBlind());
			write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] adjusting BB\n");
	}
	// BB out of range?
	if (IsGreater(tablelimit_unreliable_input.bblind, (tablelimit_unreliable_input.sblind*3)))
	{
		if (IsGreater(tablelimit_unreliable_input.sblind, 0))
		{
			SetBigBlind(GuessBigBlindFromSmallBlind());
		}
	}
	// SB out of range?
	if (IsGreaterOrEqual(tablelimit_unreliable_input.sblind, tablelimit_unreliable_input.bblind))
	{
		// Formerly this code was checking for SB > BB,
		// but up to now we never saw an online-game with identical blinds.
		// SB >= BB does usually mean, that SB did already act and complete.
		SetSmallBlind(GuessSmallBlindFromBigBlind());
		write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] : adjusting SB\n");
	}
	// Always force big-bet = 2 * big-blind
	SetBigBet(GuessBigBetFromBigBlind());
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] adjusting big bet\n");
}

void CSymbolEngineTableLimits::CalcBetsizesForEveryStreet()
{
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] CalcBetsizesForEveryStreet()\n");
	// Use the best  known values here, not tablelimit_unreliable_input
	SetBet(k_betround_preflop, bblind());															
	SetBet(k_betround_flop,    bblind());															
	SetBet(k_betround_turn,    bigbet()>=0.01 ? bigbet() : (isnl() || ispl() ? bblind() : bblind()*2));	
	SetBet(k_betround_river,   bigbet()>=0.01 ? bigbet() : (isnl() || ispl() ? bblind() : bblind()*2));	
}

void CSymbolEngineTableLimits::CalcTableLimits()
{
	// This is basically the old function CSymbols::CalcStakes()
	// with some extension at the end to auto-lock the blinds,
	// if the values are reasonable.
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] CalcTableLimits()\n");
	if (!IsCalculationNeccessary())
	{
		return;
	}
	SetSmallBlind(0);
	SetBigBlind(0);
	SetBigBet(0);
	SetAnte(0);

	// Save the parts we scraped successfully
	if (p_scraper->s_limit_info()->found_sblind)
		SetSmallBlind(p_scraper->s_limit_info()->sblind);								// sblind
	if (p_scraper->s_limit_info()->found_bblind)
		SetBigBlind(p_scraper->s_limit_info()->bblind);									// bblind
	if (p_scraper->s_limit_info()->found_ante)
		SetAnte(p_scraper->s_limit_info()->ante);										// ante
	if (p_scraper->s_limit_info()->found_limit)
		SetGametype(p_scraper->s_limit_info()->limit);									// lim
	if (p_scraper->s_limit_info()->found_bbet)
		SetBigBet(p_scraper->s_limit_info()->bbet);

	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] input from scraper: small blind: %f\n", tablelimit_unreliable_input.sblind);
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] input from scraper: big blind:   %f\n", tablelimit_unreliable_input.bblind);
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] input from scraper: big bet:     %f\n", tablelimit_unreliable_input.bbet);
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] input from scraper: gametype:    %d\n", _gametype);             
	// Figure out bb/sb based on game type
	if (gametype() == k_gametype_NL || gametype() == k_gametype_PL)
	{
		CalcTableLimits_NL_PL();
	}
	else if (gametype() == k_gametype_FL || gametype() == k_gametype_unknown)
	{
		CalcTableLimits_FL_AndUnknownGametype();
	}

	// if we still do not have blinds, then infer them from the posted bets
	if (p_betround_calculator->betround() == k_betround_preflop && (tablelimit_unreliable_input.sblind==0 || tablelimit_unreliable_input.bblind==0))
	{
		SearchTableForSbAndBbValue();			
	}

	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] calculated result: small blind: %f\n", tablelimit_unreliable_input.sblind);
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] calculated result: big blind:   %f\n", tablelimit_unreliable_input.bblind);
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] calculated result: big bet:     %f\n", tablelimit_unreliable_input.bbet);
	AdjustForReasonableness();
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] adjusted result: small blind: %f\n", tablelimit_unreliable_input.sblind);
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] adjusted result: big blind:   %f\n", tablelimit_unreliable_input.bblind);
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] adjusted result: big bet:     %f\n", tablelimit_unreliable_input.bbet);

	AcceptNewValuesIfGood();
	AutoLockBlinds();
	// Calc miminum betsizes for every streeet (after! we have potentially locked the blinds)
	CalcBetsizesForEveryStreet();
}

void CSymbolEngineTableLimits::AcceptNewValuesIfGood()
{
	if ((tablelimit_unreliable_input.sblind >= 0.01) 
		&& (tablelimit_unreliable_input.bblind > tablelimit_unreliable_input.sblind)
		&& (tablelimit_unreliable_input.bbet >= tablelimit_unreliable_input.bblind))
	{
		tablelimit_last_known_good_value.sblind = tablelimit_unreliable_input.sblind;
		tablelimit_last_known_good_value.bblind = tablelimit_unreliable_input.bblind;
		tablelimit_last_known_good_value.bbet   = tablelimit_unreliable_input.bbet;
	}
}

double CSymbolEngineTableLimits::sblind()
{
	if (blinds_locked_for_complete_session)
	{
		return tablelimit_locked_for_complete_session.sblind;
	}
	else if (blinds_locked_for_current_hand)
	{
		return tablelimit_locked_for_current_hand.sblind;
	}
	else
	{
		return tablelimit_last_known_good_value.sblind;
	}
}

double CSymbolEngineTableLimits::bblind()
{
	if (blinds_locked_for_complete_session)
	{
		return tablelimit_locked_for_complete_session.bblind;
	}
	else if (blinds_locked_for_current_hand)
	{
		return tablelimit_locked_for_current_hand.bblind;
	}
	else
	{
		return tablelimit_last_known_good_value.bblind;
	}
}

double CSymbolEngineTableLimits::bigbet()
{
	if (blinds_locked_for_complete_session)
	{
		return tablelimit_locked_for_complete_session.bbet;
	}
	else if (blinds_locked_for_current_hand)
	{
		return tablelimit_locked_for_current_hand.bbet;
	}
	else
	{
		return tablelimit_last_known_good_value.bbet;
	}
}

double CSymbolEngineTableLimits::ante()
{
	return _ante; 
}

double CSymbolEngineTableLimits::bet(int betround)
{
	assert(betround >= k_betround_preflop);
	assert(betround <= k_betround_river);
	return (_betsizes_for_all_bettingrounds[betround]);
}

double CSymbolEngineTableLimits::bet()
{
	return (bet(p_betround_calculator->betround()));
}

CString CSymbolEngineTableLimits::GetGametypeAsString()
{
	CString result = "";
	if (isnl())
	{
		result = "NL";
	}
	else if (ispl())
	{
		result = "PL";
	}
	else if (isfl())
	{
		result = "FL";
	}
	else
	{
		result = "?L";
	}
	if (p_symbol_engine_istournament->istournament())
	{
		result += "T";
	}
	return result;
}

bool CSymbolEngineTableLimits::EvaluateSymbol(const char *name, double *result, bool log /* = false */)
{
	if (memcmp(name, "is", 2)==0)
	{
		if (memcmp(name, "isnl", 4)==0 && strlen(name)==4)
		{
			*result = isnl();
		}
		else if (memcmp(name, "ispl", 4)==0 && strlen(name)==4)
		{
			*result = ispl();
		}
		else if (memcmp(name, "isfl", 4)==0 && strlen(name)==4)	
		{
			*result = isfl();
		}
		else
		{
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}
	else if (memcmp(name, "bet", 3)==0)
	{
		if (memcmp(name, "bet", 3)==0 && strlen(name)==3)
		{
			*result = bet();
		}
		else if (memcmp(name, "bet", 3)==0 && strlen(name)==4)	
		{
			*result = bet(name[3]-'0');
		}
		else
		{
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}
	if (memcmp(name, "bblind", 6)==0 && strlen(name)==6)		
	{
		*result = bblind();
	}
	else if (memcmp(name, "sblind", 6)==0 && strlen(name)==6)	
	{
		*result = sblind();
	}
	else if (memcmp(name, "ante", 4)==0 && strlen(name)==4)	
	{
		*result = ante();
	}
	else if (memcmp(name, "lim", 3)==0 && strlen(name)==3)	
	{
		*result = gametype();
	}
	else
	{
		// Symbol of a different symbol-engine
		return false;
	}
	// Valid symbol
	return true;
}

CString CSymbolEngineTableLimits::SymbolsProvided() {
  return "isnl ispl isfl lim bet bblind sblind ante ";
}