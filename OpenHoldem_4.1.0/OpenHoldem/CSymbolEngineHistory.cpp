#include "stdafx.h"
#include "CSymbolEngineHistory.h"

#include "CBetroundCalculator.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineChipAmounts.h"
#include "..\CTablemap\CTablemap.h"
#include "NumericalFunctions.h"

CSymbolEngineHistory *p_symbol_engine_history  = NULL;

CSymbolEngineHistory::CSymbolEngineHistory()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	assert(p_symbol_engine_active_dealt_playing != NULL);
	assert(p_symbol_engine_chip_amounts != NULL);
	assert(p_symbol_engine_userchair != NULL);

	_betround = k_betround_preflop;
}

CSymbolEngineHistory::~CSymbolEngineHistory()
{}

void CSymbolEngineHistory::InitOnStartup()
{
	ResetOnConnection();
}

void CSymbolEngineHistory::ResetOnConnection()
{
	ResetOnHandreset();
}

void CSymbolEngineHistory::ResetOnHandreset()
{
	_betround = k_betround_preflop; 

	// Element 0 is unused
	for (int i=0; i<(k_number_of_betrounds+1); i++)
	{
		_nplayersround[i] = 0;
		_nbetsround[i] = 0;
		_didchec[i] = 0;
		_didcall[i] = 0;
		_didrais[i] = 0;
		_didswag[i] = 0;
	}
}

void CSymbolEngineHistory::ResetOnNewRound()
{}

void CSymbolEngineHistory::ResetOnMyTurn()
{}

void CSymbolEngineHistory::ResetOnHeartbeat()
{
	_betround = p_betround_calculator->betround();
	CalculateHistory();
}


void CSymbolEngineHistory::CalculateHistory()
{
	if (_nplayersround[_betround] == 0)
	{
		_nplayersround[_betround] = 
			p_symbol_engine_active_dealt_playing->nplayersplaying();
	}

	double maxbet = 0.0;
	for (int i=0; i<p_tablemap->nchairs(); i++)
	{
		// Be careful: in some cases it might be that a user folds,
		// but "Fold" gets displayed where formerly his bet got displayed.
		// This may lead to ugly mis-scrapes, that's why he have to check
		// if the user is still playing.
		// (http://www.maxinmontreal.com/forums/viewtopic.php?f=111&t=10929)
		double current_players_bet = p_symbol_engine_chip_amounts->currentbet(i);
		if ((current_players_bet > maxbet)
			&& IsBitSet(p_symbol_engine_active_dealt_playing->playersplayingbits(), i))
		{
			maxbet = current_players_bet;
		}
	}

	if (p_tablelimits->bet() > 0)
	{
		maxbet /= p_tablelimits->bet();
	}
	if (maxbet > _nbetsround[_betround])
	{
		_nbetsround[_betround] = maxbet;									// nbetsroundx
	}
}