#pragma once
//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Validating the game-state, especially scraped data
//
//******************************************************************************

#ifdef VALIDATOR_DLL_EXPORTS
#define VALIDATOR_DLL_API __declspec(dllexport)
#else
#define VALIDATOR_DLL_API __declspec(dllimport)
#endif

VALIDATOR_DLL_API bool ValidateGamestate(
    bool use_heuristic_rules,
  	bool is_tournament,
	  bool versus_bin_loaded);