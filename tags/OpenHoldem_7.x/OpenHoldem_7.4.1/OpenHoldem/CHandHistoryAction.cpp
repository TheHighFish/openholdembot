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
#include "CHandHistoryAction.h"

#include "CPreferences.h"
#include "CScraper.h"
#include "MagicNumbers.h"

CHandHistoryAction *p_handhistory_action = NULL;

CHandHistoryAction::CHandHistoryAction() {
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	//
	// This engine does not use any other engines.
}

CHandHistoryAction::~CHandHistoryAction() {
}

void CHandHistoryAction::InitOnStartup() {
}

void CHandHistoryAction::ResetOnConnection() {
}

void CHandHistoryAction::ResetOnHandreset() {
}

void CHandHistoryAction::ResetOnNewRound() {
}

void CHandHistoryAction::ResetOnMyTurn() {
}

void CHandHistoryAction::ResetOnHeartbeat() {
}

bool CHandHistoryAction::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
  // No symbols provided
	return false;
}

CString CHandHistoryAction::SymbolsProvided() {
  // No symbols provided
  return " ";
}
	