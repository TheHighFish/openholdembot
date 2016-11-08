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
//
// nopponentstruelyraising counts all people who voluntarily bet more than needed,
// especially:
//  * all raisers
//  * the first voluntary better postflop
// but not
//  * the infamous "blind-raisers" (Winholdem)
//  * people posting antes 
//
// nopponentstruelyraising counts only the info that is visible at the table,
// i.e. one orbit (max). Formerly it was well-defined only at out turn,
// but we try to make it well-defined all the time, mainly because
// people don't understand the restrictions of "Raises" (OpenPPl,
// implemented with the use of nopponentsraising:
// http://www.maxinmontreal.com/forums/viewtopic.php?f=297&t=18141)
//
// Now nopponentstruelyraising should count:
//  * a full orbit when it is our turn
//  * a partial orbit from dealer to hero if we did not yet act
//  * a psartial orbit behind us if we already acted 
//    (similar to RaisesSinceLastPlay, but might include a bettor)
//  * an orbit after the dealer if the userchair is unknown
//    (not really usable for OpenPPL which updates at our turn, 
//    but at least somewhat meaningful in the debug-tab).
//
//******************************************************************************

#include "stdafx.h"
#include "CSymbolEngineRaisers.h"

#include <assert.h>
#include "CBetroundCalculator.h"
#include "CScraper.h"
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
#include "..\StringFunctionsDLL\string_functions.h"

CSymbolEngineRaisers *p_symbol_engine_raisers = NULL;

