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
	// public accessors for button names
	CString get_allin_button_name() { return _allin_button_name; }
	CString get_raise_button_name() { return _raise_button_name; }
	CString get_call_button_name() { return _call_button_name; }
	CString get_check_button_name() { return _check_button_name; }
	CString get_fold_button_name() { return _fold_button_name; }
	CString get_prefold_button_name() { return _prefold_button_name; }
	CString get_i3_button_name() { return _i3_button_name; }
	CString get_sitin_button_name() { return _sitin_button_name; }
	CString get_sitout_button_name() { return _sitout_button_name; }
	CString get_leave_button_name() { return _leave_button_name; }
	CString get_autopost_button_name() { return _autopost_button_name; }
	CString get_i86_button_name() { return _i86_button_name; }
	CString get_i86X_button_name(int i) { return _i86X_button_name[i]; }
public:
	// public accessors for region names
	CString get_i3_edit_name() { return _i3_edit_name; }
	CString get_i3_slider_name() { return _i3_slider_name; }
	CString get_i3_handle_name() { return _i3_handle_name; }

public:
	// public accessors for button visible
	bool get_allin_button_visible() { return _allin_button_visible; }
	bool get_raise_button_visible() { return _raise_button_visible; }
	bool get_call_button_visible() { return _call_button_visible; }
	bool get_check_button_visible() { return _check_button_visible; }
	bool get_fold_button_visible() { return _fold_button_visible; }
	bool get_prefold_button_visible() { return _prefold_button_visible; }
	bool get_i3_button_visible() { return _i3_button_visible; }
	bool get_sitin_button_visible() { return _sitin_button_visible; }
	bool get_sitout_button_visible() { return _sitout_button_visible; }
	bool get_leave_button_visible() { return _leave_button_visible; }
	bool get_autopost_button_visible() { return _autopost_button_visible; }
	bool get_i86_button_visible() { return _i86_button_visible; }
	bool get_i86X_button_visible(int i) { return _i86X_button_visible[i]; }
public:
	// public accessors for region visible
	/*
	bool get_i3_edit_visible() { return _i3_edit_visible; };
	bool get_i3_slider_visible() { return _i3_slider_visible; };
	bool get_i3_handle_visible() { return _i3_slider_visible; };
	*/

private:
	// private functions
	int SearchForButtonNumber(int button_code);
	bool GetButtonVisible(int button_number);
	bool SearchForButtonVisible(int button_code);
	CString GetButtonName(int button_number);
	CString SearchForButtonName(int button_code);

public:
	// private mutators for button names
	void set_allin_button_name(CString s) { _allin_button_name = s; }
	void set_raise_button_name(CString s) { _raise_button_name = s; }
	void set_call_button_name(CString s) { _call_button_name = s; }
	void set_check_button_name(CString s) { _check_button_name = s; }
	void set_fold_button_name(CString s) { _fold_button_name = s; }
	void set_prefold_button_name(CString s) { _prefold_button_name = s; }

	void set_sitin_button_name(CString s) { _sitin_button_name = s; }
	void set_sitout_button_name(CString s) { _sitout_button_name = s; }
	void set_leave_button_name(CString s) { _leave_button_name = s; }
	void set_autopost_button_name(CString s) { _autopost_button_name = s; }
	void set_i86X_button_name(int i, CString s) { _i86X_button_name[i] = s; }
	// hardcoded
	void set_i3_button_name() { _i3_button_name = "i3button"; }
	void set_i86_button_name() { _i86_button_name = "i86button"; }
public:
	// private mutators for region names
	void set_i3_edit_name() { _i3_edit_name = "i3edit"; }
	void set_i3_slider_name() { _i3_slider_name = "i3slider"; }
	void set_i3_handle_name() { _i3_handle_name = "i3handle"; }
public:
	// private mutators for button visible
	void set_allin_button_visible(bool b) { _allin_button_visible = b; }
	void set_raise_button_visible(bool b) { _raise_button_visible = b; }
	void set_call_button_visible(bool b) { _call_button_visible = b; }
	void set_check_button_visible(bool b) { _check_button_visible = b; }
	void set_fold_button_visible(bool b) { _fold_button_visible = b; }
	void set_prefold_button_visible(bool b) { _prefold_button_visible = b; }
	void set_i3_button_visible(bool b) { _i3_button_visible = b; }
	void set_sitin_button_visible(bool b) { _sitin_button_visible = b; }
	void set_sitout_button_visible(bool b) { _sitout_button_visible = b; }
	void set_leave_button_visible(bool b) { _leave_button_visible = b; }
	void set_autopost_button_visible(bool b) { _autopost_button_visible = b; }
	void set_i86_button_visible(bool b) { _i86_button_visible = b; }
	void set_i86X_button_visible(int i, bool b) { _i86X_button_visible[i] = b; }
public:
	// private mutators for region visible
	/*
	void set_i3_edit_visible(bool b) { _i3_edit_visible = b; }
	void set_i3_slider_visible(bool b) { _i3_slider_visible = b; }
	void set_i3_handle_visible(bool b) { _i3_handle_visible = b; }
	*/

private:
	// button names
	CString _allin_button_name;
	CString _raise_button_name;
	CString _call_button_name;
	CString _check_button_name;
	CString _fold_button_name;
	CString _prefold_button_name;
	CString _i3_button_name;
	CString _sitin_button_name;
	CString _sitout_button_name;
	CString _leave_button_name;
	CString _autopost_button_name;
	CString _i86_button_name;
	CString _i86X_button_name[k_max_number_of_i86X_buttons];
private:
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
	bool _prefold_button_visible;
	bool _i3_button_visible;
	bool _sitin_button_visible;
	bool _sitout_button_visible;
	bool _leave_button_visible;
	bool _autopost_button_visible;
	bool _i86_button_visible;
	bool _i86X_button_visible[k_max_number_of_i86X_buttons];
private:
	// region is visible
	/*
	bool _i3_edit_visible;
	bool _i3_slider_visible;
	bool _i3_handle_visible;
	*/

} *p_scraper_access;

#endif // INC_CSCRAPERACCESS_H