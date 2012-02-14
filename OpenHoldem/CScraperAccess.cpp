#include "stdafx.h"
#include "CScraperAccess.h"

#include "CPreferences.h"
#include "CScraper.h"
#include "CTablemapAccess.h"

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
	const bool (CScraper::*StringMatch)(CString) = NULL;

	switch (button_code)
	{
		// ALLIN
		case k_button_allin:
			StringMatch = &CScraper::IsStringAllin;
			break;
		// RAISE
		case k_button_raise:
			StringMatch = &CScraper::IsStringRaise;
			break;
		// CALL
		case k_button_call:
			StringMatch = &CScraper::IsStringCall;
			break;
		// FOLD
		case k_button_fold:
			StringMatch = &CScraper::IsStringFold;
			break;
		// CHECK
		case k_button_check:
			StringMatch = &CScraper::IsStringCheck;
			break;
		// SITIN
		case k_button_sitin:
			StringMatch = &CScraper::IsStringSitin;
			break;
		// SITOUT
		case k_button_sitout:
			StringMatch = &CScraper::IsStringSitout;
			break;
		// LEAVE
		case k_button_leave:
			StringMatch = &CScraper::IsStringLeave;
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
			if ((p_scraper->*StringMatch)(p_scraper->button_label(j)))
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

	if (button_number != k_button_undefined && 
		p_scraper->GetButtonState(button_number) && 
		p_tablemap_access->IsTableMapRectDefined(GetButtonName(button_number)))
	{
		button_visible = true;
	}

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

	if (button_number != k_button_undefined && 
		p_scraper->GetButtonState(button_number) && 
		p_tablemap_access->IsTableMapRectDefined(GetButtonName(button_number)))
	{
		button_visible = true;
	}

	return button_visible;
}

void CScraperAccess::SetScraperAccessData()
{
	// find button numbers
	int allin_button_number = SearchForButtonNumber(k_button_allin);
	int raise_button_number = SearchForButtonNumber(k_button_raise);
	int call_button_number = SearchForButtonNumber(k_button_call);
	int check_button_number = SearchForButtonNumber(k_button_check);
	int fold_button_number = SearchForButtonNumber(k_button_fold);
	int sitin_button_number = SearchForButtonNumber(k_button_sitin);
	int sitout_button_number = SearchForButtonNumber(k_button_sitout);
	int leave_button_number = SearchForButtonNumber(k_button_leave);
	// int prefold_button_number = SearchForButtonNumber(k_button_prefold);
	// int autopost_button_number = SearchForButtonNumber(k_button_autopost);


	// set button names
	set_allin_button_name(GetButtonName(allin_button_number));
	set_raise_button_name(GetButtonName(raise_button_number));
	set_call_button_name(GetButtonName(call_button_number));
	set_check_button_name(GetButtonName(check_button_number));
	set_fold_button_name(GetButtonName(fold_button_number));
	set_sitin_button_name(GetButtonName(sitin_button_number));
	set_sitout_button_name(GetButtonName(sitout_button_number));
	set_leave_button_name(GetButtonName(leave_button_number));
	//set_prefold_button_name(GetButtonName(prefold_button_number));
	//set_autopost_button_name(GetButtonName(autopost_button_number));
	// hardcoded i3button
	set_i3_button_name();
	// hardcoded i3slider
	set_i3_slider_name();
	// hardcoded i3handle
	set_i3_handle_name();
	// hardcoded i86button
	set_i86_button_name();
	// hardcoded i86Xbuttons
	for (int i = 0; i < k_max_number_of_i86X_buttons; i++)
		set_i86X_button_name(i, GetButtonName(k_button_i86*k_max_number_of_i86X_buttons + i));


	// Set Button States
	set_allin_button_visible(GetButtonVisible(allin_button_number));
	set_raise_button_visible(GetButtonVisible(raise_button_number));
	set_call_button_visible(GetButtonVisible(call_button_number));
	set_check_button_visible(GetButtonVisible(check_button_number));
	set_fold_button_visible(GetButtonVisible(fold_button_number));
	set_sitin_button_visible(GetButtonVisible(sitin_button_number));
	set_sitout_button_visible(GetButtonVisible(sitout_button_number));
	set_leave_button_visible(GetButtonVisible(leave_button_number));
	// Missing button_codes !!!
	//set_prefold_button_visible(IsButtonVisible(k_button_prefold));
	//set_autopost_button_visible(IsButtonVisible(k_button_autopost));
	// hardcoded i3button - special case since i3state can be true when there is no i3button
	set_i3_button_visible(GetButtonVisible(k_button_i3));
	// hardcoded i86button
	set_i86_button_visible(GetButtonVisible(k_button_i86));
	// hardcoded i86Xbuttons
	for (int i = 0; i < k_max_number_of_i86X_buttons; i++)
		set_i86X_button_visible(i, GetButtonVisible(k_button_i86*k_max_number_of_i86X_buttons + i));
}