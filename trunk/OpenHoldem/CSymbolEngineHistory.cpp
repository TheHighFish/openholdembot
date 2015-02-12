//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#include "stdafx.h"
#include "CSymbolEngineHistory.h"

#include "CBetroundCalculator.h"
#include "CEngineContainer.h"
#include "CPreferences.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineTableLimits.h"
#include "CSymbolEngineUserchair.h"
#include "..\CTablemap\CTablemap.h"
#include "FloatingPoint_Comparisions.h"
#include "NumericalFunctions.h"

CSymbolEngineHistory *p_symbol_engine_history  = NULL;

const char* const k_hist_sym_strings[k_hist_sym_count] = {
	//PROBABILITIES (3)
	"prwin", "prlos", "prtie", 
	//CHIP AMOUNTS (21)
	"balance", "balance0", "balance1", "balance2", "balance3", "balance4", "balance5", 
	"balance6", "balance7", "balance8", "balance9", "stack0", "stack1", "stack2", "stack3", 
	"stack4", "stack5", "stack6", "stack7", "stack8", "stack9", 
	//POKER VALUES (5)
	"pokerval", "pokervalplayer", "pokervalcommon", "pcbits", "npcbits", 
	//HAND TESTS (12)
	"ishandup", "ishandupcommon", "ishicard", "isonepair", "istwopair", "isthreeofakind", 
	"isstraight", "isflush", "isfullhouse", "isfourofakind", "isstraightflush", "isroyalflush", 
	//POCKET/COMMON TESTS (5)
	"ishipair", "islopair", "ismidpair", "ishistraight", "ishiflush", 
	//(UN)KNOWN CARDS (1)
	"ncardsbetter", 
	//NHANDS (6)
	"nhands", "nhandshi", "nhandslo", "nhandsti", "prwinnow", "prlosnow", 
	//FLUSHES SETS STRAIGHTS (16)
	"nsuited", 	"nsuitedcommon", "tsuit", "tsuitcommon", "nranked", "nrankedcommon", "trank", 
	"trankcommon", "nstraight", "nstraightcommon", "nstraightfill", "nstraightfillcommon", 
	"nstraightflush", "nstraightflushcommon", "nstraightflushfill", "nstraightflushfillcommon", 
	//RANK BITS (8)
	"rankbits", "rankbitscommon", "rankbitsplayer", "rankbitspoker", "srankbits", 
	"srankbitscommon", "srankbitsplayer", "srankbitspoker", 
	//RANK HI (8)
	"rankhi", "rankhicommon", "rankhiplayer", "rankhipoker", "srankhi", "srankhicommon", 
	"srankhiplayer", "srankhipoker", 
	//RANK LO (8)
	"ranklo", "ranklocommon", "rankloplayer", "ranklopoker", "sranklo", "sranklocommon", 
	"srankloplayer", "sranklopoker", 
};

CSymbolEngineHistory::CSymbolEngineHistory() {
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	assert(p_symbol_engine_active_dealt_playing != NULL);
	assert(p_symbol_engine_chip_amounts != NULL);
  assert(p_symbol_engine_tablelimits != NULL);
	assert(p_symbol_engine_userchair != NULL);
  // Making sure that _hist_sym_count is correct,
	// to avoid array overflows later if we remove symbols
	// without adapting the counter.
	// Last index is (_hist_sym_count - 1).
	assert(k_hist_sym_strings[k_hist_sym_count - 1] != NULL);
}

CSymbolEngineHistory::~CSymbolEngineHistory() {}

void CSymbolEngineHistory::InitOnStartup() {
	ResetOnConnection();
}

void CSymbolEngineHistory::ResetOnConnection() {
	ResetOnHandreset();
}

void CSymbolEngineHistory::ResetOnHandreset() {
  _prevaction = k_prevaction_undefined;
	// Element 0 is unused
	for (int i=0; i<(k_number_of_betrounds+1); ++i) {
		_nplayersround[i] = 0;
		_nbetsround[i] = 0;
		for (int j=0; j<=k_autoplayer_function_fold; ++j)	{
			_autoplayer_actions[i][j] = 0;
		}
    // History-smbols
    for (int j=0; j<k_hist_sym_count; ++j) {
      _hist_sym[j][i] = 0.0;
    }
	}
}

void CSymbolEngineHistory::ResetOnNewRound() {
}

