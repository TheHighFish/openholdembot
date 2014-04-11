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
#include "CSymbolEngineOpenPPLPositions.h"

#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CStringMatch.h"
#include "MagicNumbers.h"

/*
##f$OpenPPL_StillToActPreflop##
##f$OpenPPL_StillToActPostflop##
##f$OpenPPL_StillToAct##
##f$OpenPPL_SmallBlind##
##f$OpenPPL_BigBlind##
##f$OpenPPL_Button##
##f$OpenPPL_CutOff##
##f$OpenPPL_MiddlePosition3##
##f$OpenPPL_MiddlePosition2##
##f$OpenPPL_MiddlePosition1##
##f$OpenPPL_EarlyPosition3##
##f$OpenPPL_EarlyPosition2##
##f$OpenPPL_EarlyPosition1##
##f$OpenPPL_UTG##
##f$OpenPPL_InSmallBlind##
##f$OpenPPL_InBigBlind##
##f$OpenPPL_TheBlinds##
##f$OpenPPL_InTheBlinds##
##f$OpenPPL_EarlyPosition##
##f$OpenPPL_InEarlyPosition##
##f$OpenPPL_MiddlePosition##
##f$OpenPPL_InMiddlePosition##
##f$OpenPPL_LatePosition##
##f$OpenPPL_InLatePosition##
##f$OpenPPL_InButton##
##f$OpenPPL_InCutOff##
##f$OpenPPL_InMiddlePosition3##
##f$OpenPPL_InMiddlePosition2##
##f$OpenPPL_InMiddlePosition1##
##f$OpenPPL_InEarlyPosition3##
##f$OpenPPL_InEarlyPosition2##
##f$OpenPPL_InEarlyPosition1##
##f$OpenPPL_InUTG##
##f$OpenPPL_TrueDealPositionOfBigBlind##
##f$OpenPPL_BigBlindChair##
##f$OpenPPL_SmallBlindChair##
##f$OpenPPL_ButtonChair##
##f$OpenPPL_CutOffChair##
##f$OpenPPL_MiddlePosition3Chair##
##f$OpenPPL_MiddlePosition2Chair##
##f$OpenPPL_MiddlePosition1Chair##
##f$OpenPPL_EarlyPosition3Chair##
##f$OpenPPL_EarlyPosition2Chair##
##f$OpenPPL_EarlyPosition1Chair##
##f$OpenPPL_DealPositionLastRaiser##
##f$OpenPPL_EarlyPosition1Calling##
##f$OpenPPL_EarlyPosition2Calling##
##f$OpenPPL_EarlyPosition3Calling##
##f$OpenPPL_MiddlePosition1Calling##
##f$OpenPPL_MiddlePosition2Calling##
##f$OpenPPL_MiddlePosition3Calling##
##f$OpenPPL_CutOffCalling##
##f$OpenPPL_ButtonCalling##
##f$OpenPPL_SmallBlindCalling##
##f$OpenPPL_BigBlindCalling##
##f$OpenPPL_EarlyPosition1Raising##
##f$OpenPPL_EarlyPosition2Raising##
##f$OpenPPL_EarlyPosition3Raising##
##f$OpenPPL_MiddlePosition1Raising##
##f$OpenPPL_MiddlePosition2Raising##
##f$OpenPPL_MiddlePosition3Raising##
##f$OpenPPL_CutOffRaising##
##f$OpenPPL_ButtonRaising##
##f$OpenPPL_SmallBlindRaising##
##f$OpenPPL_BigBlindRaising##
##f$OpenPPL_FirstCallerPosition##
##f$OpenPPL_FirstRaiserPosition##
##f$OpenPPL_LastCallerPosition##
##f$OpenPPL_LastRaiserPosition##
##f$OpenPPL_First##
##f$OpenPPL_Middle##
##f$OpenPPL_Last##
##f$OpenPPL_Position##
*/

CSymbolEngineOpenPPLPositions *p_symbol_engine_open_ppl_positions = NULL;

CSymbolEngineOpenPPLPositions::CSymbolEngineOpenPPLPositions()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	//
	// This engine does not use any other engines.
}

CSymbolEngineOpenPPLPositions::~CSymbolEngineOpenPPLPositions()
{}

void CSymbolEngineOpenPPLPositions::InitOnStartup()
{
	ResetOnConnection();
}

void CSymbolEngineOpenPPLPositions::ResetOnConnection()
{}

void CSymbolEngineOpenPPLPositions::ResetOnHandreset()
{}

void CSymbolEngineOpenPPLPositions::ResetOnNewRound()
{}

void CSymbolEngineOpenPPLPositions::ResetOnMyTurn()
{}

void CSymbolEngineOpenPPLPositions::ResetOnHeartbeat()
{}

bool CSymbolEngineOpenPPLPositions::EvaluateSymbol(const char *name, double *result)
{
	// Symbol of a different symbol-engine
	return false;
}