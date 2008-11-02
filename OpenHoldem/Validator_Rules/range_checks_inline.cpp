
//****************************************************
//
// General symbols
//

BEGIN_TESTCASE
    TESTCASE_ID ("0001")
    REASONING ("Range check for symbol ismanual")
    PRECONDITION (true)
    POSTCONDITION ((gws("ismanual") == true) || (gws("ismanual") == false))
    SYMBOLS_POSSIBLY_AFFECTED ("ismanual")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0002")
    REASONING ("Range check for symbol isppro")
    PRECONDITION (true)
    POSTCONDITION ((gws("isppro") == true) || (gws("isppro") == false))
    SYMBOLS_POSSIBLY_AFFECTED ("isppro")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0003")
    REASONING ("Range check for symbol site ")
    PRECONDITION (true)
    POSTCONDITION ((gws("site") == true) || (gws("site") == false))
    SYMBOLS_POSSIBLY_AFFECTED ("site")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0004")
    REASONING ("Range check for symbol nchairs ")
    PRECONDITION (true)
    POSTCONDITION ((gws("nchairs") >= 2) && (gws("nchairs") <= 10))
    SYMBOLS_POSSIBLY_AFFECTED ("nchairs")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0005")
    REASONING ("Range check for symbol isbring")
    PRECONDITION (true)
    POSTCONDITION ((gws("isbring") == true) || (gws("isbring") == false))
    SYMBOLS_POSSIBLY_AFFECTED ("isbring")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0006")
    REASONING ("Range check for symbol session")
    PRECONDITION (true)
    POSTCONDITION ((gws("session") >= 0) && (gws("session") <= 9))
    SYMBOLS_POSSIBLY_AFFECTED ("session")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0007")
    REASONING ("Range check for symbol handnumber")
    PRECONDITION (true)
    POSTCONDITION (gws("handnumber") >= 0)
    SYMBOLS_POSSIBLY_AFFECTED ("handnumber")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0008")
    REASONING ("Range check for symbol version")
    PRECONDITION (true)
    POSTCONDITION (gws("version") >= 0)
    SYMBOLS_POSSIBLY_AFFECTED ("version")
END_TESTCASE


//****************************************************
//
// Table map symbols
//

BEGIN_TESTCASE
    TESTCASE_ID ("0009")
    REASONING ("Range check for symbol swagdelay")
    PRECONDITION (true)
    POSTCONDITION (gws("swagdelay") >= 0)
    SYMBOLS_POSSIBLY_AFFECTED ("swagdelay")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0010")
    REASONING ("Range check for symbol allidelay")
    PRECONDITION (true)
    POSTCONDITION (gws("allidelay") >= 0)
    SYMBOLS_POSSIBLY_AFFECTED ("allidelay")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0011")
    REASONING ("Range check for symbol swagtextmethod")
    PRECONDITION (true)
    POSTCONDITION ((gws("swagtextmethod") == 1) || (gws("swagtextmethod") == 2) || (gws("swagtextmethod") == 3))
    SYMBOLS_POSSIBLY_AFFECTED ("swagtextmethod")
END_TESTCASE


//****************************************************
//
// Formula file
//


BEGIN_TESTCASE
    TESTCASE_ID ("0012")
    REASONING ("Range check for symbol rake")
    PRECONDITION (true)
    POSTCONDITION ((gws("rake") >= 0) && (gws("rake") <= 100))
    SYMBOLS_POSSIBLY_AFFECTED ("rake")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0013")
    REASONING ("Range check for symbol nit")
    PRECONDITION (true)
    POSTCONDITION (gws("nit") >= 0)
    SYMBOLS_POSSIBLY_AFFECTED ("nit")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0014")
    REASONING ("Range check for symbol bankroll")
    PRECONDITION (true)
    POSTCONDITION (gws("bankroll") >= 0)
    SYMBOLS_POSSIBLY_AFFECTED ("bankroll")
END_TESTCASE



//****************************************************
//
// Limits
//

