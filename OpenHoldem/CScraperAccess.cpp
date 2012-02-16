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

void CScraperAccess::SetScraperAccessData()
{
	// find button numbers
	_allin_button_number  = SearchForButtonNumber(k_button_allin);
	_raise_button_number  = SearchForButtonNumber(k_button_raise);
	_call_button_number   = SearchForButtonNumber(k_button_call);
	_check_button_number  = SearchForButtonNumber(k_button_check);
	_fold_button_number   = SearchForButtonNumber(k_button_fold);
	_sitin_button_number  = SearchForButtonNumber(k_button_sitin);
	_sitout_button_number = SearchForButtonNumber(k_button_sitout);
	_leave_button_number  = SearchForButtonNumber(k_button_leave);
	// _prefold_button_number = SearchForButtonNumber(k_button_prefold);
	// _autopost_button_number = SearchForButtonNumber(k_button_autopost);


	// set button names
	_allin_button_name  = GetButtonName(_allin_button_number);
	_raise_button_name  = GetButtonName(_raise_button_number);
	_call_button_name   = GetButtonName(_call_button_number);
	_check_button_name  = GetButtonName(_check_button_number);
	_fold_button_name   = GetButtonName(_fold_button_number);
	_sitin_button_name  = GetButtonName(_sitin_button_number);
	_sitout_button_name = GetButtonName(_sitout_button_number);
	_leave_button_name  = GetButtonName(_leave_button_number);
	//_prefold_button_name = GetButtonName(prefold_button_number);
	//_autopost_button_name = GetButtonName(autopost_button_number);

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
	_allin_button_visible  = GetButtonVisible(_allin_button_number);
	_raise_button_visible  = GetButtonVisible(_raise_button_number);
	_call_button_visible   = GetButtonVisible(_call_button_number);
	_check_button_visible  = GetButtonVisible(_check_button_number);
	_fold_button_visible   = GetButtonVisible(_fold_button_number);
	_sitin_button_visible  = GetButtonVisible(_sitin_button_number);
	_sitout_button_visible = GetButtonVisible(_sitout_button_number);
	_leave_button_visible  = GetButtonVisible(_leave_button_number);
	//set_prefold_button_visible(IsButtonVisible(k_button_prefold));
	//set_autopost_button_visible(IsButtonVisible(k_button_autopost));

	// hardcoded i3button - special case since i3state can be true when there is no i3button
	_i3_button_visible = GetButtonVisible(k_button_i3);
	// hardcoded i86button

	_i86_button_visible = GetButtonVisible(k_button_i86);
	// hardcoded i86Xbuttons
	for (int i = 0; i < k_max_number_of_i86X_buttons; i++)
		_i86X_button_visible[i] = GetButtonVisible(k_button_i86*k_max_number_of_i86X_buttons + i);
}