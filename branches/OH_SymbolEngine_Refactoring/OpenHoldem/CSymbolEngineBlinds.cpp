#include "stdafx.h"
#include "CSymbolEngineBlinds.h"

#include "CScraper.h"
#include "MagicNumbers.h"

CSymbolEngineBlinds::CSymbolEngineBlinds()
{}

CSymbolEngineBlinds::~CSymbolEngineBlinds()
{}

void CSymbolEngineBlinds::InitOnStartup()
{
	ResetOnConnection();
}

void CSymbolEngineBlinds::ResetOnConnection()
{
	ResetOnHandreset();
}

void CSymbolEngineBlinds::ResetOnHandreset()
{
	_playersblindbits = 0;
	_opponentsblindbits = 0;
	_bblindbits = 0;
}

void CSymbolEngineBlinds::ResetOnNewRound()
{}

bool CSymbolEngineBlinds::BlindsAreUnknown()
{
	return (_playersblindbits == 0
		|| _opponentsblindbits == 0
		||_bblindbits == 0);

}

void CSymbolEngineBlinds::ResetOnMyTurn()
{
	// Only updating when it is my turn (stable frames)
	// and blinds are unknown
	if (ismyturn && BlindsAreUnknown())
	{
		CalculateBlinds();
	}
}

void CSymbolEngineBlinds::ResetOnHeartbeat()
{}

void CSymbolEngineBlinds::CalculateBlinds()
{
	// Heads-Up
	if (_sym.nplayersdealt == 2 && ((int)_sym.playersplayingbits & (1<<(int)_sym.dealerchair)))
	{	
		sbchair = _sym.dealerchair;
		set_sym_bblindbits((int)_sym.playersplayingbits ^ k_exponents[sbchair]);						// bblindbits
		set_sym_playersblindbits((int)_sym.playersplayingbits);											// playersblindbits
		set_sym_opponentsblindbits((int)_sym.playersplayingbits ^ k_exponents[(int)_sym.userchair]);	// playersblindbits
	}

	else
	{
		// Is Hero SB or BB ?
		my_bet = p_scraper->player_bet(_sym.userchair);

		if (my_bet <= p_tablelimits->sblind() && my_bet > 0)
		{
			sbchair = _sym.userchair;
			set_sym_playersblindbits(k_exponents[(int)_sym.userchair]);
		}

		if (my_bet <= p_tablelimits->bblind() && my_bet > p_tablelimits->sblind())
		{
			bbchair = _sym.userchair;
			set_sym_bblindbits(k_exponents[bbchair]);
			set_sym_playersblindbits(k_exponents[(int)_sym.userchair]);
		}

		for (int i=_sym.dealerchair+1; i<_sym.dealerchair+p_tablemap->nchairs(); i++)
		{
			p_bet = p_scraper->player_bet(i%p_tablemap->nchairs());

			// search SB
			if (sbchair == -1 && p_bet <= p_tablelimits->sblind() && p_bet > 0) 
			{
				sbchair = i%p_tablemap->nchairs();
				set_sym_playersblindbits((int)_sym.playersblindbits | k_exponents[sbchair]);			// playersblindbits
				set_sym_opponentsblindbits((int)_sym.opponentsblindbits | k_exponents[sbchair]);		// opponentsblindbits
			}
			// search BB
			if (bbchair == -1 && p_bet <= p_tablelimits->bblind() && p_bet > p_tablelimits->sblind() && i%p_tablemap->nchairs() != sbchair)
			{
				bbchair = i%p_tablemap->nchairs();	
				set_sym_bblindbits(k_exponents[bbchair]);												// big blind bit
				set_sym_playersblindbits((int)_sym.playersblindbits | k_exponents[bbchair]);			// playersblindbits
				set_sym_opponentsblindbits((int)_sym.opponentsblindbits | k_exponents[bbchair]);		// opponentsblindbits
			}
		}
		

		// SB not found correction.
		// Will only apply if we are the bb + missed action(s). most common case. 
		// Restrictions : 3 or less players were dealt or last bb is active
		if (sbchair == -1 && (_sym.nplayersdealt < 3 || (bbchair == _sym.userchair && _sym.nchairsdealtright == 1)))
		{
			for (int i=_sym.dealerchair+1; i<_sym.dealerchair+p_tablemap->nchairs(); i++)
			{
				p_bet = p_scraper->player_bet(i%p_tablemap->nchairs());

				// 1st caller/raiser after dealer is sb
				if (p_bet >= p_tablelimits->bblind() && sbchair == -1 && i%p_tablemap->nchairs() != bbchair)
				{
					sbchair = i%p_tablemap->nchairs();
					set_sym_playersblindbits((int)_sym.playersblindbits | k_exponents[sbchair]);
					set_sym_opponentsblindbits((int)_sym.opponentsblindbits | k_exponents[sbchair]);	// opponentsblindbits
				}
			}
		}
	}							
}