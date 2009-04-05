BEGIN_TESTCASE
    TESTCASE_ID ("1100")
    HEURISTIC_RULE (false)
    REASONING ("If it's my turn postflop, my previous action could be neither allin nor fold.")
    PRECONDITION (gws("ismyturn") && (gws("betround") > 1))
    POSTCONDITION ((gws("prevaction") != 4) && (gws("prevaction") != -1))
    SYMBOLS_POSSIBLY_AFFECTED ("betround, ismyturn, prevaction")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1101")
    HEURISTIC_RULE (false)
    REASONING ("If it's my turn preflop, my previous action could not be allin.")
    PRECONDITION (gws("ismyturn") && (gws("betround") == 1))
    POSTCONDITION (gws("prevaction") != 4)
    SYMBOLS_POSSIBLY_AFFECTED ("betround, ismyturn, prevaction")
END_TESTCASE