BEGIN_TESTCASE
    TESTCASE_ID ("0015")
    REASONING ("Range check for symbol bblind")
    PRECONDITION (true)
    POSTCONDITION (gws("bblind") >= 0.02)
    SYMBOLS_POSSIBLY_AFFECTED ("bblind")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0016")
    REASONING ("Range check for symbol sblind")
    PRECONDITION (true)
    POSTCONDITION (gws("sblind") >= 0.01)
    SYMBOLS_POSSIBLY_AFFECTED ("sblind")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0017")
    REASONING ("Range check for symbol ante")
    PRECONDITION (true)
    POSTCONDITION (gws("ante") >= 0)
    SYMBOLS_POSSIBLY_AFFECTED ("ante")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0018")
    REASONING ("Range check for symbol lim")
    PRECONDITION (true)
    POSTCONDITION ((gws("lim") == 0) || (gws("lim") == 1) || (gws("lim") == 2))
    SYMBOLS_POSSIBLY_AFFECTED ("lim")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0019")
    REASONING ("Range check for symbol isnl")
    PRECONDITION (true)
    POSTCONDITION ((gws("isnl") == true) || (gws("isnl") == false))
    SYMBOLS_POSSIBLY_AFFECTED ("isnl")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0020")
    REASONING ("Range check for symbol ispl")
    PRECONDITION (true)
    POSTCONDITION ((gws("ispl") == true) || (gws("ispl") == false))
    SYMBOLS_POSSIBLY_AFFECTED ("ispl")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0021")
    REASONING ("Range check for symbol isfl")
    PRECONDITION (true)
    POSTCONDITION ((gws("isfl") == true) || (gws("isfl") == false))
    SYMBOLS_POSSIBLY_AFFECTED ("isfl")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0022")
    REASONING ("Range check for symbol sraiprev")
    PRECONDITION (true)
    POSTCONDITION (gws("sraiprev") > 0)
    SYMBOLS_POSSIBLY_AFFECTED ("sraiprev")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0023")
    REASONING ("Range check for symbol sraimin")
    PRECONDITION (true)
    POSTCONDITION (gws("sraimin") > 0)
    SYMBOLS_POSSIBLY_AFFECTED ("sraimin")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0024")
    REASONING ("Range check for symbol sraimax")
    PRECONDITION (true)
    POSTCONDITION (gws("sraimax") >= 0)
    SYMBOLS_POSSIBLY_AFFECTED ("sraimax")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0025")
    REASONING ("Range check for symbol istournament")
    PRECONDITION (true)
    POSTCONDITION ((gws("istournament") == true) || (gws("istournament") == false))
    SYMBOLS_POSSIBLY_AFFECTED ("istournament")
END_TESTCASE


//****************************************************
//
// Hand Rank
//


BEGIN_TESTCASE
    TESTCASE_ID ("0026")
    REASONING ("Range check for symbol handrank")
    PRECONDITION (true)
    POSTCONDITION ((gws("handrank") >= 1) && (gws("handrank") <= 2652))
    SYMBOLS_POSSIBLY_AFFECTED ("handrank")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0027")
    REASONING ("Range check for symbol handrank169")
    PRECONDITION (true)
    POSTCONDITION ((gws("handrank169") >= 1) && (gws("handrank169") <= 169))
    SYMBOLS_POSSIBLY_AFFECTED ("handrank169")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0028")
    REASONING ("Range check for symbol handrank2652")
    PRECONDITION (true)
    POSTCONDITION ((gws("handrank2652") >= 12) && (gws("handrank2652") <= 2652))
    SYMBOLS_POSSIBLY_AFFECTED ("handrank2652")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0029")
    REASONING ("Range check for symbol handrank1326")
    PRECONDITION (true)
    POSTCONDITION ((gws("handrank1326") >= 6) && (gws("handrank1326") <= 1326))
    SYMBOLS_POSSIBLY_AFFECTED ("handrank1326")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0030")
    REASONING ("Range check for symbol handrank1000")
    PRECONDITION (true)
    POSTCONDITION ((gws("handrank1000") >= 4) && (gws("handrank1000") <= 1000))
    SYMBOLS_POSSIBLY_AFFECTED ("handrank1000")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0031")
    REASONING ("Range check for symbol handrankp")
    PRECONDITION (true)
    POSTCONDITION ((gws("handrankp") >= 1) && (gws("handrankp") <= 2652))
    SYMBOLS_POSSIBLY_AFFECTED ("handrankp")
