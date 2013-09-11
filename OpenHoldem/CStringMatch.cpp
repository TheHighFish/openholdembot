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
		if (t.Mid(i,1).FindOneOf("$0123456789,.¢ckm") == k_undefined)
			return false;

		if (t.Mid(i,1)==".")
			num_dots++;

		if (num_dots>0 && t.Mid(i,1).FindOneOf("0123456789") != -1)
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

	if (s.MakeLower().Left(5) == "allin" ||
		s.MakeLower().Left(5) == "a11in" ||
		s.MakeLower().Left(5) == "allln" ||
		s.MakeLower().Left(5) == "a111n" ||
		s.MakeLower().Left(5) == "aiiin")
	{
		return true;
	}

	return false;
}

const bool CStringMatch::IsStringRaise(CString s)
{
	// Check for bad parameters
	if (!s || s == "")
		return false;

	if (s.MakeLower().Left(5) == "raise" ||
		s.MakeLower().Left(5) == "ra1se" ||
		s.MakeLower().Left(5) == "ralse" ||
		s.MakeLower().Left(3) == "bet")
	{
		return true;
	}

	return false;
}

const bool CStringMatch::IsStringCall(CString s)
{
	// Check for bad parameters
	if (!s || s == "")
		return false;

	if (s.MakeLower().Left(4) == "call" ||
		s.MakeLower().Left(4) == "caii" ||
		s.MakeLower().Left(4) == "ca11")
	{
		return true;
	}

	return false;
}

const bool CStringMatch::IsStringCheck(CString s)
{
	// Check for bad parameters
	if (!s || s == "")
		return false;

	if (s.MakeLower().Left(5) == "check" ||
		s.MakeLower().Left(5) == "cheok")
	{
		return true;
	}

	return false;
}

const bool CStringMatch::IsStringFold(CString s)
{
	// Check for bad parameters
	if (!s || s == "")
		return false;

	if (s.MakeLower().Left(4) == "fold" ||
		s.MakeLower().Left(4) == "fo1d" ||
		s.MakeLower().Left(4) == "foid")
	{
		return true;
	}

	return false;
}

const bool CStringMatch::IsStringAutopost(CString s)
{
	// Check for bad parameters
	if (!s || s == "")
		return false;

	s.Remove(' ');
	s.Remove('-');

	if (s.MakeLower().Left(8) == "autopost" ||
		s.MakeLower().Left(8) == "aut0p0st")
	{
		return true;
	}

	return false;
}

const bool CStringMatch::IsStringSitin(CString s)
{
	// Check for bad parameters
	if (!s || s == "")
		return false;

	s.Remove(' ');
	s.Remove('-');

	if (s.MakeLower().Left(5) == "sitin" ||
		s.MakeLower().Left(5) == "s1t1n")
	{
		return true;
	}

	return false;
}

const bool CStringMatch::IsStringSitout(CString s)
{
	// Check for bad parameters
	if (!s || s == "")
		return false;

	s.Remove(' ');
	s.Remove('-');

	if (s.MakeLower().Left(6) == "sitout" ||
		s.MakeLower().Left(6) == "s1tout" ||
		s.MakeLower().Left(6) == "sit0ut" ||
		s.MakeLower().Left(6) == "s1t0ut")
	{
		return true;
	}

	return false;
}

const bool CStringMatch::IsStringLeave(CString s)
{
	// Check for bad parameters
	if (!s || s == "")
		return false;

	if (s.MakeLower().Left(5) == "leave")
	{
		return true;
	}

	return false;
}

const bool CStringMatch::IsStringPrefold(CString s)
{
	// Check for bad parameters
	if (!s || s == "")
		return false;

	if (s.MakeLower().Left(7) == "prefold")
	{
		return true;
	}

	return false;
}


const bool CStringMatch::IsStringSeated(CString s)
{
	// Check for bad parameters
	if (!s || s == "")
		return false;

	if (s.MakeLower().Left(5) == "false" ||
		s.MakeLower().Left(8) == "unseated")
	{
		return false;
	}

	else if (s.MakeLower().Left(4) == "true" ||
		s.MakeLower().Left(6) == "seated")
	{
		return true;
	}

	return false;
}

const bool CStringMatch::IsStringActive(CString s)
{
	// Check for bad parameters
	if (!s || s == "")
		return p_tablemap->activemethod() == 2 ? true : false;

	// new method: active unless pXactive returns false/inactive/out/away
	if (p_tablemap->activemethod() == 2)
	{
		if (s.MakeLower().Left(5) == "false" ||
			s.MakeLower().Left(8) == "inactive" ||
			s.MakeLower().Left(3) == "out" ||
			s.MakeLower().Left(4) == "away")
		{
			return false;
		}

		return true;
	}
	// old method: inactive unless pXactive returns true/active
	else
	{
		if (s=="")
			return false;

		if (s.MakeLower().Left(8) == "inactive")
		{
			return false;
		}

		if (s.MakeLower().Left(4) == "true" ||
			s.MakeLower().Left(6) == "active")
		{
			return true;
		}

		return false;
	}
}

const bool CStringMatch::IsStringCardback(CString s)
{
	// Check for bad parameters
	if (!s || s == "")
		return false;

	if (s.MakeLower().Left(4) == "true" ||
		s.MakeLower().Left(8) == "cardback")
	{
		return true;
	}

	return false;
}

const bool CStringMatch::IsStringDealer(CString s)
{
	// Check for bad parameters
	if (!s || s == "")
		return false;

	if (s.MakeLower().Left(4) == "true" ||
		s.MakeLower().Left(6) == "dealer")
	{
		return true;
	}

	return false;
}