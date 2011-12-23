#include "stdafx.h"
#include <atlstr.h>
#include <time.h>
#include "CAutoconnector.h"
#include "CHandresetDetector.h"
#include "COcclusionCheck.h"
#include "CPreferences.h"
#include "CRebuyManagement.h"
#include "CScraper.h"
#include "CSymbols.h"
#include "..\CTablemap\CTablemap.h"
#include "CTableLimits.h"
#include "debug.h"


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
	unsigned int MinimumTimeDifference = prefs.rebuy_minimum_time_to_next_try();
	// Make sure, we don't try to rebuy too often in a short time
	time(&CurrentTime);
	double RebuyTimeDifference = difftime(CurrentTime, RebuyLastTime);
	if (RebuyTimeDifference < MinimumTimeDifference)
	{
		write_log(prefs.debug_rebuy(), "[CRebuyManagement] MinimumDelayElapsed(): false\n");
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
	else if (p_handreset_detector->GetHandNumber() > PreviousRebuyHandNumber)
	{
		return true;
	}
	write_log(prefs.debug_rebuy(), "[CRebuyManagement] ChangeInHandNumber(): false\n");
	return false;
}

bool CRebuyManagement::NoCards()
{
	if (!prefs.rebuy_condition_no_cards()) 
	{
		return true;
	}
	double UserChair = p_symbols->sym()->userchair;
	if ((UserChair < 0) || (UserChair > 9)) 
	{
		// "No cards", but not even seated.
		// We should never get into that situation,
		// as the autoplayer can't get enabled without a userchair.
		// If all goes wrong, the rebuy-script has to handle that case.
		return true;
	}

	const unsigned int Card0 = p_scraper->card_player(UserChair, 0);
	int unsigned Card1 = p_scraper->card_player(UserChair, 1);
	if (((Card0 == CARD_NOCARD) || (Card0 == WH_NOCARD))
		&& ((Card1 == CARD_NOCARD) || (Card1 == WH_NOCARD)))
	{
		return true;
	}
	// We hold cards. No good time to rebuy,
	// if we play at a real casino.
	write_log(prefs.debug_rebuy(), "[CRebuyManagement] No_Cards: false: we hold cards.\n");	
	return false;
}

bool CRebuyManagement::OcclusionCheck()
{
	if (!prefs.rebuy_condition_heuristic_check_for_occlusion()) 
	{
		return true;
	}
	else if (p_occlusioncheck->UserBalanceOccluded())
	{
		write_log(prefs.debug_rebuy(), "[CRebuyManagement] OcclusionCheck: false (occluded)\n");
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
		write_log(prefs.debug_rebuy(), "[CRebuyManagement] RebuyPossible: true\n");
		return true;
	}
	else
	{
		write_log(prefs.debug_rebuy(), "[CRebuyManagement] RebuyPossible: false\n");
		return false;
	}
}

void CRebuyManagement::TryToRebuy()
{
	write_log(prefs.debug_rebuy(), "[CRebuyManagement] TryToRebuy()\n");
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
	write_log(prefs.debug_rebuy(), "[CRebuyManagement] ExecuteRebuyScript");
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
	double SmallBlind = p_tablelimits->sblind();
	double BigBlind = p_tablelimits->bblind();
	double BigBet = p_tablelimits->bbet();
	double UserChair = p_symbols->sym()->userchair;
	double Balance = p_symbols->sym()->balance[10];
	double TargetAmount = p_symbols->f$rebuy();
	CString RebuyScript = prefs.rebuy_script();
	CString CommandLine;
	CommandLine.Format(CString("%s %s %u %f %f %f %f %f %f"), 
		RebuyScript, Casino, WindowHandleOfThePokerTable, 
		UserChair, Balance, SmallBlind, BigBlind, BigBet, TargetAmount);
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
	write_log(prefs.debug_rebuy(), "[CRebuyManagement] command line: %s\n", CommandLine);
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
		write_log(prefs.debug_rebuy(), ErrorMessage.GetBuffer());
		MessageBox(0, ErrorMessage.GetBuffer(), "Error", 0);
	}
}


