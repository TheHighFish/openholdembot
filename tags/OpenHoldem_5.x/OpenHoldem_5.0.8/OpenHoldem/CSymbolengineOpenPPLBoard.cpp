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
#include "CSymbolEngineOpenPPLBoard.h"

#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CStringMatch.h"
#include "CSymbolEnginePokerval.h"
#include "MagicNumbers.h"
#include "NumericalFunctions.h"

/*
##f$OpenPPL_RankOfFirstHoleCard##
##f$OpenPPL_RankOfSecondHoleCard##
##f$OpenPPL_FirstFlopCard##
##f$OpenPPL_SecondFlopCard##
##f$OpenPPL_ThirdFlopCard##
##f$OpenPPL_TurnCard##
##f$OpenPPL_RiverCard##
##f$OpenPPL_2ndBestBoardCard##
##f$OpenPPL_AcePresentOnFlop##
##f$OpenPPL_AcePresentOnTurn##
##f$OpenPPL_BestBoardCard##
##f$OpenPPL_DoubleGutShotOnBoard##
##f$OpenPPL_ExactlyOneStraightPossibleOnFlop##
##f$OpenPPL_FlopCardPairedonTurn##
##f$OpenPPL_FlushPossible##
##f$OpenPPL_FlushOnBoard##
##f$OpenPPL_FlushPossibleOnFlop##
##f$OpenPPL_FlushPossibleOnTurn##
##f$OpenPPL_FourCardsToWheelOnBoard##
##f$OpenPPL_FourOf1SuiteOnTurn##
##f$OpenPPL_FullHouseOnBoard##
##f$OpenPPL_HighestRankOnFlop##
##f$OpenPPL_KingPresentOnFlop##
##f$OpenPPL_LowCardsOnBoard##
##f$OpenPPL_MoreThanOneStraightPossibleOnFlop##
##f$OpenPPL_MoreThanOneStraightPossibleOnRiver##
##f$OpenPPL_RankBitsCommonRiver##
##f$OpenPPL_NumberOfStraightsPossibleOnTurn##
##f$OpenPPL_NumberOfStraightsPossibleOnRiver##
##f$OpenPPL_MoreThanOneOneCardStraightPossible##
##f$OpenPPL_MoreThanOneStraightPossibleOnTurn##
##f$OpenPPL_NumberOfOneCardStraightsPossibleOnTurn##
##f$OpenPPL_OneCardFlushPossible##
##f$OpenPPL_OneCardStraightFlushPossible##
##f$OpenPPL_StraightFlushOnBoard##
##f$OpenPPL_OneCardStraightPossible##
##f$OpenPPL_OneCardStraightPossibleOnTurn_AceHigh##
##f$OpenPPL_OneCardStraightPossibleOnTurn_KingHigh##
##f$OpenPPL_OneCardStraightPossibleOnTurn_QueenHigh##
##f$OpenPPL_OneCardStraightPossibleOnTurn_JackHigh##
##f$OpenPPL_OneCardStraightPossibleOnTurn_TenHigh##
##f$OpenPPL_OneCardStraightPossibleOnTurn_NineHigh##
##f$OpenPPL_OneCardStraightPossibleOnTurn_EightHigh##
##f$OpenPPL_OneCardStraightPossibleOnTurn_SevenHigh##
##f$OpenPPL_OneCardStraightPossibleOnTurn_SixHigh##
##f$OpenPPL_OneCardStraightPossibleOnTurn_FiveHigh##
##f$OpenPPL_OneCardStraightPossibleOnTurn##
##f$OpenPPL_Only1OneCardStraightPossible##
##f$OpenPPL_OpenEndedStraightDrawOnBoard##
##f$OpenPPL_OpenEndedStraightDrawPossibleOnFlop##
##f$OpenPPL_IndividualOverCardsOnBoard##
##f$OpenPPL_OvercardsOnBoard##
##f$OpenPPL_PairOnBoard##
##f$OpenPPL_PairOnFlop##
##f$OpenPPL_LowestFlopCard##
##f$OpenPPL_LowestFlopCardPairedOnTurn##
##f$OpenPPL_OnlyOneStraightPossible##
##f$OpenPPL_PairOnTurn##
##f$OpenPPL_QuadsOnBoard##
##f$OpenPPL_QueenPresentOnFlop##
##f$OpenPPL_RankBitsCommonLowCardsOnly##
##f$OpenPPL_RiverCardisOvercardToBoard##
##f$OpenPPL_2ndTopFlopCard##
##f$OpenPPL_2ndTopFlopCard_UnpairedFlop##
##f$OpenPPL_2ndTopFlopCard_PairOnFlop##
##f$OpenPPL_SecondTopFlopCardPairedOnRiver##
##f$OpenPPL_SecondTopFlopCardPairedOnTurn##
##f$OpenPPL_2ndTopFlopCardPairedOnRiver##
##f$OpenPPL_2ndTopFlopCardPairedOnTurn##
##f$OpenPPL_srankbitshero##
##f$OpenPPL_srankavailable##
##f$OpenPPL_StraightFlushPossibleByOthers##
##f$OpenPPL_SuitsOnBoard##
##f$OpenPPL_SuitsOnFlop##
##f$OpenPPL_SuitsOnRiver##
##f$OpenPPL_SuitsOnTurn##
##f$OpenPPL_StraightFlushPossible##
##f$OpenPPL_StraightOnBoard##
##f$OpenPPL_StraightPossible##
##f$OpenPPL_RankBitsCommOnFlop##
##f$OpenPPL_RankBitsCommonTurn##
##f$OpenPPL_StraightPossibleOnFlop##
##f$OpenPPL_StraightPossibleOnTurn##
##f$OpenPPL_ThreeCardStraightOnBoard##
##f$OpenPPL_TopFlopCard##
##f$OpenPPL_TopFlopCardPairedonRiver##
##f$OpenPPL_TopFlopCardPairedonTurn##
##f$OpenPPL_TripsOnBoard##
##f$OpenPPL_TripsOnBoardOnTurn##
##f$OpenPPL_TurnCardisOvercardToBoard##
##f$OpenPPL_TurnCardPaired##
##f$OpenPPL_TwoOfOneSuitPresentOnFlop##
##f$OpenPPL_TwoPairOnBoard##
##f$OpenPPL_TwoPairOnBoardOnTurn##
##f$OpenPPL_UncoordinatedFlop##
##f$OpenPPL_WheelPossible##
*/

