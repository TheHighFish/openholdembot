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

#ifndef INC_CSTRINGMATCH_H
#define INC_CSTRINGMATCH_H

extern class CStringMatch
{
public:
	// public functions
	CStringMatch();
	~CStringMatch();
public:
	const bool IsNumeric(const CString t);
	const bool IsStringAllin(const CString s);
	const bool IsStringRaise(const CString s);
	const bool IsStringCall(const CString s);
	const bool IsStringCheck(const CString s);
	const bool IsStringFold(const CString s);
	const bool IsStringAutopost(const CString s);
	const bool IsStringSitin(const CString s);
	const bool IsStringSitout(const CString s);
	const bool IsStringLeave(const CString s);
	const bool IsStringPrefold(const CString s);
	const bool IsStringSeated(const CString s);
	const bool IsStringActive(const CString s);
	const bool IsStringCardback(const CString s);
	const bool IsStringDealer(const CString s);

} *p_string_match;

#endif // INC_CSTRINGMATCH_H