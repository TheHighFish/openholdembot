// ListOfKeywords_Inc.cpp
//
// This file gets included as pure CPP as part of the parser definition,
// otherwise the parser would become way too large to keep it understandable
// and AFAIK we can't distribute it over multiple modules.


// Options
keyword_on = str_p("on") | "On" | "ON";
keyword_off = str_p("off") | "Off" | "OFF";

// Beginning of custom code
keyword_custom = str_p("custom") | "Custom" | "CUSTOM";

// List extension (not standard PPL)
keyword_lists = str_p("lists") | "Lists" | "LISTS";
keyword_list = str_p("list") | "List" | "LIST";
keyword_new = str_p("new") | "New" | "NEW";

// Symbol extension (not standard PPL)
keyword_symbols = str_p("symbols") | "Symbols" | "SYMBOLS";
keyword_symbol = str_p("symbol") | "Symbol" | "SYMBOL";
keyword_end = str_p("end") | "End" | "END";

// Code sections
keyword_preflop = str_p("preflop") | "Preflop" | "PREFLOP";
keyword_flop = str_p("flop") | "Flop" | "FLOP";
keyword_turn = str_p("turn") | "Turn" | "TURN";
keyword_river = str_p("river") | "River" | "RIVER";

// When conditions
keyword_when = str_p("when") | "When" | "WHEN";
keyword_others = str_p("others") | "Others" | "OTHERS";

// Boolean operators
keyword_and = (str_p("and") | "And" | "AND");
keyword_xor = (str_p("xor") | "Xor" | "XOr" | "XOR");
keyword_or = (str_p("or") | "Or" | "OR");			
keyword_not = (str_p("not") | "Not" | "NOT"); // No print_operator() here, as we want to add extra-brackets for clarification.

// Hand- and board-expressions
keyword_suited = str_p("suited") | "Suited" | "SUITED";			
keyword_board = str_p("board") | "Board" | "BOARD";
keyword_hand = str_p("hand") | "Hand" | "HAND";

// Actions
keyword_force = (str_p("force") | "Force" | "FORCE");
keyword_beep = (str_p("beep") | "Beep" | "BEEP")[error_beep_not_supported()];
keyword_call = str_p("call") | "Call" | "CALL";
keyword_play = str_p("play") | "Play" | "PLAY";
keyword_raise = str_p("raise") | "Raise" | "RAISE";
keyword_raisemin = str_p("raisemin") | "Raisemin" | "RaiseMin" | "RAISEMIN";
keyword_raisehalfpot = str_p("raisehalfpot") | "Raisehalfpot" | "RaiseHalfPot" | "RAISEHALFPOT";
keyword_raisepot = str_p("raisepot") | "Raisepot" | "RaisePot" | "RAISEPOT";
keyword_raisemax = str_p("raisemax") | "Raisemax" | "RaiseMax" | "RAISEMAX";
// "Allin" is no standard PPL-action,
// but it is a convenient extension for somewhat confused users.
keyword_allin = str_p("allin") | "Allin" | "ALLIN";
keyword_fold = str_p("fold") | "Fold" | "FOLD";
keyword_bet = str_p("bet") | "Bet" | "BET";
keyword_betmin = str_p("betmin") | "Betmin" | "BetMin" | "BETMIN";
keyword_bethalfpot = str_p("bethalfpot") | "Bethalfpot" | "BetHalfPot" | "BETHALFPOT";
keyword_betpot = str_p("betpot") | "Betpot" | "BetPot" | "BETPOT";
keyword_betmax = str_p("betmax") | "Betmax" | "BetMax" | "BETMAX";	
keyword_sitout = str_p("sitout") | "Sitout" | "SitOut" | "SITOUT";

// Return statement (not standard PPL)
keyword_return = str_p("return") | "Return" | "RETURN";

// Boolean constants
keyword_true = str_p("true") | "True" | "TRUE";
keyword_false = str_p("false") | "False" | "FALSE";




