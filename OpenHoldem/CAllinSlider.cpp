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
#include "CAllinSlider.h"

#include "CAutoConnector.h"
#include "CAutoplayerTrace.h"
#include "CCasinoInterface.h"
#include "CPreferences.h"
#include "../CTablemap/CTablemap.h"
#include "OpenHoldem.h"

CAllinSlider::CAllinSlider() {
  ResetHandlePosition();
}

CAllinSlider::~CAllinSlider() {
}

void CAllinSlider::SetHandlePosition(const POINT position) {
  _position.x = position.x;
  _position.y = position.y;
}

void CAllinSlider::ResetHandlePosition() {
  _position.x = kUndefined;
  _position.y = kUndefined;
}

bool CAllinSlider::SlideAllin() {
  //!!!!!
  POINT	cur_pos = { 0 };
  // Not really (0, 0), but (-1, -1), out of the screen
  POINT	point_null = { kUndefined, kUndefined };

  write_log(preferences.debug_autoplayer(), "[AllinSlider] Starting DoSlider...\n");
  //if (!(p_scraper_access->i3_slider_defined && p_scraper_access->i3_handle_defined)) {
    write_log(preferences.debug_autoplayer(), "[AllinSlider] ...ending DoSlider early (i3handle or i3slider are not defined in the tablemap)\n");
    //!!!!!return false;
  //}
  if ((_position.x != kUndefined) && (_position.y != kUndefined)) {
    write_log(preferences.debug_autoplayer(), "[AllinSlider] ...ending DoSlider early (handle not found - i3handle must use a transform that resolves to either 'handle' or 'true')\n");
    return false;
  }
  // Click and drag handle
  RECT drag_region;
  // !!!!! Compare with old code!
  RECT i3_handle_region, i3_slider_region; // undefined!!!!!
  drag_region.left = _position.x + ((i3_handle_region.right - i3_handle_region.left) / 2);
  drag_region.top = _position.y + ((i3_handle_region.bottom - i3_handle_region.top) / 2);
  drag_region.right = _position.x + (i3_slider_region.right - i3_slider_region.left);
  drag_region.bottom = drag_region.top;

  write_log(preferences.debug_autoplayer(), "[AllinSlider] Slider : Calling mouse.dll to jam from %d,%d to %d,%d\n", drag_region.left, drag_region.top, drag_region.right, drag_region.bottom);
  (theApp._dll_mouse_click_drag) (p_autoconnector->attached_hwnd(), drag_region, NULL, point_null);

  write_log(preferences.debug_autoplayer(), "[AllinSlider] Sleeping %d ms\n.", preferences.swag_delay_3());
  Sleep(preferences.swag_delay_3());

  // Click confirmation button //!!!!! Duplicate to Betsize-box?
  /*
  if (p_tablemap->swagconfirmationmethod() == BETCONF_ENTER) {
    write_log(preferences.debug_autoplayer(), "[AllinSlider] Slider Confirmation : calling keyboard.dll to press 'Enter'\n");
    (theApp._dll_keyboard_sendkey) (p_autoconnector->attached_hwnd(), r_null, VK_RETURN, GetFocus(), cur_pos);
  }
  else if (p_tablemap->swagconfirmationmethod() == BETCONF_CLICKBET
    && (p_casino_interface->LogicalAutoplayerButton(k_autoplayer_function_raise)->IsClickable() || p_casino_interface->BetsizeConfirmationButton()->IsClickable())) {
    int confirmation_button = k_button_undefined;

    // use allin button if it exists,  //!!!!!
    // otherwise use the bet/raise button region
    if (p_casino_interface->LogicalAutoplayerButton(k_autoplayer_function_allin)->IsClickable()) {
      write_log(preferences.debug_autoplayer(), "[AllinSlider] Slider Confirmation : Using the allin button\n");
      confirmation_button = k_autoplayer_function_allin;
    }
    else {
      write_log(preferences.debug_autoplayer(), "[AllinSlider] Slider Confirmation : Using the raise button\n");
      confirmation_button = k_autoplayer_function_raise;
    }

    if (p_tablemap->buttonclickmethod() == BUTTON_DOUBLECLICK) {
      ClickButtonSequence(confirmation_button, confirmation_button, k_double_click_delay);
    }
    else {
      BetsizeConfirmationButton()->Click();
    }
  }
  else {
    write_log(preferences.debug_autoplayer(), "[AllinSlider] ...ending DoSlider early (invalid betsizeconfirmationmethod).\n");
    return false;
  }
  */
  p_autoplayer_trace->Print(ActionConstantNames(k_autoplayer_function_allin), true);
  write_log(preferences.debug_autoplayer(), "[AllinSlider] Jam complete: %d,%d,%d,%d\n", drag_region.left, drag_region.top, drag_region.right, drag_region.bottom);

  // reset elapsedauto symbol
  write_log(preferences.debug_autoplayer(), "[AllinSlider] ...ending DoSlider.\n");
  return true;
}