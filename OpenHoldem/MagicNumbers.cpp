#include "stdafx.h"
#include "MagicNumbers.h"
#include "assert.h"

// Function to access the name of the action constants.
// As fold is negative we can no longer use the constants 
// as indices for an array.
const char* ActionConstantNames(int action_constant)
{
	// names of action-constants for use in the autoplayer-log.
	// Formerly 4 digits (WinHoldem-style), now more sane.
	if (action_constant == k_action_undefined)
		return "UNDEFINED";
	else if (action_constant == k_action_fold)
		return "FOLD";
	else if (action_constant == k_action_check)
		return "CHECK";
	else if (action_constant == k_action_call)
		return "CALL";
	else if (action_constant == k_action_raise)
		return "RAISE";
	else if (action_constant == k_action_betsize)
		return "NL-BETSIZING";
	else if (action_constant == k_action_allin)
		return "ALLIn";
	else
	{
		assert(k_this_must_not_happen);
		return "";
	}
}

int DefaultButtonNumber(int button_code)
{
	/*
		Returns the default button number by definition
		(ignoring label overrides) 

		0 - fold button
		1 - call button
		2 - raise button 
		3 - allin button.
	*/

	// i3button
	if (button_code == k_button_i3)
		return button_code;

	if (button_code == k_button_i86)
		return button_code;

	if ((button_code >= k_button_i86 * k_max_number_of_i86X_buttons) && (button_code < k_button_i86*k_max_number_of_i86X_buttons + k_max_number_of_i86X_buttons))
		return button_code;

	int button_number = k_button_undefined;

	switch(button_code)
	{
		case k_button_fold:
			button_number = 0;
			break;
		case k_button_call:
			button_number = 1;
			break;
		case k_button_raise:
			button_number = 2;
			break;
		case k_button_allin:
			button_number = 3;
			break;
		default :
			break;
	}

	return button_number;
}