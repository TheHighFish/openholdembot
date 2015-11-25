//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose: Providing higher-level accessors to the data
//  of the CScraper-class like "UserHasCards()".
//
//*******************************************************************************

#include "stdafx.h"
#include "CScraperAccess.h"

#include <assert.h>
#include "CCasinoInterface.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CStringMatch.h"
#include "CSymbolEngineUserchair.h"
#include "..\CTablemap\CTableMapAccess.h"
#include "CTableState.h"
#include "MagicNumbers.h"


CScraperAccess *p_scraper_access = NULL;


CScraperAccess::CScraperAccess()
{
}

CScraperAccess::~CScraperAccess()
{
}

bool CScraperAccess::IsValidCard(int Card)
{
	if (Card >= 0 && Card < kNumberOfCardsPerDeck)
		return true;

	return false;
}

// Button Functions
int CScraperAccess::SearchForButtonNumber(int button_code)
{
	/*
		Searches tablemap labels for button definitions/overrides
		returns the button number if a label is defined
		or the default button number otherwise.
	*/

	int button_number = k_button_undefined;

	// Define a function pointer for the string matching function corresponding to each button_code
	const bool (CStringMatch::*StringMatch)(CString) = NULL;

	switch (button_code)
	{
		// ALLIN
		case k_button_allin:
			StringMatch = &CStringMatch::IsStringAllin;
			break;
		// RAISE
		case k_button_raise:
			StringMatch = &CStringMatch::IsStringRaise;
			break;
		// CALL
		case k_button_call:
			StringMatch = &CStringMatch::IsStringCall;
			break;
		// FOLD
		case k_button_fold:
			StringMatch = &CStringMatch::IsStringFold;
			break;
		// CHECK
		case k_button_check:
			StringMatch = &CStringMatch::IsStringCheck;
			break;
		// SITIN
		case k_button_sitin:
			StringMatch = &CStringMatch::IsStringSitin;
			break;
		// SITOUT
		case k_button_sitout:
			StringMatch = &CStringMatch::IsStringSitout;
			break;
		// LEAVE
		case k_button_leave:
			StringMatch = &CStringMatch::IsStringLeave;
			break;
    // REMATCH
		case k_button_rematch:
			StringMatch = &CStringMatch::IsStringRematch;
			break;
		// PRE-FOLD
		case k_button_prefold:
			StringMatch = &CStringMatch::IsStringPrefold;
			break;
		default:
			break;
	}

	// If a string verification routine is available
	if (StringMatch)
	{
		// Check if there is a match for any of the corresponding button indexes
		// and save it as the button number
		for (int j = 0; j < k_max_number_of_buttons; j++)	{
			if ((p_string_match->*StringMatch)(p_table_state->_SCI._button_label[j]))	{
				button_number = j;
				break;
			}
		}
	}

	return button_number;
}

CString CScraperAccess::SearchForButtonName(int button_code)
{
	/*
		Searches for a button name
		ATM it only handles the i%dbutton format
	*/

	CString button_name = "";
	int button_number = SearchForButtonNumber(button_code);

	// If the button number is still undefined
	// return the default button number
	if (button_number == k_button_undefined)
		button_number = DefaultButtonNumber(button_code);

	if (button_number != k_button_undefined)
		button_name.Format("i%dbutton", button_number);

	return button_name;
}

bool CScraperAccess::SearchForButtonVisible(int button_code)
{
	/*
		Checks if a button is visible
		i.e. available for taking an action
	*/

	bool button_visible = false;
	int button_number = SearchForButtonNumber(button_code);

	if (button_number != k_button_undefined && p_scraper->GetButtonState(button_number))
		button_visible = true;

	return button_visible;
}

CString CScraperAccess::GetButtonName(int button_number)
{
	/*
		Returns a button name
	*/

	if (button_number == k_button_undefined)
		return "";

	CString button_name = "";
	button_name.Format("i%dbutton", button_number);

	return button_name;
}

bool CScraperAccess::GetButtonVisible(int button_number)
{
	/*
		Checks if a button is visible
		i.e. available for taking an action
	*/

	bool button_visible = false;

	if (button_number != k_button_undefined && p_scraper->GetButtonState(button_number))
		button_visible = true;

	return button_visible;
}

