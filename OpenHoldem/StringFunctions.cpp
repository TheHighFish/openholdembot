//********************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//********************************************************************************
//
// Purpose: Providing functions that deal with strings
//
//********************************************************************************

#include <stdafx.h>
#include "StringFunctions.h"

#include "assert.h"
#include "NumericalFunctions.h"

bool StringIsExactMatch(const char *string_a, const char *string_b) {
	return ((strlen(string_a) == strlen(string_b))
		&& StringAIsPrefixOfStringB(string_a, string_b));
}

bool StringAIsPrefixOfStringB(const char *string_a, const char *string_b) {
	if (strlen(string_a) > strlen(string_b)) {
		return false;
	}
	// Result of memcmp == 0 means: identical
	// See http://www.cplusplus.com/reference/clibrary/cstring/memcmp/
	return (memcmp(string_a, string_b, strlen(string_a)) == 0);
}

CString Bool2CString(bool b) {
	return (b ? "true" : "false");
}

CString Number2CString(double number, int default_precision) {
	assert(default_precision >= 0);
	CString result;
	if (IsInteger(number)) {
		result.Format("%d", int(number));
	}	else {
		CString format_string;
		format_string.Format("%%1.%if", default_precision);
		result.Format(format_string, number);
	}
	return result;
}

CString IntToBinaryString(int number, int min_digits /* = 1 */) {
  CString result;
  AssertRange(min_digits, 1, 32);
  if (number < 0) {
    result = "-";
    number = 0 - number;
  }
  result += "0b";
  CString digits; 
  do {
    if ((number % 2) == 0) {
      digits = "0" + digits;
    } else {
      digits = "1" + digits;
    }
    number /= 2;
	  --min_digits;
  } while ((number > 0) || (min_digits > 0));
  result += digits;
  return result;
};

CString CStringRemoveLeft(CString string, int number_of_characters_to_remove) {
	int length = string.GetLength();
	int number_of_character_to_keep = length - number_of_characters_to_remove;
	return string.Right(number_of_character_to_keep);
}

CString CStringRemoveRight(CString string, int number_of_characters_to_remove) {
	int length = string.GetLength();
	int number_of_character_to_keep = length - number_of_characters_to_remove;
	return string.Left(number_of_character_to_keep);
}

int  DigitCharacterToNumber(char digit) {
  if ((digit >= '0') && (digit <= '9')) {
    return (digit - '0');
  }
  // Actually we want to return kUndefined (-1)
  // but the result gets used as an index without any additional checks,
  // so we return something "fail-safe" for the error-case.
  return kUndefinedZero;
}

char RightCharacter(CString string, int index_from_right /* = 0 */) {
  assert(index_from_right >= 0);
  int index_from_left = string.GetLength() - index_from_right - 1;
  return string[index_from_left];
}

int RightDigitCharacterToNumber(CString string, int index_from_right /* = 0 */) {
  assert(index_from_right >= 0);
  return DigitCharacterToNumber(RightCharacter(string, index_from_right));
}