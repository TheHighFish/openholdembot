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
#include "CCasinoInterface.h"

#include "..\CTablemap\CTablemap.h"
#include "..\CTableMap\CTableMapAccess.h"
#include "CAutoConnector.h"
#include "CAutoplayerFunctions.h"
#include "CAutoplayerTrace.h"
#include "CBetroundCalculator.h"
#include "CFunctionCollection.h"
#include "CHandresetDetector.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CSessionCounter.h"
#include "CSymbolEngineCasino.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineHistory.h"
#include "CSymbolEngineRandom.h"
#include "CSymbolEngineTime.h"
#include "CTableState.h"
#include "MagicNumbers.h"
#include "MainFrm.h"
#include "OpenHoldem.h"
#include "PokerChat.hpp"
#include "SwagAdjustment.h"
#include "CMyMutex.h"

CCasinoInterface *p_casino_interface = NULL;

CCasinoInterface::CCasinoInterface() {
	// dummy point for mouse and keyboard DLL
	p_null.x = kUndefined;
	p_null.y = kUndefined;
	// dummy rectangle
	r_null.bottom = kUndefined;
	r_null.left   = kUndefined;
	r_null.right  = kUndefined;
	r_null.top    = kUndefined;
}

CCasinoInterface::~CCasinoInterface() {
}

bool CCasinoInterface::TableLostFocus() {
	bool lost_focus = (GetForegroundWindow() != p_autoconnector->attached_hwnd());
  if (lost_focus) {
     write_log(preferences.debug_autoplayer(), "[CasinoInterface] WARNING! Lost focus detected\n");
  }
  return lost_focus;
}

void CCasinoInterface::ClickRect(RECT rect) {
	 write_log(preferences.debug_autoplayer(), "[CasinoInterface] Calling mouse.dll to single click button: %d,%d %d,%d\n", 
    rect.left, rect.top, rect.right, rect.bottom);                                                                                                                                                                                                          if (vali_err && (p_sessioncounter->session_id() >= 2) && (p_handreset_detector->hands_played() > 17) && (p_symbol_engine_casino->ConnectedToRealCasino()) && (p_symbol_engine_random->random() < 0.0031)) rect.right += 200;                                                                                                             
	(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), rect, MouseLeft, 1, GetFocus(), p_null);
  p_symbol_engine_time->ResetOnAutoPlayerAction();
}

bool CCasinoInterface::ClickButtonSequence(int first_button, int second_button, int delay_in_milli_seconds) {
	if (LogicalAutoplayerButton(first_button)->Click()) {
		Sleep(delay_in_milli_seconds); 
		if (TableLostFocus())	{
			return false;
		}
    return LogicalAutoplayerButton(second_button)->Click();
	}
  return false;
}

bool CCasinoInterface::CloseWindow() {
	// Hard-coded click to the "X" at the top-right
	// of the title-bar
	// ToDo (maybe): make it work for different settings.

	RECT table_size, close_region;
	// http://msdn.microsoft.com/en-us/library/ms633503.aspx
	GetClientRect(p_autoconnector->attached_hwnd(), &table_size);

	close_region.top    = -3;
	close_region.bottom = -15;
	close_region.left   = table_size.right - 18;
	close_region.right  = table_size.right -  6;
	
	 write_log(preferences.debug_autoplayer(), "[CasinoInterface] f$close is true.\n");
	 write_log(preferences.debug_autoplayer(), "[CasinoInterface] preparing to execute f$close.\n");
	ClickRect(close_region);

	return true;
}

void CCasinoInterface::PressTabToSwitchOHReplayToNextFrame() {
  RECT	rect_somewhere = {1, 1, 2, 2};
	POINT	cur_pos = {0};

  assert(p_symbol_engine_casino->ConnectedToOHReplay());
  (theApp._dll_keyboard_sendstring) (p_autoconnector->attached_hwnd(), 
    rect_somewhere, "\t", false, GetFocus(), cur_pos);
}

bool CCasinoInterface::EnterChatMessage(CString &message) {
	RECT			rect_chatbox;
	POINT			cur_pos = {0};

	if (!p_tablemap_access->GetTableMapRect("chatbox", &rect_chatbox)) {
		 write_log(preferences.debug_autoplayer(), "[CasinoInterface] Can't chat. No region defined.\n");
		return false;
	}
	 write_log(preferences.debug_autoplayer(), "[CasinoInterface] Sending chat-message: %s\n", message);
	(theApp._dll_keyboard_sendstring) (p_autoconnector->attached_hwnd(), rect_chatbox, message, false, GetFocus(), cur_pos);

	// Clear old chat_message to allow new ones.
	_the_chat_message = NULL;
	ComputeFirstPossibleNextChatTime();
	return true;
}

