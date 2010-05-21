#include "stdafx.h"
#include "CTableMapAccess.h"

#include "../CTablemap/CTablemap.h"

CTablemapAccess *p_tablemap_access = NULL;

CTablemapAccess::CTablemapAccess()
{
}

CTablemapAccess::~CTablemapAccess()
{
}

CString CTablemapAccess::GetButtonName(int button_code)
{
	return ""; //!!!
}

int CTablemapAccess::GetPlayerCards(int seat_number, int first_or_second_card)
{
	return 0;
}

bool CTablemapAccess::IsValidCard(int Card)
{
	return true;
}

bool CTablemapAccess::DoesButtonExist(int button_code)
{
	return true;
}


bool CTablemapAccess::GetButtonRect(int button_code, RECT *_rect)
{
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

bool CTablemapAccess::GetTableMapRect(CString, RECT *_rect)
{
	return true;
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