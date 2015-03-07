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

double BetPotFactor(int betpot_action_code) {
  assert(betpot_action_code >= k_autoplayer_function_betpot_2_1);
  assert(betpot_action_code <= k_autoplayer_function_betpot_1_4);
  double betpot_factor = 1.0;
  switch (betpot_action_code) {
    case k_autoplayer_function_betpot_2_1:
      betpot_factor = 2.0;
      break;
    case k_autoplayer_function_betpot_1_1:
      betpot_factor = 1.0;
      break;
    case k_autoplayer_function_betpot_3_4:
      betpot_factor = 0.75;
      break;
    case k_autoplayer_function_betpot_2_3:
      betpot_factor = 0.667;
      break;
    case k_autoplayer_function_betpot_1_2:
      betpot_factor = 0.5;
      break;
    case k_autoplayer_function_betpot_1_3:
      betpot_factor = 0.333;
      break;
    case k_autoplayer_function_betpot_1_4:
      betpot_factor = 0.25;
      break;
    default:
      betpot_factor = 1.0;
  }
  return betpot_factor;
}

double BetsizeForBetpot(int betpot_action_code) {
  assert(betpot_action_code >= k_autoplayer_function_betpot_2_1);
  assert(betpot_action_code <= k_autoplayer_function_betpot_1_4);
  double pot_after_i_call = SYM->p_symbol_engine_chip_amounts()->pot()
    + SYM->p_symbol_engine_chip_amounts()->call();
  double additional_money_into_pot = BetPotFactor(betpot_action_code) 
    * pot_after_i_call;
  double final_betsize = SYM->p_symbol_engine_chip_amounts()->currentbet(USER_CHAIR)
    + SYM->p_symbol_engine_chip_amounts()->call()
    + additional_money_into_pot;
  assert(final_betsize > 0);
  return final_betsize;
}