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
#include "SwagAdjustment.h"

#include <assert.h>
#include "BetpotCalculations.h"
#include "CPreferences.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolengineGameType.h"
#include "CSymbolEngineIsTournament.h"
#include "CSymbolEngineUserchair.h"
#include "CSymbolEngineTableLimits.h"
#include "..\..\CTablemap\CTablemap.h"
#include "CTableState.h"
#include "debug.h"
#include "NumericalFunctions.h"

double MinimumBetsizeDueToPreviousRaise() {
	double minimums_swag_amount = (p_symbol_engine_chip_amounts->call() 
		+ p_symbol_engine_chip_amounts->sraiprev());
	// If there are no bets and no raises the min-bet is 1 big-blind
	if (minimums_swag_amount <= 0)	{
		if (p_symbol_engine_tablelimits->bblind() > 0) 	{
			write_log(preferences.debug_betsize_adjustment(), "[SwagAdjustment] MinimumBetsizeDueToPreviousRaise: set to 1 big-blind\n");
			minimums_swag_amount = p_symbol_engine_tablelimits->bblind();
		}	else {
			write_log(preferences.debug_betsize_adjustment(), "[SwagAdjustment] MinimumBetsizeDueToPreviousRaise: SERIOUS TABLEMAP-PROBLEM; no bets; big-blind unknown; setting it to 0.02\n");
			// Setting it to the absolute minimum
			minimums_swag_amount = 0.02;
		}
	}
	write_log(preferences.debug_betsize_adjustment(), "[SwagAdjustment] MinimumBetsizeDueToPreviousRaise: %.2f\n", minimums_swag_amount);
	assert(minimums_swag_amount > 0);
	return minimums_swag_amount;
}

double MaximumPossibleBetsizeBecauseOfBalance() {
	assert(p_symbol_engine_userchair != NULL);
	int userchair = p_symbol_engine_userchair->userchair();
	AssertRange(userchair, k_first_chair, k_last_chair);
	double maximum_betsize = p_symbol_engine_chip_amounts->currentbet(userchair)
		+ p_table_state->User()->_balance;
	if (maximum_betsize <= 0) {
    // http://www.maxinmontreal.com/forums/viewtopic.php?f=110&t=17915#p124550
    write_log(k_always_log_errors, "[SwagAdjustment] Invalid balance and bet. Sum <= 0.\n");
  }
	write_log(preferences.debug_betsize_adjustment(), "[SwagAdjustment] MaximumPossibleBetsizeBecauseOfBalance %.2f\n",
		maximum_betsize);
	return maximum_betsize;
}

double SwagAmountAjustedToCasino(double amount_to_raise_to) {
	int userchair = p_symbol_engine_userchair->userchair();
	assert(amount_to_raise_to >= 0);
	double swag_amount_ajusted_to_casino = amount_to_raise_to;
	// WinHoldems f$srai should return, what we want to add to (call + currentbet)
	// so we have to calculate our adjustment the other way.
	// http://www.winholdem.net/help/help-formula.html
	// http://forum.winholdem.net/wbb/viewtopic.php?t=1849
	if (p_tablemap->swagtextmethod() == 2) {
		// Old adjustment: call, so currentbet is too much
		swag_amount_ajusted_to_casino = amount_to_raise_to - p_symbol_engine_chip_amounts->currentbet(userchair);
	}	else if (p_tablemap->swagtextmethod() == 3)	{
		// Old adjustment: call + currentbet.
		// Everything fine, nothing to do.
	}	else {
		// Default: swagtextmethod == 1
		// Old adjustment: 0, currentbet and call are too much.
		swag_amount_ajusted_to_casino = amount_to_raise_to 
			- p_symbol_engine_chip_amounts->currentbet(userchair)
			- p_symbol_engine_chip_amounts->call();
	}
	write_log(preferences.debug_betsize_adjustment(), "[SwagAdjustment] SwagAmountAjustedToCasino %.2f\n",
		swag_amount_ajusted_to_casino);
	return swag_amount_ajusted_to_casino;
}

bool BetSizeIsAllin(double amount_to_raise_to_in_dollars_and_cents) {
	double maximum_betsize = MaximumPossibleBetsizeBecauseOfBalance();
	write_log(preferences.debug_betsize_adjustment(), "[SwagAdjustment] BetSizeIsAllin() desired betsize: %.2f\n",
		amount_to_raise_to_in_dollars_and_cents);
	write_log(preferences.debug_betsize_adjustment(), "[SwagAdjustment] BetSizeIsAllin() maximum betsize: %.2f\n",
		maximum_betsize);
	return (amount_to_raise_to_in_dollars_and_cents >= maximum_betsize);
}

