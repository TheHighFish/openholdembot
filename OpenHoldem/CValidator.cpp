//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

//!!!!!
#pragma warning(error:4005)   

#include "StdAfx.h"
#include "CValidator.h"

#include "CEngineContainer.h"
#include "CPreferences.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineIsTournament.h"
#include "CSymbolEngineVersus.h"
#include "..\DLLs\GamestateValidation_DLL\GamestateValidation.h"

CValidator *p_validator = NULL;

CValidator::CValidator() {
  _enabled_manually = false;
}

CValidator::~CValidator() {
}

void CValidator::SetEnabledManually(bool Enabled) {
	_enabled_manually = Enabled;
}

void CValidator::Validate() {
  if (!p_engine_container->symbol_engine_autoplayer()->ismyturn()) {
    // Validate only if it is my turn.
    //   * because then we have stable frames
    //   * because then it matters most
    return;
  }
	if (preferences.validator_enabled()
		  // Manually enabled via toolbar?
		  || (_enabled_manually)) {
	  // Validate.
	  //
	  // Validator-rules are defined in "pseudo-code",
	  // that is easily understandable for non-programmers,
	  // but can be turned into C-code using macro-techniques.
	  //
	  // Due to the large number of rules, 
	  // we just put them in external files
	  // and include them here as is.
	  //
    ValidateGamestate(preferences.validator_use_heuristic_rules(),
      p_engine_container->symbol_engine_istournament()->istournament(),
      p_engine_container->symbol_engine_versus()->VersusBinLoaded());
  }
}