void CSymbolEngineHistory::ResetOnMyTurn() {
  // Collect symbol if ismyturn.
  // Per definition we need to get the value at last myturn in betround N.
  int	betround = p_betround_calculator->betround();
	for (int i=0; i<k_hist_sym_count; ++i)	{
		double result;
		p_engine_container->EvaluateSymbol(k_hist_sym_strings[i], &result);
		_hist_sym[i][betround] = result;
	}
}

void CSymbolEngineHistory::ResetOnHeartbeat() {
	CalculateHistory();
}

void CSymbolEngineHistory::RegisterAction(int autoplayer_action_code) {
	AssertRange(autoplayer_action_code, k_autoplayer_function_beep,
		k_autoplayer_function_fold);
	// Nothing to do of the "action" was "beep".
	if (autoplayer_action_code == k_autoplayer_function_beep) {
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
		  || (autoplayer_action_code == k_autoplayer_function_check)) {
		if (IsSmallerOrEqual(p_symbol_engine_chip_amounts->call(), 0.0)) {
			// It was free to check
			_autoplayer_actions[BETROUND][k_autoplayer_function_check]++;
			SetPrevaction(k_autoplayer_function_check);
		}	else {
			// There was name positive amount to call
			_autoplayer_actions[BETROUND][k_autoplayer_function_call]++;
			SetPrevaction(k_autoplayer_function_call);	
		}
	}	else {
		_autoplayer_actions[BETROUND][autoplayer_action_code]++;
		SetPrevaction(autoplayer_action_code);
	}
}