END_TESTCASE


//****************************************************
//
// Chairs
//


BEGIN_TESTCASE
    TESTCASE_ID ("0032")
    REASONING ("Range check for symbol chair")
    PRECONDITION (true)
    POSTCONDITION ((gws("chair") >= 0) && (gws("chair") <= 9))
    SYMBOLS_POSSIBLY_AFFECTED ("chair")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0033")
    REASONING ("Range check for symbol userchair")
    PRECONDITION (true)
    POSTCONDITION ((gws("userchair") >= 0) && (gws("userchair") <= 9))
    SYMBOLS_POSSIBLY_AFFECTED ("userchair")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0034")
    REASONING ("Range check for symbol dealerchair")
    PRECONDITION (true)
    POSTCONDITION ((gws("dealerchair") >= 0) && (gws("dealerchair") <= 9))
    SYMBOLS_POSSIBLY_AFFECTED ("dealerchair")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0035")
    REASONING ("Range check for symbol raischair")
    PRECONDITION (true)
    POSTCONDITION ((gws("raischair") >= 0) && (gws("raischair") <= 9))
    SYMBOLS_POSSIBLY_AFFECTED ("raischair")
END_TESTCASE



//****************************************************
//
// Rounds / Positions
//

BEGIN_TESTCASE
    TESTCASE_ID ("0036")
    REASONING ("Range check for symbol betround")
    PRECONDITION (true)
    POSTCONDITION ((gws("betround") == 1) || (gws("betround") == 2) || (gws("betround") == 3) || (gws("betround") == 4))
    SYMBOLS_POSSIBLY_AFFECTED ("betround")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0037")
    REASONING ("Range check for symbol br")
    PRECONDITION (true)
    POSTCONDITION (gws("br") == gws("betround"))
    SYMBOLS_POSSIBLY_AFFECTED ("br")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0038")
    REASONING ("Range check for symbol betposition")
    PRECONDITION (true)
    POSTCONDITION ((gws("betposition") >= 1) && (gws("betposition") <= gws("nplayersdealt")))
    SYMBOLS_POSSIBLY_AFFECTED ("betposition, nplayersdealt")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0039")
    REASONING ("Range check for symbol dealposition")
    PRECONDITION (true)
    POSTCONDITION ((gws("dealposition") >= 1) && (gws("dealposition") <= gws("nplayersdealt")))
    SYMBOLS_POSSIBLY_AFFECTED ("dealposition, nplayersdealt")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0040")
    REASONING ("Range check for symbol originaldealposition")
    PRECONDITION (true)
    POSTCONDITION ((gws("originaldealposition") >= 1) && (gws("originaldealposition") <= gws("nplayersdealt")))
    SYMBOLS_POSSIBLY_AFFECTED ("originaldealposition, nplayersdealt")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0041")
    REASONING ("Range check for symbol callposition")
    PRECONDITION (true)
    POSTCONDITION ((gws("callposition") >= -9) && (gws("callposition") <= 9))
    SYMBOLS_POSSIBLY_AFFECTED ("callposition")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0042")
    REASONING ("Range check for symbol seatposition")
    PRECONDITION (true)
    POSTCONDITION ((gws("seatposition") >= -9) && (gws("seatposition") <= 9))
    SYMBOLS_POSSIBLY_AFFECTED ("")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0043")
    REASONING ("Range check for symbol dealpositionrais")
    PRECONDITION (true)
    POSTCONDITION ((gws("dealpositionrais") >= 1) && (gws("dealpositionrais") <= 10))
    SYMBOLS_POSSIBLY_AFFECTED ("dealpositionrais")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0044")
    REASONING ("Range check for symbol betpositionrais")
    PRECONDITION (true)
    POSTCONDITION ((gws("betpositionrais") >= 1) && (gws("betpositionrais") <= 10))
    SYMBOLS_POSSIBLY_AFFECTED ("betpositionrais")
