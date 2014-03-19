#include "stdafx.h"
#include "CParseErrors.h"

#include "CFormulaParser.h"
#include "CTokenizer.h"

void CParseErrors::Error(CString short_message)
{
	CString message;
	message.Format("%s%s%s%s%s%i%s%s%i%s%s%s%s%s",
		"Error: ", short_message, "\n"
		"============================================================\n"
		"Function: ", CFormulaParser::CurrentFunctionName(), "\n"
		"Line absolute: ", CTokenizer::LineAbsolute(), "\n",
		"Line relative: ", CTokenizer::LineRelative(), "\n",
		"============================================================\n",
		ErroneousCodeSnippet(), "\n",
		"============================================================\n");
	MessageBox(0, message, "Parse Error", 0);
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


