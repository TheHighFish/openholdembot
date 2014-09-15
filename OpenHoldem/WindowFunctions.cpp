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
	if (window == hShellWnd)
		return true;

	// Need testing for Windows XP, if this work, The code below should be remove.
	RECT rect;
	if (!GetWindowRect(window, &rect))
	{
		return false;
	}
	// We don't know a perfect way to detect the taskbar.
	// Therefore we use some heuristics and assume it is a horizontal taskbar.
	// For me the size is (1028x30) and the x-position starts at -2.
	// We assume: height <= 32 and length >= 20 * height.
	const int kMaxTaskbarHeight = 32;
	int height = rect.bottom - rect.top;
	if (height > kMaxTaskbarHeight) return false;
	int length = rect.right - rect.left;
	if (length < 20 * height) return false;
	return true;
}