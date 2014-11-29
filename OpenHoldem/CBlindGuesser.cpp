//****************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//****************************************************************************** 
//
// Purpose: first guess of blind values for CTableLimits,
//   which might be overridden bz blind-locking.
//
//****************************************************************************** 

#include "stdafx.h"
#include "CBlindGuesser.h"

CBlindGuesser::CBlindGuesser() {
}

CBlindGuesser::~CBlindGuesser() {
}

void CBlindGuesser::Guess(double *sblind, double *bblind, double *bbet) {
}

/*




void CSymbolEngineTableLimits::CalcTableLimits_NL_PL()
{
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] CalcTableLimits_NL_PL()\n");
	if (tablelimit_unreliable_input.sblind==0)
	{
		if (p_scraper->s_limit_info()->sb_bb > 0)
			SetSmallBlind(p_scraper->s_limit_info()->sb_bb);
	}
	if (tablelimit_unreliable_input.bblind==0)
	{
		if (p_scraper->s_limit_info()->bb_BB > 0)
			SetBigBlind(p_scraper->s_limit_info()->bb_BB);
	}
	if (tablelimit_unreliable_input.bbet==0)
	{
		if (p_scraper->s_limit_info()->bb_BB > 0)
			SetBigBet(p_scraper->s_limit_info()->bb_BB);
		else if (p_scraper->s_limit_info()->sb_bb > 0)
			SetBigBet(p_scraper->s_limit_info()->sb_bb*2);
		else if (tablelimit_unreliable_input.bblind > 0)
			SetBigBet(tablelimit_unreliable_input.bblind);
		else if (tablelimit_unreliable_input.sblind > 0)
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
		if (p_scraper->s_limit_info()->sb_bb > 0)
			SetSmallBlind(p_scraper->s_limit_info()->sb_bb);
	}
	if (tablelimit_unreliable_input.bblind==0)
	{
		if (p_scraper->s_limit_info()->bb_BB > 0)
			SetBigBlind(p_scraper->s_limit_info()->bb_BB);
	}
	if (tablelimit_unreliable_input.bbet==0)
	{
		if (p_scraper->s_limit_info()->bb_BB > 0)
			SetBigBet(p_scraper->s_limit_info()->bb_BB*2);
		else if (p_scraper->s_limit_info()->sb_bb > 0)
			SetBigBet(p_scraper->s_limit_info()->sb_bb*4);
		else if (tablelimit_unreliable_input.bblind >0)
			SetBigBet(tablelimit_unreliable_input.bblind*2);
		else if (tablelimit_unreliable_input.sblind > 0)
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
    else if (p_table_state->_players[next_chair].HasAnyCards())
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
}*/