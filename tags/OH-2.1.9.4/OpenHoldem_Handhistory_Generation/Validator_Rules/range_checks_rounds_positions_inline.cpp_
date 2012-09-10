
//
// Rounds / Positions
//

BEGIN_TESTCASE
    TESTCASE_ID ("0036")
    HEURISTIC_RULE (false)
    REASONING ("Range check for symbol betround")
    PRECONDITION (true)
    POSTCONDITION ((gws("betround") == 1) || (gws("betround") == 2) || (gws("betround") == 3) || (gws("betround") == 4))
    SYMBOLS_POSSIBLY_AFFECTED ("betround")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0037")
    HEURISTIC_RULE (false)
    REASONING ("Range check for symbol br")
    PRECONDITION (true)
    POSTCONDITION (gws("br") == gws("betround"))
    SYMBOLS_POSSIBLY_AFFECTED ("br, betround")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0038")
    HEURISTIC_RULE (false)
    REASONING ("Range check for symbol betposition")
    PRECONDITION (gws("nplayersdealt") > 1)
    POSTCONDITION ((gws("betposition") >= 1) && (gws("betposition") <= gws("nplayersdealt")))
    SYMBOLS_POSSIBLY_AFFECTED ("betposition, nplayersdealt")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0039")
    HEURISTIC_RULE (false)
    REASONING ("Range check for symbol dealposition")
    PRECONDITION (gws("nplayersdealt") > 1)
    POSTCONDITION ((gws("dealposition") >= 1) && (gws("dealposition") <= gws("nplayersdealt")) || (gws("dealposition") == UNDEFINED_ZERO))
    SYMBOLS_POSSIBLY_AFFECTED ("dealposition, nplayersdealt")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0040")
    HEURISTIC_RULE (false)
    REASONING ("Range check for symbol originaldealposition")
    PRECONDITION (true)
    POSTCONDITION ((gws("originaldealposition") >= 1) && (gws("originaldealposition") <= gws("nplayersdealt")))
    SYMBOLS_POSSIBLY_AFFECTED ("originaldealposition, nplayersdealt")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0041")
    HEURISTIC_RULE (false)
    REASONING ("Range check for symbol callposition")
    PRECONDITION (true)
    POSTCONDITION ((gws("callposition") >= -9) && (gws("callposition") <= 9))
    SYMBOLS_POSSIBLY_AFFECTED ("callposition")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0042")
    HEURISTIC_RULE (false)
    REASONING ("Range check for symbol seatposition")
    PRECONDITION (true)
    POSTCONDITION ((gws("seatposition") >= 0) && (gws("seatposition") <= 10))
    SYMBOLS_POSSIBLY_AFFECTED ("seatposition")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0043")
    HEURISTIC_RULE (false)
    REASONING ("Range check for symbol dealpositionrais")
    PRECONDITION (true)
    POSTCONDITION ((gws("dealpositionrais") >= 1) && (gws("dealpositionrais") <= 10))
    SYMBOLS_POSSIBLY_AFFECTED ("dealpositionrais")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0044")
    HEURISTIC_RULE (false)
    REASONING ("Range check for symbol betpositionrais")
    PRECONDITION (true)
    POSTCONDITION ((gws("betpositionrais") >= 1) && (gws("betpositionrais") <= 10))
    SYMBOLS_POSSIBLY_AFFECTED ("betpositionrais")
END_TESTCASE