double RoundedBetsizeForTournaments(double amount_to_raise_to_in_dollars_and_cents) {
	if (!p_symbol_engine_istournament->istournament()) {
		write_log(preferences.debug_betsize_adjustment(), 
			"[SwagAdjustment] Game-type is cash-game. No rounding necessary.\n");
		return amount_to_raise_to_in_dollars_and_cents;
	}
	write_log(preferences.debug_betsize_adjustment(), 
			"[SwagAdjustment] This is a tournament. Rounding to full dollars or to allin\n");
	if (BetSizeIsAllin(amount_to_raise_to_in_dollars_and_cents)) {
		write_log(preferences.debug_betsize_adjustment(), 
			"[SwagAdjustment] Tournament, but no rounding because we will go allin.\n");
		return amount_to_raise_to_in_dollars_and_cents;
	}	else {
		int rounded_amount = int(amount_to_raise_to_in_dollars_and_cents);
		write_log(preferences.debug_betsize_adjustment(), 
			"[SwagAdjustment] Rounded dollars for tournaments: %.2f\n", rounded_amount);
		return rounded_amount;
	}
}

double MaximumBetsizeForGameType() {
	double maximum_betsize = 0;
	if (p_symbol_engine_gametype->ispl())	{
		write_log(preferences.debug_betsize_adjustment(),
			"[SwagAdjustment] Game-type is Pot Limit.\n");
		maximum_betsize = BetsizeForBetpot(k_autoplayer_function_betpot_1_1);
    write_log(preferences.debug_betsize_adjustment(), 
      "[SwagAdjustment] MaximumBetsizeForPotLimit: %.2f\n", maximum_betsize);
	}	else if (p_symbol_engine_gametype->isfl()) {
		write_log(preferences.debug_betsize_adjustment(), 
			"[SwagAdjustment] Game-type is Fixed Limit. No \"swagging\" supported.\n");
		maximum_betsize = 0;
	}	else {
		int userchair = p_symbol_engine_userchair->userchair();
		write_log(preferences.debug_betsize_adjustment(), 
			"[SwagAdjustment] Game-type is No Limit. Betsize restricted only by users balance.\n");
		maximum_betsize = MaximumPossibleBetsizeBecauseOfBalance();
	}
	write_log(preferences.debug_betsize_adjustment(), 
		"[SwagAdjustment] MaximumBetsizeForGameType(): %.2f\n", maximum_betsize);
	assert(maximum_betsize >= 0);
	return maximum_betsize;	
}

double RoundToBeautifulBetsize(const double amount_to_raise_to) {
  assert(amount_to_raise_to >= p_symbol_engine_tablelimits->bblind());
  // Don't round very smal betsizes.
  // There might be aa reason for it in very small pots.
  if (amount_to_raise_to < (2.0 * p_symbol_engine_tablelimits->bblind())) {
    write_log(preferences.debug_betsize_adjustment(),
      "[SwagAdjustment] RoundToBeautifulBetsize: no rounding, because too small betsize\n");
    return amount_to_raise_to;
  }
  // Otherwise: eo0und to beautiful numbers without caring about our stack/
  // This adjustment will happen later.
  double result;
  // sblind / nnlind
  if (amount_to_raise_to < (10 * p_symbol_engine_tablelimits->sblind())) {
    result = Rounding(amount_to_raise_to, p_symbol_engine_tablelimits->sblind());
  } else {
    result = Rounding(amount_to_raise_to, p_symbol_engine_tablelimits->bblind());
  }
 
  // whole numbers (larg mu,bers)
  if (result > 25000) {
    result = Rounding(result, 1000);
  } else if (result > 2500) {
    result = Rounding(result, 100);
  } else if (result > 250) {
    result = Rounding(result, 10);
  } else if (result > 20) {
    result = Rounding(result, 1);
  }  
  // Whole numbers (small),
  // but onlz if we want to bet more than 0.90.
  // Avoid rounding to 0.
  else if ((result > 0.90) && (abs(result - int(result) < 0.10))) {
    result = Rounding(result, 1);
  }
  write_log(preferences.debug_betsize_adjustment(),
    "[SwagAdjustment] RoundToBeautifulBetsize: %.2f rounded to %.2f\n",
    amount_to_raise_to, result);
  return result;
}

double AdjustedBetsize(double amount_to_raise_to) {
	double original_amount_to_raise_to = amount_to_raise_to;
  amount_to_raise_to = RoundToBeautifulBetsize(amount_to_raise_to);
	AdaptValueToMinMaxRange(&amount_to_raise_to, MinimumBetsizeDueToPreviousRaise(), amount_to_raise_to);
	AdaptValueToMinMaxRange(&amount_to_raise_to, amount_to_raise_to, MaximumBetsizeForGameType());
	AdaptValueToMinMaxRange(&amount_to_raise_to, amount_to_raise_to, MaximumPossibleBetsizeBecauseOfBalance());
	AdaptValueToMinMaxRange(&amount_to_raise_to, amount_to_raise_to, RoundedBetsizeForTournaments(amount_to_raise_to));
	AdaptValueToMinMaxRange(&amount_to_raise_to, 0, SwagAmountAjustedToCasino(amount_to_raise_to));
	return amount_to_raise_to; 
}