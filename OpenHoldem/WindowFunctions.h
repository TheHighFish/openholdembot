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

#ifndef	INC_WINDOW_FUNCTIONS_H
#define INC_WINDOW_FUNCTIONS_H


inline void MinimizeWindow(HWND window)
{
	// http://msdn.microsoft.com/en-us/library/windows/desktop/ms633548%28v=vs.85%29.aspx
	ShowWindow(window, SW_MINIMIZE);
}

inline void WinGetProcessPath(HWND window, char* path, int max_length)
{
	//!!!int PID = GetWindowThreadProcessId(window, ));
	//GetModuleFileName(PID, path, max_length);
}

inline bool WinIsDesktop(HWND window)
{
	// http://msdn.microsoft.com/en-us/library/windows/desktop/ms633504%28v=vs.85%29.aspx
	return (GetDesktopWindow() == window);
}

inline bool WinIsMaximized(HWND window)
{
	return ::IsZoomed(window);
}

inline bool WinIsMinimized(HWND window)
{
	return ::IsIconic(window);
}

inline bool WinIsOpenHoldem(HWND window)
{
	return false; //!!!
}

inline bool WinIsOutOfScreen(HWND window)
{
	RECT rect;
	static RECT desktop;
	if (!GetWindowRect(window, &rect))
	{
		return false;
	}
	return((rect.right < 0)
		|| (rect.bottom < 0)
		|| (rect.top > desktop.bottom)
		|| (rect.left > desktop.right));
}

inline bool WinIsProgramManager(HWND window)
{
	return (FindWindow("Progman", "Program Manager") == window);
}

inline bool WinIsTaskManager(HWND window)
{
	return (FindWindow(NULL, "Windows Task-Manager") == window);
}

inline bool WinIsZeroSized(HWND window)
{
	RECT rect;
	if (!GetWindowRect(window, &rect))
	{
		return false;
	}
	return ((rect.right <= rect.left)
		|| (rect.bottom <= rect.top));
}

#endif INC_WINDOW_FUNCTIONS_H