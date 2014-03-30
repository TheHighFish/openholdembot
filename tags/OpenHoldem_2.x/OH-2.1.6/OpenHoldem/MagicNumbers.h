#ifndef _INC_MAGICNUMBERS_H

#define _INC_MAGICNUMBERS_H

// Used for bit-calculations: 2^N
const int k_exponents[11] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

// Betting rounds
const int k_number_of_betrounds = 4;
const int k_betround_preflop	= 1;
const int k_betround_flop		= 2;
const int k_betround_turn		= 3;
const int k_betround_river		= 4;

// Number of players
const int k_max_number_of_players = 10;

// Number of (side)pots
const int k_max_number_of_pots = 10;

// Number of cards
const int k_number_of_community_cards  = 5;
const int k_number_of_cards_per_player = 2;

// Game type
const int k_gametype_unknown = -1;
const int k_gametype_NL      =  0;
const int k_gametype_PL  	 =  1;
const int k_gametype_FL      =  2;

// Flags
const int k_number_of_flags = 20;

//  File accessable?
//  (<unistd.h> is not contained in MSCVPP)
const int F_OK = 0;

// "This must not happen."
// It is better to have a named constant then to write "assert(false);".
const bool k_this_must_not_happen = false;

// Action constants for the autoplayer
enum ActionConstant
{
	k_action_undefined = 0,
	k_action_fold,
	k_action_check,
	k_action_call,
	k_action_raise,		// min-raise
	k_action_swag,		// "stated wager", i.e. using f$betsize
	k_action_allin
};

// 4 digit names of action-constants for use in the autoplayer-log.
const CString k_action_constant_names[k_action_allin+1] =
{"UNDF", "FOLD", "CHEC", "CALL", "RAIS", "SWAG", "ALLI"};

// for rank to card translation
const static char *k_card_chars = "23456789TJQKA";

// Index for current betting round in arrays
// we prefer to store values for preflop..river in 1..4
// and use index 0 for the current round.
// The old code did it differently (4 for the current round)
const int k_storage_index_for_current_round = 0;

#endif // _INC_MAGICNUMBERS_H
