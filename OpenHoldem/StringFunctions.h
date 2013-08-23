//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*****************************************************************************
//
// Purpose: Providing functions that deal with strings
//
//*****************************************************************************

#ifndef INC_STRINGFUNCTIONS_H
#define INC_STRINGFUNCTIONS_H

bool StringIsExactMatch(const char *string_a, const char *string_b);
bool StringAIsPrefixOfStringB(const char *string_a, const char *string_b);
CString Bool2CString(bool b);

// Converts any number to a CString
// * without digits for integers
// * with 9 digits precision for reals
CString Number2CString(double number);

#endif INC_STRINGFUNCTIONS_H