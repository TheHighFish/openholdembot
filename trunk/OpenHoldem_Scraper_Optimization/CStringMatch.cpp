//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose:
//
//***************************************************************************** 

#include "StdAfx.h"
#include "CStringMatch.h"
#include "..\CTablemap\CTablemap.h"


CStringMatch *p_string_match = NULL;

CStringMatch::CStringMatch(void)
{
}

CStringMatch::~CStringMatch(void)
{
}

const bool CStringMatch::IsNumeric(CString t)
{
	int num_dots = 0, nums_after_dot = 0;

	// Check for bad parameters
	if (!t || t == "")
		return false;

	for (int i=0; i<t.GetLength(); i++)
	{
		if (CString(t[i]).FindOneOf("$0123456789,.¢ckm") == k_undefined)
			return false;

		if (t[i]=='.')
			num_dots++;

		if (num_dots>0 && isdigit(t[i]))
			nums_after_dot++;
	}

	if (num_dots!=0 && num_dots!=1)
		return false;

	if (num_dots>0 && nums_after_dot!=2)
		return false;

	return true;
}

const bool CStringMatch::IsStringAllin(CString s)
{
	// Check for bad parameters
	if (!s || s == "")
		return false;

	s.Remove(' ');
	s.Remove('-');
	CString s_lower_case = s.MakeLower();
	s = s_lower_case.Left(5);
	return (s == "allin"
		|| s == "a11in"
		|| s == "allln"
		|| s == "a111n"
		|| s == "aiiin");
}

const bool CStringMatch::IsStringRaise(CString s)
{
	// Check for bad parameters
	if (!s || s == "")
		return false;

	CString s_lower_case = s.MakeLower();
	s = s_lower_case.Left(5);
	return (s == "raise"
		|| s == "ra1se"
		|| s == "ralse"
		|| s.Left(3) == "bet");
}

const bool CStringMatch::IsStringCall(CString s)
{
	// Check for bad parameters
	if (!s || s == "")
		return false;

	CString s_lower_case = s.MakeLower();
	s = s_lower_case.Left(4);
	return (s == "call" || s == "caii" || s == "ca11");
}

const bool CStringMatch::IsStringCheck(CString s)
{
	// Check for bad parameters
	if (!s || s == "")
		return false;
	CString s_lower_case = s.MakeLower();
	s = s_lower_case.Left(5);

	return (s == "check" || s == "cheok");
}

const bool CStringMatch::IsStringFold(CString s)
{
	// Check for bad parameters
	if (!s || s == "")
		return false;

	CString s_lower_case = s.MakeLower();
	s = s_lower_case.Left(4);
	return (s == "fold" || s == "fo1d" || s == "foid");
}

const bool CStringMatch::IsStringAutopost(CString s)
{
	// Check for bad parameters
	if (!s || s == "")
		return false;

	s.Remove(' ');
	s.Remove('-');

	CString s_lower_case = s.MakeLower();
	s = s_lower_case.Left(8);

	return (s == "autopost" || s == "aut0p0st");
}

const bool CStringMatch::IsStringSitin(CString s)
{
	// Check for bad parameters
	if (!s || s == "")
		return false;

	s.Remove(' ');
	s.Remove('-');
	CString s_lower_case = s.MakeLower();
	s = s_lower_case.Left(5);
	return (s == "sitin" ||	s == "s1t1n");
}

const bool CStringMatch::IsStringSitout(CString s)
{
	// Check for bad parameters
	if (!s || s == "")
		return false;

	s.Remove(' ');
	s.Remove('-');

	CString s_lower_case = s.MakeLower();
	s = s_lower_case.Left(6);
	return (s == "sitout" || s == "s1tout" || s == "sit0ut" || s == "s1t0ut");
}

const bool CStringMatch::IsStringLeave(CString s)
{
	// Check for bad parameters
	if (!s || s == "")
		return false;

	return (s.MakeLower().Left(5) == "leave");
}

const bool CStringMatch::IsStringPrefold(CString s)
{
	// Check for bad parameters
	if (!s || s == "")
		return false;

	return (s.MakeLower().Left(7) == "prefold");
}


const bool CStringMatch::IsStringSeated(CString s)
{
	// Check for bad parameters
	if (!s || s == "")
		return false;

	CString s_lower_case = s.MakeLower();
	if (s_lower_case.Left(5) == "false" || s_lower_case.Left(8) == "unseated")
	{
		return false;
	}

	else 
	{
		return (s_lower_case.Left(4) == "true" || s_lower_case.Left(6) == "seated");
	}
}

const bool CStringMatch::IsStringActive(CString s)
{
	// Check for bad parameters
	if (!s || s == "")
		return p_tablemap->activemethod() == 2 ? true : false;

	CString s_lower_case = s.MakeLower();
	s = s_lower_case;
	// new method: active unless pXactive returns false/inactive/out/away
	if (p_tablemap->activemethod() == 2)
	{
		return (!(s.Left(5) == "false" 
			|| s.Left(8) == "inactive"
			|| s.Left(3) == "out" 
			|| s.Left(4) == "away"));
	}
	// old method: inactive unless pXactive returns true/active
	else
	{
		if (s == "")
			return false;

		if (s.Left(8) == "inactive")
		{
			return false;
		}

		return (s.Left(4) == "true" 
			|| s.Left(6) == "active");
	}
}

const bool CStringMatch::IsStringCardback(CString s)
{
	// Check for bad parameters
	if (!s || s == "")
		return false;

	CString s_lower_case = s.MakeLower();
	s = s_lower_case;
	return (s.Left(4) == "true" || s.Left(8) == "cardback");
}

const bool CStringMatch::IsStringDealer(CString s)
{
	// Check for bad parameters
	if (!s || s == "")
		return false;

	CString s_lower_case = s.MakeLower();
	s = s_lower_case;

	return (s.Left(4) == "true" || s.Left(6) == "dealer");
}