END_TESTCASE

//****************************************************
//
// Probabilities
//

BEGIN_TESTCASE
    TESTCASE_ID ("0045")
    REASONING ("Range check for symbol prwin")
    PRECONDITION (true)
    POSTCONDITION ((gws("prwin") >= 0) && (gws("prwin") <= 1))
    SYMBOLS_POSSIBLY_AFFECTED ("prwin")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0046")
    REASONING ("Range check for symbol prlos")
    PRECONDITION (true)
    POSTCONDITION ((gws("prlos") >= 0) && (gws("prlos") <= 1))
    SYMBOLS_POSSIBLY_AFFECTED ("prlos")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0047")
    REASONING ("Range check for symbol prtie")
    PRECONDITION (true)
    POSTCONDITION ((gws("prtie") >= 0) && (gws("prtie") <= 1))
    SYMBOLS_POSSIBLY_AFFECTED ("prtie")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0048")
    REASONING ("Range check for symbol prwinnow")
    PRECONDITION (true)
    POSTCONDITION ((gws("prwinnow") >= 0) && (gws("prwinnow") <= 1))
    SYMBOLS_POSSIBLY_AFFECTED ("prwinnow")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0049")
    REASONING ("Range check for symbol prlosnow")
    PRECONDITION (true)
    POSTCONDITION ((gws("prlosnow") >= 0) && (gws("prlosnow") <= 1))
    SYMBOLS_POSSIBLY_AFFECTED ("prlosnow")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0050")
    REASONING ("Range check for symbol random")
    PRECONDITION (true)
    POSTCONDITION ((gws("random") >= 0) && (gws("random") <= 1))
    SYMBOLS_POSSIBLY_AFFECTED ("random")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0051")
    REASONING ("Range check for symbol randomhand")
    PRECONDITION (true)
    POSTCONDITION ((gws("randomhand") >= 0) && (gws("randomhand") <= 1))
    SYMBOLS_POSSIBLY_AFFECTED ("randomhand")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0052")
    REASONING ("Range check for symbol randomround")
    PRECONDITION (true)
    POSTCONDITION ((gws("randomround") >= 0) && (gws("randomround") <= 1))
    SYMBOLS_POSSIBLY_AFFECTED ("randomround")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0053")
    REASONING ("Range check for symbol randomround1")
    PRECONDITION (true)
    POSTCONDITION ((gws("randomround1") >= 0) && (gws("randomround1") <= 1))
    SYMBOLS_POSSIBLY_AFFECTED ("randomround1")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0054")
    REASONING ("Range check for symbol randomround2")
    PRECONDITION (true)
    POSTCONDITION ((gws("randomround2") >= 0) && (gws("randomround2") <= 1))
    SYMBOLS_POSSIBLY_AFFECTED ("randomround2")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0055")
    REASONING ("Range check for symbol randomround3")
    PRECONDITION (true)
    POSTCONDITION ((gws("randomround3") >= 0) && (gws("randomround3") <= 1))
    SYMBOLS_POSSIBLY_AFFECTED ("randomround3")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0056")
    REASONING ("Range check for symbol randomround4")
    PRECONDITION (true)
    POSTCONDITION ((gws("randomround4") >= 0) && (gws("randomround4") <= 1))
    SYMBOLS_POSSIBLY_AFFECTED ("randomround4")
END_TESTCASE

//
// Statistics
//   Note that these symbols have not been created for use in OpenHoldem yet. 
//   Please see the to-do thread in the OpenHoldem stickies section of the forum for more information.
//


//
// P Formula
//

BEGIN_TESTCASE
    TESTCASE_ID ("0057")
    REASONING ("Range check for symbol defcon")
    PRECONDITION (true)
    POSTCONDITION ((gws("defcaon") >= 0) && (gws("defcon") <= 1))
    SYMBOLS_POSSIBLY_AFFECTED ("defcon")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0058")
    REASONING ("Range check for symbol isdefmode")
    PRECONDITION (true)
    POSTCONDITION ((gws("isdefmode") == true) || (gws("isdefmode") == false))
    SYMBOLS_POSSIBLY_AFFECTED ("isdefmode")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0059")
    REASONING ("Range check for symbol isaggmode")
    PRECONDITION (true)
    POSTCONDITION ((gws("isaggmode") == true) || (gws("isaggmode") == false))
    SYMBOLS_POSSIBLY_AFFECTED ("isaggmode")
