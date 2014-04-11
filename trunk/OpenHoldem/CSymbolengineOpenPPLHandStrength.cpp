//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose:
//
//***************************************************************************** 

#include "stdafx.h"
#include "CSymbolEngineOpenPPLHandStrength.h"

#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CStringMatch.h"
#include "MagicNumbers.h"

/*##f$OpenPPL_HadOverPairOnFlop##
##f$OpenPPL_HadOverPairOnTurn##
##f$OpenPPL_Had2ndOverPairOnFlop##
##f$OpenPPL_Had2ndOverPairOnTurn##
##f$OpenPPL_HadPairOnFlop##
##f$OpenPPL_HadPairOnTurn##
##f$OpenPPL_HadSetOnFlop##
##f$OpenPPL_HadSetOnTurn##
##f$OpenPPL_HadTopPairOnFlop##
##f$OpenPPL_Had2ndTopPairOnFlop##
##f$OpenPPL_HighestRankOnTurn##
##f$OpenPPL_HadTopPairOnTurn##
##f$OpenPPL_HadTwoPairOnFlop##
##f$OpenPPL_Have2ndBestKicker##
##f$OpenPPL_Have2ndBestKickerOrBetter##
##f$OpenPPL_Have2ndBestOverPair##
##f$OpenPPL_Have2ndBestOverPairOrBetter##
##f$OpenPPL_Have2ndNutFlush##
##f$OpenPPL_Have2ndNutFlushDraw##
##f$OpenPPL_HighCardOf2ndBestPossibleStraightIfBestStraightIsAceHigh##
##f$OpenPPL_HighCardOf2ndBestPossibleStraightIfBestStraightIsKingHigh##
##f$OpenPPL_HighCardOf2ndBestPossibleStraightIfBestStraightIsQueenHigh##
##f$OpenPPL_HighCardOf2ndBestPossibleStraightIfBestStraightIsJackHigh##
##f$OpenPPL_HighCardOf2ndBestPossibleStraightIfBestStraightIsTenHigh##
##f$OpenPPL_HighCardOf2ndBestPossibleStraightIfBestStraightIsNineHigh##
##f$OpenPPL_HighCardOf2ndBestPossibleStraightIfBestStraightIsEightHigh##
##f$OpenPPL_HighCardOf2ndBestPossibleStraightIfBestStraightIsSevenHigh##
##f$OpenPPL_HighCardOf2ndBestPossibleStraightIfBestStraightIsSixHigh##
##f$OpenPPL_HighCardOfBestPossibleStraight##
##f$OpenPPL_HighCardOf2ndBestPossibleStraight##
##f$OpenPPL_HighCardOfOurStraight##
##f$OpenPPL_Have2ndNutStraight##
##f$OpenPPL_Have2ndOverPair##
##f$OpenPPL_Have2ndTopPair##
##f$OpenPPL_Have2ndTopSet##
##f$OpenPPL_TopBitOfRankBitsCommon##
##f$OpenPPL_2ndTopBitOfRankBitsCommon##
##f$OpenPPL_3rdTopBitOfRankBitsCommon##
##f$OpenPPL_4thTopBitOfRankBitsCommon##
##f$OpenPPL_5thTopBitOfRankBitsCommon##
##f$OpenPPL_RankBitsCommonWithoutTopBit##
##f$OpenPPL_RankBitsCommonWithoutTopTwoBits##
##f$OpenPPL_RankBitsCommonWithoutTopThreeBits##
##f$OpenPPL_RankBitsCommonWithoutTopFourBits##
##f$OpenPPL_Have3rdBestKicker##
##f$OpenPPL_Have3rdBestKickerOrBetter##
##f$OpenPPL_Have3rdBestOverPair##
##f$OpenPPL_3rdBestBoardCard##
##f$OpenPPL_Have3rdBestOverPairOrBetter##
##f$OpenPPL_Have3rdNutFlush##
##f$OpenPPL_Have3rdNutFlushDraw##
##f$OpenPPL_Have3rdOverPair##
##f$OpenPPL_Have3rdTopPair##
##f$OpenPPL_ThirdTopPairRank##
##f$OpenPPL_Have5thTopPair##
##f$OpenPPL_FifthTopPairRank##
##f$OpenPPL_Have3rdTopSet##
##f$OpenPPL_Have4thNutFlush##
##f$OpenPPL_Have4thNutFlushDraw##
##f$OpenPPL_4thBestBoardCard##
##f$OpenPPL_Have4thOverPair##
##f$OpenPPL_Have4thTopPair##
##f$OpenPPL_ForthTopPairRank##
##f$OpenPPL_Have4thTopSet##
##f$OpenPPL_Have5thNutFlush##
##f$OpenPPL_Have5thNutFlushDraw##
##f$OpenPPL_5thBestBoardCard##
##f$OpenPPL_Have5thOverPair##
##f$OpenPPL_HaveAnyKindOfInsideStraightDraw##
##f$OpenPPL_HaveBackdoorFlushDrawContributingTwoCards##
##f$OpenPPL_HaveBackdoorNutFlushDraw##
##f$OpenPPL_HaveBackDoor2ndNutFlushDraw##
##f$OpenPPL_HaveBackDoor3rdNutFlushDraw##
##f$OpenPPL_HaveBackdoorFlushDraw##
##f$OpenPPL_HaveBestKicker##
##f$OpenPPL_HaveBestKickerOrBetter##
##f$OpenPPL_HaveBestOverPair##
##f$OpenPPL_HaveBestOverPairOrBetter##
##f$OpenPPL_LowestBitOfRankBitsCommon##
##f$OpenPPL_RankBitsCommonWithoutLowestBit##
##f$OpenPPL_LowestBoardCard##
##f$OpenPPL_2ndLowestBoardCard##
##f$OpenPPL_HaveBottomPair##
##f$OpenPPL_HaveBottomSet##
##f$OpenPPL_HaveBottomTrips##
##f$OpenPPL_HaveBottomTwoPair##
##f$OpenPPL_HaveDoubleGutshotDraw##
##f$OpenPPL_HaveFlush##
##f$OpenPPL_HaveFlushDraw##
##f$OpenPPL_HaveFullHouse##
##f$OpenPPL_HaveNothing##
##f$OpenPPL_HaveNutFlush##
##f$OpenPPL_HaveNutFlushCard##
##f$OpenPPL_HaveNutFlushDraw##
##f$OpenPPL_HaveNutStraight##
##f$OpenPPL_MyKicker##
##f$OpenPPL_NumberOfBetterKickers##
##f$OpenPPL_RankBitsCommonWithAdditionalAce##
##f$OpenPPL_RankBitsCommonWithAdditionalKing##
##f$OpenPPL_RankBitsCommonWithAdditionalQueen##
##f$OpenPPL_RankBitsCommonWithAdditionalJack##
##f$OpenPPL_RankBitsCommonWithAdditionalTen##
##f$OpenPPL_RankBitsCommonWithAdditionalNine##
##f$OpenPPL_RankBitsCommonWithAdditionalEight##
##f$OpenPPL_RankBitsCommonWithAdditionalSeven##
##f$OpenPPL_RankBitsCommonWithAdditionalSix##
##f$OpenPPL_RankBitsCommonWithAdditionalFive##
##f$OpenPPL_RankBitsCommonWithAdditionalFour##
##f$OpenPPL_RankBitsCommonWithAdditionalThree##
##f$OpenPPL_RankBitsCommonWithAdditionalTwo##
##f$OpenPPL_BestPossibleStraightForVillainIfAceHitsTheBoard##
##f$OpenPPL_BestPossibleStraightForVillainIfKingHitsTheBoard##
##f$OpenPPL_BestPossibleStraightForVillainIfQueenHitsTheBoard##
##f$OpenPPL_BestPossibleStraightForVillainIfJackHitsTheBoard##
##f$OpenPPL_BestPossibleStraightForVillainIfTenHitsTheBoard##
##f$OpenPPL_BestPossibleStraightForVillainIfNineHitsTheBoard##
##f$OpenPPL_BestPossibleStraightForVillainIfEightHitsTheBoard##
##f$OpenPPL_BestPossibleStraightForVillainIfSevenHitsTheBoard##
##f$OpenPPL_BestPossibleStraightForVillainIfSixHitsTheBoard##
##f$OpenPPL_BestPossibleStraightForVillainIfFiveHitsTheBoard##
##f$OpenPPL_BestPossibleStraightForVillainIfFourHitsTheBoard##
##f$OpenPPL_BestPossibleStraightForVillainIfThreeHitsTheBoard##
##f$OpenPPL_BestPossibleStraightForVillainIfTwoHitsTheBoard##
##f$OpenPPL_RankBitsWithAdditionalAce##
##f$OpenPPL_RankBitsWithAdditionalKing##
##f$OpenPPL_RankBitsWithAdditionalQueen##
##f$OpenPPL_RankBitsWithAdditionalJack##
##f$OpenPPL_RankBitsWithAdditionalTen##
##f$OpenPPL_RankBitsWithAdditionalNine##
##f$OpenPPL_RankBitsWithAdditionalEight##
##f$OpenPPL_RankBitsWithAdditionalSeven##
##f$OpenPPL_RankBitsWithAdditionalSix##
##f$OpenPPL_RankBitsWithAdditionalFive##
##f$OpenPPL_RankBitsWithAdditionalFour##
##f$OpenPPL_RankBitsWithAdditionalThree##
##f$OpenPPL_RankBitsWithAdditionalTwo##
##f$OpenPPL_BestPossibleStraightForUsIfAceHitsTheBoard##
##f$OpenPPL_BestPossibleStraightForUsIfKingHitsTheBoard##
##f$OpenPPL_BestPossibleStraightForUsIfQueenHitsTheBoard##
##f$OpenPPL_BestPossibleStraightForUsIfJackHitsTheBoard##
##f$OpenPPL_BestPossibleStraightForUsIfTenHitsTheBoard##
##f$OpenPPL_BestPossibleStraightForUsIfNineHitsTheBoard##
##f$OpenPPL_BestPossibleStraightForUsIfEightHitsTheBoard##
##f$OpenPPL_BestPossibleStraightForUsIfSevenHitsTheBoard##
##f$OpenPPL_BestPossibleStraightForUsIfSixHitsTheBoard##
##f$OpenPPL_BestPossibleStraightForUsIfFiveHitsTheBoard##
##f$OpenPPL_BestPossibleStraightForUsIfFourHitsTheBoard##
##f$OpenPPL_BestPossibleStraightForUsIfThreeHitsTheBoard##
##f$OpenPPL_BestPossibleStraightForUsIfTwoHitsTheBoard##
##f$OpenPPL_GetBestPossibleStraightIfAceHitsTheBoard##
##f$OpenPPL_GetBestPossibleStraightIfKingHitsTheBoard##
##f$OpenPPL_GetBestPossibleStraightIfQueenHitsTheBoard##
##f$OpenPPL_GetBestPossibleStraightIfJackHitsTheBoard##
##f$OpenPPL_GetBestPossibleStraightIfTenHitsTheBoard##
##f$OpenPPL_GetBestPossibleStraightIfNineHitsTheBoard##
##f$OpenPPL_GetBestPossibleStraightIfEightHitsTheBoard##
##f$OpenPPL_GetBestPossibleStraightIfSevenHitsTheBoard##
##f$OpenPPL_GetBestPossibleStraightIfSixHitsTheBoard##
##f$OpenPPL_GetBestPossibleStraightIfFiveHitsTheBoard##
##f$OpenPPL_GetBestPossibleStraightIfFourHitsTheBoard##
##f$OpenPPL_GetBestPossibleStraightIfThreeHitsTheBoard##
##f$OpenPPL_GetBestPossibleStraightIfTwoHitsTheBoard##
##f$OpenPPL_NumberOfCardranksThatGiveUsTheBestPossibleStraight##
##f$OpenPPL_HaveNutStraightDraw##
##f$OpenPPL_HaveInsideNutStraightDraw##
##f$OpenPPL_HaveInsideStraightDraw##
##f$OpenPPL_HaveNuts##
##f$OpenPPL_HaveNutStraightFlush##
##f$OpenPPL_HaveOpenEndedStraightDraw##
##f$OpenPPL_HaveQuads##
##f$OpenPPL_HaveOverPair##
##f$OpenPPL_HaveOverPairOrBetter##
##f$OpenPPL_HavePair##
##f$OpenPPL_HavePocketPair##
##f$OpenPPL_HaveRunnerRunnerFlushAtRiver##
##f$OpenPPL_HaveSet##
##f$OpenPPL_HaveTopTrips##
##f$OpenPPL_HaveTrips##
##f$OpenPPL_TripsRank##
##f$OpenPPL_TripsKickerRank##
##f$OpenPPL_HaveStraight##
##f$OpenPPL_HaveStraightDraw##
##f$OpenPPL_HaveStraightFlush##
##f$OpenPPL_CommonCard0Paired##
##f$OpenPPL_CommonCard1Paired##
##f$OpenPPL_CommonCard2Paired##
##f$OpenPPL_CommonCard3Paired##
##f$OpenPPL_CommonCard4Paired##
##f$OpenPPL_RankBitsCommonWithoutCommonRank0Pair##
##f$OpenPPL_RankBitsCommonWithoutCommonRank1Pair##
##f$OpenPPL_RankBitsCommonWithoutCommonRank2Pair##
##f$OpenPPL_RankBitsCommonWithoutCommonRank3Pair##
##f$OpenPPL_RankBitsCommonWithoutCommonRank4Pair##
##f$OpenPPL_RankBitsCommonWithoutPairs##
##f$OpenPPL_HighestUnpairedBoardCard##
##f$OpenPPL_HaveTopNonBoardPairedPair##
##f$OpenPPL_HaveTopPair##
##f$OpenPPL_TopPairRank##
##f$OpenPPL_TopPairKickerRank##
##f$OpenPPL_SecondTopPairRank##
##f$OpenPPL_HaveTopSet##
##f$OpenPPL_HaveTopTwoPair##
##f$OpenPPL_RankbitsCommonWithoutAceLow##
##f$OpenPPL_HaveTwoPair##
##f$OpenPPL_HaveUnderPair##
##f$OpenPPL_HaveUnderStraight##
##f$OpenPPL_NumberOfUnknownSuitedOvercards##
##f$OpenPPL_RankOfTopPairOnBoard##
##f$OpenPPL_RankOfBottomPairOnBoard##
##f$OpenPPL_Overcards##
##f$OpenPPL_PairInHand##
##f$OpenPPL_RunnerRunnerFlushPossibleAtRiver##
##f$OpenPPL_SuitsInHand##*/

CSymbolEngineOpenPPLOpenPPLHandStrength *p_symbol_engine_open_ppl_handstrength = NULL;

CSymbolEngineOpenPPLOpenPPLHandStrength::CSymbolEngineOpenPPLOpenPPLHandStrength()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	//
	// This engine does not use any other engines.
}

CSymbolEngineOpenPPLOpenPPLHandStrength::~CSymbolEngineOpenPPLOpenPPLHandStrength()
{}

void CSymbolEngineOpenPPLOpenPPLHandStrength::InitOnStartup()
{
	ResetOnConnection();
}

void CSymbolEngineOpenPPLOpenPPLHandStrength::ResetOnConnection()
{}

void CSymbolEngineOpenPPLOpenPPLHandStrength::ResetOnHandreset()
{}

void CSymbolEngineOpenPPLOpenPPLHandStrength::ResetOnNewRound()
{}

void CSymbolEngineOpenPPLOpenPPLHandStrength::ResetOnMyTurn()
{}

void CSymbolEngineOpenPPLOpenPPLHandStrength::ResetOnHeartbeat()
{}

bool CSymbolEngineOpenPPLOpenPPLHandStrength::EvaluateSymbol(const char *name, double *result)
{
	// Symbol of a different symbol-engine
	return false;
}