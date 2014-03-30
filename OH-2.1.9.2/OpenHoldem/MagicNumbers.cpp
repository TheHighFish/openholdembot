#include "stdafx.h"
#include "MagicNumbers.h"
#include "assert.h"

// Function to access the name of the action constants.
// As fold is negative we can no longer use the constants 
// as indices for an array.
const char* ActionConstantNames(int action_constant)
{
	// 4 digit names of action-constants for use in the autoplayer-log.
	if (action_constant == k_action_undefined)
		return "UNDF";
	else if (action_constant == k_action_fold)
		return "FOLD";
	else if (action_constant == k_action_check)
		return "CHEC";
	else if (action_constant == k_action_call)
		return "CALL";
	else if (action_constant == k_action_raise)
		return "RAIS";
	else if (action_constant == k_action_swag)
		return "SWAG";
	else if (action_constant == k_action_jam)
		return "JAM";
	else if (action_constant == k_action_allin)
		return "ALLI";
	else 
		assert(k_this_must_not_happen);
}