//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Hotkeys as alternative for autoplayer-buttons
//
//******************************************************************************

#ifndef INC_CCASINOHOTKEYS_H
#define INC_CCASINOHOTKEYS_H

#include "MagicNumbers.h"

class CCasinoHotkeys {
 public:
	CCasinoHotkeys();
	~CCasinoHotkeys();
 public:
  bool PressHotkey(const CString name_of_hotkey_or_button);
 private:
  CString HotkeyName(const CString name_of_hotkey_or_button);
  CString HotkeyLookup(const CString name_of_hotkey_or_button);
}; 

#endif // INC_CCASINOHOTKEYS_H