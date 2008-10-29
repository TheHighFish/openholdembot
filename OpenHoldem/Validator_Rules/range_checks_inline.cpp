
BEGIN_TESTCASE
    TESTCASE_ID("0001")
    REASONING("Range-check for rake.")
    PRECONDITION(true)
    POSTCONDITION((gws("rake") >= 0) && (gws("rake") <= 1.0))
    SYMBOLS_POSSIBLY_AFFECTED("rake")
END_TESTCASE