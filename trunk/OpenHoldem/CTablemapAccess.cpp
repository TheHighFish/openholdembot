#include "stdafx.h"
#include "CTableMapAccess.h"

#include "CScraper.h"
#include "../CTablemap/CTablemap.h"

CTablemapAccess *p_tablemap_access = NULL;

CTablemapAccess::CTablemapAccess()
{
}

CTablemapAccess::~CTablemapAccess()
{
}


int CTablemapAccess::GetPlayerCards(int seat_number, int first_or_second_card)
{
	return p_scraper->card_player(seat_number, first_or_second_card);
}

int CTablemapAccess::GetCommonCard(int index_zero_to_four)
{
	return p_scraper->card_common(index_zero_to_four);
}

bool CTablemapAccess::IsValidCard(int Card)
{
	if (Card >= 0 && Card < k_number_of_cards_per_deck)
		return true;

	return false;
}


int CTablemapAccess::DefaultButtonNumber(int button_code)
{
	/*
		Returns the default button number by definition
		(ignoring label overrides) 

		0 - fold button
		1 - call button
		2 - raise button 
		3 - allin button.
	*/

	int button_number = k_button_undefined;

	switch(button_code)
	{
		case k_button_fold:
			button_number = 0;
			break;
		case k_button_call:
			button_number = 1;
			break;
		case k_button_raise:
			button_number = 2;
			break;
		case k_button_allin:
			button_number = 3;
			break;
		default :
			break;
	}

	return button_number;
}