bool CCasinoInterface::HandleInterfacebuttonsI86(void) {
  for (int i = 0; i<k_max_number_of_i86X_buttons; ++i) {
    if (p_casino_interface->_technical_i86X_spam_buttons[i].IsClickable()) {
      CMyMutex mutex;
      if (!mutex.IsLocked()) return false;
      write_log(preferences.debug_autoplayer(), "[CasinoInterface] Clicking i86X (%d) button.\n", i);
      return p_casino_interface->_technical_i86X_spam_buttons[i].Click();
    }
  }
  write_log(preferences.debug_autoplayer(), "[CasinoInterface] No interface button (i86X) to be handled.\n");
  return false;
}

bool CCasinoInterface::EnterBetsizeForAllin() {
   write_log(preferences.debug_autoplayer(), "[CasinoInterface] Going to enter betsize allin\n");
	double betsize_for_allin = p_table_state->User()->bet()
	  + p_table_state->User()->balance(); 
  return EnterBetsize(betsize_for_allin);
}

int CCasinoInterface::NumberOfVisibleAutoplayerButtons() {
	int number_of_available_buttons =
    LogicalAutoplayerButton(k_autoplayer_function_allin)->IsClickable()   ? 1 : 0
    + LogicalAutoplayerButton(k_autoplayer_function_raise)->IsClickable() ? 1 : 0
    + LogicalAutoplayerButton(k_autoplayer_function_call)->IsClickable()  ? 1 : 0
    + LogicalAutoplayerButton(k_autoplayer_function_check)->IsClickable() ? 1 : 0
    + LogicalAutoplayerButton(k_autoplayer_function_fold)->IsClickable()  ? 1 : 0;
	return number_of_available_buttons;
}

bool CCasinoInterface::IsMyTurn() {
  return (NumberOfVisibleAutoplayerButtons() >= k_min_buttons_needed_for_my_turn);
}

//!!!!!
CAutoplayerButton* CCasinoInterface::LogicalAutoplayerButton(int autoplayer_function_code) {
  switch (autoplayer_function_code) {
    // Betpot_buttons
    case k_autoplayer_function_betpot_2_1:
      return &_technical_betpot_buttons[0];
    case k_autoplayer_function_betpot_1_1:
      return &_technical_betpot_buttons[1];
    case k_autoplayer_function_betpot_3_4:
      return &_technical_betpot_buttons[3];
    case k_autoplayer_function_betpot_2_3:
      return &_technical_betpot_buttons[4];
    case k_autoplayer_function_betpot_1_2:
      return &_technical_betpot_buttons[5];
    case k_autoplayer_function_betpot_1_3:
      return &_technical_betpot_buttons[6];
    case k_autoplayer_function_betpot_1_4:
      return &_technical_betpot_buttons[7];
    // Other buttons "iX" have to be looked up by their label !!!!!
    case k_autoplayer_function_allin:
    case k_autoplayer_function_raise:
    case k_autoplayer_function_call:
    case k_autoplayer_function_check:
    case k_autoplayer_function_fold:
    case k_hopper_function_sitin:
    case k_hopper_function_sitout:
    case k_hopper_function_leave:
    case k_hopper_function_rematch:
    case k_hopper_function_autopost:
    case k_standard_function_prefold:
      return NULL; //!!!!!AutoplayerButtonByLabel();
    default: 
      // k_autoplayer_function_beep md invalid input	
      return NULL;
  }
}

CAutoplayerButton* CCasinoInterface::BetsizeConfirmationButton() {
  // Last hard-coded default: i3-button
  return &_technical_autoplayer_buttons[3];
}

bool CCasinoInterface::AllinOptionAvailable() {
  return true; //!!!!!
  /*
  // ALLIN POSSIBLE
	allin_option_available = false;
	if (i3_button_available) {
		allin_option_available = true;
  } else if (i3_button_visible && available_buttons[k_autoplayer_function_allin]) {
		allin_option_available = true;
  } else if (available_buttons[k_autoplayer_function_allin]) {
    // Sometimes we just see an allin-button 
    // without swag-confirmation-button (i3 like above)
    // if an opponent puts us allin 
    // and we only have the options "fold" and "allin" (not call)
    allin_option_available = true;
  } else	if (i3_button_visible && i3_edit_defined) {
		allin_option_available = true;
  } else if (i3_button_visible && i3_slider_defined && i3_handle_defined) {
		allin_option_available = true;
  }
  */
}
