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
#include "CHandHistoryDealPhase.h"

#include "CBetroundCalculator.h"
#include "CEngineContainer.h"
#include "CHandHistoryWriter.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineDealerchair.h"
#include "CSymbolEngineTableLimits.h"
#include "..\CTablemap\CTablemap.h"
#include "CTableState.h"
#include "MagicNumbers.h"

CHandHistoryDealPhase::CHandHistoryDealPhase() {
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	assert(SYM->p_symbol_engine_active_dealt_playing() != NULL);
  assert(SYM->p_symbol_engine_chip_amounts() != NULL);
  assert(SYM->p_symbol_engine_dealerchair() != NULL);
  assert(SYM->p_symbol_engine_tablelimits() != NULL);
  // No dependency to CHandHistoryWriter as this modules
  // does not compute any symbols but collects our data.
}

CHandHistoryDealPhase::~CHandHistoryDealPhase() {
}

void CHandHistoryDealPhase::InitOnStartup() {
}

void CHandHistoryDealPhase::ResetOnConnection() {
}

void CHandHistoryDealPhase::ResetOnHandreset() {
  _job_done = false;
}

void CHandHistoryDealPhase::ResetOnNewRound() {
}

void CHandHistoryDealPhase::ResetOnMyTurn() {
}

void CHandHistoryDealPhase::ResetOnHeartbeat() {
  if (_job_done) return;
  if (BETROUND > k_betround_preflop) {
    // Can only happen when we join a table
    // in the middle of a hand
    _job_done = true;
    return;
  }
  if (SYM->p_symbol_engine_active_dealt_playing()->nopponentsdealt() < 1 ) {
    // Blind-Posting not yet finished
    return;
  }
  // At least 1 player dealt
  // Now we know everybody who posted a blind or ante
  // Searching clockwise for blind posters
  bool smallblind_seen = false;
  bool bigblind_seen   = false;
  int last_chair  = SYM->p_symbol_engine_dealerchair()->dealerchair();
  int first_chair = (last_chair + 1) % p_tablemap->nchairs();
  for (int i=first_chair; i<=last_chair; ++i) {
    double currentbet = SYM->p_symbol_engine_chip_amounts()->currentbet(i);
    if (currentbet <= 0) {
      // Not having to post, not posting or not participating at all
      continue;
    }
    if (smallblind_seen && bigblind_seen) {
      if (currentbet < SYM->p_symbol_engine_tablelimits()->sblind()) {
        SYM->p_handhistory_writer()->PostsAnte(i);
      }
      // We ignore additional people with a bigblind
      // They are maybe additional blind-posters
      // but in general can't be distinguished from callers
      continue;
    }
    if (smallblind_seen) {
      assert(!bigblind_seen);
      // Player must be posting the bigblind
      SYM->p_handhistory_writer()->PostsBigBlind(i);
      bigblind_seen = true;
      continue;
    }
    // Otherwise> not yet blinds seen
    // Usually a smallblind
    // Might be also a bigblind with missing small blind
    assert(currentbet > 0);
    if (currentbet <= SYM->p_symbol_engine_tablelimits()->sblind()) {
      SYM->p_handhistory_writer()->PostsSmallBlind(0);
      smallblind_seen = true;
      continue;
    }
    if ((currentbet > SYM->p_symbol_engine_tablelimits()->sblind()) 
        && (currentbet <= SYM->p_symbol_engine_tablelimits()->sblind())) {
      // Bigblind and missing smallblind
      SYM->p_handhistory_writer()->PostsBigBlind(i);
      smallblind_seen = true;
      bigblind_seen = true;
      continue;
    }
  }
}

bool CHandHistoryDealPhase::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
  // No symbols provided
	return false;
}

CString CHandHistoryDealPhase::SymbolsProvided() {
  // No symbols provided
  return " ";
}
	