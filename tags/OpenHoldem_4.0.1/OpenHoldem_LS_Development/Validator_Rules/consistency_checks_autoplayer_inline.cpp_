BEGIN_TESTCASE
    TESTCASE_ID ("1300")
    HEURISTIC_RULE (false)
    REASONING ("If it's my turn, I can't be sitting out.")
    PRECONDITION (gws("ismyturn"))
    POSTCONDITION (!gws("issittingout"))
    SYMBOLS_POSSIBLY_AFFECTED ("ismyturn, issittingout")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1301")
    HEURISTIC_RULE (false)
    REASONING ("If I'm sitting out, it can't be my turn.")
    PRECONDITION (gws("issittingout"))
    POSTCONDITION (!gws("ismyturn"))
    SYMBOLS_POSSIBLY_AFFECTED ("issittingout, ismyturn")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1302")
    HEURISTIC_RULE (false)
    REASONING ("If I'm sitting in, I can't be sitting out.")
    PRECONDITION (gws("issittingin"))
    POSTCONDITION (!gws("issittingout"))
    SYMBOLS_POSSIBLY_AFFECTED ("issittingin, issittingout")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1303")
    HEURISTIC_RULE (false)
    REASONING ("If I'm sitting out, I can't be sitting in.")
    PRECONDITION (gws("issittingout"))
    POSTCONDITION (!gws("issittingin"))
    SYMBOLS_POSSIBLY_AFFECTED ("issittingin, issittingout")
END_TESTCASE







