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
#include "CSymbolEngineOpenPPLBettingAction.h"

#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CStringMatch.h"
#include "MagicNumbers.h"

/*##f$OpenPPL_InitBets##
##f$OpenPPL_Bets##
##f$OpenPPL_BigBlindSittingOut##
##f$OpenPPL_SmallBlindSittingOut##
##f$OpenPPL_BotCalledBeforeFlop##
##f$OpenPPL_AggressorChair##
##f$OpenPPL_BotIsLastRaiser##
##f$OpenPPL_BotRaisedBeforeFlop##
##f$OpenPPL_BotRaisedOnFlop##
##f$OpenPPL_BotRaisedOnTurn##
##f$OpenPPL_BotsActionsOnFlop##
##f$OpenPPL_BotsActionsOnThisRound##
##f$OpenPPL_BotsActionsOnThisRoundIncludingChecks##
##f$OpenPPL_BotsActionsPreflop##
##f$OpenPPL_InitBotsLastAction##
##f$OpenPPL_BotsLastAction##
##f$OpenPPL_InitBotsLastPreflopAction##
##f$OpenPPL_BotsLastPreflopAction##
##f$OpenPPL_CalledOnFlop##
##f$OpenPPL_CalledOnTurn##
##f$OpenPPL_BotCheckedPreflop##
##f$OpenPPL_BotCheckedOnFlop##
##f$OpenPPL_BotCheckedOnTurn##
##f$OpenPPL_BotCheckedOnRiver##
##f$OpenPPL_AddNopponentscalling##
##f$OpenPPL_InitCalls##
##f$OpenPPL_Calls##
##f$OpenPPL_PT_LastCaller_Chair##
##f$OpenPPL_LastRaiserCurrentBet##
##f$OpenPPL_PlayersRaisingOrColdCallingTheLastRaise##
##f$OpenPPL_CallsSinceLastRaise##
##f$OpenPPL_Folds##
##f$OpenPPL_ChecksAfterHero##
##f$OpenPPL_ChecksInFrontOfHero##
##f$OpenPPL_CalculateChecks##
##f$OpenPPL_InitChecks##
##f$OpenPPL_Checks##
##f$OpenPPL_DealPosition1Chair##
##f$OpenPPL_CurrentbetOfDealPosition1Chair##
##f$OpenPPL_PlayersBehindDealPosition2ChairDidAct##
##f$OpenPPL_CalculateMissingSmallBlind##
##f$OpenPPL_InitMissingSmallBlind##
##f$OpenPPL_MissingSmallBlind##
##f$OpenPPL_NoBettingOnFlop##
##f$OpenPPL_NoBettingOnTurn##
##f$OpenPPL_NoVillainBetOrRaisedOnFlop##
##f$OpenPPL_NoVillainBetOrRaisedOnTurn##
##f$OpenPPL_OpponentCalledOnFlop##
##f$OpenPPL_OpponentCalledOnTurn##
##f$OpenPPL_OpponentStacksizeHeadsUp##
##f$OpenPPL_InitRaisesBeforeFlop##
##f$OpenPPL_InitRaisesOnFlop##
##f$OpenPPL_InitRaisesOnTurn##
##f$OpenPPL_NumberOfRaisesBeforeFlop##
##f$OpenPPL_NumberOfRaisesOnFlop##
##f$OpenPPL_NumberOfRaisesOnTurn##
##f$OpenPPL_Calculate_Raises##
##f$OpenPPL_InitRaises##
##f$OpenPPL_Raises##
##f$OpenPPL_RaisesBeforeFlop##
##f$OpenPPL_RaisesOnFlop##
##f$OpenPPL_RaisesOnTurn##
##f$OpenPPL_RaisesBeforeOurFirstActionThisRoundPostflop##
##f$OpenPPL_AntesPresent##
##f$OpenPPL_NumberOfBlindRaisersInThisGame##
##f$OpenPPL_NumberOfOpponentsLimpingPreflop##
##f$OpenPPL_RaisesBeforeOurFirstAction##
##f$OpenPPL_RaisesSinceLastPlayOnOurSecondAction##
##f$OpenPPL_RaisesSinceLastPlayAfterOurSecondAction##
##f$OpenPPL_RaisesSinceLastPlay##
##f$OpenPPL_Chair0StartingStackSize##
##f$OpenPPL_Chair1StartingStackSize##
##f$OpenPPL_Chair2StartingStackSize##
##f$OpenPPL_Chair3StartingStackSize##
##f$OpenPPL_Chair4StartingStackSize##
##f$OpenPPL_Chair5StartingStackSize##
##f$OpenPPL_Chair6StartingStackSize##
##f$OpenPPL_Chair7StartingStackSize##
##f$OpenPPL_Chair8StartingStackSize##
##f$OpenPPL_Chair9StartingStackSize##
##f$OpenPPL_ButtonStartingStackSize##
##f$OpenPPL_CutOffStartingStackSize##
##f$OpenPPL_MiddlePosition3StartingStackSize##
##f$OpenPPL_MiddlePosition2StartingStackSize##
##f$OpenPPL_MiddlePosition1StartingStackSize##
##f$OpenPPL_EarlyPosition3StartingStackSize##
##f$OpenPPL_EarlyPosition2StartingStackSize##
##f$OpenPPL_EarlyPosition1StartingStackSize##
##f$OpenPPL_BigBlindStartingStackSize##
##f$OpenPPL_SmallBlindStartingStackSize##
##f$OpenPPL_BigBlindCurrentBet##
##f$OpenPPL_SmallBlindCurrentBet##
##f$OpenPPL_MaximumPossibleBetsizeIndollars##
##f$OpenPPL_SidePotInDollars##
##f$OpenPPL_SidePot##
##f$OpenPPL_MaxStillToActStackSizeAfterSmallBlind##
##f$OpenPPL_MaxStillToActStackSizeAfterButton##
##f$OpenPPL_MaxStillToActStackSizeAfterCutOff##
##f$OpenPPL_MaxStillToActStackSizeAfterMiddlePosition3##
##f$OpenPPL_MaxStillToActStackSizeAfterMiddlePosition2##
##f$OpenPPL_MaxStillToActStackSizeAfterMiddlePosition1##
##f$OpenPPL_MaxStillToActStackSizeAfterEarlyPosition3##
##f$OpenPPL_MaxStillToActStackSizeAfterEarlyPosition2##
##f$OpenPPL_MaxStillToActStackSizeAfterEarlyPosition1##
##f$OpenPPL_MinStillToActStackSizeAfterSmallBlind##
##f$OpenPPL_MinStillToActStackSizeAfterButton##
##f$OpenPPL_MinStillToActStackSizeAfterCutOff##
##f$OpenPPL_MinStillToActStackSizeAfterMiddlePosition3##
##f$OpenPPL_MinStillToActStackSizeAfterMiddlePosition2##
##f$OpenPPL_MinStillToActStackSizeAfterMiddlePosition1##
##f$OpenPPL_MinStillToActStackSizeAfterEarlyPosition3##
##f$OpenPPL_MinStillToActStackSizeAfterEarlyPosition2##
##f$OpenPPL_MinStillToActStackSizeAfterEarlyPosition1##
##f$OpenPPL_InitMaxOpponentStackSize##
##f$OpenPPL_InitMinOpponentStackSize##
##f$OpenPPL_MaxOpponentStackSizeCalculation##
##f$OpenPPL_MinOpponentStackSizeCalculation##
##f$OpenPPL_OpponentsWithHigherStack##
##f$OpenPPL_CalculateStartingStackSize##
##f$OpenPPL_InitStartingStackSize##
##f$OpenPPL_StartingStackSize##
##f$OpenPPL_DollarsToCall##
##f$OpenPPL_AmountToCall##
##f$OpenPPL_BetSize##
##f$OpenPPL_BigBlindSize##
##f$OpenPPL_MaxOpponentStackSize##
##f$OpenPPL_MaxStillToActStackSize##
##f$OpenPPL_MinOpponentStackSize##
##f$OpenPPL_MinStillToActStackSize##
##f$OpenPPL_PotSize##
##f$OpenPPL_StackSize##
##f$OpenPPL_StackSizeAtBeginningOfCurrentRound##
##f$OpenPPL_TotalInvested##
##f$OpenPPL_TotalInvestedThisRound##*/

CSymbolEngineOpenPPLBettingAction *p_symbol_engine_open_ppl = NULL;

CSymbolEngineOpenPPLBettingAction::CSymbolEngineOpenPPLBettingAction()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	//
	// This engine does not use any other engines.
}

CSymbolEngineOpenPPLBettingAction::~CSymbolEngineOpenPPLBettingAction()
{}

void CSymbolEngineOpenPPLBettingAction::InitOnStartup()
{
	ResetOnConnection();
}

void CSymbolEngineOpenPPLBettingAction::ResetOnConnection()
{}

void CSymbolEngineOpenPPLBettingAction::ResetOnHandreset()
{}

void CSymbolEngineOpenPPLBettingAction::ResetOnNewRound()
{}

void CSymbolEngineOpenPPLBettingAction::ResetOnMyTurn()
{}

void CSymbolEngineOpenPPLBettingAction::ResetOnHeartbeat()
{}

bool CSymbolEngineOpenPPLBettingAction::EvaluateSymbol(const char *name, double *result)
{
	// Symbol of a different symbol-engine
	return false;
}