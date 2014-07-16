
//*****************************************************************************
//
// OpenPPL Library
//   Supporting functions for the OpenPPL converter
//
// OpenPPL is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
// The original PPL (Poker Programming Language) got invented by
//   Shanky Bonusbots:      http://bonusbots.com/support/index.php
//
// Required OpenHoldem version: 4.5.0 or later
//   * because we make use of the new symbol nopponentstruelyraising
//
// Contributors:
//   Bob3456
//   CoolBotMe123
//   CrumbleSunday
//   Dr.Kaktuz
//   Efu
//   Fn101
//   Fqfq	
//   Gecko
//   JConner
//   JokeDoke
//   KaeKruse
//   Kyo
//   Lavezzi
//   MerzkiyTipon
//   Morello
//   MrRuder
//   Nik0
//   NPNG
//   Poker_Dog
//   Postenjak
//   Rambam
//   Relhby
//   Satanikut
//   Stangelnator
//   SteveG1963
//   Strikles
//   SuperBlak
//   TheHighFish
//   ThemAndMe
//   Want2Learn
//   Zolp
//
//*****************************************************************************

//*****************************************************************************
//
//  Predefined action constants
//  ===========================
//
//  Positive values mean:  betsizes (in big-blinds)
//  Small negative values: percentaged potsized bets
//  Large negative values: action constants
//
//  Note: Beep is not supported.
//
//*****************************************************************************
double CalculateOpenPPL_Fold()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1000000);
}

double CalculateOpenPPL_Check()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1000001);
}

double CalculateOpenPPL_Undefined_BetSize_Probably_Due_To_Misscraped_Pot_Defaulting_To_Minraise()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1000002);
}

double CalculateOpenPPL_RaiseMin()    // Please revisit the return type. int or bool might be more appropriate
{
    return (Raise());
}

double CalculateOpenPPL_RaiseHalfPot()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1000003);
}

double CalculateOpenPPL_RaisePot()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1000004);
}

double CalculateOpenPPL_RaiseMax()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1000005);
}

double CalculateOpenPPL_Call()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1000006);
}

double CalculateOpenPPL_Play()    // Please revisit the return type. int or bool might be more appropriate
{
    return (Call());
}

double CalculateOpenPPL_Bet()    // Please revisit the return type. int or bool might be more appropriate
{
    return (Raise());
}

double CalculateOpenPPL_BetMin()    // Please revisit the return type. int or bool might be more appropriate
{
    return (Bet());
}

double CalculateOpenPPL_BetHalfPot()    // Please revisit the return type. int or bool might be more appropriate
{
    return (RaiseHalfPot());
}

double CalculateOpenPPL_BetPot()    // Please revisit the return type. int or bool might be more appropriate
{
    return (RaisePot());
}

double CalculateOpenPPL_BetMax()    // Please revisit the return type. int or bool might be more appropriate
{
    return (RaiseMax());
}

double CalculateOpenPPL_Allin()    // Please revisit the return type. int or bool might be more appropriate
{
    return (RaiseMax());
}

double CalculateOpenPPL_None()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1000007);
}

double CalculateOpenPPL_Raise()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1000008);
}

double CalculateOpenPPL_SitOut()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1000009);
}

double CalculateOpenPPL_Close()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1000010);
}

double CalculateOpenPPL_Beep()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1000011);
}

double CalculateOpenPPL_Do_Not_PreFold()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Automatically added to the end of the prefold-section
        // for syntactical completeness.
        -1000012);
}

double CalculateOpenPPL_UndefinedReturnValue()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Every user-defined function ends with this return-value
        // mainly to ensure syntactical correctness
        // and to avoid undefined return values.
        // OpenHoldem would return 0, when a return-value was not specified,
        // but this might be meaningful in most contexts.
        //
        // To always be on the save site we use...
        // UndefinedReturnValue = Fold = -1
        Fold());
}


//*****************************************************************************
//
//  OpenPPL - Card Constants
//  ===========================
//  
//  Both ranks and suits according to the OpenHoldem docu
//
//*****************************************************************************
double CalculateOpenPPL_ConstCardAce()    // Please revisit the return type. int or bool might be more appropriate
{
    return (14);
}

double CalculateOpenPPL_ConstCardKing()    // Please revisit the return type. int or bool might be more appropriate
{
    return (13);
}

double CalculateOpenPPL_ConstCardQueen()    // Please revisit the return type. int or bool might be more appropriate
{
    return (12);
}

double CalculateOpenPPL_ConstCardJack()    // Please revisit the return type. int or bool might be more appropriate
{
    return (11);
}

double CalculateOpenPPL_ConstCardTen()    // Please revisit the return type. int or bool might be more appropriate
{
    return (10);
}

double CalculateOpenPPL_ConstCardNine()    // Please revisit the return type. int or bool might be more appropriate
{
    return (9);
}

double CalculateOpenPPL_ConstCardEight()    // Please revisit the return type. int or bool might be more appropriate
{
    return (8);
}

double CalculateOpenPPL_ConstCardSeven()    // Please revisit the return type. int or bool might be more appropriate
{
    return (7);
}

double CalculateOpenPPL_ConstCardSix()    // Please revisit the return type. int or bool might be more appropriate
{
    return (6);
}

double CalculateOpenPPL_ConstCardFive()    // Please revisit the return type. int or bool might be more appropriate
{
    return (5);
}

double CalculateOpenPPL_ConstCardFour()    // Please revisit the return type. int or bool might be more appropriate
{
    return (4);
}

double CalculateOpenPPL_ConstCardThree()    // Please revisit the return type. int or bool might be more appropriate
{
    return (3);
}

double CalculateOpenPPL_ConstCardTwo()    // Please revisit the return type. int or bool might be more appropriate
{
    return (2);
}

double CalculateOpenPPL_ConstSuitClubs()    // Please revisit the return type. int or bool might be more appropriate
{
    return (1);
}

double CalculateOpenPPL_ConstSuitDiamonds()    // Please revisit the return type. int or bool might be more appropriate
{
    return (2);
}

double CalculateOpenPPL_ConstSuitHearts()    // Please revisit the return type. int or bool might be more appropriate
{
    return (3);
}

double CalculateOpenPPL_ConstSuitSpades()    // Please revisit the return type. int or bool might be more appropriate
{
    return (4);
}


//*****************************************************************************
//
// OpenPPL - Betting Round Constants
//
//*****************************************************************************
double CalculateOpenPPL_ConstBetRoundPreflop()    // Please revisit the return type. int or bool might be more appropriate
{
    return (1);
}

double CalculateOpenPPL_ConstBetRoundFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return (2);
}

double CalculateOpenPPL_ConstBetRoundTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return (3);
}

double CalculateOpenPPL_ConstBetRoundRiver()    // Please revisit the return type. int or bool might be more appropriate
{
    return (4);
}


//*****************************************************************************
//
// OpenPPL - Other Constants
//
//*****************************************************************************
double CalculateOpenPPL_True()    // Please revisit the return type. int or bool might be more appropriate
{
    return (1);
}

double CalculateOpenPPL_False()    // Please revisit the return type. int or bool might be more appropriate
{
    return (0);
}


//*****************************************************************************
//
// OpenPPL - verbose errors and warning
//
// Instead of returning just -1 we define constants like
// "HINT_EarlyPosition1_Does_Not_Exist"
// This way we get a verbose explanation in the log-file.
//
//*****************************************************************************
double CalculateOpenPPL_ERROR_THIS_SHOULD_NOT_HAPPEN__THERE_SEEMS_TO_BE_A_PROBLEM_IN_THE_LIBRARY()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_ERROR_Blinds_Not_Recognized_Correctly__Check_Your_TableMap()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_ERROR_Raises_Is_Negative__Trying_To_Adapt_In_A_Reasonable_Way()    // Please revisit the return type. int or bool might be more appropriate
{
    return (0);
}

double CalculateOpenPPL_ERROR_Suit_Of_Card_Not_Recognized_Correctly__Check_Your_TableMap()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_ERROR_Rank_Of_Card_Not_Recognized_Correctly__Check_Your_TableMap()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_ERROR_Betround_Out_Of_Range()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_ERROR_BotsActionsOnThisRound_Out_Of_Range()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_ERROR_ButtonChair_Not_Recognized__Check_Your_TableMap()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_ERROR_CutOffChair_Not_Recognized__Check_Your_TableMap()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_ERROR_MiddlePosition3Chair_Not_Recognized__Check_Your_TableMap()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_ERROR_MiddlePosition2Chair_Not_Recognized__Check_Your_TableMap()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_ERROR_MiddlePosition1Chair_Not_Recognized__Check_Your_TableMap()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_ERROR_EarlyPosition3Chair_Not_Recognized__Check_Your_TableMap()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_ERROR_EarlyPosition2Chair_Not_Recognized__Check_Your_TableMap()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_ERROR_EarlyPosition1Chair_Not_Recognized__Check_Your_TableMap()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_ERROR_BigBlindChair_Not_Recognized__Check_Your_TableMap()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_ERROR_SmallBlindChair_Not_Recognized__Check_Your_TableMap()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_ERROR_DealPosition1Chair_Not_Recognized__Check_Your_TableMap()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_ERROR_We_Dont_Have_Any_Straight__No_Not_Call_This_Function()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_ERROR_Dealposition_Out_Of_Range__Check_Your_TableMap()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_ERROR_We_Dont_Have_Any_Straight__Do_Not_Call_This_Function()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_ERROR_No_Opponents_Detected__Check_Your_TableMap()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Returning -bblind, because we divide by bblind to -1.
        (0 - bblind()));
}

double CalculateOpenPPL_ERROR_Game_Is_Not_HeadsUp()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_ERROR_You_Dont_Have_Any_Kicker()    // Please revisit the return type. int or bool might be more appropriate
{
    return (0);
}

double CalculateOpenPPL_HINT_We_Dont_Have_A_FullHouse_At_All()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Docu says: this case is 0; and this special case needs extra care.
        // However returning a high number instead of 0 seems better to us,
        // as it indicates the weakness and avoids any pitfalls.
        999);
}

double CalculateOpenPPL_HINT_There_Are_No_True_Raisers()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_HINT_Such_A_Straight_Does_Not_Exist()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_HINT_Something_Is_Wrong__It_Seems_You_Are_Not_Even_Seated()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_HINT_EarlyPosition1_Does_Not_Exist()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_HINT_EarlyPosition2_Does_Not_Exist()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_HINT_EarlyPosition3_Does_Not_Exist()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_HINT_MiddlePosition1_Does_Not_Exist()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_HINT_MiddlePosition2_Does_Not_Exist()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_HINT_MiddlePosition3_Does_Not_Exist()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_HINT_CutOff_Does_Not_Exist()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_HINT_There_Is_No_SmallNlind_In_This_Game()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1 );
}

double CalculateOpenPPL_HINT_There_Is_No_Raiser_Therefore_No_CurrentBet()    // Please revisit the return type. int or bool might be more appropriate
{
    return (0);
}

double CalculateOpenPPL_HINT_There_Is_No_Pair_On_The_Board()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_HINT_Nothing_To_Do_But_All_OK()    // Please revisit the return type. int or bool might be more appropriate
{
    return (0);
}

double CalculateOpenPPL_HINT_SuitsOnBoard_Undefined_For_This_Betting_Round()    // Please revisit the return type. int or bool might be more appropriate
{
    return (0);
}

double CalculateOpenPPL_HINT_StillToAct_Is_Only_Valid_On_First_Orbit()    // Please revisit the return type. int or bool might be more appropriate
{
    return (0);
}

double CalculateOpenPPL_HINT_There_Are_No_Common_Cards_Preflop()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_HINT_All_Opponents_Allin()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Returning the stacksize of our last opponent
        0);
}

double CalculateOpenPPL_HINT_No_Oppponent_Or_Not_Headsup()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_HINT_No_FirstCaller()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_HINT_No_LastCaller()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_HINT_No_FirstRaiser()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_HINT_No_Last_Raiser()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_HINT_No_Last_Raiser_Or_Not_Enough_Hands()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1);
}

double CalculateOpenPPL_HINT_We_Wont_Close_The_Table_While_Not_Sitting_Out()    // Please revisit the return type. int or bool might be more appropriate
{
    return (False());
}


//*****************************************************************************
//
// OpenPPL - Verbosity
//
// Some OpenHoldem-symbols are awfully illnamed,
// thanks to Ray E. Bornert who invented WinHoldem.
// rais, swag, $$pr0, etc.
// We provide meaningful names here to make this library more readable.
//
//*****************************************************************************
double CalculateOpenPPL_RankOfFirstHoleCard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ($$pr0());
}

double CalculateOpenPPL_RankOfSecondHoleCard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ($$pr1());
}

double CalculateOpenPPL_FirstFlopCard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ($$cr0());
}

double CalculateOpenPPL_SecondFlopCard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ($$cr1());
}

double CalculateOpenPPL_ThirdFlopCard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ($$cr2());
}

double CalculateOpenPPL_TurnCard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ($$cr3());
}

double CalculateOpenPPL_RiverCard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ($$cr4());
}


//***********************************************************
//                                                           
//  Technical functions                                      
//  ===================                                      
//                                                           
//  Functions needed for integration into OpenHoldem.        
//  They translate the f$Decision-function to standard       
//  autoplayer-functions like f$alli, f$rais and f$call.     
//
//  The OpenPPL-logic must be able to handle several
//  different kinds of actions, that must be distinguishable.
//  Therefore we use 3 different ranges for these actions: 
//  * positive values: betsize in big-blinds,
//    using Shankys RaiseBy-semantics.
//    "RAISE 3 FORCE" will be translated to
//    "3 * f$RaiseByBigBlinds"
//    where f$RaiseByBigBlinds simply is 1.
//  * small negative values: percentaged potsized bets
//    "RAISE 70% FORCE" will be translated to 
//    "70 * f$RaiseByPercentagedPotsize"
//    where f$RaiseByPercentagedPotsize is -1.
//    f$betsize will then translate this to the correct bet-amount.
//  * large negative values (< 1.000.000): 
//    fixed action constants for check, call, raise, etc. 
//                                                           
//***********************************************************
double CalculateOpenPPL_Decision()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// f$preflop, f$flop, f$turn and f$river will be added by the translator.
        (betround() == ConstBetRoundPreflop()) ? f$preflop() :
        (betround() == ConstBetRoundFlop())    ? f$flop() :
        (betround() == ConstBetRoundTurn())    ? f$turn() :
        (betround() == ConstBetRoundRiver())   ? f$river() :
        ERROR_Betround_Out_Of_Range());
}

double CalculateRaiseByBigBlinds()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Positive amounts are betsizes in big-blinds
        1);
}

double CalculateRaiseByPercentagedPotsize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Small negative values are used for percentaged potsized bets.
        // We also do the multiplication with 0.01 here.
        // Don't use " / 100 *" because that can lead to an integer-division,
        // and 70/100 will unexpectedly get evaluated as 0.
        -0.01);
}

double CalculateOpenPPL_DecisionIsPercentagedPotsizedBet()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((Decision() <= 0) 
        && (Decision() >= -1000));
}

double CalculateOpenPPL_DecisionIsFixedBetInBigBlinds()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((Decision() > 0) );
}

double CalculateOpenPPL_DecisionIsBetPotOrBetHalfPotOrBetMax()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Used for sanity checks in f$swag
        (Decision() == BetHalfPot())
        || (Decision() == BetPot())
        || (Decision() == BetMax()));
}

double CalculateOpenPPL_DecisionIsFixedAction()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((Decision() < -1000000) );
}

double CalculateOpenPPL_RememberToSitoutIfNecessary()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// OpenHoldem handles autoplayer actions when it is heros turn,
        // but f$sitin, f$sitout and f$leave once every N heartbeats,
        // contrary to OpenPPL were both can happen only at heros turn.
        // Therefore we have to remember a decision to SitOut somehow
        // and recall it later.
        (Decision() == SitOut()) ? me_st_GoBuddySitOut_1() : 
        (Decision() == Close())  ? (me_st_GoBuddySitOut_1() + me_st_GoBuddyClose_1() ) : 0);
}

double Calculatebeep()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// f$beep is the very first function to be called on each turn,
        // so we abuse it to initialize memory-symbhols as a side-effect
        // and only then we do the real evaluation.
        //
        // We can't use addition here to call the functions in correct sequential order,
        // because OH doesn't add the operands in order:
        // http://www.maxinmontreal.com/forums/viewtopic.php?f=111&t=14525&start=0
        (InitMemorySymbols() * 0)
        || (RememberToSitoutIfNecessary() * 0)
        || (Decision() == Beep()));
}

double Calculatealli()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((Decision() == RaiseMax())
        || ((Decision() > StackSize()) && (StackSize() > 0))
        || (DecisionIsPercentagedPotsizedBet() 
        && BetSizeForPercentagedPotsizeBet() >= MaximumBetsizeInDollars()));
}



// Betpot functions
// OpenPPL knows only the following action-command:
//   * BetPot
//   * BetHalfPot
// However we support all betpot-functions as backup-actions.
double Calculatebetpot_2_1()    // Please revisit the return type. int or bool might be more appropriate
{
    return (f$alli());
}

double Calculatebetpot_1_1()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((f$alli()  || (Decision() == BetPot())));
}

double Calculatebetpot_3_4()    // Please revisit the return type. int or bool might be more appropriate
{
    return (f$betpot_1_1());
}

double Calculatebetpot_2_3()    // Please revisit the return type. int or bool might be more appropriate
{
    return (f$betpot_1_1());
}

double Calculatebetpot_1_2()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((f$betpot_1_1() || (Decision() == BetHalfPot())));
}

double Calculatebetpot_1_3()    // Please revisit the return type. int or bool might be more appropriate
{
    return (f$betpot_1_2());
}

double Calculatebetpot_1_4()    // Please revisit the return type. int or bool might be more appropriate
{
    return (f$betpot_1_2());
}

double CalculateOpenPPL_MaximumBetsizeInDollars()    // Please revisit the return type. int or bool might be more appropriate
{
    return (MaximumPossibleBetsizeIndollars());
}

double CalculateOpenPPL_BetSizeForBetHalfPot()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Calculated in dollars and with raise-to-semantics
        // First we call, then we add half of the pot (after we called)
        // pot already includes currentbet, so we must not consider this twice.
        (currentbet() + call() + 0.5 * (pot() + call())));
}

double CalculateOpenPPL_BetSizeForBetPot()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Calculated in dollars and with raise-to-semantics
        // pot includes currentbet, call is the amount we need to call.
        (currentbet() + call() + (pot() + call())));
}

double CalculateOpenPPL_BetSizeForPercentagedPotsizeBet()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// f$Decision is negative, -0.70 means: 70% potsize
        (currentbet() + call() + (-1 * Decision()) * (pot() + call())));
}

double CalculateOpenPPL_BetSizeForFixedBetInBigBlinds()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Calculated in dollars and with raise-to-semantics
        ((Decision() + ncallbets()) * bblind()));
}

double CalculateOpenPPL_BetSizeForAllin()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Calculated in dollars and with raise-to-semantics
        (balance() + currentbet()));
}

double Calculatebetsize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// f$betsize gets counted in Dollars.
        // OpenHoldems semantics is "raise-to".
        f$alli() ? BetSizeForAllin() :
        (Decision() == BetPot()) ? BetSizeForBetPot() : 
        (Decision() == BetHalfPot()) ? BetSizeForBetHalfPot() :
        // Finally caring about about absolute betsizes.
        // Shankys semantics is "raise-by" and in big blinds,
        // so we have to translate it.
        DecisionIsFixedBetInBigBlinds() ?  BetSizeForFixedBetInBigBlinds() :
        DecisionIsPercentagedPotsizedBet() ? BetSizeForPercentagedPotsizeBet() :
        0);
}

double Calculaterais()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((Decision() == Raise())
        || DecisionIsBetPotOrBetHalfPotOrBetMax()
        || DecisionIsPercentagedPotsizedBet()
        || DecisionIsFixedBetInBigBlinds()
        //
        // Below is 2 times the same expression,
        // but we want to log the very verbose explanation
        // only in the case of a true error
        // and not for normal evaluations.
        //
        || (Decision() == 0 && f$betsize() == Undefined_BetSize_Probably_Due_To_Misscraped_Pot_Defaulting_To_Minraise()));
}

double Calculatecall()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((Decision() == Check())
        || (Decision() == Call())
        || (AmountToCall() == 0)
        || f$rais());
}

double Calculatecheck()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Always check/fold as long as the action is not "Beep".
        (Decision() != Beep()));
}

double Calculatefold()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Always check/fold as long as the action is not "Beep".
        (Decision() != Beep()));
}

double Calculatesitout()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Sitout if GoBuddyTakeABreakAndABeer is 1
        // then reset this variable to allow f$sitin later.
        // http://www.maxinmontreal.com/forums/viewtopic.php?f=259&t=15515
        me_re_GoBuddySitOut()
        + (0 * me_st_GoBuddySitOut_0()));
}

double Calculateclose()    // Please revisit the return type. int or bool might be more appropriate
{
    return (me_re_GoBuddyClose() ?
        (issittingin() ? HINT_We_Wont_Close_The_Table_While_Not_Sitting_Out() : True()) :
        False());
}


//*****************************************************************************
//
// OpenPPL - BettingAction Symbols
//
//*****************************************************************************
double CalculateOpenPPL_InitBets()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// No bet in front of us to call
        ((currentbet() == 0) && (AmountToCall() == 0)) ? me_st_MemBets_0() :
        // A bet to call and we din't act before or we checked
        ((currentbet() == 0) && (AmountToCall() > 0)) ? me_st_MemBets_1() :
        // Otherwise: we did already bet ourself or call a bet.
        // So don't change anything
        HINT_Nothing_To_Do_But_All_OK());
}

double CalculateOpenPPL_Bets()    // Please revisit the return type. int or bool might be more appropriate
{
    return (me_re_MemBets());
}

double CalculateOpenPPL_BigBlindSittingOut()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1 + msgbox$BigBlindStittingOut_Bnot_Byet_Bimplemented_D_NYour_Bhelp_Bis_Brequired_D());
}

double CalculateOpenPPL_SmallBlindSittingOut()    // Please revisit the return type. int or bool might be more appropriate
{
    return (-1 + msgbox$SmallBlindSittingOut_Bnot_Byet_Bimplemented_D_NYour_Bhelp_Bis_Brequired_D());
}

double CalculateOpenPPL_BotCalledBeforeFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return (didcallround1());
}

double CalculateOpenPPL_AggressorChair()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// raischair (formerly ac_aggressor) alone is somewhat unreliable, as it gets scraped (by bets).
        // But if we raise and there are super-fast calls behind us (e.g. at PokerAcademy),
        // then we don't have stable frames and soon we will switch to the next betting round.
        // (There are however no problems, if somebody else raises,
        // because we always get stable frames at out turn.)
        // So we also check for our last action and the amount to call.
        // (-1 = fold 0 = check 1 = call 2 = raise e3 = swag 4 = allin)
        (((prevaction() ==  2) || (prevaction() == 3)) && (AmountToCall() == 0)) ? userchair() :
        raischair());
}

double CalculateOpenPPL_BotIsLastRaiser()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((AggressorChair()==userchair()));
}

double CalculateOpenPPL_BotRaisedBeforeFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((didraisround1() || didswaground1()));
}

double CalculateOpenPPL_BotRaisedOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((didraisround2() || didswaground2()));
}

double CalculateOpenPPL_BotRaisedOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((didraisround3() || didswaground3()));
}

double CalculateOpenPPL_BotsActionsOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Checks are not counted
        (didcallround2() + didraisround2() + didswaground2()));
}

double CalculateOpenPPL_BotsActionsOnThisRound()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Checks are not counted
        (didcall() + didrais() + didswag()));
}

double CalculateOpenPPL_BotsActionsOnThisRoundIncludingChecks()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Folds and allins are not relevant, as then the hand is over for us.
        (didchec() + didcall() + didswag() + didrais()));
}

double CalculateOpenPPL_BotsActionsPreflop()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Checks are not counted per definition
        // and we can't check preflop without posting a blind out of order (first hand only).
        (didcallround1() + didraisround1() + didswaground1()));
}

