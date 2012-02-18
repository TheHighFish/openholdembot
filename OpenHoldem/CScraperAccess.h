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
	void SetScraperAccessData();
	int GetPlayerCards(int seat_number, int first_or_second_card);
	int GetCommonCard(int index_zero_to_four);
	bool IsValidCard(int Card);

public:
	// public accessors for button numbers
	int get_allin_button_number()	{ return _allin_button_number; }
	int get_raise_button_number()	{ return _raise_button_number; }
	int get_call_button_number()	{ return _call_button_number; }
	int get_check_button_number()	{ return _check_button_number; }
	int get_fold_button_number()	{ return _fold_button_number; }
	int get_sitin_button_number()	{ return _sitin_button_number; }
	int get_sitout_button_number()	{ return _sitout_button_number; }
	int get_leave_button_number()	{ return _leave_button_number; }
	int prefold_button_number()		{ return _prefold_button_number; }
	int autopost_button_number()	{ return _autopost_button_number; }

public:
	// public accessors for button names
	CString get_allin_button_name()		{ return _allin_button_name; }
	CString get_raise_button_name()		{ return _raise_button_name; }
	CString get_call_button_name()		{ return _call_button_name; }
	CString get_check_button_name()		{ return _check_button_name; }
	CString get_fold_button_name()		{ return _fold_button_name; }
	CString get_sitin_button_name()		{ return _sitin_button_name; }
	CString get_sitout_button_name()	{ return _sitout_button_name; }
	CString get_leave_button_name()		{ return _leave_button_name; }
	CString get_prefold_button_name()	{ return _prefold_button_name; }
	CString get_autopost_button_name()	{ return _autopost_button_name; }
	CString get_i86_button_name()		{ return "i86button"; }
	CString get_i86X_button_name(int i)	{ return _i86X_button_name[i]; }
	CString get_i3_button_name()		{ return "i3button"; }

public:
	// public accessors for button visible
	bool get_allin_button_visible()		{ return _allin_button_visible; }
	bool get_raise_button_visible()		{ return _raise_button_visible; }
	bool get_call_button_visible()		{ return _call_button_visible; }
	bool get_check_button_visible()		{ return _check_button_visible; }
	bool get_fold_button_visible()		{ return _fold_button_visible; }
	bool get_sitin_button_visible()		{ return _sitin_button_visible; }
	bool get_sitout_button_visible()	{ return _sitout_button_visible; }
	bool get_leave_button_visible()		{ return _leave_button_visible; }
	bool get_prefold_button_visible()	{ return _prefold_button_visible; }
	bool get_autopost_button_visible()	{ return _autopost_button_visible; }
	bool get_i86_button_visible()		{ return _i86_button_visible; }
	bool get_i86X_button_visible(int i)	{ return _i86X_button_visible[i]; }
	bool get_i3_button_visible()		{ return _i3_button_visible; }

private:
	// private functions
	int		SearchForButtonNumber(int button_code);
	bool	SearchForButtonVisible(int button_code);
	CString	SearchForButtonName(int button_code);
	bool	GetButtonVisible(int button_number);
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

private:
	// button is visible
	bool _allin_button_visible;
	bool _raise_button_visible;
	bool _call_button_visible;
	bool _check_button_visible;
	bool _fold_button_visible;
	bool _sitin_button_visible;
	bool _sitout_button_visible;
	bool _leave_button_visible;
	bool _prefold_button_visible;
	bool _autopost_button_visible;
	bool _i86_button_visible;
	bool _i86X_button_visible[k_max_number_of_i86X_buttons];
	bool _i3_button_visible;

} *p_scraper_access;

#endif // INC_CSCRAPERACCESS_H