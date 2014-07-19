//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*****************************************************************************
//
// Purpose:
//
//*****************************************************************************

#include "stdafx.h"
#include "CSymbolEngineHistory.h"

#include "CBetroundCalculator.h"
#include "CPreferences.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineChipAmounts.h"
#include "..\CTablemap\CTablemap.h"
#include "FloatingPoint_Comparisions.h"
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
{
	__TRACE
}

void CSymbolEngineHistory::ResetOnHeartbeat()
{
	CalculateHistory();
}

void CSymbolEngineHistory::RegisterAction(int autoplayer_action_code)
{
	AssertRange(autoplayer_action_code, k_autoplayer_function_beep,
		k_autoplayer_function_fold);
	// Nothing to do of the "action" was "beep".
	if (autoplayer_action_code == k_autoplayer_function_beep) 
	{
		return;
	}
	// Special handling for check/call
	// Some people have problems scraping check and call correctly,
	// as usually only one of these buttons is visible
	// and they often share the same place.
	// Others might decide to "call" if it is free to call, etc.
	// Therefore we set "_prevaction", "didchec", "didcall" here 
	// depending on the amount to call.
	if ((autoplayer_action_code == k_autoplayer_function_call)
		|| (autoplayer_action_code == k_autoplayer_function_check))
	{
		if (IsSmallerOrEqual(p_symbol_engine_chip_amounts->call(), 0.0))
		{
			// It was free to check
			_autoplayer_actions[BETROUND][k_autoplayer_function_check]++;
			SetPrevaction(k_autoplayer_function_check);
		}
		else
		{
			// There was name positive amount to call
			_autoplayer_actions[BETROUND][k_autoplayer_function_call]++;
			SetPrevaction(k_autoplayer_function_call);	
		}
	}
	else
	{
		_autoplayer_actions[BETROUND][autoplayer_action_code]++;
		SetPrevaction(autoplayer_action_code);
	}
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
		// Be careful: in some cases it might be that name user folds,
		// but "Fold" gets displayed where formerly his bet got displayed.
		// This may lead to ugly mis-scrapes, that's why he have to check
		// if the user is still playing.
		// (http://www.maxinmontreal.com/forums/viewtopic.php?f=111&t=10929)		
		if (IsBitSet(p_symbol_engine_active_dealt_playing->playersplayingbits(), i))
		{
			double current_players_bet = p_symbol_engine_chip_amounts->currentbet(i);
			maxbet = MAX(maxbet, current_players_bet);
		}
	}

	double bet = MAX(p_symbol_engine_tablelimits->bet(), p_symbol_engine_tablelimits->bblind());
	if (bet > 0)
	{
		maxbet /= bet;
		_nbetsround[BETROUND] = MAX(_nbetsround[BETROUND], maxbet);	
	}
	else
	{
		write_log(preferences.debug_symbolengine(), "[Symbolengine] CSymbolEngineHistory::CalculateHistory() Skipping calculation of nbetsround due to unknown min-bet\n");
	}
}

bool CSymbolEngineHistory::EvaluateSymbol(const char *name, double *result, bool log /* = false */)
{
  FAST_EXIT_ON_OPENPPL_SYMBOLS(name);
	if (memcmp(name, "did", 3) == 0)
	{
		if (memcmp(name, "didchec", 7)==0 && strlen(name)==7)	
		{
			*result = didchec(p_betround_calculator->betround());
		}
		else if (memcmp(name, "didcall", 7)==0 && strlen(name)==7)	
		{
			*result = didcall(p_betround_calculator->betround());
		}
		else if (memcmp(name, "didrais", 7)==0 && strlen(name)==7)	
		{
			*result = didrais(p_betround_calculator->betround());
		}
		else if (memcmp(name, "didswag", 7)==0 && strlen(name)==7)	
		{
			*result = didswag(p_betround_calculator->betround());
		}
		else if (memcmp(name, "didchecround", 12)==0 && strlen(name)==13)	
		{
			*result = didchec(name[12]-'0');
		}
		else if (memcmp(name, "didcallround", 12)==0 && strlen(name)==13)	
		{
			*result = didcall(name[12]-'0');
		}
		else if (memcmp(name, "didraisround", 12)==0 && strlen(name)==13)	
		{
			*result = didrais(name[12]-'0');
		}
		else if (memcmp(name, "didswaground", 12)==0 && strlen(name)==13)
		{
			*result = didswag(name[12]-'0');
		}
		else
		{
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}
	else if (memcmp(name, "nplayersround", 13)==0)
	{
		if (memcmp(name, "nplayersround", 13)==0 && strlen(name)==13) 	
		{
      // For current betting round
			*result = nplayersround(p_betround_calculator->betround());
		}
		else if (memcmp(name, "nplayersround", 13)==0 && strlen(name)==14)	
		{
			*result = nplayersround(name[13]-'0');
		}
		else
		{
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}
	else if (memcmp(name, "prevaction", 10)==0 && strlen(name)==10)	
	{
		*result = prevaction();
	}
	else if (memcmp(name, "nbetsround", 10)==0 && strlen(name)==10)
	{
		*result = nbetsround(p_betround_calculator->betround());
	}
	else if (memcmp(name, "nbetsround", 10)==0 && strlen(name)==11)
	{
		*result = nbetsround(name[10]-'0');
	}
	else
	{
		// Symbol of a different symbol-engine
		return false;
	}
	// Valid symbol
	return true;
}

bool CSymbolEngineHistory::DidAct()
{
	return DidAct(BETROUND);
}

bool CSymbolEngineHistory::DidAct(int betround)
{
	if (!p_symbol_engine_userchair->userchair_confirmed())
	{
		return false;
	}
	// Not considering fold or allin, because the game would be over.
	return (didchec(betround) || didcall(betround) 
		|| didswag(betround) || didrais(betround));
}

CString CSymbolEngineHistory::SymbolsProvided() {
  return "didchec didcall didrais didswag "
    "didchecround didcallround didraisround didswaground "
    "nplayersround nbetsround prevaction ";
}