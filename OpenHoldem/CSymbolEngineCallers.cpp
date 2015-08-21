//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose:
//
//*******************************************************************************

#include "stdafx.h"
#include "CSymbolEngineCallers.h"

#include <assert.h>
#include "CBetroundCalculator.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CStringMatch.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineDealerchair.h"
#include "CSymbolEngineHistory.h"
#include "CSymbolEngineTableLimits.h"
#include "CSymbolEngineUserchair.h"
#include "CPreferences.h"
#include "CTableState.h"
#include "NumericalFunctions.h"
#include "StringFunctions.h"

CSymbolEngineCallers *p_symbol_engine_callers = NULL;

// Some symbols are only well-defined if it is my turn
#define RETURN_UNDEFINED_VALUE_IF_NOT_MY_TURN { if (!p_symbol_engine_autoplayer->ismyturn()) *result = kUndefined; }

CSymbolEngineCallers::CSymbolEngineCallers() {
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	assert(p_symbol_engine_active_dealt_playing != NULL);
  assert(p_symbol_engine_autoplayer != NULL);
	assert(p_symbol_engine_chip_amounts != NULL);
	assert(p_symbol_engine_dealerchair != NULL);
	assert(p_symbol_engine_tablelimits != NULL);
	assert(p_symbol_engine_userchair != NULL);
	// Also using p_symbol_engine_history one time,
	// but because we use "old" information here
	// there is no dependency on this cycle.
}

CSymbolEngineCallers::~CSymbolEngineCallers() {
}

void CSymbolEngineCallers::InitOnStartup() {
}

void CSymbolEngineCallers::ResetOnConnection() {
  _nchairs = p_tablemap->nchairs();
	ResetOnHandreset();
}

void CSymbolEngineCallers::ResetOnHandreset() {
	// callbits, raisbits, etc.
	for (int i=kBetroundPreflop; i<=kBetroundRiver; i++) {
		_callbits[i] = 0;
	}
	_nopponentscalling  = 0;
}

void CSymbolEngineCallers::ResetOnNewRound() {
  _firstcaller_chair = kUndefined;
  _lastcaller_chair = kUndefined;
}

void CSymbolEngineCallers::ResetOnMyTurn() {
	CalculateCallers();
}

void CSymbolEngineCallers::ResetOnHeartbeat() {
}

void CSymbolEngineCallers::CalculateCallers() {
	// nopponentscalling
	//
	// nopponentscalling is "difficult" to calculate 
  // and has to work only when it is our turn.
  // Then we can simply start searching after the userchair 
  // (or dealer on first action preflop)
	// and do a circular search for callers.
  _nopponentscalling = 0;
  _firstcaller_chair = kUndefined;
  _lastcaller_chair = kUndefined;
	double current_bet = FirstPossibleRaisersBet();
	for (int i=FirstPossibleCaller(); 
		  (i%_nchairs)!=USER_CHAIR; 
		  ++i) {
		int chair = i % _nchairs;
    	// Exact match required. Players being allin don't count as callers.
		if ((p_table_state->_players[chair]._bet == current_bet) 
        && (current_bet > 0)) {
			int new_callbits = _callbits[BETROUND] | k_exponents[chair];
			_callbits[BETROUND] = new_callbits;
      ++_nopponentscalling;
      // We have a caller, at least the temporary last one
      _lastcaller_chair = chair;
      if (_firstcaller_chair == kUndefined) {
        // We found the first caller
        _firstcaller_chair = chair;
      }
		}
		else if (p_table_state->_players[chair]._bet > current_bet) {
			current_bet = p_table_state->_players[chair]._bet;
		}
	}
	AssertRange(_callbits[BETROUND], 0, k_bits_all_ten_players_1_111_111_111);
  AssertRange(_nopponentscalling,   0, kMaxNumberOfPlayers);
}

int CSymbolEngineCallers::FirstPossibleCaller() {
  // First action preflop> the guy behind the dealer
  if ((p_betround_calculator->betround() == kBetroundPreflop)
      && !p_symbol_engine_history->DidAct()) {
    return ((DEALER_CHAIR + 1) % _nchairs);
  }
  // Otherwise> the guy behind hero,
  // searching a complete orbit from hero to hero.
  return 0;
}

int CSymbolEngineCallers::LastPossibleCaller() {
  // The guy before the user
  return ((USER_CHAIR + _nchairs - 1) % _nchairs);
}

double CSymbolEngineCallers::FirstPossibleRaisersBet() {
  double users_bet = p_table_state->User()->_bet;
  if (users_bet >= p_symbol_engine_tablelimits->bblind()) {
    return users_bet;
  }
  // Othewrwise> return something that is
  //   * greater than 0 to avoid counting checkers as callers
  //   * is smaller than BB to avoid counting the BB or a min-bettor as caller
  return (0.9 * p_symbol_engine_tablelimits->bblind());
}

bool CSymbolEngineCallers::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
  FAST_EXIT_ON_OPENPPL_SYMBOLS(name);
	if (memcmp(name, "nopponentscalling", 17)==0 && strlen(name)==17) {
    RETURN_UNDEFINED_VALUE_IF_NOT_MY_TURN
		*result = nopponentscalling();
		return true;
	}	else if (memcmp(name, "callbits", 8)==0 && strlen(name)==9) {
    RETURN_UNDEFINED_VALUE_IF_NOT_MY_TURN
		*result = callbits(RightDigitCharacterToNumber(name));
    return true;
  } else if (memcmp(name, "firstcaller_chair", 17)==0) {
		*result = _firstcaller_chair;
		return true;
	} else if (memcmp(name, "lastcaller_chair", 16)==0) {
		*result = _lastcaller_chair;
		return true;
  }
	// Symbol of a different symbol-engine
	return false;
}

CString CSymbolEngineCallers::SymbolsProvided() {
  CString list = "nopponentscalling firstcaller_chair lastcaller_chair ";
  list += RangeOfSymbols("callbits%i", kBetroundPreflop, kBetroundRiver);
  return list;
}