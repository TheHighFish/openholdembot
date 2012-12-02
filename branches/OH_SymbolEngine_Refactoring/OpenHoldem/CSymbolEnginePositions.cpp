#include "stdafx.h"
#include "CSymbolEnginePositions.h"

#include "CScraper.h"
#include "CStringMatch.h"

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
	found_userchair = false;
	for (int i=dealerchair+1; i<=dealerchair+p_tablemap->nchairs() && _user_chair_confirmed; i++)
	{
		double p_bet = p_scraper->player_bet(i%p_tablemap->nchairs());

		if ((i%p_tablemap->nchairs()) == _sym.userchair)
		{
			found_userchair = true;
		}
		else if ((int) _sym.playersdealtbits & (1<<(i%p_tablemap->nchairs())))
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
	for (int i=_sym.dealerchair+1; i<=_sym.dealerchair+p_tablemap->nchairs() && (i%p_tablemap->nchairs())!=_sym.raischair; i++)
	{
		if ((int)_sym.playersplayingbits & (1<<(i%p_tablemap->nchairs())))
		{
			_betpositionrais++;
		}
		if ((int)_sym.playersdealtbits & (1<<(i%p_tablemap->nchairs())))
		{
			_dealpositionrais++;
		}
	}
}

void CSymbolEnginePositions::CalculatePositionsForTheUserchair()
{
	for (int i=_sym.dealerchair+1; i<=_sym.dealerchair+p_tablemap->nchairs() && (i%p_tablemap->nchairs())!=_sym.userchair; i++)
	{
		if ((int)_sym.playersplayingbits & (1<<(i%p_tablemap->nchairs())))
		{
			_betposition++;
		}
		if ((int)_sym.playersdealtbits & (1<<(i%p_tablemap->nchairs())))
		{
			_dealposition++;
		}
	}

	for (int i=_sym.raischair+1; i<=_sym.raischair+p_tablemap->nchairs() && (i%p_tablemap->nchairs())!=_sym.userchair; i++)
	{
		if ((int)_sym.playersplayingbits & (1<<(i%p_tablemap->nchairs())))
		{
			_callposition++;
		}
	}
}