double CalculateOpenPPL_InitBotsLastAction()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Initialization on first action preflop
        // This has to be done *before* we call the Decision-function,
        // InitBotsLastAction has to be the last init-function to be called
        // (inside InitMemorySymbols)
        // because it will call the final f$Decision routine.
        //
        ((betround() == ConstBetRoundPreflop()) && (BotsActionsOnThisRoundIncludingChecks() == 0)) ? me_st_MemBotsLastAction_None() :
        //
        // "Initialize" first action on other rounds to last value of last round,
        // i.e. do nothing and keep the old value.
        //
        (((betround() != ConstBetRoundPreflop()) && (BotsActionsOnThisRoundIncludingChecks() == 0)) ? 0 : 0)
        //
        // Then we call Decision and store BotsLastAction for the next time...
        //
        // Freeplay (Check) in case we want to Check/Call/Fold
        || (   (AmountToCall() == 0) 
        && (   (Decision() == Call())  || (Decision() == Play())
        || (Decision() == Check()) || (Decision() == Fold()))) ? me_st_MemBotsLastAction_Check() :
        // Call = Play
        (  (Decision() == Call())
        || (Decision() == Play())) ? me_st_MemBotsLastAction_Call() :
        // Fold
        (Decision() == Fold()) ? me_st_MemBotsLastAction_Fold() :
        // Bet, Raise, BetHalfPot, BetPot, etc.
        // Also handling fixed betsizes and peercentaged potsized bets.
        (  (Decision() == Raise())
        || (Decision() == RaiseMin())
        || (Decision() == RaiseHalfPot())
        || (Decision() == RaiseMax())
        || (Decision() == Bet())
        || (Decision() == BetMin())
        || (Decision() == BetHalfPot())
        || (Decision() == BetMax())
        || (DecisionIsPercentagedPotsizedBet() || DecisionIsFixedBetInBigBlinds())) ?
        ((ncallbets() == 0) ? me_st_MemBotsLastAction_Bet() : me_st_MemBotsLastAction_Raise()) :
        // Other cases: this should not happen
        // Handling them as "None"
        me_st_MemBotsLastAction_None());
}

double CalculateOpenPPL_BotsLastAction()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Just to be extra safe...
        ((betround() == ConstBetRoundPreflop()) && (BotsActionsOnThisRoundIncludingChecks() == 0)) ? None() :
        // Caring about a possible check, when a fold could not have happened
        (me_re_MemBotsLastAction() == Fold()) ? Check() :
        // The last action we executed (tried to execute, to be precise)
        me_re_MemBotsLastAction());
}

double CalculateOpenPPL_InitBotsLastPreflopAction()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Don't use BotsLastAction on the first orbit,
        // because that variable has to be the last one to be initialized
        // and is therefore undefined.
        ((betround() == ConstBetRoundPreflop()) && (BotsActionsOnThisRoundIncludingChecks() == 0)) ? 
        me_st_MemBotsLastPreflopAction_None() :
        ((betround() == ConstBetRoundPreflop()) && (BotsActionsOnThisRoundIncludingChecks() > 0)) ? 
        me_st_MemBotsLastPreflopAction_BotsLastAction() : 
        ((betround() == ConstBetRoundFlop()) && (BotsActionsOnThisRoundIncludingChecks() == 0)) ?
        me_st_MemBotsLastPreflopAction_BotsLastAction() : 0  );
}

double CalculateOpenPPL_BotsLastPreflopAction()    // Please revisit the return type. int or bool might be more appropriate
{
    return (me_re_MemBotsLastPreflopAction());
}

double CalculateOpenPPL_CalledOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return (didcallround2());
}

double CalculateOpenPPL_CalledOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return (didcallround3());
}

double CalculateOpenPPL_BotCheckedPreflop()    // Please revisit the return type. int or bool might be more appropriate
{
    return (didchecround1());
}

double CalculateOpenPPL_BotCheckedOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return (didchecround2());
}

double CalculateOpenPPL_BotCheckedOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return (didchecround3());
}

double CalculateOpenPPL_BotCheckedOnRiver()    // Please revisit the return type. int or bool might be more appropriate
{
    return (didchecround4());
}

double CalculateOpenPPL_AddNopponentscalling()    // Please revisit the return type. int or bool might be more appropriate
{
    return (Calls() + nopponentscalling());
}

double CalculateOpenPPL_InitCalls()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((BotsActionsOnThisRoundIncludingChecks() == 0) ?
        me_st_MemCalls_nopponentscalling() :
        me_st_MemCalls_AddNopponentscalling());
}

double CalculateOpenPPL_Calls()    // Please revisit the return type. int or bool might be more appropriate
{
    return (me_re_MemCalls());
}

double CalculateOpenPPL_PT_LastCaller_Chair()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Can't be big blind (0), as it is for the first orbit preflop only
        LastCallerPosition() == 1 ? SmallBlindChair() :
        LastCallerPosition() == 2 ? ButtonChair() :
        LastCallerPosition() == 3 ? CutOffChair() :
        LastCallerPosition() == 4 ? MiddlePosition3Chair() :
        LastCallerPosition() == 5 ? MiddlePosition2Chair() :
        LastCallerPosition() == 6 ? MiddlePosition1Chair() :
        LastCallerPosition() == 7 ? EarlyPosition3Chair() :
        LastCallerPosition() == 8 ? EarlyPosition2Chair() :
        LastCallerPosition() == 9 ? EarlyPosition1Chair() :
        HINT_No_LastCaller());
}

double CalculateOpenPPL_LastRaiserCurrentBet()    // Please revisit the return type. int or bool might be more appropriate
{
    return (raischair() == 0 ? currentbet0() :
        raischair() == 1 ? currentbet1() :
        raischair() == 2 ? currentbet2() :
        raischair() == 3 ? currentbet3() :
        raischair() == 4 ? currentbet4() :
        raischair() == 5 ? currentbet5() :
        raischair() == 6 ? currentbet6() :
        raischair() == 7 ? currentbet7() :
        raischair() == 8 ? currentbet8() :
        raischair() == 9 ? currentbet9() :
        HINT_There_Is_No_Raiser_Therefore_No_CurrentBet());
}

double CalculateOpenPPL_PlayersRaisingOrColdCallingTheLastRaise()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((currentbet0() == LastRaiserCurrentBet() ? 1 : 0)
        + (currentbet1() == LastRaiserCurrentBet() ? 1 : 0)
        + (currentbet2() == LastRaiserCurrentBet() ? 1 : 0)
        + (currentbet3() == LastRaiserCurrentBet() ? 1 : 0)
        + (currentbet4() == LastRaiserCurrentBet() ? 1 : 0)
        + (currentbet5() == LastRaiserCurrentBet() ? 1 : 0)
        + (currentbet6() == LastRaiserCurrentBet() ? 1 : 0)
        + (currentbet7() == LastRaiserCurrentBet() ? 1 : 0)
        + (currentbet8() == LastRaiserCurrentBet() ? 1 : 0)
        + (currentbet9() == LastRaiserCurrentBet() ? 1 : 0));
}

double CalculateOpenPPL_CallsSinceLastRaise()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((PlayersRaisingOrColdCallingTheLastRaise() - 1));
}

double CalculateOpenPPL_Folds()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((betround() == ConstBetRoundPreflop()) ? (nplayersdealt()  - nopponentsplaying() - 1) :
        (betround() == ConstBetRoundFlop())    ? (nplayersround2() - nopponentsplaying() - 1) :
        (betround() == ConstBetRoundTurn())    ? (nplayersround3() - nopponentsplaying() - 1) :
        (betround() == ConstBetRoundRiver())   ? (nplayersround4() - nopponentsplaying() - 1) :
        ERROR_Betround_Out_Of_Range());
}

double CalculateOpenPPL_ChecksAfterHero()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Valid second orbit only
        // All active players with a zero bet
        (BotsActionsOnThisRoundIncludingChecks() == 0) ? 0 :
        nopponentschecking());
}

double CalculateOpenPPL_ChecksInFrontOfHero()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// No checks preflop. Not even big blind can check (game over).
        // We ignore additional blind-posters however.
        (betround() == ConstBetRoundPreflop()) ? 0 :
        // I did already act, so no longer any posters in front of me
        (currentbet() > 0) ? 0 :
        // A bet to call, subtract all players with a non-zero bet in front of us
        (AmountToCall() > 0) ? (betposition() - nopponentsbetting() - 1):
        // All players before us checked, not even open-folds can throw this logic off.
        (betposition() - 1));
}

double CalculateOpenPPL_CalculateChecks()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((BotsActionsOnThisRoundIncludingChecks() == 0) ? ChecksInFrontOfHero() :
        // Stored value of checks on previous orbit plus players behind
        (BotsActionsOnThisRoundIncludingChecks() == 1) ? (Checks() + ChecksAfterHero()) :
        // There can no longer be any checking players, so don't change anything
        HINT_Nothing_To_Do_But_All_OK());
}

double CalculateOpenPPL_InitChecks()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((BotsActionsOnThisRound() <= 1) ? me_st_MemChecks_CalculateChecks() : 
        HINT_Nothing_To_Do_But_All_OK());
}

double CalculateOpenPPL_Checks()    // Please revisit the return type. int or bool might be more appropriate
{
    return (me_re_MemChecks());
}

double CalculateOpenPPL_DealPosition1Chair()    // Please revisit the return type. int or bool might be more appropriate
{
    return (ac_dealpos0() == 1 ? 0 :
        ac_dealpos1() == 1 ? 1 :
        ac_dealpos2() == 1 ? 2 :
        ac_dealpos3() == 1 ? 3 :
        ac_dealpos4() == 1 ? 4 :
        ac_dealpos5() == 1 ? 5 :
        ac_dealpos6() == 1 ? 6 :
        ac_dealpos7() == 1 ? 7 :
        ac_dealpos8() == 1 ? 8 :
        ac_dealpos9() == 1 ? 9 :
        ERROR_DealPosition1Chair_Not_Recognized__Check_Your_TableMap());
}

double CalculateOpenPPL_CurrentbetOfDealPosition1Chair()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((DealPosition1Chair() == 0) ? currentbet0() :
        (DealPosition1Chair() == 1) ? currentbet1() :
        (DealPosition1Chair() == 2) ? currentbet2() :
        (DealPosition1Chair() == 3) ? currentbet3() :
        (DealPosition1Chair() == 4) ? currentbet4() :
        (DealPosition1Chair() == 5) ? currentbet5() :
        (DealPosition1Chair() == 6) ? currentbet6() :
        (DealPosition1Chair() == 7) ? currentbet7() :
        (DealPosition1Chair() == 8) ? currentbet8() :
        (DealPosition1Chair() == 9) ? currentbet9() :
        ERROR_DealPosition1Chair_Not_Recognized__Check_Your_TableMap());
}

double CalculateOpenPPL_PlayersBehindDealPosition2ChairDidAct()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Needed for dectection of missing small blind.
        // First checking for folded players,
        // because lots ofnewbies won't scrape-players correctly and mix everything up.
        // This affects calculation of small blind, big blind,
        // all positions and finally lots of other things...
        (nopponentsfolded() > 0) ? True() :
        // Then check for active players with a zero bet
        ((playersplayingbits() & 0b0000000001()) && (currentbet0() == 0)) ? False() :
        ((playersplayingbits() & 0b0000000010()) && (currentbet1() == 0)) ? False() :
        ((playersplayingbits() & 0b0000000100()) && (currentbet2() == 0)) ? False() :
        ((playersplayingbits() & 0b0000001000()) && (currentbet3() == 0)) ? False() :
        ((playersplayingbits() & 0b0000010000()) && (currentbet4() == 0)) ? False() :
        ((playersplayingbits() & 0b0000100000()) && (currentbet5() == 0)) ? False() :
        ((playersplayingbits() & 0b0001000000()) && (currentbet6() == 0)) ? False() :
        ((playersplayingbits() & 0b0010000000()) && (currentbet7() == 0)) ? False() :
        ((playersplayingbits() & 0b0100000000()) && (currentbet8() == 0)) ? False() :
        ((playersplayingbits() & 0b1000000000()) && (currentbet9() == 0)) ? False() :
        True());
}

double CalculateOpenPPL_CalculateMissingSmallBlind()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// True, if SB is missing, false otherwise
        // Should be called at our first action preflop only.
        (CurrentbetOfDealPosition1Chair() == sblind()) ? False() :
        // If we are NOT the 2nd player to be dealt and see a bet 
        // of 1 big blind left to the dealer, then it is the big blind (SB missing)
        ((dealposition() != 2) && (CurrentbetOfDealPosition1Chair() == bblind())) ? True() :
        // Problematic is only the case when I am in "big blind" (2nd player to be dealt)
        // * if bet of DealPosition1Chair > 1 bblind then SB raised and is present
        // * if bet = 1 big blind and players "behind me" did act, then SB is present and limped
        // * if bet = 1 big blind and players behind me still to act, then SB is missing
        ((dealposition() == 2) && (CurrentbetOfDealPosition1Chair() > bblind())) ? False() :
        // Case 2 and 3: precondition: bet = 1 big blind
        ((dealposition() == 2) && PlayersBehindDealPosition2ChairDidAct()) ? False() :
        ((dealposition() == 2) && !PlayersBehindDealPosition2ChairDidAct()) ? True() :
        ERROR_THIS_SHOULD_NOT_HAPPEN__THERE_SEEMS_TO_BE_A_PROBLEM_IN_THE_LIBRARY());
}

double CalculateOpenPPL_InitMissingSmallBlind()    // Please revisit the return type. int or bool might be more appropriate
{
    return (((betround() == ConstBetRoundPreflop()) &&  (BotsActionsOnThisRoundIncludingChecks() == 0)) ? 
        me_st_MemMissingSmallBlind_CalculateMissingSmallBlind() :
        HINT_Nothing_To_Do_But_All_OK());
}

double CalculateOpenPPL_MissingSmallBlind()    // Please revisit the return type. int or bool might be more appropriate
{
    return (me_re_MemMissingSmallBlind());
}

double CalculateOpenPPL_NoBettingOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((nbetsround2() == 0));
}

double CalculateOpenPPL_NoBettingOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((nbetsround3() == 0));
}

double CalculateOpenPPL_NoVillainBetOrRaisedOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return (((raisbits2() & ~(1<<userchair())) == 0));
}

double CalculateOpenPPL_NoVillainBetOrRaisedOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return (((raisbits3() & ~(1<<userchair())) == 0));
}

double CalculateOpenPPL_OpponentCalledOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((betround() > ConstBetRoundFlop())
        && ((didraisround2() + didswaground2()) == 1)
        && !didchecround2()
        && !didcallround2()
        && NoVillainBetOrRaisedOnFlop());
}

double CalculateOpenPPL_OpponentCalledOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((betround() > ConstBetRoundTurn())
        && ((didraisround3() + didswaground3()) == 1)
        && !didchecround3()
        && !didcallround3()
        && NoVillainBetOrRaisedOnTurn());
}

double CalculateOpenPPL_OpponentStacksizeHeadsUp()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((bblind()==0) ? ERROR_Blinds_Not_Recognized_Correctly__Check_Your_TableMap():
        (nopponentsplaying() == NumberOfOpponentsAllin()) ? HINT_All_Opponents_Allin() :
        (nopponentsplaying()-NumberOfOpponentsAllin() > 1) ? ERROR_Game_Is_Not_HeadsUp() :
        ((playersplayingbits() & 0b0000000001()) && (nopponentsplaying()-NumberOfOpponentsAllin()==1) 
        && userchair()!=0 && balance0()>0) ? balance0()/bblind():
        ((playersplayingbits() & 0b0000000010()) && (nopponentsplaying()-NumberOfOpponentsAllin()==1) 
        && userchair()!=1 && balance1()>0) ? balance1()/bblind():
        ((playersplayingbits() & 0b0000000100()) && (nopponentsplaying()-NumberOfOpponentsAllin()==1) 
        && userchair()!=2 && balance2()>0) ? balance2()/bblind():
        ((playersplayingbits() & 0b0000001000()) && (nopponentsplaying()-NumberOfOpponentsAllin()==1) 
        && userchair()!=3 && balance3()>0) ? balance3()/bblind():
        ((playersplayingbits() & 0b0000010000()) && (nopponentsplaying()-NumberOfOpponentsAllin()==1) 
        && userchair()!=4 && balance4()>0) ? balance4()/bblind():
        ((playersplayingbits() & 0b0000100000()) && (nopponentsplaying()-NumberOfOpponentsAllin()==1) 
        && userchair()!=5 && balance5()>0) ? balance5()/bblind():
        ((playersplayingbits() & 0b0001000000()) && (nopponentsplaying()-NumberOfOpponentsAllin()==1) 
        && userchair()!=6 && balance6()>0) ? balance6()/bblind():
        ((playersplayingbits() & 0b0010000000()) && (nopponentsplaying()-NumberOfOpponentsAllin()==1) 
        && userchair()!=7 && balance7()>0) ? balance7()/bblind():
        ((playersplayingbits() & 0b0100000000()) && (nopponentsplaying()-NumberOfOpponentsAllin()==1) 
        && userchair()!=8 && balance8()>0) ? balance8()/bblind():
        ((playersplayingbits() & 0b1000000000()) && (nopponentsplaying()-NumberOfOpponentsAllin()==1) 
        && userchair()!=9 && balance9()>0) ? balance9()/bblind():
        ERROR_THIS_SHOULD_NOT_HAPPEN__THERE_SEEMS_TO_BE_A_PROBLEM_IN_THE_LIBRARY());
}


double CalculateOpenPPL_InitRaisesBeforeFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((betround() == ConstBetRoundPreflop()) ? me_st_MemRaisesBeforeFlop_Raises() : 
        HINT_Nothing_To_Do_But_All_OK());
}

double CalculateOpenPPL_InitRaisesOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((betround() == ConstBetRoundFlop()) ? me_st_MemRaisesOnFlop_Raises() : 
        HINT_Nothing_To_Do_But_All_OK());
}

double CalculateOpenPPL_InitRaisesOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((betround() == ConstBetRoundTurn()) ? me_st_MemRaisesOnTurn_Raises() : 
        HINT_Nothing_To_Do_But_All_OK());
}

double CalculateOpenPPL_NumberOfRaisesBeforeFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((betround() > ConstBetRoundPreflop()) ? me_re_MemRaisesBeforeFlop() : 0);
}

double CalculateOpenPPL_NumberOfRaisesOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((betround() > ConstBetRoundFlop()) ? me_re_MemRaisesOnFlop() : 
        HINT_Nothing_To_Do_But_All_OK());
}

double CalculateOpenPPL_NumberOfRaisesOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((betround() > ConstBetRoundTurn()) ? me_re_MemRaisesOnTurn() : 0);
}

double CalculateOpenPPL_Calculate_Raises()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((me_re_MemRaises() + RaisesSinceLastPlay()));
}

double CalculateOpenPPL_InitRaises()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((BotsActionsOnThisRoundIncludingChecks() == 0) ? me_st_MemRaises_RaisesSinceLastPlay() :
        me_st_MemRaises_Calculate_Raises());
}

double CalculateOpenPPL_Raises()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Raises may be -1 if there are additional blind-posters,
        // because nopponentsraising is wrong in that case.
        // But this has to be handled in OH 2.2.0.
        // For the moment we add some sanity-checks.
        ((me_re_MemRaises() < 0) && (betround() == ConstBetRoundPreflop()) && (ncallbets() == 1)) ? 
        (ERROR_Raises_Is_Negative__Trying_To_Adapt_In_A_Reasonable_Way() + 0) :
        ((me_re_MemRaises() < 0) && (betround() == ConstBetRoundPreflop()) && (ncallbets() > 1))  ? 
        (ERROR_Raises_Is_Negative__Trying_To_Adapt_In_A_Reasonable_Way() + 1) :
        // Should not happen postflop, but a sanity-check can't harm.
        ((me_re_MemRaises() < 0) && (betround() >= ConstBetRoundFlop())) ? 
        (ERROR_Raises_Is_Negative__Trying_To_Adapt_In_A_Reasonable_Way() + 0) :
        // Otherwise: probably everything ok
        me_re_MemRaises());
}

double CalculateOpenPPL_RaisesBeforeFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((NumberOfRaisesBeforeFlop() > 0));
}

double CalculateOpenPPL_RaisesOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((NumberOfRaisesOnFlop() > 0));
}

double CalculateOpenPPL_RaisesOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((NumberOfRaisesOnTurn() > 0));
}

double CalculateOpenPPL_RaisesBeforeOurFirstActionThisRoundPostflop()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((nopponentsraising() > 0) ? (nopponentsraising() - 1) : 0);
}

double CalculateOpenPPL_AntesPresent()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// First orbit preflop only,
        // as this function tries to guess the antes from the bets.
        ((currentbet0() > 0) && (currentbet0() < sblind()) && (balance0() > 0))
        || ((currentbet1() > 0) && (currentbet1() < sblind()) && (balance1() > 0))
        || ((currentbet2() > 0) && (currentbet2() < sblind()) && (balance2() > 0))
        || ((currentbet3() > 0) && (currentbet3() < sblind()) && (balance3() > 0))
        || ((currentbet4() > 0) && (currentbet4() < sblind()) && (balance4() > 0))
        || ((currentbet5() > 0) && (currentbet5() < sblind()) && (balance5() > 0))
        || ((currentbet6() > 0) && (currentbet6() < sblind()) && (balance6() > 0))
        || ((currentbet7() > 0) && (currentbet7() < sblind()) && (balance7() > 0))
        || ((currentbet8() > 0) && (currentbet8() < sblind()) && (balance8() > 0))
        || ((currentbet9() > 0) && (currentbet9() < sblind()) && (balance9() > 0)));
}

double CalculateOpenPPL_NumberOfBlindRaisersInThisGame()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((AntesPresent() ? 1 : 0) + (MissingSmallBlind() ? 1 : 2));
}

double CalculateOpenPPL_NumberOfOpponentsLimpingPreflop()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// First orbit preflop only
        // Does not count the user
        // Does not count the big-blind, who can't limp
        ((currentbet0() == bblind()) && (userchair() != 0) && (BigBlindChair() != 0) ? 1 : 0)
        + ((currentbet1() == bblind()) && (userchair() != 1) && (BigBlindChair() != 1) ? 1 : 0)
        + ((currentbet2() == bblind()) && (userchair() != 2) && (BigBlindChair() != 2) ? 1 : 0)
        + ((currentbet3() == bblind()) && (userchair() != 3) && (BigBlindChair() != 3) ? 1 : 0)
        + ((currentbet4() == bblind()) && (userchair() != 4) && (BigBlindChair() != 4) ? 1 : 0)
        + ((currentbet5() == bblind()) && (userchair() != 5) && (BigBlindChair() != 5) ? 1 : 0)
        + ((currentbet6() == bblind()) && (userchair() != 6) && (BigBlindChair() != 6) ? 1 : 0)
        + ((currentbet7() == bblind()) && (userchair() != 7) && (BigBlindChair() != 7) ? 1 : 0)
        + ((currentbet8() == bblind()) && (userchair() != 8) && (BigBlindChair() != 8) ? 1 : 0)
        + ((currentbet9() == bblind()) && (userchair() != 9) && (BigBlindChair() != 9) ? 1 : 0));
}

double CalculateOpenPPL_RaisesBeforeOurFirstAction()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((BotsActionsOnThisRoundIncludingChecks() > 0) ? 0 :
        (betround() > ConstBetRoundPreflop()) ? RaisesBeforeOurFirstActionThisRoundPostflop() :
        nopponentstruelyraising());
}

double CalculateOpenPPL_RaisesSinceLastPlayOnOurSecondAction()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// If we checked the first time the bettor sits behind us.
        (didchec() && (nopponentsraising() > 0) ) ? (nopponentsraising() - 1) :
        (didchec() && (nopponentsraising() == 0)) ? 0 :
        // Otherwise: either we were the first bettor
        // or there was a bet and maybe raises in front of us,
        // so all technical "raises" behind us are true raises
        nopponentsraising());
}

double CalculateOpenPPL_RaisesSinceLastPlayAfterOurSecondAction()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Now all technical raises are true raises, no bettors possible
        nopponentsraising());
}

double CalculateOpenPPL_RaisesSinceLastPlay()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((BotsActionsOnThisRoundIncludingChecks() == 0) ? RaisesBeforeOurFirstAction() :
        (BotsActionsOnThisRoundIncludingChecks() == 1) ? RaisesSinceLastPlayOnOurSecondAction() :
        (BotsActionsOnThisRoundIncludingChecks() >= 2) ? RaisesSinceLastPlayAfterOurSecondAction() :
        ERROR_BotsActionsOnThisRound_Out_Of_Range());
}


