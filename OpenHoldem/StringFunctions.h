//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose: Providing functions that deal with strings
//
//*******************************************************************************

#ifndef INC_STRINGFUNCTIONS_H
#define INC_STRINGFUNCTIONS_H

bool StringIsExactMatch(const char *string_a, const char *string_b);
bool StringAIsPrefixOfStringB(const char *string_a, const char *string_b);
CString Bool2CString(bool b);

// Converts any number to a CString
// * without digits for integers
// * with N digits precision for reals
CString Number2CString(double number, int default_precision=2);

CString IntToBinaryString(int number, int min_digits = 1);

CString CStringRemoveLeft(CString string, int number_of_characters_to_remove);
CString CStringRemoveRight(CString string, int number_of_characters_to_remove);

// For index calculation in symbol lookups like "didcallround2"
// without having to do -'0' everywhere and without having to hard-code
// the length of the symbol (common failure on renaming).
// Former bad code:
//   else if (memcmp(name, "didcallround", 12)==0 && strlen(name)==13)	{
//		 *result = didcall(name[12]-'0');
int  DigitCharacterToNumber(char digit);
char RightCharacter(CString string, int index_from_right = 0);
int  RightDigitCharacterToNumber(CString string, int index_from_right = 0);

#endif INC_STRINGFUNCTIONS_H