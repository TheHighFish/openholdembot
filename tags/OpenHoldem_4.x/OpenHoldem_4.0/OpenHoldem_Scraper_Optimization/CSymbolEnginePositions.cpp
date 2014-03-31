#include "stdafx.h"
#include "CSymbolEnginePositions.h"

#include <assert.h>
#include "CScraper.h"
#include "CStringMatch.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineDealerchair.h"
#include "CSymbolEngineRaisersCallers.h"
#include "CSymbolEngineUserchair.h"

CSymbolEnginePositions *p_symbol_engine_positions = NULL;

CSymbolEnginePositions::CSymbolEnginePositions()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	assert(p_symbol_engine_active_dealt_playing != NULL);
	assert(p_symbol_engine_dealerchair != NULL);
	assert(p_symbol_engine_userchair != NULL);
}

CSymbolEnginePositions::~CSymbolEnginePositions()
{}

void CSymbolEnginePositions::InitOnStartup()
{}

void CSymbolEnginePositions::ResetOnConnection()
{}

void CSymbolEnginePositions::ResetOnHandreset()
{
	_userchair = p_symbol_engine_userchair->userchair();
}

void CSymbolEnginePositions::ResetOnNewRound()
{}

void CSymbolEnginePositions::ResetOnMyTurn()
{
	CalculatePositionForTheRaiser();
	CalculatePositionsForTheUserchair();
}

void CSymbolEnginePositions::ResetOnHeartbeat()
{
	_dealerchair        = p_symbol_engine_dealerchair->dealerchair();
	_raischair          = p_symbol_engine_raisers_callers->raischair();
	_playersdealtbits   = p_symbol_engine_active_dealt_playing->playersdealtbits();
	_playersplayingbits = p_symbol_engine_active_dealt_playing->playersplayingbits();

	CalculateNChairsDealtLeftRight();
}

void CSymbolEnginePositions::CalculateNChairsDealtLeftRight()
{
	_nchairsdealtright = 0;
	_nchairsdealtleft  = 0;

	bool found_userchair = false;
	for (int i=_dealerchair+1; 
		i<=_dealerchair+p_tablemap->nchairs() 
			&& p_symbol_engine_userchair->userchair_confirmed();
		i++)
	{
		double p_bet = p_scraper->player_bet(i%p_tablemap->nchairs());

		if ((i%p_tablemap->nchairs()) == _userchair)
		{
			found_userchair = true;
		}
		else if (_playersdealtbits & (1<<(i%p_tablemap->nchairs())))
		{
			if (!found_userchair)
			{
				_nchairsdealtright++;
			}
			else
			{
				_nchairsdealtleft++;
			}
		}
	}
	AssertRange(_nchairsdealtright, 0, (k_max_number_of_players - 1));
	AssertRange(_nchairsdealtleft,  0, (k_max_number_of_players - 1));
}

void CSymbolEnginePositions::CalculatePositionForTheRaiser()
{
	_betpositionrais  = 0;
	_dealpositionrais = 0;

	for (int i=_dealerchair+1; 
		i<=(_dealerchair+p_tablemap->nchairs()) 
			&& (i%p_tablemap->nchairs())!=_raischair; 
		i++)
	{
		if (_playersplayingbits & (1<<(i%p_tablemap->nchairs())))
		{
			_betpositionrais++;
		}
		if (_playersdealtbits & (1<<(i%p_tablemap->nchairs())))
		{
			_dealpositionrais++;
		}
	}
	AssertRange(_betpositionrais,  k_undefined, k_max_number_of_players);
	AssertRange(_dealpositionrais, k_undefined, k_max_number_of_players);
}

void CSymbolEnginePositions::CalculatePositionsForTheUserchair()
{
	_betposition  = 0;
	_dealposition = 0;
	_callposition = 0;

	for (int i=_dealerchair+1; 
		i<=_dealerchair+p_tablemap->nchairs() 
			&& (i%p_tablemap->nchairs())!=_userchair; 
		i++)
	{
		if (_playersplayingbits & (1<<(i%p_tablemap->nchairs())))
		{
			_betposition++;
		}
		if (_playersdealtbits & (1<<(i%p_tablemap->nchairs())))
		{
			_dealposition++;
		}
	}

	for (int i=_raischair+1; i<=_raischair+p_tablemap->nchairs() && (i%p_tablemap->nchairs())!=_userchair; i++)
	{
		if (_playersplayingbits & (1<<(i%p_tablemap->nchairs())))
		{
			_callposition++;
		}
	}
	AssertRange(_betposition,  k_undefined, k_max_number_of_players);
	AssertRange(_dealposition, k_undefined, k_max_number_of_players);
	AssertRange(_callposition, k_undefined, k_max_number_of_players);
}