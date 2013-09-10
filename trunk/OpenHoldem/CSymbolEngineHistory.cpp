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
	// Element 0 is unused
	for (int i=0; i<(k_number_of_betrounds+1); i++)
	{
		_nplayersround[i] = 0;
		_nbetsround[i] = 0;
		for (int j=0; j<=k_autoplayer_function_fold; j++)
		{
			_autoplayer_actions[i][j] = 0;
		}
	}
}

void CSymbolEngineHistory::ResetOnNewRound()
{}

void CSymbolEngineHistory::ResetOnMyTurn()
{}

void CSymbolEngineHistory::ResetOnHeartbeat()
{
	CalculateHistory();
}

void CSymbolEngineHistory::RegisterAction(int autoplayer_action_code)
{
	AssertRange(autoplayer_action_code, k_autoplayer_function_allin,
		k_autoplayer_function_fold);
	_autoplayer_actions[BETROUND][autoplayer_action_code]++;
	SetPrevaction(autoplayer_action_code);
}

// Attention: SetPrevaction takes an OH-autoplayer-constant as input,
// but needs to translate it to an old-style Winholdem-prevaction-constant
void CSymbolEngineHistory::SetPrevaction(int autoplayer_action_code)
{
	switch (autoplayer_action_code)
	{
		case k_autoplayer_function_allin:
			_prevaction = k_prevaction_allin;
			break;
		case k_autoplayer_function_betpot_2_1:		
		case k_autoplayer_function_betpot_1_1:
		case k_autoplayer_function_betpot_3_4:
		case k_autoplayer_function_betpot_2_3:
		case k_autoplayer_function_betpot_1_2:
		case k_autoplayer_function_betpot_1_3:
		case k_autoplayer_function_betpot_1_4:
			_prevaction = k_prevaction_betsize;
			break;
		case k_autoplayer_function_betsize:
			_prevaction = k_prevaction_betsize;
			break;
		case k_autoplayer_function_raise:
			_prevaction = k_prevaction_raise;
			break;
		case k_autoplayer_function_call:
			_prevaction = k_prevaction_call;
			break;
		case k_autoplayer_function_check:
			_prevaction = k_prevaction_check;
			break;
		case k_autoplayer_function_fold:
			_prevaction = k_prevaction_fold;
			break;
		default:
			_prevaction = k_prevaction_undefined;
			break;
	};
}

void CSymbolEngineHistory::CalculateHistory()
{
	if (_nplayersround[BETROUND] == 0)
	{
		_nplayersround[BETROUND] = 
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

	if (p_symbol_engine_tablelimits->bet() > 0)
	{
		maxbet /= p_symbol_engine_tablelimits->bet();
	}
	if (maxbet > _nbetsround[BETROUND])
	{
		_nbetsround[BETROUND] = maxbet;									// nbetsroundx
	}
}