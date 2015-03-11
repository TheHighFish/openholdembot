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
#include "CSymbolEngineRaisersCallers.h"

#include <assert.h>
#include "CBetroundCalculator.h"
#include "CScraper.h"
#include "CScrapedActionInterface.h"
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

CSymbolEngineRaisersCallers *p_symbol_engine_raisers_callers = NULL;

// Some symbols are only well-defined if it is my turn
#define RETURN_UNDEFINED_VALUE_IF_NOT_MY_TURN { if (!SYM->p_symbol_engine_autoplayer()->ismyturn()) *result = k_undefined; }

CSymbolEngineRaisersCallers::CSymbolEngineRaisersCallers() {
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

CSymbolEngineRaisersCallers::~CSymbolEngineRaisersCallers() {
}

void CSymbolEngineRaisersCallers::InitOnStartup() {
}

void CSymbolEngineRaisersCallers::ResetOnConnection() {
	ResetOnHandreset();
}

void CSymbolEngineRaisersCallers::ResetOnHandreset() {
	// callbits, raisbits, etc.
	for (int i=k_betround_preflop; i<=k_betround_river; i++) {
		_callbits[i] = 0;
		_raisbits[i] = 0;
		_foldbits[i] = 0;
    _lastraised[i] = k_undefined;
	}
	_raischair = k_undefined;
	_nplayerscallshort  = 0;
	_nopponentsbetting  = 0;
	_nopponentstruelyraising = 0;
	_nopponentsfolded   = 0;
	_nopponentscalling  = 0;
	_nopponentschecking = 0;
}

void CSymbolEngineRaisersCallers::ResetOnNewRound() {
}

void CSymbolEngineRaisersCallers::ResetOnMyTurn() {
	CalculateRaisers();
	CalculateCallers();
}

void CSymbolEngineRaisersCallers::ResetOnHeartbeat() {
	CalculateNOpponentsCheckingBettingFolded();
	CalculateFoldBits();
}

double CSymbolEngineRaisersCallers::LastOrbitsLastRaisersBet() {
	// Not yet acted: 0
	if (!SYM->p_symbol_engine_history()->DidAct()) {
    if (p_betround_calculator->betround() == k_betround_preflop) {
      // Preflop:
      // Start with big blind and forget about former blind raisers
      return SYM->p_symbol_engine_tablelimits()->bblind();
    } else {
      // Postflop
		  return 0.0;
    }
	}
	if (p_table_state->User()->HasKnownCards())	{
		// Otherwise: either we are the raiser (highest bet)
		// Or we called the raise (highest bet too)
		return p_table_state->User()->_bet;
	}
	// Otherwise meaningless
	return 0.0;
}

void CSymbolEngineRaisersCallers::CalculateRaisers() {
	_nopponentstruelyraising = 0;
	if (SYM->p_symbol_engine_chip_amounts()->call() <= 0.0) 	{
		// There are no bets and raises.
		// Skip the calculations to keep the raischair of the previous round.
		// http://www.maxinmontreal.com/forums/viewtopic.php?f=156&t=16806
    write_log(preferences.debug_symbolengine(), 
      "[CSymbolEngineRaisersCallers] No bet to call, therefore no raises\n");
		return;
	}
	// Raischair, nopponentstruelyraising, raisbits
	int first_possible_raiser = FirstPossibleRaiser();
	int last_possible_raiser  = LastPossibleRaiser();
	double highest_bet = LastOrbitsLastRaisersBet();
  write_log(preferences.debug_symbolengine(), "[CSymbolEngineRaisersCallers] Searching for raisers from chair %i to %i with a bet higher than %.2f\n",
		first_possible_raiser, last_possible_raiser, highest_bet); 
	for (int i=first_possible_raiser; i<=last_possible_raiser; ++i) {
		int chair = i % p_tablemap->nchairs();
		double current_players_bet = SYM->p_symbol_engine_chip_amounts()->currentbet(chair);
    write_log(preferences.debug_symbolengine(), 
      "[CSymbolEngineRaisersCallers] chair %d bet %.2f\n",
      chair, current_players_bet);
		// Raisers are people
		// * with a higher bet than players before them
		// * who are still playing, not counting people who bet/fold in later orbits
    // * either betting/raising postflop or truely raising preflop
    //   (not counting the infamous "blind raisers")
    if (!p_table_state->_players[chair].HasAnyCards()) {
      write_log(preferences.debug_symbolengine(), 
        "[CSymbolEngineRaisersCallers] chair %d has no cards.\n", chair);
      continue;
    } else if (current_players_bet <= highest_bet) {
      write_log(preferences.debug_symbolengine(), 
        "[CSymbolEngineRaisersCallers] chair %d is not raising\n", chair);
      continue;
    } else if ((p_betround_calculator->betround() == k_betround_preflop)
				&& (current_players_bet <= SYM->p_symbol_engine_tablelimits()->bblind())) {
      write_log(preferences.debug_symbolengine(), 
        "[CSymbolEngineRaisersCallers] chair %d so-called \"blind raiser\". To be ignored.\n", chair);
      continue;
    }
		highest_bet = current_players_bet;
    write_log(preferences.debug_symbolengine(), "[CSymbolEngineRaisersCallers] Opponent %i raising to %s\n",
			chair, Number2CString(highest_bet));
		_raischair = chair;
		int new_raisbits = _raisbits[BETROUND] | k_exponents[chair];
		_raisbits[BETROUND] = new_raisbits;
		assert(chair != USER_CHAIR);
		++_nopponentstruelyraising;
	}
	AssertRange(_raischair, k_undefined, k_last_chair);
  _lastraised[BETROUND] = _raischair;
	write_log(preferences.debug_symbolengine(), "[CSymbolEngineRaisersCallers] nopponentstruelyraising: %i\n", _nopponentstruelyraising);
	write_log(preferences.debug_symbolengine(), "[CSymbolEngineRaisersCallers] raischair: %i\n", _raischair);
}

void CSymbolEngineRaisersCallers::CalculateCallers() {
	// nopponentscalling
	//
	// nopponentscalling is "difficult" to calculate 
  // and has to work only when it is our turn.
  // Then we can simply start searching after the userchair
	// and do a circular search for callers.
  _nopponentscalling = 0;
  int first_bettor = SYM->p_symbol_engine_userchair()->userchair();
	double current_bet = p_table_state->_players[first_bettor]._bet;
	for (int i=first_bettor+1; 
		  i<=first_bettor+p_tablemap->nchairs()-1; 
		  ++i) {
		int chair = i%p_tablemap->nchairs();
    	// Exact match required. Players being allin don't count as callers.
		if ((p_table_state->_players[chair]._bet == current_bet) 
        && (current_bet > 0)) {
			int new_callbits = _callbits[BETROUND] | k_exponents[chair];
			_callbits[BETROUND] = new_callbits;
			_nopponentscalling++;
		}
		else if (p_table_state->_players[chair]._bet > current_bet) {
			current_bet = p_table_state->_players[chair]._bet;
		}
	}
	AssertRange(_callbits[BETROUND], 0, k_bits_all_ten_players_1_111_111_111);
	AssertRange(_nopponentscalling,   0, k_max_number_of_players);
}

int CSymbolEngineRaisersCallers::FirstPossibleRaiser() {
  // For nopponentstruelyraising we can always 
  // start behind the userchair and search clockwise
  // until the last player in front of the user.
  // This works both preflop and postflop,
  // both in and out of the blinds,
  // both before and after our first action. ;-)
	int first_possible_raiser = (USER_CHAIR + 1) % p_tablemap->nchairs();
	return first_possible_raiser;
}

int CSymbolEngineRaisersCallers::LastPossibleRaiser() {
	// For technical reasons (for-loop) we handle the modulo-operation 
	// inside the loop and not here.
	return (FirstPossibleRaiser() + p_tablemap->nchairs() - 1);
}

void CSymbolEngineRaisersCallers::CalculateNOpponentsCheckingBettingFolded()
{
	_nplayerscallshort  = 0;
	_nopponentsbetting  = 0;
	_nopponentsfolded   = 0;
	_nopponentschecking = 0;

  assert(p_tablemap->nchairs() <= k_max_number_of_players);
	for (int i=0; i<p_tablemap->nchairs(); i++)
	{
		double current_players_bet = SYM->p_symbol_engine_chip_amounts()->currentbet(i);
		if (current_players_bet < RaisersBet()
      && p_table_state->_players[i].HasAnyCards())
		{
			_nplayerscallshort++;
		}
		if (i == USER_CHAIR)
		{
			// No opponent;
			// Nothing more to do
			continue;
		}
		if (current_players_bet > 0) 
		{
			_nopponentsbetting++;
		}
		// Players might have been betting, but folded, so no else for the if
		if ((SYM->p_symbol_engine_active_dealt_playing()->playersdealtbits() & (1<<(i)))
        && !p_table_state->_players[i].HasAnyCards())	{
			_nopponentsfolded++;					
		}
		if (p_table_state->_players[i].HasAnyCards() 
			  && current_players_bet == 0) {
			_nopponentschecking++;
		}
	}
	AssertRange(_nplayerscallshort,  0, k_max_number_of_opponents_at_full_ring_table);
	// Using k_max_number_of_players instead of k_max_number_of_opponents_at_full_ring_table below,
	// as it might be that the user is not seated or user-chair not recognized
	AssertRange(_nopponentsbetting,  0, k_max_number_of_players);
	AssertRange(_nopponentsfolded,   0, k_max_number_of_players);
	AssertRange(_nopponentschecking, 0, k_max_number_of_players);
}

double CSymbolEngineRaisersCallers::RaisersBet()
{
	// The raisers bet is simply the largest bet at the table.
	// So we don't have to know the raisers chair for that.
	double result = 0;
	for (int i=0; i<p_tablemap->nchairs(); i++)
	{
		double current_players_bet = SYM->p_symbol_engine_chip_amounts()->currentbet(i);
		if (current_players_bet > result
      && p_table_state->_players[i].HasAnyCards())
		{
			result = current_players_bet;
		}
	}
	return result;
}

void CSymbolEngineRaisersCallers::CalculateFoldBits()
{
	// foldbits (very late, as they depend on the dealt symbols)
	int new_foldbits = 0;
	for (int i=0; i<p_tablemap->nchairs(); i++)
	{
		if (!p_table_state->_players[i].HasAnyCards()) {
			new_foldbits |= k_exponents[i];
		}
	}
	// remove players, who didn't get dealt.
	new_foldbits &= SYM->p_symbol_engine_active_dealt_playing()->playersdealtbits();

	// remove players, who folded in earlier betting-rounds.
	if (BETROUND >= k_betround_flop)
	{
		new_foldbits &= (~_foldbits[k_betround_preflop]);
	}
	if (BETROUND >= k_betround_turn)
	{
		new_foldbits &= (~_foldbits[k_betround_flop]);
	}
	if (BETROUND >= k_betround_river)   
	{
		new_foldbits &= (~_foldbits[k_betround_turn]);
	}
	_foldbits[BETROUND] = new_foldbits;
}

int CSymbolEngineRaisersCallers::LastRaised(const int round) {
  AssertRange(round, k_betround_preflop, k_betround_river);
  return _lastraised[round];
}

bool CSymbolEngineRaisersCallers::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
  FAST_EXIT_ON_OPENPPL_SYMBOLS(name);
	if (memcmp(name, "nopponents", 10)==0) {
		if (memcmp(name, "nopponentschecking", 18)==0 && strlen(name)==18) {
			*result = nopponentschecking();
		}	else if (memcmp(name, "nopponentscalling", 17)==0 && strlen(name)==17) {
      RETURN_UNDEFINED_VALUE_IF_NOT_MY_TURN
			*result = nopponentscalling();
		}	else if (memcmp(name, "nopponentstruelyraising", 23)==0 && strlen(name)==23) {
      RETURN_UNDEFINED_VALUE_IF_NOT_MY_TURN
			*result = nopponentstruelyraising();
		}	else if (memcmp(name, "nopponentsbetting", 17)==0 && strlen(name)==17) {
			*result = nopponentsbetting();
		}	else if (memcmp(name, "nopponentsfolded", 16)==0 && strlen(name)==16)	{
			*result = nopponentsfolded();
		}	else {
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}
	if (memcmp(name, "nplayerscallshort", 17)==0 && strlen(name)==17)	{
    RETURN_UNDEFINED_VALUE_IF_NOT_MY_TURN
		*result = nplayerscallshort();
	}	else if (memcmp(name, "raischair", 9)==0 && strlen(name)==9) {
		*result = raischair();
	}	else if (memcmp(name, "raisbits", 8)==0 && strlen(name)==9) {
    RETURN_UNDEFINED_VALUE_IF_NOT_MY_TURN
		*result = raisbits(name[8]-'0');
	}	else if (memcmp(name, "callbits", 8)==0 && strlen(name)==9) {
    RETURN_UNDEFINED_VALUE_IF_NOT_MY_TURN
		*result = callbits(name[8]-'0');
	}	else if (memcmp(name, "foldbits", 8)==0 && strlen(name)==9) {
    RETURN_UNDEFINED_VALUE_IF_NOT_MY_TURN
		*result = foldbits(name[8]-'0');
	} else if (memcmp(name, "lastraised", 10)==0 && strlen(name)==11) { 
    *result = LastRaised(name[10]-'0');
  }	else {
		// Symbol of a different symbol-engine
		return false;
	}
	// Valid symbol
	return true;
}

CString CSymbolEngineRaisersCallers::SymbolsProvided() {
  CString list = "nopponentschecking nopponentscalling "
    "nopponentstruelyraising nopponentsbetting nopponentsfolded "
    "nplayerscallshort raischair ";
  list += RangeOfSymbols("raisbits%i", k_betround_preflop, k_betround_river);
  list += RangeOfSymbols("callbits%i", k_betround_preflop, k_betround_river);
  list += RangeOfSymbols("foldbits%i", k_betround_preflop, k_betround_river);
  list += RangeOfSymbols("lastraised%i", k_betround_preflop, k_betround_river);
  return list;
}