END_TESTCASE


//
// Chip Amounts
//

BEGIN_TESTCASE
    TESTCASE_ID ("0060")
    REASONING ("Range check for symbol balance ")
    PRECONDITION (true)
    POSTCONDITION (gws("balance") >= 0)
    SYMBOLS_POSSIBLY_AFFECTED ("balance ")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0061")
    REASONING ("Range check for symbols balance0..balance9 ")
    PRECONDITION (true)
    POSTCONDITION ((gws("balance0") >= 0) && (gws("balance1") >= 0) && (gws("balance2") >= 0) && (gws("balance3") >= 0) && (gws("balance4") >= 0) && (gws("balance5") >= 0) && (gws("balance6") >= 0) && (gws("balance7") >= 0) && (gws("balance8") >= 0) && (gws("balance9") >= 0))
    SYMBOLS_POSSIBLY_AFFECTED ("balance0..balance9")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0062")
    REASONING ("Range check for symbols stack0..stack9")
    PRECONDITION (true)
    POSTCONDITION ((gws("stack0") >= gws("stack1")) && (gws("stack1") >= gws("stack2")) && (gws("stack2") >= gws("stack3")) && (gws("stack3") >= gws("stack4")) && (gws("stack4") >= gws("stack5")) && (gws("stack5") >= gws("stack6")) && (gws("stack6") >= gws("stack7")) && (gws("stack7") >= gws("stack8")) && (gws("stack8") >= gws("stack9")) && (gws("stack9") >= 0))
    SYMBOLS_POSSIBLY_AFFECTED ("stack0..stack9")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0063")
    REASONING ("Range check for symbol currentbet")
    PRECONDITION (true)
    POSTCONDITION (gws("currentbet") >= 0)
    SYMBOLS_POSSIBLY_AFFECTED ("currentbet")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0064")
    REASONING ("Range check for symbols currentbet0..currentbet9 ")
    PRECONDITION (true)
    POSTCONDITION ((gws("currentbet0") >= 0) && (gws("currentbet1") >= 0) && (gws("currentbet2") >= 0) && (gws("currentbet3") >= 0) && (gws("currentbet4") >= 0) && (gws("currentbet5") >= 0) && (gws("currentbet6") >= 0) && (gws("currentbet7") >= 0) && (gws("currentbet8") >= 0) && (gws("currentbet9") >= 0))
    SYMBOLS_POSSIBLY_AFFECTED ("currentbet0..currentbet9")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0065")
    REASONING ("Range check for symbol call")
    PRECONDITION (true)
    POSTCONDITION (gws("call") >= 0)
    SYMBOLS_POSSIBLY_AFFECTED ("call")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0066")
    REASONING ("Range check for symbol bet")
    PRECONDITION (true)
    POSTCONDITION (gws("bet") >= 0)
    SYMBOLS_POSSIBLY_AFFECTED ("bet")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0067")
    REASONING ("Range check for symbols bet1..bet4")
    PRECONDITION (true)
    POSTCONDITION ((gws("bet1") > 0) && (gws("bet2") > 0) && (gws("bet3") > 0) && (gws("bet4") > 0))
    SYMBOLS_POSSIBLY_AFFECTED ("bet1..bet4")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0068")
    REASONING ("Range check for symbol pot")
    PRECONDITION (true)
    POSTCONDITION (gws("pot") == (gws("potcommon") + gws("potplayer")))
    SYMBOLS_POSSIBLY_AFFECTED ("pot, potcommon, potplayer")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0069")
    REASONING ("Range check for symbol potcommon")
    PRECONDITION (true)
    POSTCONDITION (gws("potcommon") >= 0)
    SYMBOLS_POSSIBLY_AFFECTED ("potcommon")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0070")
    REASONING ("Range check for symbol potplayer")
    PRECONDITION (true)
    POSTCONDITION (gws("potplayer") >= 0)
    SYMBOLS_POSSIBLY_AFFECTED ("potplayer")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0071")
    REASONING ("Range check for symbol callshort")
    PRECONDITION (true)
    POSTCONDITION (gws("callshort") >= 0)
    SYMBOLS_POSSIBLY_AFFECTED ("callshort")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0072")
    REASONING ("Range check for symbol raisshort")
    PRECONDITION (true)
    POSTCONDITION (gws("raisshort") >= 0)
    SYMBOLS_POSSIBLY_AFFECTED ("raisshort")
