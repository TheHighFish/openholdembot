//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Managing access to global data,
//   avoiding circular dependency of DLLs to OpenHoldem.
//   Temp solution until we get rid of all global data,
//   once mpdulatization has been improved
//
//******************************************************************************

#define GLOBALS_DLL_EXPORTS

#include "globals.h"
#include "..\Preferences_DLL\Preferences.h"

CPreferences preferences;

GLOBALS_DLL_API void InitGlobals() {
}

GLOBALS_DLL_API CPreferences* Preferences() {
  return &preferences;
}

