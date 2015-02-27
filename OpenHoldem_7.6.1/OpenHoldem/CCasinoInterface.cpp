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

#include "..\CTableMap\CTableMapAccess.h"
#include "CAutoConnector.h"
#include "CAutoplayerFunctions.h"
#include "CAutoplayerTrace.h"
#include "CBetroundCalculator.h"
#include "CFunctionCollection.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CSymbolEngineCasino.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineHistory.h"
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
	p_null.x = 0;
	p_null.y = 0;
	// dummy rectangle
	r_null.bottom = -1;
	r_null.left   = -1;
	r_null.right  = -1;
	r_null.top    = -1;
}

CCasinoInterface::~CCasinoInterface() {
}

bool CCasinoInterface::TableLostFocus() {
	return (GetForegroundWindow() != p_autoconnector->attached_hwnd());
}

void CCasinoInterface::ClickRect(RECT rect) {
	write_log(preferences.debug_autoplayer(), "[CasinoInterface] Calling mouse.dll to single click button: %d,%d %d,%d\n", 
		rect.left, rect.top, rect.right, rect.bottom);

	(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), rect, MouseLeft, 1, GetFocus(), p_null);
  p_symbol_engine_time->ResetOnAutoPlayerAction();
}

bool CCasinoInterface::ButtonAvailable(int autoplayer_code) {
	return p_scraper_access->available_buttons[autoplayer_code];
}

bool CCasinoInterface::ButtonClickable(int autoplayer_code) {
	return (ButtonAvailable(autoplayer_code)
		&& p_scraper_access->visible_buttons[autoplayer_code]);
}

bool CCasinoInterface::ClickButton(int autoplayer_function_code) {
	//write_log(preferences.debug_autoplayer(), "[CasinoInterface]  
	if (ButtonClickable(autoplayer_function_code)) {
		ClickRect(action_buttons[autoplayer_function_code]);
		write_log(preferences.debug_autoplayer(), "[CasinoInterface] Clicked button %s\n", k_standard_function_names[autoplayer_function_code]);
		return true;
	}	else {
		write_log(preferences.debug_autoplayer(), "[CasinoInterface] Could not click button %s. Either undefined or not visible.\n", k_standard_function_names[autoplayer_function_code]);
		return false;
	}
}

bool CCasinoInterface::ClickButtonSequence(int first_button, int second_button, int delay_in_milli_seconds) {
	if (ClickButton(first_button)) {
		Sleep(delay_in_milli_seconds); 
		if (TableLostFocus())	{
			return false;
		}
    return ClickButton(second_button);
	}
  return false;
}

