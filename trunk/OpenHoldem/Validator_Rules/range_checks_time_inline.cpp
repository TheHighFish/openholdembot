//
// Time
//

BEGIN_TESTCASE
    TESTCASE_ID ("0180")
    REASONING ("Range check for symbol elapsed")
    PRECONDITION (true)
    POSTCONDITION (gws("elapsed") >= 0)
    SYMBOLS_POSSIBLY_AFFECTED ("elapsed")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0181")
    REASONING ("Range check for symbol elapsedhand")
    PRECONDITION (true)
    POSTCONDITION (gws("elapsedhand") >= 0)
    SYMBOLS_POSSIBLY_AFFECTED ("elapsedhand")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0182")
    REASONING ("Range check for symbol elapsedauto")
    PRECONDITION (true)
    POSTCONDITION (gws("elapsedauto") >= 0)
    SYMBOLS_POSSIBLY_AFFECTED ("elapsedauto")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0183")
    REASONING ("Range check for symbol elapsedtoday")
    PRECONDITION (true)
    POSTCONDITION (gws("elapsedtoday") >= 0)
    SYMBOLS_POSSIBLY_AFFECTED ("elapsedtoday")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0184")
    REASONING ("Range check for symbol elapsed1970")
    PRECONDITION (true)
    POSTCONDITION (gws("elapsed1970") >= 1166832000)
    SYMBOLS_POSSIBLY_AFFECTED ("elapsed1970")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0185")
    REASONING ("Range check for symbol clocks")
    PRECONDITION (true)
    POSTCONDITION (gws("clocks") >= 0)
    SYMBOLS_POSSIBLY_AFFECTED ("clocks")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0186")
    REASONING ("Range check for symbol nclockspersecond")
    PRECONDITION (true)
    POSTCONDITION (gws("nclockspersecond") >= 0)
    SYMBOLS_POSSIBLY_AFFECTED ("nclockspersecond")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0187")
    REASONING ("Range check for symbol ncps")
    PRECONDITION (true)
    POSTCONDITION (gws("ncps") == gws("nclockspersecond"))
    SYMBOLS_POSSIBLY_AFFECTED ("ncps")
END_TESTCASE