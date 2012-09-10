BEGIN_TESTCASE
    TESTCASE_ID ("1800")
    HEURISTIC_RULE (false)
    REASONING ("If I'm not aggressor, but an aggressor truly exists and nobody did act this round , then he must be the aggressor of the previous round and has to act behind me.")
    PRECONDITION ((gws("ac_aggressor") != gws("userchair")) && (gws("ac_aggressor") >= 0) && (gws("ncallbets") == 0))
    POSTCONDITION ((gws("betround") > 1) && gws("ac_agchair_after"))
    SYMBOLS_POSSIBLY_AFFECTED ("ac_aggressor , userchair, ncallbets, betround, ac_agchair_after")
END_TESTCASE
 
 
BEGIN_TESTCASE
    TESTCASE_ID ("1801")
    HEURISTIC_RULE (false)
    REASONING ("If I'm last to act and not aggressor and an aggressor truly exists, he has to act before me.")
    PRECONDITION ((gws("betposition") == gws("nplayersplaying")) && (gws("ac_aggressor") != gws("userchair")) && (gws("ac_aggressor") >= 0))
    POSTCONDITION ((gws("ac_agchair_after") == false))
    SYMBOLS_POSSIBLY_AFFECTED ("betposition, nplayersplaying, ac_aggressor, userchair, ac_agchair_after")
END_TESTCASE
 
 
BEGIN_TESTCASE
    TESTCASE_ID ("1802")
    HEURISTIC_RULE (false)
    REASONING ("If I did raise and not call and my bet is the highest one (not neccessary unique highest), then I am the aggresor.")
    PRECONDITION ((gws("didrais") || gws("didswag")) && (gws("currentbet") >= gws("currentbet0")) && (gws("currentbet") >= gws("currentbet1")) && (gws("currentbet") >= gws("currentbet2")) && (gws("currentbet") >= gws("currentbet3")) && (gws("currentbet") >= gws("currentbet4")) && (gws("currentbet") >= gws("currentbet5")) && (gws("currentbet") >= gws("currentbet6")) && (gws("currentbet") >= gws("currentbet7")) && (gws("currentbet") >= gws("currentbet8")) && (gws("currentbet") >= gws("currentbet9")))
    POSTCONDITION (gws("ac_aggressor") == gws("userchair"))
    SYMBOLS_POSSIBLY_AFFECTED ("didrais, didswag, currentbet, currentbet0, currentbet1, currentbet2, currentbet3, currentbet4, currentbet5, currentbet6, currentbet7, currentbet8, currentbet9, ac_aggressor, userchair")
END_TESTCASE
 
 
BEGIN_TESTCASE
    TESTCASE_ID ("1803")
    HEURISTIC_RULE (false)
    REASONING ("If there is a bet to call (to avoid problems with an aggressor-chair from the previous round), then I can't be the aggressor.")
    PRECONDITION (gws("nbetstocall"))
    POSTCONDITION (gws("ac_aggressor") != gws("userchair"))
    SYMBOLS_POSSIBLY_AFFECTED ("nbetstocall, ac_aggressor, userchair")
END_TESTCASE
 
 
// TODO:
//
// The player that did raise last must be the aggressor and vice versa.
// If I'm playing and the player to my right has a higher bet, then he must be the aggressor.
// If I didn't raise this round and was not the last to raise in the previous betting round, then I can't be the aggressor.
// If my bet is not the highest one  (not neccessary unique highest), then I can't be the aggressor.

