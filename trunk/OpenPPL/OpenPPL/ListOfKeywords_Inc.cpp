//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*****************************************************************************
//
// Purpose:
//
//*****************************************************************************


// ListOfKeywords_Inc.cpp
//
// This file gets included as pure CPP as part of the parser definition,
// otherwise the parser would become way too large to keep it understandable
// and AFAIK we can't distribute it over multiple modules.
//
// We also add "invalid_symbol" everywhere to detect invalid characters
// not only in symbols but also where keywords are expected.

// Options
keyword_on = str_p("on") | "On" | "ON" | invalid_symbol;
keyword_off = str_p("off") | "Off" | "OFF" | invalid_symbol;

// Beginning of custom code
keyword_custom = str_p("custom") | "Custom" | "CUSTOM" | invalid_symbol;

// List extension (not standard PPL)
keyword_lists = str_p("lists") | "Lists" | "LISTS" | invalid_symbol;
keyword_list = str_p("list") | "List" | "LIST" | invalid_symbol;
keyword_new = str_p("new") | "New" | "NEW" | invalid_symbol;

// Symbol extension (not standard PPL)
keyword_symbols = str_p("symbols") | "Symbols" | "SYMBOLS" | invalid_symbol;
keyword_symbol = str_p("symbol") | "Symbol" | "SYMBOL" | invalid_symbol;
keyword_end = str_p("end") | "End" | "END" | invalid_symbol;

// Code sections
keyword_prefold = str_p("prefold") | "Prefold" | "PREFOLD" | invalid_symbol;
keyword_preflop = str_p("preflop") | "Preflop" | "PREFLOP" | invalid_symbol;
keyword_flop = str_p("flop") | "Flop" | "FLOP" | invalid_symbol;
keyword_turn = str_p("turn") | "Turn" | "TURN" | invalid_symbol;
keyword_river = str_p("river") | "River" | "RIVER" | invalid_symbol;

// When conditions
keyword_when = str_p("when") | "When" | "WHEN" | invalid_symbol;
keyword_others = str_p("others") | "Others" | "OTHERS" | invalid_symbol;

// Boolean operators
keyword_and = (str_p("and") | "And" | "AND") | invalid_symbol;
keyword_xor = (str_p("xor") | "Xor" | "XOr" | "XOR") | invalid_symbol;
keyword_or = (str_p("or") | "Or" | "OR") | invalid_symbol;			
keyword_not = (str_p("not") | "Not" | "NOT") | invalid_symbol;

// Bit operators
keyword_bitand = (str_p("bitand") | "BitAnd" | "BITAND") | invalid_symbol;
keyword_bitor = (str_p("bitor") | "BitOr" | "BITOR") | invalid_symbol;
keyword_bitxor = (str_p("bitxor") | "BitXor" | "BITXOR") | invalid_symbol;
keyword_bitnot = (str_p("bitnot") | "BitNot" | "BITNOT") | invalid_symbol;

// Hand- and board-expressions
keyword_suited = str_p("suited") | "Suited" | "SUITED" | invalid_symbol;	
keyword_board = str_p("board") | "Board" | "BOARD" | invalid_symbol;
keyword_hand = str_p("hand") | "Hand" | "HAND" | invalid_symbol;

// Actions
keyword_force = (str_p("force") | "Force" | "FORCE") | invalid_symbol;
keyword_beep = ((str_p("beep") | "Beep" | "BEEP") | invalid_symbol);
keyword_call = str_p("call") | "Call" | "CALL" | invalid_symbol;
keyword_check = str_p("check") | "Check" | "CHECK" | invalid_symbol;
keyword_play = str_p("play") | "Play" | "PLAY" | invalid_symbol;
keyword_raise = str_p("raise") | "Raise" | "RAISE" | invalid_symbol;
keyword_raisemin = str_p("raisemin") | "Raisemin" | "RaiseMin" | "RAISEMIN" | invalid_symbol;
keyword_raisehalfpot = str_p("raisehalfpot") | "Raisehalfpot" | "RaiseHalfPot" | "RAISEHALFPOT" | invalid_symbol;
keyword_raisepot = str_p("raisepot") | "Raisepot" | "RaisePot" | "RAISEPOT" | invalid_symbol;
keyword_raisemax = str_p("raisemax") | "Raisemax" | "RaiseMax" | "RAISEMAX" | invalid_symbol;
// "Allin" is no standard PPL-action,
// but it is a convenient extension for somewhat confused users.
keyword_allin = str_p("allin") | "Allin" | "ALLIN" | invalid_symbol;
keyword_fold = str_p("fold") | "Fold" | "FOLD" | invalid_symbol;
keyword_bet = str_p("bet") | "Bet" | "BET" | invalid_symbol;
keyword_betmin = str_p("betmin") | "Betmin" | "BetMin" | "BETMIN" | invalid_symbol;
keyword_bethalfpot = str_p("bethalfpot") | "Bethalfpot" | "BetHalfPot" | "BETHALFPOT" | invalid_symbol;
keyword_betpot = str_p("betpot") | "Betpot" | "BetPot" | "BETPOT" | invalid_symbol;
keyword_betmax = str_p("betmax") | "Betmax" | "BetMax" | "BETMAX" | invalid_symbol;
keyword_sitout = str_p("sitout") | "Sitout" | "SitOut" | "SITOUT" | invalid_symbol;
keyword_sitout = str_p("close")  | "Close"  | "CLOSE"  | invalid_symbol;

// Return statement (not standard PPL)
keyword_return = str_p("return") | "Return" | "RETURN" | invalid_symbol;

// Shanky-style delay
keyword_delay = str_p("delay") | "Delay" | "DELAY" | invalid_symbol;

// Boolean constants
keyword_true = str_p("true") | "True" | "TRUE" | invalid_symbol;
keyword_false = str_p("false") | "False" | "FALSE" | invalid_symbol;