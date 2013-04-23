#include "stdafx.h"
#include "CSymbolEngineBlinds.h"

#include <assert.h>
#include "CScraper.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineDealerchair.h"
#include "CSymbolEnginePositions.h"
#include "CSymbolEngineUserchair.h"
#include "CTableLimits.h"
#include "MagicNumbers.h"

CSymbolEngineBlinds *p_symbol_engine_blinds = NULL;

CSymbolEngineBlinds::CSymbolEngineBlinds()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	assert(p_symbol_engine_dealerchair != NULL);
	assert(p_symbol_engine_positions != NULL);
	assert(p_symbol_engine_userchair != NULL);
}

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
	_userchair   = p_symbol_engine_userchair->userchair();
	_dealerchair = p_symbol_engine_dealerchair->dealerchair();
	_playersblindbits = 0;
	_bblindbits = 0;
}

void CSymbolEngineBlinds::ResetOnNewRound()
{}

bool CSymbolEngineBlinds::BlindsAreUnknown()
{
	return (_playersblindbits == 0
		||_bblindbits == 0);

}

void CSymbolEngineBlinds::ResetOnMyTurn()
{
	_nplayersdealt      = p_symbol_engine_active_dealt_playing->nplayersdealt();
	_playersplayingbits = p_symbol_engine_active_dealt_playing->playersplayingbits();
	_dealerchair        = p_symbol_engine_dealerchair->dealerchair();

	// Only updating when it is my turn (stable frames)
	// and blinds are unknown
	if (BlindsAreUnknown())
	{
		CalculateBlinds();
	}
}

void CSymbolEngineBlinds::ResetOnHeartbeat()
{}

void CSymbolEngineBlinds::CalculateBlinds()
{
	int sbchair = k_undefined;
	int bbchair = k_undefined;

	// Heads-Up
	if (_nplayersdealt == 2 && (_playersplayingbits & (1<<_dealerchair)))
	{	
		int sbchair = _dealerchair;
		_bblindbits = _playersplayingbits ^ k_exponents[sbchair];
		_playersblindbits = _playersplayingbits;
	}

	else
	{
		// Is Hero SB or BB ?
		double my_bet = p_scraper->player_bet(_userchair);

		if (my_bet <= p_tablelimits->sblind() && my_bet > 0)
		{
			sbchair = _userchair;
			_playersblindbits = k_exponents[_userchair];
		}

		if (my_bet <= p_tablelimits->bblind() && my_bet > p_tablelimits->sblind())
		{
			bbchair = _userchair;
			_bblindbits = k_exponents[bbchair];
			_playersblindbits = k_exponents[_userchair];
		}

		for (int i=_dealerchair+1; i<_dealerchair+p_tablemap->nchairs(); i++)
		{
			double p_bet = p_scraper->player_bet(i%p_tablemap->nchairs());

			// search SB
			if (sbchair == k_undefined && p_bet <= p_tablelimits->sblind() && p_bet > 0) 
			{
				sbchair = i%p_tablemap->nchairs();
				_playersblindbits |= k_exponents[sbchair];		
			}
			// search BB
			if (bbchair == k_undefined && p_bet <= p_tablelimits->bblind() && p_bet > p_tablelimits->sblind() && i%p_tablemap->nchairs() != sbchair)
			{
				bbchair = i%p_tablemap->nchairs();	
				_bblindbits = k_exponents[bbchair];
				_playersblindbits |= k_exponents[bbchair];		
				
			}
		}
		
		// SB not found correction.
		// Will only apply if we are the bb + missed action(s). most common case. 
		// Restrictions : 3 or less players were dealt or last bb is active
		if (sbchair == k_undefined && (_nplayersdealt < 3 || (bbchair == _userchair && p_symbol_engine_positions->nchairsdealtright() == 1)))
		{
			for (int i=_dealerchair+1; i<_dealerchair+p_tablemap->nchairs(); i++)
			{
				double p_bet = p_scraper->player_bet(i%p_tablemap->nchairs());

				// 1st caller/raiser after dealer is sb
				if (p_bet >= p_tablelimits->bblind() && sbchair == k_undefined && i%p_tablemap->nchairs() != bbchair)
				{
					sbchair = i%p_tablemap->nchairs();
					_playersblindbits |= k_exponents[sbchair];
				}
			}
		}
	}							
}