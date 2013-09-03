#include "stdafx.h"
#include "CSymbolEngineBlinds.h"

#include <assert.h>
#include "CScraper.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineDealerchair.h"
#include "CSymbolEnginePositions.h"
#include "CSymbolEngineUserchair.h"
#include "CSymbolEngineTableLimits.h"
#include "MagicNumbers.h"

CSymbolEngineBlinds *p_symbol_engine_blinds = NULL;

CSymbolEngineBlinds::CSymbolEngineBlinds()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	assert(p_symbol_engine_dealerchair != NULL);
	assert(p_symbol_engine_positions != NULL);
	assert(p_symbol_engine_tablelimits != NULL);
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
	if (p_symbol_engine_active_dealt_playing->playersdealtbits() == 2 && (p_symbol_engine_active_dealt_playing->nplayersdealt() & (1<<DEALER_CHAIR)))
	{	
		int sbchair = DEALER_CHAIR;
		_bblindbits = p_symbol_engine_active_dealt_playing->nplayersdealt() ^ k_exponents[sbchair];
		_playersblindbits = p_symbol_engine_active_dealt_playing->nplayersdealt();
	}

	else
	{
		// Is Hero SB or BB ?
		double my_bet = p_scraper->player_bet(USER_CHAIR);

		if (my_bet <= p_symbol_engine_tablelimits->sblind() && my_bet > 0)
		{
			sbchair = USER_CHAIR;
			_playersblindbits = k_exponents[USER_CHAIR];
		}

		if (my_bet <= p_symbol_engine_tablelimits->bblind() && my_bet > p_symbol_engine_tablelimits->sblind())
		{
			bbchair = USER_CHAIR;
			_bblindbits = k_exponents[bbchair];
			_playersblindbits = k_exponents[USER_CHAIR];
		}

		for (int i=DEALER_CHAIR+1; i<DEALER_CHAIR+p_tablemap->nchairs(); i++)
		{
			double p_bet = p_scraper->player_bet(i%p_tablemap->nchairs());

			// search SB
			if (sbchair == k_undefined && p_bet <= p_symbol_engine_tablelimits->sblind() && p_bet > 0) 
			{
				sbchair = i%p_tablemap->nchairs();
				_playersblindbits |= k_exponents[sbchair];		
			}
			// search BB
			if (bbchair == k_undefined && p_bet <= p_symbol_engine_tablelimits->bblind() && p_bet > p_symbol_engine_tablelimits->sblind() && i%p_tablemap->nchairs() != sbchair)
			{
				bbchair = i%p_tablemap->nchairs();	
				_bblindbits = k_exponents[bbchair];
				_playersblindbits |= k_exponents[bbchair];		
				
			}
		}
		
		// SB not found correction.
		// Will only apply if we are the bb + missed action(s). most common case. 
		// Restrictions : 3 or less players were dealt or last bb is active
		if (sbchair == k_undefined && (p_symbol_engine_active_dealt_playing->playersdealtbits() < 3 || (bbchair == USER_CHAIR && p_symbol_engine_positions->nchairsdealtright() == 1)))
		{
			for (int i=DEALER_CHAIR+1; i<DEALER_CHAIR+p_tablemap->nchairs(); i++)
			{
				double p_bet = p_scraper->player_bet(i%p_tablemap->nchairs());

				// 1st caller/raiser after dealer is sb
				if (p_bet >= p_symbol_engine_tablelimits->bblind() && sbchair == k_undefined && i%p_tablemap->nchairs() != bbchair)
				{
					sbchair = i%p_tablemap->nchairs();
					_playersblindbits |= k_exponents[sbchair];
				}
			}
		}
	}							
}