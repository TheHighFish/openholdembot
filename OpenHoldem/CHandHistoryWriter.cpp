//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Part of the modular handhistory-generator.
//   Not really a symbol-engine but it is convenient
//   to make use of the symbol-engine-concepts,
//   Also used for the new summary in the logs.
//
// This module collects hh-data and writes it
//   at appropriate moments to disk, both for
//   real hand-histories and for the summary in the logs.
//
//******************************************************************************

#include "stdafx.h"
#include "CHandHistoryWriter.h"

#include "CHandHistoryDealPhase.h"
#include "CHandHistoryUncontested.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CTableState.h"
#include "MagicNumbers.h"

CHandHistoryWriter *p_handhistory_writer = NULL;

CHandHistoryWriter::CHandHistoryWriter() {
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	//
	// This engine collects data from other HH-modules
  // and therefore should be executes after all the rest.
  assert(p_handhistory_deal_phase != NULL);
  assert(p_handhistory_uncontested != NULL);
  _lines_collected = 0;
}

CHandHistoryWriter::~CHandHistoryWriter() {
}

void CHandHistoryWriter::InitOnStartup() {
}

void CHandHistoryWriter::ResetOnConnection() {
}

void CHandHistoryWriter::ResetOnHandreset() {
  //write_log(true, "######## Test ################\n");
  WriteHistory();
}

void CHandHistoryWriter::ResetOnNewRound() {
}

void CHandHistoryWriter::ResetOnMyTurn() {
  WriteHistory();
}

void CHandHistoryWriter::ResetOnHeartbeat() {
}

void CHandHistoryWriter::AddMessage(CString message) {
  return;
  assert(_lines_collected < kMaxLines);
  _handhistory_data[_lines_collected] = message;
  ++_lines_collected;
}

void CHandHistoryWriter::PostsSmallBlind(int chair) {
}

void CHandHistoryWriter::PostsBigBlind(int chair) {
}

void CHandHistoryWriter::PostsAnte(int chair) {
}

void CHandHistoryWriter::Checks(int chair) {
}

void CHandHistoryWriter::Folds(int chair) {
}

void CHandHistoryWriter::Calls(int chair) {
}

void CHandHistoryWriter::Raises(int chair) {
}

void CHandHistoryWriter::WinsUncontested(int chair) {
}

CString CHandHistoryWriter::PlayerName(int chair) {
  assert(chair >= 0);
  assert(chair <= k_last_chair);
  return p_table_state->_players[chair]._name;
}

// Should be called
// * when a hand is over
// * when it is my turn (summary in the log)
void CHandHistoryWriter::WriteHistory() {
  return;
  write_log_separator(true, "Summary (might be not accurate)");
  for (int i=0; i<_lines_collected; ++i) {
    write_log(true, (char*)(LPCTSTR)_handhistory_data[i]); 
  }
  write_log_separator(true, "");
  _lines_collected = 0;
}

bool CHandHistoryWriter::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
  // No symbols provided
	return false;
}

CString CHandHistoryWriter::SymbolsProvided() {
  // No symbols provided
  return "";
}
	
