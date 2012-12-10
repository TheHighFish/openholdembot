#include "stdafx.h"
#include "CSymbolEngineChipAmounts.h"

#include "CScraper.h"
#include "CSymbolEngineUserchair.h"

CSymbolEngineChipAmounts *p_symbol_engine_chip_amounts = NULL;

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
	_nchairs = p_tablemap->nchairs();
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
	if (player == _userchair) 
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
		if (i < _nchairs)
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
	for (int i=0; i<_nchairs; i++)
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
	for (int i=0; i<_nchairs-1; i++)
	{
		for (int j=i+1; j<_nchairs; j++)
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
	for (int i=0; i<_nchairs; i++)
	{
		assert(_stack[i] >= 0);
		_stack[i] = _stack[i];									
	}
	for (int i=_nchairs; i<k_max_number_of_players; i++)
	{
		_stack[i] = 0;
	}
}

void CSymbolEngineChipAmounts::CalculateCurrentbets()
{
	for (int i=0; i<_nchairs; i++)
	{
		_currentbet[i] = p_scraper->player_bet(i); 
	}
}

void CSymbolEngineChipAmounts::CalculatePots()
{
	_pot = 0;
	_potplayer = 0;
	_potcommon = 0;
	for (int i=0; i<_nchairs; i++)
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

void CSymbolEngineChipAmounts::CalculateAmountsToCallToRaise() 
{
	int	next_largest_bet = 0;
	double largest_bet = Largestbet();

	if (p_symbol_engine_userchair->userchair_confirmed())
		
	{
		_call = largest_bet - _currentbet[_userchair];
	}
	else
	{
		_call = 0;
	}

	next_largest_bet = 0;
	for (int i=0; i<_nchairs; i++)
	{
		if (_currentbet[i] != largest_bet 
			&& _currentbet[i] > next_largest_bet)
		{
			next_largest_bet = _currentbet[i];
		}
	}
	_sraiprev = largest_bet - next_largest_bet;			

	if (p_symbol_engine_userchair->userchair_confirmed())
	{
		_sraimin = largest_bet + _call;
		_sraimax = _balance[_userchair] - _call;
		if (_sraimax < 0)
		{
			_sraimax = 0;
		}
	}
	
}

void CSymbolEngineChipAmounts::CalculateBetsToCallToRaise() 
{
	double bet = p_tablelimits->bet();
	if (bet <= 0)
	{
		return;
	}
	if (p_symbol_engine_userchair->userchair_confirmed())
	{
		_nbetstocall = _call / bet;
		_nbetstorais = _nbetstocall + 1;	
	}
	_ncallbets = Largestbet() / bet;				
	_nraisbets = _ncallbets + 1;												
}

double CSymbolEngineChipAmounts::Largestbet()
{
	double largest_bet = 0.0;
	for (int i=0; i<_nchairs; i++)
	{
		if (_currentbet[i] > largest_bet) 
		{
			largest_bet = _currentbet[i];
		}
	}
	return largest_bet;
}

/*!!!

		// We do no longer check for cardbacks,
		// but for cardbacks or cards.
		// This way we can play all cards face-up at PokerAcademy.
		// http://www.maxinmontreal.com/forums/viewtopic.php?f=111&t=13384
		if (p_scraper->card_player(i, 0) != CARD_NOCARD 
			&& p_scraper->card_player(i, 1) != CARD_NOCARD 
			&& i != p_symbol_engine_userchair->userchair())
			*/