bool CCasinoInterface::UseSliderForAllin() {
  POINT			cur_pos = {0};
	POINT			point_null = {-1, -1};

	write_log(preferences.debug_autoplayer(), "[CasinoInterface] Starting DoSlider...\n");

	if (!(p_scraper_access->i3_slider_defined && p_scraper_access->i3_handle_defined)) {
		write_log(preferences.debug_autoplayer(), "[CasinoInterface] ...ending DoSlider early (i3handle or i3slider are not defined in the tablemap)\n");
		return false;
	}
	if (!p_table_state->_SCI._handle_found_at_xy)	{
		write_log(preferences.debug_autoplayer(), "[CasinoInterface] ...ending DoSlider early (handle not found - i3handle must use a transform that resolves to either 'handle' or 'true')\n");
		return false;
	}
  // Click and drag handle
	RECT drag_region;
	drag_region.left      = p_table_state->_SCI._handle_xy.x + ((i3_handle_region.right - i3_handle_region.left)/2);
	drag_region.top       = p_table_state->_SCI._handle_xy.y + ((i3_handle_region.bottom - i3_handle_region.top)/2);
	drag_region.right     = p_table_state->_SCI._handle_xy.x + (i3_slider_region.right - i3_slider_region.left);
	drag_region.bottom    = drag_region.top;		

	write_log(preferences.debug_autoplayer(), "[CasinoInterface] Slider : Calling mouse.dll to jam from %d,%d to %d,%d\n", drag_region.left, drag_region.top, drag_region.right, drag_region.bottom);
	(theApp._dll_mouse_click_drag) (p_autoconnector->attached_hwnd(), drag_region, NULL, point_null);

	write_log(preferences.debug_autoplayer(), "[CasinoInterface] Sleeping %d ms\n.", preferences.swag_delay_3());
	Sleep(preferences.swag_delay_3());

	// Click confirmation button
	if (p_tablemap->swagconfirmationmethod() == BETCONF_ENTER) 	{
		write_log(preferences.debug_autoplayer(), "[CasinoInterface] Slider Confirmation : calling keyboard.dll to press 'Enter'\n");
		(theApp._dll_keyboard_sendkey) (p_autoconnector->attached_hwnd(), r_null, VK_RETURN, GetFocus(), cur_pos);
	}	else if (p_tablemap->swagconfirmationmethod() == BETCONF_CLICKBET &&
			 (p_scraper_access->available_buttons[k_autoplayer_function_raise] || p_scraper_access->i3_button_available)) {
		int confirmation_button = k_button_undefined;

		// use allin button if it exists,  
		// otherwise use the bet/raise button region
		if (p_scraper_access->available_buttons[k_autoplayer_function_allin])	{
			write_log(preferences.debug_autoplayer(), "[CasinoInterface] Slider Confirmation : Using the allin button\n");
			confirmation_button = k_autoplayer_function_allin;
		}	else {
			write_log(preferences.debug_autoplayer(), "[CasinoInterface] Slider Confirmation : Using the raise button\n");
			confirmation_button = k_autoplayer_function_raise;
		}

		if (p_tablemap->buttonclickmethod() == BUTTON_DOUBLECLICK)		{
			ClickButtonSequence(confirmation_button, confirmation_button, k_double_click_delay);
		}	else {
			ClickButton(confirmation_button);
		}
	}	else {
		write_log(preferences.debug_autoplayer(), "[CasinoInterface] ...ending DoSlider early (invalid swagconfirmationmethod).\n");
		return false;
	}

	p_autoplayer_trace->Print(ActionConstantNames(k_autoplayer_function_allin), true);
	write_log(preferences.debug_autoplayer(), "[CasinoInterface] Jam complete: %d,%d,%d,%d\n", drag_region.left, drag_region.top, drag_region.right, drag_region.bottom);

	// reset elapsedauto symbol
	write_log(preferences.debug_autoplayer(), "[CasinoInterface] ...ending DoSlider.\n");
	return true;
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

bool CCasinoInterface::ClickI86ButtonIfAvailable(int button_number)
{
	assert(button_number >= 0);
	assert(button_number < k_max_number_of_i86X_buttons);

	if (p_scraper_access->i86X_button_available[button_number])
	{
		CMyMutex	mutex;

		if (!mutex.IsLocked())
			return false;
		write_log(preferences.debug_autoplayer(), "[CasinoInterface] Found valid i86 (%d) button and clicked it.\n", button_number);
		ClickRect(i86X_button[button_number]);
		return true;
	}

	return false;
}

void CCasinoInterface::SelectSwagText()
{
	if (p_tablemap->swagselectionmethod() == TEXTSEL_SINGLECLICK)
	{
		write_log(preferences.debug_autoplayer(), "[CasinoInterface] Text selection; calling mouse.dll to single click: %d,%d %d,%d\n", 
			i3_edit_region.left, i3_edit_region.top, i3_edit_region.right, i3_edit_region.bottom);
		(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), i3_edit_region, MouseLeft, 1, NULL, p_null);
	}
	else if (p_tablemap->swagselectionmethod() == TEXTSEL_DOUBLECLICK)
	{
		write_log(preferences.debug_autoplayer(), "[CasinoInterface] Text selection; calling mouse.dll to double click: %d,%d %d,%d\n", 
			i3_edit_region.left, i3_edit_region.top, i3_edit_region.right, i3_edit_region.bottom);
		(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), i3_edit_region, MouseLeft, 2, NULL, p_null);
	}
	else if (p_tablemap->swagselectionmethod() == TEXTSEL_TRIPLECLICK)
	{
		write_log(preferences.debug_autoplayer(), "[CasinoInterface] Text selection; calling mouse.dll to triple click: %d,%d %d,%d\n", 
			i3_edit_region.left, i3_edit_region.top, i3_edit_region.right, i3_edit_region.bottom);
		(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), i3_edit_region, MouseLeft, 3, NULL, p_null);
	}
	else if (p_tablemap->swagselectionmethod() == TEXTSEL_CLICKDRAG)
	{
		write_log(preferences.debug_autoplayer(), "[CasinoInterface] Text selection; calling mouse.dll to click drag: %d,%d %d,%d\n", 
			i3_edit_region.left, i3_edit_region.top, i3_edit_region.right, i3_edit_region.bottom);
		(theApp._dll_mouse_click_drag) (p_autoconnector->attached_hwnd(), i3_edit_region, NULL, p_null);
	}

	else if (p_tablemap->swagselectionmethod() == TEXTSEL_NOTHING)
	{
	}
	else
	{
		write_log(preferences.debug_autoplayer(), "[CasinoInterface] ...ending DoSwag early (invalid swagselectionmethod).\n");
		return;
	}
}