//*****************************************************************************
//
// OpenPPL - Betsizes and StackSizes - supporting technical functions
//
//*****************************************************************************
double CalculateOpenPPL_Chair0StartingStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Amount in bets (big blinds for preflop)
        (bet() > 0) ? ((balance0() + currentbet0()) / bet()) :
        ERROR_Blinds_Not_Recognized_Correctly__Check_Your_TableMap());
}

double CalculateOpenPPL_Chair1StartingStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Amount in bets (big blinds for preflop)
        (bet() > 0) ? ((balance1() + currentbet1()) / bet()) :
        ERROR_Blinds_Not_Recognized_Correctly__Check_Your_TableMap());
}

double CalculateOpenPPL_Chair2StartingStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Amount in bets (big blinds for preflop)
        (bet() > 0) ? ((balance2() + currentbet2()) / bet()) :
        ERROR_Blinds_Not_Recognized_Correctly__Check_Your_TableMap());
}

double CalculateOpenPPL_Chair3StartingStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Amount in bets (big blinds for preflop)
        (bet() > 0) ? ((balance3() + currentbet3()) / bet()) :
        ERROR_Blinds_Not_Recognized_Correctly__Check_Your_TableMap());
}

double CalculateOpenPPL_Chair4StartingStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Amount in bets (big blinds for preflop)
        (bet() > 0) ? ((balance4() + currentbet4()) / bet()) :
        ERROR_Blinds_Not_Recognized_Correctly__Check_Your_TableMap());
}

double CalculateOpenPPL_Chair5StartingStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Amount in bets (big blinds for preflop)
        (bet() > 0) ? ((balance5() + currentbet5()) / bet()) :
        ERROR_Blinds_Not_Recognized_Correctly__Check_Your_TableMap());
}

double CalculateOpenPPL_Chair6StartingStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Amount in bets (big blinds for preflop)
        (bet() > 0) ? ((balance6() + currentbet6()) / bet()) :
        ERROR_Blinds_Not_Recognized_Correctly__Check_Your_TableMap());
}

double CalculateOpenPPL_Chair7StartingStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Amount in bets (big blinds for preflop)
        (bet() > 0) ? ((balance7() + currentbet7()) / bet()) :
        ERROR_Blinds_Not_Recognized_Correctly__Check_Your_TableMap());
}

double CalculateOpenPPL_Chair8StartingStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Amount in bets (big blinds for preflop)
        (bet() > 0) ? ((balance8() + currentbet8()) / bet()) :
        ERROR_Blinds_Not_Recognized_Correctly__Check_Your_TableMap());
}

double CalculateOpenPPL_Chair9StartingStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Amount in bets (big blinds for preflop)
        (bet() > 0) ? ((balance9() + currentbet9()) / bet()) :
        ERROR_Blinds_Not_Recognized_Correctly__Check_Your_TableMap());
}

double CalculateOpenPPL_ButtonStartingStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Amount in bets (big blinds for preflop)
        ButtonChair() == 0 ? Chair0StartingStackSize() :
        ButtonChair() == 1 ? Chair1StartingStackSize() :
        ButtonChair() == 2 ? Chair2StartingStackSize() :
        ButtonChair() == 3 ? Chair3StartingStackSize() :
        ButtonChair() == 4 ? Chair4StartingStackSize() :
        ButtonChair() == 5 ? Chair5StartingStackSize() :
        ButtonChair() == 6 ? Chair6StartingStackSize() :
        ButtonChair() == 7 ? Chair7StartingStackSize() :
        ButtonChair() == 8 ? Chair8StartingStackSize() :
        ButtonChair() == 9 ? Chair9StartingStackSize() :
        ERROR_ButtonChair_Not_Recognized__Check_Your_TableMap());
}

double CalculateOpenPPL_CutOffStartingStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Amount in bets (big blinds for preflop)
        CutOffChair() == 0 ? Chair0StartingStackSize() :
        CutOffChair() == 1 ? Chair1StartingStackSize() :
        CutOffChair() == 2 ? Chair2StartingStackSize() :
        CutOffChair() == 3 ? Chair3StartingStackSize() :
        CutOffChair() == 4 ? Chair4StartingStackSize() :
        CutOffChair() == 5 ? Chair5StartingStackSize() :
        CutOffChair() == 6 ? Chair6StartingStackSize() :
        CutOffChair() == 7 ? Chair7StartingStackSize() :
        CutOffChair() == 8 ? Chair8StartingStackSize() :
        CutOffChair() == 9 ? Chair9StartingStackSize() :
        ERROR_CutOffChair_Not_Recognized__Check_Your_TableMap());
}

double CalculateOpenPPL_MiddlePosition3StartingStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Amount in bets (big blinds for preflop)
        MiddlePosition3Chair() == 0 ? Chair0StartingStackSize() :
        MiddlePosition3Chair() == 1 ? Chair1StartingStackSize() :
        MiddlePosition3Chair() == 2 ? Chair2StartingStackSize() :
        MiddlePosition3Chair() == 3 ? Chair3StartingStackSize() :
        MiddlePosition3Chair() == 4 ? Chair4StartingStackSize() :
        MiddlePosition3Chair() == 5 ? Chair5StartingStackSize() :
        MiddlePosition3Chair() == 6 ? Chair6StartingStackSize() :
        MiddlePosition3Chair() == 7 ? Chair7StartingStackSize() :
        MiddlePosition3Chair() == 8 ? Chair8StartingStackSize() :
        MiddlePosition3Chair() == 9 ? Chair9StartingStackSize() :
        ERROR_MiddlePosition3Chair_Not_Recognized__Check_Your_TableMap());
}

double CalculateOpenPPL_MiddlePosition2StartingStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Amount in bets (big blinds for preflop)
        MiddlePosition2Chair() == 0 ? Chair0StartingStackSize() :
        MiddlePosition2Chair() == 1 ? Chair1StartingStackSize() :
        MiddlePosition2Chair() == 2 ? Chair2StartingStackSize() :
        MiddlePosition2Chair() == 3 ? Chair3StartingStackSize() :
        MiddlePosition2Chair() == 4 ? Chair4StartingStackSize() :
        MiddlePosition2Chair() == 5 ? Chair5StartingStackSize() :
        MiddlePosition2Chair() == 6 ? Chair6StartingStackSize() :
        MiddlePosition2Chair() == 7 ? Chair7StartingStackSize() :
        MiddlePosition2Chair() == 8 ? Chair8StartingStackSize() :
        MiddlePosition2Chair() == 9 ? Chair9StartingStackSize() :
        ERROR_MiddlePosition2Chair_Not_Recognized__Check_Your_TableMap());
}

double CalculateOpenPPL_MiddlePosition1StartingStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Amount in bets (big blinds for preflop)
        MiddlePosition1Chair() == 0 ? Chair0StartingStackSize() :
        MiddlePosition1Chair() == 1 ? Chair1StartingStackSize() :
        MiddlePosition1Chair() == 2 ? Chair2StartingStackSize() :
        MiddlePosition1Chair() == 3 ? Chair3StartingStackSize() :
        MiddlePosition1Chair() == 4 ? Chair4StartingStackSize() :
        MiddlePosition1Chair() == 5 ? Chair5StartingStackSize() :
        MiddlePosition1Chair() == 6 ? Chair6StartingStackSize() :
        MiddlePosition1Chair() == 7 ? Chair7StartingStackSize() :
        MiddlePosition1Chair() == 8 ? Chair8StartingStackSize() :
        MiddlePosition1Chair() == 9 ? Chair9StartingStackSize() :
        ERROR_MiddlePosition1Chair_Not_Recognized__Check_Your_TableMap());
}

double CalculateOpenPPL_EarlyPosition3StartingStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Amount in bets (big blinds for preflop)
        EarlyPosition3Chair() == 0 ? Chair0StartingStackSize() :
        EarlyPosition3Chair() == 1 ? Chair1StartingStackSize() :
        EarlyPosition3Chair() == 2 ? Chair2StartingStackSize() :
        EarlyPosition3Chair() == 3 ? Chair3StartingStackSize() :
        EarlyPosition3Chair() == 4 ? Chair4StartingStackSize() :
        EarlyPosition3Chair() == 5 ? Chair5StartingStackSize() :
        EarlyPosition3Chair() == 6 ? Chair6StartingStackSize() :
        EarlyPosition3Chair() == 7 ? Chair7StartingStackSize() :
        EarlyPosition3Chair() == 8 ? Chair8StartingStackSize() :
        EarlyPosition3Chair() == 9 ? Chair9StartingStackSize() :
        ERROR_EarlyPosition3Chair_Not_Recognized__Check_Your_TableMap());
}

double CalculateOpenPPL_EarlyPosition2StartingStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Amount in bets (big blinds for preflop)
        EarlyPosition2Chair() == 0 ? Chair0StartingStackSize() :
        EarlyPosition2Chair() == 1 ? Chair1StartingStackSize() :
        EarlyPosition2Chair() == 2 ? Chair2StartingStackSize() :
        EarlyPosition2Chair() == 3 ? Chair3StartingStackSize() :
        EarlyPosition2Chair() == 4 ? Chair4StartingStackSize() :
        EarlyPosition2Chair() == 5 ? Chair5StartingStackSize() :
        EarlyPosition2Chair() == 6 ? Chair6StartingStackSize() :
        EarlyPosition2Chair() == 7 ? Chair7StartingStackSize() :
        EarlyPosition2Chair() == 8 ? Chair8StartingStackSize() :
        EarlyPosition2Chair() == 9 ? Chair9StartingStackSize() :
        ERROR_EarlyPosition2Chair_Not_Recognized__Check_Your_TableMap());
}

double CalculateOpenPPL_EarlyPosition1StartingStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Amount in bets (big blinds for preflop)
        EarlyPosition1Chair() == 0 ? Chair0StartingStackSize() :
        EarlyPosition1Chair() == 1 ? Chair1StartingStackSize() :
        EarlyPosition1Chair() == 2 ? Chair2StartingStackSize() :
        EarlyPosition1Chair() == 3 ? Chair3StartingStackSize() :
        EarlyPosition1Chair() == 4 ? Chair4StartingStackSize() :
        EarlyPosition1Chair() == 5 ? Chair5StartingStackSize() :
        EarlyPosition1Chair() == 6 ? Chair6StartingStackSize() :
        EarlyPosition1Chair() == 7 ? Chair7StartingStackSize() :
        EarlyPosition1Chair() == 8 ? Chair8StartingStackSize() :
        EarlyPosition1Chair() == 9 ? Chair9StartingStackSize() :
        ERROR_EarlyPosition1Chair_Not_Recognized__Check_Your_TableMap());
}

double CalculateOpenPPL_BigBlindStartingStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Amount in bets (big blinds for preflop)
        BigBlindChair() == 0 ? Chair0StartingStackSize() :
        BigBlindChair() == 1 ? Chair1StartingStackSize() :
        BigBlindChair() == 2 ? Chair2StartingStackSize() :
        BigBlindChair() == 3 ? Chair3StartingStackSize() :
        BigBlindChair() == 4 ? Chair4StartingStackSize() :
        BigBlindChair() == 5 ? Chair5StartingStackSize() :
        BigBlindChair() == 6 ? Chair6StartingStackSize() :
        BigBlindChair() == 7 ? Chair7StartingStackSize() :
        BigBlindChair() == 8 ? Chair8StartingStackSize() :
        BigBlindChair() == 9 ? Chair9StartingStackSize() :
        ERROR_BigBlindChair_Not_Recognized__Check_Your_TableMap());
}

double CalculateOpenPPL_SmallBlindStartingStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Amount in bets (big blinds for preflop)
        SmallBlindChair() == 0 ? Chair0StartingStackSize() :
        SmallBlindChair() == 1 ? Chair1StartingStackSize() :
        SmallBlindChair() == 2 ? Chair2StartingStackSize() :
        SmallBlindChair() == 3 ? Chair3StartingStackSize() :
        SmallBlindChair() == 4 ? Chair4StartingStackSize() :
        SmallBlindChair() == 5 ? Chair5StartingStackSize() :
        SmallBlindChair() == 6 ? Chair6StartingStackSize() :
        SmallBlindChair() == 7 ? Chair7StartingStackSize() :
        SmallBlindChair() == 8 ? Chair8StartingStackSize() :
        SmallBlindChair() == 9 ? Chair9StartingStackSize() :
        ERROR_SmallBlindChair_Not_Recognized__Check_Your_TableMap());
}

double CalculateOpenPPL_BigBlindCurrentBet()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Amount in bets (big blinds for preflop)
        (bet() <= 0) ? ERROR_Blinds_Not_Recognized_Correctly__Check_Your_TableMap() :
        (BigBlindChair() == 0) ? (currentbet0() / bet()) :
        (BigBlindChair() == 1) ? (currentbet1() / bet()) :
        (BigBlindChair() == 2) ? (currentbet2() / bet()) :
        (BigBlindChair() == 3) ? (currentbet3() / bet()) :
        (BigBlindChair() == 4) ? (currentbet4() / bet()) :
        (BigBlindChair() == 5) ? (currentbet5() / bet()) :
        (BigBlindChair() == 6) ? (currentbet6() / bet()) :
        (BigBlindChair() == 7) ? (currentbet7() / bet()) :
        (BigBlindChair() == 8) ? (currentbet8() / bet()) :
        (BigBlindChair() == 9) ? (currentbet9() / bet()) :
        ERROR_BigBlindChair_Not_Recognized__Check_Your_TableMap());
}

double CalculateOpenPPL_SmallBlindCurrentBet()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Amount in bets (big blinds for preflop)
        (bet() <= 0) ? ERROR_Blinds_Not_Recognized_Correctly__Check_Your_TableMap() :
        (SmallBlindChair() == 0) ? (currentbet0() / bet()) :
        (SmallBlindChair() == 1) ? (currentbet1() / bet()) :
        (SmallBlindChair() == 2) ? (currentbet2() / bet()) :
        (SmallBlindChair() == 3) ? (currentbet3() / bet()) :
        (SmallBlindChair() == 4) ? (currentbet4() / bet()) :
        (SmallBlindChair() == 5) ? (currentbet5() / bet()) :
        (SmallBlindChair() == 6) ? (currentbet6() / bet()) :
        (SmallBlindChair() == 7) ? (currentbet7() / bet()) :
        (SmallBlindChair() == 8) ? (currentbet8() / bet()) :
        (SmallBlindChair() == 9) ? (currentbet9() / bet()) :
        ERROR_SmallBlindChair_Not_Recognized__Check_Your_TableMap());
}

double CalculateOpenPPL_MaximumPossibleBetsizeIndollars()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((balance() + currentbet()));
}

double CalculateOpenPPL_SidePotInDollars()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Money in the side-pot, i.e. the money we don't compete for.
        ((currentbet0() > MaximumPossibleBetsizeIndollars()) ? currentbet0() - MaximumPossibleBetsizeIndollars() : 0)
        + ((currentbet1() > MaximumPossibleBetsizeIndollars()) ? currentbet1() - MaximumPossibleBetsizeIndollars() : 0)
        + ((currentbet2() > MaximumPossibleBetsizeIndollars()) ? currentbet2() - MaximumPossibleBetsizeIndollars() : 0)
        + ((currentbet3() > MaximumPossibleBetsizeIndollars()) ? currentbet3() - MaximumPossibleBetsizeIndollars() : 0)
        + ((currentbet4() > MaximumPossibleBetsizeIndollars()) ? currentbet4() - MaximumPossibleBetsizeIndollars() : 0)
        + ((currentbet5() > MaximumPossibleBetsizeIndollars()) ? currentbet5() - MaximumPossibleBetsizeIndollars() : 0)
        + ((currentbet6() > MaximumPossibleBetsizeIndollars()) ? currentbet6() - MaximumPossibleBetsizeIndollars() : 0)
        + ((currentbet7() > MaximumPossibleBetsizeIndollars()) ? currentbet7() - MaximumPossibleBetsizeIndollars() : 0)
        + ((currentbet8() > MaximumPossibleBetsizeIndollars()) ? currentbet8() - MaximumPossibleBetsizeIndollars() : 0)
        + ((currentbet9() > MaximumPossibleBetsizeIndollars()) ? currentbet9() - MaximumPossibleBetsizeIndollars() : 0));
}

double CalculateOpenPPL_SidePot()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Money in the side-pot, i.e. the money we don't compete for (measured in big blinds)
        (bblind() > 0) ? (SidePotInDollars() / bblind()) : 0);
}

double CalculateOpenPPL_MaxStillToActStackSizeAfterSmallBlind()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Only one player behind SB, so it is easy
        BigBlindStartingStackSize());
}

double CalculateOpenPPL_MaxStillToActStackSizeAfterButton()    // Please revisit the return type. int or bool might be more appropriate
{
    return (SmallBlindStartingStackSize() > MaxStillToActStackSizeAfterSmallBlind() ?
        SmallBlindStartingStackSize() : MaxStillToActStackSizeAfterSmallBlind());
}

double CalculateOpenPPL_MaxStillToActStackSizeAfterCutOff()    // Please revisit the return type. int or bool might be more appropriate
{
    return (ButtonStartingStackSize() > MaxStillToActStackSizeAfterButton() ?
        ButtonStartingStackSize() : MaxStillToActStackSizeAfterButton());
}

double CalculateOpenPPL_MaxStillToActStackSizeAfterMiddlePosition3()    // Please revisit the return type. int or bool might be more appropriate
{
    return (CutOffStartingStackSize() > MaxStillToActStackSizeAfterCutOff()?
        CutOffStartingStackSize() : MaxStillToActStackSizeAfterCutOff());
}

double CalculateOpenPPL_MaxStillToActStackSizeAfterMiddlePosition2()    // Please revisit the return type. int or bool might be more appropriate
{
    return (MiddlePosition3StartingStackSize() > MaxStillToActStackSizeAfterMiddlePosition3() ?
        MiddlePosition3StartingStackSize() : MaxStillToActStackSizeAfterMiddlePosition3());
}

double CalculateOpenPPL_MaxStillToActStackSizeAfterMiddlePosition1()    // Please revisit the return type. int or bool might be more appropriate
{
    return (MiddlePosition2StartingStackSize() > MaxStillToActStackSizeAfterMiddlePosition2() ?
        MiddlePosition2StartingStackSize() : MaxStillToActStackSizeAfterMiddlePosition2());
}

double CalculateOpenPPL_MaxStillToActStackSizeAfterEarlyPosition3()    // Please revisit the return type. int or bool might be more appropriate
{
    return (MiddlePosition1StartingStackSize() > MaxStillToActStackSizeAfterMiddlePosition1() ?
        MiddlePosition1StartingStackSize() : MaxStillToActStackSizeAfterMiddlePosition1());
}

double CalculateOpenPPL_MaxStillToActStackSizeAfterEarlyPosition2()    // Please revisit the return type. int or bool might be more appropriate
{
    return (EarlyPosition3StartingStackSize() > MaxStillToActStackSizeAfterEarlyPosition3() ?
        EarlyPosition3StartingStackSize() : MaxStillToActStackSizeAfterEarlyPosition3());
}

double CalculateOpenPPL_MaxStillToActStackSizeAfterEarlyPosition1()    // Please revisit the return type. int or bool might be more appropriate
{
    return (EarlyPosition2StartingStackSize() > MaxStillToActStackSizeAfterEarlyPosition2() ?
        EarlyPosition2StartingStackSize() : MaxStillToActStackSizeAfterEarlyPosition2());
}

double CalculateOpenPPL_MinStillToActStackSizeAfterSmallBlind()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Only one player behind SB, so it is easy
        BigBlindStartingStackSize());
}

double CalculateOpenPPL_MinStillToActStackSizeAfterButton()    // Please revisit the return type. int or bool might be more appropriate
{
    return (SmallBlindStartingStackSize() < MinStillToActStackSizeAfterSmallBlind() ?
        SmallBlindStartingStackSize() : MinStillToActStackSizeAfterSmallBlind());
}

double CalculateOpenPPL_MinStillToActStackSizeAfterCutOff()    // Please revisit the return type. int or bool might be more appropriate
{
    return (ButtonStartingStackSize() < MinStillToActStackSizeAfterButton() ?
        ButtonStartingStackSize() : MinStillToActStackSizeAfterButton());
}

double CalculateOpenPPL_MinStillToActStackSizeAfterMiddlePosition3()    // Please revisit the return type. int or bool might be more appropriate
{
    return (CutOffStartingStackSize() < MinStillToActStackSizeAfterCutOff() ?
        CutOffStartingStackSize() : MinStillToActStackSizeAfterCutOff());
}

double CalculateOpenPPL_MinStillToActStackSizeAfterMiddlePosition2()    // Please revisit the return type. int or bool might be more appropriate
{
    return (MiddlePosition3StartingStackSize() < MinStillToActStackSizeAfterMiddlePosition3() ?
        MiddlePosition3StartingStackSize() : MinStillToActStackSizeAfterMiddlePosition3());
}

double CalculateOpenPPL_MinStillToActStackSizeAfterMiddlePosition1()    // Please revisit the return type. int or bool might be more appropriate
{
    return (MiddlePosition2StartingStackSize() < MinStillToActStackSizeAfterMiddlePosition2() ?
        MiddlePosition2StartingStackSize() : MinStillToActStackSizeAfterMiddlePosition2());
}

double CalculateOpenPPL_MinStillToActStackSizeAfterEarlyPosition3()    // Please revisit the return type. int or bool might be more appropriate
{
    return (MiddlePosition1StartingStackSize() < MinStillToActStackSizeAfterMiddlePosition1() ?
        MiddlePosition1StartingStackSize() : MinStillToActStackSizeAfterMiddlePosition1());
}

double CalculateOpenPPL_MinStillToActStackSizeAfterEarlyPosition2()    // Please revisit the return type. int or bool might be more appropriate
{
    return (EarlyPosition3StartingStackSize() < MinStillToActStackSizeAfterEarlyPosition3() ?
        EarlyPosition3StartingStackSize() : MinStillToActStackSizeAfterEarlyPosition3());
}

double CalculateOpenPPL_MinStillToActStackSizeAfterEarlyPosition1()    // Please revisit the return type. int or bool might be more appropriate
{
    return (EarlyPosition2StartingStackSize() < MinStillToActStackSizeAfterEarlyPosition2() ?
        EarlyPosition2StartingStackSize() : MinStillToActStackSizeAfterEarlyPosition2());
}

double CalculateOpenPPL_InitMaxOpponentStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((betround() == ConstBetRoundPreflop()) ? me_st_MaxOpponentStackSize_MaxOpponentStackSizeCalculation() : 
        HINT_Nothing_To_Do_But_All_OK());
}

double CalculateOpenPPL_InitMinOpponentStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((betround() == ConstBetRoundPreflop()) ? me_st_MinOpponentStackSize_MinOpponentStackSizeCalculation() :
        HINT_Nothing_To_Do_But_All_OK());
}

double CalculateOpenPPL_MaxOpponentStackSizeCalculation()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((bblind() <= 0) ? ERROR_Blinds_Not_Recognized_Correctly__Check_Your_TableMap() :
        (balance_rank0() == (balance() + currentbet())) ? (balance_rank1() / bblind()) : (balance_rank0() / bblind()));
}

double CalculateOpenPPL_MinOpponentStackSizeCalculation()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((bblind() <= 0) ? ERROR_Blinds_Not_Recognized_Correctly__Check_Your_TableMap() :
        ( 1 / bblind() *
        ( nplayersdealt() == 10 ?
        (balance_rank9() == (balance() + currentbet()) ? balance_rank8() : balance_rank9()) :
        nplayersdealt() == 9 ?
        (balance_rank8() == (balance() + currentbet()) ? balance_rank7() : balance_rank8()) :
        nplayersdealt() == 8 ?
        (balance_rank7() == (balance() + currentbet()) ? balance_rank6() : balance_rank7()) :
        nplayersdealt() == 7 ?
        (balance_rank6() == (balance() + currentbet()) ? balance_rank5() : balance_rank6()) :
        nplayersdealt() == 6 ?
        (balance_rank5() == (balance() + currentbet()) ? balance_rank4() : balance_rank5()) :
        nplayersdealt() == 5 ?
        (balance_rank4() == (balance() + currentbet()) ? balance_rank3() : balance_rank4()) :
        nplayersdealt() == 4 ?
        (balance_rank3() == (balance() + currentbet()) ? balance_rank2() : balance_rank3()) :
        nplayersdealt() == 3 ?
        (balance_rank2() == (balance() + currentbet()) ? balance_rank1() : balance_rank2()) :
        nplayersdealt() == 2 ?
        (balance_rank1() == (balance() + currentbet()) ? balance_rank0() : balance_rank1()) :
        ERROR_No_Opponents_Detected__Check_Your_TableMap())));
}

