//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Clean interface for scraper, casino-interface amd autoplayer
//
//******************************************************************************

#include "stdafx.h"
#include "CAutoplayerButton.h"

#include "CCasinoInterface.h"
#include "CPreferences.h"
#include "CStringMatch.h"
#include "MagicNumbers.h"

CAutoplayerButton::CAutoplayerButton() {
  _label = "";
  SetClickable(false);
  _region.bottom = kUndefined;
  _region.left   = kUndefined;
  _region.right  = kUndefined;
  _region.top    = kUndefined;
}

CAutoplayerButton::~CAutoplayerButton() {
}

bool CAutoplayerButton::Click() {
  if (_clickable) {
    /*!!!!!
    // Try to send a hotkey first, if specified in tablemap
    if (casino_hotkeys.PressHotkey(k_standard_function_names[autoplayer_function_code])) {
      write_log(preferences.debug_autoplayer(), "[CasinoInterface] Pressed hotkey for button button %s\n", k_standard_function_names[autoplayer_function_code]);
      return true;
    }
    */
    // Otherwise: click the button the normal way
    p_casino_interface->ClickRect(_region);
    //!!!!!write_log(preferences.debug_autoplayer(), "[CasinoInterface] Clicked button %s\n", k_standard_function_names[autoplayer_function_code]);
    return true;
  } else {
     //!!!!!write_log(preferences.debug_autoplayer(), "[CasinoInterface] Could not click button %s. Either undefined or not visible.\n", k_standard_function_names[autoplayer_function_code]);
    return false;
  }
}

void CAutoplayerButton::SetClickable(bool clickable) {
  _clickable = clickable;
}

void CAutoplayerButton::SetState(const CString state) {
  CString button_state_lower_case = state;
  button_state_lower_case.MakeLower();
  if (button_state_lower_case.Left(4) == "true"
      || button_state_lower_case.Left(2) == "on"
      || button_state_lower_case.Left(3) == "yes"
      || button_state_lower_case.Left(7) == "checked"
      || button_state_lower_case.Left(3) == "lit") {
    SetClickable(true);
  } else {
    SetClickable(false);
  }
}

void CAutoplayerButton::SetLabel(const CString label) {
  _label = label;
}

bool CAutoplayerButton::IsAllin() {
  return p_string_match->IsStringAllin(_label);
}

bool CAutoplayerButton::IsRaise() {
  CString s_lower_case = _label.MakeLower();
  s_lower_case = s_lower_case.Left(5);
  return (s_lower_case == "raise"
    || s_lower_case == "ra1se"
    || s_lower_case == "ralse"
    || s_lower_case.Left(3) == "bet"
    // Last occurence of swag for backward compatibility
    || s_lower_case.Left(4) == "swag");
}

bool CAutoplayerButton::IsCall() {
  CString s_lower_case = _label.MakeLower();
  s_lower_case = s_lower_case.Left(4);
  return (s_lower_case == "call" || s_lower_case == "caii" || s_lower_case == "ca11");
}

bool CAutoplayerButton::IsCheck() {
  CString s_lower_case = _label.MakeLower();
  s_lower_case = s_lower_case.Left(5);
  return (s_lower_case == "check" || s_lower_case == "cheok");
}

bool CAutoplayerButton::IsFold() {
  CString s_lower_case = _label.MakeLower();
  s_lower_case = s_lower_case.Left(4);
  return (s_lower_case == "fold" || s_lower_case == "fo1d" || s_lower_case == "foid");
}

bool CAutoplayerButton::IsAutopost() {
  CString s_lower_case = _label;
  s_lower_case.Remove(' ');
  s_lower_case.Remove('-');
  s_lower_case.MakeLower();
  s_lower_case = s_lower_case.Left(8);
  return (s_lower_case == "autopost" || s_lower_case == "aut0p0st");
}

bool CAutoplayerButton::IsSitin() {
  CString s_lower_case = _label;
  s_lower_case.MakeLower();
  s_lower_case.Remove(' ');
  s_lower_case.Remove('-');
  s_lower_case = s_lower_case.Left(5);
  return (s_lower_case == "sitin" || s_lower_case == "s1t1n");
}

bool CAutoplayerButton::IsSitout() {
  CString s_lower_case = _label;
  s_lower_case.MakeLower();
  s_lower_case.Remove(' ');
  s_lower_case.Remove('-');
  s_lower_case = s_lower_case.Left(6);
  return (s_lower_case == "sitout" || s_lower_case == "s1tout" || s_lower_case == "sit0ut" || s_lower_case == "s1t0ut");
}

bool CAutoplayerButton::IsLeave() {
  return (_label.MakeLower().Left(5) == "leave");
}

bool CAutoplayerButton::IsRematch() {
  return (_label.MakeLower().Left(5) == "rematch");
}

bool CAutoplayerButton::IsPrefold() {
  return (_label.MakeLower().Left(7) == "prefold");
}






/*
bool CScraper::GetButtonState(int button_index) {
  CString l_button_state = "";
  if (button_index <= 9) {
    if (p_symbol_engine_casino->ConnectedToManualMode() && button_index == 5) {
      // Don't MakeLower our mm_network symbol
      l_button_state = p_table_state->_SCI._button_state[button_index];
    }
    else {
      // Default
      l_button_state = p_table_state->_SCI._button_state[button_index].MakeLower();
    }
    return GetButtonState(l_button_state);
  }
  else if (button_index >= 860) {
    l_button_state = p_table_state->_SCI._i86X_button_state[button_index - 860];
    return GetButtonState(l_button_state);
  }
  return false;
} 


bool CCasinoInterface::ButtonAvailable(int autoplayer_code) {
  return p_scraper_access->available_buttons[autoplayer_code];
}

bool CCasinoInterface::ButtonClickable(int autoplayer_code) {
  return (ButtonAvailable(autoplayer_code)
    && p_scraper_access->visible_buttons[autoplayer_code]);
}*/
/*
bool CScraperAccess::GetBetpotButtonVisible(int button_code)
{
  
  Checks if a betpot button is visible
  i.e. available for taking an action
  

  CString betpot_button_state = p_table_state->_SCI._betpot_button_state[button_code];
  return p_scraper->GetButtonState(betpot_button_state);
}*/
/*
void CScraper::SetButtonState(CString *button_state, CString text) {
  if (text != "")
  {
    *button_state = text;
  }
  else
  {
    *button_state = "false";
  }
}*/