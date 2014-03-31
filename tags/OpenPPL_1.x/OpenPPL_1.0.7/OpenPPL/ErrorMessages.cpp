#include <afxwin.h>
#include "ErrorMessages.h"
#include "LineCounter.h"
#include <assert.h>
#include <iostream> 
#include <windows.h>

static const char *short_error_messages[k_number_of_error_codes] =
{
	"ERROR: \"BEEP\" is no valid action for OpenPPL.",
	"ERROR: Invalid card expression.",
	"ERROR: Missing keyword \"FORCE\".",
	"ERROR: Missing code section.",
	"ERROR: Missing closing bracket.",
	"ERROR: Missing keyword \"CUSTOM\".",
	"ERROR: Missing brackets around card expressions.",
	"ERROR: Missing brackets for when-condition.",
	"ERROR: Invalid character(s).",
	"ERROR: Too many open-ended when-conditions.",
	"ERROR: Unknown symbol",
	"ERROR: Unknown symbol, containing \"Suited\".",
	"ERROR: Found operator, expecting an action.",
	"ERROR: Missing list number.",
	"ERROR: Invalid hand-list.",
	"ERROR: Too many cards in hand.",
	"ERROR: General syntax error."
};

static const char *detailed_error_messages[k_number_of_error_codes] =
{
	// k_error_beep_not_supported
	"OpenHoldem is meant as a bot, not as a tool for manual play.\n"
	"Human and bot competing for the mouse just calls for troubles.\n"
	"\n"
	"Please complete your formula so that all actions are specified.\n",

	// k_error_invalid_card_expression
	"A card expression looks e.g. like\n"
	"  * AA\n"
	"  * AK Suited\n"
	"  * ATT\n"
	"  * AcAs\n"
	"  * 3456\n",

	// k_error_action_without_force
	"OpenPPL does not provide any default bot logic in the background,\n"
	"so all user-defined actions are final and the keyword \"Force\" is mandatory.\n",

	// k_error_missing_code_section
	"OpenPPL does not provide any default bot logic,\n"
	"so the code sections for preflop, flop, turn and river\n"
	"are all mandatory.\n"
	"However: if you don't need all 4 sections\n"
	"(e.g. preflop push/fold-code or imcomplete profile)\n"
	"you should add some simple code:\n"
	"\n"
	"FLOP\n"
	"WHEN Others FOLD FORCE\n",

	// k_error_missing_closing_bracket
	"We miss a closing bracket near that location,\n"
	"but it might also be any other kind of syntax error,\n"
	"e.g. a misspelled keyword or a missing operator.\n",

	// k_error_missing_keyword_custom
	"It seems. the option settings are over.\n"
	"Now we expect \"CUSTOM\" followed by symbol definitions and user defined code.\n"
	"But there also may be an error in the option settings.\n",

	// k_error_missing_brackets_for_card_expression
	"Our parser is a bit greeddy.\n"
	"So lets assume, you have the following expressions:\n"
	"\n"
	"WHEN Hand = TT AND...RAISE Force\n" 
	"WHEN Board = 456 CALL FORCE\n"
	"\n"
	"In the first case it will take the \"A\" of \"AND\" as an ace,\n"
	"in the second case it will take \"6 C\" as a 6 of clubs.\n"
	"\n"
	"Therefore it is strictly required to bracket all hand- and board-expressions.\n"
	"E.g. \"WHEN ((Hand = TT) AND...) RAISE FORCE\".\n",

	// k_error_missing_brackets_for_when_condition
	"We are sorry, but for technical reasons\n"
	"all conditions need to be enclosed in brackets.\n"
	"\n"
	"E.g. \"WHEN (StackSize < 20) RAISEMAX FORCE\"\n"
	"\n"
	"Sole exception: WHEN Others FOLD FORCE\n",

	//k_error_invalid_character,
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
	"[<=>] comparisons\n"
	"[.] the decimal point in numbers\n"
	"\n"
	"Please get rid of the superfluous characters.\n",

	// k_error_too_many_open_ended_when_conditions
	"There is only one open-ended when-condition allowed.\n"
	"Code like\n"
	"\n"
	"WHEN A\n"
	"  WHEN B\n"
	"    WHEN C RAISEMAX FORCE\n"
	"\n"
	"is not possible with OpenPPL, as it would not be clear,\n"
	"how the conditions are nested.\n",

	// k_error_unknown_symbol
	"The translator detected an unknown symbol,\n"
	"that seems to be neither an OpenPPL-symbol\n"
	"nor an OpenHoldem symbol.\n"
	"\n"
	"Most probably it is simply misspelled.\n"
	"Please also note, that OH-script is case-sensitive\n"
	"and OpenPPL is partially case-sensitive.\n"
	"Please refer to the docu for more information.\n",

	// k_error_unknown_symbol_ending_with_suited
	"The translator detected an unknown symbol,\n"
	"that seems to be neither an OpenPPL-symbol\n"
	"nor an OpenHoldem symbol.\n"
	"\n"
	"As the symbol ends on \"SUITED\" we assume,\n"
	"that you simply forgot a space, e.g.\n"
	"\"ASUITED\" instead of \"A SUITED\".\n",

	// k_error_operator_instead_of_action
	"We expect an action here, but found an operator,\n"
	"but most likely you just forgot another pair of brackets here.\n"
	"\n"
	"For example: WHEN (Hand = AA) OR (Hand = KK) RAISEMAX FORCE\n"
	"instead of WHEN ((Hand = AA) OR (Hand = KK)) RAISEMAX FORCE\n",


	// k_error_missing_list_number
	"Every list needs a unique list number in the range [0..999]\n"
	"For example:\n"
	"\n"
	"NEW LIST 007\n"
    "    // Allin-range against crazy maniacs\n"
    "    AA KK QQ JJ TT 99\n"
    "    AKs AQs AJs ATs KQs\n"
    "    AKo AQo AJo\n" 
	"END LIST\n",

	// k_error_invalid_list_content
	"Invalid hand-list detected.\n"
	"There can be mainly two reasons:\n"
	"  * misspelled hands\n" 
	"  * missing \"END LIST\"\n",

	// k_error_too_many_cards_in_hand
	"Your hand expression seems to contain more than 2 cards.\n"
	"At most 2 of them are possible, at least one is required.\n",

	// k_error_general
	"No detailed desription available.\n"
	"Common mistakes are:\n"
	"  * misspelling of keywords\n"
	"  * illegal characters\n"
	"Maybe you even did something very special?\n"
	"\n"
	"Finding this error may be difficult.\n"
	"It could be slightly before the location that got reported.\n"
	"If all else fails, you could try to:\n"
	"  * simplify the expressions\n"
	"  * temporary comment out some code\n"
	"  * add as much brackets as possible\n"
	"\n"
	"Good luck, dear friend.\n"
};

