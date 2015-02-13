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
#include "WindowFunctions.h"

#include <assert.h>
#include "CSharedMem.h"
#include "Psapi.h"

RECT desktop_dimensions;
bool desktop_dimensions_calculated = false;


bool WinCalculateDesktopDimensions()
{
	if (!GetWindowRect(GetDesktopWindow(), &desktop_dimensions))
	{
		return false;
	}
	desktop_dimensions_calculated = true;
	return true;
}

bool WinIsOpenHoldem(HWND window)
{
	DWORD PID;
	if (!GetWindowThreadProcessId(window, &PID))
	{
		return false;
	}
	return (p_sharedmem->IsAnyOpenHoldemProcess(PID));
}

bool WinIsBring(HWND window)
{
	return WinIsFromThisProgram(window, "bring.exe");
}

bool WinIsOutOfScreen(HWND window)
{
	if (!desktop_dimensions_calculated)
	{
		WinCalculateDesktopDimensions();
	}
	RECT rect;
	if (!GetWindowRect(window, &rect))
	{
		return false;
	}
	return((rect.right < 0)
		|| (rect.bottom < 0)
		|| (rect.top > desktop_dimensions.bottom)
		|| (rect.left > desktop_dimensions.right));
}

bool WinIsTaskbar(HWND window)
{
	// Good way to Find and Detect the taskbar.
	// This should work with Windows XP and Windows 7
	HWND hShellWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL);
	return window == hShellWnd;
}

bool WinIsProgramManager(HWND window)
{
	return WinIsFromThisProgram(window, "progman.exe");
}

bool WinIsTaskManager(HWND window)
{
	return WinIsFromThisProgram(window, "taskmgr.exe");
}

bool WinIsFromThisProgram(HWND window, const char* programName)
{
	if (window)
	{
		DWORD dwProcessId = 0;
		DWORD dwThreadId = GetWindowThreadProcessId(window, &dwProcessId);

		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
									  PROCESS_VM_READ, FALSE, dwProcessId);

		if (hProcess)
		{
			 bool match = false;
			 char nameProc[MAX_PATH];
			 if (GetProcessImageFileName(hProcess, nameProc, MAX_PATH))
			 {
				 match = strstr(nameProc, programName) != NULL ;
			 }
			 CloseHandle(hProcess);
			 return match;
		}
	}
	return false;
}