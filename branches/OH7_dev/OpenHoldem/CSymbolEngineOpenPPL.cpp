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
#include "CSymbolEngineOpenPPL.h"

#include "CBetroundCalculator.h"
#include "CFunctionCollection.h"
#include "CPreferences.h"
#include "CSymbolEngineAutoplayer.h"

CSymbolEngineOpenPPL *p_symbol_engine_openppl = NULL;

CSymbolEngineOpenPPL::CSymbolEngineOpenPPL() {
  // The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
  assert(p_betround_calculator != NULL);
  assert(p_symbol_engine_autoplayer != NULL);
}

CSymbolEngineOpenPPL::~CSymbolEngineOpenPPL() {
}

void CSymbolEngineOpenPPL::InitOnStartup() {
}

void CSymbolEngineOpenPPL::ResetOnConnection() {
  _decision = k_undefined;
}

void CSymbolEngineOpenPPL::ResetOnHandreset() {
  _decision = k_undefined;
}

void CSymbolEngineOpenPPL::ResetOnNewRound() {
}

void CSymbolEngineOpenPPL::ResetOnMyTurn() {
  _decision = EvaluateOpenPPLMainFunctionForCurrentBetround();
}

void CSymbolEngineOpenPPL::ResetOnHeartbeat() {
}

bool CSymbolEngineOpenPPL::IsOpenPPLProfile() {
  for (int i=k_autoplayer_function_beep; i<=k_autoplayer_function_call; ++i) {
    if (p_function_collection->Exists(k_standard_function_names[i])) {
      // Checing all autoplayer-functions, except check/fold
      // The latter ones get (usually) auto-generated and are compatible with OpenPPL.
      write_log(preferences.debug_symbolengine_open_ppl(),
        "[CSymbolEngineOpenPPL] Autoplayer function %s exists. Therefore OH-script, not OpenPPL.\n",
        k_standard_function_names[i]);
      return false;
    }
  }
  // Also: at least f$preflop should exist in an OpenPPL-profile
  if (!p_function_collection->Exists(k_OpenPPL_function_names[k_betround_preflop])) {
    write_log(preferences.debug_symbolengine_open_ppl(),
      "[CSymbolEngineOpenPPL] Missing function f$preflop. Therefore not OpenPPL.\n");
    return false;
  }
  write_log(preferences.debug_symbolengine_open_ppl(),
    "[CSymbolEngineOpenPPL] OpenPPL-profile detected\n");
  return true;
}

double CSymbolEngineOpenPPL::EvaluateOpenPPLMainFunctionForCurrentBetround() {
  int betround = p_betround_calculator->betround();
  if ((betround < k_betround_preflop) || (betround < k_betround_river)) {
    write_log(preferences.debug_symbolengine_open_ppl(),
      "[CSymbolEngineOpenPPL] Invalid betround. Not evaluating OpenPPL\n");
    return k_undefined_zero; // will be treated as check/fold
  }
  double result = p_function_collection->Evaluate(
     k_OpenPPL_function_names[betround], true);
  return result;
}

double CSymbolEngineOpenPPL::Decision() {
  // OpenPPPL-symbols and playing decision only valid when my turn.
  assert(p_symbol_engine_autoplayer->ismyturn());
  return _decision;
}

CString CSymbolEngineOpenPPL::SymbolsProvided() {
  // This symbol-engine does not provide any symbols,
  // it just triggers initializations and calculations
  return "";
}

/*void CSymbolEngineOpenPPL::CalculateBackupAction() {

}*/