double CalculateOpenPPL_OpponentsWithHigherStack()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// No need to tinker with memory symbols here,
        // this logic is valid for all betting rounds.
        (((balance0() + currentbet0()) > MaximumPossibleBetsizeIndollars()) ? 1: 0)
        + (((balance1() + currentbet1()) > MaximumPossibleBetsizeIndollars()) ? 1: 0)
        + (((balance2() + currentbet2()) > MaximumPossibleBetsizeIndollars()) ? 1: 0)
        + (((balance3() + currentbet3()) > MaximumPossibleBetsizeIndollars()) ? 1: 0)
        + (((balance4() + currentbet4()) > MaximumPossibleBetsizeIndollars()) ? 1: 0)
        + (((balance5() + currentbet5()) > MaximumPossibleBetsizeIndollars()) ? 1: 0)
        + (((balance6() + currentbet6()) > MaximumPossibleBetsizeIndollars()) ? 1: 0)
        + (((balance7() + currentbet7()) > MaximumPossibleBetsizeIndollars()) ? 1: 0)
        + (((balance8() + currentbet8()) > MaximumPossibleBetsizeIndollars()) ? 1: 0)
        + (((balance9() + currentbet9()) > MaximumPossibleBetsizeIndollars()) ? 1: 0));
}

double CalculateOpenPPL_CalculateStartingStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Precondition: first hand, preflop
        (bblind() > 0) ? ((currentbet() + balance()) / bblind()) : ERROR_Blinds_Not_Recognized_Correctly__Check_Your_TableMap());
}

double CalculateOpenPPL_InitStartingStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (((handsplayed() == 0 || StartingStackSize() == 0) && (betround() == ConstBetRoundPreflop())) ?
        me_st_MemStartingStackSize_CalculateStartingStackSize() : HINT_Nothing_To_Do_But_All_OK());
}

double CalculateOpenPPL_StartingStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (me_re_MemStartingStackSize());
}


//*****************************************************************************
//
// OpenPPL - Betsizes and StackSizes
//
//*****************************************************************************
double CalculateOpenPPL_DollarsToCall()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((call() <= balance()) ? call() : balance());
}

double CalculateOpenPPL_AmountToCall()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((bblind() > 0) ? (DollarsToCall() / bblind()) : ERROR_Blinds_Not_Recognized_Correctly__Check_Your_TableMap());
}

double CalculateOpenPPL_BetSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (ncallbets());
}

double CalculateOpenPPL_BigBlindSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bet1());
}

double CalculateOpenPPL_MaxOpponentStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (me_re_MaxOpponentStackSize());
}

double CalculateOpenPPL_MaxStillToActStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (userchair() == BigBlindChair()        ? 0 :
        userchair() == SmallBlindChair()      ? MaxStillToActStackSizeAfterSmallBlind()      :
        userchair() == ButtonChair()          ? MaxStillToActStackSizeAfterButton()          :
        userchair() == CutOffChair()          ? MaxStillToActStackSizeAfterCutOff()          :
        userchair() == MiddlePosition3Chair() ? MaxStillToActStackSizeAfterMiddlePosition3() :
        userchair() == MiddlePosition2Chair() ? MaxStillToActStackSizeAfterMiddlePosition2() :
        userchair() == MiddlePosition1Chair() ? MaxStillToActStackSizeAfterMiddlePosition1() :
        userchair() == EarlyPosition3Chair()  ? MaxStillToActStackSizeAfterEarlyPosition3()  :
        userchair() == EarlyPosition2Chair()  ? MaxStillToActStackSizeAfterEarlyPosition2()  :
        userchair() == EarlyPosition1Chair()  ? MaxStillToActStackSizeAfterEarlyPosition1()  :
        HINT_Something_Is_Wrong__It_Seems_You_Are_Not_Even_Seated());
}

double CalculateOpenPPL_MinOpponentStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (me_re_MinOpponentStackSize());
}

double CalculateOpenPPL_MinStillToActStackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return (userchair() == BigBlindChair()        ? 0:
        userchair() == SmallBlindChair()      ? MinStillToActStackSizeAfterSmallBlind()      :
        userchair() == ButtonChair()          ? MinStillToActStackSizeAfterButton()          :
        userchair() == CutOffChair()          ? MinStillToActStackSizeAfterCutOff()          :
        userchair() == MiddlePosition3Chair() ? MinStillToActStackSizeAfterMiddlePosition3() :
        userchair() == MiddlePosition2Chair() ? MinStillToActStackSizeAfterMiddlePosition2() :
        userchair() == MiddlePosition1Chair() ? MinStillToActStackSizeAfterMiddlePosition1() :
        userchair() == EarlyPosition3Chair()  ? MinStillToActStackSizeAfterEarlyPosition3()  :
        userchair() == EarlyPosition2Chair()  ? MinStillToActStackSizeAfterEarlyPosition2()  :
        userchair() == EarlyPosition1Chair()  ? MinStillToActStackSizeAfterEarlyPosition1()  :
        HINT_Something_Is_Wrong__It_Seems_You_Are_Not_Even_Seated());
}

double CalculateOpenPPL_PotSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((bblind() > 0) ? (pot() / bblind()) - SidePot(): ERROR_Blinds_Not_Recognized_Correctly__Check_Your_TableMap());
}

double CalculateOpenPPL_StackSize()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((bblind() > 0) ? (balance() / bblind()) : ERROR_Blinds_Not_Recognized_Correctly__Check_Your_TableMap());
}

double CalculateOpenPPL_StackSizeAtBeginningOfCurrentRound()    // Please revisit the return type. int or bool might be more appropriate
{
    return (StackSize() + TotalInvestedThisRound());
}

double CalculateOpenPPL_TotalInvested()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((betround() == ConstBetRoundPreflop() && nbetsround()  > 0) ? ncurrentbets() :
        (betround() == ConstBetRoundFlop()    && nbetsround1() > 0) ? (nbetsround1() + ncurrentbets()) :
        (betround() == ConstBetRoundTurn()    && nbetsround1() > 0) ? (nbetsround1() + nbetsround2() + ncurrentbets()) :
        (betround() == ConstBetRoundRiver()   && nbetsround1() > 0) ? (nbetsround1() + nbetsround2() + nbetsround3() + ncurrentbets()) :
        ERROR_Betround_Out_Of_Range());
}

double CalculateOpenPPL_TotalInvestedThisRound()    // Please revisit the return type. int or bool might be more appropriate
{
    return (ncurrentbets());
}


//*****************************************************************************
//
// OpenPPL - Board Symbols
//
//*****************************************************************************
double CalculateOpenPPL_2ndBestBoardCard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Only considering individual ranks, paired cards get ignored this way
        // 14 (ace) can't happen, but anyway...
        (RankBitsCommonWithoutTopBit() & 0b100000000000000()) != 0 ? ConstCardAce()   :
        (RankBitsCommonWithoutTopBit() & 0b010000000000000()) != 0 ? ConstCardKing()  :
        (RankBitsCommonWithoutTopBit() & 0b001000000000000()) != 0 ? ConstCardQueen() :
        (RankBitsCommonWithoutTopBit() & 0b000100000000000()) != 0 ? ConstCardJack()  :
        (RankBitsCommonWithoutTopBit() & 0b000010000000000()) != 0 ? ConstCardTen()   :
        (RankBitsCommonWithoutTopBit() & 0b000001000000000()) != 0 ? ConstCardNine()  :
        (RankBitsCommonWithoutTopBit() & 0b000000100000000()) != 0 ? ConstCardEight() :
        (RankBitsCommonWithoutTopBit() & 0b000000010000000()) != 0 ? ConstCardSeven() :
        (RankBitsCommonWithoutTopBit() & 0b000000001000000()) != 0 ? ConstCardSix()   :
        (RankBitsCommonWithoutTopBit() & 0b000000000100000()) != 0 ? ConstCardFive()  :
        (RankBitsCommonWithoutTopBit() & 0b000000000010000()) != 0 ? ConstCardFour()  :
        (RankBitsCommonWithoutTopBit() & 0b000000000001000()) != 0 ? ConstCardThree() :
        (RankBitsCommonWithoutTopBit() & 0b000000000000100()) != 0 ? ConstCardTwo()   :
        // Last two bits are ace (low) and unused.
        // Both ignored.
        -1);
}

double CalculateOpenPPL_AcePresentOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((FirstFlopCard() == ConstCardAce()) || (SecondFlopCard() == ConstCardAce()) || (ThirdFlopCard() == ConstCardAce()));
}

double CalculateOpenPPL_AcePresentOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return (AcePresentOnFlop() || (TurnCard() == ConstCardAce()));
}

double CalculateOpenPPL_BestBoardCard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankhicommon());
}

double CalculateOpenPPL_DoubleGutShotOnBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (((rankbitscommon() >> 8) &0x7f()) == 0x5d() ? 1 :
        ((rankbitscommon() >> 7) &0x7f()) == 0x5d() ? 1 :
        ((rankbitscommon() >> 6) &0x7f()) == 0x5d() ? 1 :
        ((rankbitscommon() >> 5) &0x7f()) == 0x5d() ? 1 :
        ((rankbitscommon() >> 4) &0x7f()) == 0x5d() ? 1 :
        ((rankbitscommon() >> 3) &0x7f()) == 0x5d() ? 1 :
        ((rankbitscommon() >> 2) &0x7f()) == 0x5d() ? 1 :
        ((rankbitscommon() >> 1) &0x7f()) == 0x5d() ? 1 : 0);
}

double CalculateOpenPPL_ExactlyOneStraightPossibleOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((StraightPossibleOnFlop() && AcePresentOnFlop() && ((LowestFlopCard() >= 10) || (TopFlopCard() <= 5)))
        || (StraightPossibleOnFlop() && (TopFlopCard() - LowestFlopCard() == 4)));
}

double CalculateOpenPPL_FlopCardPairedonTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return (FirstFlopCard() == TurnCard()
        || SecondFlopCard() == TurnCard()
        || ThirdFlopCard() == TurnCard());
}

double CalculateOpenPPL_FlushPossible()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((nsuitedcommon() >= 3));
}

double CalculateOpenPPL_FlushOnBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (($$cs0() == $$cs1() && $$cs0() == $$cs2() && $$cs0() == $$cs3() && $$cs0() == $$cs4()));
}

double CalculateOpenPPL_FlushPossibleOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return (($$cs0() == $$cs1() && $$cs0() == $$cs2()));
}

double CalculateOpenPPL_FlushPossibleOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((  ($$cs0() == $$cs1() && $$cs0() == $$cs2())
        || ($$cs0() == $$cs1() && $$cs0() == $$cs3())
        || ($$cs0() == $$cs2() && $$cs0() == $$cs3())
        || ($$cs1() == $$cs2() && $$cs1() == $$cs3()) ));
}

double CalculateOpenPPL_FourCardsToWheelOnBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(rankbitscommon() & 0b0000000000111110()) >= 4);
}

double CalculateOpenPPL_FourOf1SuiteOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((($$cs0() == $$cs1()) && ($$cs2() == $$cs3()) && ($$cs0() == $$cs3())));
}

double CalculateOpenPPL_FullHouseOnBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((pokervalcommon() & fullhouse()));
}

double CalculateOpenPPL_HighestRankOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return (((FirstFlopCard() >= SecondFlopCard()) && (FirstFlopCard() >= ThirdFlopCard())) ? FirstFlopCard() :
        ((SecondFlopCard() >= FirstFlopCard()) && (SecondFlopCard() >= ThirdFlopCard())) ? SecondFlopCard() :
        ((ThirdFlopCard() >= FirstFlopCard()) && (ThirdFlopCard() >= SecondFlopCard())) ? ThirdFlopCard() :
        -1);
}

double CalculateOpenPPL_KingPresentOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((FirstFlopCard() == ConstCardKing())
        || (SecondFlopCard() == ConstCardKing())
        || (ThirdFlopCard() == ConstCardKing()));
}

double CalculateOpenPPL_LowCardsOnBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_countRankBitsCommonLowCardsOnly());
}

double CalculateOpenPPL_MoreThanOneStraightPossibleOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return (StraightPossibleOnFlop() && !ExactlyOneStraightPossibleOnFlop());
}

double CalculateOpenPPL_MoreThanOneStraightPossibleOnRiver()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((betround() == ConstBetRoundRiver() && (NumberOfStraightsPossibleOnRiver() == 1)));
}

double CalculateOpenPPL_RankBitsCommonRiver()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// As this can only be called at the river it is identical to rankbiscommon
        rankbitscommon());
}

double CalculateOpenPPL_NumberOfStraightsPossibleOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((bit_count()((RankBitsCommonTurn() >> 10) & 0x1f()) >= 3 ? 1 : 0)
        + (bit_count()((RankBitsCommonTurn() >>  9) & 0x1f()) >= 3 ? 1 : 0)
        + (bit_count()((RankBitsCommonTurn() >>  8) & 0x1f()) >= 3 ? 1 : 0)
        + (bit_count()((RankBitsCommonTurn() >>  7) & 0x1f()) >= 3 ? 1 : 0)
        + (bit_count()((RankBitsCommonTurn() >>  6) & 0x1f()) >= 3 ? 1 : 0)
        + (bit_count()((RankBitsCommonTurn() >>  5) & 0x1f()) >= 3 ? 1 : 0)
        + (bit_count()((RankBitsCommonTurn() >>  4) & 0x1f()) >= 3 ? 1 : 0)
        + (bit_count()((RankBitsCommonTurn() >>  3) & 0x1f()) >= 3 ? 1 : 0)
        + (bit_count()((RankBitsCommonTurn() >>  2) & 0x1f()) >= 3 ? 1 : 0)
        + (bit_count()((RankBitsCommonTurn() >>  1) & 0x1f()) >= 3 ? 1 : 0));
}

double CalculateOpenPPL_NumberOfStraightsPossibleOnRiver()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((bit_count()((RankBitsCommonRiver() >> 10) & 0x1f()) >= 3 ? 1 : 0)
        + (bit_count()((RankBitsCommonRiver() >>  9) & 0x1f()) >= 3 ? 1 : 0)
        + (bit_count()((RankBitsCommonRiver() >>  8) & 0x1f()) >= 3 ? 1 : 0)
        + (bit_count()((RankBitsCommonRiver() >>  7) & 0x1f()) >= 3 ? 1 : 0)
        + (bit_count()((RankBitsCommonRiver() >>  6) & 0x1f()) >= 3 ? 1 : 0)
        + (bit_count()((RankBitsCommonRiver() >>  5) & 0x1f()) >= 3 ? 1 : 0)
        + (bit_count()((RankBitsCommonRiver() >>  4) & 0x1f()) >= 3 ? 1 : 0)
        + (bit_count()((RankBitsCommonRiver() >>  3) & 0x1f()) >= 3 ? 1 : 0)
        + (bit_count()((RankBitsCommonRiver() >>  2) & 0x1f()) >= 3 ? 1 : 0)
        + (bit_count()((RankBitsCommonRiver() >>  1) & 0x1f()) >= 3 ? 1 : 0));
}

double CalculateOpenPPL_MoreThanOneOneCardStraightPossible()    // Please revisit the return type. int or bool might be more appropriate
{
    return (DoubleGutShotOnBoard() || OpenEndedStraightDrawOnBoard());
}

double CalculateOpenPPL_MoreThanOneStraightPossibleOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((NumberOfStraightsPossibleOnTurn() > 1));
}

double CalculateOpenPPL_NumberOfOneCardStraightsPossibleOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((OneCardStraightPossibleOnTurn_AceHigh()   ? 1 : 0)
        + (OneCardStraightPossibleOnTurn_KingHigh()  ? 1 : 0)
        + (OneCardStraightPossibleOnTurn_QueenHigh() ? 1 : 0)
        + (OneCardStraightPossibleOnTurn_JackHigh()  ? 1 : 0)
        + (OneCardStraightPossibleOnTurn_TenHigh()   ? 1 : 0)
        + (OneCardStraightPossibleOnTurn_NineHigh()  ? 1 : 0)
        + (OneCardStraightPossibleOnTurn_EightHigh() ? 1 : 0)
        + (OneCardStraightPossibleOnTurn_SevenHigh() ? 1 : 0)
        + (OneCardStraightPossibleOnTurn_SixHigh()   ? 1 : 0)
        + (OneCardStraightPossibleOnTurn_FiveHigh()  ? 1 : 0));
}

double CalculateOpenPPL_OneCardFlushPossible()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((nsuitedcommon() >= 4));
}

double CalculateOpenPPL_OneCardStraightFlushPossible()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((nstraightflushfillcommon() <= 1));
}

double CalculateOpenPPL_StraightFlushOnBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((nstraightflushfillcommon() ==0));
}

double CalculateOpenPPL_OneCardStraightPossible()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((nstraightfillcommon() <= 1));
}

double CalculateOpenPPL_OneCardStraightPossibleOnTurn_AceHigh()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((bit_count()(RankBitsCommonTurn() & 0b111110000000000()) >= 4)   );
}

double CalculateOpenPPL_OneCardStraightPossibleOnTurn_KingHigh()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((bit_count()(RankBitsCommonTurn() & 0b011111000000000()) >= 4)  );
}

double CalculateOpenPPL_OneCardStraightPossibleOnTurn_QueenHigh()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((bit_count()(RankBitsCommonTurn() & 0b001111100000000()) >= 4)  );
}

double CalculateOpenPPL_OneCardStraightPossibleOnTurn_JackHigh()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((bit_count()(RankBitsCommonTurn() & 0b000111110000000()) >= 4)  );
}

double CalculateOpenPPL_OneCardStraightPossibleOnTurn_TenHigh()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((bit_count()(RankBitsCommonTurn() & 0b000011111000000()) >= 4)  );
}

double CalculateOpenPPL_OneCardStraightPossibleOnTurn_NineHigh()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((bit_count()(RankBitsCommonTurn() & 0b000001111100000()) >= 4)  );
}

double CalculateOpenPPL_OneCardStraightPossibleOnTurn_EightHigh()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((bit_count()(RankBitsCommonTurn() & 0b000000111110000()) >= 4)   );
}

double CalculateOpenPPL_OneCardStraightPossibleOnTurn_SevenHigh()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((bit_count()(RankBitsCommonTurn() & 0b000000011111000()) >= 4)   );
}

double CalculateOpenPPL_OneCardStraightPossibleOnTurn_SixHigh()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((bit_count()(RankBitsCommonTurn() & 0b000000001111100()) >= 4)   );
}

double CalculateOpenPPL_OneCardStraightPossibleOnTurn_FiveHigh()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((bit_count()(RankBitsCommonTurn() & 0b000000000111110()) >= 4));
}

double CalculateOpenPPL_OneCardStraightPossibleOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((NumberOfOneCardStraightsPossibleOnTurn() >= 1));
}

double CalculateOpenPPL_Only1OneCardStraightPossible()    // Please revisit the return type. int or bool might be more appropriate
{
    return (OneCardStraightPossible() && !MoreThanOneOneCardStraightPossible());
}

double CalculateOpenPPL_OpenEndedStraightDrawOnBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((nstraightcommon() == 4 && (bit_count()((rankbitscommon() >> 10) & 0x1f()) < 4) && (bit_count()((rankbitscommon() >> 1) & 0x1f()) < 4)) ? 1 : 0);
}

double CalculateOpenPPL_OpenEndedStraightDrawPossibleOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return (((TopFlopCard() - 2ndTopFlopCard() <= 3) && (TopFlopCard() - 2ndTopFlopCard() > 0) && (TopFlopCard() != ConstCardAce()))
        || ((2ndTopFlopCard() - LowestFlopCard() <= 3) && (2ndTopFlopCard() - LowestFlopCard() >0) && (LowestFlopCard() >= ConstCardTwo())));
}

double CalculateOpenPPL_IndividualOverCardsOnBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Works like OvercardsOnBoard, except that it counts cards only once
        bit_count()(rankbitscommon() >> (rankhiplayer() + 1)));
}

double CalculateOpenPPL_OvercardsOnBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (((FirstFlopCard()  > rankhiplayer()) ? 1 : 0)
        + ((SecondFlopCard() > rankhiplayer()) ? 1 : 0)
        + ((ThirdFlopCard()  > rankhiplayer()) ? 1 : 0)
        + ((TurnCard()       > rankhiplayer()) ? 1 : 0)
        + ((RiverCard()      > rankhiplayer()) ? 1 : 0));
}

double CalculateOpenPPL_PairOnBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((nrankedcommon() >= 2));
}

double CalculateOpenPPL_PairOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((FirstFlopCard() == SecondFlopCard() || SecondFlopCard() == ThirdFlopCard() || FirstFlopCard() == ThirdFlopCard()));
}

double CalculateOpenPPL_LowestFlopCard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (FirstFlopCard()  <= SecondFlopCard() && FirstFlopCard()  <=  ThirdFlopCard()  ? FirstFlopCard() :
        SecondFlopCard() <= FirstFlopCard()  && SecondFlopCard() <=  ThirdFlopCard()  ? SecondFlopCard() :
        ThirdFlopCard()  <= FirstFlopCard()  && ThirdFlopCard()  <=  SecondFlopCard() ? ThirdFlopCard() : 
        -1);
}

double CalculateOpenPPL_LowestFlopCardPairedOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((LowestFlopCard() == TurnCard()) );
}

double CalculateOpenPPL_OnlyOneStraightPossible()    // Please revisit the return type. int or bool might be more appropriate
{
    return (StraightPossible()
        && !(MoreThanOneStraightPossibleOnFlop()
        || MoreThanOneStraightPossibleOnTurn()
        || MoreThanOneStraightPossibleOnRiver()));
}

double CalculateOpenPPL_PairOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return (PairOnFlop()
        || TopFlopCardPairedonTurn()
        || 2ndTopFlopCardPairedOnTurn()
        || LowestFlopCardPairedOnTurn());
}

double CalculateOpenPPL_QuadsOnBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((nrankedcommon() == 4));
}

double CalculateOpenPPL_QueenPresentOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((FirstFlopCard()==ConstCardQueen())
        || (SecondFlopCard()==ConstCardQueen())
        || (ThirdFlopCard()==ConstCardQueen()));
}

double CalculateOpenPPL_RankBitsCommonLowCardsOnly()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Bits 8..ace(low), last bit is unused
        (rankbitscommon() & 0b111111110()));
}

double CalculateOpenPPL_RiverCardisOvercardToBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((  RiverCard() > FirstFlopCard()
        && RiverCard() > SecondFlopCard()
        && RiverCard() > ThirdFlopCard()
        && RiverCard() > TurnCard() ));
}

double CalculateOpenPPL_2ndTopFlopCard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (PairOnFlop() ? 2ndTopFlopCard_PairOnFlop() :
        !PairOnFlop() ? 2ndTopFlopCard_UnpairedFlop() :
        -1);
}

double CalculateOpenPPL_2ndTopFlopCard_UnpairedFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return (!PairOnFlop() && FirstFlopCard()  >= SecondFlopCard() && FirstFlopCard()  >= ThirdFlopCard()  && SecondFlopCard() >= ThirdFlopCard()  ? SecondFlopCard() :
        !PairOnFlop() && FirstFlopCard()  >= SecondFlopCard() && FirstFlopCard()  >= ThirdFlopCard()  && SecondFlopCard() <= ThirdFlopCard()  ? ThirdFlopCard()  :
        !PairOnFlop() && SecondFlopCard() >= FirstFlopCard()  && SecondFlopCard() >= ThirdFlopCard()  && FirstFlopCard()  >= ThirdFlopCard()  ? FirstFlopCard()  :
        !PairOnFlop() && SecondFlopCard() >= FirstFlopCard()  && SecondFlopCard() >= ThirdFlopCard()  && FirstFlopCard()  <= ThirdFlopCard()  ? ThirdFlopCard()  :
        !PairOnFlop() && ThirdFlopCard()  >= FirstFlopCard()  && ThirdFlopCard()  >= SecondFlopCard() && FirstFlopCard()  >= SecondFlopCard() ? FirstFlopCard()  :
        !PairOnFlop() && ThirdFlopCard()  >= FirstFlopCard()  && ThirdFlopCard()  >= SecondFlopCard() && FirstFlopCard()  <= SecondFlopCard() ? SecondFlopCard() :
        -1);
}

