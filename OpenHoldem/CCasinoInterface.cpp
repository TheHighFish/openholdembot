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

int CCasinoInterface::NumberOfVisibleAutoplayerButtons() {
  int result = LogicalAutoplayerButton(k_autoplayer_function_fold)->IsClickable()
    + LogicalAutoplayerButton(k_autoplayer_function_call)->IsClickable()
    + LogicalAutoplayerButton(k_autoplayer_function_check)->IsClickable()
    + LogicalAutoplayerButton(k_autoplayer_function_raise)->IsClickable()
    + LogicalAutoplayerButton(k_autoplayer_function_allin)->IsClickable();
   write_log(preferences.debug_autoplayer(), "[CasinoInterface] %i autoplayer buttons visible.\n", result);
  return result;
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

bool CCasinoInterface::EnterBetsize(double total_betsize_in_dollars) {
  if (_betsize_input_box.IsReadyToBeUsed()) {
    return _betsize_input_box.EnterBetsize(total_betsize_in_dollars);
  }
  return false;
}

bool CCasinoInterface::UseSliderForAllin() {
  return _allin_slider.SlideAllin();
}

bool CCasinoInterface::IsMyTurn() {
  return (NumberOfVisibleAutoplayerButtons() >= k_min_buttons_needed_for_my_turn);
}

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
  default:
    // The i86-autoplayer-buttons are flexible
    // and have to be searched by label.
    for (int i = 0; i < k_max_number_of_buttons; ++i) {
      // Other buttons "iX" have to be looked up by their label
      switch (autoplayer_function_code) {
      case k_autoplayer_function_allin:
        if (_technical_autoplayer_buttons[i].IsAllin()) {
          return &_technical_autoplayer_buttons[i];
        }
      case k_autoplayer_function_raise:
        if (_technical_autoplayer_buttons[i].IsRaise()) {
          return &_technical_autoplayer_buttons[i];
        }
      case k_autoplayer_function_call:
        if (_technical_autoplayer_buttons[i].IsCall()) {
          return &_technical_autoplayer_buttons[i];
        }
      case k_autoplayer_function_check:
        if (_technical_autoplayer_buttons[i].IsCheck()) {
          return &_technical_autoplayer_buttons[i];
        }
      case k_autoplayer_function_fold:
        if (_technical_autoplayer_buttons[i].IsFold()) {
          return &_technical_autoplayer_buttons[i];
        }
      case k_hopper_function_sitin:
        if (_technical_autoplayer_buttons[i].IsSitin()) {
          return &_technical_autoplayer_buttons[i];
        }
      case k_hopper_function_sitout:
        if (_technical_autoplayer_buttons[i].IsSitout()) {
          return &_technical_autoplayer_buttons[i];
        }
      case k_hopper_function_leave:
        if (_technical_autoplayer_buttons[i].IsLeave()) {
          return &_technical_autoplayer_buttons[i];
        }
      case k_hopper_function_rematch:
        if (_technical_autoplayer_buttons[i].IsRematch()) {
          return &_technical_autoplayer_buttons[i];
        }
      case k_hopper_function_autopost:
        if (_technical_autoplayer_buttons[i].IsAutopost()) {
          return &_technical_autoplayer_buttons[i];
        }
      case k_standard_function_prefold:
        if (_technical_autoplayer_buttons[i].IsPrefold()) {
          return &_technical_autoplayer_buttons[i];
        }
      }
    }
  }
  // k_autoplayer_function_beep and invalid input
  return &_non_clickable_fake_button;
}

CAutoplayerButton* CCasinoInterface::BetsizeConfirmationButton() {
  // Last hard-coded default: i3-button
  return &_technical_autoplayer_buttons[3];
}

bool CCasinoInterface::AllinOptionAvailable() {
  if (LogicalAutoplayerButton(k_autoplayer_function_allin)->IsClickable()) {
    return true;
  }
  if (BetsizeConfirmationButton()->IsClickable()) {
    return true;
  }
  if (_allin_slider.SlideAllinPossible()) {
    return true;
  }
  return false;
}