int CTablemapAccess::SearchForButtonNumber(int button_code)
{
	/*
		Searches tablemap labels for button definitions/overrides
		returns the button number if a label is defined
		or the default button number otherwise.
	*/

	if (button_code == k_button_i3)
		return 3;

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


CString CTablemapAccess::GetButtonName(int button_code)
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

bool CTablemapAccess::DoesButtonExist(int button_code)
{
	/*
		Checks if a button is visible
		i.e. available for taking an action
	*/

	bool button_exists = false;
	int button_number = k_button_undefined;

	if (button_code == 86 || button_code >= 860)
		button_number = button_code;
	else
		button_number = SearchForButtonNumber(button_code);

	if (button_number != k_button_undefined && p_scraper->GetButtonState(button_number))
		button_exists = true;

	return button_exists;
}


bool CTablemapAccess::GetButtonRect(int button_code, RECT *_rect)
{
	/*
		Finds standard button regions
		and inserts details into RECT parameter
	*/

	CString button_name = GetButtonName(button_code);
	RMapCI wanted_button = p_tablemap->r$()->find(button_name);

	if (wanted_button != p_tablemap->r$()->end())
	{
		_rect->left   = wanted_button->second.left;
		_rect->top    = wanted_button->second.top;
		_rect->right  = wanted_button->second.right;
		_rect->bottom = wanted_button->second.bottom;

		return true;
	}

	return false;
}

bool CTablemapAccess::GetTableMapRect(CString region_name, RECT *_rect)
{
	/*
		Finds special tablemap regions (i3_edit, i3_slider, i3_handle, i86X buttons)
		and inserts details into RECT parameter
	*/

	RMapCI wanted_region = p_tablemap->r$()->find(region_name);

	if (wanted_region != p_tablemap->r$()->end())
	{
		_rect->left   = wanted_region->second.left;
		_rect->top    = wanted_region->second.top;
		_rect->right  = wanted_region->second.right;
		_rect->bottom = wanted_region->second.bottom;

		return true;
	}

	return false;
}

/*
int CAutoplayer::GetR$ButtonIndices(void)
{
	int				i = 0;
	int				button_index = 0;
	CString			s = "";
	int				num_seen = 0;

	//////////////////////////////////////////////////////////
	// find ALLIN button region from scraper
	button_index = -1;
	for (i=0; i<=9 && button_index==-1; i++)
	{
		if (p_scraper->GetButtonState(i) && p_scraper->IsStringAllin(p_scraper->button_label(i)))
		{
			button_index = i;
			num_seen++;
		}
	}

	// find allin button region from table map
	s.Format("i%dbutton", button_index);
	//_alli_but = p_tablemap->r$()->find(s.GetString());

	//////////////////////////////////////////////////////////
	// find RAISE button region from scraper
	button_index = -1;
	for (i=0; i<=9 && button_index==-1; i++)
	{
		if (p_scraper->GetButtonState(i) && p_scraper->IsStringRaise(p_scraper->button_label(i)))
		{
			button_index = i;
			num_seen++;
		}
	}

	// find rais button region from table map
	s.Format("i%dbutton", button_index);
	_rais_but = p_tablemap->r$()->find(s.GetString());

	//////////////////////////////////////////////////////////
	// find CALL button region from scraper
	button_index = -1;
	for (i=0; i<=9 && button_index==-1; i++)
	{
		if (p_scraper->GetButtonState(i) && p_scraper->IsStringCall(p_scraper->button_label(i)))
		{
			button_index = i;
			num_seen++;
		}
	}

	// find call button region from table map
	s.Format("i%dbutton", button_index);
	_call_but = p_tablemap->r$()->find(s.GetString());

	//////////////////////////////////////////////////////////
	// find CHECK button region from scraper
	button_index = -1;
	for (i=0; i<=9 && button_index==-1; i++)
	{
		if (p_scraper->GetButtonState(i) && p_scraper->IsStringCheck(p_scraper->button_label(i)))
		{
			button_index = i;
			num_seen++;
		}
	}

	// find chec button region from table map
	s.Format("i%dbutton", button_index);
	_chec_but = p_tablemap->r$()->find(s.GetString());

	//////////////////////////////////////////////////////////
	// find FOLD button region from scraper
	button_index = -1;
	for (i=0; i<=9 && button_index==-1; i++)
	{
		if (p_scraper->GetButtonState(i) && p_scraper->IsStringFold(p_scraper->button_label(i)))
		{
			button_index = i;
			num_seen++;
		}
	}

	// find fold button region from table map
	s.Format("i%dbutton", button_index);
	_fold_but = p_tablemap->r$()->find(s.GetString());

	//////////////////////////////////////////////////////////
	// find AUTOPOST button region from scraper
	//
	button_index = -1;
	_autopost_state = true;
	for (i=0; i<=9 && button_index==-1; i++)
	{
		if (p_scraper->IsStringAutopost(p_scraper->button_label(i)))
		{
			_autopost_state = p_scraper->GetButtonState(i);
			button_index = i;
		}
	}

	// find autopost button region from table map
	s.Format("i%dbutton", button_index);
	_autopost_but = p_tablemap->r$()->find(s.GetString());

	//////////////////////////////////////////////////////////
	// find SITIN button region from scraper
	button_index = -1;
	_sitin_state = true;
	for (i=0; i<=9 && button_index==-1; i++)
	{
		if (p_scraper->IsStringSitin(p_scraper->button_label(i)))
		{
			_sitin_state = p_scraper->GetButtonState(i);
			button_index = i;
		}
	}

	// find sitin button region from table map
	s.Format("i%dbutton", button_index);
	_sitin_but = p_tablemap->r$()->find(s.GetString());

	//////////////////////////////////////////////////////////
	// find SITOUT button region from scraper
	button_index = -1;
	_sitout_state = false;
	for (i=0; i<=9 && button_index==-1; i++)
	{
		if (p_scraper->IsStringSitout(p_scraper->button_label(i)))
		{
			_sitout_state = p_scraper->GetButtonState(i);
			button_index = i;
		}
	}

	// find sitout button region from table map
	s.Format("i%dbutton", button_index);
	_sitout_but = p_tablemap->r$()->find(s.GetString());

	//////////////////////////////////////////////////////////
	// find LEAVE button region from scraper
	button_index = -1;
	for (i=0; i<=9 && button_index==-1; i++)
	{
		if (p_scraper->GetButtonState(i) && p_scraper->IsStringLeave(p_scraper->button_label(i)))
		{
			button_index = i;
		}
	}

	// find leave button region from table map
	s.Format("i%dbutton", button_index);
	_leave_but = p_tablemap->r$()->find(s.GetString());

	//////////////////////////////////////////////////////////
	// find PREFOLD button region from scraper
	button_index = -1;
	for (i=0; i<=9 && button_index==-1; i++)
	{
		if (p_scraper->GetButtonState(i) && p_scraper->IsStringPrefold(p_scraper->button_label(i)))
		{
			button_index = i;
		}
	}

	// find prefold button region from table map
	s.Format("i%dbutton", button_index);
	_pre_fold_but = p_tablemap->r$()->find(s.GetString());

	//////////////////////////////////////////////////////////
	// find i86 button region from scraper
	_i86_state = false;
	_i86_but = p_tablemap->r$()->end();
	if (p_scraper->GetButtonState(86))
	{
		_i86_but = p_tablemap->r$()->find("i86button");
		_i86_state = true;
	}

	//////////////////////////////////////////////////////////
	// find i86X button region from scraper
	for (i=0; i<=9; i++)
	{
		_i86X_state[i] = false;
		_i86X_but[i] = p_tablemap->r$()->end();
		if (p_scraper->GetButtonState(860+i))
		{
			s.Format("i86%dbutton", i);
			_i86X_but[i] = p_tablemap->r$()->find(s.GetString());
			_i86X_state[i] = true;
		}
	}

	return num_seen;
}
*/