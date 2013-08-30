#include "stdafx.h"

#include "SwagAdjustment.h"
#include <assert.h>
#include "CPreferences.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineIsTournament.h"
#include "CSymbolEngineUserchair.h"
#include "CTableLimits.h"
#include "..\..\CTablemap\CTablemap.h"
#include "debug.h"
#include "NumericalFunctions.h"

double MinimumBetsizeDueToPreviousRaise()
{
	double minimums_swag_amount = (p_symbol_engine_chip_amounts->call() 
		+ p_symbol_engine_chip_amounts->sraiprev());
	write_log(preferences.debug_betsize_adjustment(), "[SwagAdjustment] MinimumBetsizeDueToPreviousRaise: %f\n", minimums_swag_amount);
	assert(minimums_swag_amount > 0);
	return minimums_swag_amount;
}

double MaximumPossibleBetsizeBecauseOfBalance()
{
	assert(p_symbol_engine_userchair != NULL);
	int userchair = p_symbol_engine_userchair->userchair();
	AssertRange(userchair, k_first_chair, k_last_chair);
	double maximum_betsize = p_symbol_engine_chip_amounts->currentbet(userchair)
		+ p_symbol_engine_chip_amounts->balance(userchair);
	assert (maximum_betsize > 0);
	write_log(preferences.debug_betsize_adjustment(), "[SwagAdjustment] MaximumPossibleBetsizeBecauseOfBalance %f\n",
		maximum_betsize);
	return maximum_betsize;
}

double MaximumBetsizeForPotLimit()
{
	// ToDo: Preflop
	double maximum_swag_amount_for_pot_limit = 2 * (p_symbol_engine_chip_amounts->pot()
		+ p_symbol_engine_chip_amounts->pot()); // !!???
	write_log(preferences.debug_betsize_adjustment(), "[SwagAdjustment] MaximumBetsizeForPotLimit: %f\n", maximum_swag_amount_for_pot_limit);
	assert(maximum_swag_amount_for_pot_limit >= 0);
	return maximum_swag_amount_for_pot_limit;
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
	write_log(preferences.debug_betsize_adjustment(), "[SwagAdjustment] SwagAmountAjustedToCasino %f\n",
		swag_amount_ajusted_to_casino);
	return swag_amount_ajusted_to_casino;
}

bool BetSizeIsAllin(double amount_to_raise_to_in_dollars_and_cents)
{
	double maximum_betsize = MaximumPossibleBetsizeBecauseOfBalance();
	write_log(preferences.debug_betsize_adjustment(), "[SwagAdjustment] BetSizeIsAllin() desired betsize: %f\n",
		amount_to_raise_to_in_dollars_and_cents);
	write_log(preferences.debug_betsize_adjustment(), "[SwagAdjustment] BetSizeIsAllin() maximum betsize: %f\n",
		maximum_betsize);
	return (maximum_betsize >= amount_to_raise_to_in_dollars_and_cents);
}

double RoundedBetsizeForTournaments(double amount_to_raise_to_in_dollars_and_cents)
{
	if (!p_symbol_engine_istournament->istournament())
	{
		write_log(preferences.debug_betsize_adjustment(), 
			"[SwagAdjustment] Game-type is cash-game. No rounding necessary.\n");
		return amount_to_raise_to_in_dollars_and_cents;
	}
	else if (BetSizeIsAllin(amount_to_raise_to_in_dollars_and_cents))
	{
		write_log(preferences.debug_betsize_adjustment(), 
			"[SwagAdjustment] Tournament, but no rounding because we will go allin.\n");
		return amount_to_raise_to_in_dollars_and_cents;
	}
	else
	{
		int rounded_amount = int(amount_to_raise_to_in_dollars_and_cents);
		write_log(preferences.debug_betsize_adjustment(), 
			"[SwagAdjustment] Rounded dollars for tournaments: %f.2\n", rounded_amount);
		return rounded_amount;
	}
}

double MaximumBetsizeForGameType()
{
	double maximum_betsize = 0;
	if (p_tablelimits->ispl())
	{
		write_log(preferences.debug_betsize_adjustment(),
			"[SwagAdjustment] Game-type is Pot Limit.\n");
		maximum_betsize = MaximumBetsizeForPotLimit();
	}
	else if (p_tablelimits->isfl())
	{
		write_log(preferences.debug_betsize_adjustment(), 
			"[SwagAdjustment] Game-type is Fixed Limit. No \"swagging\" supported.\n");
		maximum_betsize = 0;
	}
	else
	{
		int userchair = p_symbol_engine_userchair->userchair();
		write_log(preferences.debug_betsize_adjustment(), 
			"[SwagAdjustment] Game-type is No Limit. Betsize restricted only by users balance.\n");
		maximum_betsize = MaximumPossibleBetsizeBecauseOfBalance();
	}
	write_log(preferences.debug_betsize_adjustment(), 
		"[SwagAdjustment] MaximumBetsizeForGameType(): %f\n", maximum_betsize);
	assert(maximum_betsize >= 0);
	return maximum_betsize;	
}

double AdjustedBetsize(double amount_to_raise_to)
{
	double original_amount_to_raise_to = amount_to_raise_to;
	AdaptValueToMinMaxRange(&amount_to_raise_to, MinimumBetsizeDueToPreviousRaise(), amount_to_raise_to);
	AdaptValueToMinMaxRange(&amount_to_raise_to, amount_to_raise_to, MaximumBetsizeForGameType());
	AdaptValueToMinMaxRange(&amount_to_raise_to, amount_to_raise_to, MaximumPossibleBetsizeBecauseOfBalance());
	AdaptValueToMinMaxRange(&amount_to_raise_to, amount_to_raise_to, RoundedBetsizeForTournaments(amount_to_raise_to));
	AdaptValueToMinMaxRange(&amount_to_raise_to, 0, SwagAmountAjustedToCasino(amount_to_raise_to));
	AssertRange(amount_to_raise_to, 0, original_amount_to_raise_to);
	return amount_to_raise_to; 
}