BEGIN_TESTCASE
    TESTCASE_ID ("1200")
    HEURISTIC_RULE (false)
    REASONING ("If I did raise or call or swag, I must have a non-zero bet in front of me.")
    PRECONDITION (gws("didrais") || gws("didcall") || gws("didswag"))
    POSTCONDITION (gws("currentbet") > 0)
    SYMBOLS_POSSIBLY_AFFECTED ("didrais, didcall, didswag, currentbet")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1201")
    HEURISTIC_RULE (false)
    REASONING ("If some players are betting, but I did not yet act, then there must be a positive amount to call.")
    PRECONDITION ((gws("nplayersbetting") > 0) && !(gws("didrais") || gws("didcall") || gws("didswag")))
    POSTCONDITION (gws("nbetstocall") > 0)
    SYMBOLS_POSSIBLY_AFFECTED ("nplayersbetting, didrais, didcall, didswag, nbetstocall")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1202")
    HEURISTIC_RULE (false)
    REASONING ("If I'm the aggressor, my current bet can't be zero.")
    PRECONDITION ((gws("ac_aggressor") == gws("userchair")) && (gws("nplayersplaying") >= 2))
    POSTCONDITION (gws("currentbet") > 0)
    SYMBOLS_POSSIBLY_AFFECTED ("ac_aggressor, userchair, currentbet, nplayersplaying")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1203")
    HEURISTIC_RULE (false)
    REASONING ("If I did not yet act, I can't have a bet in front of me.")
    PRECONDITION (!(gws("didrais") || gws("didcall") || gws("didswag")))
    POSTCONDITION (gws("currentbet") == 0)
    SYMBOLS_POSSIBLY_AFFECTED ("didrais, didcall, didswag, currentbet")
END_TESTCASE

