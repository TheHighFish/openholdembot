//
// (Un)known Cards
//

BEGIN_TESTCASE
    TESTCASE_ID ("0132")
    HEURISTIC_RULE (false)
    REASONING ("Range check for symbol nouts")
    PRECONDITION (true)
    POSTCONDITION ((gws("nouts") >= 0) && (gws("nouts") <= 50))
    SYMBOLS_POSSIBLY_AFFECTED ("nouts")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0133")
    HEURISTIC_RULE (false)
    REASONING ("Range check for symbol ncardsknown")
    PRECONDITION (true)
    POSTCONDITION ((gws("ncardsknown") >= 0) && (gws("ncardsknown") <= 7))
    SYMBOLS_POSSIBLY_AFFECTED ("ncardsknown")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0134")
    HEURISTIC_RULE (false)
    REASONING ("Range check for symbol ncardsunknown")
    PRECONDITION (true)
    POSTCONDITION (((gws("ncardsunknown") >= 45) && (gws("ncardsunknown") <= 52)) || (gws("ncardsunknown") == UNDEFINED_ZERO))
    SYMBOLS_POSSIBLY_AFFECTED ("ncardsunknown")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0135")
    HEURISTIC_RULE (false)
    REASONING ("Range check for symbol ncardsbetter")
    PRECONDITION (true)
    POSTCONDITION ((gws("ncardsbetter") >= 0) && (gws("ncardsbetter") <= 50))
    SYMBOLS_POSSIBLY_AFFECTED ("ncardsbetter")
END_TESTCASE