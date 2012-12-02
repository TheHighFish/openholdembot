#include "stdafx.h"
#include "CSymbolEngineChipAmounts.h"

#include "CScraper.h"

CSymbolEngineChipAmounts::CSymbolEngineChipAmounts()
{}

CSymbolEngineChipAmounts::~CSymbolEngineChipAmounts()
{}

void CSymbolEngineChipAmounts::InitOnStartup()
{
	ResetOnConnection();
}

void CSymbolEngineChipAmounts::ResetOnConnection()
{
	ResetOnHandreset();

	_maxbalance = k_undefined;
	_balanceatstartofsession = k_undefined;
	nchairs = p_tablemap->nchairs();
}

void CSymbolEngineChipAmounts::ResetOnHandreset()
{
	for (int i=0; i<k_max_number_of_players; i++)
	{
		_balance[i]    = 0;
		_stack[i]      = 0;
		_currentbet[i] = 0;
		_stacks_at_hand_start[i] = 0;
		_stacks_at_hand_start[i] = 0;
		_stacks_at_hand_start[i] = p_scraper->player_balance(i) + p_scraper->player_bet(i);
	}
	_pot = 0;
	_potplayer = 0;
	_potcommon = 0;
}

void CSymbolEngineChipAmounts::ResetOnNewRound()
{}

void CSymbolEngineChipAmounts::ResetOnMyTurn()
{}

void CSymbolEngineChipAmounts::ResetOnHeartbeat()
{
	CalculateBalances();
	CalculateStacks();
	CalculateCurrentbets();
	CalculatePots();
}

void CSymbolEngineChipAmounts::SetBalance(const int player, const double d) 
{
	assert(player >= 0);
	assert(player < k_max_number_of_players);
	_balance[player] = d;
	if (player == userchair) 
	{
		SetMaxBalanceConditionally(d);
		SetBalanceAtStartOfSessionConditionally(d);
	}
}

void CSymbolEngineChipAmounts::SetMaxBalanceConditionally(const double d) 
{ 
	if (d > _maxbalance) 
	{
		_maxbalance = d;
	}
}

void CSymbolEngineChipAmounts::SetBalanceAtStartOfSessionConditionally(const double d) 
{ 
	if (_balanceatstartofsession <= 0) 
	{
		_balanceatstartofsession = d;
	}
}

void CSymbolEngineChipAmounts::CalculateBalances()
{
	for (int i=0; i<k_max_number_of_players; i++)
	{
		if (i < nchairs)
		{
			SetBalance(i, p_scraper->player_balance(i));
		}
		else
		{
			SetBalance(i, 0);
		}
	}
}

//!!!
bool PlayerHasCards(int player)
{
	assert(player >= 0);
	assert(player < k_max_number_of_players);

	return ((p_scraper->card_player(player, 0) != CARD_NOCARD) 
		&&  (p_scraper->card_player(player, 1) != CARD_NOCARD));
}

void CSymbolEngineChipAmounts::CalculateStacks()
{
	double	temp = 0;
	// simple bubble sort for 10 stack values
	for (int i=0; i<nchairs; i++)
	{
		if (PlayerHasCards(i))
		{
			_stack[i] = _balance[i];
		}
		else
		{
			_stack[i] = 0;
		}
	}
	for (int i=0; i<nchairs-1; i++)
	{
		for (int j=i+1; j<nchairs; j++)
		{
			if (_stack[i] < _stack[j])
			{
				// !!! swap
				temp      = _stack[i];
				_stack[i] = _stack[j];
				_stack[j] = temp;
			}
		}
	}
	for (int i=0; i<nchairs; i++)
	{
		assert(_stack[i] >= 0);
		_stack[i] = _stack[i];									
	}
	for (int i=nchairs; i<k_max_number_of_players; i++)
	{
		_stack[i] = 0;
	}
}

void CSymbolEngineChipAmounts::CalculateCurrentbets()
{
	for (int i=0; i<nchairs; i++)
	{
		_currentbet[i] = p_scraper->player_bet(i); 
	}
}

void CSymbolEngineChipAmounts::CalculatePots()
{
	_pot = 0;
	_potplayer = 0;
	_potcommon = 0;
	for (int i=0; i<nchairs; i++)
	{
		_potplayer += _currentbet[i];							
	}

	// pot, potcommon, based on value of potmethod
	if (p_tablemap->potmethod() == 2)															
	{
		_pot = p_scraper->pot(0);
		_potcommon = _pot - _potplayer;
	}
	else if(p_tablemap->potmethod() == 3) 
	{
		_pot = p_scraper->pot(0);
		for (int i=1; i<k_max_number_of_pots; i++)
		{
			_pot = max(_pot, p_scraper->pot(i));
		}
		_potcommon = _pot - _potplayer;
	}
	
	else  // potmethod() == 1
	{
		_potcommon = 0;
		for (int i=0; i<k_max_number_of_pots; i++)
		{
			_potcommon += p_scraper->pot(i);
		}
		_pot = _potcommon + _potplayer;
	}
}


// !!! formerly CalcChipamtsLimits
void CSymbolEngineChipAmounts::CalculateAmountsToCallToRaise() 
{
	int	next_largest_bet = 0;

	_call = (_user_chair_confirmed && _sym.raischair != -1) ?
			   _sym.currentbet[(int) _sym.raischair] - _sym.currentbet[(int) _sym.userchair] : 0;

	next_largest_bet = 0;
	for (int i=0; i<p_tablemap->nchairs(); i++)
	{
		if (_sym.currentbet[i] != _sym.currentbet[(int) _sym.raischair] && _sym.currentbet[i]>next_largest_bet)
		{
			next_largest_bet = _sym.currentbet[i];
		}
	}
	_sraiprev = _currentbet[_raischair] - next_largest_bet;			

	if (_user_chair_confirmed)
	{
		_sraimin = _currentbet[userchair] + _call;
		_sraimax = _balance[userchair] - _call;
		if (_sraimax < 0)
		{
			_sraimax = 0;
		}
	}
	
}


//!!! formerly void CSymbols::CalcNumbets(void)
void CSymbolEngineChipAmounts::CalculateBetsToCallToRaise() 
{
	double bet = p_tablelimits->bet();
	if (bet <= 0)
	{
		return;
	}
	if (_user_chair_confirmed)
	{
		_nbetstocall = _call / bet;
		_nbetstorais = _nbetstocall + 1;	
	}
	_ncallbets = _currentbet[raischair] / bet;					
	_nraisbets = _ncallbets + 1;												
}


/*!!!

		// We do no longer check for cardbacks,
		// but for cardbacks or cards.
		// This way we can play all cards face-up at PokerAcademy.
		// http://www.maxinmontreal.com/forums/viewtopic.php?f=111&t=13384
		if (p_scraper->card_player(i, 0) != CARD_NOCARD 
			&& p_scraper->card_player(i, 1) != CARD_NOCARD 
			&& i != _sym.userchair)
			*/