BEGIN_TESTCASE
    TESTCASE_ID ("1100")
    HEURISTIC_RULE (false)
    REASONING ("If it's my turn, my previous action could have been neither allin nor fold.")
    PRECONDITION (gws("ismyturn"))
    POSTCONDITION ((gws("prevaction") != 4) && (gws("prevaction") != -1))
    SYMBOLS_POSSIBLY_AFFECTED ("ismyturn, prevaction")
END_TESTCASE

