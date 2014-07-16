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

#include <stdafx.h>
#include "StringFunctions.h"

#include "assert.h"
#include "NumericalFunctions.h"

bool StringIsExactMatch(const char *string_a, const char *string_b)
{
	return ((strlen(string_a) == strlen(string_b))
		&& StringAIsPrefixOfStringB(string_a, string_b));
}

bool StringAIsPrefixOfStringB(const char *string_a, const char *string_b)
{
	if (strlen(string_a) > strlen(string_b))
	{
		return false;
	}
	// Result of memcmp == 0 means: identical
	// See http://www.cplusplus.com/reference/clibrary/cstring/memcmp/
	return (memcmp(string_a, string_b, strlen(string_a)) == 0);
}

CString Bool2CString(bool b)
{
	return (b ? "true" : "false");
}

CString Number2CString(double number, int default_precision)
{
	assert(default_precision >= 0);
	CString result;
	if (IsInteger(number))
	{
		result.Format("%d", int(number));
	}
	else
	{
		CString format_string;
		format_string.Format("%%1.%if", default_precision);
		result.Format(format_string, number);
	}
	return result;
}

CString CStringRemoveLeft(CString string, int number_of_characters_to_remove)
{
	int length = string.GetLength();
	int number_of_character_to_keep = length - number_of_characters_to_remove;
	return string.Right(number_of_character_to_keep);
}

CString CStringRemoveRight(CString string, int number_of_characters_to_remove)
{
	int length = string.GetLength();
	int number_of_character_to_keep = length - number_of_characters_to_remove;
	return string.Left(number_of_character_to_keep);
}