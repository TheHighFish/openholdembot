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
#include "CSymbolEngineUserDLL.h"
#include "..\Reference User DLL\user.h"

CSymbolEngineUserDLL *p_symbol_engine_formula_loading = NULL;

CSymbolEngineUserDLL::CSymbolEngineUserDLL() {
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	//
	// This engine does not use any other engines.
}

CSymbolEngineUserDLL::~CSymbolEngineUserDLL() {
}

void CSymbolEngineUserDLL::InitOnStartup() {
  DLLUpdateOnNewFormula();
}

void CSymbolEngineUserDLL::UpdateOnConnection() {
  DLLUpdateOnConnection();
}

void CSymbolEngineUserDLL::UpdateOnHandreset() {
  DLLUpdateOnHandreset();
}

void CSymbolEngineUserDLL::UpdateOnNewRound() {
  DLLUpdateOnNewRound();
}

void CSymbolEngineUserDLL::UpdateOnMyTurn() {
  DLLUpdateOnMyTurn();
}

void CSymbolEngineUserDLL::UpdateOnHeartbeat() {
  DLLUpdateOnHeartbeat();
}

bool CSymbolEngineUserDLL::EvaluateSymbol(const CString name, double *result, bool log /* = false */) {
  FAST_EXIT_ON_OPENPPL_SYMBOLS(name);
  if (memcmp(name, "dll$", 4) != 0) {
    // Symbol of a different symbol-engine
    return false;
  }
  *result = ProcessQuery(name);
	return true;
}

CString CSymbolEngineUserDLL::SymbolsProvided() {
  return " ";
}
