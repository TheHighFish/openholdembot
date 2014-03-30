#ifndef _INC_MAGICNUMBERS_H

#define _INC_MAGICNUMBERS_H

// ATTENTION!
//   Do only use elementary data-types here.
//   Complex data-types like objects (especially CStrings)
//   will cause problems during initialization and termination,
//   if they get accessed but are not yet initialized or no longer valid.
//   http://www.maxinmontreal.com/forums/viewtopic.php?f=111&t=11724
//   (Same problem like the singletons in OpenHoldem.cpp)

// Used for bit-calculations: 2^N
const int k_exponents[11] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

// Betting rounds
// Some data-structures use an additional value for the current round.
// This is IMO (THF) superfluous and  dangerous, but lots of work
// and even more dangerous to change.
const int k_number_of_betrounds = 4;
const int k_betround_current	= 0;
const int k_betround_preflop	= 1;
const int k_betround_flop		= 2;
const int k_betround_turn		= 3;
const int k_betround_river		= 4;

// Number of players
const int k_max_number_of_players = 10;

// Number of (side)pots
const int k_max_number_of_pots = 10;

// Number of cards
const int k_number_of_community_cards  =  5;
const int k_number_of_cards_per_player =  2;
const int k_number_of_cards_per_deck   = 52;

// Suits and ranks
const int k_number_of_suits_per_deck = 4;
const int k_number_of_ranks_per_deck = 13;

// Number of buttons
const int k_max_number_of_buttons = 10;

// Game type
const int k_gametype_unknown = -1;
const int k_gametype_NL      =  0;
const int k_gametype_PL  	 =  1;
const int k_gametype_FL      =  2;

// Flags
const int k_number_of_flags = 20;

// Memory symbols
const int k_max_number_of_memory_symbols = 2048;
const int k_max_length_of_memory_symbols = 64;

//  File accessable?
//  (<unistd.h> is not contained in MSCVPP)
const int F_OK = 0;

// "This must not happen."
// It is better to have a named constant then to write "assert(false);".
const bool k_this_must_not_happen = false;

// Action constants for the autoplayer
// have to be backward compatible to WinHoldem
// http://www.maxinmontreal.com/wiki/index.php5?title=OpenHoldem:EndUserDocumentation:Symbols#History
// (-1=fold 0=chec 1=call 2=rais 3=swag 4=alli)
enum ActionConstant
{
	k_action_undefined = -2,
	k_action_fold = -1,
	k_action_check = 0,		// new in OpenHoldem
	k_action_call = 1,
	k_action_raise = 2,		// min-raise
	k_action_swag = 3,		// "stated wager", i.e. using f$betsize
	k_action_allin = 4,
	k_action_jam = 5        // not really an action, but required for correctly logging the slider.
};

// Function to access the name of the action constants.
// As fold is negative we can no longer use the constants 
// as indices for an array.
const char* ActionConstantNames(int action_constant);

// for rank to card translation
const static char *k_card_chars = "23456789TJQKA";

// Index for current betting round in arrays
// we prefer to store values for preflop..river in 1..4
// and use index 0 for the current round.
// The old code did it differently (4 for the current round)
const int k_storage_index_for_current_round = 0;

// Functions names, both user-defined and standard, e.g. "f$alli"
const int k_max_size_of_function_name = 256;

#endif // _INC_MAGICNUMBERS_H
