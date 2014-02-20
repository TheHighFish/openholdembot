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

void WinGetProcessPath(HWND window, TCHAR* path, int max_length)
{
	//int PID = GetWindowThreadProcessId(window, ));
	//GetModuleFileName(PID, path, max_length);

	/*HMODULE hModule = (HMODULE)GetClassLongPtr(window, GCLP_HMODULE);
	if(!hModule)
	{
		path = "";
		return;
	}
	if(!GetModuleFileName(hModule, path, sizeof(path) / sizeof(TCHAR)))
	{
		path = "";
		return;
	}*/

	path = "";

	DWORD PID;
	if (!GetWindowThreadProcessId(window, &PID))
	{
		return;
	}
	HANDLE process_handle = OpenProcess(PROCESS_QUERY_INFORMATION, false, PID);
	if (process_handle != NULL)
	{
		GetModuleFileNameEx(process_handle, NULL, path, max_length);
		CloseHandle(process_handle);
	}
}

bool WinIsOpenHoldem(HWND window)
{
	//TCHAR path[MAX_PATH];
	//WinGetProcessPath(window, path, MAX_PATH);
	//MessageBox(0, path, "WinProcessPath", 0);
	return false; //!!!
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