double CalculateOpenPPL_2ndTopFlopCard_PairOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return (PairOnFlop() && FirstFlopCard()  == SecondFlopCard() && FirstFlopCard()  > ThirdFlopCard()  ? ThirdFlopCard() :
        PairOnFlop() && FirstFlopCard()  == SecondFlopCard() && FirstFlopCard()  < ThirdFlopCard()  ? FirstFlopCard() :
        PairOnFlop() && FirstFlopCard()  == ThirdFlopCard() && FirstFlopCard()  > SecondFlopCard()  ? SecondFlopCard() :
        PairOnFlop() && FirstFlopCard()  == ThirdFlopCard() && FirstFlopCard()  < SecondFlopCard()  ? FirstFlopCard() :
        PairOnFlop() && SecondFlopCard()  == ThirdFlopCard() && SecondFlopCard()  > FirstFlopCard()  ? FirstFlopCard() :
        PairOnFlop() && SecondFlopCard()  == ThirdFlopCard() && SecondFlopCard()  < FirstFlopCard()  ? SecondFlopCard() :
        -1);
}


// Shanky is a bit inconsistent with the naming
// So we provide both 2ndTopFlopCardPairedOnRiver and SecondTopFlopCardPairedOnRiver,
// same for turn. "SecondTopFlopCardPairedOnRiver" is the "standard" here.
double CalculateOpenPPL_SecondTopFlopCardPairedOnRiver()    // Please revisit the return type. int or bool might be more appropriate
{
    return (2ndTopFlopCardPairedOnRiver());
}

double CalculateOpenPPL_SecondTopFlopCardPairedOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return (2ndTopFlopCardPairedOnTurn());
}

double CalculateOpenPPL_2ndTopFlopCardPairedOnRiver()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((2ndTopFlopCard() == RiverCard()));
}

double CalculateOpenPPL_2ndTopFlopCardPairedOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((2ndTopFlopCard() == TurnCard()));
}

double CalculateOpenPPL_srankbitshero()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((($$ps0()==tsuit() ? (RankOfFirstHoleCard()!=ConstCardAce() ? 2**RankOfFirstHoleCard() : 0b100000000000010()) : 0)
        | ($$ps1()==tsuit() ? (RankOfSecondHoleCard()!=ConstCardAce() ? 2**RankOfSecondHoleCard() : 0b100000000000010()) : 0)));
}

double CalculateOpenPPL_srankavailable()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Available suited cards for other players,
        // possibly contributing to a flush or straight flush
        ~(srankbitscommon() | srankbitshero()));
}

double CalculateOpenPPL_StraightFlushPossibleByOthers()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((nstraightflushfillcommon() <= 2));
}

double CalculateOpenPPL_SuitsOnBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((betround() == ConstBetRoundFlop())  ? SuitsOnFlop() :
        (betround() == ConstBetRoundTurn())  ? SuitsOnTurn() :
        (betround() == ConstBetRoundRiver()) ? SuitsOnRiver() :
        HINT_SuitsOnBoard_Undefined_For_This_Betting_Round());
}

double CalculateOpenPPL_SuitsOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((betround() < ConstBetRoundFlop()) ? HINT_SuitsOnBoard_Undefined_For_This_Betting_Round() : 
        ( ($$cs0() == $$cs1() && $$cs1() == $$cs2()) ? 1 :
        (($$cs0() == $$cs1() && $$cs1() != $$cs2()) || ($$cs0() == $$cs2() && $$cs0() != $$cs1()) || ($$cs0() != $$cs1() && $$cs1() == $$cs2())) ? 2 :
        ($$cs0() != $$cs1() && $$cs1() != $$cs2() && $$cs0() != $$cs2()) ? 3 : ERROR_THIS_SHOULD_NOT_HAPPEN__THERE_SEEMS_TO_BE_A_PROBLEM_IN_THE_LIBRARY()));
}

double CalculateOpenPPL_SuitsOnRiver()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((betround() < ConstBetRoundRiver()) ? HINT_SuitsOnBoard_Undefined_For_This_Betting_Round() : 
        ( SuitsOnTurn()
        + ((($$cs4() != $$cs0()) && ($$cs4() != $$cs1()) && ($$cs4() != $$cs2()) && ($$cs4() != $$cs3())) ? 1 : 0)));
}

double CalculateOpenPPL_SuitsOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((betround() < ConstBetRoundTurn()) ? HINT_SuitsOnBoard_Undefined_For_This_Betting_Round() : 
        ( SuitsOnFlop() 
        + ((($$cs3() != $$cs0()) && ($$cs3() != $$cs1()) && ($$cs3() != $$cs2())) ? 1 : 0)));
}

double CalculateOpenPPL_StraightFlushPossible()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()((srankbitscommon() >> 10) & 0x1f()) >= 3 ? 1 :
        bit_count()((srankbitscommon() >> 9) & 0x1f()) >= 3 ? 1 :
        bit_count()((srankbitscommon() >> 8) & 0x1f()) >= 3 ? 1 :
        bit_count()((srankbitscommon() >> 7) & 0x1f()) >= 3 ? 1 :
        bit_count()((srankbitscommon() >> 6) & 0x1f()) >= 3 ? 1 :
        bit_count()((srankbitscommon() >> 5) & 0x1f()) >= 3 ? 1 :
        bit_count()((srankbitscommon() >> 4) & 0x1f()) >= 3 ? 1 :
        bit_count()((srankbitscommon() >> 3) & 0x1f()) >= 3 ? 1 :
        bit_count()((srankbitscommon() >> 2) & 0x1f()) >= 3 ? 1 :
        bit_count()((srankbitscommon() >> 1) & 0x1f()) >= 3 ? 1 : 0);
}

double CalculateOpenPPL_StraightOnBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((nstraightcommon() >= 5));
}

double CalculateOpenPPL_StraightPossible()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((nstraightfillcommon() <= 2));
}

double CalculateOpenPPL_RankBitsCommOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((FirstFlopCard() != ConstCardAce() ? 2**FirstFlopCard() : 0b100000000000010())
        | (SecondFlopCard() != ConstCardAce() ? 2**SecondFlopCard() : 0b100000000000010())
        | (ThirdFlopCard() != ConstCardAce() ? 2**ThirdFlopCard() : 0b100000000000010()));
}

double CalculateOpenPPL_RankBitsCommonTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((FirstFlopCard() != ConstCardAce() ? 2**FirstFlopCard() : 0b100000000000010())
        | (SecondFlopCard() != ConstCardAce() ? 2**SecondFlopCard() : 0b100000000000010())
        | (ThirdFlopCard() != ConstCardAce() ? 2**ThirdFlopCard() : 0b100000000000010())
        | (TurnCard() != ConstCardAce() ? 2**TurnCard() : 0b100000000000010()));
}

double CalculateOpenPPL_StraightPossibleOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((bit_count()((RankBitsCommOnFlop() >> 10) & 0x1f()) >= 3) ? 1 :
        (bit_count()((RankBitsCommOnFlop() >>  9) & 0x1f()) >= 3) ? 1 :
        (bit_count()((RankBitsCommOnFlop() >>  8) & 0x1f()) >= 3) ? 1 :
        (bit_count()((RankBitsCommOnFlop() >>  7) & 0x1f()) >= 3) ? 1 :
        (bit_count()((RankBitsCommOnFlop() >>  6) & 0x1f()) >= 3) ? 1 :
        (bit_count()((RankBitsCommOnFlop() >>  5) & 0x1f()) >= 3) ? 1 :
        (bit_count()((RankBitsCommOnFlop() >>  4) & 0x1f()) >= 3) ? 1 :
        (bit_count()((RankBitsCommOnFlop() >>  3) & 0x1f()) >= 3) ? 1 :
        (bit_count()((RankBitsCommOnFlop() >>  2) & 0x1f()) >= 3) ? 1 :
        (bit_count()((RankBitsCommOnFlop() >>  1) & 0x1f()) >= 3) ? 1 : 0);
}

double CalculateOpenPPL_StraightPossibleOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((bit_count()((RankBitsCommonTurn() >> 10) & 0x1f()) >= 3) ? 1 :
        (bit_count()((RankBitsCommonTurn() >>  9) & 0x1f()) >= 3) ? 1 :
        (bit_count()((RankBitsCommonTurn() >>  8) & 0x1f()) >= 3) ? 1 :
        (bit_count()((RankBitsCommonTurn() >>  7) & 0x1f()) >= 3) ? 1 :
        (bit_count()((RankBitsCommonTurn() >>  6) & 0x1f()) >= 3) ? 1 :
        (bit_count()((RankBitsCommonTurn() >>  5) & 0x1f()) >= 3) ? 1 :
        (bit_count()((RankBitsCommonTurn() >>  4) & 0x1f()) >= 3) ? 1 :
        (bit_count()((RankBitsCommonTurn() >>  3) & 0x1f()) >= 3) ? 1 :
        (bit_count()((RankBitsCommonTurn() >>  2) & 0x1f()) >= 3) ? 1 :
        (bit_count()((RankBitsCommonTurn() >>  1) & 0x1f()) >= 3) ? 1 : 0);
}

double CalculateOpenPPL_ThreeCardStraightOnBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (nstraightcommon() >= 3);
}

double CalculateOpenPPL_TopFlopCard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (FirstFlopCard() >= SecondFlopCard() && FirstFlopCard() >= ThirdFlopCard() ? FirstFlopCard() :
        SecondFlopCard() >= FirstFlopCard() && SecondFlopCard() >= ThirdFlopCard() ? SecondFlopCard() :
        ThirdFlopCard() >= FirstFlopCard() && ThirdFlopCard() >= SecondFlopCard() ? ThirdFlopCard() :
        ERROR_THIS_SHOULD_NOT_HAPPEN__THERE_SEEMS_TO_BE_A_PROBLEM_IN_THE_LIBRARY());
}

double CalculateOpenPPL_TopFlopCardPairedonRiver()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((TopFlopCard() == RiverCard()));
}

double CalculateOpenPPL_TopFlopCardPairedonTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((TopFlopCard() == TurnCard()));
}

double CalculateOpenPPL_TripsOnBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((nrankedcommon() >= 3));
}

double CalculateOpenPPL_TripsOnBoardOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((FirstFlopCard() == SecondFlopCard() && SecondFlopCard() == ThirdFlopCard())
        || (FirstFlopCard() == SecondFlopCard() && SecondFlopCard() == TurnCard())
        || (FirstFlopCard() == ThirdFlopCard() && ThirdFlopCard() == TurnCard())
        || (SecondFlopCard() == ThirdFlopCard() && ThirdFlopCard() == TurnCard()));
}

double CalculateOpenPPL_TurnCardisOvercardToBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((  TurnCard() > FirstFlopCard()
        && TurnCard() > SecondFlopCard()
        && TurnCard() > ThirdFlopCard() ));
}

double CalculateOpenPPL_TurnCardPaired()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((TurnCard() == RiverCard()));
}

double CalculateOpenPPL_TwoOfOneSuitPresentOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((($$cs0() == $$cs1()) || ($$cs0() == $$cs2()) || ($$cs1() == $$cs2())));
}

double CalculateOpenPPL_TwoPairOnBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((pokervalcommon() & twopair()) || (pokervalcommon() & fullhouse()));
}

double CalculateOpenPPL_TwoPairOnBoardOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((betround()>ConstBetRoundFlop() && $$cr0()==$$cr1() && $$cr2()==$$cr3() && $$cr0()!=$$cr2())
        || (betround()>ConstBetRoundFlop() && $$cr0()==$$cr2() && $$cr1()==$$cr3() && $$cr0()!=$$cr1())
        || (betround()>ConstBetRoundFlop() && $$cr0()==$$cr3() && $$cr1()==$$cr2() && $$cr0()!=$$cr1()));
}

double CalculateOpenPPL_UncoordinatedFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return (!PairOnFlop()
        // No possible flush: guaranteed by the next condition...
        // three different suits
        && (SuitsOnFlop() == 3)
        && !StraightPossibleOnFlop()
        && !OpenEndedStraightDrawPossibleOnFlop());
}

double CalculateOpenPPL_WheelPossible()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((bit_count()(rankbitscommon() & 0b111110()) >= 3));
}


//*****************************************************************************
//
// OpenPPL - Hand(Strength) Symbols
//
//*****************************************************************************
double CalculateOpenPPL_HadOverPairOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((  ispair()
        && RankOfFirstHoleCard() > FirstFlopCard()
        && RankOfFirstHoleCard() > SecondFlopCard()
        && RankOfFirstHoleCard() > ThirdFlopCard() ));
}

double CalculateOpenPPL_HadOverPairOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((  ispair()
        && RankOfFirstHoleCard() > FirstFlopCard()
        && RankOfFirstHoleCard() > SecondFlopCard()
        && RankOfFirstHoleCard() > ThirdFlopCard() 
        && RankOfFirstHoleCard() > TurnCard() );
}


double CalculateOpenPPL_Had2ndOverPairOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return (ispair()
        && rankhiplayer() < TopFlopCard()
        && rankhiplayer() > 2ndTopFlopCard());
}

double CalculateOpenPPL_Had2ndOverPairOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return (Had2ndOverPairOnFlop() && (($$cr3()==TopFlopCard())|| ($$cr3()<TopFlopCard())));
}

double CalculateOpenPPL_HadPairOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return (ispair() 
        || RankOfFirstHoleCard()  == FirstFlopCard() 
        || RankOfFirstHoleCard()  == SecondFlopCard() 
        || RankOfFirstHoleCard()  == ThirdFlopCard()  
        || RankOfSecondHoleCard() == FirstFlopCard() 
        || RankOfSecondHoleCard() == SecondFlopCard()  
        || RankOfSecondHoleCard() == ThirdFlopCard());
}

double CalculateOpenPPL_HadPairOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return (HadPairOnFlop() 
        || FlopCardPairedonTurn() 
        || RankOfFirstHoleCard() == TurnCard() 
        || RankOfSecondHoleCard() == TurnCard());
}

double CalculateOpenPPL_HadSetOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return (PairInHand() && (RankOfFirstHoleCard()==$$cr0() || RankOfFirstHoleCard()==$$cr1() || RankOfFirstHoleCard()==$$cr2()) );
}

double CalculateOpenPPL_HadSetOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return (HadSetOnFlop() || (PairInHand() && RankOfFirstHoleCard()==$$cr3()) );
}

double CalculateOpenPPL_HadTopPairOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return (((RankOfFirstHoleCard() == HighestRankOnFlop()) || (RankOfSecondHoleCard() == HighestRankOnFlop())));
}

double CalculateOpenPPL_Had2ndTopPairOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return (((RankOfFirstHoleCard() == 2ndTopFlopCard()) || (RankOfSecondHoleCard() == 2ndTopFlopCard())));
}


double CalculateOpenPPL_HighestRankOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return (((TurnCard() > HighestRankOnFlop()) ? TurnCard() : HighestRankOnFlop()));
}

double CalculateOpenPPL_HadTopPairOnTurn()    // Please revisit the return type. int or bool might be more appropriate
{
    return (( RankOfFirstHoleCard()==HighestRankOnTurn() || RankOfSecondHoleCard()==HighestRankOnTurn() ));
}

double CalculateOpenPPL_HadTwoPairOnFlop()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((RankOfFirstHoleCard() == FirstFlopCard() || RankOfFirstHoleCard() == SecondFlopCard() || RankOfFirstHoleCard() == ThirdFlopCard())
        && (RankOfSecondHoleCard() == SecondFlopCard() || RankOfSecondHoleCard() == SecondFlopCard() || RankOfSecondHoleCard() == ThirdFlopCard())
        && !ispair());
}

double CalculateOpenPPL_Have2ndBestKicker()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((NumberOfBetterKickers() == 1));
}

double CalculateOpenPPL_Have2ndBestKickerOrBetter()    // Please revisit the return type. int or bool might be more appropriate
{
    return (Have2ndBestKicker() || HaveBestKickerOrBetter());
}

double CalculateOpenPPL_Have2ndBestOverPair()    // Please revisit the return type. int or bool might be more appropriate
{
    return (($KK()
        && FirstFlopCard() < ConstCardKing()
        && SecondFlopCard() < ConstCardKing()
        && ThirdFlopCard() < ConstCardKing()
        && TurnCard() < ConstCardKing()
        && RiverCard() < ConstCardKing()));
}

double CalculateOpenPPL_Have2ndBestOverPairOrBetter()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((Have2ndBestOverPair() || HaveBestOverPairOrBetter()));
}

double CalculateOpenPPL_Have2ndNutFlush()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((HaveFlush() && (NumberOfUnknownSuitedOvercards() == 1)));
}

double CalculateOpenPPL_Have2ndNutFlushDraw()    // Please revisit the return type. int or bool might be more appropriate
{
    return (HaveFlushDraw() && (NumberOfUnknownSuitedOvercards() == 1));
}

double CalculateOpenPPL_HighCardOf2ndBestPossibleStraightIfBestStraightIsAceHigh()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(rankbitscommon() & 0b011111000000000()) >= 3 ? ConstCardKing()  :
        bit_count()(rankbitscommon() & 0b001111100000000()) >= 3 ? ConstCardQueen() :
        bit_count()(rankbitscommon() & 0b000111110000000()) >= 3 ? ConstCardJack()  :
        bit_count()(rankbitscommon() & 0b000011111000000()) >= 3 ? ConstCardTen()   :
        bit_count()(rankbitscommon() & 0b000001111100000()) >= 3 ? ConstCardNine()  :
        bit_count()(rankbitscommon() & 0b000000111110000()) >= 3 ? ConstCardEight() :
        bit_count()(rankbitscommon() & 0b000000011111000()) >= 3 ? ConstCardSeven() :
        bit_count()(rankbitscommon() & 0b000000001111100()) >= 3 ? ConstCardSix()   :
        bit_count()(rankbitscommon() & 0b000000000111110()) >= 3 ? ConstCardFive()  :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_HighCardOf2ndBestPossibleStraightIfBestStraightIsKingHigh()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(rankbitscommon() & 0b001111100000000()) >= 3 ? ConstCardQueen() :
        bit_count()(rankbitscommon() & 0b000111110000000()) >= 3 ? ConstCardJack()  :
        bit_count()(rankbitscommon() & 0b000011111000000()) >= 3 ? ConstCardTen()   :
        bit_count()(rankbitscommon() & 0b000001111100000()) >= 3 ? ConstCardNine()  :
        bit_count()(rankbitscommon() & 0b000000111110000()) >= 3 ? ConstCardEight() :
        bit_count()(rankbitscommon() & 0b000000011111000()) >= 3 ? ConstCardSeven() :
        bit_count()(rankbitscommon() & 0b000000001111100()) >= 3 ? ConstCardSix()   :
        bit_count()(rankbitscommon() & 0b000000000111110()) >= 3 ? ConstCardFive()  :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_HighCardOf2ndBestPossibleStraightIfBestStraightIsQueenHigh()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(rankbitscommon() & 0b000111110000000()) >= 3 ? ConstCardJack()  :
        bit_count()(rankbitscommon() & 0b000011111000000()) >= 3 ? ConstCardTen()   :
        bit_count()(rankbitscommon() & 0b000001111100000()) >= 3 ? ConstCardNine()  :
        bit_count()(rankbitscommon() & 0b000000111110000()) >= 3 ? ConstCardEight() :
        bit_count()(rankbitscommon() & 0b000000011111000()) >= 3 ? ConstCardSeven() :
        bit_count()(rankbitscommon() & 0b000000001111100()) >= 3 ? ConstCardSix()   :
        bit_count()(rankbitscommon() & 0b000000000111110()) >= 3 ? ConstCardFive()  :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_HighCardOf2ndBestPossibleStraightIfBestStraightIsJackHigh()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(rankbitscommon() & 0b000011111000000()) >= 3 ? ConstCardTen()   :
        bit_count()(rankbitscommon() & 0b000001111100000()) >= 3 ? ConstCardNine()  :
        bit_count()(rankbitscommon() & 0b000000111110000()) >= 3 ? ConstCardEight() :
        bit_count()(rankbitscommon() & 0b000000011111000()) >= 3 ? ConstCardSeven() :
        bit_count()(rankbitscommon() & 0b000000001111100()) >= 3 ? ConstCardSix()   :
        bit_count()(rankbitscommon() & 0b000000000111110()) >= 3 ? ConstCardFive()  :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_HighCardOf2ndBestPossibleStraightIfBestStraightIsTenHigh()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(rankbitscommon() & 0b000001111100000()) >= 3 ? ConstCardNine()  :
        bit_count()(rankbitscommon() & 0b000000111110000()) >= 3 ? ConstCardEight() :
        bit_count()(rankbitscommon() & 0b000000011111000()) >= 3 ? ConstCardSeven() :
        bit_count()(rankbitscommon() & 0b000000001111100()) >= 3 ? ConstCardSix()   :
        bit_count()(rankbitscommon() & 0b000000000111110()) >= 3 ? ConstCardFive()  :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_HighCardOf2ndBestPossibleStraightIfBestStraightIsNineHigh()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(rankbitscommon() & 0b000000111110000()) >= 3 ? ConstCardEight() :
        bit_count()(rankbitscommon() & 0b000000011111000()) >= 3 ? ConstCardSeven() :
        bit_count()(rankbitscommon() & 0b000000001111100()) >= 3 ? ConstCardSix()   :
        bit_count()(rankbitscommon() & 0b000000000111110()) >= 3 ? ConstCardFive()  :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_HighCardOf2ndBestPossibleStraightIfBestStraightIsEightHigh()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(rankbitscommon() & 0b000000011111000()) >= 3 ? ConstCardSeven() :
        bit_count()(rankbitscommon() & 0b000000001111100()) >= 3 ? ConstCardSix()   :
        bit_count()(rankbitscommon() & 0b000000000111110()) >= 3 ? ConstCardFive()  :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_HighCardOf2ndBestPossibleStraightIfBestStraightIsSevenHigh()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(rankbitscommon() & 0b000000001111100()) >= 3 ? ConstCardSix()  :
        bit_count()(rankbitscommon() & 0b000000000111110()) >= 3 ? ConstCardFive() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_HighCardOf2ndBestPossibleStraightIfBestStraightIsSixHigh()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(rankbitscommon() & 0b000000000111110()) >= 3 ? ConstCardFive() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_HighCardOfBestPossibleStraight()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(rankbitscommon() & 0b111110000000000()) >= 3 ? ConstCardAce()   :
        bit_count()(rankbitscommon() & 0b011111000000000()) >= 3 ? ConstCardKing()  :
        bit_count()(rankbitscommon() & 0b001111100000000()) >= 3 ? ConstCardQueen() :
        bit_count()(rankbitscommon() & 0b000111110000000()) >= 3 ? ConstCardJack()  :
        bit_count()(rankbitscommon() & 0b000011111000000()) >= 3 ? ConstCardTen()   :
        bit_count()(rankbitscommon() & 0b000001111100000()) >= 3 ? ConstCardNine()  :
        bit_count()(rankbitscommon() & 0b000000111110000()) >= 3 ? ConstCardEight() :
        bit_count()(rankbitscommon() & 0b000000011111000()) >= 3 ? ConstCardSeven() :
        bit_count()(rankbitscommon() & 0b000000001111100()) >= 3 ? ConstCardSix()   :
        bit_count()(rankbitscommon() & 0b000000000111110()) >= 3 ? ConstCardFive()  :
        -1);
}

double CalculateOpenPPL_HighCardOf2ndBestPossibleStraight()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((HighCardOfBestPossibleStraight() == ConstCardAce())   ? HighCardOf2ndBestPossibleStraightIfBestStraightIsAceHigh()   :
        (HighCardOfBestPossibleStraight() == ConstCardKing())  ? HighCardOf2ndBestPossibleStraightIfBestStraightIsKingHigh()  :
        (HighCardOfBestPossibleStraight() == ConstCardQueen()) ? HighCardOf2ndBestPossibleStraightIfBestStraightIsQueenHigh() :
        (HighCardOfBestPossibleStraight() == ConstCardJack())  ? HighCardOf2ndBestPossibleStraightIfBestStraightIsJackHigh()  :
        (HighCardOfBestPossibleStraight() == ConstCardTen())   ? HighCardOf2ndBestPossibleStraightIfBestStraightIsTenHigh()   :
        (HighCardOfBestPossibleStraight() == ConstCardNine())  ? HighCardOf2ndBestPossibleStraightIfBestStraightIsNineHigh()  :
        (HighCardOfBestPossibleStraight() == ConstCardEight()) ? HighCardOf2ndBestPossibleStraightIfBestStraightIsEightHigh() :
        (HighCardOfBestPossibleStraight() == ConstCardSeven()) ? HighCardOf2ndBestPossibleStraightIfBestStraightIsSevenHigh() :
        (HighCardOfBestPossibleStraight() == ConstCardSix())   ? HighCardOf2ndBestPossibleStraightIfBestStraightIsSixHigh()   :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_HighCardOfOurStraight()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Precondition: we have a straight
        bit_count()(rankbits() & 0b111110000000000()) == 5 ? ConstCardAce()   :
        bit_count()(rankbits() & 0b011111000000000()) == 5 ? ConstCardKing()  :
        bit_count()(rankbits() & 0b001111100000000()) == 5 ? ConstCardQueen() :
        bit_count()(rankbits() & 0b000111110000000()) == 5 ? ConstCardJack()  :
        bit_count()(rankbits() & 0b000011111000000()) == 5 ? ConstCardTen()   :
        bit_count()(rankbits() & 0b000001111100000()) == 5 ? ConstCardNine()  :
        bit_count()(rankbits() & 0b000000111110000()) == 5 ? ConstCardEight() :
        bit_count()(rankbits() & 0b000000011111000()) == 5 ? ConstCardSeven() :
        bit_count()(rankbits() & 0b000000001111100()) == 5 ? ConstCardSix()   :
        bit_count()(rankbits() & 0b000000000111110()) == 5 ? ConstCardFive()  :
        ERROR_We_Dont_Have_Any_Straight__Do_Not_Call_This_Function());
}