END_TESTCASE

//
// Flags (0127..0128)
//

BEGIN_TESTCASE
    TESTCASE_ID ("0127")
    REASONING ("Range check for symbol fmax")
    PRECONDITION (true)
    POSTCONDITION ((gws("fmax") >= 0) && (gws("fmax") <= 9))
    SYMBOLS_POSSIBLY_AFFECTED ("fmax")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0128")
    REASONING ("Range check for symbol fbits")
    PRECONDITION (true)
    POSTCONDITION ((gws("fbits") >= 0) && (gws("fbits") <= 1023))
    SYMBOLS_POSSIBLY_AFFECTED ("fbits")
END_TESTCASE


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


//
// Table stats symbols
//

BEGIN_TESTCASE
    TESTCASE_ID ("0284")
    REASONING ("Range check for symbol floppct")
    PRECONDITION (true)
    POSTCONDITION ((gws("floppct") >= 0) && (gws("floppct") <= 1))
    SYMBOLS_POSSIBLY_AFFECTED ("floppct")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0285")
    REASONING ("Range check for symbol turnpct")
    PRECONDITION (true)
    POSTCONDITION ((gws("turnpct") >= 0) && (gws("turnpct") <= 1))
    SYMBOLS_POSSIBLY_AFFECTED ("turnpct")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0286")
    REASONING ("Range check for symbol riverpct")
    PRECONDITION (true)
    POSTCONDITION ((gws("riverpct") >= 0) && (gws("riverpct") <= 1))
    SYMBOLS_POSSIBLY_AFFECTED ("riverpct")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0287")
    REASONING ("Range check for symbol avgbetspf")
    PRECONDITION (true)
    POSTCONDITION (gws("avgbetspf") >= 0)
    SYMBOLS_POSSIBLY_AFFECTED ("avgbetspf")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0288")
    REASONING ("Range check for symbol tablepfr")
    PRECONDITION (true)
    POSTCONDITION ((gws("tablepfr") >= 0) && (gws("tablepfr") <= 1))
    SYMBOLS_POSSIBLY_AFFECTED ("tablepfr")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0289")
    REASONING ("Range check for symbol maxbalance")
    PRECONDITION (true)
    POSTCONDITION (gws("maxbalance") >= 0)
    SYMBOLS_POSSIBLY_AFFECTED ("maxbalance")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0290")
    REASONING ("Range check for symbol handsplayed")
    PRECONDITION (true)
    POSTCONDITION (gws("handsplayed") >= 0)
    SYMBOLS_POSSIBLY_AFFECTED ("handsplayed")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0291")
    REASONING ("Range check for symbols balance_rank0..balance_rank9")
    PRECONDITION (true)
    POSTCONDITION ((gws("balance_rank0") >= gws("balance_rank1")) && (gws("balance_rank1") >= gws("balance_rank2")) && (gws("balance_rank2") >= gws("balance_rank3")) && (gws("balance_rank3") >= gws("balance_rank4")) && (gws("balance_rank4") >= gws("balance_rank5")) && (gws("balance_rank5") >= gws("balance_rank6")) && (gws("balance_rank6") >= gws("balance_rank7")) && (gws("balance_rank7") >= gws("balance_rank8")) && (gws("balance_rank8") >= gws("balance_rank9")) && (gws("balance_rank") >= 0))
    SYMBOLS_POSSIBLY_AFFECTED ("balance_rank0..balance_rank9")
END_TESTCASE


//
// Card symbols - part 1
//

