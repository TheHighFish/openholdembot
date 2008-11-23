//
// Cards
//

#define CARD_UNKNOWN 0x00
#define CARDBACK     0xff


BEGIN_TESTCASE
    TESTCASE_ID ("0400")
    REASONING ("All cards have to be unique. Partial rule for $pc0")
    PRECONDITION ((gws("$pc0") != CARD_UNKNOWN) && (gws("$pc0") != CARDBACK))
    POSTCONDITION ((gws("$pc0") != gws("$pc1")) && (gws("$pc0") != gws("$cc0")) && (gws("$pc0") != gws("$cc1")) && (gws("$pc0") != gws("$cc2")) && (gws("$pc0") != gws("$cc3")) && (gws("$pc0") != gws("$cc4")))
    SYMBOLS_POSSIBLY_AFFECTED ("$pc0. $pc1, $cc0, $cc1, $cc2, $cc3, $cc4")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0401")
    REASONING ("All cards have to be unique. Partial rule for $pc1")
    PRECONDITION ((gws("$pc1") != CARD_UNKNOWN) && (gws("$pc1") != CARDBACK))
    POSTCONDITION ((gws("$pc0") != gws("$pc1")) && (gws("$pc1") != gws("$cc0")) && (gws("$pc1") != gws("$cc1")) && (gws("$pc1") != gws("$cc2")) && (gws("$pc1") != gws("$cc3")) && (gws("$pc1") != gws("$cc4")))
    SYMBOLS_POSSIBLY_AFFECTED ("$pc0. $pc1, $cc0, $cc1, $cc2, $cc3, $cc4")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0402")
    REASONING ("All cards have to be unique. Partial rule for $cc0")
    PRECONDITION ((gws("$cc0") != CARD_UNKNOWN) && (gws("$cc0") != CARDBACK))
    POSTCONDITION ((gws("$cc0") != gws("$pc0")) && (gws("$cc0") != gws("$pc1")) && (gws("$cc0") != gws("$cc1")) && (gws("$cc0") != gws("$cc2")) && (gws("$cc0") != gws("$cc3")) && (gws("$cc0") != gws("$cc4")))
    SYMBOLS_POSSIBLY_AFFECTED ("$pc0. $pc1, $cc0, $cc1, $cc2, $cc3, $cc4")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0403")
    REASONING ("All cards have to be unique. Partial rule for $cc1")
    PRECONDITION ((gws("$cc1") != CARD_UNKNOWN) && (gws("$cc1") != CARDBACK))
    POSTCONDITION ((gws("$cc1") != gws("$pc0")) && (gws("$cc1") != gws("$pc1")) && (gws("$cc1") != gws("$cc0")) && (gws("$cc1") != gws("$cc2")) && (gws("$cc1") != gws("$cc3")) && (gws("$cc1") != gws("$cc4")))
    SYMBOLS_POSSIBLY_AFFECTED ("$pc0. $pc1, $cc0, $cc1, $cc2, $cc3, $cc4")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0404")
    REASONING ("All cards have to be unique. Partial rule for $cc2")
    PRECONDITION ((gws("$cc2") != CARD_UNKNOWN) && (gws("$cc2") != CARDBACK))
    POSTCONDITION ((gws("$cc2") != gws("$pc0")) && (gws("$cc2") != gws("$pc1")) && (gws("$cc2") != gws("$cc0")) && (gws("$cc2") != gws("$cc1")) && (gws("$cc2") != gws("$cc3")) && (gws("$cc2") != gws("$cc4")))
    SYMBOLS_POSSIBLY_AFFECTED ("$pc0. $pc1, $cc0, $cc1, $cc2, $cc3, $cc4")
END_TESTCASE



