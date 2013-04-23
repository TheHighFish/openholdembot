#include "stdafx.h"
#include "CTableLimits.h"

#include <assert.h>
#include "CBetroundCalculator.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineDealerchair.h"
#include "debug.h"
#include "FloatingPoint_Comparisions.h"
#include "MagicNumbers.h"
#include "Median.h"
#include "Numericalfunctions.h"


CTableLimits	*p_tablelimits = NULL;


CTableLimits::CTableLimits()
{
	ResetOnConnection();
}

CTableLimits::~CTableLimits()
{
}

void CTableLimits::ResetOnConnection()
{
	write_log(3, "CTableLimits::ResetOnConnection()\n");
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

void CTableLimits::ResetOnHandreset()
{
	write_log(3, "CTableLimits::ResetOnHandreset()\n");
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

void CTableLimits::ResetEachHeartBeatCycle()
{
	write_log(3, "CTableLimits::ResetEachHeartBeatCycle()\n");
	tablelimit_unreliable_input.sblind = 0;
	tablelimit_unreliable_input.bblind = 0;
	tablelimit_unreliable_input.bbet = 0;
}

void CTableLimits::ResetBets()
{
	for (int i=k_betround_preflop; i<=k_betround_river; i++)
	{
		SetBet(i, 0);
	}
}


void CTableLimits::AutoLockBlindsForCashgamesAfterNHands()
{
	write_log(3, "CTableLimits::AutoLockBlindsForCashgamesAfterNHands()\n");
	write_log(3, "CTableLimits: blinds_locked_for_complete_session: %d\n", blinds_locked_for_complete_session);
	write_log(3, "CTableLimits: istournament: %d\n", IsTrue(p_tablelimits->istournament()));
	if (blinds_locked_for_complete_session || p_tablelimits->istournament())
	{
		write_log(3, "CTableLimits: blinds_locked_for_complete_session or istournament\n");
		write_log(3, "CTableLimits: Leaving CTableLimits::AutoLockBlindsForCashgamesAfterNHands() early\n");
		return;
	}
	if (number_of_saved_tablelimits == k_number_of_hands_to_autolock_blinds_for_cashgames)
	{
		// We simply take the median as the "correct" value.
		// This works, as long as less than half of the values are too small
		// and less than half of the values are too high.
		// Rasonable assumption, otherwise we have a serious problem anyway.
		tablelimit_locked_for_complete_session.sblind = median(tablelimits_first_N_hands_sblind, k_number_of_hands_to_autolock_blinds_for_cashgames);
		tablelimit_locked_for_complete_session.bblind = median(tablelimits_first_N_hands_bblind, k_number_of_hands_to_autolock_blinds_for_cashgames);
		tablelimit_locked_for_complete_session.bbet   = median(tablelimits_first_N_hands_bbet,   k_number_of_hands_to_autolock_blinds_for_cashgames);
		blinds_locked_for_complete_session = true;
		write_log(3, "Blinds locked at %f / %f / %f\n", tablelimit_locked_for_complete_session.sblind, 
			tablelimit_locked_for_complete_session.bblind, tablelimit_locked_for_complete_session.bbet);
	}
	else
	{
		write_log(3, "Not yet enough hands to lock blinds permanent.\n");
	}
}

bool CTableLimits::ReasonableBlindsForCurrentHand()
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
	write_log(3, "CTableLimits: ismyturn: %d\n", p_symbol_engine_autoplayer->ismyturn());
	write_log(3, "CTableLimits: sblind: %f\n", tablelimit_unreliable_input.sblind);
	write_log(3, "CTableLimits: bblind: %f\n", tablelimit_unreliable_input.bblind);
	write_log(3, "CTableLimits: bbet: %f\n", tablelimit_unreliable_input.bbet); 
	if (/*p_symbol_engine_autoplayer->ismyturn() && */ (tablelimit_unreliable_input.sblind >= 0.01) 
		&& (tablelimit_unreliable_input.bblind >= tablelimit_unreliable_input.sblind) 
		&& (tablelimit_unreliable_input.bbet   >= tablelimit_unreliable_input.bblind))
	{
		write_log(3, "CTableLimits: ReasonableBlindsForCurrentHand(): true\n");
		return true;
	}
	else
	{
		write_log(3, "CTableLimits: ReasonableBlindsForCurrentHand(): false\n");
		return false;
	}
}

void CTableLimits::RememberBlindsForCashgames()
{
	write_log(3, "CTableLimits::RememberBlindsForCashgames()\n");
	if (number_of_saved_tablelimits < k_number_of_hands_to_autolock_blinds_for_cashgames)
	{
		write_log(3, "CTableLimits: Saving blinds for hand %d\n", number_of_saved_tablelimits);
		tablelimits_first_N_hands_sblind[number_of_saved_tablelimits] = tablelimit_locked_for_current_hand.sblind;
		tablelimits_first_N_hands_bblind[number_of_saved_tablelimits] = tablelimit_locked_for_current_hand.bblind;
		tablelimits_first_N_hands_bbet[number_of_saved_tablelimits]   = tablelimit_locked_for_current_hand.bbet;
		number_of_saved_tablelimits++;
	}
	else
	{
		write_log(3, "CTableLimits: Already enough saved blinds.\n");
	}
}

void CTableLimits::AutoLockBlindsForCurrentHand()
{
	write_log(3, "CTableLimits::AutoLockBlindsForCurrentHand()\n");
	blinds_locked_for_current_hand = true;
	tablelimit_locked_for_current_hand.sblind = tablelimit_last_known_good_value.sblind;
	tablelimit_locked_for_current_hand.bblind = tablelimit_last_known_good_value.bblind;
	tablelimit_locked_for_current_hand.bbet	  = tablelimit_last_known_good_value.bbet;
	write_log(3, "CTableLimits: Locked blinds at %f / %f / %f\n", tablelimit_locked_for_current_hand.sblind,
		tablelimit_locked_for_current_hand.bblind, tablelimit_locked_for_current_hand.bbet);
	RememberBlindsForCashgames();
}

void CTableLimits::SetSmallBlind(double small_blind)
{
	assert(small_blind >= 0);
	tablelimit_unreliable_input.sblind = small_blind; 
}

void CTableLimits::SetBigBlind(double big_blind)
{
	assert(big_blind >= 0);
	tablelimit_unreliable_input.bblind = big_blind;
}

void CTableLimits::SetBigBet(double big_bet)
{
	assert(big_bet >= 0);
	tablelimit_unreliable_input.bbet = big_bet;
}

void CTableLimits::SetAnte(double ante)
{
	assert(ante >= 0);
	_ante = ante;
}

void CTableLimits::SetGametype(int gametype)
{
	write_log(3, "CTableLimits::SetGametype() new gametype: %d\n", gametype);
	assert(gametype >= k_gametype_unknown);
	assert(gametype <= k_gametype_FL);
	_gametype = gametype;
}

void CTableLimits::SetBet(int betround, double bet)
{
	write_log(3, "CTableLimits::SetBet()\n");
	write_log(3, "New bet for betround[%d]: %f\n", betround, bet);
	assert(betround >= k_betround_preflop);
	assert(betround <= k_betround_river);
	assert(bet >= 0);
	_betsizes_for_all_bettingrounds[betround] = bet;
}

void CTableLimits::AutoLockBlinds()
{
	write_log(3, "CTableLimits::AutoLockBlinds()\n");
	write_log(3, "CTableLimits: blinds_locked_for_current_hand: %d\n", blinds_locked_for_current_hand);
	write_log(3, "CTableLimits: ReasonableBlindsForCurrentHand(): %d\n", ReasonableBlindsForCurrentHand());
	if (!blinds_locked_for_current_hand && ReasonableBlindsForCurrentHand())
	{
		AutoLockBlindsForCurrentHand();
		AutoLockBlindsForCashgamesAfterNHands();
	}
}

void CTableLimits::CalcTableLimits_NL_PL()
{
	write_log(3, "CTableLimits::CalcTableLimits_NL_PL()\n");
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

bool CTableLimits::IsCalculationNeccessary()
{
	write_log(3, "CTableLimits::IsCalculationNeccessary()\n");
	if (tablelimit_unreliable_input.sblind <= 0)
	{
		write_log(3, "CTableLimits: sblind=0, calculation neccessary.\n");
		return true;
	}
	if (tablelimit_unreliable_input.bblind <= 0)
	{
		write_log(3, "CTableLimits: bblind=0, calculation neccessary.\n");
		return true;
	}
	write_log(3, "CTableLimits: no calculatoin neccessary.\n");
	return false;
}

void CTableLimits::CalcTableLimits_FL_AndUnknownGametype()
{
	write_log(3, "CTableLimits::CalcTableLimits_FL_AndUnknownGametype()\n");
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

void CTableLimits::SwapBlindsIfSbGreaterThanBBAndNotZero()
{
	// If SB is greater than BB that is usually a mistake and we should swap.
	// But we don't swap, if BB is zero, as that does mean, 
	// that we just got the amount wrong (BB got cards, i.e. dealt, but no bet).
	write_log(3, "CTableLimits::SwapBlindsIfSbGreaterThanBBAndNotZero()\n");
	if ((tablelimit_unreliable_input.sblind > tablelimit_unreliable_input.bblind)
		&& (tablelimit_unreliable_input.bblind >= 0.01))
	{
		write_log(3, "CTableLimits::SwapBlindsIfSbGreaterThanBBAndNotZero swap neccessary\n");
		SwapDoubles(&tablelimit_unreliable_input.sblind, &tablelimit_unreliable_input.bblind);
	}
}

void CTableLimits::SearchTableForSbAndBbValue()
{
	bool	found_inferred_sb = false, found_inferred_bb = false;

	write_log(3, "CTableLimits::SearchTableForSbAndBbValue()\n");
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
		// If we find cards without a necessary blind
		// then we will try it as unknown (zero).
		if (p_scraper->player_bet(next_chair) >= 0.01)
		{
			write_log(3, "CTableLimits::SearchTableForSbAndBbValue found bet for chair %d\n", next_chair);
			// Bet found
			if (!found_inferred_sb)
			{
				// Not yet SB known
				if (tablelimit_unreliable_input.sblind==0)
				{
					SetSmallBlind(p_scraper->player_bet(next_chair));
					write_log(3, "CTableLimits: found inferred SB at chair %d: %f\n", next_chair, p_scraper->player_bet(next_chair));
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
						SetBigBlind(p_scraper->player_bet(i%p_tablemap->nchairs()));
						write_log(3, "CTableLimits: found inferred BB at chair %d: %f\n", next_chair, p_scraper->player_bet(i%p_tablemap->nchairs()));
					}
					// heads up - reversed blinds
					else
					{
						SetBigBlind(tablelimit_unreliable_input.sblind);
						SetSmallBlind(p_scraper->player_bet(next_chair));
						write_log(3, "CTableLimits: found inferred BB headsup at chair %d: %f\n", next_chair, p_scraper->player_bet(i%p_tablemap->nchairs()));
						write_log(3, "CTableLimits: Swapping blinds.\n");
					}
					found_inferred_bb = true;
				}
				break;
			}
		}
		else if ((p_scraper->card_player(next_chair, 0) != CARD_NOCARD)  
			&& (p_scraper->card_player(next_chair, 0) != CARD_NOCARD))
		{
			write_log(3, "CTableLimits::SearchTableForSbAndBbValue found no bet for chair %d\n", next_chair);
			write_log(3, "CTableLimits::SearchTableForSbAndBbValue but found cards for chair %d\n", next_chair);
			// Awful. Found cards, but not the expected blind.
			// Either a misread, or a blind is missing.
			// Read it as zero and try to guess it later correctly.
			if (!found_inferred_sb)
			{
				found_inferred_sb = true;
				write_log(3, "CTableLimits: expected SB, but not found\n");
				// Don't set any value.
				// Who knows, we might even have correct data 
				// from a previous round!? 
			}
			else if (!found_inferred_bb)
			{
				found_inferred_bb = true;
				write_log(3, "CTableLimits: expected BB, but not found\n");
				// Don't set any value.
				// Who knows, we might even have correct data 
				// from a previous round!? 
				break;
			}
		}
		else
		{
			write_log(3, "CTableLimits::SearchTableForSbAndBbValue found neither bet nor cards for chair %d\n", next_chair);
		}
	}
	SwapBlindsIfSbGreaterThanBBAndNotZero();
}

double CTableLimits::GuessSmallBlindFromBigBlind()
{
	write_log(3, "CTableLimits::GuessSmallBlindFromBigBlind()\n");
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

double CTableLimits::GuessBigBlindFromSmallBlind()
{	// Special cases: NL 0.10/0.25.
	// Some casinos do also provide NL 0.10/0.20,
	// but that is rare, we do assume 0.10/0.25 here.
	write_log(3, "CTableLimits::GuessBigBlindFromSmallBlind()\n");
	if (gametype() == k_gametype_NL && IsEqual(tablelimit_unreliable_input.sblind, 0.10))
	{
		return 0.25;
	}
	return (tablelimit_unreliable_input.sblind * 2);
}

double CTableLimits::GuessBigBetFromBigBlind()
{
	write_log(3, "CTableLimits::GuessBigBetFromBigBlind()\n");
	if ((gametype() == k_gametype_NL) || (gametype() == k_gametype_PL))
	{
		write_log(3, "CTableLimits::GuessBigBetFromBigBlind() BB = bb\n");
		return (tablelimit_unreliable_input.bblind);
	}
	else
	{
		write_log(3, "CTableLimits::GuessBigBetFromBigBlind() BB = 2*bb\n");
		return (tablelimit_unreliable_input.bblind*2);
	}
}

void CTableLimits::AdjustForReasonableness()
{
	write_log(3, "CTableLimits::CheckForReasonableness()\n");
	// SB unknown?
	if (IsSmallerOrEqual(tablelimit_unreliable_input.sblind, 0))
	{
		SetSmallBlind(GuessSmallBlindFromBigBlind());
			write_log(3, "CTableLimits: adjusting SB\n");
	}
	// BB unknown?
	if (IsSmallerOrEqual(tablelimit_unreliable_input.bblind, 0))
	{
		SetBigBlind(GuessBigBlindFromSmallBlind());
			write_log(3, "CTableLimits: adjusting BB\n");
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
		write_log(3, "CTableLimits: adjusting SB\n");
	}
	// Always force big-bet = 2 * big-blind
	SetBigBet(GuessBigBetFromBigBlind());
	write_log(3, "CTableLimits: adjusting big bet\n");
}

void CTableLimits::CalcBetsizesForEveryStreet()
{
	write_log(3, "CTableLimits::CalcBetsizesForEveryStreet()\n");
	// Use the best  known values here, not tablelimit_unreliable_input
	SetBet(k_betround_preflop, bblind());															
	SetBet(k_betround_flop,    bblind());															
	SetBet(k_betround_turn,    bigbet()>=0.01 ? bigbet() : (isnl() || ispl() ? bblind() : bblind()*2));	
	SetBet(k_betround_river,   bigbet()>=0.01 ? bigbet() : (isnl() || ispl() ? bblind() : bblind()*2));	
}

void CTableLimits::CalcTableLimits()
{
	// This is basically the old function CSymbols::CalcStakes()
	// with some extension at the end to auto-lock the blinds,
	// if the values are reasonable.
	write_log(3, "CTableLimits::CalcTableLimits()\n");
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
	_istournament = p_scraper->s_limit_info()->istournament;	

	write_log(3, "CTableLimits: input from scraper: small blind: %f\n", tablelimit_unreliable_input.sblind);
	write_log(3, "CTableLimits: input from scraper: big blind:   %f\n", tablelimit_unreliable_input.bblind);
	write_log(3, "CTableLimits: input from scraper: big bet:     %f\n", tablelimit_unreliable_input.bbet);
	write_log(3, "CTableLimits: input from scraper: gametype:    %d\n", _gametype);             
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

	write_log(3, "CTableLimits: calculated result: small blind: %f\n", tablelimit_unreliable_input.sblind);
	write_log(3, "CTableLimits: calculated result: big blind:   %f\n", tablelimit_unreliable_input.bblind);
	write_log(3, "CTableLimits: calculated result: big bet:     %f\n", tablelimit_unreliable_input.bbet);
	AdjustForReasonableness();
	write_log(3, "CTableLimits: adjusted result: small blind: %f\n", tablelimit_unreliable_input.sblind);
	write_log(3, "CTableLimits: adjusted result: big blind:   %f\n", tablelimit_unreliable_input.bblind);
	write_log(3, "CTableLimits: adjusted result: big bet:     %f\n", tablelimit_unreliable_input.bbet);

	AcceptNewValuesIfGood();
	AutoLockBlinds();
	// Calc miminum betsizes for every streeet (after! we have potentially locked the blinds)
	CalcBetsizesForEveryStreet();
}

void CTableLimits::AcceptNewValuesIfGood()
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

double CTableLimits::sblind()
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

double CTableLimits::bblind()
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

double CTableLimits::bigbet()
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

double CTableLimits::ante()
{
	return _ante; 
}

double CTableLimits::bet(int betround)
{
	assert(betround >= k_betround_preflop);
	assert(betround <= k_betround_river);
	return (_betsizes_for_all_bettingrounds[betround]);
}

double CTableLimits::bet()
{
	return (bet(p_betround_calculator->betround()));
}