BEGIN_TESTCASE
    TESTCASE_ID ("0298")
    REASONING ("Range check for symbol $$ps0")
    PRECONDITION (true)
    POSTCONDITION ((gws("$$ps0") >= 1) && (gws("$$ps0") <= 4))
    SYMBOLS_POSSIBLY_AFFECTED ("$$ps0")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0299")
    REASONING ("Range check for symbol $$ps1")
    PRECONDITION (true)
    POSTCONDITION ((gws("$$ps1") >= 1) && (gws("$$ps1") <= 4))
    SYMBOLS_POSSIBLY_AFFECTED ("$$ps1")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0300")
    REASONING ("Range check for symbol $$cs0")
    PRECONDITION (true)
    POSTCONDITION ((gws("$$cs0") >= 1) && (gws("$$cs0") <= 4))
    SYMBOLS_POSSIBLY_AFFECTED ("$$cs0")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0301")
    REASONING ("Range check for symbol $$cs1")
    PRECONDITION (true)
    POSTCONDITION ((gws("$$cs1") >= 1) && (gws("$$cs1") <= 4))
    SYMBOLS_POSSIBLY_AFFECTED ("$$cs1")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0302")
    REASONING ("Range check for symbol $$cs2")
    PRECONDITION (true)
    POSTCONDITION ((gws("$$cs2") >= 1) && (gws("$$cs2") <= 4))
    SYMBOLS_POSSIBLY_AFFECTED ("$$cs2")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0303")
    REASONING ("Range check for symbol $$cs3")
    PRECONDITION (true)
    POSTCONDITION ((gws("$$cs3") >= 1) && (gws("$$cs3") <= 4))
    SYMBOLS_POSSIBLY_AFFECTED ("$$cs3")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0304")
    REASONING ("Range check for symbol $$cs4")
    PRECONDITION (true)
    POSTCONDITION ((gws("$$cs4") >= 1) && (gws("$$cs4") <= 4))
    SYMBOLS_POSSIBLY_AFFECTED ("$$cs4")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0305")
    REASONING ("Range check for symbol $$pr0")
    PRECONDITION (true)
    POSTCONDITION ((gws("$$pr0") >= 1) && (gws("$$pr0") <= 14))
    SYMBOLS_POSSIBLY_AFFECTED ("$$pr0")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0306")
    REASONING ("Range check for symbol $$pr1")
    PRECONDITION (true)
    POSTCONDITION ((gws("$$pr1") >= 1) && (gws("$$pr1") <= 14))
    SYMBOLS_POSSIBLY_AFFECTED ("$$pr1")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0307")
    REASONING ("Range check for symbol $$cr0")
    PRECONDITION (true)
    POSTCONDITION ((gws("$$cr0") >= 1) && (gws("$$cr0") <= 14))
    SYMBOLS_POSSIBLY_AFFECTED ("$$cr0")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0308")
    REASONING ("Range check for symbol $$cr1")
    PRECONDITION (true)
    POSTCONDITION ((gws("$$cr1") >= 1) && (gws("$$cr1") <= 14))
    SYMBOLS_POSSIBLY_AFFECTED ("$$cr1")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0309")
    REASONING ("Range check for symbol $$cr2")
    PRECONDITION (true)
    POSTCONDITION ((gws("$$cr2") >= 1) && (gws("$$cr2") <= 14))
    SYMBOLS_POSSIBLY_AFFECTED ("$$cr2")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0310")
    REASONING ("Range check for symbol $$cs3")
    PRECONDITION (true)
    POSTCONDITION ((gws("$$cr3") >= 1) && (gws("$$cr3") <= 14))
    SYMBOLS_POSSIBLY_AFFECTED ("$$cr3")
END_TESTCASE


BEGIN_TESTCASE
    TESTCASE_ID ("0311")
    REASONING ("Range check for symbol $$cr4")
    PRECONDITION (true)
    POSTCONDITION ((gws("$$cr4") >= 1) && (gws("$$cr4") <= 14))
    SYMBOLS_POSSIBLY_AFFECTED ("$$cr4")
END_TESTCASE