// Attention: SetPrevaction takes an OH-autoplayer-constant as input,
// but needs to translate it to an old-style Winholdem-prevaction-constant
void CSymbolEngineHistory::SetPrevaction(int autoplayer_action_code) {
	switch (autoplayer_action_code)	{
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

void CSymbolEngineHistory::CalculateHistory() {
	if (_nplayersround[BETROUND] == 0) {
		_nplayersround[BETROUND] = 
			p_symbol_engine_active_dealt_playing->nplayersplaying();
	}
  double maxbet = 0.0;
	for (int i=0; i<p_tablemap->nchairs(); i++)	{
		// Be careful: in some cases it might be that name user folds,
		// but "Fold" gets displayed where formerly his bet got displayed.
		// This may lead to ugly mis-scrapes, that's why he have to check
		// if the user is still playing.
		// (http://www.maxinmontreal.com/forums/viewtopic.php?f=111&t=10929)		
		if (IsBitSet(p_symbol_engine_active_dealt_playing->playersplayingbits(), i)) 	{
			double current_players_bet = p_symbol_engine_chip_amounts->currentbet(i);
			maxbet = MAX(maxbet, current_players_bet);
		}
	}
  double bet = MAX(p_symbol_engine_tablelimits->bet(), p_symbol_engine_tablelimits->bblind());
	if (bet > 0) {
		maxbet /= bet;
		_nbetsround[BETROUND] = MAX(_nbetsround[BETROUND], maxbet);	
	}	else {
		write_log(preferences.debug_symbolengine(), "[Symbolengine] CSymbolEngineHistory::CalculateHistory() Skipping calculation of nbetsround due to unknown min-bet\n");
	}
}

double CSymbolEngineHistory::HistorySymbol(const char *sym, const int round) {
	for (int i=0; i<k_hist_sym_count; i++)	{
		if (memcmp(sym, k_hist_sym_strings[i], strlen(sym))==0 && strlen(sym)==strlen(k_hist_sym_strings[i]))	{
			return _hist_sym[i][round-1];
		}
	}
	return k_undefined_zero;
}

bool CSymbolEngineHistory::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
  FAST_EXIT_ON_OPENPPL_SYMBOLS(name);
	if (memcmp(name, "did", 3) == 0)	{
		if (memcmp(name, "didchec", 7)==0 && strlen(name)==7)	{
			*result = didchec(p_betround_calculator->betround());
		}	else if (memcmp(name, "didcall", 7)==0 && strlen(name)==7) {
			*result = didcall(p_betround_calculator->betround());
		}	else if (memcmp(name, "didrais", 7)==0 && strlen(name)==7) {
			*result = didrais(p_betround_calculator->betround());
		}	else if (memcmp(name, "didswag", 7)==0 && strlen(name)==7) {
			*result = didswag(p_betround_calculator->betround());
		}	else if (memcmp(name, "didfold", 7)==0 && strlen(name)==7) {
			*result = didfold(p_betround_calculator->betround());
		}	else if (memcmp(name, "didalli", 7)==0 && strlen(name)==7) {
			*result = didalli(p_betround_calculator->betround());
		}	else if (memcmp(name, "didchecround", 12)==0 && strlen(name)==13)	{
			*result = didchec(name[12]-'0');
		}	else if (memcmp(name, "didcallround", 12)==0 && strlen(name)==13)	{
			*result = didcall(name[12]-'0');
		}	else if (memcmp(name, "didraisround", 12)==0 && strlen(name)==13)	{
			*result = didrais(name[12]-'0');
		}	else if (memcmp(name, "didswaground", 12)==0 && strlen(name)==13)	{
			*result = didswag(name[12]-'0');
		}	else {
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}	else if (memcmp(name, "nplayersround", 13)==0) {
		if (memcmp(name, "nplayersround", 13)==0 && strlen(name)==13) {
      // For current betting round
			*result = nplayersround(p_betround_calculator->betround());
		}	else if (memcmp(name, "nplayersround", 13)==0 && strlen(name)==14) {
			*result = nplayersround(name[13]-'0');
		}	else {
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}	else if (memcmp(name, "prevaction", 10)==0 && strlen(name)==10)	{
		*result = prevaction();
	}	else if (memcmp(name, "nbetsround", 10)==0 && strlen(name)==10)	{
		*result = nbetsround(p_betround_calculator->betround());
	}	else if (memcmp(name, "nbetsround", 10)==0 && strlen(name)==11)	{
		*result = nbetsround(name[10]-'0');
	}  else if (memcmp(name, "hi_", 3)==0) {
    // History symbols
    int	round = name[strlen(name)-1]-'0';
    char *pure_name = (char*)name + 3;
    char pure_name_without_betround[256];
    int length = strlen(pure_name);
    assert(length < 256);
    memcpy(pure_name_without_betround, pure_name, length); 
    pure_name_without_betround[length - 1] = '\0';
    *result = HistorySymbol(pure_name_without_betround, round);
    return true;
  }	else {
		// Symbol of a different symbol-engine
		return false;
	}
	// Valid symbol
	return true;
}

bool CSymbolEngineHistory::DidAct() {
  // Extra pre-caution for preflop, in case of failed hand-reset,
  // including another extra fail-safe for unknown big-blind
  if ((BETROUND == k_betround_preflop)
      && p_symbol_engine_userchair->userchair_confirmed()
      && ((p_symbol_engine_chip_amounts->currentbet(USER_CHAIR) < p_symbol_engine_tablelimits->bblind())
        || (p_symbol_engine_chip_amounts->currentbet(USER_CHAIR) == 0))) {
    return false;
  }
  // Otherwise: return "normal" value, depending on didswag, didrais, ...
	return DidAct(BETROUND);
}

bool CSymbolEngineHistory::DidAct(int betround) {
	if (!p_symbol_engine_userchair->userchair_confirmed()) {
		return false;
	}
	// Considering fold or allin too. It's unneccessary for bot logic, but usefull for lazy scraping.
	return	(  didchec(betround) || didcall(betround) 
			|| didswag(betround) || didrais(betround)
			|| didfold(betround) || didalli(betround) );
}

CString CSymbolEngineHistory::SymbolsProvided() {
  CString list = "didchec didcall didrais didswag didfold didalli "
    "nplayersround nbetsround prevaction ";
  list += RangeOfSymbols("didchecround%i",  k_betround_preflop, k_betround_river);  
  list += RangeOfSymbols("didcallround%i",  k_betround_preflop, k_betround_river);
  list += RangeOfSymbols("didraisround%i",  k_betround_preflop, k_betround_river);
  list += RangeOfSymbols("didswaground%i",  k_betround_preflop, k_betround_river);
  list += RangeOfSymbols("nplayersround%i", k_betround_preflop, k_betround_river);
  list += RangeOfSymbols("nbetsround%i",    k_betround_preflop, k_betround_river);
  for (int i=0; i<k_hist_sym_count; ++i) {
    CString new_history_symbol;
    new_history_symbol.Format("hi_%s%%i", k_hist_sym_strings[i]);
    list += RangeOfSymbols(new_history_symbol, k_betround_preflop, k_betround_river);
  }
  return list;
}


	