CSymbolEngineOpenPPLBoard *p_symbol_engine_open_ppl_board = NULL;

CSymbolEngineOpenPPLBoard::CSymbolEngineOpenPPLBoard()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	assert(p_symbol_engine_pokerval != NULL);
}

CSymbolEngineOpenPPLBoard::~CSymbolEngineOpenPPLBoard()
{}

void CSymbolEngineOpenPPLBoard::InitOnStartup()
{
	ResetOnConnection();
}

void CSymbolEngineOpenPPLBoard::ResetOnConnection()
{}

void CSymbolEngineOpenPPLBoard::ResetOnHandreset()
{
	_wheel_possible = false;
}

void CSymbolEngineOpenPPLBoard::ResetOnNewRound()
{}

void CSymbolEngineOpenPPLBoard::ResetOnMyTurn()
{
	_wheel_possible = WheelPossible();
}

void CSymbolEngineOpenPPLBoard::ResetOnHeartbeat()
{}

bool CSymbolEngineOpenPPLBoard::WheelPossible()
{
	const int kRankbits_A2345 = 0x1F; // 0b111110
	return (bitcount(
		p_symbol_engine_pokerval->rankbitscommon() & kRankbits_A2345) 
			>= 3);
}

bool CSymbolEngineOpenPPLBoard::EvaluateSymbol(const char *name, double *result)
{
	if (memcmp(name, "wheelpossible", 13)==0 && strlen(name)==13)	
	{
		*result = _wheel_possible;
		return true;
	}
	// Symbol of a different symbol-engine
	return false;
}