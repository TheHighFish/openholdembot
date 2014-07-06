//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#include "stdafx.h"
#include <atlstr.h>
#include <time.h>
#include "CAutoconnector.h"
#include "CAutoplayerFunctions.h"
#include "CHandresetDetector.h"
#include "COcclusionCheck.h"
#include "CPreferences.h"
#include "CRebuyManagement.h"
#include "CScraper.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineUserchair.h"
#include "..\CTablemap\CTablemap.h"
#include "CSymbolEngineTableLimits.h"
#include "CTableState.h"
#include "debug.h"
#include "OH_MessageBox.h"

CRebuyManagement *p_rebuymanagement = NULL;


CRebuyManagement::CRebuyManagement()
{
	// Init time of last rebuy in a reasonable way at startup.
	time(&RebuyLastTime);
	PreviousRebuyHandNumber = "";
}

CRebuyManagement::~CRebuyManagement()	
{
}


bool CRebuyManagement::MinimumDelayElapsed()
{
	unsigned int MinimumTimeDifference = preferences.rebuy_minimum_time_to_next_try();
	// Make sure, we don't try to rebuy too often in a short time
	time(&CurrentTime);
	double RebuyTimeDifference = difftime(CurrentTime, RebuyLastTime);
	if (RebuyTimeDifference < MinimumTimeDifference)
	{
		write_log(preferences.debug_rebuy(), "[CRebuyManagement] MinimumDelayElapsed(): false\n");
		return false;
	}
	return true;
}

bool CRebuyManagement::ChangeInHandNumber()
{
	if (!preferences.rebuy_condition_change_in_handnumber()) 
	{
		return true;
	}
	else if (p_handreset_detector->GetHandNumber() > PreviousRebuyHandNumber)
	{
		return true;
	}
	write_log(preferences.debug_rebuy(), "[CRebuyManagement] ChangeInHandNumber(): false\n");
	return false;
}

bool CRebuyManagement::NoCards()
{
	if (!preferences.rebuy_condition_no_cards()) 
	{
		return true;
	}
	int UserChair = p_symbol_engine_userchair->userchair();
	if ((UserChair < 0) || (UserChair > 9)) 
	{
		// "No cards", but not even seated.
		// We should never get into that situation,
		// as the autoplayer can't get enabled without a userchair.
		// If all goes wrong, the rebuy-script has to handle that case.
		return true;
	}
	if (p_table_state->_players[UserChair].HasKnownCards())
	{
		return true;
	}
	// We hold cards. No good time to rebuy,
	// if we play at a real casino.
	write_log(preferences.debug_rebuy(), "[CRebuyManagement] No_Cards: false: we hold cards.\n");	
	return false;
}

bool CRebuyManagement::OcclusionCheck()
{
	if (!preferences.rebuy_condition_heuristic_check_for_occlusion()) 
	{
		return true;
	}
	else if (p_occlusioncheck->UserBalanceOccluded())
	{
		write_log(preferences.debug_rebuy(), "[CRebuyManagement] OcclusionCheck: false (occluded)\n");
		return false;
	}
	return true;
}


bool CRebuyManagement::RebuyPossible()
{
	if (MinimumDelayElapsed()
		&& ChangeInHandNumber()
		&& NoCards()
		&& OcclusionCheck())
	{
		write_log(preferences.debug_rebuy(), "[CRebuyManagement] RebuyPossible: true\n");
		return true;
	}
	else
	{
		write_log(preferences.debug_rebuy(), "[CRebuyManagement] RebuyPossible: false\n");
		return false;
	}
}

void CRebuyManagement::TryToRebuy()
{
	write_log(preferences.debug_rebuy(), "[CRebuyManagement] TryToRebuy()\n");
	if (RebuyPossible())
	{
		RebuyLastTime = CurrentTime;		
		PreviousRebuyHandNumber = p_handreset_detector->GetHandNumber();
		ExecuteRebuyScript();
	}
}	

void CRebuyManagement::ExecuteRebuyScript()
{
	// Call the external rebuy script.
	//
	// CAUTION! DO NOT USE THIS FUNCTION DIRECTLY!
	// It has to be protected by a mutex.
	// We assume, the autoplayer does that.
	//
	// Build command-line-options for rebuy-script
	write_log(preferences.debug_rebuy(), "[CRebuyManagement] ExecuteRebuyScript");
	CString Casino;
	if (p_tablemap->s$()->find("sitename") != p_tablemap->s$()->end())
	{
		Casino = p_tablemap->s$()->find("sitename")->second.text.GetString();
	}
	else
	{
		Casino = "Undefined";
	}
	HWND WindowHandleOfThePokerTable = p_autoconnector->attached_hwnd();
	double SmallBlind = p_symbol_engine_tablelimits->sblind();
	double BigBlind = p_symbol_engine_tablelimits->bblind();
	double BigBet = p_symbol_engine_tablelimits->bigbet();
	int UserChair = p_symbol_engine_userchair->userchair();
	double Balance = p_symbol_engine_chip_amounts->balance(UserChair);
	double TargetAmount = p_autoplayer_functions->f$rebuy();
	CString RebuyScript = preferences.rebuy_script();
	CString CommandLine;
	CommandLine.Format(CString("%s %s %u %f %f %f %f %f %f"), 
		RebuyScript, Casino, WindowHandleOfThePokerTable, 
		UserChair, Balance, SmallBlind, BigBlind, BigBet, TargetAmount);
	// For some docu on "CreateProcess" see:
	// http://pheatt.emporia.edu/courses/2005/cs260s05/hand39/hand39.htm
	// http://msdn.microsoft.com/en-us/library/aa908775.aspx
	// http://www.codeproject.com/KB/threads/CreateProcessEx.aspx
	STARTUPINFO StartupInfo;
    PROCESS_INFORMATION ProcessInfo; 
	memset(&StartupInfo, 0, sizeof(StartupInfo));
	memset(&ProcessInfo, 0, sizeof(ProcessInfo));
	StartupInfo.cb = sizeof(StartupInfo); 
	write_log(preferences.debug_rebuy(), "[CRebuyManagement] command line: %s\n", CommandLine);
	if(CreateProcess(NULL, CommandLine.GetBuffer(), NULL, 
		false, 0, NULL, NULL, 0, &StartupInfo, &ProcessInfo))
	{
		// Docu for WaitForSingleObject:
		// http://msdn.microsoft.com/en-us/library/ms687032(VS.85).aspx
		// It seems, none of the exitcodes is relevant for us.
		//
		// Wait for termination of the rebuy-script, if necessary forever,
		// as we can't release the (autoplayer)mutex as long as the script is running.
		int ExitCode = WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
	}
	else
	{
		CString ErrorMessage = CString("Could not execute rebuy-script: ") + CString(RebuyScript) + "\n";
		write_log(preferences.debug_rebuy(), ErrorMessage.GetBuffer());
		OH_MessageBox_Error_Warning(ErrorMessage.GetBuffer(), "Error");
	}
}


