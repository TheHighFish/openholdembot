BEGIN_TESTCASE
    TESTCASE_ID ("0900")
    HEURISTIC_RULE (false)
    REASONING ("There can't be more players at the turn than at the flop.")
    PRECONDITION (true)
    POSTCONDITION (gws("floppct") >= gws("turnpct"))
    SYMBOLS_POSSIBLY_AFFECTED ("floppct, turnpct")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0901")
    HEURISTIC_RULE (false)
    REASONING ("There can't be more players at the river than at the turn.")
    PRECONDITION (true)
    POSTCONDITION (gws("turnpct") <= gws("riverpct"))
    SYMBOLS_POSSIBLY_AFFECTED ("turnpct, riverpct")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0902")
    HEURISTIC_RULE (false)
    REASONING ("My highest balance can't be greater than my actual balance.")
    PRECONDITION (true)
    POSTCONDITION (gws("maxbalance") >= gws("balance"))
    SYMBOLS_POSSIBLY_AFFECTED ("maxbalance, balance")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0903")
    HEURISTIC_RULE (false)
    REASONING ("If it's a FL game, the there must be at least 1 bet preflop and at most 4 on average.")
    PRECONDITION (true)
    POSTCONDITION ((gws("avgbetspf") >= 1) &&(gws("avgbetspf") <= 4)) 
    SYMBOLS_POSSIBLY_AFFECTED ("avgbetspf")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0904")
    HEURISTIC_RULE (true)
    REASONING ("Check for numer of hands played, under the assumption that a hand can last at most 2 minutes on average.")
    PRECONDITION (gws("elapsed") >= 120)
    POSTCONDITION ((gws("handsplayed") *120 ) >= gws("elapsed") )
    SYMBOLS_POSSIBLY_AFFECTED ("handsplayed, elapsed")
END_TESTCASE