/* 
DOES NOT WORK THAT WAY!  

BEGIN_TESTCASE
    TESTCASE_ID ("1204")
    HEURISTIC_RULE (false)
    REASONING ("If it's postflop, the sum of all stacks, all bets and the pot must be equal to the sum of all stacks (and all bets and at the pot) beginning of the game.")
    PRECONDITION (gws("betround") >= 2)
    POSTCONDITION ((gws("balance0") + gws("balance1") + gws("balance2") + gws("balance3") + gws("balance4") + gws("balance5") + gws("balance6") + gws("balance7") + gws("balance8") + gws("balance9") + gws("currentbet0") + gws("currentbet1") + gws("currentbet2") + gws("currentbet3") + gws("currentbet4") + gws("currentbet5") + gws("currentbet6") + gws("currentbet7") + gws("currentbet8") + gws("currentbet9") + gws("potcommon")) == (gws("hi_balance011") + gws("hi_balance11") + gws("hi_balance21") + gws("hi_balance31") + gws("hi_balance41") +gws("hi_balance51") + gws("hi_balance61") + gws("hi_balance71") + gws("hi_balance81") + gws("hi_balance91") + gws("hi_currentbet01") + gws("hi_currentbet11") + gws("hi_currentbet21") + gws("hi_currentbet31") + gws("hi_currentbet41") + gws("hi_currentbet51") + gws("hi_currentbet61") + gws("hi_currentbet71") + gws("hi_currentbet81") + gws("hi_currentbet91") + gws("hi_potcommon1")))
    SYMBOLS_POSSIBLY_AFFECTED ("balance0, balance1, balance2, balance3, balance4, balance5, balance6, balance7, balance8, balance9, currentbet0, currentbet1, currentbet2, currentbet3, currentbet4, currentbet5, currentbet6, currentbet7, currentbet8, currentbet9, potcommon, hi_balance011, hi_balance11, hi_balance21, hi_balance31, hi_balance41 +hi_balance51, hi_balance61, hi_balance71, hi_balance81, hi_balance91, hi_currentbet01, hi_currentbet11, hi_currentbet21, hi_currentbet31, hi_currentbet41, hi_currentbet51, hi_currentbet61, hi_currentbet71, hi_currentbet81, hi_currentbet91, hi_potcommon1")
END_TESTCASE
*/
 
  
BEGIN_TESTCASE
    TESTCASE_ID ("1205")
    HEURISTIC_RULE (false)
    REASONING ("If it's preflop, all players playing (or sitting in) must have a positive balance or (if they are allin) a bet in front of them. Partial rule for player 0.")
    PRECONDITION ((gws("betround") == 1) && ((int(gws("playersplayingbits")) & 0x01) == 0x01))
    POSTCONDITION ((gws("currentbet0") > 0) || (gws("balance1") > 0))
    SYMBOLS_POSSIBLY_AFFECTED ("betround, playersplayingbits, currentbet0, balance0")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1206")
    HEURISTIC_RULE (false)
    REASONING ("If it's preflop, all players playing (or sitting in) must have a positive balance or (if they are allin) a bet in front of them. Partial rule for player 1.")
    PRECONDITION ((gws("betround") == 1) && ((int(gws("playersplayingbits")) & 0x02) == 0x02))
    POSTCONDITION ((gws("currentbet1") > 0) || (gws("balance1") > 0))
    SYMBOLS_POSSIBLY_AFFECTED ("betround, playersplayingbits, currentbet1, balance1")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1207")
    HEURISTIC_RULE (false)
    REASONING ("If it's preflop, all players playing (or sitting in) must have a positive balance or (if they are allin) a bet in front of them. Partial rule for player 2.")
    PRECONDITION ((gws("betround") == 1) && ((int(gws("playersplayingbits")) & 0x04) == 0x04))
    POSTCONDITION ((gws("currentbet2") > 0) || (gws("balance2") > 0))
    SYMBOLS_POSSIBLY_AFFECTED ("betround, playersplayingbits, currentbet2, balance2")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1208")
    HEURISTIC_RULE (false)
    REASONING ("If it's preflop, all players playing (or sitting in) must have a positive balance or (if they are allin) a bet in front of them. Partial rule for player 3.")
    PRECONDITION ((gws("betround") == 1) && ((int(gws("playersplayingbits")) & 0x08) == 0x08))
    POSTCONDITION ((gws("currentbet3") > 0) || (gws("balance3") > 0))
    SYMBOLS_POSSIBLY_AFFECTED ("betround, playersplayingbits, currentbet3, balance3")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1209")
    HEURISTIC_RULE (false)
    REASONING ("If it's preflop, all players playing (or sitting in) must have a positive balance or (if they are allin) a bet in front of them. Partial rule for player 4.")
    PRECONDITION ((gws("betround") == 1) && ((int(gws("playersplayingbits")) & 0x10) == 0x10))
    POSTCONDITION ((gws("currentbet4") > 0) || (gws("balance4") > 0))
    SYMBOLS_POSSIBLY_AFFECTED ("betround, playersplayingbits, currentbet4, balance4")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1210")
    HEURISTIC_RULE (false)
    REASONING ("If it's preflop, all players playing (or sitting in) must have a positive balance or (if they are allin) a bet in front of them. Partial rule for player 5.")
    PRECONDITION ((gws("betround") == 1) && ((int(gws("playersplayingbits")) & 0x20) == 0x20))
    POSTCONDITION ((gws("currentbet5") > 0) || (gws("balance5") > 0))
    SYMBOLS_POSSIBLY_AFFECTED ("betround, playersplayingbits, currentbet5, balance5")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1211")
    HEURISTIC_RULE (false)
    REASONING ("If it's preflop, all players playing (or sitting in) must have a positive balance or (if they are allin) a bet in front of them. Partial rule for player 6.")
    PRECONDITION ((gws("betround") == 1) && ((int(gws("playersplayingbits")) & 0x40) == 0x40))
    POSTCONDITION ((gws("currentbet6") > 0) || (gws("balance6") > 0))
    SYMBOLS_POSSIBLY_AFFECTED ("betround, playersplayingbits, currentbet6, balance6")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1212")
    HEURISTIC_RULE (false)
    REASONING ("If it's preflop, all players playing (or sitting in) must have a positive balance or (if they are allin) a bet in front of them. Partial rule for player7 .")
    PRECONDITION ((gws("betround") == 1) && ((int(gws("playersplayingbits")) & 0x80) == 0x80))
    POSTCONDITION ((gws("currentbet7") > 0) || (gws("balance7") > 0))
    SYMBOLS_POSSIBLY_AFFECTED ("betround, playersplayingbits, currentbet7, balance7")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1213")
    HEURISTIC_RULE (false)
    REASONING ("If it's preflop, all players playing (or sitting in) must have a positive balance or (if they are allin) a bet in front of them. Partial rule for player 8.")
    PRECONDITION ((gws("betround") == 1) && ((int(gws("playersplayingbits")) & 0x0100) == 0x0100))
    POSTCONDITION ((gws("currentbet8") > 0) || (gws("balance8") > 0))
    SYMBOLS_POSSIBLY_AFFECTED ("betround, playersplayingbits, currentbet8, balance8")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1214")
    HEURISTIC_RULE (false)
    REASONING ("If it's preflop, all players playing (or sitting in) must have a positive balance or (if they are allin) a bet in front of them. Partial rule for player 9.")
    PRECONDITION ((gws("betround") == 1) && ((int(gws("playersplayingbits")) & 0x0200) == 0x0200))
    POSTCONDITION ((gws("currentbet9") > 0) || (gws("balance9") > 0))
    SYMBOLS_POSSIBLY_AFFECTED ("betround, playersplayingbits, currentbet9, balance9")
END_TESTCASE

/*
TODO

BEGIN_TESTCASE
    TESTCASE_ID ("1215")
    HEURISTIC_RULE (false)
    REASONING ("If everybody did act once (read: I did act twice), everybody playing must have a positive bet in front of him or he must be allin and have a stack of 0. Partial rule for player 0.")
    PRECONDITION (((gws("didcheck") + gws("didcall") + gws("didrais") + gws("didswag")) >= 2) && (TODO))
    POSTCONDITION ()
    SYMBOLS_POSSIBLY_AFFECTED ("")
END_TESTCASE
*/
 




