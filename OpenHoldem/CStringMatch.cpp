//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#include "StdAfx.h"
#include "CStringMatch.h"
#include "..\CTablemap\CTablemap.h"

CStringMatch *p_string_match = NULL;

CStringMatch::CStringMatch(void) {
}

CStringMatch::~CStringMatch(void) {
}

const bool CStringMatch::IsNumeric(const CString t) { // !!! needs to be changed
  int num_dots = 0;
  int nums_after_dot = 0;
  // Check for bad parameters
  if (!t || t == "") {
    return false;
  }
	for (int i=0; i<t.GetLength(); ++i)	{
    if (CString(t[i]).FindOneOf("$0123456789,.¢ckm") == kUndefined) {
      return false;
    }
    if (t[i] == '.') {
      ++num_dots;
    }
    if (num_dots > 0 && isdigit(t[i])) {
      nums_after_dot++;
    }
	}
  if (num_dots != 0 && num_dots != 1) {
    return false;
  }
  if (num_dots > 0 && nums_after_dot != 2) {
    return false;
  }
  return true;
}

const bool CStringMatch::IsStringSeated(const CString s) {
  // Check for bad parameters
  if (!s || s == "") {
    return false;
  }
  CString s_lower_case = s;
  s_lower_case.MakeLower();
	if (s_lower_case.Left(5) == "false" || s_lower_case.Left(8) == "unseated"){
		return false;
	}	else {
		return (s_lower_case.Left(4) == "true" || s_lower_case.Left(6) == "seated");
	}
}

const bool CStringMatch::IsStringActive(const CString s) {
	// Check for bad parameters
  if (!s || s == "") {
    return false;
  }
	CString s_lower_case = s;
  s_lower_case.MakeLower();
  if (s_lower_case.Left(5) == "false"
			|| s_lower_case.Left(8) == "inactive"
			|| s_lower_case.Left(3) == "out"
			|| s_lower_case.Left(4) == "away") {
    return false;
  }
	// old method: inactive unless pXactive returns true/active
  if (s_lower_case.Left(4) == "true" || s_lower_case.Left(6) == "active") {
    return true;
  }
  assert(kThisMustNotHappen);
  return false;
}

const bool CStringMatch::IsStringCardback(const CString s) {
	// Check for bad parameters
  if (!s || s == "") {
    return false;
  }
  CString s_lower_case = s;
	s_lower_case.MakeLower();
	return (s_lower_case.Left(4) == "true" || s_lower_case.Left(8) == "cardback");
}

const bool CStringMatch::IsStringDealer(const CString s) {
  // Check for bad parameters
  if (!s || s == "") {
    return false;
  }
	CString s_lower_case = s;
  s_lower_case.MakeLower();
	return (s_lower_case.Left(4) == "true" || s_lower_case.Left(6) == "dealer");
}

const bool CStringMatch::IsStringAllin(const CString s) {
  // Check for bad parameters
  if (!s || s == "") {
    return false;
  }
  CString s_lower_case = s;
  s_lower_case.MakeLower();
  s_lower_case.Remove(' ');
  s_lower_case.Remove('-');
  s_lower_case = s_lower_case.Left(5);
  return (s_lower_case == "allin"
    || s_lower_case == "a11in"
    || s_lower_case == "allln"
    || s_lower_case == "a111n"
    || s_lower_case == "aiiin"
    || s_lower_case == "buyin"
    || s_lower_case.Left(3) == "max");
}