double CalculateOpenPPL_Have2ndNutStraight()    // Please revisit the return type. int or bool might be more appropriate
{
    return (HaveStraight()
        && (HighCardOf2ndBestPossibleStraight() == HighCardOfOurStraight()));
}

double CalculateOpenPPL_Have2ndOverPair()    // Please revisit the return type. int or bool might be more appropriate
{
    return (ispair() && (RankOfFirstHoleCard() < BestBoardCard()) && (RankOfFirstHoleCard() > 2ndBestBoardCard()));
}

double CalculateOpenPPL_Have2ndTopPair()    // Please revisit the return type. int or bool might be more appropriate
{
    return (RankOfFirstHoleCard() == 2ndBestBoardCard() || RankOfSecondHoleCard() == 2ndBestBoardCard());
}

double CalculateOpenPPL_Have2ndTopSet()    // Please revisit the return type. int or bool might be more appropriate
{
    return (ispair() && RankOfFirstHoleCard() == 2ndBestBoardCard());
}

double CalculateOpenPPL_TopBitOfRankBitsCommon()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((1 << rankhicommon()));
}

double CalculateOpenPPL_2ndTopBitOfRankBitsCommon()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((1 << 2ndBestBoardCard()));
}

double CalculateOpenPPL_3rdTopBitOfRankBitsCommon()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((1 << 3rdBestBoardCard()));
}

double CalculateOpenPPL_4thTopBitOfRankBitsCommon()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((1 << 4thBestBoardCard()));
}

double CalculateOpenPPL_5thTopBitOfRankBitsCommon()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((1 << 5thBestBoardCard()));
}

double CalculateOpenPPL_RankBitsCommonWithoutTopBit()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankbitscommon() & ~TopBitOfRankBitsCommon());
}

double CalculateOpenPPL_RankBitsCommonWithoutTopTwoBits()    // Please revisit the return type. int or bool might be more appropriate
{
    return (RankBitsCommonWithoutTopBit() & ~2ndTopBitOfRankBitsCommon());
}

double CalculateOpenPPL_RankBitsCommonWithoutTopThreeBits()    // Please revisit the return type. int or bool might be more appropriate
{
    return (RankBitsCommonWithoutTopTwoBits() & ~3rdTopBitOfRankBitsCommon());
}

double CalculateOpenPPL_RankBitsCommonWithoutTopFourBits()    // Please revisit the return type. int or bool might be more appropriate
{
    return (RankBitsCommonWithoutTopThreeBits() & ~4thTopBitOfRankBitsCommon());
}

double CalculateOpenPPL_Have3rdBestKicker()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((NumberOfBetterKickers() == 2));
}

double CalculateOpenPPL_Have3rdBestKickerOrBetter()    // Please revisit the return type. int or bool might be more appropriate
{
    return (Have3rdBestKicker() || Have2ndBestKickerOrBetter());
}

double CalculateOpenPPL_Have3rdBestOverPair()    // Please revisit the return type. int or bool might be more appropriate
{
    return ($QQ()
        && FirstFlopCard() < ConstCardQueen()
        && SecondFlopCard() < ConstCardQueen()
        && ThirdFlopCard() < ConstCardQueen()
        && TurnCard() < ConstCardQueen()
        && RiverCard() < ConstCardQueen());
}

double CalculateOpenPPL_3rdBestBoardCard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Only considering individual ranks, paired cards get ignored this way
        // 14 (ace) can't happen, but anyway...
        (RankBitsCommonWithoutTopTwoBits() & 0b100000000000000()) != 0 ? ConstCardAce() :
        (RankBitsCommonWithoutTopTwoBits() & 0b010000000000000()) != 0 ? ConstCardKing() :
        (RankBitsCommonWithoutTopTwoBits() & 0b001000000000000()) != 0 ? ConstCardQueen() :
        (RankBitsCommonWithoutTopTwoBits() & 0b000100000000000()) != 0 ? ConstCardJack() :
        (RankBitsCommonWithoutTopTwoBits() & 0b000010000000000()) != 0 ? ConstCardTen() :
        (RankBitsCommonWithoutTopTwoBits() & 0b000001000000000()) != 0 ? ConstCardNine() :
        (RankBitsCommonWithoutTopTwoBits() & 0b000000100000000()) != 0 ? ConstCardEight() :
        (RankBitsCommonWithoutTopTwoBits() & 0b000000010000000()) != 0 ? ConstCardSeven() :
        (RankBitsCommonWithoutTopTwoBits() & 0b000000001000000()) != 0 ? ConstCardSix() :
        (RankBitsCommonWithoutTopTwoBits() & 0b000000000100000()) != 0 ? ConstCardFive() :
        (RankBitsCommonWithoutTopTwoBits() & 0b000000000010000()) != 0 ? ConstCardFour() :
        (RankBitsCommonWithoutTopTwoBits() & 0b000000000001000()) != 0 ? ConstCardThree() :
        (RankBitsCommonWithoutTopTwoBits() & 0b000000000000100()) != 0 ? ConstCardTwo() :
        // Last two bits are ace (low) and unused.
        // Both ignored.
        -1);
}

double CalculateOpenPPL_Have3rdBestOverPairOrBetter()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((Have3rdBestOverPair() ||  Have2ndBestOverPairOrBetter()));
}

double CalculateOpenPPL_Have3rdNutFlush()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((HaveFlush() && (NumberOfUnknownSuitedOvercards() == 2)));
}

double CalculateOpenPPL_Have3rdNutFlushDraw()    // Please revisit the return type. int or bool might be more appropriate
{
    return (HaveFlushDraw() && (NumberOfUnknownSuitedOvercards() == 2));
}

double CalculateOpenPPL_Have3rdOverPair()    // Please revisit the return type. int or bool might be more appropriate
{
    return (ispair() && (RankOfFirstHoleCard() < 2ndBestBoardCard()) && (RankOfFirstHoleCard() > 3rdBestBoardCard()));
}

double CalculateOpenPPL_Have3rdTopPair()    // Please revisit the return type. int or bool might be more appropriate
{
    return (RankOfFirstHoleCard() == 3rdBestBoardCard() || RankOfSecondHoleCard() == 3rdBestBoardCard());
}

double CalculateOpenPPL_ThirdTopPairRank()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((RankOfFirstHoleCard() == 3rdBestBoardCard() || RankOfSecondHoleCard() == 3rdBestBoardCard()) ? 3rdBestBoardCard() :0);
}

double CalculateOpenPPL_Have5thTopPair()    // Please revisit the return type. int or bool might be more appropriate
{
    return (RankOfFirstHoleCard() == 5thBestBoardCard() || RankOfSecondHoleCard() == 5thBestBoardCard() );
}

double CalculateOpenPPL_FifthTopPairRank()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((RankOfFirstHoleCard() == 5thBestBoardCard() || RankOfSecondHoleCard() == 5thBestBoardCard()) ? 5thBestBoardCard(): 0);
}

double CalculateOpenPPL_Have3rdTopSet()    // Please revisit the return type. int or bool might be more appropriate
{
    return (ispair() && RankOfFirstHoleCard() == 3rdBestBoardCard());
}

double CalculateOpenPPL_Have4thNutFlush()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((HaveFlush() && (NumberOfUnknownSuitedOvercards() == 3)));
}

double CalculateOpenPPL_Have4thNutFlushDraw()    // Please revisit the return type. int or bool might be more appropriate
{
    return (HaveFlushDraw() && (NumberOfUnknownSuitedOvercards() == 3));
}

double CalculateOpenPPL_4thBestBoardCard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Only considering individual ranks, paired cards get ignored this way
        // 14 (ace) can't happen, but anyway...
        (RankBitsCommonWithoutTopThreeBits() & 0b100000000000000()) != 0 ? ConstCardAce() :
        (RankBitsCommonWithoutTopThreeBits() & 0b010000000000000()) != 0 ? ConstCardKing() :
        (RankBitsCommonWithoutTopThreeBits() & 0b001000000000000()) != 0 ? ConstCardQueen() :
        (RankBitsCommonWithoutTopThreeBits() & 0b000100000000000()) != 0 ? ConstCardJack() :
        (RankBitsCommonWithoutTopThreeBits() & 0b000010000000000()) != 0 ? ConstCardTen() :
        (RankBitsCommonWithoutTopThreeBits() & 0b000001000000000()) != 0 ? ConstCardNine() :
        (RankBitsCommonWithoutTopThreeBits() & 0b000000100000000()) != 0 ? ConstCardEight() :
        (RankBitsCommonWithoutTopThreeBits() & 0b000000010000000()) != 0 ? ConstCardSeven() :
        (RankBitsCommonWithoutTopThreeBits() & 0b000000001000000()) != 0 ? ConstCardSix() :
        (RankBitsCommonWithoutTopThreeBits() & 0b000000000100000()) != 0 ? ConstCardFive() :
        (RankBitsCommonWithoutTopThreeBits() & 0b000000000010000()) != 0 ? ConstCardFour() :
        (RankBitsCommonWithoutTopThreeBits() & 0b000000000001000()) != 0 ? ConstCardThree() :
        (RankBitsCommonWithoutTopThreeBits() & 0b000000000000100()) != 0 ? ConstCardTwo() :
        // Last two bits are ace (low) and unused.
        // Both ignored.
        -1);
}

double CalculateOpenPPL_Have4thOverPair()    // Please revisit the return type. int or bool might be more appropriate
{
    return (ispair() && (RankOfFirstHoleCard() < 3rdBestBoardCard()) && (RankOfFirstHoleCard() > 4thBestBoardCard()));
}

double CalculateOpenPPL_Have4thTopPair()    // Please revisit the return type. int or bool might be more appropriate
{
    return (RankOfFirstHoleCard() == 4thBestBoardCard() || RankOfSecondHoleCard() == 4thBestBoardCard());
}

double CalculateOpenPPL_ForthTopPairRank()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((RankOfFirstHoleCard() == 4thBestBoardCard() || RankOfSecondHoleCard() == 4thBestBoardCard()) ? 4thBestBoardCard() : 0);
}

double CalculateOpenPPL_Have4thTopSet()    // Please revisit the return type. int or bool might be more appropriate
{
    return (ispair() && RankOfFirstHoleCard() == 4thBestBoardCard());
}

double CalculateOpenPPL_Have5thNutFlush()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((HaveFlush() && (NumberOfUnknownSuitedOvercards() == 4)));
}

double CalculateOpenPPL_Have5thNutFlushDraw()    // Please revisit the return type. int or bool might be more appropriate
{
    return (HaveFlushDraw() && (NumberOfUnknownSuitedOvercards() == 4));
}

double CalculateOpenPPL_5thBestBoardCard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Only considering individual ranks, paired cards get ignored this way
        // 14 (ace) can't happen, but anyway...
        (RankBitsCommonWithoutTopFourBits() & 0b100000000000000()) != 0 ? ConstCardAce() :
        (RankBitsCommonWithoutTopFourBits() & 0b010000000000000()) != 0 ? ConstCardKing() :
        (RankBitsCommonWithoutTopFourBits() & 0b001000000000000()) != 0 ? ConstCardQueen() :
        (RankBitsCommonWithoutTopFourBits() & 0b000100000000000()) != 0 ? ConstCardJack() :
        (RankBitsCommonWithoutTopFourBits() & 0b000010000000000()) != 0 ? ConstCardTen() :
        (RankBitsCommonWithoutTopFourBits() & 0b000001000000000()) != 0 ? ConstCardNine() :
        (RankBitsCommonWithoutTopFourBits() & 0b000000100000000()) != 0 ? ConstCardEight() :
        (RankBitsCommonWithoutTopFourBits() & 0b000000010000000()) != 0 ? ConstCardSeven() :
        (RankBitsCommonWithoutTopFourBits() & 0b000000001000000()) != 0 ? ConstCardSix() :
        (RankBitsCommonWithoutTopFourBits() & 0b000000000100000()) != 0 ? ConstCardFive() :
        (RankBitsCommonWithoutTopFourBits() & 0b000000000010000()) != 0 ? ConstCardFour() :
        (RankBitsCommonWithoutTopFourBits() & 0b000000000001000()) != 0 ? ConstCardThree() :
        (RankBitsCommonWithoutTopFourBits() & 0b000000000000100()) != 0 ? ConstCardTwo() :
        // Last two bits are ace (low) and unused.
        // Both ignored.
        -1);
}

double CalculateOpenPPL_Have5thOverPair()    // Please revisit the return type. int or bool might be more appropriate
{
    return (ispair() && (RankOfFirstHoleCard() < 4thBestBoardCard()) && (RankOfFirstHoleCard() > 5thBestBoardCard()));
}

double CalculateOpenPPL_HaveAnyKindOfInsideStraightDraw()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Note: This does also count straightdraws at the board,
        // where we don't contribute any card. Special care should be taken
        // in HaveInsideStraightDraw
        (nstraightfill() <= 1));
}

double CalculateOpenPPL_HaveBackdoorFlushDrawContributingTwoCards()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((betround() == ConstBetRoundFlop())
        && (nsuited() >= 3)
        && ($$ps0() == tsuit())
        && ($$ps1() == tsuit()));
}

double CalculateOpenPPL_HaveBackdoorNutFlushDraw()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((betround() == ConstBetRoundFlop())
        && (nsuited() >= 3)
        && (nsuited() > nsuitedcommon()) 
        && (NumberOfUnknownSuitedOvercards() == 0));
}

double CalculateOpenPPL_HaveBackDoor2ndNutFlushDraw()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((betround() == ConstBetRoundFlop())
        && (nsuited() >= 3)
        && (nsuited() > nsuitedcommon()) 
        && (NumberOfUnknownSuitedOvercards() == 1));
}

double CalculateOpenPPL_HaveBackDoor3rdNutFlushDraw()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((betround() == ConstBetRoundFlop())
        && (nsuited() >= 3)
        && (nsuited() > nsuitedcommon()) 
        && (NumberOfUnknownSuitedOvercards() == 2));
}

double CalculateOpenPPL_HaveBackdoorFlushDraw()    // Please revisit the return type. int or bool might be more appropriate
{
    return (HaveBackdoorFlushDrawContributingTwoCards()
        || HaveBackdoorNutFlushDraw()
        || HaveBackDoor2ndNutFlushDraw());
}

double CalculateOpenPPL_HaveBestKicker()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((NumberOfBetterKickers() == 0));
}

double CalculateOpenPPL_HaveBestKickerOrBetter()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((HaveBestKicker() || HavePair()  || HaveOverPairOrBetter()));
}

double CalculateOpenPPL_HaveBestOverPair()    // Please revisit the return type. int or bool might be more appropriate
{
    return (($AA()
        && FirstFlopCard() < ConstCardAce()
        && SecondFlopCard() < ConstCardAce()
        && ThirdFlopCard() < ConstCardAce()
        && TurnCard() < ConstCardAce()
        && RiverCard() < ConstCardAce()));
}

double CalculateOpenPPL_HaveBestOverPairOrBetter()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((HaveBestOverPair()
        || HaveTwoPair()
        || HaveSet()
        || HaveTrips()
        || HaveStraight()
        || HaveFlush()
        || HaveFullHouse()
        || HaveQuads()
        || HaveStraightFlush()));
}

double CalculateOpenPPL_LowestBitOfRankBitsCommon()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((1<<ranklocommon()));
}

double CalculateOpenPPL_RankBitsCommonWithoutLowestBit()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// we negate lowest-bit and then do an and-operation
        rankbitscommon() & ~LowestBitOfRankBitsCommon());
}

double CalculateOpenPPL_LowestBoardCard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (ranklocommon());
}

double CalculateOpenPPL_2ndLowestBoardCard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((RankBitsCommonWithoutLowestBit() & 0b000000000000100()) != 1 ? ConstCardTwo()   :
        (RankBitsCommonWithoutLowestBit() & 0b000000000001000()) != 1 ? ConstCardThree() :
        (RankBitsCommonWithoutLowestBit() & 0b000000000010000()) != 1 ? ConstCardFour()  :
        (RankBitsCommonWithoutLowestBit() & 0b000000000100000()) != 1 ? ConstCardFive()  :
        (RankBitsCommonWithoutLowestBit() & 0b000000001000000()) != 1 ? ConstCardSix()   :
        (RankBitsCommonWithoutLowestBit() & 0b000000010000000()) != 1 ? ConstCardSeven() :
        (RankBitsCommonWithoutLowestBit() & 0b000000100000000()) != 1 ? ConstCardEight() :
        (RankBitsCommonWithoutLowestBit() & 0b000001000000000()) != 1 ? ConstCardNine()  :
        (RankBitsCommonWithoutLowestBit() & 0b000010000000000()) != 1 ? ConstCardTen()   :
        (RankBitsCommonWithoutLowestBit() & 0b000100000000000()) != 1 ? ConstCardJack()  :
        (RankBitsCommonWithoutLowestBit() & 0b001000000000000()) != 1 ? ConstCardQueen() :
        (RankBitsCommonWithoutLowestBit() & 0b010000000000000()) != 1 ? ConstCardKing()  :
        (RankBitsCommonWithoutLowestBit() & 0b100000000000000()) != 1 ? ConstCardAce()   :
        -1);
}

double CalculateOpenPPL_HaveBottomPair()    // Please revisit the return type. int or bool might be more appropriate
{
    return (( isonepair() && (rankhiplayer() == ranklocommon() || rankloplayer() == ranklocommon()) ));
}

double CalculateOpenPPL_HaveBottomSet()    // Please revisit the return type. int or bool might be more appropriate
{
    return (((nrankedcommon() == 1) && ispair() && isthreeofakind() && (rankhiplayer() == ranklocommon())));
}

double CalculateOpenPPL_HaveBottomTrips()    // Please revisit the return type. int or bool might be more appropriate
{
    return (($$pr0() == RankOfBottomPairOnBoard() || $$pr1() == RankOfBottomPairOnBoard()));
}

double CalculateOpenPPL_HaveBottomTwoPair()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((RankOfFirstHoleCard() == LowestBoardCard() && RankOfSecondHoleCard() == 2ndLowestBoardCard())
        || (RankOfFirstHoleCard() == 2ndLowestBoardCard() && RankOfSecondHoleCard() == LowestBoardCard()));
}

