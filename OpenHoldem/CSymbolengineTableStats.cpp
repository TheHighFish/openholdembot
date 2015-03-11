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
#include "CSymbolEngineTableStats.h"

#include "CBetroundCalculator.h"
#include "CHandresetDetector.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CScrapedActionInterface.h"
#include "CStringMatch.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineChipAmounts.h"
#include "MagicNumbers.h"

CSymbolEngineTableStats *p_symbol_engine_table_stats = NULL;

CSymbolEngineTableStats::CSymbolEngineTableStats() {
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
  assert(p_symbol_engine_active_dealt_playing != NULL);
  assert(p_symbol_engine_chip_amounts != NULL);
  ResetOnConnection();
}

CSymbolEngineTableStats::~CSymbolEngineTableStats() {
}

void CSymbolEngineTableStats::InitOnStartup() {
}

void CSymbolEngineTableStats::ResetOnConnection() {
  for (int i=0; i<=kNumberOfHands; ++i) {
    seen_flop[i] = 0;
    seen_turn[i] = 0;
    seen_river[i] = 0;
    bets_preflop[i] = 0;
    raised_preflop[i] = 0;
    dealt_players[i] = 0;
  }
}

void CSymbolEngineTableStats::ResetOnHandreset() {
  ShiftOldestDataAway();
}

void CSymbolEngineTableStats::ResetOnNewRound() {
}

void CSymbolEngineTableStats::ResetOnMyTurn() {
}

void CSymbolEngineTableStats::ResetOnHeartbeat() {
  UpdateData();
}

void CSymbolEngineTableStats::ShiftOldestDataAway() {
  for (int i=0; i<kNumberOfHands; ++i) {
    seen_flop[i]      = seen_flop[i+1];
    seen_turn[i]      = seen_turn[i+1];
    seen_river[i]     = seen_river[i+1];
    bets_preflop[i]   = bets_preflop[i+1];
    raised_preflop[i] = raised_preflop[i+1];
    dealt_players[i]  = dealt_players[i+1];
  }
  seen_flop[kIndexCurrentHand] = 0;
  seen_turn[kIndexCurrentHand] = 0;
  seen_river[kIndexCurrentHand] = 0;
  bets_preflop[kIndexCurrentHand] = 0;
  raised_preflop[kIndexCurrentHand] = 0;
  dealt_players[kIndexCurrentHand] = 0;
}

void CSymbolEngineTableStats::UpdateData() {
  switch (BETROUND) {
    case k_betround_preflop:
      dealt_players[kIndexCurrentHand] = SYM->p_symbol_engine_active_dealt_playing()->nplayersdealt();
      bets_preflop[kIndexCurrentHand] = SYM->p_symbol_engine_chip_amounts()->ncallbets();
      if (SYM->p_symbol_engine_chip_amounts()->ncallbets() > 1) {
        // raised_preflop is a "counter" with state 0/1
        // so that we can use SumUp().
        raised_preflop[kIndexCurrentHand] = 1;
      }
      break;
    case k_betround_flop:
      if (seen_flop[kIndexCurrentHand] == 0) {
        seen_flop[kIndexCurrentHand] = SYM->p_symbol_engine_active_dealt_playing()->nopponentsplaying();
      }
      break;
    case k_betround_turn:
      if (seen_turn[kIndexCurrentHand] == 0) {
        seen_turn[kIndexCurrentHand] = SYM->p_symbol_engine_active_dealt_playing()->nopponentsplaying();
      }
      break;
    case k_betround_river:
      if (seen_river[kIndexCurrentHand] == 0) {
        seen_river[kIndexCurrentHand] = SYM->p_symbol_engine_active_dealt_playing()->nopponentsplaying();
      }
      break;
  }
}

double CSymbolEngineTableStats::SumUp(int *data_set_for_n_hands) {
  // Return-tzpe is double, not int.
  // This waz we easily avoid the unexpected effects of integer-division.
  int sum = 0;
  for (int i=0; i<kNumberOfHands; ++i) {
    sum += data_set_for_n_hands[i];
  }
  return sum;
}

int CSymbolEngineTableStats::NumberOfHandsStored() {
  int hands_played = SYM->p_handreset_detector()->hands_played();
  if (hands_played > kNumberOfHands) {
    // All entries contains valid date
    return kNumberOfHands;
  } else if (hands_played == 0) {
    // No data at all
    // Avoid potential division by zero
    return 1;
  } else {
    // Parts of the arrays contain meaningful data
    // Rest is 0, at the moment unused
    return hands_played;
  }
}

double CSymbolEngineTableStats::FlopTurnRiverPct(int betround) {
  switch (BETROUND) {
    case k_betround_flop:
      return SumUp(seen_flop) / SumUp(dealt_players);
    case k_betround_turn:
      return SumUp(seen_turn) / SumUp(dealt_players);
    case k_betround_river:
      return SumUp(seen_river) / SumUp(dealt_players);
    default: 
      return k_undefined_zero;
  }
}


double CSymbolEngineTableStats::AvgBetsPf() {
	return SumUp(bets_preflop) / NumberOfHandsStored();
}

double CSymbolEngineTableStats::TablePfr() {
	return SumUp(raised_preflop) / NumberOfHandsStored();
}

bool CSymbolEngineTableStats::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
  if (memcmp(name, "floppct", 7)==0 && strlen(name)==7) {  				
    *result = FlopTurnRiverPct(k_betround_flop);
    return true;
  }
  if (memcmp(name, "turnpct", 7)==0 && strlen(name)==7) {  	
    *result = FlopTurnRiverPct(k_betround_turn);
    return true;
  }
  if (memcmp(name, "riverpct", 8)==0 && strlen(name)==8) {  	
    *result = FlopTurnRiverPct(k_betround_river);
    return true;
  }
  if (memcmp(name, "avgbetspf", 9)==0 && strlen(name)==9) {  
    *result = AvgBetsPf();
    return true;
  }
  if (memcmp(name, "tablepfr", 8)==0 && strlen(name)==8) { 	
    *result = TablePfr();	
    return true;
  }
	// Symbol of a different symbol-engine
	return false;
}

CString CSymbolEngineTableStats::SymbolsProvided() {
  return "floppct turnpct riverpct avgbetspf tablepfr ";
}