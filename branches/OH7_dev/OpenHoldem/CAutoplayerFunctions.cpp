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
#include "CAutoplayerFunctions.h"

#include <assert.h>
#include "CBetroundCalculator.h"
#include "CFunctionCollection.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineTableLimits.h"
#include "CSymbolEngineUserchair.h"
#include "CPreferences.h"
#include "MagicNumbers.h"
#include "StringFunctions.h"

// TODO make it a symbol-engine?
// Call it!

CAutoplayerFunctions *p_autoplayer_functions = NULL;


CAutoplayerFunctions::CAutoplayerFunctions()
{
	Reset();
}

void CAutoplayerFunctions::Reset() {
	for (int i=0; i<k_number_of_standard_functions; i++) {
		SetAutoplayerFunction(i, 0);
	}
}

void CAutoplayerFunctions::SetAutoplayerFunction(const int function_to_bn_set, const double new_value)
{
	assert(function_to_bn_set >= 0);
	assert(function_to_bn_set < k_number_of_standard_functions);
	_autoplayer_functionvalues[function_to_bn_set] = new_value;
};

double CAutoplayerFunctions::GetAutoplayerFunctionValue(const int function_to_bn_queried)
{
	assert(function_to_bn_queried >= 0);
	assert(function_to_bn_queried < k_number_of_standard_functions);
	return _autoplayer_functionvalues[function_to_bn_queried];
}

void CAutoplayerFunctions::CalcPrimaryFormulas() {	
  if (p_function_collection->IsOpenPPLProfile()) {
    CalcPrimaryFormulasOpenPPL();
  } else {
    CalcPrimaryFormulasOHScript();
  }
}

void CAutoplayerFunctions::CalcPrimaryFormulasOHScript() {
  bool trace_needed = preferences.trace_enabled();
  write_log(preferences.debug_symbolengine(), "[CAutoplayerFunctions] Trace enabled: %s\n", Bool2CString(preferences.trace_enabled()));
	for (int i=k_autoplayer_function_beep; i<=k_autoplayer_function_fold; i++) {
		SetAutoplayerFunction(i, // function to be set
			p_function_collection->Evaluate(k_standard_function_names[i], trace_needed));
		write_log(preferences.debug_symbolengine(), "[CAutoplayerFunctions] Primary formulas; %s: %f\n", 
			k_standard_function_names[i], GetAutoplayerFunctionValue(i));
	}
}

void CAutoplayerFunctions::CalcPrimaryFormulasOpenPPL() {
  bool trace_needed = preferences.trace_enabled();
  write_log(preferences.debug_symbolengine(), "[CAutoplayerFunctions] Trace enabled: %s\n", Bool2CString(preferences.trace_enabled()));
  int betround = p_betround_calculator->betround();
	if (betround < k_betround_preflop || betround > k_betround_river) {
    write_log(preferences.debug_symbolengine(), 
      "[CAutoplayerFunctions] Betround out of range. Can't calculate OpenPPL-decision\n");
    return;
  }
  double decision = p_function_collection->Evaluate(k_OpenPPL_function_names[betround], trace_needed);
  write_log(preferences.debug_symbolengine(), 
    "[CAutoplayerFunctions] Decision (non-translated) = %.2f\n", decision);
  TranslateOpenPPLDecisionToAutoplayerFunctions(decision);
}

void CAutoplayerFunctions::TranslateOpenPPLDecisionToAutoplayerFunctions(double decision) {
  Reset();
  // Positive values mean:  betsizes (in big-blinds)
  // Small negative values: percentaged potsized bets
  // Large negative values: action constants
  if (decision > 0) {
    // OpenHoldem uses f$betsize in dollars
    assert(p_symbol_engine_tablelimits != NULL);
    double betsize = decision * p_symbol_engine_tablelimits->bblind();
    SetAutoplayerFunction(k_autoplayer_function_betsize, betsize);
  } else if (IsPercentagePotsizeBet(decision)) {
    double betsize = BetSizeForPercentagedPotsizeBet(decision);
    SetAutoplayerFunction(k_autoplayer_function_betsize, betsize);
  } else {
    // Large negative values: action constants
    assert(decision < -1000);
    //!!!!!
  }
  
  //!!!CalculateOpenPPLBackupActions();
  /*// Also be prepared to raise/call if "swagging" is not possible
    SetAutoplayerFunction(k_autoplayer_function_raise, true);
    SetAutoplayerFunction(k_autoplayer_function_call,  true);
  // Always be prepared to check/fold, except the decision was Beep*/
}

/*void CAutoplayerFunctions::CalculateOpenPPLBackupActions() {

}*/

bool CAutoplayerFunctions::IsPercentagePotsizeBet(double decision) {
  // Small negative values: percentaged potsized bets
  // According to the old library...
  return ((decision <= 0) && (decision >= -1000));
}

double CAutoplayerFunctions::BetSizeForPercentagedPotsizeBet(double decision) {
  double percentage_0_100 = -100 * decision;
  write_log(preferences.debug_symbolengine(), 
    "[CAutoplayerFunctions] Calculate f$betsize for %.2f percent potsize\n",
    percentage_0_100);
  assert(p_symbol_engine_chip_amounts != NULL);
  assert(p_symbol_engine_userchair != NULL);
  assert(p_symbol_engine_userchair->userchair_confirmed());
  double betsize = p_symbol_engine_chip_amounts->currentbet(USER_CHAIR) 
    + p_symbol_engine_chip_amounts->call() 
    + (-1 * decision) * (p_symbol_engine_chip_amounts->pot() + p_symbol_engine_chip_amounts->call());
    write_log(preferences.debug_symbolengine(), 
    "[CAutoplayerFunctions] f$betsize is %s\n",
    Number2CString(betsize, 2));
  return betsize;
}

void CAutoplayerFunctions::CalcSecondaryFormulas(void)
{
	for (int i=k_standard_function_prefold; i<=k_standard_function_delay; i++)
	{
		SetAutoplayerFunction(i, // function to be set
			p_function_collection->Evaluate(k_standard_function_names[i], true));
		write_log(preferences.debug_symbolengine(), "[CAutoplayerFunctions] Secondary formulas; %s: %f\n", 
			k_standard_function_names[i], GetAutoplayerFunctionValue(i));
	}
}

