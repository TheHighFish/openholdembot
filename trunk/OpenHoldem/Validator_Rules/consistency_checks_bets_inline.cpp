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



