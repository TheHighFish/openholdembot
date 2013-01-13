//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*****************************************************************************
//
// Purpose: Providing higher-level accessors to the data
//  of the CScraper-class like "UserHasCards()".
//
//*****************************************************************************

#ifndef INC_CSCRAPERACCESS_H
#define INC_CSCRAPERACCESS_H

#include <windows.h>

extern class CScraperAccess
{

public:
	CScraperAccess();
	~CScraperAccess();

public:
	// public functions
	void	GetNeccessaryTablemapObjects();
	void	InitOnConnect();
	int		GetPlayerCards(int seat_number, int first_or_second_card);
	int		GetCommonCard(int index_zero_to_four);
	bool	IsValidCard(int Card);
	int		NumberOfVisibleButtons();
	bool	PlayerHasCards(int player);
	bool	PlayerHasKnownCards(int player);
	bool	IsKnownCard(int card);
	bool	IsCardOrCardBack(int card);
	bool	UserHasCards();
public:
	bool	IsMyTurn()	{ return (NumberOfVisibleButtons() >= k_min_buttons_needed_for_my_turn); }

public:
	bool get_betpot_button_visible(int numerator, int denominator);

private:
	// private functions
	int		SearchForButtonNumber(int button_code);
	bool	SearchForButtonVisible(int button_code);
	CString	SearchForButtonName(int button_code);
	bool	GetButtonVisible(int button_number);
	bool	GetBetpotButtonVisible(int button_number);
	CString	GetButtonName(int button_number);


private:
	// button numbers
	int _allin_button_number;
	int _raise_button_number;
	int _call_button_number;
	int _check_button_number;
	int _fold_button_number;
	int _sitin_button_number;
	int _sitout_button_number;
	int _leave_button_number;
	int _prefold_button_number;
	int _autopost_button_number;

private:
	// button names
	CString button_names[k_number_of_autoplayer_functions];

	CString _i86_button_name;
	CString _i86X_button_name[k_max_number_of_i86X_buttons];
	CString _i3_button_name;
	// region names
	CString _i3_edit_name;
	CString _i3_slider_name;
	CString _i3_handle_name;

public:
	// visible
	bool visible_buttons[k_number_of_autoplayer_functions];
	bool i3_button_visible;
	bool i86_button_visible;
	bool i86X_button_visible[k_max_number_of_i86X_buttons];
	// defined
	bool defined_buttons[k_number_of_autoplayer_functions];
	bool i3_button_defined;
	bool i3_edit_defined;
	bool i3_slider_defined;
	bool i3_handle_defined;
	bool i86_button_defined;
	bool i86X_button_defined[k_max_number_of_i86X_buttons];
	// available
	bool available_buttons[k_number_of_autoplayer_functions];
	bool i3_button_available;	
	bool i86_button_available;
	bool i86X_button_available[k_max_number_of_i86X_buttons];

public:
	//!!! probably superfluos! bool _i86X_state[k_max_number_of_i86X_buttons];
	bool allin_option_available;

} *p_scraper_access;

#endif // INC_CSCRAPERACCESS_H