//
// List Tests 78..79
//   The other list test are trivial
//

BEGIN_TESTCASE
    TESTCASE_ID ("0078")
    HEURISTIC_RULE (false)
    REASONING ("Range check for symbol nlistmax")
    PRECONDITION (true)
    POSTCONDITION ((gws("nlistmax") >= 0) || (gws("nlistmax") == UNDEFINED_NEGATIVE_ONE))
    SYMBOLS_POSSIBLY_AFFECTED ("nlistmax")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0079")
    HEURISTIC_RULE (false)
    REASONING ("Range check for symbol nlistmin ")
    PRECONDITION (true)
    POSTCONDITION (gws("nlistmin") >= 0 || (gws("nlistmin") == UNDEFINED_NEGATIVE_ONE))
    SYMBOLS_POSSIBLY_AFFECTED ("nlistmin ")
END_TESTCASE