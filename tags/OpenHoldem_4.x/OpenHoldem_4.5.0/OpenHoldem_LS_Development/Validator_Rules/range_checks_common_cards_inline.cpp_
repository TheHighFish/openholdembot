//
// Common Cards 
//

BEGIN_TESTCASE
    TESTCASE_ID ("00129")
    HEURISTIC_RULE (false)
    REASONING ("Range check for symbol ncommoncardspresent")
    PRECONDITION (true)
    POSTCONDITION ((gws("ncommoncardspresent") >= 0) && (gws("ncommoncardspresent") <= 5))
    SYMBOLS_POSSIBLY_AFFECTED ("ncommoncardspresent")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0130")
    HEURISTIC_RULE (false)
    REASONING ("Range check for symbol ncommoncardsknown")
    PRECONDITION (true)
    POSTCONDITION ((gws("ncommoncardsknown") >= 0) && (gws("ncommoncardsknown") <= gws("ncommoncardspresent")))
    SYMBOLS_POSSIBLY_AFFECTED ("ncommoncardsknown, ncommoncardspresent")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0131")
    HEURISTIC_RULE (false)
    REASONING ("Range check for symbol nflopc")
    PRECONDITION (true)
    POSTCONDITION (gws("nflopc") == gws("ncommoncardsknown"))
    SYMBOLS_POSSIBLY_AFFECTED ("nflopc, ncommoncardsknown")
END_TESTCASE