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
#include "CSymbolEngineMTTInfo.h"

#include "CBetroundCalculator.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CStringMatch.h"
#include "CTableState.h"
#include "MagicNumbers.h"

CSymbolEngineMTTInfo *p_symbol_engine_mtt_info = NULL;

CSymbolEngineMTTInfo::CSymbolEngineMTTInfo() {
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	//
	// This engine does not use any other engines.
}

CSymbolEngineMTTInfo::~CSymbolEngineMTTInfo() {
}

void CSymbolEngineMTTInfo::InitOnStartup() {
	ResetOnConnection();
}

void CSymbolEngineMTTInfo::ResetOnConnection() {
  _mtt_number_entrants = 0;
	_mtt_players_remaining = 0;
	_mtt_my_rank = 0;
	_mtt_paid_places = 0;
	_mtt_largest_stack = 0.0;
	_mtt_average_stack = 0.0;
	_mtt_smallest_stack = 0.0;
}

void CSymbolEngineMTTInfo::ResetOnHandreset() {
}

void CSymbolEngineMTTInfo::ResetOnNewRound() {
}

void CSymbolEngineMTTInfo::ResetOnMyTurn() {
}

void CSymbolEngineMTTInfo::ResetOnHeartbeat() {
}

bool CSymbolEngineMTTInfo::EvaluateSymbol(const char *name, double *result, bool log /* = false */)
{
  FAST_EXIT_ON_OPENPPL_SYMBOLS(name);
  if (memcmp(name, "mtt_", 4) != 0) {
    // Symbol of a different symbol-engine
	  return false;
  }
	// MTT symbols
  if (memcmp(name, "mtt_number_entrants", 19)==0)        *result = _mtt_number_entrants;
  else if (memcmp(name, "mtt_players_remaining", 21)==0) *result = _mtt_players_remaining;
  else if (memcmp(name, "mtt_my_rank", 11)==0)	         *result = _mtt_my_rank;
  else if (memcmp(name, "mtt_paid_places", 15)==0)       *result = _mtt_paid_places;
  else if (memcmp(name, "mtt_largest_stack", 17)==0)	   *result = _mtt_largest_stack;
  else if (memcmp(name, "mtt_average_stack", 17)==0)	   *result = _mtt_average_stack;
  else if (memcmp(name, "mtt_smallest_stack", 18)==0) 	 *result = _mtt_smallest_stack;
  else {
	  // Symbol of a different symbol-engine
	  return false;
  }
  return true;
}

CString CSymbolEngineMTTInfo::SymbolsProvided() {
  return "mtt_number_entrants mtt_players_remaining mtt_paid_places mtt_my_rank "
    "mtt_largest_stack mtt_average_stack mtt_smallest_stack ";
}
