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
#include "CAutoplayerFunctions.h"

#include <assert.h>
#include "CBetroundCalculator.h"
#include "CFunctionCollection.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineOpenPPL.h"
#include "CSymbolEngineTableLimits.h"
#include "CSymbolEngineUserchair.h"
#include "CPreferences.h"
#include "MagicNumbers.h"
#include "StringFunctions.h"

// TODO make it a symbol-engine?
// Call it!

CAutoplayerFunctions *p_autoplayer_functions = NULL;

CAutoplayerFunctions::CAutoplayerFunctions() {
}

double CAutoplayerFunctions::GetAutoplayerFunctionValue(const int function_code) {
	assert(function_code >= 0);
	assert(function_code < k_number_of_standard_functions);
  return p_function_collection->Evaluate(k_standard_function_names[function_code]);
}

void CAutoplayerFunctions::CalcPrimaryFormulas() {
  write_log(preferences.debug_formula(), "[CAutoplayerFunctions] CalcPrimaryFormulas()\n");
  write_log(true, "f$alli = %.3f\n", p_function_collection->EvaluateAutoplayerFunction(k_autoplayer_function_allin));
  p_function_collection->ClearCache();
  write_log(true, "f$alli = %.3f\n", p_function_collection->EvaluateAutoplayerFunction(k_autoplayer_function_allin));
  if (p_function_collection->IsOpenPPLProfile()) {
    CalcPrimaryFormulasOpenPPL();
    write_log(true, "f$alli = %.3f\n", p_function_collection->EvaluateAutoplayerFunction(k_autoplayer_function_allin));
    CalculateOpenPPLBackupActions();
    write_log(true, "f$alli = %.3f\n", p_function_collection->EvaluateAutoplayerFunction(k_autoplayer_function_allin));
    return;
  }
  // Otherwiese: OH-script
  assert(!p_function_collection->IsOpenPPLProfile());
  CalcPrimaryFormulasOHScript();
}

void CAutoplayerFunctions::CalcPrimaryFormulasOHScript() {
  write_log(preferences.debug_formula(), "[CAutoplayerFunctions] CalcPrimaryFormulasOHScript()\n");
  bool trace_needed = preferences.trace_enabled();
  write_log(preferences.debug_formula(), "[CAutoplayerFunctions] Trace enabled: %s\n", Bool2CString(preferences.trace_enabled()));
	for (int i=k_autoplayer_function_beep; i<=k_autoplayer_function_fold; ++i) {
		double result = p_function_collection->Evaluate(k_standard_function_names[i], trace_needed);
		write_log(preferences.debug_formula(), "[CAutoplayerFunctions] Primary formulas; %s: %f\n", 
			k_standard_function_names[i], result);
	}
}

void CAutoplayerFunctions::CalcPrimaryFormulasOpenPPL() {
  write_log(preferences.debug_formula(), "[CAutoplayerFunctions] CalcPrimaryFormulasOpenPPL()\n");
  bool trace_needed = preferences.trace_enabled();
  write_log(preferences.debug_formula(), "[CAutoplayerFunctions] Trace enabled: %s\n", Bool2CString(preferences.trace_enabled()));
  // First do the calculation of memory/history-symbols,
  //   * exactly once per my turn
  //   * when we have stable frames (isfinal-answer == true)
  //   * shortly before the main OpenPPL-evaluations
  // Unfortunatellz doing this in CSymbolEngineOpen::PPLResetOnMyTurn() 
  // did not work as expected.
  assert(p_symbol_engine_autoplayer->isfinalanswer());
  p_symbol_engine_open_ppl->InitMemorySymbols();
  // Now do the main evaluation
  int betround = p_betround_calculator->betround();
	if (betround < k_betround_preflop || betround > k_betround_river) {
    write_log(preferences.debug_formula(), 
      "[CAutoplayerFunctions] Betround out of range. Can't calculate OpenPPL-decision\n");
    return;
  }
  double decision = p_function_collection->Evaluate(k_OpenPPL_function_names[betround], 
    trace_needed);
  write_log(preferences.debug_formula(), 
    "[CAutoplayerFunctions] Decision (non-translated) = %.2f\n", decision);
  TranslateOpenPPLDecisionToAutoplayerFunctions(decision);
}

