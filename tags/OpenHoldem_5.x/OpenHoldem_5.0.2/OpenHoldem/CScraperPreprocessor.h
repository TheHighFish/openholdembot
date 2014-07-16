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

#ifndef INC_CSCRAPERPREPROCESSOR_H
#define INC_CSCRAPERPREPROCESSOR_H

#include "string.h"

class CScraperPreprocessor
{
public:
	CScraperPreprocessor();
	~CScraperPreprocessor();
public:
	static void PreprocessMonetaryString(CString *monetary_string);
	static void PreprocessTitleString(CString *title_string);
	static void ProcessBalanceNumbersOnly(CString *balance_and_or_potential_text);
};

#endif INC_CSCRAPERPREPROCESSOR_H