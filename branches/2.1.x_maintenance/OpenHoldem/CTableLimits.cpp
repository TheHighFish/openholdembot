#include "stdafx.h"
#include "CTableLimits.h"

#include "CSymbols.h"


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
	if (!blinds_locked)
	{
		stable_sblind = 0; 
		stable_bblind = 0;
		stable_bigbet = 0;
	}
	number_of_stable_tablelimits;
	for (int i=0; i<k_number_of_hands_to_autolock_blinds; i++)
	{
		stable_sblind_first_n_hands[i] = 0;
		stable_bblind_first_n_hands[i] = 0;
		stable_bigbet_first_n_hands[i] = 0;
	}
	ResetOnHandreset();
}

void CTableLimits::ResetOnHandreset()
{
	sblind_current_scrape = 0; 
	bblind_current_scrape = 0;
	bigbet_current_scrape = 0;
}

void CTableLimits::LockBlinds(double small_blind, double big_blind, double big_bet)
{
	stable_sblind = small_blind; 
	stable_bblind = big_blind;
	stable_bigbet = big_bet;
	blinds_locked = true;
}

void CTableLimits::LockBlindsManually(double small_blind, double big_blind, double big_bet)
{
	LockBlinds(small_blind, big_blind, big_bet);
}

void CTableLimits::AutoLockBlindsForCashgamesAfterNHands()
{
	// !!! ...
	LockBlinds(0, 0, 0); //!!!
}

void CTableLimits::SetSmallBlind(double small_blind)
{
	sblind_current_scrape = small_blind; 
}

void CTableLimits::SetBigBlind(double big_blind)
{
	bblind_current_scrape = big_blind;
}

void CTableLimits::SetBigBet(double big_bet)
{
	bigbet_current_scrape = big_bet;
}

bool CTableLimits::FirstActionThisHandAndEnoughStableFrames()
{
	// Enough stable frames?
	// !!! Make sure, isfinalanswer gets calculated before the limits
	if (p_symbols->sym()->isfinalanswer)
	{
		return (0); //!!!
	}
	return false;
}

void CTableLimits::ComputeStableTableLimits()
{
	//!!!
	if (FirstActionThisHandAndEnoughStableFrames())
	{
		stable_sblind_first_n_hands[0] = stable_sblind;
		stable_bblind_first_n_hands[0] = stable_bblind;
		stable_bigbet_first_n_hands[0] = stable_bigbet;
	}
}

/*
int		i = 0;
	bool	found_inferred_sb = false, found_inferred_bb = false;

	// !!! tablelimits !!!
	set_sym_sblind(0);
	set_sym_bblind(0);
	set_bigbet(0);
	set_sym_ante(0);

		// Save the parts we scraped successfully
		if (p_scraper->s_limit_info()->found_sblind)
		set_sym_sblind(p_scraper->s_limit_info()->sblind);									// sblind

		if (p_scraper->s_limit_info()->found_bblind)
		set_sym_bblind(p_scraper->s_limit_info()->bblind);									// bblind

		if (p_scraper->s_limit_info()->found_ante)
		set_sym_ante(p_scraper->s_limit_info()->ante);										// ante

		if (p_scraper->s_limit_info()->found_limit)
		set_sym_lim(p_scraper->s_limit_info()->limit);										// lim

		if (p_scraper->s_limit_info()->found_bbet)
		set_bigbet(p_scraper->s_limit_info()->bbet);

		// Figure out bb/sb based on game type
		if (_sym.lim == LIMIT_NL || _sym.lim == LIMIT_PL)
		{
			if (_sym.sblind==0)
			{
				if (p_scraper->s_limit_info()->found_sb_bb)
				set_sym_sblind(p_scraper->s_limit_info()->sb_bb);
			}

			if (_sym.bblind==0)
			{
				if (p_scraper->s_limit_info()->found_bb_BB)
				set_sym_bblind(p_scraper->s_limit_info()->bb_BB);
			}

			if (_bigbet==0)
			{
				if (p_scraper->s_limit_info()->found_bb_BB)
				set_bigbet(p_scraper->s_limit_info()->bb_BB);

				else if (p_scraper->s_limit_info()->found_sb_bb)
				set_bigbet(p_scraper->s_limit_info()->sb_bb*2);

				else if (_sym.bblind!=0)
				set_bigbet(_sym.bblind);

				else if (_sym.sblind!=0)
				set_bigbet(_sym.sblind*2);

			}
		}

		else if (_sym.lim == LIMIT_FL || _sym.lim == -1)
		{
			if (_sym.sblind==0)
			{
				if (p_scraper->s_limit_info()->found_sb_bb)
				set_sym_sblind(p_scraper->s_limit_info()->sb_bb);
			}

			if (_sym.bblind==0)
			{
				if (p_scraper->s_limit_info()->found_bb_BB)
				set_sym_bblind(p_scraper->s_limit_info()->bb_BB);
			}

			if (_bigbet==0)
			{
				if (p_scraper->s_limit_info()->found_bb_BB)
				set_bigbet(p_scraper->s_limit_info()->bb_BB*2);

				else if (p_scraper->s_limit_info()->found_sb_bb)
				set_bigbet(p_scraper->s_limit_info()->sb_bb*4);

				else if (_sym.bblind!=0)
				set_bigbet(_sym.bblind*2);

				else if (_sym.sblind!=0)
				set_bigbet(_sym.sblind*4);
			}
		}

	// If we have NOT locked the blinds then do this stuff
	if (!p_scraper->s_lock_blinds()->blinds_are_locked) 
	{
			// if we still do not have blinds, then infer them from the posted bets
			if (_sym.br == 1 && (_sym.sblind==0 || _sym.bblind==0))
			{
				for (i=_sym.dealerchair+1; i<=_sym.dealerchair+p_tablemap->nchairs(); i++)
				{
					if (p_scraper->card_player(i%p_tablemap->nchairs(), 0) != CARD_NOCARD && 
						p_scraper->card_player(i%p_tablemap->nchairs(), 0) != CARD_NOCARD)
					{
						if (p_scraper->player_bet(i%p_tablemap->nchairs()) != 0 && !found_inferred_sb)
						{
							if (_sym.sblind==0)
							{
							set_sym_sblind(p_scraper->player_bet(i%p_tablemap->nchairs()));
								found_inferred_sb = true;
							}
						}

						else if (p_scraper->player_bet(i%p_tablemap->nchairs()) != 0 && found_inferred_sb && !found_inferred_bb)
						{
							if (_sym.bblind==0)
							{
								// !heads up - normal blinds
								if (i%p_tablemap->nchairs() != _sym.dealerchair)
								{
								set_sym_bblind(p_scraper->player_bet(i%p_tablemap->nchairs()));
								}
								// heads up - reversed blinds
								else
								{
								set_sym_bblind(_sym.sblind);
								set_sym_sblind(p_scraper->player_bet(i%p_tablemap->nchairs()));
								}
								found_inferred_bb = true;
							}
						}
					}
				}

				// check for reasonableness
				if (_sym.bblind > _sym.sblind*3+0.001)
				set_sym_bblind(_sym.sblind*2);

				if (_sym.sblind > _sym.bblind+0.001)
				set_sym_sblind(_sym.bblind/2);

				if (_bigbet > _sym.bblind*2+0.001 || _bigbet < _sym.bblind*2-0.001)
				set_bigbet(_sym.bblind*2);

			}
		}
	*/