#include "stdafx.h"
#include "CScraperAccess.h"

#include "CPreferences.h"
#include "CScraper.h"
#include "CStringMatch.h"
#include "..\CTablemap\CTableMapAccess.h"

#include "MagicNumbers.h"


CScraperAccess *p_scraper_access = NULL;


CScraperAccess::CScraperAccess()
{
}

CScraperAccess::~CScraperAccess()
{
}

// Card Functions
int CScraperAccess::GetPlayerCards(int seat_number, int first_or_second_card)
{
	return p_scraper->card_player(seat_number, first_or_second_card);
}

int CScraperAccess::GetCommonCard(int index_zero_to_four)
{
	return p_scraper->card_common(index_zero_to_four);
}

bool CScraperAccess::IsValidCard(int Card)
{
	if (Card >= 0 && Card < k_number_of_cards_per_deck)
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
		default:
			break;
	}

	// If a string verification routine is available
	if (StringMatch)
	{
		// Check if there is a match for any of the corresponding button indexes
		// and save it as the button number
		for (int j = 0; j < k_max_number_of_buttons; j++)
		{
			if ((p_string_match->*StringMatch)(p_scraper->button_label(j)))
			{
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

bool CScraperAccess::GetBetpotButtonVisible(int i)
{
	CString betpot_button_state = p_scraper->betpot_button_state(i);

	if (betpot_button_state.MakeLower().Left(4) == "true" ||
		betpot_button_state.MakeLower().Left(2) == "on" ||
		betpot_button_state.MakeLower().Left(3) == "yes" ||
		betpot_button_state.MakeLower().Left(7) == "checked" ||
		betpot_button_state.MakeLower().Left(3) == "lit" )
	{
		return true;
	}

	return false;
}

void CScraperAccess::SetScraperAccessData()
{
	// find button numbers
	_allin_button_number	= SearchForButtonNumber(k_button_allin);
	_raise_button_number	= SearchForButtonNumber(k_button_raise);
	_call_button_number		= SearchForButtonNumber(k_button_call);
	_check_button_number	= SearchForButtonNumber(k_button_check);
	_fold_button_number		= SearchForButtonNumber(k_button_fold);
	_sitin_button_number	= SearchForButtonNumber(k_button_sitin);
	_sitout_button_number	= SearchForButtonNumber(k_button_sitout);
	_leave_button_number	= SearchForButtonNumber(k_button_leave);
	_prefold_button_number	= SearchForButtonNumber(k_button_prefold);
	_autopost_button_number	= SearchForButtonNumber(k_button_autopost);

	// set button names
	_allin_button_name		= GetButtonName(_allin_button_number);
	_raise_button_name		= GetButtonName(_raise_button_number);
	_call_button_name		= GetButtonName(_call_button_number);
	_check_button_name		= GetButtonName(_check_button_number);
	_fold_button_name		= GetButtonName(_fold_button_number);
	_sitin_button_name		= GetButtonName(_sitin_button_number);
	_sitout_button_name		= GetButtonName(_sitout_button_number);
	_leave_button_name		= GetButtonName(_leave_button_number);
	_prefold_button_name	= GetButtonName(_prefold_button_number);
	_autopost_button_name	= GetButtonName(_autopost_button_number);

	// hardcoded i3button
	_i3_button_name = "i3button";
	// hardcoded i3slider
	_i3_slider_name = "i3slider";
	// hardcoded i3handle
	_i3_handle_name = "i3handle";

	// hardcoded i86button
	_i86_button_name = "i86button";

	// hardcoded i86Xbuttons
	CString button_name = "";
	for (int i = 0; i < k_max_number_of_i86X_buttons; i++)
	{
		button_name.Format("i%dbutton", k_button_i86*k_max_number_of_i86X_buttons + i);
		_i86X_button_name[i] = button_name;
	}

	// Set Button States
	allin_button_visible	= GetButtonVisible(_allin_button_number);
	raise_button_visible	= GetButtonVisible(_raise_button_number);
	call_button_visible		= GetButtonVisible(_call_button_number);
	check_button_visible	= GetButtonVisible(_check_button_number);
	fold_button_visible		= GetButtonVisible(_fold_button_number);
	sitin_button_visible	= GetButtonVisible(_sitin_button_number);
	sitout_button_visible	= GetButtonVisible(_sitout_button_number);
	leave_button_visible	= GetButtonVisible(_leave_button_number);
	prefold_button_visible	= GetButtonVisible(_prefold_button_number);
	autopost_button_visible	= GetButtonVisible(_autopost_button_number);

	// hardcoded i3button - special case since i3state can be true when there is no i3button
	i3_button_visible = GetButtonVisible(k_button_i3);
	// hardcoded i86button

	i86_button_visible = GetButtonVisible(k_button_i86);
	// hardcoded i86Xbuttons
	for (int i = 0; i < k_max_number_of_i86X_buttons; i++)
		i86X_button_visible[i] = GetButtonVisible(k_button_i86*k_max_number_of_i86X_buttons + i);
}

void CScraperAccess::GetNeccessaryTablemapObjects()
{
	SetScraperAccessData();

	// Defined
	allin_button_defined	= p_tablemap_access->GetButtonRect(_allin_button_name, &allin_button);
	raise_button_defined	= p_tablemap_access->GetButtonRect(_raise_button_name, &raise_button);
	call_button_defined		= p_tablemap_access->GetButtonRect(_call_button_name, &call_button);
	check_button_defined	= p_tablemap_access->GetButtonRect(_check_button_name, &check_button);
	fold_button_defined		= p_tablemap_access->GetButtonRect(_fold_button_name, &fold_button);
	sitin_button_defined	= p_tablemap_access->GetButtonRect(_sitin_button_name, &sitin_button);
	sitout_button_defined	= p_tablemap_access->GetButtonRect(_sitout_button_name, &sitout_button);
	leave_button_defined	= p_tablemap_access->GetButtonRect(_leave_button_name, &leave_button);

	i3_button_defined		= p_tablemap_access->GetButtonRect("i3button", &i3_button);
	i3_edit_defined			= p_tablemap_access->GetTableMapRect("i3edit", &i3_edit_region);
	i3_slider_defined		= p_tablemap_access->GetTableMapRect("i3slider", &i3_slider_region);
	i3_handle_defined		= p_tablemap_access->GetTableMapRect("i3button", &i3_handle_region);

	i86_button_defined		= p_tablemap_access->GetButtonRect("i86button", &i86_button);
	CString i86X_button_name = "";
	for (int i = 0; i < k_max_number_of_i86X_buttons; i++)
	{
		i86X_button_name.Format("i86%dbutton", i);
		i86X_button_defined[i]     = p_tablemap_access->GetButtonRect(i86X_button_name, &i86X_button[i]);
	}
	
	// Available
	button_available.clear();

	allin_button_available	= allin_button_defined && allin_button_visible;
	button_available[k_button_allin] = allin_button_available;

	raise_button_available	= raise_button_defined && raise_button_visible;
	button_available[k_button_raise] = raise_button_available;

	call_button_available	= call_button_defined && call_button_visible;
	button_available[k_button_call] = call_button_available;

	check_button_available	= check_button_defined && check_button_visible;
	button_available[k_button_check] = check_button_available;

	fold_button_available	= fold_button_defined && fold_button_visible;
	button_available[k_button_fold] = fold_button_available;

	sitin_button_available	= sitin_button_defined && sitin_button_visible;
	button_available[k_button_sitin] = sitin_button_available;

	sitout_button_available	= sitout_button_defined && sitout_button_visible;
	button_available[k_button_sitout] = sitout_button_available;

	leave_button_available	= leave_button_defined && leave_button_visible;
	button_available[k_button_leave] = leave_button_available;

	i3_button_available		= i3_button_defined && i3_button_visible;
	button_available[k_button_i3] = i3_button_available;

	i86_button_available	= i86_button_defined && i86_button_visible;
	button_available[k_button_i86] = i86_button_available;

	for (int i = 0; i < k_max_number_of_i86X_buttons; i++)
	{
		i86X_button_available[i]   = i86X_button_defined[i] && i86X_button_visible[i];
	}

	for (int i = 0; i < k_max_betpot_buttons; i++)
	{
		betpot_button_defined[i]     = p_tablemap_access->GetButtonRect(k_betpot_button_name[i], &betpot_button[i]);
		betpot_button_available[i]   = betpot_button_defined[i] && betpot_button_visible[i];
	}

	allin_option_available = false;

	if (i3_button_available)
		allin_option_available = true;
	if (i3_button_visible && allin_button_available)
		allin_option_available = true;
	if (i3_button_visible && i3_edit_defined)
		allin_option_available = true;
	if (i3_button_visible && i3_slider_defined && i3_handle_defined)
		allin_option_available = true;
}

int CScraperAccess::NumberOfVisibleButtons()
{
	// Buttons for playing actions, e.g. fold or allin.
	// There have to be at least 2 to make it our turn.
	int number_of_visible_buttons = 0
		+ (allin_option_available ? 1 : 0)
		+ (raise_button_available ? 1 : 0)
		+ (call_button_available  ? 1 : 0)
		+ (check_button_available ? 1 : 0)
		+ (fold_button_available  ? 1 : 0);

	return number_of_visible_buttons;
}