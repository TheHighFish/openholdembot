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
#include "CParseErrors.h"

#include "CFormulaParser.h"
#include "CTokenizer.h"
#include "OH_MessageBox.h"

bool CParseErrors::_is_error = false;

void CParseErrors::Error(CString short_message)
{
	CString message;
	message.Format("%s%s%s%s%s%i%s%s%i%s%s%s%s%s",
		"Error: ", short_message, 
		"============================================================\n"
		"Function: ", CFormulaParser::CurrentFunctionName(), "\n"
		"Line absolute: ", CTokenizer::LineAbsolute(), "\n",
		"Line relative: ", CTokenizer::LineRelative(), "\n",
		"============================================================\n",
		ErroneousCodeSnippet(), "\n",
		"============================================================\n");
	OH_MessageBox_Interactive(message, "Parse Error", 0);
}

void CParseErrors::ErrorUnknownIdentifier(CString name) {
  CString message;
  message.Format("Unknown identifier: %s", name);
  Error(message);
}

CString CParseErrors::ErroneousCodeSnippet()
{
	// We have seen functions up to several 1000 lines 
	// (insane Shanky-style) and lines up 5000 characters
	// (auto-generated code).
	// Here we want to build some code-snippet 
	// that fits into a message-box. ;-)
	const int kLinesOfCodeToShow = 5;
	const int kMaxCharactersPerLineToShow = 60;
	char* remaining_input = CTokenizer::RemainingInput();
  if (remaining_input == NULL) return "";

	CString erroneous_code_snippet;
	for (int i=0; i<kLinesOfCodeToShow; i++)
	{ 
		for (int j=0; j<kMaxCharactersPerLineToShow; j++)
		{
			char next_character = remaining_input[j];
			erroneous_code_snippet += next_character;
			if (next_character == '\n')
			{
				// End of line reached
				// Therefore terminate this line early
				break;
			}
			else if (next_character == '\0')
			{
				// End of function reached
				// Therefore terminate completely
				goto EndOfOuterLoop;
			}
		}
		erroneous_code_snippet += '\n';
		// Skip to end of line 
		char next_character;
		do
		{
			next_character = *remaining_input;
			remaining_input++;	
		}
		while ((next_character != '\n') && (next_character != '\0'));
	}
EndOfOuterLoop:
	return erroneous_code_snippet;
}

void CParseErrors::ClearErrorStatus() {
  _is_error = false;
} 

bool CParseErrors::AnyError() {
  return _is_error;
}