CSymbolEngineRaisers::CSymbolEngineRaisers() {
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

CSymbolEngineRaisers::~CSymbolEngineRaisers() {
}

void CSymbolEngineRaisers::InitOnStartup() {
}

void CSymbolEngineRaisers::UpdateOnConnection() {
	UpdateOnHandreset();
}

void CSymbolEngineRaisers::UpdateOnHandreset() {
  // !!!!! Update symbols like raischair, ....
  // !!!!! only when it is my turn later!?
	// raisbits, foldbits, etc.
	for (int i=kBetroundPreflop; i<=kBetroundRiver; i++) {
		_raisbits[i] = 0;
    _lastraised[i] = kUndefined;
	}
	_raischair = kUndefined;
	_firstraiser_chair = kUndefined;
	_nopponentstruelyraising = 0;
}

void CSymbolEngineRaisers::UpdateOnNewRound() {
	_firstraiser_chair = kUndefined;
}

void CSymbolEngineRaisers::UpdateOnMyTurn() {
}

void CSymbolEngineRaisers::UpdateOnHeartbeat() {
  CalculateRaisers();
}

double CSymbolEngineRaisers::LastOrbitsLastRaisersBet() {
	// Not yet acted: 0
	if (!p_symbol_engine_history->DidAct()) {
    if (p_betround_calculator->betround() == kBetroundPreflop) {
      // Preflop:
      // Start with big blind and forget about former blind raisers
      return p_symbol_engine_tablelimits->bblind();
    } else {
      // Postflop
		  return 0.0;
    }
	}
	if (p_table_state->User()->HasKnownCards())	{
		// Otherwise: either we are the raiser (highest bet)
		// Or we called the raise (highest bet too)
		return p_table_state->User()->_bet.GetValue();
	}
	// Otherwise meaningless
	return 0.0;
}

void CSymbolEngineRaisers::CalculateRaisers() {
	_nopponentstruelyraising = 0;
	if (p_symbol_engine_chip_amounts->call() <= 0.0) 	{
		// There are no bets and raises.
		// Skip the calculations to keep the raischair of the previous round.
		// http://www.maxinmontreal.com/forums/viewtopic.php?f=156&t=16806
    write_log(preferences.debug_symbolengine(), 
      "[CSymbolEngineRaisers] No bet to call, therefore no raises\n");
		return;
	}
	// Raischair, nopponentstruelyraising, raisbits
	int first_possible_raiser = FirstPossibleRaiser();
	int last_possible_raiser  = LastPossibleRaiser();
	double highest_bet = LastOrbitsLastRaisersBet();
  write_log(preferences.debug_symbolengine(), "[CSymbolEngineRaisers] Searching for raisers from chair %i to %i with a bet higher than %.2f\n",
		first_possible_raiser, last_possible_raiser, highest_bet); 
	for (int i=first_possible_raiser; i<=last_possible_raiser; ++i) {
		int chair = i % p_tablemap->nchairs();
		double current_players_bet = p_table_state->Player(chair)->_bet.GetValue();
    write_log(preferences.debug_symbolengine(), 
      "[CSymbolEngineRaisers] chair %d bet %.2f\n",
      chair, current_players_bet);
		// Raisers are people
		// * with a higher bet than players before them
		// * who are still playing, not counting people who bet/fold in later orbits
    // * either betting/raising postflop or truely raising preflop
    //   (not counting the infamous "blind raisers")
    if (!p_table_state->Player(chair)->HasAnyCards()) {
      write_log(preferences.debug_symbolengine(), 
        "[CSymbolEngineRaisers] chair %d has no cards.\n", chair);
      continue;
    } else if (current_players_bet <= highest_bet) {
      write_log(preferences.debug_symbolengine(), 
        "[CSymbolEngineRaisers] chair %d is not raising\n", chair);
      continue;
    } else if ((p_betround_calculator->betround() == kBetroundPreflop)
				&& (current_players_bet <= p_symbol_engine_tablelimits->bblind())) {
      write_log(preferences.debug_symbolengine(), 
        "[CSymbolEngineRaisers] chair %d so-called \"blind raiser\". To be ignored.\n", chair);
      continue;
    } else if ((p_betround_calculator->betround() == kBetroundPreflop)
      && p_table_state->Player(chair)->PostingBothBlinds()) {
      write_log(preferences.debug_symbolengine(), 
        "[CSymbolEngineRaisers] chair %d is posting both blinds at once. To be ignored.\n", chair);
      continue;
    }
		highest_bet = current_players_bet;
    write_log(preferences.debug_symbolengine(), "[CSymbolEngineRaisers] Opponent %i raising to %s\n",
			chair, Number2CString(highest_bet));
		_raischair = chair;
		int new_raisbits = _raisbits[BETROUND] | k_exponents[chair];
		_raisbits[BETROUND] = new_raisbits;
		assert(chair != USER_CHAIR);
			if (_firstraiser_chair == kUndefined ){
			_firstraiser_chair = chair;
		}
		++_nopponentstruelyraising;
	}
	AssertRange(_raischair, kUndefined, kLastChair);
  _lastraised[BETROUND] = _raischair;
	write_log(preferences.debug_symbolengine(), "[CSymbolEngineRaisers] nopponentstruelyraising: %i\n", _nopponentstruelyraising);
	write_log(preferences.debug_symbolengine(), "[CSymbolEngineRaisers] raischair: %i\n", _raischair);
	write_log(preferences.debug_symbolengine(), "[CSymbolEngineRaisers]  firstraiser_chair (empty if no raiser) : %i\n", _firstraiser_chair);
}

int CSymbolEngineRaisers::FirstPossibleRaiser() {
  // For nopponentstruelyraising we can always 
  // start behind the userchair and search clockwise
  // until the last player in front of the user.
  // This works both preflop and postflop,
  // both in and out of the blinds,
  // both before and after our first action. ;-)
	int first_possible_raiser = (USER_CHAIR + 1) % p_tablemap->nchairs();
	return first_possible_raiser;
}

int CSymbolEngineRaisers::LastPossibleRaiser() {
	// For technical reasons (for-loop) we handle the modulo-operation 
	// inside the loop and not here.
	return (FirstPossibleRaiser() + p_tablemap->nchairs() - 1);
}

double CSymbolEngineRaisers::RaisersBet() {
	// The raisers bet is simply the largest bet at the table.
	// So we don't have to know the raisers chair for that.
	double result = 0;
	for (int i=0; i<p_tablemap->nchairs(); i++)	{
		double current_players_bet = p_table_state->Player(i)->_bet.GetValue();
		if (current_players_bet > result && p_table_state->Player(i)->HasAnyCards()) 	{
			result = current_players_bet;
		}
	}
	return result;
}

int CSymbolEngineRaisers::LastRaised(const int round) {
  AssertRange(round, kBetroundPreflop, kBetroundRiver);
  return _lastraised[round];
}

bool CSymbolEngineRaisers::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
  FAST_EXIT_ON_OPENPPL_SYMBOLS(name);
	if (memcmp(name, "nopponents", 10)==0) {
		if (memcmp(name, "nopponentstruelyraising", 23)==0 && strlen(name)==23) {
			*result = nopponentstruelyraising();
		}	else {
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	} else if (memcmp(name, "raischair", 9) == 0 && strlen(name) == 9) {
		*result = raischair();
	} else if (memcmp(name, "firstraiser_chair", 17) == 0) {
		*result = firstraiser_chair();
	}	else if (memcmp(name, "raisbits", 8)==0 && strlen(name)==9) {
		*result = raisbits(name[8]-'0');
	} else if (memcmp(name, "lastraised", 10)==0 && strlen(name)==11) { 
    *result = LastRaised(name[10]-'0');
  }	else {
		// Symbol of a different symbol-engine
		return false;
	}
	// Valid symbol
	return true;
}

CString CSymbolEngineRaisers::SymbolsProvided() {
  CString list = "nopponentstruelyraising raischair firstraiser_chair  ";
  list += RangeOfSymbols("raisbits%i", kBetroundPreflop, kBetroundRiver);
  list += RangeOfSymbols("lastraised%i", kBetroundPreflop, kBetroundRiver);
  return list;
}
