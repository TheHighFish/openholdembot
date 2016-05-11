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

#ifndef INC_CCasinoHotkey_H
#define INC_CCasinoHotkey_H

#include "MagicNumbers.h"

class CCasinoHotkey {
 public:
	CCasinoHotkey();
	~CCasinoHotkey();
 public:
  bool PressHotkey();
  void Set(CString key);
 private:
  char _key_to_be_pressed;
}; 

#endif // INC_CCasinoHotkey_H