void CCasinoInterface::DeleteSwagText()
{
	if (p_tablemap->swagdeletionmethod() == TEXTDEL_DELETE)
	{
		write_log(preferences.debug_autoplayer(), "[CasinoInterface] Text deletion; calling keyboard.dll to press 'delete'\n");
		(theApp._dll_keyboard_sendkey) (p_autoconnector->attached_hwnd(), r_null, VK_DELETE, NULL, p_null);
	}
	else if (p_tablemap->swagdeletionmethod() == TEXTDEL_BACKSPACE)
	{
		write_log(preferences.debug_autoplayer(), "[CasinoInterface] Text deletion; calling keyboard.dll to press 'backspace'\n");
		(theApp._dll_keyboard_sendkey) (p_autoconnector->attached_hwnd(), r_null, VK_BACK, NULL, p_null);
	}
	else if (p_tablemap->swagdeletionmethod() == TEXTDEL_NOTHING)
	{}
}

bool CCasinoInterface::EnterBetsize(double total_betsize_in_dollars) {
	POINT			cur_pos = {0};
	bool			lost_focus = false;
	POINT			point_null = {-1, -1};
	CString		swag_amt;

	write_log(preferences.debug_autoplayer(), "[CasinoInterface] Starting DoSwag...\n");

	// swag regions are hard coded as #3 for now, due to legacy WH standard
	if (!p_scraper_access->i3_edit_defined || !p_scraper_access->i3_button_available)
	{
		write_log(preferences.debug_autoplayer(), "[CasinoInterface] ...ending DoSwag early (no edit field or no i3button).\n");
		return false;
	}

	SelectSwagText();

	// First sleep(), THEN check for stolen focus, then act
	//  NOT the other way: http://www.maxinmontreal.com/forums/viewtopic.php?f=120&t=14791

	write_log(preferences.debug_autoplayer(), "[CasinoInterface] Sleeping %dms.\n", preferences.swag_delay_1());
	Sleep(preferences.swag_delay_1()); 
	// Check for stolen , and thus misswag
	if (TableLostFocus()) {
		lost_focus = true;
  }
	DeleteSwagText();

	write_log(preferences.debug_autoplayer(), "[CasinoInterface] Sleeping %dms.\n", preferences.swag_delay_2());
	Sleep(preferences.swag_delay_2());
	// Check for stolen focus, and thus misswag
	if (TableLostFocus()) {
		lost_focus = true;
  }
	// SWAG AMOUNT ENTRY
	double swag_adjusted = AdjustedBetsize(total_betsize_in_dollars);
	swag_amt = Number2CString(swag_adjusted);
	write_log(preferences.debug_autoplayer(), "[CasinoInterface] Swag amount (not adjusted): %.2f\n", total_betsize_in_dollars);
	write_log(preferences.debug_autoplayer(), "[CasinoInterface] Swag amount; calling keyboard.dll to swag (adjusted): %s %d,%d %d,%d\n", 
		swag_amt, i3_edit_region.left, i3_edit_region.top, i3_edit_region.right, i3_edit_region.bottom);
	(theApp._dll_keyboard_sendstring) (p_autoconnector->attached_hwnd(), i3_edit_region, swag_amt, preferences.swag_use_comma(), NULL, point_null);

	write_log(preferences.debug_autoplayer(), "[CasinoInterface] Sleeping %dms.\n", preferences.swag_delay_3());
	Sleep(preferences.swag_delay_3());
	// Check for stolen focus, and thus misswag
	if (TableLostFocus()) {
    lost_focus = true;
  }
  // BET CONFIRMATION ACTION
	if (!lost_focus) {
		if (p_tablemap->swagconfirmationmethod() == BETCONF_ENTER) 	{
			write_log(preferences.debug_autoplayer(), "[CasinoInterface] Confirmation; calling keyboard.dll to press 'Enter'\n");
			(theApp._dll_keyboard_sendkey) (p_autoconnector->attached_hwnd(), r_null, VK_RETURN, GetFocus(), cur_pos);
		}	else if (p_tablemap->swagconfirmationmethod() == BETCONF_CLICKBET 
			  && p_scraper_access->available_buttons[k_autoplayer_function_raise]) 	{
			write_log(preferences.debug_autoplayer(), "[CasinoInterface] Bet Confirmation: Using raise button\n");
      if (p_tablemap->buttonclickmethod() == BUTTON_DOUBLECLICK) 	{
				ClickButtonSequence(k_autoplayer_function_raise, 
					k_autoplayer_function_raise, 
					k_double_click_delay);
			}	else {
				ClickButton(k_autoplayer_function_raise);
			}
		}	else if (p_tablemap->swagconfirmationmethod() == BETCONF_NOTHING)	{
		} else {
			write_log(preferences.debug_autoplayer(), "[CasinoInterface] ...ending DoSwag early (invalid swagconfirmationmethod).\n");
      write_log(preferences.debug_autoplayer(), "[CasinoInterface] Valid options are: \"enter\", \"click bet\", \"nothing\"\n");
			return false;
		}
		p_autoplayer_trace->Print(ActionConstantNames(k_autoplayer_function_betsize), true);
	}
	int betround = p_betround_calculator->betround();
	write_log(preferences.debug_autoplayer(), "[CasinoInterface] ...ending DoSwag, 'didswag' now: %d\n", 
		p_symbol_engine_history->didswag(betround));
	return (!lost_focus);
}

bool CCasinoInterface::EnterBetsizeForAllin() {
  write_log(preferences.debug_autoplayer(), "[CasinoInterface] Going to swag allin\n");
  int userchair = p_symbol_engine_userchair->userchair();
	double betsize_for_allin = p_symbol_engine_chip_amounts->currentbet(userchair)
	  + p_table_state->User()->_balance; 
  return EnterBetsize(betsize_for_allin);
}

int CCasinoInterface::NumberOfVisibleAutoplayerButtons() {
	int number_of_available_buttons =
		(p_scraper_access->available_buttons[k_autoplayer_function_allin] ? 1 : 0)
	  + (p_scraper_access->available_buttons[k_autoplayer_function_raise] ? 1 : 0)
	  + (p_scraper_access->available_buttons[k_autoplayer_function_call]  ? 1 : 0)
	  + (p_scraper_access->available_buttons[k_autoplayer_function_check] ? 1 : 0)
	  + (p_scraper_access->available_buttons[k_autoplayer_function_fold]  ? 1 : 0);
	return number_of_available_buttons;
}
