//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#ifndef INC_CSTRINGMATCH_H
#define INC_CSTRINGMATCH_H

#include "CSpaceOptimizedGlobalObject.h"

class CStringMatch : public CSpaceOptimizedGlobalObject {
 public:
	// public functions
	CStringMatch();
	~CStringMatch();
 public:
	const bool IsNumeric(const CString t);// !!! unused
	const bool IsStringSeated(const CString s);
	const bool IsStringActive(const CString s);
	const bool IsStringCardback(const CString s); // !!! unused
	const bool IsStringDealer(const CString s);
  const bool IsStringAllin(const CString s);
};

extern CStringMatch *p_string_match;

#endif // INC_CSTRINGMATCH_H