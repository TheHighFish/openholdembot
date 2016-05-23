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

#include "stdafx.h"
#include "CCasinoHotkey.h"

#include "CCasinoInterface.h"
#include "CPreferences.h"
#include "..\CTablemap\CTablemap.h"
#include "StringFunctions.h"

const char kHotkeyUndefined = '\0';

CCasinoHotkey::CCasinoHotkey() {
  _key_to_be_pressed = kHotkeyUndefined;
}

CCasinoHotkey::~CCasinoHotkey() {
}

bool CCasinoHotkey::PressHotkey() {
  if (_key_to_be_pressed == kHotkeyUndefined) {
    return false;
  }
  if (isalnum(_key_to_be_pressed)) {
     write_log(preferences.debug_autoplayer(),
      "[CasinoHotkeys] Going to press key [%c]\n",
      _key_to_be_pressed);
  } else {
    write_log(preferences.debug_autoplayer(),
      "[CasinoHotkeys] Going to press non-alpha-numeric key [%x]\n",
      _key_to_be_pressed);
  }
  p_casino_interface->SendKey(_key_to_be_pressed);
  return true;
}

void CCasinoHotkey::Set(CString key) {
  _key_to_be_pressed = kHotkeyUndefined;
  if (key == "") {
    return;
  }
  key.MakeLower();
  // Analyze hotkey-combination
  if (key.GetLength() == 1) {
    _key_to_be_pressed = key[0];
    if (!isalnum(_key_to_be_pressed)) {
      // We expect a..z, A..Z, 0..9
      _key_to_be_pressed = kHotkeyUndefined;
    }
    return;
  } else if (key.GetLength() == 1) {
    if (key.MakeLower() == "space") {
      _key_to_be_pressed = ' ';
    } else if (key == "enter") {
      // Enter = carriage return = 0x0D
      // https://en.wikipedia.org/wiki/ASCII
      _key_to_be_pressed = char(0x0d);
    } else if (key == "return") {
      // Same as "enter"
      _key_to_be_pressed = char(0x0d);
    } else if (key == "right") {
      _key_to_be_pressed = VK_RIGHT;
    } else if (key == "left") {
      _key_to_be_pressed = VK_LEFT;
    } else if (key == "up") {
      _key_to_be_pressed = VK_UP;
    } else if (key == "down") {
      _key_to_be_pressed = VK_DOWN;
    }
  }
}

/*CString CCasinoHotkey::HotkeyName(const CString name_of_hotkey_or_button) {
  CString result = name_of_hotkey_or_button;
  if (result.Left(2) == "r$") {
    result = CStringRemoveLeft(result, 2);
  }
  if (result.Right(6) == "button") {
    result = CStringRemoveRight(result, 6);
  }
  if (result.Right(6) != "hotkey") {
    result += "hotkey";
  }
   write_log(preferences.debug_autoplayer(), 
    "[CasinoHotkeys] Hotkey for [%s] is [%s]\n",
    name_of_hotkey_or_button,
    result);
  return result;
}
*/
