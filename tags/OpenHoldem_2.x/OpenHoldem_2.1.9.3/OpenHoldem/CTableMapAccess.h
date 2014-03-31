#ifndef INC_CTABLEMAPACCESS_H
#define INC_CTABLEMAPACCESS_H

#include <windows.h>

const int k_button_i3     = 3;
const int k_button_fold   = 10;
const int k_button_allin  = 11;
const int k_button_raise  = 12;
const int k_button_call   = 13;
const int k_button_check  = 14;
const int k_button_sitin  = 20;
const int k_button_sitout = 21;
const int k_button_leave  = 22;
const int k_button_i86    = 30;

const int k_number_of_i86X_buttons = 10;

extern class CTablemapAccess
{
public:
	CTablemapAccess();
	~CTablemapAccess();
public:
	bool DoesButtonExist(int button_code);
	bool GetButtonRect(int button_code, RECT *_rect);
public:
	int	GetPlayerCards(int seat_number, int first_or_second_card);
	int GetCommonCard(int index_zero_to_four);
public:
	bool GetTableMapRect(CString, RECT *_rect);
public:
	bool IsValidCard(int Card);
private:
	CString GetButtonName(int button_code);
} *p_tablemap_access;

#endif // INC_CTABLEMAPACCESS_H