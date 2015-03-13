//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: class for string comparisons,
//   "dealer", seated", etc.
//   Stateless. therefore no instance needed
//
//******************************************************************************

#ifndef INC_CSTRINGMATCH_H
#define INC_CSTRINGMATCH_H

class CStringMatch {
 public:
	// public functions
	CStringMatch();
	~CStringMatch();
 public:
	static bool IsNumeric(const CString t);
	static bool IsStringAllin(const CString s);
	static bool IsStringRaise(const CString s);
	static bool IsStringCall(const CString s);
	static bool IsStringCheck(const CString s);
	static bool IsStringFold(const CString s);
	static bool IsStringAutopost(const CString s);
	static bool IsStringSitin(const CString s);
	static bool IsStringSitout(const CString s);
	static bool IsStringLeave(const CString s);
	static bool IsStringPrefold(const CString s);
	static bool IsStringSeated(const CString s);
	static bool IsStringActive(const CString s);
	static bool IsStringCardback(const CString s);
	static bool IsStringDealer(const CString s);
 public:
  static bool IsStringMatch(const CString s, const int button_code);
};

#endif // INC_CSTRINGMATCH_H