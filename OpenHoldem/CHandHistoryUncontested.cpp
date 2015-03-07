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
#include "CHandHistoryUncontested.h"

#include "CHandHistoryWriter.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CTableState.h"
#include "MagicNumbers.h"
#include "NumericalFunctions.h"

CHandHistoryUncontested::CHandHistoryUncontested() {
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
  assert(p_symbol_engine_active_dealt_playing != NULL);
	_job_done = false;
}

CHandHistoryUncontested::~CHandHistoryUncontested() {
}

void CHandHistoryUncontested::InitOnStartup() {
}

void CHandHistoryUncontested::ResetOnConnection() {
}

void CHandHistoryUncontested::ResetOnHandreset() {
  _job_done = false;
}

void CHandHistoryUncontested::ResetOnNewRound() {
}

void CHandHistoryUncontested::ResetOnMyTurn() {
}

void CHandHistoryUncontested::ResetOnHeartbeat() {
  if (_job_done) return;
  if (SYM->p_symbol_engine_active_dealt_playing()->nplayersdealt() < 2) return;
  if (SYM->p_symbol_engine_active_dealt_playing()->nplayersactive() != 1) return;
  // So here we are:
  // * a game is going on
  // * only 1 player is left, who wins uncontested
  for (int i=0; i<p_tablemap->nchairs(); ++i) {
    if (IsBitSet(SYM->p_symbol_engine_active_dealt_playing()->playersactivebits(), i)) {
      CString message;
      message.Format("Player %s wins the pot uncontested\n",
        p_table_state->_players[i]._name);
      SYM->p_handhistory_writer()->AddMessage(message);
      _job_done = true;
      return;
    }
  }
}

bool CHandHistoryUncontested::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
  // No symbols provided
	return false;
}

CString CHandHistoryUncontested::SymbolsProvided() {
  // No symbols provided
  return " ";
}
	