void CAutoplayerFunctions::CheckIfDecisionMatchesElementaryAction(int decision, int action) {
  CString action_name;
  // Translate aqutoplayer-constant to OPPL-action-name
  switch (action) {
    case k_autoplayer_function_beep:
      action_name = "Beep";
      break;
    case k_autoplayer_function_allin:
      action_name = "RaiseMax";
      break;
    case k_autoplayer_function_betpot_1_1:
      action_name = "RaisePot";
      break;
    case k_autoplayer_function_betpot_1_2:
      action_name = "RaiseHalfPot";
      break;
    case k_autoplayer_function_raise:
      action_name = "Raise";
      break;
    case k_autoplayer_function_call:
      action_name = "Call";
      break;
    case k_autoplayer_function_check:
      action_name = "Check";
      break;
    case k_autoplayer_function_fold:
      action_name = "Fold";
      break;
    default:
      assert (k_this_must_not_happen);
  }
  double open_ppl_action_code = p_function_collection->Evaluate(action_name);
  write_log(preferences.debug_symbolengine_open_ppl(),
    "[CAutoplayerFunctions] Checking if decision %.3f matches action %s (%.3f)\n",
    decision, action_name, open_ppl_action_code);
  if (decision == open_ppl_action_code) {
    write_log(preferences.debug_symbolengine_open_ppl(),
    "[CAutoplayerFunctions] Decision matches action\n");
    p_function_collection->SetAutoplayerFunctionValue(action, true);
  }
}

void CAutoplayerFunctions::TranslateOpenPPLDecisionToAutoplayerFunctions(double decision) {
  write_log(preferences.debug_formula(), "[CAutoplayerFunctions] TranslateOpenPPLDecisionToAutoplayerFunctions()\n");
  // Positive values mean:  betsizes (in big-blinds)
  // Small negative values: percentaged potsized bets
  // Large negative values: action constants
  if (decision > 0) {
    // OpenHoldem uses f$betsize in dollars
    assert(p_symbol_engine_tablelimits != NULL);
    double betsize = decision * p_symbol_engine_tablelimits->bblind();
    p_function_collection->SetAutoplayerFunctionValue(k_autoplayer_function_betsize, 
      betsize);
  } else if (IsPercentagePotsizeBet(decision)) {
    double betsize = BetSizeForPercentagedPotsizeBet(decision);
    p_function_collection->SetAutoplayerFunctionValue(k_autoplayer_function_betsize, 
      betsize);
  } else if (decision < -1000) {
    // Large negative values: action constants
    CheckIfDecisionMatchesElementaryAction(decision, k_autoplayer_function_beep);
    CheckIfDecisionMatchesElementaryAction(decision, k_autoplayer_function_allin);
    CheckIfDecisionMatchesElementaryAction(decision, k_autoplayer_function_betpot_1_1);
    CheckIfDecisionMatchesElementaryAction(decision, k_autoplayer_function_betpot_1_2);
    CheckIfDecisionMatchesElementaryAction(decision, k_autoplayer_function_raise);
    CheckIfDecisionMatchesElementaryAction(decision, k_autoplayer_function_call);
    CheckIfDecisionMatchesElementaryAction(decision, k_autoplayer_function_check);
    CheckIfDecisionMatchesElementaryAction(decision, k_autoplayer_function_fold);
  } else {
    // This can onlz be undefined == 0.0
    assert(decision == k_undefined_zero);
    write_log(preferences.debug_symbolengine_open_ppl(),
      "[CAutoplayerFunctions] OpenPPL-decision undefined. Defaulting to check/fold.\n");
    p_function_collection->SetAutoplayerFunctionValue(k_autoplayer_function_check, true);
    p_function_collection->SetAutoplayerFunctionValue(k_autoplayer_function_fold, true);
  }
}

