#ifndef INC_CSCRAPERACCESS_H
#define INC_CSCRAPERACCESS_H

#include <windows.h>
#include <map>

extern class CScraperAccess
{

public:
	CScraperAccess();
	~CScraperAccess();

public:
	// public functions
	void	SetScraperAccessData();
	int		GetPlayerCards(int seat_number, int first_or_second_card);
	int		GetCommonCard(int index_zero_to_four);
	bool	IsValidCard(int Card);
	void	GetNeccessaryTablemapObjects();
	int		NumberOfVisibleButtons();

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
	CString _allin_button_name;
	CString _raise_button_name;
	CString _call_button_name;
	CString _check_button_name;
	CString _fold_button_name;
	CString _sitin_button_name;
	CString _sitout_button_name;
	CString _leave_button_name;
	CString _prefold_button_name;
	CString _autopost_button_name;
	CString _i86_button_name;
	CString _i86X_button_name[k_max_number_of_i86X_buttons];
	CString _i3_button_name;
	// region names
	CString _i3_edit_name;
	CString _i3_slider_name;
	CString _i3_handle_name;

public:
	// button is visible
	bool allin_button_visible;
	bool raise_button_visible;
	bool call_button_visible;
	bool check_button_visible;
	bool fold_button_visible;
	bool sitin_button_visible;
	bool sitout_button_visible;
	bool leave_button_visible;
	bool prefold_button_visible;
	bool autopost_button_visible;
	bool i86_button_visible;
	bool i86X_button_visible[k_max_number_of_i86X_buttons];
	bool i3_button_visible;
	bool betpot_button_visible[k_max_betpot_buttons];

// tablemap regions
public:
	RECT allin_button;
	RECT raise_button;
	RECT call_button;
	RECT check_button;
	RECT fold_button;
	RECT prefold_button;
	RECT sitin_button;
	RECT sitout_button;
	RECT leave_button;
	RECT autopost_button;
	RECT i86_button;
	RECT i86X_button[k_max_number_of_i86X_buttons];
	RECT i3_button;     // Old WH-Standard; i3 is for swag
	RECT i3_edit_region;
	RECT i3_slider_region;
	RECT i3_handle_region;
	RECT betpot_button[k_max_betpot_buttons];

// regions defined
public:
	bool allin_button_defined;
	bool raise_button_defined;
	bool call_button_defined;
	bool check_button_defined;
	bool fold_button_defined;
	bool prefold_button_defined;
	bool sitin_button_defined;
	bool sitout_button_defined;
	bool leave_button_defined;
	bool autopost_button_defined;
	bool i86_button_defined;
	bool i86X_button_defined[k_max_number_of_i86X_buttons];
	bool i3_button_defined;
	bool i3_edit_defined;
	bool i3_slider_defined;
	bool i3_handle_defined;
	bool betpot_button_defined[k_max_betpot_buttons];

// regions available
public:
	bool allin_option_available;
	bool allin_button_available;
	bool raise_button_available;
	bool call_button_available;
	bool check_button_available;
	bool fold_button_available;
	bool prefold_button_available;
	bool sitin_button_available;
	bool sitout_button_available;
	bool leave_button_available;
	bool autopost_button_available;
	bool i86_button_available;
	bool i86X_button_available[k_max_number_of_i86X_buttons];
	bool i3_button_available;
	bool betpot_button_available[k_max_betpot_buttons];

	std::map<const int, bool> button_available;

public:
	bool _i86X_state[k_max_number_of_i86X_buttons];

} *p_scraper_access;

#endif // INC_CSCRAPERACCESS_H