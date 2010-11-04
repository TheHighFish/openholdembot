#include <afxwin.h>
#include "ErrorMessages.h"
#include <assert.h>
#include <iostream> 
#include <windows.h>

static const char *error_messages[k_number_of_error_codes] =
{
	// k_error_beep_not_supported
	"Error: \"Beep\" is no valid action for OpenPPL, as\n"
	"\n"
	"* OpenHoldem is meant as a bot, not as a tool for manual play.\n"
	"* Human and bot competing for the mouse just calls for troubles.\n"
	"\n"
	"Please complete your formula so that all actions are specified.\n",

	// k_error_specific_suits_not_supported
	"Error: Specific suits are not supported by OpenPPL\n"
	"as they do not matter and are not supported by OH-script.\n"
	"\n"
	"Example:\n"
	"When hand = AcT call  force\n"
	"When hand = AT  raise force\n"
	"\n"
	"Coding this way is the old way to randomize actions\n"
	"before randomization go introduced into PPL.\n"
	"\n"
	"We are sorry, but you should change this part of your formula.\n",

	// k_error_action_without_force
	"Error: Missing keyword \"Force\".\n"
	"\n"
	"OpenPPL does not provide any default bot logic in the background,\n"
	"so all user-defined actions are final and the keyword \"Force\" is mandatory.\n",

	// k_error_missing_code_section
	"Error: Missing code section.\n"
	"\n"
	"OpenPPL does not provide any default bot logic,\n"
	"so the code sections for preflop, flop, turn and river\n"
	"are all mandatory.\n",

	// k_error_missing_closing_bracket
	"Error: Missing closing bracket.\n"
	"\n"
	"We miss a closing bracket near that location,\n"
	"but it might also be any other kind of syntax error,\n"
	"e.g. a misspelled keyword or a missing operator.\n",

	// k_error_missing_keyword_custom
	"Error: Missing keyword \"Custom\".\n"
	"\n"
	"It seems. the option settings are over.\n"
	"Now we expect \"Custom\" followed by symbol definitions and user defined code.\n"
	"But there also may be an error in the option settings.\n",

	// k_error_missing_brackets_for_card_expression
	"Error: no brackets around card expressions.\n"
	"\n"
	"Our parser is a bit greeddy.\n"
	"So lets assume, you have the following expressions:\n"
	"\n"
	"When Hand = TT And...Raise Force\n" 
	"When Board = 456 Call Force\n"
	"\n"
	"In the first case it will take the \"A\" of \"And\" as an ace,\n"
	"in the second case it will take \"6 C\" as a 6 of clubs.\n"
	"\n"
	"Therefore it is strictly required to bracket all hand- and board-expressions.\n",

	//k_error_invalid_character,
	"Error: invalid character(s).\n"
	"\n"
	"At or near the critical code location is at least one invalid character.\n"
	"LEGAL characters are:\n"
	"[A-Z]\n"
	"[a-z]\n"
	"[0-9]\n"
	"// one-line-comments followed by arbitrary characters\n"
	"/* multi-line comments */\n"
	"Spaces, tabs and new lines\n"
	"[+-*/%] operators\n"
	"[()] brackets\n"
	"[_] underscores\n"
	"[<=>] comparisions\n"
	"[.] the decimal point in numbers\n"
	"\n"
	"Please get rid of the superfluos characters.\n",

	// k_error_general
	"General syntax error.\n"	
	"\n"
	"No detailed desription available.\n"
	"Common mistakes are:\n"
	"* misspelling of keywords\n"
	"* illegal characters)\n"
	"\n"
	"Maybe you even did something very special?\n"
};

void TerminateProcessing()
{
	using namespace std;
	cout << endl << endl << "Error: Processing stopped due to syntax errors." << endl;
	cout << "Output incomplete and unusable." << endl << endl;
	exit(-1);
}

void ErrorMessage(int error_code, CString invalid_code_snippet)
{
	assert(error_code >= 0);
	assert(error_code < k_number_of_error_codes);
	CString error_explanation = error_messages[error_code];

	int error_line = 123;
	int error_column = 456;
	CString error_location;
	error_location.Format("Line %d\nColumn %d\n", error_line, error_column);
	CString separator_line = "\n============================================================\n\n";
	CString error_message = error_explanation + separator_line
		+ invalid_code_snippet + separator_line + error_location;
	MessageBox(0, error_message, "OpenPPL: Syntax Error", 0);
	TerminateProcessing();
}