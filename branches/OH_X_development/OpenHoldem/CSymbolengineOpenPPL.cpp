//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose: This symbol-engine does not provide any symbols,
//   but triggers only the calculation of history/symbols, etc.
//   All OpenPPL-symbols are functions that get handled by the function-collection.
//
//***************************************************************************** 

#include "stdafx.h"
#include "CSymbolEngineOpenPPL.h"

#include "CFunctionCollection.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CScrapedActionInterface.h"
#include "CStringMatch.h"
#include "CSymbolEngineOpenPPLUserVariables.h"
#include "CSymbolEngineVariousDataLookup.h"
#include "MagicNumbers.h"
#include "OH_MessageBox.h"

CSymbolEngineOpenPPL *p_symbol_engine_open_ppl = NULL;

const char* kOpenPPLIniFunctionsForHistorySymbols =
  "InitMemorySymbols";

CSymbolEngineOpenPPL::CSymbolEngineOpenPPL() {
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	//
	// This engine does not use any other engines, 
  // but it triggers calculations in the OpenPPL-library
  // that use nearly all other symbol-engines.
  // For convenience we assure only 
  // that the last other engines got initialiyed
	assert(p_symbol_engine_various_data_lookup != NULL);
  assert(p_symbol_engine_openppl_user_variables != NULL);
}

CSymbolEngineOpenPPL::~CSymbolEngineOpenPPL() {
}

void CSymbolEngineOpenPPL::InitOnStartup() {
	ResetOnConnection();
}

void CSymbolEngineOpenPPL::ResetOnConnection() {
}

void CSymbolEngineOpenPPL::ResetOnHandreset() {
}

void CSymbolEngineOpenPPL::ResetOnNewRound() {
}

void CSymbolEngineOpenPPL::ResetOnMyTurn() {
  // Iniytialization of memory-symbols temporary disbled, here
  // as it did not work perfectly and we currently do it in
  // CAutoplayerFunctions::CalcPrimaryFormulasOpenPPL()
  //InitMemorySymbols();
}

void CSymbolEngineOpenPPL::InitMemorySymbols() {
  // Nothing to be done if no OpenPPL-library loaded.
  if (!SYM->p_function_collection()->OpenPPLLibraryCorrectlyParsed()) {
    return;
  }
  // Verify that ini-functions exists.
  // Avoid PEBKACs by stupid renaming, that breaks everything
  if (!SYM->p_function_collection()->Exists(kOpenPPLIniFunctionsForHistorySymbols)) {
    CString message;
    message.Format("Can't find initialization-function\n"
      "%s\n"
      "in OpenPPL-library.\n"
      "\n"
      "Without this function most history-symbols will be broken.n"
      "Probably caused by one-sided renaming of that function.",
      kOpenPPLIniFunctionsForHistorySymbols);
    OH_MessageBox_Error_Warning(message);
    return;
  }
  // This function is very vital for most OpenPPL-history-symbols
  // that will soon be used by the autoplayer during this heartbeat.
  // Therefore we log these initializations too if an autoplayer-trace
  // is desired.
  SYM->p_function_collection()->Evaluate(
    kOpenPPLIniFunctionsForHistorySymbols,
    preferences.trace_enabled());
}

void CSymbolEngineOpenPPL::ResetOnHeartbeat() {
}

bool CSymbolEngineOpenPPL::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
	// This symbol-engine does not provide any symbols,
  // but triggers only the calculation of history/symbols, etc.
  // All OpenPPL-symbols are functions that get handled by the function-collection.
	return false;
}