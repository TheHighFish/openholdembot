#include "stdafx.h"
#include <atlstr.h>
#include <time.h>
#include "CAutoconnector.h"
#include "CPreferences.h"
#include "CRebuyManagement.h"
#include "CSymbols.h"
#include "debug.h"


CRebuyManagement *p_rebuymanagement = NULL;


CRebuyManagement::CRebuyManagement()
{
	// Init time of last rebuy in a reasonable way at startup.
	time(&RebuyLastTime);
	PreviousRebuyHandNumber = -1;
}

CRebuyManagement::~CRebuyManagement()	
{
}


bool CRebuyManagement::MinimumDelayElapsed()
{
	unsigned int MinimumTimeDifference = prefs.rebuy_minimum_time_to_next_try();
	// Make sure, we don't try to rebuy too often in a short time
	time(&CurrentTime);
	double RebuyTimeDifference = difftime(CurrentTime, RebuyLastTime);
	if (RebuyTimeDifference < MinimumTimeDifference)
	{
		write_log(3, "CRebuyManagement::MinimumDelayElapsed(): false");
		return false;
	}
	return true;
}

bool CRebuyManagement::ChangeInHandNumber()
{
	if (!prefs.rebuy_condition_change_in_handnumber()) 
	{
		return true;
	}
	else if (p_symbols->sym()->handnumber > PreviousRebuyHandNumber)
	{
		return true;
	}
	else
	{
		write_log(3, "CRebuyManagement::ChangeInHandNumber(): false");
		return false;
	}	
}

bool CRebuyManagement::NoCards()
{
	if (!prefs.rebuy_condition_no_cards()) return true;
	if (/*!(GameStateEngine.CasinoIsPokerAcademy() || GameStateEngine.CasinoIsManualMode())
		&&*/ false /*!!!BettingAction_Does_Hero_Still_Hold_Cards*/)
	{
		// We hold cards. No good time to rebuy,
		// if we play at a real casino.
		//Debug.Send_Message("CRebuyManagement::RebuyCondition_No_Cards: false: we hold cards.");	
		return false;
	}
	//Debug.Send_Message("CRebuyManagement::RebuyCondition_No_Cards: true.");
	return true;
}

bool CRebuyManagement::OcclusionCheck()
{
	if (!prefs.rebuy_condition_heuristic_check_for_occlusion()) return true;
	//return (!OcclusionCheck.UserBalanceOccluded());
	return true;
}


bool CRebuyManagement::RebuyPossible()
{
	if (MinimumDelayElapsed()
		&& ChangeInHandNumber()
		&& NoCards()
		&& OcclusionCheck())
	{
		write_log(3, "CRebuyManagement::RebuyPossible: true");
		return true;
	}
	else
	{
		write_log(3, "CRebuyManagement::RebuyPossible: false");
		return false;
	}
}

void CRebuyManagement::TryToRebuy()
{
	write_log(3, "CRebuyManagement::TryToRebuy()");
	if (RebuyPossible())
	{
		RebuyLastTime = CurrentTime;		
		PreviousRebuyHandNumber = p_symbols->sym()->handnumber;
		ExecuteUniversalRebuyScript();
	}
}	

void CRebuyManagement::ExecuteUniversalRebuyScript()
{
	// Call the external rebuy script.
	//
	// CAUTION! DO NOT USE THIS FUNCTION DIRECTLY!
	// It has to be protected by a mutex.
	//
	// Build command-line-options for rebuy-script
	write_log(3, "CRebuyManagement::ExecuteUniversalRebuyScript");
	//!!!CString Casino = GameStateEngine.GetCasinoName();
	HWND WindowHandleOfThePokerTable = p_autoconnector->attached_hwnd();
	double UserChair = p_symbols->sym()->userchair;
	double Balance = p_symbols->sym()->balance[10];
	double BigBlind = p_symbols->sym()->bblind;
	double TargetAmount = p_symbols->f$rebuy();
	CString CommandLine;
	CommandLine.Format(CString("%s %s %u %f %f %f %f"), 
		CString("Universal_RebuyScript.exe"), 
		/*Casino,*/ WindowHandleOfThePokerTable, 
		UserChair, Balance, BigBlind, TargetAmount);
	/* 
	MessageBox(0, CommandLine, CString("CommandLine"), 0);
	*/
	// For some docu on "CreateProcess" see:
	// http://pheatt.emporia.edu/courses/2005/cs260s05/hand39/hand39.htm
	// http://msdn.microsoft.com/en-us/library/aa908775.aspx
	// http://www.codeproject.com/KB/threads/CreateProcessEx.aspx
	STARTUPINFO StartupInfo;
    PROCESS_INFORMATION ProcessInfo; 
	memset(&StartupInfo, 0, sizeof(StartupInfo));
	memset(&ProcessInfo, 0, sizeof(ProcessInfo));
	StartupInfo.cb = sizeof(StartupInfo); 
	write_log(3, "Rebuy: ", CommandLine);
	if(CreateProcess(NULL, CommandLine.GetBuffer(), NULL, 
		false, 0, NULL, NULL, 0, &StartupInfo, &ProcessInfo))
	{
		// Wait for termination of the rebuy-script for at most 30 seconds.
		int ExitCode = WaitForSingleObject(ProcessInfo.hProcess, 30000);
		// To do: exit code!!!
	}
	else
	{
		write_log(0, "Calling rebuy-script failed.");
	}
}


