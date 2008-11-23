//
// Table map symbols
//

BEGIN_TESTCASE
    TESTCASE_ID ("0009")
    HEURISTIC_RULE (false)
    REASONING ("Range check for symbol swagdelay")
    PRECONDITION (true)
    POSTCONDITION (gws("swagdelay") >= 0)
    SYMBOLS_POSSIBLY_AFFECTED ("swagdelay")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0010")
    HEURISTIC_RULE (false)
    REASONING ("Range check for symbol allidelay")
    PRECONDITION (true)
    POSTCONDITION (gws("allidelay") >= 0)
    SYMBOLS_POSSIBLY_AFFECTED ("allidelay")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0011")
    HEURISTIC_RULE (false)
    REASONING ("Range check for symbol swagtextmethod")
    PRECONDITION (true)
    POSTCONDITION ((gws("swagtextmethod") == 1) || (gws("swagtextmethod") == 2) || (gws("swagtextmethod") == 3))
    SYMBOLS_POSSIBLY_AFFECTED ("swagtextmethod")
END_TESTCASE