BEGIN_TESTCASE
    TESTCASE_ID ("0405")
    REASONING ("All cards have to be unique. Partial rule for $cc3")
    PRECONDITION ((gws("$cc3") != CARD_UNKNOWN) && (gws("$cc3") != CARDBACK))
    POSTCONDITION ((gws("$cc3") != gws("$pc0")) && (gws("$cc3") != gws("$pc1")) && (gws("$cc3") != gws("$cc0")) && (gws("$cc3") != gws("$cc1")) && (gws("$cc3") != gws("$cc2")) && (gws("$cc3") != gws("$cc4")))
    SYMBOLS_POSSIBLY_AFFECTED ("$pc0. $pc1, $cc0, $cc1, $cc2, $cc3, $cc4")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0406")
    REASONING ("All cards have to be unique. Partial rule for $cc4")
    PRECONDITION ((gws("$cc4") != CARD_UNKNOWN) && (gws("$cc4") != CARDBACK))
    POSTCONDITION ((gws("$cc4") != gws("$pc0")) && (gws("$cc4") != gws("$pc4")) && (gws("$cc4") != gws("$cc0")) && (gws("$cc4") != gws("$cc1")) && (gws("$cc4") != gws("$cc2")) && (gws("$cc4") != gws("$cc3")))
    SYMBOLS_POSSIBLY_AFFECTED ("$pc0. $pc1, $cc0, $cc1, $cc2, $cc3, $cc4")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0407")
    REASONING ("If there's a turn card, there also have to be all 3 flop cards.")
    PRECONDITION ((gws("$cc3") != CARD_UNKNOWN) && (gws("$cc3") != CARDBACK))
    POSTCONDITION ((gws("$cc0") != CARD_UNKNOWN) && (gws("$cc0") != CARDBACK) && (gws("$cc1") != CARD_UNKNOWN) && (gws("$cc1") != CARDBACK) && (gws("$cc2") != CARD_UNKNOWN) && (gws("$cc2") != CARDBACK))
    SYMBOLS_POSSIBLY_AFFECTED ("$cc0, $cc1, $cc2, $cc3")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0408")
    REASONING ("If there's a river card, there also have to be all 3 flop cards and the turn card.")
    PRECONDITION ((gws("$cc4") != CARD_UNKNOWN) && (gws("$cc4") != CARDBACK))
    POSTCONDITION ((gws("$cc0") != CARD_UNKNOWN) && (gws("$cc0") != CARDBACK) && (gws("$cc1") != CARD_UNKNOWN) && (gws("$cc1") != CARDBACK) && (gws("$cc2") != CARD_UNKNOWN) && (gws("$cc2") != CARDBACK) && (gws("$cc3") != CARD_UNKNOWN) && (gws("$cc3") != CARDBACK))
    SYMBOLS_POSSIBLY_AFFECTED ("$cc0, $cc1, $cc2, $cc3, $cc4")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0409")
    REASONING ("If it's my turn, then I must hold cards.")
    PRECONDITION (gws("ismyturn"))
    POSTCONDITION ((gws("$pc0") != CARD_UNKNOWN) && (gws("$pc0") != CARDBACK) && (gws("$pc1") != CARD_UNKNOWN) && (gws("$pc1") != CARDBACK))
    SYMBOLS_POSSIBLY_AFFECTED ("ismyturn, $pc0, $pc1")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0410")
    REASONING ("If my first card is know, then my second card has also to be known.")
    PRECONDITION ((gws("$pc0") != CARD_UNKNOWN) && (gws("$pc0") != CARDBACK))
    POSTCONDITION ((gws("$pc1") != CARD_UNKNOWN) && (gws("$pc1") != CARDBACK))
    SYMBOLS_POSSIBLY_AFFECTED ("$pc0, $pc1")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0411")
    REASONING ("If my second card is know, then my first card has also to be known.")
    PRECONDITION ((gws("$pc1") != CARD_UNKNOWN) && (gws("$pc1") != CARDBACK))
    POSTCONDITION ((gws("$pc0") != CARD_UNKNOWN) && (gws("$pc0") != CARDBACK))
    SYMBOLS_POSSIBLY_AFFECTED ("$pc0, $pc1")
END_TESTCASE


// If I'm not active, then I can't hold cards.

// If I'n not playing, I cant hold cards.