double CalculateOpenPPL_HaveDoubleGutshotDraw()    // Please revisit the return type. int or bool might be more appropriate
{
    return (`((rankbits >> 1) & 0b1011101) == 5  // A_345_7
        || `((rankbits >> 2) & 0b1011101) == 5  // 2_456_8
        || `((rankbits >> 3) & 0b1011101) == 5  // 3_567_9
        || `((rankbits >> 4) & 0b1011101) == 5  // 4_678_T
        || `((rankbits >> 5) & 0b1011101) == 5  // 5_789_J
        || `((rankbits >> 6) & 0b1011101) == 5  // 6_89T_Q
        || `((rankbits >> 7) & 0b1011101) == 5  // 8_9TJ_K
        || `((rankbits >> 8) & 0b1011101) == 5  // 9_TJQ_A
        || `((rankbits >> 1) & 0b11011011) == 6 // A2_45_78
        || `((rankbits >> 2) & 0b11011011) == 6 // 23_56_89
        || `((rankbits >> 3) & 0b11011011) == 6 // 34_67_9T
        || `((rankbits >> 4) & 0b11011011) == 6 // 45_78_TJ
        || `((rankbits >> 5) & 0b11011011) == 6 // 56_89_JQ
        || `((rankbits >> 6) & 0b11011011) == 6 // 67_9T_QK
        || `((rankbits >> 7) & 0b11011011) == 6 // 78_TJ_KA);
}

double CalculateOpenPPL_HaveFlush()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((nsuited() >= 5));
}

double CalculateOpenPPL_HaveFlushDraw()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((nsuited() >= 4));
}

double CalculateOpenPPL_HaveFullHouse()    // Please revisit the return type. int or bool might be more appropriate
{
    return (isfullhouse() || HaveQuads());
}

double CalculateOpenPPL_HaveNothing()    // Please revisit the return type. int or bool might be more appropriate
{
    return (!HavePair() && !(Overcards() == 2) && !HaveBestOverPairOrBetter() && !HaveBackdoorFlushDraw() && !HaveStraightDraw() && !HaveInsideStraightDraw() && !HaveFlushDraw()
        // && !HaveBackdoorStraightDraw);
}


double CalculateOpenPPL_HaveNutFlush()    // Please revisit the return type. int or bool might be more appropriate
{
    return (HaveNutStraightFlush()
        || (HaveFlush() && (NumberOfUnknownSuitedOvercards() == 0)));
}

double CalculateOpenPPL_HaveNutFlushCard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((NumberOfUnknownSuitedOvercards() == 0));
}

double CalculateOpenPPL_HaveNutFlushDraw()    // Please revisit the return type. int or bool might be more appropriate
{
    return (HaveNutStraightFlush()
        || (HaveFlushDraw() && (NumberOfUnknownSuitedOvercards() == 0)));
}


double CalculateOpenPPL_HaveNutStraight()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((ishistraight() || HaveNutStraightFlush()));
}

double CalculateOpenPPL_MyKicker()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((isfourofakind() || istwopair()) ? pokerval() & 0x0F() :
        isthreeofakind() ? (pokerval() >> 4) & 0x0F() :
        isonepair() ? (pokerval() >> 8) & 0x0F() :
        ERROR_You_Dont_Have_Any_Kicker());
}

double CalculateOpenPPL_NumberOfBetterKickers()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Cards better than my kicker...
        ((ConstCardAce() - MyKicker())
        // ...minus the cards in my 5-card-hand which are better than my kicker
        - (bit_count()(rankbitspoker() >> MyKicker()))));
}

double CalculateOpenPPL_RankBitsCommonWithAdditionalAce()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankbitscommon() | 0b100000000000010());
}

double CalculateOpenPPL_RankBitsCommonWithAdditionalKing()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankbitscommon() | 0b010000000000000());
}

double CalculateOpenPPL_RankBitsCommonWithAdditionalQueen()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankbitscommon() | 0b001000000000000());
}

double CalculateOpenPPL_RankBitsCommonWithAdditionalJack()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankbitscommon() | 0b000100000000000());
}

double CalculateOpenPPL_RankBitsCommonWithAdditionalTen()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankbitscommon() | 0b000010000000000());
}

double CalculateOpenPPL_RankBitsCommonWithAdditionalNine()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankbitscommon() | 0b000001000000000());
}

double CalculateOpenPPL_RankBitsCommonWithAdditionalEight()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankbitscommon() | 0b000000100000000());
}

double CalculateOpenPPL_RankBitsCommonWithAdditionalSeven()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankbitscommon() | 0b000000010000000());
}

double CalculateOpenPPL_RankBitsCommonWithAdditionalSix()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankbitscommon() | 0b000000001000000());
}

double CalculateOpenPPL_RankBitsCommonWithAdditionalFive()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankbitscommon() | 0b000000000100000());
}

double CalculateOpenPPL_RankBitsCommonWithAdditionalFour()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankbitscommon() | 0b000000000010000());
}

double CalculateOpenPPL_RankBitsCommonWithAdditionalThree()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankbitscommon() | 0b000000000001000());
}

double CalculateOpenPPL_RankBitsCommonWithAdditionalTwo()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankbitscommon() | 0b000000000000100());
}

double CalculateOpenPPL_BestPossibleStraightForVillainIfAceHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(RankBitsCommonWithAdditionalAce() & 0b111110000000000()) >= 3 ? ConstCardAce() :
        // We can ignore all straights without an ace here
        bit_count()(RankBitsCommonWithAdditionalAce() & 0b000000000111110()) >= 3 ? ConstCardFive() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_BestPossibleStraightForVillainIfKingHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(RankBitsCommonWithAdditionalKing() & 0b111110000000000()) >= 3 ? ConstCardAce() :
        bit_count()(RankBitsCommonWithAdditionalKing() & 0b011111000000000()) >= 3 ? ConstCardKing() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_BestPossibleStraightForVillainIfQueenHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(RankBitsCommonWithAdditionalQueen() & 0b111110000000000()) >= 3 ? ConstCardAce() :
        bit_count()(RankBitsCommonWithAdditionalQueen() & 0b011111000000000()) >= 3 ? ConstCardKing() :
        bit_count()(RankBitsCommonWithAdditionalQueen() & 0b001111100000000()) >= 3 ? ConstCardQueen() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_BestPossibleStraightForVillainIfJackHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(RankBitsCommonWithAdditionalJack() & 0b111110000000000()) >= 3 ? ConstCardAce() :
        bit_count()(RankBitsCommonWithAdditionalJack() & 0b011111000000000()) >= 3 ? ConstCardKing() :
        bit_count()(RankBitsCommonWithAdditionalJack() & 0b001111100000000()) >= 3 ? ConstCardQueen() :
        bit_count()(RankBitsCommonWithAdditionalJack() & 0b000111110000000()) >= 3 ? ConstCardJack() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_BestPossibleStraightForVillainIfTenHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(RankBitsCommonWithAdditionalTen() & 0b111110000000000()) >= 3 ? ConstCardAce() :
        bit_count()(RankBitsCommonWithAdditionalTen() & 0b011111000000000()) >= 3 ? ConstCardKing() :
        bit_count()(RankBitsCommonWithAdditionalTen() & 0b001111100000000()) >= 3 ? ConstCardQueen() :
        bit_count()(RankBitsCommonWithAdditionalTen() & 0b000111110000000()) >= 3 ? ConstCardJack() :
        bit_count()(RankBitsCommonWithAdditionalTen() & 0b000011111000000()) >= 3 ? ConstCardTen() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_BestPossibleStraightForVillainIfNineHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(RankBitsCommonWithAdditionalNine() & 0b011111000000000()) >= 3 ? ConstCardKing() :
        bit_count()(RankBitsCommonWithAdditionalNine() & 0b001111100000000()) >= 3 ? ConstCardQueen() :
        bit_count()(RankBitsCommonWithAdditionalNine() & 0b000111110000000()) >= 3 ? ConstCardJack() :
        bit_count()(RankBitsCommonWithAdditionalNine() & 0b000011111000000()) >= 3 ? ConstCardTen() :
        bit_count()(RankBitsCommonWithAdditionalNine() & 0b000001111100000()) >= 3 ? ConstCardNine() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_BestPossibleStraightForVillainIfEightHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(RankBitsCommonWithAdditionalEight() & 0b001111100000000()) >= 3 ? ConstCardQueen() :
        bit_count()(RankBitsCommonWithAdditionalEight() & 0b000111110000000()) >= 3 ? ConstCardJack() :
        bit_count()(RankBitsCommonWithAdditionalEight() & 0b000011111000000()) >= 3 ? ConstCardTen() :
        bit_count()(RankBitsCommonWithAdditionalEight() & 0b000001111100000()) >= 3 ? ConstCardNine() :
        bit_count()(RankBitsCommonWithAdditionalEight() & 0b000000111110000()) >= 3 ? ConstCardEight() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_BestPossibleStraightForVillainIfSevenHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(RankBitsCommonWithAdditionalSeven() & 0b000111110000000()) >= 3 ? ConstCardJack() :
        bit_count()(RankBitsCommonWithAdditionalSeven() & 0b000011111000000()) >= 3 ? ConstCardTen() :
        bit_count()(RankBitsCommonWithAdditionalSeven() & 0b000001111100000()) >= 3 ? ConstCardNine() :
        bit_count()(RankBitsCommonWithAdditionalSeven() & 0b000000111110000()) >= 3 ? ConstCardEight() :
        bit_count()(RankBitsCommonWithAdditionalSeven() & 0b000000011111000()) >= 3 ? ConstCardSeven() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_BestPossibleStraightForVillainIfSixHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(RankBitsCommonWithAdditionalSix() & 0b000011111000000()) >= 3 ? ConstCardTen() :
        bit_count()(RankBitsCommonWithAdditionalSix() & 0b000001111100000()) >= 3 ? ConstCardNine() :
        bit_count()(RankBitsCommonWithAdditionalSix() & 0b000000111110000()) >= 3 ? ConstCardEight() :
        bit_count()(RankBitsCommonWithAdditionalSix() & 0b000000011111000()) >= 3 ? ConstCardSeven() :
        bit_count()(RankBitsCommonWithAdditionalSix() & 0b000000001111100()) >= 3 ? ConstCardSix() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_BestPossibleStraightForVillainIfFiveHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(RankBitsCommonWithAdditionalFive() & 0b000001111100000()) >= 3 ? ConstCardNine() :
        bit_count()(RankBitsCommonWithAdditionalFive() & 0b000000111110000()) >= 3 ? ConstCardEight() :
        bit_count()(RankBitsCommonWithAdditionalFive() & 0b000000011111000()) >= 3 ? ConstCardSeven() :
        bit_count()(RankBitsCommonWithAdditionalFive() & 0b000000001111100()) >= 3 ? ConstCardSix() :
        bit_count()(RankBitsCommonWithAdditionalFive() & 0b000000000111110()) >= 3 ? ConstCardFive() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_BestPossibleStraightForVillainIfFourHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(RankBitsCommonWithAdditionalFour() & 0b000000111110000()) >= 3 ? ConstCardEight() :
        bit_count()(RankBitsCommonWithAdditionalFour() & 0b000000011111000()) >= 3 ? ConstCardSeven() :
        bit_count()(RankBitsCommonWithAdditionalFour() & 0b000000001111100()) >= 3 ? ConstCardSix() :
        bit_count()(RankBitsCommonWithAdditionalFour() & 0b000000000111110()) >= 3 ? ConstCardFive() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_BestPossibleStraightForVillainIfThreeHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(RankBitsCommonWithAdditionalThree() & 0b000000011111000()) >= 3 ? ConstCardSeven() :
        bit_count()(RankBitsCommonWithAdditionalThree() & 0b000000001111100()) >= 3 ? ConstCardSix() :
        bit_count()(RankBitsCommonWithAdditionalThree() & 0b000000000111110()) >= 3 ? ConstCardFive() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_BestPossibleStraightForVillainIfTwoHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(RankBitsCommonWithAdditionalTwo() & 0b000000001111100()) >= 3 ? ConstCardSix() :
        bit_count()(RankBitsCommonWithAdditionalTwo() & 0b000000000111110()) >= 3 ? ConstCardFive() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_RankBitsWithAdditionalAce()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankbits() | 0b100000000000010());
}

double CalculateOpenPPL_RankBitsWithAdditionalKing()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankbits() | 0b010000000000000());
}

double CalculateOpenPPL_RankBitsWithAdditionalQueen()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankbits() | 0b001000000000000());
}

double CalculateOpenPPL_RankBitsWithAdditionalJack()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankbits() | 0b000100000000000());
}

double CalculateOpenPPL_RankBitsWithAdditionalTen()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankbits() | 0b000010000000000());
}

double CalculateOpenPPL_RankBitsWithAdditionalNine()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankbits() | 0b000001000000000());
}

double CalculateOpenPPL_RankBitsWithAdditionalEight()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankbits() | 0b000000100000000());
}

double CalculateOpenPPL_RankBitsWithAdditionalSeven()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankbits() | 0b000000010000000());
}

double CalculateOpenPPL_RankBitsWithAdditionalSix()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankbits() | 0b000000001000000());
}

double CalculateOpenPPL_RankBitsWithAdditionalFive()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankbits() | 0b000000000100000());
}

double CalculateOpenPPL_RankBitsWithAdditionalFour()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankbits() | 0b000000000010000());
}

double CalculateOpenPPL_RankBitsWithAdditionalThree()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankbits() | 0b000000000001000());
}

double CalculateOpenPPL_RankBitsWithAdditionalTwo()    // Please revisit the return type. int or bool might be more appropriate
{
    return (rankbits() | 0b000000000000100());
}

double CalculateOpenPPL_BestPossibleStraightForUsIfAceHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(RankBitsWithAdditionalAce() & 0b111110000000000()) >= 5 ? ConstCardAce() :
        // We can ignore all straights without an ace here
        bit_count()(RankBitsWithAdditionalAce() & 0b000000000111110()) >= 5 ? ConstCardFive() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_BestPossibleStraightForUsIfKingHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(RankBitsWithAdditionalKing() & 0b111110000000000()) >= 5 ? ConstCardAce() :
        bit_count()(RankBitsWithAdditionalKing() & 0b011111000000000()) >= 5 ? ConstCardKing() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_BestPossibleStraightForUsIfQueenHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(RankBitsWithAdditionalQueen() & 0b111110000000000()) >= 5 ? ConstCardAce() :
        bit_count()(RankBitsWithAdditionalQueen() & 0b011111000000000()) >= 5 ? ConstCardKing() :
        bit_count()(RankBitsWithAdditionalQueen() & 0b001111100000000()) >= 5 ? ConstCardQueen() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_BestPossibleStraightForUsIfJackHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(RankBitsWithAdditionalJack() & 0b111110000000000()) >= 5 ? ConstCardAce() :
        bit_count()(RankBitsWithAdditionalJack() & 0b011111000000000()) >= 5 ? ConstCardKing() :
        bit_count()(RankBitsWithAdditionalJack() & 0b001111100000000()) >= 5 ? ConstCardQueen() :
        bit_count()(RankBitsWithAdditionalJack() & 0b000111110000000()) >= 5 ? ConstCardJack() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_BestPossibleStraightForUsIfTenHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(RankBitsWithAdditionalTen() & 0b111110000000000()) >= 5 ? ConstCardAce() :
        bit_count()(RankBitsWithAdditionalTen() & 0b011111000000000()) >= 5 ? ConstCardKing() :
        bit_count()(RankBitsWithAdditionalTen() & 0b001111100000000()) >= 5 ? ConstCardQueen() :
        bit_count()(RankBitsWithAdditionalTen() & 0b000111110000000()) >= 5 ? ConstCardJack() :
        bit_count()(RankBitsWithAdditionalTen() & 0b000011111000000()) >= 5 ? ConstCardTen() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_BestPossibleStraightForUsIfNineHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(RankBitsWithAdditionalNine() & 0b011111000000000()) >= 5 ? ConstCardKing() :
        bit_count()(RankBitsWithAdditionalNine() & 0b001111100000000()) >= 5 ? ConstCardQueen() :
        bit_count()(RankBitsWithAdditionalNine() & 0b000111110000000()) >= 5 ? ConstCardJack() :
        bit_count()(RankBitsWithAdditionalNine() & 0b000011111000000()) >= 5 ? ConstCardTen() :
        bit_count()(RankBitsWithAdditionalNine() & 0b000001111100000()) >= 5 ? ConstCardNine() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_BestPossibleStraightForUsIfEightHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(RankBitsWithAdditionalEight() & 0b001111100000000()) >= 5 ? ConstCardQueen() :
        bit_count()(RankBitsWithAdditionalEight() & 0b000111110000000()) >= 5 ? ConstCardJack() :
        bit_count()(RankBitsWithAdditionalEight() & 0b000011111000000()) >= 5 ? ConstCardTen() :
        bit_count()(RankBitsWithAdditionalEight() & 0b000001111100000()) >= 5 ? ConstCardNine() :
        bit_count()(RankBitsWithAdditionalEight() & 0b000000111110000()) >= 5 ? ConstCardEight() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_BestPossibleStraightForUsIfSevenHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(RankBitsWithAdditionalSeven() & 0b000111110000000()) >= 5 ? ConstCardJack() :
        bit_count()(RankBitsWithAdditionalSeven() & 0b000011111000000()) >= 5 ? ConstCardTen() :
        bit_count()(RankBitsWithAdditionalSeven() & 0b000001111100000()) >= 5 ? ConstCardNine() :
        bit_count()(RankBitsWithAdditionalSeven() & 0b000000111110000()) >= 5 ? ConstCardEight() :
        bit_count()(RankBitsWithAdditionalSeven() & 0b000000011111000()) >= 5 ? ConstCardSeven() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_BestPossibleStraightForUsIfSixHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(RankBitsWithAdditionalSix() & 0b000011111000000()) >= 5 ? ConstCardTen() :
        bit_count()(RankBitsWithAdditionalSix() & 0b000001111100000()) >= 5 ? ConstCardNine() :
        bit_count()(RankBitsWithAdditionalSix() & 0b000000111110000()) >= 5 ? ConstCardEight() :
        bit_count()(RankBitsWithAdditionalSix() & 0b000000011111000()) >= 5 ? ConstCardSeven() :
        bit_count()(RankBitsWithAdditionalSix() & 0b000000001111100()) >= 5 ? ConstCardSix() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_BestPossibleStraightForUsIfFiveHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(RankBitsWithAdditionalFive() & 0b000001111100000()) >= 5 ? ConstCardNine() :
        bit_count()(RankBitsWithAdditionalFive() & 0b000000111110000()) >= 5 ? ConstCardEight() :
        bit_count()(RankBitsWithAdditionalFive() & 0b000000011111000()) >= 5 ? ConstCardSeven() :
        bit_count()(RankBitsWithAdditionalFive() & 0b000000001111100()) >= 5 ? ConstCardSix() :
        bit_count()(RankBitsWithAdditionalFive() & 0b000000000111110()) >= 5 ? ConstCardFive() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_BestPossibleStraightForUsIfFourHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(RankBitsWithAdditionalFour() & 0b000000111110000()) >= 5 ? ConstCardEight() :
        bit_count()(RankBitsWithAdditionalFour() & 0b000000011111000()) >= 5 ? ConstCardSeven() :
        bit_count()(RankBitsWithAdditionalFour() & 0b000000001111100()) >= 5 ? ConstCardSix() :
        bit_count()(RankBitsWithAdditionalFour() & 0b000000000111110()) >= 5 ? ConstCardFive() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_BestPossibleStraightForUsIfThreeHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(RankBitsWithAdditionalThree() & 0b000000011111000()) >= 5 ? ConstCardSeven() :
        bit_count()(RankBitsWithAdditionalThree() & 0b000000001111100()) >= 5 ? ConstCardSix() :
        bit_count()(RankBitsWithAdditionalThree() & 0b000000000111110()) >= 5 ? ConstCardFive() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_BestPossibleStraightForUsIfTwoHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(RankBitsWithAdditionalTwo() & 0b000000001111100()) >= 5 ? ConstCardSix() :
        bit_count()(RankBitsWithAdditionalTwo() & 0b000000000111110()) >= 5 ? ConstCardFive() :
        HINT_Such_A_Straight_Does_Not_Exist());
}

double CalculateOpenPPL_GetBestPossibleStraightIfAceHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((BestPossibleStraightForUsIfAceHitsTheBoard() != -1)
        && (BestPossibleStraightForUsIfAceHitsTheBoard() >= BestPossibleStraightForVillainIfAceHitsTheBoard()));
}

double CalculateOpenPPL_GetBestPossibleStraightIfKingHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((BestPossibleStraightForUsIfKingHitsTheBoard() != -1)
        && (BestPossibleStraightForUsIfKingHitsTheBoard() >= BestPossibleStraightForVillainIfKingHitsTheBoard()));
}

double CalculateOpenPPL_GetBestPossibleStraightIfQueenHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((BestPossibleStraightForUsIfQueenHitsTheBoard() != -1)
        && (BestPossibleStraightForUsIfQueenHitsTheBoard() >= BestPossibleStraightForVillainIfQueenHitsTheBoard()));
}

double CalculateOpenPPL_GetBestPossibleStraightIfJackHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((BestPossibleStraightForUsIfJackHitsTheBoard() != -1)
        && (BestPossibleStraightForUsIfJackHitsTheBoard() >= BestPossibleStraightForVillainIfJackHitsTheBoard()));
}

double CalculateOpenPPL_GetBestPossibleStraightIfTenHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((BestPossibleStraightForUsIfTenHitsTheBoard() != -1)
        && (BestPossibleStraightForUsIfTenHitsTheBoard() >= BestPossibleStraightForVillainIfTenHitsTheBoard()));
}

double CalculateOpenPPL_GetBestPossibleStraightIfNineHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((BestPossibleStraightForUsIfNineHitsTheBoard() != -1)
        && (BestPossibleStraightForUsIfNineHitsTheBoard() >= BestPossibleStraightForVillainIfNineHitsTheBoard()));
}

double CalculateOpenPPL_GetBestPossibleStraightIfEightHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((BestPossibleStraightForUsIfEightHitsTheBoard() != -1)
        && (BestPossibleStraightForUsIfEightHitsTheBoard() >= BestPossibleStraightForVillainIfEightHitsTheBoard()));
}

double CalculateOpenPPL_GetBestPossibleStraightIfSevenHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((BestPossibleStraightForUsIfSevenHitsTheBoard() != -1)
        && (BestPossibleStraightForUsIfSevenHitsTheBoard() >= BestPossibleStraightForVillainIfSevenHitsTheBoard()));
}

double CalculateOpenPPL_GetBestPossibleStraightIfSixHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((BestPossibleStraightForUsIfSixHitsTheBoard() != -1)
        && (BestPossibleStraightForUsIfSixHitsTheBoard() >= BestPossibleStraightForVillainIfSixHitsTheBoard()));
}

double CalculateOpenPPL_GetBestPossibleStraightIfFiveHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((BestPossibleStraightForUsIfFiveHitsTheBoard() != -1)
        && (BestPossibleStraightForUsIfFiveHitsTheBoard() >= BestPossibleStraightForVillainIfFiveHitsTheBoard()));
}

double CalculateOpenPPL_GetBestPossibleStraightIfFourHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((BestPossibleStraightForUsIfFourHitsTheBoard() != -1)
        && (BestPossibleStraightForUsIfFourHitsTheBoard() >= BestPossibleStraightForVillainIfFourHitsTheBoard()));
}

double CalculateOpenPPL_GetBestPossibleStraightIfThreeHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((BestPossibleStraightForUsIfThreeHitsTheBoard() != -1)
        && (BestPossibleStraightForUsIfThreeHitsTheBoard() >= BestPossibleStraightForVillainIfThreeHitsTheBoard()));
}

double CalculateOpenPPL_GetBestPossibleStraightIfTwoHitsTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((BestPossibleStraightForUsIfTwoHitsTheBoard() != -1)
        && (BestPossibleStraightForUsIfTwoHitsTheBoard() >= BestPossibleStraightForVillainIfTwoHitsTheBoard()));
}

double CalculateOpenPPL_NumberOfCardranksThatGiveUsTheBestPossibleStraight()    // Please revisit the return type. int or bool might be more appropriate
{
    return (GetBestPossibleStraightIfAceHitsTheBoard()
        + GetBestPossibleStraightIfKingHitsTheBoard()
        + GetBestPossibleStraightIfQueenHitsTheBoard()
        + GetBestPossibleStraightIfJackHitsTheBoard()
        + GetBestPossibleStraightIfTenHitsTheBoard()
        + GetBestPossibleStraightIfNineHitsTheBoard()
        + GetBestPossibleStraightIfEightHitsTheBoard()
        + GetBestPossibleStraightIfSevenHitsTheBoard()
        + GetBestPossibleStraightIfSixHitsTheBoard()
        + GetBestPossibleStraightIfFiveHitsTheBoard()
        + GetBestPossibleStraightIfFourHitsTheBoard()
        + GetBestPossibleStraightIfThreeHitsTheBoard()
        + GetBestPossibleStraightIfTwoHitsTheBoard());
}

double CalculateOpenPPL_HaveNutStraightDraw()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((nsuitedcommon() < 2) && (NumberOfCardranksThatGiveUsTheBestPossibleStraight() >= 2));
}

double CalculateOpenPPL_HaveInsideNutStraightDraw()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((NumberOfCardranksThatGiveUsTheBestPossibleStraight() >= 1));
}

double CalculateOpenPPL_HaveInsideStraightDraw()    // Please revisit the return type. int or bool might be more appropriate
{
    return (HaveAnyKindOfInsideStraightDraw());
}

double CalculateOpenPPL_HaveNuts()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((nhandshi() == 0));
}

double CalculateOpenPPL_HaveNutStraightFlush()    // Please revisit the return type. int or bool might be more appropriate
{
    return (isstraightflush() && (nhandshi() == 0));
}

double CalculateOpenPPL_HaveOpenEndedStraightDraw()    // Please revisit the return type. int or bool might be more appropriate
{
    return (HaveStraight()
        || ((rankbits >> 0) & 0b111100) == 0b111100 // 5432
        || ((rankbits >> 1) & 0b111100) == 0b111100 // 6543
        || ((rankbits >> 2) & 0b111100) == 0b111100 // 7654
        || ((rankbits >> 3) & 0b111100) == 0b111100 // 8765
        || ((rankbits >> 4) & 0b111100) == 0b111100 // 9876
        || ((rankbits >> 5) & 0b111100) == 0b111100 // T987
        || ((rankbits >> 6) & 0b111100) == 0b111100 // JT98
        || ((rankbits >> 7) & 0b111100) == 0b111100 // QJT9
        || ((rankbits >> 8) & 0b111100) == 0b111100 // KQJT);
}

double CalculateOpenPPL_HaveQuads()    // Please revisit the return type. int or bool might be more appropriate
{
    return (( isfourofakind() && (nrankedcommon() < 4) ));
}

double CalculateOpenPPL_HaveOverPair()    // Please revisit the return type. int or bool might be more appropriate
{
    return (ispair() && (rankhiplayer() > rankhicommon()));
}

double CalculateOpenPPL_HaveOverPairOrBetter()    // Please revisit the return type. int or bool might be more appropriate
{
    return (HaveOverPair() || HaveBestOverPairOrBetter());
}

double CalculateOpenPPL_HavePair()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((  RankOfFirstHoleCard()==RankOfSecondHoleCard()
        || RankOfFirstHoleCard()==FirstFlopCard() 
        || RankOfFirstHoleCard()==SecondFlopCard() 
        || RankOfFirstHoleCard()==ThirdFlopCard() 
        || RankOfFirstHoleCard()==TurnCard() 
        || RankOfFirstHoleCard()==RiverCard()
        || RankOfSecondHoleCard()==FirstFlopCard() 
        || RankOfSecondHoleCard()==SecondFlopCard() 
        || RankOfSecondHoleCard()==ThirdFlopCard() 
        || RankOfSecondHoleCard()==TurnCard() 
        || RankOfSecondHoleCard()==RiverCard() ));
}

double CalculateOpenPPL_HavePocketPair()    // Please revisit the return type. int or bool might be more appropriate
{
    return (ispair());
}

double CalculateOpenPPL_HaveRunnerRunnerFlushAtRiver()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((betround() == ConstBetRoundRiver() && HaveFlush() && nsuitedcommon()==3 && SuitsOnFlop()!=3 && $$cs3()==$$cs4()) ? 1:0);
}

double CalculateOpenPPL_HaveSet()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((  ispair()
        && (  (RankOfFirstHoleCard() == FirstFlopCard()) 
        || (RankOfFirstHoleCard() == SecondFlopCard()) 
        || (RankOfFirstHoleCard() == ThirdFlopCard()) 
        || (RankOfFirstHoleCard() == TurnCard()) 
        || (RankOfFirstHoleCard() == RiverCard())) ));
}

double CalculateOpenPPL_HaveTopTrips()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((!ispair() && ($$pr0() == RankOfTopPairOnBoard() || $$pr1() == RankOfTopPairOnBoard())));
}

double CalculateOpenPPL_HaveTrips()    // Please revisit the return type. int or bool might be more appropriate
{
    return (!ispair()
        &&   ((pokerval() & threeofakind()) && !(pokervalcommon() & threeofakind())
        || ((pokerval() & fullhouse())    && bit_count()(pcbits() & 0b11100()) == 1)
        ||  (pokerval() & fourofakind())  && !(pokervalcommon() & fourofakind())));
}

double CalculateOpenPPL_TripsRank()    // Please revisit the return type. int or bool might be more appropriate
{
    return (HaveTrips() && HaveTopPair()    ? TopPairRank():
        HaveTrips() && Have2ndTopPair() ? SecondTopPairRank():
        HaveTrips() && Have3rdTopPair() ? ThirdTopPairRank():
        HaveTrips() && Have4thTopPair() ? ForthTopPairRank():
        HaveTrips() && Have5thTopPair() ? FifthTopPairRank(): 0);
}

double CalculateOpenPPL_TripsKickerRank()    // Please revisit the return type. int or bool might be more appropriate
{
    return (HaveTrips() && TripsRank()==RankOfFirstHoleCard() ? RankOfSecondHoleCard():
        HaveTrips() && TripsRank()==RankOfSecondHoleCard() ? RankOfFirstHoleCard(): 0);
}

double CalculateOpenPPL_HaveStraight()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((nstraightfill() == 0));
}

double CalculateOpenPPL_HaveStraightDraw()    // Please revisit the return type. int or bool might be more appropriate
{
    return (HaveOpenEndedStraightDraw() 
        || HaveDoubleGutshotDraw());
}

double CalculateOpenPPL_HaveStraightFlush()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((isstraightflush() || isroyalflush()));
}

double CalculateOpenPPL_CommonCard0Paired()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((FirstFlopCard() == SecondFlopCard() || FirstFlopCard() == ThirdFlopCard() || FirstFlopCard() == TurnCard() || FirstFlopCard() == RiverCard()));
}

double CalculateOpenPPL_CommonCard1Paired()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((SecondFlopCard() == FirstFlopCard() || SecondFlopCard() == ThirdFlopCard() || SecondFlopCard() == TurnCard() || SecondFlopCard() == RiverCard()));
}

double CalculateOpenPPL_CommonCard2Paired()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((ThirdFlopCard() == FirstFlopCard() || ThirdFlopCard() == SecondFlopCard() || ThirdFlopCard() == TurnCard() || ThirdFlopCard() == RiverCard()));
}

double CalculateOpenPPL_CommonCard3Paired()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((TurnCard() == FirstFlopCard() || TurnCard() == SecondFlopCard() || TurnCard() == ThirdFlopCard() || TurnCard() == RiverCard()));
}

double CalculateOpenPPL_CommonCard4Paired()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((RiverCard() == FirstFlopCard() || RiverCard() == SecondFlopCard() || RiverCard() == ThirdFlopCard() || FirstFlopCard() == TurnCard()));
}

double CalculateOpenPPL_RankBitsCommonWithoutCommonRank0Pair()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((CommonCard0Paired() && (FirstFlopCard() != ConstCardAce())) ? rankbitscommon() & (~2**FirstFlopCard()) :
        (CommonCard0Paired() && (FirstFlopCard() == ConstCardAce())) ? rankbitscommon() & (~0b100000000000010()) :
        rankbitscommon());
}

double CalculateOpenPPL_RankBitsCommonWithoutCommonRank1Pair()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((CommonCard0Paired() && (SecondFlopCard() != ConstCardAce())) ? rankbitscommon() & (~2**FirstFlopCard()) :
        (CommonCard0Paired() && (SecondFlopCard() == ConstCardAce())) ? rankbitscommon() & (~0b100000000000010()) :
        rankbitscommon());
}

double CalculateOpenPPL_RankBitsCommonWithoutCommonRank2Pair()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((CommonCard0Paired() && (ThirdFlopCard() != ConstCardAce())) ? rankbitscommon() & (~2**FirstFlopCard()) :
        (CommonCard0Paired() && (ThirdFlopCard() == ConstCardAce())) ? rankbitscommon() & (~0b100000000000010()) :
        rankbitscommon());
}

double CalculateOpenPPL_RankBitsCommonWithoutCommonRank3Pair()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((CommonCard0Paired() && (TurnCard() != ConstCardAce())) ? rankbitscommon() & (~2**FirstFlopCard()) :
        (CommonCard0Paired() && (TurnCard() == ConstCardAce())) ? rankbitscommon() & (~0b100000000000010()) :
        rankbitscommon());
}

double CalculateOpenPPL_RankBitsCommonWithoutCommonRank4Pair()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((CommonCard0Paired() && (RiverCard() != ConstCardAce())) ? rankbitscommon() & (~2**FirstFlopCard()) :
        (CommonCard0Paired() && (RiverCard() == ConstCardAce())) ? rankbitscommon() & (~0b100000000000010()) :
        rankbitscommon());
}

double CalculateOpenPPL_RankBitsCommonWithoutPairs()    // Please revisit the return type. int or bool might be more appropriate
{
    return (RankBitsCommonWithoutCommonRank0Pair()
        & RankBitsCommonWithoutCommonRank1Pair()
        & RankBitsCommonWithoutCommonRank2Pair()
        & RankBitsCommonWithoutCommonRank3Pair()
        & RankBitsCommonWithoutCommonRank4Pair());
}

double CalculateOpenPPL_HighestUnpairedBoardCard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((RankBitsCommonWithoutPairs() >> ConstCardAce())   & 1 ? ConstCardAce()   :
        (RankBitsCommonWithoutPairs() >> ConstCardKing())  & 1 ? ConstCardKing()  :
        (RankBitsCommonWithoutPairs() >> ConstCardQueen()) & 1 ? ConstCardQueen() :
        (RankBitsCommonWithoutPairs() >> ConstCardJack())  & 1 ? ConstCardJack()  :
        (RankBitsCommonWithoutPairs() >> ConstCardTen())   & 1 ? ConstCardTen()   :
        (RankBitsCommonWithoutPairs() >> ConstCardNine())  & 1 ? ConstCardNine()  :
        (RankBitsCommonWithoutPairs() >> ConstCardEight()) & 1 ? ConstCardEight() :
        (RankBitsCommonWithoutPairs() >> ConstCardSeven()) & 1 ? ConstCardSeven() :
        (RankBitsCommonWithoutPairs() >> ConstCardSix())   & 1 ? ConstCardSix()   :
        (RankBitsCommonWithoutPairs() >> ConstCardFive())  & 1 ? ConstCardFive()  :
        (RankBitsCommonWithoutPairs() >> ConstCardFour())  & 1 ? ConstCardFour()  :
        (RankBitsCommonWithoutPairs() >> ConstCardThree()) & 1 ? ConstCardThree() :
        (RankBitsCommonWithoutPairs() >> ConstCardTwo())   & 1 ? ConstCardTwo()   : 0);
}

double CalculateOpenPPL_HaveTopNonBoardPairedPair()    // Please revisit the return type. int or bool might be more appropriate
{
    return (((betround() >= ConstBetRoundFlop()) && (RankOfFirstHoleCard() == HighestUnpairedBoardCard()))
        || ((betround() >= ConstBetRoundFlop()) && (RankOfSecondHoleCard() == HighestUnpairedBoardCard())));
}

double CalculateOpenPPL_HaveTopPair()    // Please revisit the return type. int or bool might be more appropriate
{
    return (( RankOfFirstHoleCard()==rankhicommon() || RankOfSecondHoleCard()==rankhicommon() ));
}

double CalculateOpenPPL_TopPairRank()    // Please revisit the return type. int or bool might be more appropriate
{
    return (HaveTopPair() && $$pr0()==rankhicommon() ? $$pr0():
        HaveTopPair() && $$pr1()==rankhicommon() ? $$pr1():
        0);
}

double CalculateOpenPPL_TopPairKickerRank()    // Please revisit the return type. int or bool might be more appropriate
{
    return (HaveTopPair() && $$pr0()==rankhicommon() ? $$pr1():
        HaveTopPair() && $$pr1()==rankhicommon() ? $$pr0():
        0);
}

double CalculateOpenPPL_SecondTopPairRank()    // Please revisit the return type. int or bool might be more appropriate
{
    return (Have2ndTopPair() && RankOfFirstHoleCard()  == 2ndBestBoardCard() ? RankOfFirstHoleCard() :
        Have2ndTopPair() && RankOfSecondHoleCard() == 2ndBestBoardCard() ? RankOfSecondHoleCard() :
        0);
}

double CalculateOpenPPL_HaveTopSet()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((ispair() && (rankhiplayer() == rankhicommon())));
}

double CalculateOpenPPL_HaveTopTwoPair()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((HaveTopPair() && Have2ndTopPair()));
}

double CalculateOpenPPL_RankbitsCommonWithoutAceLow()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Don't count ace twice in HaveTwoPair
        // bit0 is unused
        // bit 2 is ace low
        //                 AKQJT98765432-- 
        rankbitscommon() & 0b111111111111100());
}

double CalculateOpenPPL_HaveTwoPair()    // Please revisit the return type. int or bool might be more appropriate
{
    return (bit_count()(rankbitsplayer() & RankbitsCommonWithoutAceLow()) == 2);
}

double CalculateOpenPPL_HaveUnderPair()    // Please revisit the return type. int or bool might be more appropriate
{
    return (( ispair() && rankhiplayer()<ranklocommon() ));
}

double CalculateOpenPPL_HaveUnderStraight()    // Please revisit the return type. int or bool might be more appropriate
{
    return (HaveStraight()
        && (pcbits() == 0b00011() || pcbits() == 0b00001()));
}

double CalculateOpenPPL_NumberOfUnknownSuitedOvercards()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Suited unknown overcards to holecards,
        // e.g. on Board Ah Th, in Hand Jh 9h => 2 suited overcards: Kh Qh.
        bit_count()((~srankbits() & 0x7fff()) >> srankhiplayer()));
}

double CalculateOpenPPL_BoardKicker_FourOfAKindOnBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((betround() == ConstBetRoundRiver()) ? HighestUnpairedBoardCard() : 2);
}

double CalculateOpenPPL_NFH4OAK_FullHouseOnBoard_WeHaveOverPairToPair()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// 3 is the best strength possible, because two quads are always possible
        3
        // 1 OverPair for each overcard = 1 higher full house
        + (ConstCardAce() - rankhiplayer())
        // Higher set possible with pair onboard?
        + ((RankOfTopPairOnBoard() > rankhiplayer()) ? 1 : 0 )
        // If the cards on board are higher than our pair,
        // we have to subtract them again, as otherwise we count them twice,
        // as set/quads and OverPair.
        + ((RankOfTopPairOnBoard() > rankhiplayer()) ? -1 : 0 )
        + ((RankOfSetOnBoard()  > rankhiplayer()) ? -1 : 0 ));
}

double CalculateOpenPPL_NFH4OAK_FullHouseOnBoard_WePlayTheBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// 3 is the best strength possible, because two quads are always possible
        3
        // 1 OverPair for each overcard = 1 higher full house
        + (ConstCardAce() - rankhiplayer())
        // +1 if the pair is higher as the set (higher set possible)
        // -1 if the pair is higher as the set (counted twice as OverPair / set)
        // but these cases neutralize each other);
}

double CalculateOpenPPL_RankOfPairOfFullHouse()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// The last 20 bits of pokerval look like XXXX XXXX XXXX YYYY YYYY (see docu)
        // XXXX are the rank of the set
        // YYYY are the rank of the pair
        (pokerval() & 0b1111()));
}

double CalculateOpenPPL_RankOfSetOfFullHouse()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// The last 20 bits of pokerval look like XXXX XXXX XXXX YYYY YYYY (see docu)
        // XXXX are the rank of the set
        // YYYY are the rank of the pair
        ((pokerval() >> 12) & 0b1111()));
}

double CalculateOpenPPL_NFH4OAK_PairOnBoard_WeHitThePairAndAnotherCard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// 1 for each overcard on baord to RankOfTopPairOnBoard, which make better 3oaks possible = better FH
        (((rankbitscommon() & 0x100000000000000()) && (RankOfSetOfFullHouse() < ConstCardAce()  )) ? 1 : 0)
        + (((rankbitscommon() & 0x010000000000000()) && (RankOfSetOfFullHouse() < ConstCardKing() )) ? 1 : 0)
        + (((rankbitscommon() & 0x001000000000000()) && (RankOfSetOfFullHouse() < ConstCardQueen())) ? 1 : 0)
        + (((rankbitscommon() & 0x000100000000000()) && (RankOfSetOfFullHouse() < ConstCardJack() )) ? 1 : 0)
        + (((rankbitscommon() & 0x000010000000000()) && (RankOfSetOfFullHouse() < ConstCardTen()  )) ? 1 : 0)
        + (((rankbitscommon() & 0x000001000000000()) && (RankOfSetOfFullHouse() < ConstCardNine() )) ? 1 : 0)
        + (((rankbitscommon() & 0x000000100000000()) && (RankOfSetOfFullHouse() < ConstCardEight())) ? 1 : 0)
        + (((rankbitscommon() & 0x000000010000000()) && (RankOfSetOfFullHouse() < ConstCardSeven())) ? 1 : 0)
        + (((rankbitscommon() & 0x000000001000000()) && (RankOfSetOfFullHouse() < ConstCardSix()  )) ? 1 : 0)
        + (((rankbitscommon() & 0x000000000100000()) && (RankOfSetOfFullHouse() < ConstCardFive() )) ? 1 : 0)
        + (((rankbitscommon() & 0x000000000010000()) && (RankOfSetOfFullHouse() < ConstCardFour() )) ? 1 : 0)
        + (((rankbitscommon() & 0x000000000001000()) && (RankOfSetOfFullHouse() < ConstCardThree())) ? 1 : 0)
        + (((rankbitscommon() & 0x000000000000100()) && (RankOfSetOfFullHouse() < ConstCardTwo()  )) ? 1 : 0)
        // 1 for each board-card that is not the pair on board and higher than the other pair (board-user),
        // as there ar higher FHs possible (same set, higher pair)
        + (((rankbitscommon() & 0x100000000000000()) && (RankOfPairOfFullHouse() < ConstCardAce()  ) && (RankOfSetOfFullHouse() != ConstCardAce()  )) ? 1 : 0)
        + (((rankbitscommon() & 0x010000000000000()) && (RankOfPairOfFullHouse() < ConstCardKing() ) && (RankOfSetOfFullHouse() != ConstCardKing() )) ? 1 : 0)
        + (((rankbitscommon() & 0x001000000000000()) && (RankOfPairOfFullHouse() < ConstCardQueen()) && (RankOfSetOfFullHouse() != ConstCardQueen())) ? 1 : 0)
        + (((rankbitscommon() & 0x000100000000000()) && (RankOfPairOfFullHouse() < ConstCardJack() ) && (RankOfSetOfFullHouse() != ConstCardJack() )) ? 1 : 0)
        + (((rankbitscommon() & 0x000010000000000()) && (RankOfPairOfFullHouse() < ConstCardTen()  ) && (RankOfSetOfFullHouse() != ConstCardTen()  )) ? 1 : 0)
        + (((rankbitscommon() & 0x000001000000000()) && (RankOfPairOfFullHouse() < ConstCardNine() ) && (RankOfSetOfFullHouse() != ConstCardNine() )) ? 1 : 0)
        + (((rankbitscommon() & 0x000000100000000()) && (RankOfPairOfFullHouse() < ConstCardEight()) && (RankOfSetOfFullHouse() != ConstCardEight())) ? 1 : 0)
        + (((rankbitscommon() & 0x000000010000000()) && (RankOfPairOfFullHouse() < ConstCardSeven()) && (RankOfSetOfFullHouse() != ConstCardSeven())) ? 1 : 0)
        + (((rankbitscommon() & 0x000000001000000()) && (RankOfPairOfFullHouse() < ConstCardSix()  ) && (RankOfSetOfFullHouse() != ConstCardSix()  )) ? 1 : 0)
        + (((rankbitscommon() & 0x000000000100000()) && (RankOfPairOfFullHouse() < ConstCardFive() ) && (RankOfSetOfFullHouse() != ConstCardFive() )) ? 1 : 0)
        + (((rankbitscommon() & 0x000000000010000()) && (RankOfPairOfFullHouse() < ConstCardFour() ) && (RankOfSetOfFullHouse() != ConstCardFour() )) ? 1 : 0)
        + (((rankbitscommon() & 0x000000000001000()) && (RankOfPairOfFullHouse() < ConstCardThree()) && (RankOfSetOfFullHouse() != ConstCardThree())) ? 1 : 0)
        + (((rankbitscommon() & 0x000000000000100()) && (RankOfPairOfFullHouse() < ConstCardTwo()  ) && (RankOfSetOfFullHouse() != ConstCardTwo()  )) ? 1 : 0)
        // + 1 for our hand
        + 1);
}

double CalculateOpenPPL_OvercardsOnBoardToRankOfSetOfFullHouse()    // Please revisit the return type. int or bool might be more appropriate
{
    return ((rankbitscommon() & 0x100000000000000()) && (RankOfSetOfFullHouse() < ConstCardAce()  )
        + (rankbitscommon() & 0x010000000000000()) && (RankOfSetOfFullHouse() < ConstCardKing() )
        + (rankbitscommon() & 0x001000000000000()) && (RankOfSetOfFullHouse() < ConstCardQueen())
        + (rankbitscommon() & 0x000100000000000()) && (RankOfSetOfFullHouse() < ConstCardJack() )
        + (rankbitscommon() & 0x000010000000000()) && (RankOfSetOfFullHouse() < ConstCardTen()  )
        + (rankbitscommon() & 0x000001000000000()) && (RankOfSetOfFullHouse() < ConstCardNine() )
        + (rankbitscommon() & 0x000000100000000()) && (RankOfSetOfFullHouse() < ConstCardEight())
        + (rankbitscommon() & 0x000000010000000()) && (RankOfSetOfFullHouse() < ConstCardSeven())
        + (rankbitscommon() & 0x000000001000000()) && (RankOfSetOfFullHouse() < ConstCardSix()  )
        + (rankbitscommon() & 0x000000000100000()) && (RankOfSetOfFullHouse() < ConstCardFive() )
        + (rankbitscommon() & 0x000000000010000()) && (RankOfSetOfFullHouse() < ConstCardFour() )
        + (rankbitscommon() & 0x000000000001000()) && (RankOfSetOfFullHouse() < ConstCardThree())
        + (rankbitscommon() & 0x000000000000100()) && (RankOfSetOfFullHouse() < ConstCardTwo()  ));
}

double CalculateOpenPPL_NFH4OAK_TwoPairsOnBoard_WeHitBottomSet()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// 3 is the lowest nut rank possible, becuase theres still a quad and a FH with the higher pair possible.
        // Theres always a higher pair on board, so if there are two higher ranked cards in common cards,
        // theres also a higher set possible with a pocket pair and the remaining single card on board.
        3 + ((OvercardsOnBoardToRankOfSetOfFullHouse() >= 2) ? 1 : 0));
}

double CalculateOpenPPL_NFH4OAK_TwoPairsOnBoard_WeHitTopSet()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// 2 is the lowest nut rank possible, becuase theres still a quad possible.
        // Theres might be a higher card on board, so if there is a higher ranked card in common cards then our set rank,
        // theres also a higher set possible with a pocket pair and the remaining single card on board.
        2 + ((OvercardsOnBoardToRankOfSetOfFullHouse() >= 1) ? 1 : 0));
}

double CalculateOpenPPL_NFH4OAK_TwoPairsOnBoard_WeHitBottomQuads()    // Please revisit the return type. int or bool might be more appropriate
{
    return (2);
}

double CalculateOpenPPL_NFH4OAK_TwoPairsOnBoard_WeHitTopQuads()    // Please revisit the return type. int or bool might be more appropriate
{
    return (1);
}

double CalculateOpenPPL_NFH4OAK_TwoPairsOnBoard_WePairBothPairs()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// Theres might be a higher card on board,
        // so if there is a higher ranked card in common cards then our pair ranks,
        // there are also a higher pair and a higher set possible.
        1
        + (((pokervalcommon() & 0b1111()) > RankOfPairOfFullHouse()) ? 1 : 0)
        + (((pokervalcommon() & 0b1111()) > RankOfSetOfFullHouse() ) ? 1 : 0));
}

double CalculateOpenPPL_NFH4OAK_FullHouseOnBoard_WePairBothBoardCards()    // Please revisit the return type. int or bool might be more appropriate
{
    return (1);
}

double CalculateOpenPPL_NFH4OAK_FullHouseOnBoard_WePairHighSetOnBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (1);
}

double CalculateOpenPPL_NFH4OAK_FullHouseOnBoard_WePairLowSetOnBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (2);
}

double CalculateOpenPPL_NFH4OAK_FullHouseOnBoard_WePairHighPairOnBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (2);
}

double CalculateOpenPPL_RankOfTopPairOnBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (board$AA() ? ConstCardAce() :
        board$KK() ? ConstCardKing() :
        board$QQ() ? ConstCardQueen() :
        board$JJ() ? ConstCardJack() :
        board$TT() ? ConstCardTen() :
        board$99() ? ConstCardNine() :
        board$88() ? ConstCardEight() :
        board$77() ? ConstCardSeven() :
        board$66() ? ConstCardSix() :
        board$55() ? ConstCardFive() :
        board$44() ? ConstCardFour() :
        board$33() ? ConstCardThree() :
        board$22() ? ConstCardTwo() :
        HINT_There_Is_No_Pair_On_The_Board());
}

double CalculateOpenPPL_RankOfBottomPairOnBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (board$22() ? ConstCardTwo() :
        board$33() ? ConstCardThree() :
        board$44() ? ConstCardFour() :
        board$55() ? ConstCardFive() :
        board$66() ? ConstCardSix() :
        board$77() ? ConstCardSeven() :
        board$88() ? ConstCardEight() :
        board$99() ? ConstCardNine() :
        board$TT() ? ConstCardTen() :
        board$JJ() ? ConstCardJack() :
        board$QQ() ? ConstCardQueen() :
        board$KK() ? ConstCardKing() :
        board$AA() ? ConstCardAce() :
        HINT_There_Is_No_Pair_On_The_Board());
}

double CalculateOpenPPL_NFH4OAK_FullHouseOnBoard_WePairLowPairOnBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// One quads possible, so the bestcase is second nuts
        2
        // Bbecause the set does have a higher rank then our pair,
        // with the rank of the set theres no OverPair possible,
        // and therefore are only 11 OverPairs possible instead of 12 if we do have 22 pair,
        // 10 if we have 33 pair, etc.
        + (13 - RankOfBottomPairOnBoard()));
}

double CalculateOpenPPL_NFH4OAK_PairOnBoard_WeHitASetWithPocketPair()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// 2 is the lowest rank possible, because quads are still possible
        2
        // For each rank in the common cards above the rank in the pocket cards, theres a higher FH possible.
        +  (rankbitscommon() & 0b000000000001000() > rankbitsplayer() ? 1 : 0)
        + (rankbitscommon() & 0b000000000010000() > rankbitsplayer() ? 1 : 0)
        + (rankbitscommon() & 0b000000000100000() > rankbitsplayer() ? 1 : 0)
        + (rankbitscommon() & 0b000000001000000() > rankbitsplayer() ? 1 : 0)
        + (rankbitscommon() & 0b000000010000000() > rankbitsplayer() ? 1 : 0)
        + (rankbitscommon() & 0b000000100000000() > rankbitsplayer() ? 1 : 0)
        + (rankbitscommon() & 0b000001000000000() > rankbitsplayer() ? 1 : 0)
        + (rankbitscommon() & 0b000010000000000() > rankbitsplayer() ? 1 : 0)
        + (rankbitscommon() & 0b000100000000000() > rankbitsplayer() ? 1 : 0)
        + (rankbitscommon() & 0b001000000000000() > rankbitsplayer() ? 1 : 0)
        + (rankbitscommon() & 0b010000000000000() > rankbitsplayer() ? 1 : 0)
        + (rankbitscommon() & 0b100000000000000() > rankbitsplayer() ? 1 : 0)
        // If the Pair on board is higher than our pocket-pair it got already counted
        - ((trankcommon() > rankhiplayer()) ? 1 : 0));
}

double CalculateOpenPPL_NFH4OAK_PairOnBoard_WeHitQuads()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// With a single pair on board and quads for us we have the current nuts
        1);
}

double CalculateOpenPPL_NutFullHouseOrFourOfAKind_FourOfAKindOnBoard()    // Please revisit the return type. int or bool might be more appropriate
{
    return (// PreCondition: FourOfAKindOnBoard
        //
        // case 1: we play the board. e.g. 8888Q, we have J5
        (rankhiplayer() < BoardKicker_FourOfAKindOnBoard()) ?);
}


// case 1A: common kicker is higher then quads, e.g. 8888Q.ERROR!