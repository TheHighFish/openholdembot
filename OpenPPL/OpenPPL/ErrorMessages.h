#ifndef INC_ERRORMESSAGES_H
#define INC_ERRORMESSAGES_H

void ErrorMessage(int error_code, CString invalid_code_snippet);

enum error_codes
{

	k_error_beep_not_supported = 0,
	k_error_specific_suits_not_supported,
	k_error_card_expression_needs_brackets,
	// Last one: this is no error code,
	// but used for creating and indexing of arrays.
	k_number_of_error_codes
};

#endif INC_ERRORMESSAGES_H