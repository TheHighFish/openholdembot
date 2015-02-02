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
#include "BetpotCalculations.h"

#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineUserchair.h"

double BetsizeForBetpot(double betpot_factor) {
  assert(betpot_factor >= 0.25);
  assert(betpot_factor <= 2.00);
  double pot_after_i_call = p_symbol_engine_chip_amounts->pot()
    + p_symbol_engine_chip_amounts->call();
  double additional_money_into_pot = betpot_factor * pot_after_i_call;
  double final_betsize = p_symbol_engine_chip_amounts->currentbet(USER_CHAIR)
    + p_symbol_engine_chip_amounts->call()
    + additional_money_into_pot;
  assert(final_betsize > 0);
  return final_betsize;
}