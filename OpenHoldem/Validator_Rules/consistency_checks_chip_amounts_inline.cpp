BEGIN_TESTCASE
    TESTCASE_ID ("1500")
    HEURISTIC_RULE (false)
    REASONING ("If it's a FL game and a player had more than 12 BB preflop, then he can never be allin during that hand. Partial rule for player 0.")
    PRECONDITION (gws("isfl") && (gws("hi_balance01") > 12))
    POSTCONDITION (gws("balance0") > 0)
    SYMBOLS_POSSIBLY_AFFECTED ("isfl, hi_balance01, balance0")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1501")
    HEURISTIC_RULE (false)
    REASONING ("If it's a FL game and a player had more than 12 BB preflop, then he can never be allin during that hand. Partial rule for player 1.")
    PRECONDITION (gws("isfl") && (gws("hi_balance11") > 12))
    POSTCONDITION (gws("balance1") > 0)
    SYMBOLS_POSSIBLY_AFFECTED ("isfl, hi_balance11, balance1")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1502")
    HEURISTIC_RULE (false)
    REASONING ("If it's a FL game and a player had more than 12 BB preflop, then he can never be allin during that hand. Partial rule for player 2.")
    PRECONDITION (gws("isfl") && (gws("hi_balance21") > 12))
    POSTCONDITION (gws("balance2") > 0)
    SYMBOLS_POSSIBLY_AFFECTED ("isfl, hi_balance21, balance2")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1503")
    HEURISTIC_RULE (false)
    REASONING ("If it's a FL game and a player had more than 12 BB preflop, then he can never be allin during that hand. Partial rule for player 3.")
    PRECONDITION (gws("isfl") && (gws("hi_balance31") > 12))
    POSTCONDITION (gws("balance3") > 0)
    SYMBOLS_POSSIBLY_AFFECTED ("isfl, hi_balance31, balance3")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1504")
    HEURISTIC_RULE (false)
    REASONING ("If it's a FL game and a player had more than 12 BB preflop, then he can never be allin during that hand. Partial rule for player 4.")
    PRECONDITION (gws("isfl") && (gws("hi_balance41") > 12))
    POSTCONDITION (gws("balance4") > 0)
    SYMBOLS_POSSIBLY_AFFECTED ("isfl, hi_balance41, balance4")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1505")
    HEURISTIC_RULE (false)
    REASONING ("If it's a FL game and a player had more than 12 BB preflop, then he can never be allin during that hand. Partial rule for player 5.")
    PRECONDITION (gws("isfl") && (gws("hi_balance51") > 12))
    POSTCONDITION (gws("balance5") > 0)
    SYMBOLS_POSSIBLY_AFFECTED ("isfl, hi_balance51, balance5")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1506")
    HEURISTIC_RULE (false)
    REASONING ("If it's a FL game and a player had more than 12 BB preflop, then he can never be allin during that hand. Partial rule for player 6.")
    PRECONDITION (gws("isfl") && (gws("hi_balance61") > 12))
    POSTCONDITION (gws("balance6") > 0)
    SYMBOLS_POSSIBLY_AFFECTED ("isfl, hi_balance61, balance6")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1507")
    HEURISTIC_RULE (false)
    REASONING ("If it's a FL game and a player had more than 12 BB preflop, then he can never be allin during that hand. Partial rule for player 7.")
    PRECONDITION (gws("isfl") && (gws("hi_balance71") > 12))
    POSTCONDITION (gws("balance7") > 0)
    SYMBOLS_POSSIBLY_AFFECTED ("isfl, hi_balance71, balance7")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1508")
    HEURISTIC_RULE (false)
    REASONING ("If it's a FL game and a player had more than 12 BB preflop, then he can never be allin during that hand. Partial rule for player 8.")
    PRECONDITION (gws("isfl") && (gws("hi_balance81") > 12))
    POSTCONDITION (gws("balance8") > 0)
    SYMBOLS_POSSIBLY_AFFECTED ("isfl, hi_balance81, balance8")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1509")
    HEURISTIC_RULE (false)
    REASONING ("If it's a FL game and a player had more than 12 BB preflop, then he can never be allin during that hand. Partial rule for player 9.")
    PRECONDITION (gws("isfl") && (gws("hi_balance91") > 12))
    POSTCONDITION (gws("balance9") > 0)
    SYMBOLS_POSSIBLY_AFFECTED ("isfl, hi_balance91, balance9")
