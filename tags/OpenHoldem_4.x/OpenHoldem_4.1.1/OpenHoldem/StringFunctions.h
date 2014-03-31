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

#endif INC_STRINGFUNCTIONS_H