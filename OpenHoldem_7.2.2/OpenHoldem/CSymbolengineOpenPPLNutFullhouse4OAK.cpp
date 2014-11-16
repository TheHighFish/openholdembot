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
#include "CSymbolEngineOpenPPLNutFullHouse4OAK.h"

#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CStringMatch.h"
#include "MagicNumbers.h"

/*##f$OpenPPL_BoardKicker_FourOfAKindOnBoard##
##f$OpenPPL_NFH4OAK_FullHouseOnBoard_WeHaveOverPairToPair##
##f$OpenPPL_NFH4OAK_FullHouseOnBoard_WePlayTheBoard##
##f$OpenPPL_RankOfPairOfFullHouse##
##f$OpenPPL_RankOfSetOfFullHouse##
##f$OpenPPL_NFH4OAK_PairOnBoard_WeHitThePairAndAnotherCard##
##f$OpenPPL_OvercardsOnBoardToRankOfSetOfFullHouse##
##f$OpenPPL_NFH4OAK_TwoPairsOnBoard_WeHitBottomSet##
##f$OpenPPL_NFH4OAK_TwoPairsOnBoard_WeHitTopSet##
##f$OpenPPL_NFH4OAK_TwoPairsOnBoard_WeHitBottomQuads##
##f$OpenPPL_NFH4OAK_TwoPairsOnBoard_WeHitTopQuads##
##f$OpenPPL_NFH4OAK_TwoPairsOnBoard_WePairBothPairs##
##f$OpenPPL_NFH4OAK_FullHouseOnBoard_WePairBothBoardCards##
##f$OpenPPL_NFH4OAK_FullHouseOnBoard_WePairHighSetOnBoard##
##f$OpenPPL_NFH4OAK_FullHouseOnBoard_WePairLowSetOnBoard##
##f$OpenPPL_NFH4OAK_FullHouseOnBoard_WePairHighPairOnBoard##
##f$OpenPPL_NFH4OAK_FullHouseOnBoard_WePairLowPairOnBoard##
##f$OpenPPL_NFH4OAK_PairOnBoard_WeHitASetWithPocketPair##
##f$OpenPPL_NFH4OAK_PairOnBoard_WeHitQuads##
##f$OpenPPL_NutFullHouseOrFourOfAKind_FourOfAKindOnBoard##
##f$OpenPPL_NutFullHouseOrFourOfAKind_FullHouseOnBoard##
##f$OpenPPL_RankOfSetOnBoard##
##f$OpenPPL_RankOfSpareCardWhenTwoPairOnBoard##
##f$OpenPPL_NutFullHouseOrFourOfAKind_ThreeOfAKindOnBoard##
##f$OpenPPL_NutFullHouseOrFourOfAKind_TwoPairsOnBoard##
##f$OpenPPL_NutFullHouseOrFourOfAKind_PairOnBoard##
##f$OpenPPL_NutFullHouseOrFourOfAKind##*/

CSymbolEngineOpenPPLNutFullHouse4OAK *p_symbol_engine_open_ppl_nut_fullhouse_4oak = NULL;

CSymbolEngineOpenPPLNutFullHouse4OAK::CSymbolEngineOpenPPLNutFullHouse4OAK()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	//
	// This engine does not use any other engines.
}

CSymbolEngineOpenPPLNutFullHouse4OAK::~CSymbolEngineOpenPPLNutFullHouse4OAK()
{}

void CSymbolEngineOpenPPLNutFullHouse4OAK::InitOnStartup()
{
	ResetOnConnection();
}

void CSymbolEngineOpenPPLNutFullHouse4OAK::ResetOnConnection()
{}

void CSymbolEngineOpenPPLNutFullHouse4OAK::ResetOnHandreset()
{}

void CSymbolEngineOpenPPLNutFullHouse4OAK::ResetOnNewRound()
{}

void CSymbolEngineOpenPPLNutFullHouse4OAK::ResetOnMyTurn()
{}

void CSymbolEngineOpenPPLNutFullHouse4OAK::ResetOnHeartbeat()
{}

bool CSymbolEngineOpenPPLNutFullHouse4OAK::EvaluateSymbol(const char *name, double *result)
{
	// Symbol of a different symbol-engine
	return false;
}