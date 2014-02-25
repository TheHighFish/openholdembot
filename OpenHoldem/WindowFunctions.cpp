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
