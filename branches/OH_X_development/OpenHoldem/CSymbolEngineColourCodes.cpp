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
#include "CSymbolEngineColourCodes.h"

#include "CBetroundCalculator.h"
#include "CPokerTrackerThread.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CScrapedActionInterface.h"
#include "CStringMatch.h"
#include "CSymbolEnginePokerTracker.h"
#include "CTableState.h"
#include "MagicNumbers.h"

CSymbolEngineColourCodes *p_symbol_engine_colourcodes = NULL;

CSymbolEngineColourCodes::CSymbolEngineColourCodes() {
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	assert(p_symbol_engine_pokertracker != NULL);
}

CSymbolEngineColourCodes::~CSymbolEngineColourCodes() {
}

void CSymbolEngineColourCodes::InitOnStartup() {
}

void CSymbolEngineColourCodes::ResetOnConnection() {
}

void CSymbolEngineColourCodes::ResetOnHandreset() {
}

void CSymbolEngineColourCodes::ResetOnNewRound() {
}

void CSymbolEngineColourCodes::ResetOnMyTurn(){
}

void CSymbolEngineColourCodes::ResetOnHeartbeat() {
}

bool CSymbolEngineColourCodes::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
  FAST_EXIT_ON_OPENPPL_SYMBOLS(name);
	if (memcmp(name, "colour", 6) != 0) {
		// Symbol of a different symbol-engine
	  return false;
	}
  if ((memcmp(name, "colourcode", 10) == 0) && (strlen(name) == 11)) {
    // colourcode0..colourcode9
    char c_index = name[10];
    if ((c_index < '0') || (c_index > '9')) return false;
    int index = c_index - '0';
    *result = p_table_state->_players[index]._colourcode;
    return true;
  }
	// Symbol of a different symbol-engine
	return false;
}

CString CSymbolEngineColourCodes::SymbolsProvided() {
  return RangeOfSymbols("colourcode%i", k_first_chair, k_last_chair);
}

int CSymbolEngineColourCodes::ColourCodeToDisplay(const int chair) {
  assert(chair >= 0);
  assert(chair <= k_last_chair);
  // Scraped colour-code 
  // Coloour-code for PT-Icon removed in rev. 4283,
  // as PT4 does no longer support the auto-rate-icon
  return p_table_state->_players[chair]._colourcode;
}