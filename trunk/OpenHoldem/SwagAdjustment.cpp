#include "stdafx.h"

#include "SwagAdjustment.h"
#include <assert.h>
#include "CPreferences.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineUserchair.h"
#include "CTableLimits.h"
#include "..\..\CTablemap\CTablemap.h"
#include "debug.h"

double MinimumSwagAmount()
{
	double minimums_swag_amount = (p_symbol_engine_chip_amounts->call() 
		+ p_symbol_engine_chip_amounts->sraiprev());
	write_log(prefs.debug_betsize_adjustment(), "SwagAdjustment: MinimumSwagAmount: %f\n", minimums_swag_amount);
	assert(minimums_swag_amount > 0);
	return minimums_swag_amount;
}

double MaximumSwagAmountForPotLimit()
{
	// ToDo: Preflop
	double maximum_swag_amount_for_pot_limit = 2 * (p_symbol_engine_chip_amounts->pot()
		+ p_symbol_engine_chip_amounts->pot()); // !!???
	write_log(prefs.debug_betsize_adjustment(), "SwagAdjustment: MaximumSwagAmountForPotLimit: %f\n", maximum_swag_amount_for_pot_limit);
	assert(maximum_swag_amount_for_pot_limit >= 0);
	return maximum_swag_amount_for_pot_limit;
}

double MaximumSwagAmount()
{
	double maximum_swag_amount = 0;
	if (p_tablelimits->ispl())
	{
		maximum_swag_amount = MaximumSwagAmountForPotLimit();
	}
	else
	{
		int userchair = p_symbol_engine_userchair->userchair();
		maximum_swag_amount = (p_symbol_engine_chip_amounts->currentbet(userchair)
			+ p_symbol_engine_chip_amounts->balance(userchair));
	}
	write_log(prefs.debug_betsize_adjustment(), "SwagAdjustment: MaximumSwagAmount: %f\n", maximum_swag_amount);
	assert(maximum_swag_amount >= 0);
	return maximum_swag_amount;
}

double SwagAmountAjustedToMinimumAndMaximum(double amount_to_raise_to)
{
	assert(amount_to_raise_to >= 0);
	double adjusted_swag_amount = amount_to_raise_to;
	if (adjusted_swag_amount < MinimumSwagAmount())
	{
		adjusted_swag_amount = MinimumSwagAmount();
	}
	if (adjusted_swag_amount > MaximumSwagAmount())
	{
		adjusted_swag_amount = MaximumSwagAmount();
	}
	write_log(prefs.debug_betsize_adjustment(), "SwagAdjustment: SwagAmountAjustedToMinimumAndMaximum: %f\n",
		adjusted_swag_amount);
	return adjusted_swag_amount;
}

double SwagAmountAjustedToCasino(double amount_to_raise_to)
{
	int userchair = p_symbol_engine_userchair->userchair();
	assert(amount_to_raise_to >= 0);
	double swag_amount_ajusted_to_casino = amount_to_raise_to;
	// WinHoldems f$srai should return, what we want to add to (call + currentbet)
	// so we have to calculate our adjustment the other way.
	// http://www.winholdem.net/help/help-formula.html
	// http://forum.winholdem.net/wbb/viewtopic.php?t=1849
	if (p_tablemap->swagtextmethod() == 2)
	{
		// Old adjustment: call, so currentbet is too much
		swag_amount_ajusted_to_casino = amount_to_raise_to - p_symbol_engine_chip_amounts->currentbet(userchair);
	}
	else if (p_tablemap->swagtextmethod() == 3)
	{
		// Old adjustment: call + currentbet.
		// Everything fine, nothing to do.
	}
	else 
	{
		// Default: swagtextmethod == 1
		// Old adjustment: 0, currentbet and call are too much.
		swag_amount_ajusted_to_casino = amount_to_raise_to 
			- p_symbol_engine_chip_amounts->currentbet(userchair)
			- p_symbol_engine_chip_amounts->call();
	}
	write_log(prefs.debug_betsize_adjustment(), "SwagAdjustment: SwagAmountAjustedToCasino %f\n",
		swag_amount_ajusted_to_casino);
	return swag_amount_ajusted_to_casino;
}

double SwagAmountAdjusted(double amount_to_raise_to)
{
	assert(amount_to_raise_to >= 0);
	double swag_amount_adjusted = SwagAmountAjustedToMinimumAndMaximum(amount_to_raise_to);
	swag_amount_adjusted = SwagAmountAjustedToCasino(swag_amount_adjusted);
	write_log(prefs.debug_betsize_adjustment(), "SwagAdjustment: SwagAmountAdjusted (finally): %f\n",
		swag_amount_adjusted);
	return swag_amount_adjusted;
}