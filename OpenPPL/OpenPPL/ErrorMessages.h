#ifndef INC_ERRORMESSAGES_H
#define INC_ERRORMESSAGES_H

void ErrorMessage(int error_code, CString invalid_code_snippet);

enum error_codes
{
	k_error_beep_not_supported = 0,
	k_error_invalid_card_expression,
	k_error_action_without_force,
	k_error_missing_code_section,
	k_error_missing_closing_bracket,
	k_error_missing_keyword_custom,
	k_error_missing_brackets_for_card_expression,
	k_error_missing_brackets_for_when_condition,
	k_error_invalid_character,
	k_error_too_many_open_ended_when_conditions,
	//k_error_missing_when_others_fold_force,
	// We do no longer check for "when others fold force",
	// there simply is an implicit one at the end of a code block
	// and an implicit return 0 at the end of a function.
	k_error_unknown_symbol,
	k_error_unknown_symbol_ending_with_suited,
	k_error_operator_instead_of_action,
	k_error_missing_list_number,
	k_error_invalid_list_content,
	k_error_too_many_cards_in_hand,
	k_error_general,
	// Last one: this is no error code,
	// but used for creating and indexing of arrays.
	k_number_of_error_codes
};

#endif INC_ERRORMESSAGES_H