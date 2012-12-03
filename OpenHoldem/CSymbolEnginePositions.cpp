#include "stdafx.h"
#include "CSymbolEnginePositions.h"

#include "CScraper.h"
#include "CStringMatch.h"
#include "CSymbolEngineUserchair.h"

CSymbolEnginePositions::CSymbolEnginePositions()
{}

CSymbolEnginePositions::~CSymbolEnginePositions()
{}

void CSymbolEnginePositions::InitOnStartup()
{}

void CSymbolEnginePositions::ResetOnConnection()
{}

void CSymbolEnginePositions::ResetOnHandreset()
{}

void CSymbolEnginePositions::ResetOnNewRound()
{}

void CSymbolEnginePositions::ResetOnMyTurn()
{
	CalculatePositionForTheRaiser();
	CalculatePositionsForTheUserchair();
}

void CSymbolEnginePositions::ResetOnHeartbeat()
{
	CalculateNChairsDealtLeftRight();
}

void CSymbolEnginePositions::CalculateNChairsDealtLeftRight()
{
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
}

void CSymbolEnginePositions::CalculatePositionForTheRaiser()
{
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
}

void CSymbolEnginePositions::CalculatePositionsForTheUserchair()
{
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
}