bool CScraperAccess::GetBetpotButtonVisible(int button_code)
{
	/*
		Checks if a betpot button is visible
		i.e. available for taking an action
	*/

  CString betpot_button_state = p_table_state->_SCI._betpot_button_state[button_code];
	return p_scraper->GetButtonState(betpot_button_state);
}

void CScraperAccess::InitOnConnect()
{
	GetNeccessaryTablemapObjects();
}

void CScraperAccess::GetNeccessaryTablemapObjects() {
	// NUMBERS (from labels)
	_allin_button_number	  = SearchForButtonNumber(k_button_allin);
	_raise_button_number	  = SearchForButtonNumber(k_button_raise);
	_call_button_number		  = SearchForButtonNumber(k_button_call);
	_check_button_number	  = SearchForButtonNumber(k_button_check);
	_fold_button_number		  = SearchForButtonNumber(k_button_fold);
	_prefold_button_number  = SearchForButtonNumber(k_button_prefold);
	_sitin_button_number	  = SearchForButtonNumber(k_button_sitin);
	_sitout_button_number   = SearchForButtonNumber(k_button_sitout);
	_leave_button_number	  = SearchForButtonNumber(k_button_leave);
  _rematch_button_number	= SearchForButtonNumber(k_button_rematch);
	_autopost_button_number	= SearchForButtonNumber(k_button_autopost);
  // NAMES
	button_names[k_autoplayer_function_allin]		= GetButtonName(_allin_button_number);
	button_names[k_autoplayer_function_raise]		= GetButtonName(_raise_button_number);
	button_names[k_autoplayer_function_call]		= GetButtonName(_call_button_number);
	button_names[k_autoplayer_function_check]		= GetButtonName(_check_button_number);
	button_names[k_autoplayer_function_fold]		= GetButtonName(_fold_button_number);
	button_names[k_standard_function_prefold]		= GetButtonName(_prefold_button_number);
	button_names[k_hopper_function_sitin]		  = GetButtonName(_sitin_button_number);
	button_names[k_hopper_function_sitout]		= GetButtonName(_sitout_button_number);
	button_names[k_hopper_function_leave]		  = GetButtonName(_leave_button_number);
  button_names[k_hopper_function_rematch]		= GetButtonName(_rematch_button_number);
	button_names[k_hopper_function_autopost]	= GetButtonName(_autopost_button_number);
	// same for the betpot buttons - hardcoded so should only be done once at startup ?
	for (int i=k_autoplayer_function_betpot_2_1; i<=k_autoplayer_function_betpot_1_4; ++i) {
		int button_index = i - k_autoplayer_function_betpot_2_1;
		button_names[i].Format("%s_%s", 
			k_betpot_button_name[button_index], 
			"button");
	}
	// hardcoded so should only be done once at startup ?
	_i3_button_name  = "i3button";
	_i3_edit_name    = "i3edit";
	_i3_slider_name  = "i3slider";
	_i3_handle_name  = "i3handle";
	_i86_button_name = "i86button";
	CString button_name = "";
	for (int i = 0; i < k_max_number_of_i86X_buttons; i++) {
		button_name.Format("i86%dbutton", i);
		_i86X_button_name[i] = button_name;
	}
  // VISIBLE
	visible_buttons[k_autoplayer_function_allin]	= GetButtonVisible(_allin_button_number);
	visible_buttons[k_autoplayer_function_raise]	= GetButtonVisible(_raise_button_number);
	visible_buttons[k_autoplayer_function_call]		= GetButtonVisible(_call_button_number);
	visible_buttons[k_autoplayer_function_check]	= GetButtonVisible(_check_button_number);
	visible_buttons[k_autoplayer_function_fold]		= GetButtonVisible(_fold_button_number);
	visible_buttons[k_standard_function_prefold]	= GetButtonVisible(_prefold_button_number);
	visible_buttons[k_hopper_function_sitin]		= GetButtonVisible(_sitin_button_number);
	visible_buttons[k_hopper_function_sitout]		= GetButtonVisible(_sitout_button_number);
	visible_buttons[k_hopper_function_leave]		= GetButtonVisible(_leave_button_number);
  visible_buttons[k_hopper_function_rematch]	= GetButtonVisible(_rematch_button_number);
	visible_buttons[k_hopper_function_autopost]	= GetButtonVisible(_autopost_button_number);
	// visible betpot buttons
	for (int i=k_autoplayer_function_betpot_2_1; i<=k_autoplayer_function_betpot_1_4; ++i) {
		int button_index = i - k_autoplayer_function_betpot_2_1;
		visible_buttons[i] = GetBetpotButtonVisible(button_index);
	}
	// hardcoded 
	i3_button_visible = GetButtonVisible(k_button_i3);
	i86_button_visible = GetButtonVisible(k_button_i86);
	for (int i = 0; i < k_max_number_of_i86X_buttons; i++) {
		i86X_button_visible[i] = GetButtonVisible(k_button_i86*k_max_number_of_i86X_buttons + i);
	}
  // DEFINED + AVAILABLE
	for (int i=0; i<k_number_of_standard_functions; i++) {
		defined_buttons[i] = p_tablemap_access->GetButtonRect(button_names[i], &p_casino_interface->action_buttons[i]);
		available_buttons[i] = defined_buttons[i] && visible_buttons[i];
	}
	// Defined + Available special regions
	i3_button_defined		= p_tablemap_access->GetButtonRect("i3button", &p_casino_interface->i3_button);
	i3_edit_defined			= p_tablemap_access->GetTableMapRect("i3edit", &p_casino_interface->i3_edit_region);
	i3_slider_defined		= p_tablemap_access->GetTableMapRect("i3slider", &p_casino_interface->i3_slider_region);
	i3_handle_defined		= p_tablemap_access->GetTableMapRect("i3handle", &p_casino_interface->i3_handle_region);
	i86_button_defined		= p_tablemap_access->GetButtonRect("i86button", &p_casino_interface->i86_button);
	i3_button_available		= i3_button_defined && i3_button_visible;
	i86_button_available	= i86_button_defined && i86_button_visible;
	// i86Xbutton
	CString i86X_button_name = "";
	for (int i = 0; i < k_max_number_of_i86X_buttons; i++) {
		i86X_button_name.Format("i86%dbutton", i);
		i86X_button_defined[i]     = p_tablemap_access->GetButtonRect(i86X_button_name, &p_casino_interface->i86X_button[i]);
		i86X_button_available[i]   = i86X_button_defined[i] && i86X_button_visible[i];
	}
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
}

