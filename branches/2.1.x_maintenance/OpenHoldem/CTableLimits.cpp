#include "stdafx.h"
#include "CTableLimits.h"

#include <assert.h>
#include "CPokerPro.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CSymbols.h"
#include "debug.h"
#include "FloatingPoint_Comparisions.h"
#include "MagicNumbers.h"
#include "Median.h"


CTableLimits	*p_tablelimits = NULL;


CTableLimits::CTableLimits()
{
	ResetOnConnection();
	UnLockBlindsManually();
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
	ResetOnHandreset();
	_ante = 0;
	_gametype = -1;
}

void CTableLimits::ResetOnHandreset()
{
	write_log(3, "CTableLimits::ResetOnHandreset()\n");
	blinds_locked_for_current_hand = false;
	tablelimit_locked_for_current_hand.sblind = 0;
	tablelimit_locked_for_current_hand.bblind = 0;
	tablelimit_locked_for_current_hand.bbet   = 0;
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

void CTableLimits::UnLockBlindsManually() // TODO!!!
{
	write_log(3, "CTableLimits::UnLockBlindsManually()\n");
	blinds_locked_manually = false;
	tablelimit_locked_manually.sblind   = 0;
	tablelimit_locked_manually.bblind   = 0;
	tablelimit_locked_manually.bbet     = 0;
	/*tablelimit_locked_manually.ante     = 0;
	tablelimit_locked_manually.gametype = -1;*/
}

void CTableLimits::LockBlindsManually(double small_blind, double big_blind, double big_bet, double ante, int gametype)
{
	assert(small_blind >= 0);
	assert(big_blind >= 0);
	assert(big_bet >= 0);
	assert(ante >= 0);
	assert(gametype >= k_gametype_unknown);
	assert(gametype <= k_gametype_FL);
	write_log(3, "CTableLimits::LockBlindsManually()\n");
	blinds_locked_manually = true;
	tablelimit_locked_manually.sblind = small_blind; 
	tablelimit_locked_manually.bblind = big_blind;
	tablelimit_locked_manually.bbet   = big_bet;
	// gametype!!! ante
	// Save locked blinds info for future use 
	prefs.set_sblind(small_blind);
	prefs.set_bblind(big_blind);
	prefs.set_bbet(big_bet);
	prefs.set_ante(ante);
	prefs.set_gametype(gametype);
}

void CTableLimits::AutoLockBlindsForCashgamesAfterNHands()
{
	write_log(3, "CTableLimits::AutoLockBlindsForCashgamesAfterNHands()\n");
	write_log(3, "CTableLimits: blinds_locked_for_complete_session: %d\n", blinds_locked_for_complete_session);
	write_log(3, "CTableLimits: istournament: %d\n", IsTrue(p_symbols->sym()->istournament));
	if (blinds_locked_for_complete_session || IsTrue(p_symbols->sym()->istournament))
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
	// so we check for non-zero blinds.
	// Maybe we should also check for stable frames (isfinalanswer),
	// but that requires waiting until it is our turn.
	if (/*p_symbols->sym()->isfinalanswer!!! &&*/ (sblind() >= 0.01) 
		&& (bblind() >= sblind()) && (bbet() >= bblind()))
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
	tablelimit_locked_for_current_hand.sblind = tablelimit_unreliable_input.sblind;
	tablelimit_locked_for_current_hand.bblind = tablelimit_unreliable_input.bblind;
	tablelimit_locked_for_current_hand.bbet	  = tablelimit_unreliable_input.bbet;
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
	assert(gametype >= k_gametype_unknown);
	assert(gametype <= k_gametype_FL);
	_gametype = gametype;
}

void CTableLimits::SetBet(int betround, double bet)
{
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
	if (sblind()==0)
	{
		if (p_scraper->s_limit_info()->found_sb_bb)
			SetSmallBlind(p_scraper->s_limit_info()->sb_bb);
	}
	if (sblind()==0)
	{
		if (p_scraper->s_limit_info()->found_bb_BB)
			SetBigBlind(p_scraper->s_limit_info()->bb_BB);
	}
	if (bblind()==0)
	{
		if (p_scraper->s_limit_info()->found_bb_BB)
			SetBigBet(p_scraper->s_limit_info()->bb_BB);
		else if (p_scraper->s_limit_info()->found_sb_bb)
			SetBigBet(p_scraper->s_limit_info()->sb_bb*2);
		else if (bblind()!=0)
			SetBigBet(bblind());
		else if (sblind()!=0)
			SetBigBet(sblind()*2);
	}
}

bool CTableLimits::IsCalculationNeccessary()
{
	write_log(3, "CTableLimits::IsCalculationNeccessary()\n");
	if (sblind()==0)
	{
		write_log(3, "CTableLimits: sblind=0, calculation neccessary.\n");
		return true;
	}
	if (bblind()==0)
	{
		write_log(3, "CTableLimits: bblind=0, calculation neccessary.\n");
		return true;
	}
	if (p_pokerpro->IsConnected() && p_pokerpro->ppdata()->m_tinf.m_tid != 0)
	{
		write_log(3, "CTableLimits: PPro requires calculation.\n");
		return true;
	}
	write_log(3, "CTableLimits: no calculatoin neccessary.\n");
	return false;
}

void CTableLimits::CalcTableLimits_FL_AndUnknownGametype()
{
	write_log(3, "CTableLimits::CalcTableLimits_FL_AndUnknownGametype()\n");
	if (sblind()==0)
	{
		if (p_scraper->s_limit_info()->found_sb_bb)
			SetSmallBlind(p_scraper->s_limit_info()->sb_bb);
	}
	if (bblind()==0)
	{
		if (p_scraper->s_limit_info()->found_bb_BB)
			SetBigBlind(p_scraper->s_limit_info()->bb_BB);
	}
	if (bbet()==0)
	{
		if (p_scraper->s_limit_info()->found_bb_BB)
			SetBigBet(p_scraper->s_limit_info()->bb_BB*2);
		else if (p_scraper->s_limit_info()->found_sb_bb)
			SetBigBet(p_scraper->s_limit_info()->sb_bb*4);
		else if (bblind()!=0)
			SetBigBet(bblind()*2);
		else if (sblind()!=0)
			SetBigBet(sblind()*4);
	}
}

void CTableLimits::CalcTableLimitsFromPostedBets()
{
	bool	found_inferred_sb = false, found_inferred_bb = false;

	write_log(3, "CTableLimits::CalcTableLimitsFromPostedBets()\n");
	for (int i=p_symbols->sym()->dealerchair+1; i<=p_symbols->sym()->dealerchair+p_tablemap->nchairs(); i++)
	{
		// We do no longer require cards for the players,
		// as this mixed things up, when e.g. SB folds.
		if (p_scraper->player_bet(i%p_tablemap->nchairs()) >= 0.01 && !found_inferred_sb)
		{
			if (sblind()==0)
			{
				SetSmallBlind(p_scraper->player_bet(i%p_tablemap->nchairs()));
				write_log(3, "CTableLimits: found inferred SB: %f\n", p_scraper->player_bet(i%p_tablemap->nchairs()));
				found_inferred_sb = true;
			}
		}
		else if (p_scraper->player_bet(i%p_tablemap->nchairs()) >= 0.02 && found_inferred_sb && !found_inferred_bb)
		{
			if (bblind()==0)
			{
				// Not heads up - normal blinds
				if (i%p_tablemap->nchairs() != p_symbols->sym()->dealerchair)
				{
					SetBigBlind(p_scraper->player_bet(i%p_tablemap->nchairs()));
					write_log(3, "CTableLimits: found inferred BB: %f\n", p_scraper->player_bet(i%p_tablemap->nchairs()));
				}
				// heads up - reversed blinds
				else
				{
					SetBigBlind(sblind());
					SetSmallBlind(p_scraper->player_bet(i%p_tablemap->nchairs()));
					write_log(3, "CTableLimits: found inferred BB hu: %f\n", p_scraper->player_bet(i%p_tablemap->nchairs()));
					write_log(3, "CTableLimits: Swapping blinds.\n");
				}
				found_inferred_bb = true;
				break;
			}
		}
	}
}

void CTableLimits::AdjustForReasonableness()
{
	write_log(3, "CTableLimits::CheckForReasonableness()\n");
	if (IsGreater(bblind(), (sblind()*3)))
	{
		if (IsGreater(sblind(), 0))
		{
			SetBigBlind(sblind()*2);
			write_log(3, "CTableLimits: adjusting BB\n");
		}
		else
		{
			SetSmallBlind(bblind()/2);
			write_log(3, "CTableLimits: adjusting SB\n");
		}
	}
	if (IsGreaterOrEqual(sblind(), bblind()))
	{
		// Formerly this code was checking for SB > BB,
		// but up to now we never saw an online-game with identical blinds.
		// SB >= BB does usually mean, that SB did already act and complete.
		SetSmallBlind(bblind()/2);
		write_log(3, "CTableLimits: adjusting SB\n");
	}
	// Always force big-bet = 2 * big-blind
	SetBigBet(bblind()*2);
	write_log(3, "CTableLimits: adjusting big bet\n");
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

	// If we have NOT locked the blinds then do this stuff
	if (blinds_locked_manually) 
	{
		// if we still do not have blinds, then infer them from the posted bets
		if (p_symbols->sym()->br == k_betround_preflop && (sblind()==0 || bblind()==0))
		{
			CalcTableLimitsFromPostedBets();			
		}
	}
	write_log(3, "CTableLimits: calculated result: small blind: %f\n", sblind());
	write_log(3, "CTableLimits: calculated result: big blind:   %f\n", bblind());
	write_log(3, "CTableLimits: calculated result: big bet:     %f\n", bbet());
	AdjustForReasonableness();
	write_log(3, "CTableLimits: adjusted result: small blind: %f\n", sblind());
	write_log(3, "CTableLimits: adjusted result: big blind:   %f\n", bblind());
	write_log(3, "CTableLimits: adjusted result: big bet:     %f\n", bbet());
	
	AutoLockBlinds();
	// Calc miminum betsizes for every streeet (after! we have potentially locked the blinds)
	SetBet(k_betround_preflop, p_tablelimits->bblind());															
	SetBet(k_betround_flop,    p_tablelimits->bblind());															
	SetBet(k_betround_turn,    (p_tablelimits->bbet()!=0 ? p_tablelimits->bbet() : (p_tablelimits->isnl() || p_tablelimits->ispl() ? p_tablelimits->bblind() : p_tablelimits->bblind()*2)));	
	SetBet(k_betround_river,   (p_tablelimits->bbet()!=0 ? p_tablelimits->bbet() : (p_tablelimits->isnl() || p_tablelimits->ispl() ? p_tablelimits->bblind() : p_tablelimits->bblind()*2)));	
}

double CTableLimits::sblind()
{
	if (blinds_locked_manually)
	{
		return tablelimit_locked_manually.sblind;
	}
	else if (blinds_locked_for_complete_session)
	{
		return tablelimit_locked_for_complete_session.sblind;
	}
	else if (blinds_locked_for_current_hand)
	{
		return tablelimit_locked_for_current_hand.sblind;
	}
	else
	{
		return tablelimit_unreliable_input.sblind;
	}
}

double CTableLimits::bblind()
{
	if (blinds_locked_manually)
	{
		return tablelimit_locked_manually.bblind;
	}
	else if (blinds_locked_for_complete_session)
	{
		return tablelimit_locked_for_complete_session.bblind;
	}
	else if (blinds_locked_for_current_hand)
	{
		return tablelimit_locked_for_current_hand.bblind;
	}
	else
	{
		return tablelimit_unreliable_input.bblind;
	}
}

double CTableLimits::bbet()
{
	if (blinds_locked_manually)
	{
		return tablelimit_locked_manually.bbet;
	}
	else if (blinds_locked_for_complete_session)
	{
		return tablelimit_locked_for_complete_session.bbet;
	}
	else if (blinds_locked_for_current_hand)
	{
		return tablelimit_locked_for_current_hand.bbet;
	}
	else
	{
		return tablelimit_unreliable_input.bbet;
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
	return (bet(p_symbols->sym()->betround));
}