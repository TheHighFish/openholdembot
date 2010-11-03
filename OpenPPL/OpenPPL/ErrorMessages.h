#ifndef INC_ERRORMESSAGES_H
#define INC_ERRORMESSAGES_H

void ErrorMessage(int error_code, CString invalid_code_snippet);

enum error_codes
{

	k_error_beep_not_supported = 0,
	k_error_specific_suits_not_supported,
	k_error_card_expression_needs_brackets,
	k_error_action_without_force,
	k_error_missing_code_section,
	k_error_missing_closing_bracket,
	k_error_missing_keyword_custom,
	k_error_general,
	// Last one: this is no error code,
	// but used for creating and indexing of arrays.
	k_number_of_error_codes
};

#endif INC_ERRORMESSAGES_H