END_TESTCASE

 
BEGIN_TESTCASE
    TESTCASE_ID ("1510")
    HEURISTIC_RULE (false)
    REASONING ("A players balance can't increase during a game. Partial rule for player 0.")
    PRECONDITION ((gws("betround") >= 2) && ((int(gws("playersplayingbits")) & 0x0001) == 0x0001))
    POSTCONDITION (gws("hi_balance_01") >= gws("balance0"))
    SYMBOLS_POSSIBLY_AFFECTED ("betround, playersplayingbits, hi_balance_01, balance0")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1511")
    HEURISTIC_RULE (false)
    REASONING ("A players balance can't increase during a game. Partial rule for player 1.")
    PRECONDITION ((gws("betround") >= 2) && ((int(gws("playersplayingbits")) & 0x0002) == 0x0002))
    POSTCONDITION (gws("hi_balance_11") >= gws("balance1"))
    SYMBOLS_POSSIBLY_AFFECTED ("betround, playersplayingbits, hi_balance_11, balance1")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1512")
    HEURISTIC_RULE (false)
    REASONING ("A players balance can't increase during a game. Partial rule for player 2.")
    PRECONDITION ((gws("betround") >= 2) && ((int(gws("playersplayingbits")) & 0x0004) == 0x0004))
    POSTCONDITION (gws("hi_balance_21") >= gws("balance2"))
    SYMBOLS_POSSIBLY_AFFECTED ("betround, playersplayingbits, hi_balance_21, balance2")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1513")
    HEURISTIC_RULE (false)
    REASONING ("A players balance can't increase during a game. Partial rule for player 3.")
    PRECONDITION ((gws("betround") >= 2) && ((int(gws("playersplayingbits")) & 0x0008) == 0x0008))
    POSTCONDITION (gws("hi_balance_31") >= gws("balance3"))
    SYMBOLS_POSSIBLY_AFFECTED ("betround, playersplayingbits, hi_balance_31, balance3")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1514")
    HEURISTIC_RULE (false)
    REASONING ("A players balance can't increase during a game. Partial rule for player 4.")
    PRECONDITION ((gws("betround") >= 2) && ((int(gws("playersplayingbits")) & 0x0010) == 0x0010))
    POSTCONDITION (gws("hi_balance_41") >= gws("balance4"))
    SYMBOLS_POSSIBLY_AFFECTED ("betround, playersplayingbits, hi_balance_41, balance4")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1515")
    HEURISTIC_RULE (false)
    REASONING ("A players balance can't increase during a game. Partial rule for player 5.")
    PRECONDITION ((gws("betround") >= 2) && ((int(gws("playersplayingbits")) & 0x0020) == 0x0020))
    POSTCONDITION (gws("hi_balance_51") >= gws("balance5"))
    SYMBOLS_POSSIBLY_AFFECTED ("betround, playersplayingbits, hi_balance_51, balance5")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1516")
    HEURISTIC_RULE (false)
    REASONING ("A players balance can't increase during a game. Partial rule for player 6.")
    PRECONDITION ((gws("betround") >= 2) && ((int(gws("playersplayingbits")) & 0x0040) == 0x0040))
    POSTCONDITION (gws("hi_balance_61") >= gws("balance6"))
    SYMBOLS_POSSIBLY_AFFECTED ("betround, playersplayingbits, hi_balance_61, balance6")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1517")
    HEURISTIC_RULE (false)
    REASONING ("A players balance can't increase during a game. Partial rule for player 7.")
    PRECONDITION ((gws("betround") >= 2) && ((int(gws("playersplayingbits")) & 0x0080) == 0x0080))
    POSTCONDITION (gws("hi_balance_71") >= gws("balance7"))
    SYMBOLS_POSSIBLY_AFFECTED ("betround, playersplayingbits, hi_balance_71, balance7")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1518")
    HEURISTIC_RULE (false)
    REASONING ("A players balance can't increase during a game. Partial rule for player 8.")
    PRECONDITION ((gws("betround") >= 2) && ((int(gws("playersplayingbits")) & 0x0100) == 0x0100))
    POSTCONDITION (gws("hi_balance_81") >= gws("balance8"))
    SYMBOLS_POSSIBLY_AFFECTED ("betround, playersplayingbits, hi_balance_81, balance8")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("1519")
    HEURISTIC_RULE (false)
    REASONING ("A players balance can't increase during a game. Partial rule for player 9.")
    PRECONDITION ((gws("betround") >= 2) && ((int(gws("playersplayingbits")) & 0x0200) == 0x0200))
    POSTCONDITION (gws("hi_balance_91") >= gws("balance9"))
    SYMBOLS_POSSIBLY_AFFECTED ("betround, playersplayingbits, hi_balance_91, balance9")
END_TESTCASE