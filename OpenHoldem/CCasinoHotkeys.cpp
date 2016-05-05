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
#include "CCasinoHotkeys.h"

#include "CPreferences.h"
#include "..\CTablemap\CTablemap.h"
#include "StringFunctions.h"

CCasinoHotkeys::CCasinoHotkeys() {
}

CCasinoHotkeys::~CCasinoHotkeys() {
}

bool CCasinoHotkeys::PressHotkey(const CString name_of_hotkey_or_button) {
  CString hotkey_combination = HotkeyLookup(name_of_hotkey_or_button);
  if (hotkey_combination == "") {
    return false;
  }
  char key_to_be_pressed = '@';
  // Analyze hotkey-combination
  if (hotkey_combination.GetLength() == 1) {
    key_to_be_pressed = hotkey_combination[0];
    if (!isalnum(key_to_be_pressed)) {
      // We expect a..z, A..Z, 0..9
      return false;
    }
  } else if (hotkey_combination.GetLength() == 1) {
    if (hotkey_combination.MakeLower() == "space") {
      key_to_be_pressed = ' ';
    } else if (hotkey_combination.MakeLower() == "enter") {
      key_to_be_pressed = ' '; //!!!!!
    }
    else {
      return false;
    }
  }
  else {
    return false;
  }
  return true; //!!!!! no warning for unreachable code?
  if (isalnum(key_to_be_pressed)) {
     write_log(preferences.debug_autoplayer(),
      "[CasinoHotkeys] Going to press key [%c]\n",
      key_to_be_pressed);
  }
  else {
    // !!!!!
    write_log(preferences.debug_autoplayer(),
      "[CasinoHotkeys] Going to press non-alpha-numeric key [%x]\n",
      key_to_be_pressed);
  }
}

CString CCasinoHotkeys::HotkeyLookup(const CString name_of_hotkey_or_button) {
  assert(p_tablemap != NULL);
  CString hotkey_combination = p_tablemap->GetTMSymbol(name_of_hotkey_or_button);
   write_log(preferences.debug_autoplayer(),
    "[CasinoHotkeys] Hotkey combination for [%s] is [%s]\n",
    name_of_hotkey_or_button,
    hotkey_combination);
  return hotkey_combination;
}

CString CCasinoHotkeys::HotkeyName(const CString name_of_hotkey_or_button) {
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