int CScraperAccess::NumberOfVisibleButtons()
{
	// Buttons for playing actions, e.g. fold or allin.
	// There have to be at least 2 to make it our turn.
	int number_of_visible_buttons = 0
		+ (available_buttons[k_autoplayer_function_allin] ? 1 : 0)
		+ (available_buttons[k_autoplayer_function_raise] ? 1 : 0)
		+ (available_buttons[k_autoplayer_function_call]  ? 1 : 0)
		+ (available_buttons[k_autoplayer_function_check] ? 1 : 0)
		+ (available_buttons[k_autoplayer_function_fold]  ? 1 : 0);
	write_log(preferences.debug_scraper(), "[CScraperAccess] NumberOfVisibleButtons() found %d buttons\n",
		number_of_visible_buttons);
	return number_of_visible_buttons;
}

bool CScraperAccess::IsPlayerActive(int player) {
	bool result = p_table_state->_players[player]._active;
	result = result && IsPlayerSeated(player);
	return result;
}

bool CScraperAccess::IsPlayerSeated(int player)
{
	bool result = p_table_state->_players[player]._seated;
	return result;
}

bool CScraperAccess::IsGoodPlayername(int chair)
{
	CString playername = p_table_state->_players[chair]._name;
	int length = playername.GetLength();
	if (length == 0) 
		return false;
	for (int i=0; i<length; i++)
	{
		if (isalnum(playername[i])) 
			return true;
	}
	return false;
}

bool CScraperAccess::IsKnownCard(int card)
{
	if (card == CARD_NOCARD 
		|| card == CARD_BACK
		|| card > 0x0100
		|| card < 0)
	{
		return false;
	}
	return true;
}

int CScraperAccess::NumberOfCommonCards() {
  int number_of_common_cards = 0;
  for (int i=0; i<kNumberOfCommunityCards; ++i) {
    if (IsKnownCard(p_table_state->_common_cards[i].GetValue())) {
      ++number_of_common_cards;
    }
  }
  return number_of_common_cards;
}

