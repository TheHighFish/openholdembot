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

#include "stdafx.h"
#include "CScraperPreprocessor.h"
#include "CPreferences.h"
#include "../CTablemap/CTablemap.h"

CScraperPreprocessor::CScraperPreprocessor() {
}

CScraperPreprocessor::~CScraperPreprocessor() {
}

inline bool IsCurrency(char c) {
	return (c == '$' || c == '€' || c == '£' || c == '¢'); 
}

inline char TranslateCurrency(char c) {
	if (c == '€') return '$';
  else if (c == '£') return '$';
  else if (c == '¢') return 'c';
  else return c;
}

inline bool IsStandardASCII(char c) {
  return ((c >= 0) && (c <= 0x7F));
}

inline bool SaveIsDigit(char c) {
  // Characters from the extended ASCII-set >= 0x80 (non-english currencies)
  // could cause assertions for (signed) chars inside isdigit()
  // http://www.maxinmontreal.com/forums/viewtopic.php?f=110&t=17579&start=90#p122315
  if (!IsStandardASCII(c)) return false;
  return isdigit(c);
}

void CScraperPreprocessor::PreprocessMonetaryString(CString *monetary_string)
{
	// 1) Commas in numbers: "$3,45" should be raplaced by "$3.45".
	//    And "$3,456.78" should be replaced by "$3.456.78"
	// 2) Dot as delimter-symbol in numbers: "$3.456.78". 
	//    Dots followed by three (or more) digits should be removed: "$3456.78"
	// 3) Spaces inside numbers: somebody reported blinds like "$40 000" in MTTs. 
	//    They also should be removed.
	// 4) Foreign-language money-signs like Euro € should be replaced by dollars.

	int length = monetary_string->GetLength();
	CString result;

	// A small technical trick: append 3 spaces, 
	// so we don't have to check for index errors if we investigate the next 3 characters
	*monetary_string += "   ";

	// Special handling required for the first character
	char first_character  = monetary_string->GetAt(0);
	char second_character = monetary_string->GetAt(1);
	if (IsCurrency(first_character)) {
		result += TranslateCurrency(first_character);
	} else {
    result += first_character;
  }

	// Now caring about the rest, starting from 2nd character
	int second_position = 1;
	int last_position = length - 1;
	char ith_character      = ' ';
	char previous_character = ' ';
	char next_character     = ' '; 

	for (int i=second_position; i<=last_position; ++i)
	{
		ith_character  = monetary_string->GetAt(i);
		next_character = monetary_string->GetAt(i+1);
		if (IsCurrency(ith_character))
		{
			// Keep them at the moment, but replace foreign currencys by dollars.
			// The scraper will deal with them later
			result += TranslateCurrency(ith_character);
		}
		else if ((ith_character == '.') || (ith_character == ','))
		{
			// Accept commans and dots outside numbers
			if (!SaveIsDigit(previous_character) || !SaveIsDigit(next_character))
			{
				result += ith_character;
			}
			else
			{
				// Comma or dot inside number
				// If only the following 2 digits are numbers then we have a fractional-dot.
				// But if the 3rd digit is a number too then it is only a delimiter-dot
				// (for better readability) that has to be ignored.
				char third_next_character = monetary_string->GetAt(i+3);
				if (!SaveIsDigit(third_next_character))
				{
					// Append the fractional-dot (implicitly repalcing comma if necessary)
					result += '.';
				}
			}
		}
		else if (ith_character == ' ')
		{
			// Spaces inside numbers have to be ignored
			// Up to now we saw it only for very large blinds of SNGs / MTTs,
			// for example "$40 000".
			if (SaveIsDigit(previous_character) 
				&& next_character == '0'
				&& monetary_string->GetAt(i+2) == '0'
				&& monetary_string->GetAt(i+3) == '0')
			{
				// Ignore it
			}
			else
			{
				result += ' ';
			}
		}
		else
		{
			// Everything else
			// Keep it
			result += ith_character;
		}
		previous_character = ith_character;
	}
	*monetary_string = result;

	write_log(preferences.debug_scraper_preprocessor(), "Monetary string after preprocessing: %s\n", result);
}

void CScraperPreprocessor::PreprocessTitleString(CString *title_string)
{
	// Logic seems to be exactly the same for both cases
	PreprocessMonetaryString(title_string);
}

// This function removes superfluous charaters from single numbers only.
// Example: "Singlemalt raised to $60"
void CScraperPreprocessor::ProcessBalanceNumbersOnly(CString *balance_and_or_potential_text)
{
	if (p_tablemap->balancenumbersonly()) {
		for (int i=0; i<balance_and_or_potential_text->GetLength(); ++i)		{
      char next_character = balance_and_or_potential_text->GetAt(i);
			if (SaveIsDigit(next_character)) {
				continue;
			}
			// Remove (more efficiently: replace it)
			// * comma
			// * dollar-sign
			// * a..z, A..Z
			// * minus-sign
			// * brackets
			if (isalpha(next_character) 
				  || (next_character == '$')
				  || (next_character == ',')
				  || (next_character == '-')
				  || (next_character == '(')
				  || (next_character == ')')) {
				// Replace by space to be removed later
				balance_and_or_potential_text->SetAt(i, ' ');
			}
		}
		// Now remove all bad characters at once
		balance_and_or_potential_text->Remove(' ');
	}
}