void TerminateProcessing()
{
	using namespace std;
	cout << endl << endl << "ERROR: Processing stopped due to syntax errors." << endl;
	cout << "Output incomplete and unusable." << endl << endl;
	exit(-1);
}

bool invalid_code_snippet_starts_with_keyword_board(CString invalid_code_snippet)
{
	CString first_six_chars = invalid_code_snippet.Left(6).MakeLower();
	return ((first_six_chars == "board=") || (first_six_chars == "board "));
}


bool invalid_code_snippet_starts_with_keyword_hand(CString invalid_code_snippet)
{
	CString first_five_chars = invalid_code_snippet.Left(5).MakeLower();
	return ((first_five_chars == "hand=") || (first_five_chars == "hand "));
}


void ErrorMessage(int error_code, CString invalid_code_snippet)
{
	// TODO line and column
	// (If we ever manage to make the position operator work)
	assert(error_code >= 0);
	assert(error_code < k_number_of_error_codes);

	// The error_code may be of very general nature
	// and we don't want to make the grammar more complicated 
	// than necessary. So we check the beginning of
	// invalid_code_snippet for possible issues to create
	// better error messages here.
	if (invalid_code_snippet_starts_with_keyword_board(invalid_code_snippet))
	{
		error_code = k_error_missing_brackets_for_card_expression;
	}
	else if (invalid_code_snippet_starts_with_keyword_hand(invalid_code_snippet))
	{
		error_code = k_error_missing_brackets_for_card_expression;
	}
	CString short_error_explanation = short_error_messages[error_code];
	CString detailed_error_explanation = detailed_error_messages[error_code];

	//CString error_location;
	//error_location.Format("Line: %d\nColumn: %d\n", error_line, error_column);
	//error_location.Format("Line: [ %i ]\n\n", GetLineCount());
	CString separator_line = "\n============================================================\n\n";
	CString error_message = short_error_explanation + separator_line
		+ invalid_code_snippet + separator_line 
		+ detailed_error_explanation + separator_line;
		//+ error_location;
	MessageBox(0, error_message, "OpenPPL: Syntax Error", 0);
	TerminateProcessing();
}