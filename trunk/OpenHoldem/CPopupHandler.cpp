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
#include "CPopupHandler.h"

#include "CPreferences.h"
#include "WindowFunctions.h"

CPopupHandler::CPopupHandler()
{}

CPopupHandler::~CPopupHandler()
{}

void CPopupHandler::MinimizeAllOnstartup()
{
	HandleAllWindows(false);
}

void CPopupHandler::HandleAllWindows()
{
	int popup_blocker_method = preferences.popup_blocker();
	if (popup_blocker_method == k_popup_minimize)
	{
		HandleAllWindows(false);
	}
	else if (popup_blocker_method == k_popup_kill)
	{
		HandleAllWindows(true);
	}
	// else: disabled

	
}

void CPopupHandler::HandleAllWindows(bool hard_kill)
{
	
}


void CPopupHandler::HandlePotentialPopup(HWND potential_popup, bool hard_kill)
{
	// First: ignore all harmless windows
	if (!IsWindow(potential_popup))			return;
	if (!IsWindowVisible(potential_popup))	return;
	if (WinIsMinimized(potential_popup))	return;
	if (WinIsZeroSized(potential_popup))	return;
	if (WinIsOutOfScreen(potential_popup))	return;
	// Second: ignore the good windows
	// This includes:
	//   * program manager (desktop) to avoid flickering
	//   * task manager to be able to kill the popup-blocker if necessary ;-)
	if (WinIsDesktop(potential_popup))			return;
	if (WinIsTaskManager(potential_popup))		return;
	if (WinIsProgramManager(potential_popup))	return;
	if (WinIsOpenHoldem(potential_popup))		return;
	// Minimize or kill the remaining ones
	char title[MAX_WINDOW_TITLE];
	GetWindowText(potential_popup, title, MAX_WINDOW_TITLE);
	if (hard_kill)
	{
		// CloseWindow():
		// http://msdn.microsoft.com/en-us/library/windows/desktop/ms632678%28v=vs.85%29.aspx
		// DestroyWindow() can't be used:
		// http://msdn.microsoft.com/en-us/library/windows/desktop/ms632682%28v=vs.85%29.aspx
		//!!!CloseWindow(potential_popup);
		write_log(preferences.debug_popup_blocker(), "[CPopupHandler] Killed [%d] [%s]\n",
			potential_popup, title);
	}
	else
	{
		//!!!MinimizeWindow(potential_popup);
		write_log(preferences.debug_popup_blocker(), "[CPopupHandler] Minimized [%d] [%s]\n",
			potential_popup, title);
	}
}