void CAutoplayerFunctions::CalculateSingleOpenPPLBackupAction(
    int potential_action, int potential_backup) {
  double action_value = p_function_collection->EvaluateAutoplayerFunction(
    potential_action);
  write_log(preferences.debug_formula(), 
    "[CAutoplayerFunctions] %s -> %.3f\n",
    k_standard_function_names[potential_action], 
    action_value);
  if (action_value) {
    p_function_collection->SetAutoplayerFunctionValue(
      potential_backup, true);
  }
}

void CAutoplayerFunctions::CalculateOpenPPLBackupActions() {
  write_log(preferences.debug_formula(), 
    "[CAutoplayerFunctions] CalculateOpenPPLBackupActions()\n");
  // Beep is a stand-alone action
  // No backup, can't be combined with pother actions
  // (contrary to f$beep in OH-script).
  if (p_function_collection->EvaluateAutoplayerFunction(
      k_autoplayer_function_beep)) {
    // Turn check/fold off
    p_function_collection->SetAutoplayerFunctionValue(
      k_autoplayer_function_check, false);
    p_function_collection->SetAutoplayerFunctionValue(
      k_autoplayer_function_fold, false);
    return;
  }
  // Allin -> BetPot
  CalculateSingleOpenPPLBackupAction(
    k_autoplayer_function_allin, k_autoplayer_function_betpot_1_1);
  // BetPot -> 1/2 BetPot
  CalculateSingleOpenPPLBackupAction(
    k_autoplayer_function_betpot_1_1, k_autoplayer_function_betpot_1_2);
  // 1/2 BetPot -> Raise
  CalculateSingleOpenPPLBackupAction(
    k_autoplayer_function_betpot_1_2, k_autoplayer_function_raise);
  // Also: f$betsize -> Raise
  CalculateSingleOpenPPLBackupAction(
    k_autoplayer_function_betsize, k_autoplayer_function_raise);
  // Raise -> Call
  CalculateSingleOpenPPLBackupAction(
    k_autoplayer_function_raise, k_autoplayer_function_call);
  // Call -> Check
  CalculateSingleOpenPPLBackupAction(
    k_autoplayer_function_call, k_autoplayer_function_check);
  // Call -> Fold
  CalculateSingleOpenPPLBackupAction(
    k_autoplayer_function_check, k_autoplayer_function_fold);
}

bool CAutoplayerFunctions::IsPercentagePotsizeBet(double decision) {
  // Small negative values: percentaged potsized bets
  // According to the old library...
  return ((decision < 0) && (decision >= -1000));
}

double CAutoplayerFunctions::BetSizeForPercentagedPotsizeBet(double decision) {
  double percentage_0_100 = -100 * decision;
  write_log(preferences.debug_formula(), 
    "[CAutoplayerFunctions] Calculate f$betsize for %.2f percent potsize\n",
    percentage_0_100);
  assert(p_symbol_engine_chip_amounts != NULL);
  assert(p_symbol_engine_userchair != NULL);
  assert(p_symbol_engine_userchair->userchair_confirmed());
  double betsize = p_symbol_engine_chip_amounts->currentbet(USER_CHAIR) 
    + p_symbol_engine_chip_amounts->call() 
    + (-1 * decision) * (p_symbol_engine_chip_amounts->pot() + p_symbol_engine_chip_amounts->call());
    write_log(preferences.debug_formula(), 
    "[CAutoplayerFunctions] f$betsize is %s\n",
    Number2CString(betsize, 2));
  return betsize;
}

void CAutoplayerFunctions::CalcSecondaryFormulas(void)
{
	for (int i=k_standard_function_prefold; i<=k_standard_function_delay; ++i)
	{
		double result = p_function_collection->Evaluate(k_standard_function_names[i], true);
		write_log(preferences.debug_formula(), "[CAutoplayerFunctions] Secondary formulas; %s: %f\n", 
			k_standard_function_names[i], result);
	}
}

