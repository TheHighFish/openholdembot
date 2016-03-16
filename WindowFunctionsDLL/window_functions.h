//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Window function library
//   a) quite independent of the rest
//   b) might be used by ManualMode too 
//      (ResizeToClientsize is needed for support 
//      of titlebars of different sizes (Windows10))
//
//******************************************************************************



#ifndef INC_WINDOW_FUNCTIONS_H
#define INC_WINDOW_FUNCTIONS_H

#ifdef WINDOW_FUNCTIONS_EXPORTS
#define WINDOW_FUNCTIONS_API __declspec(dllexport)
#else
#define WINDOW_FUNCTIONS_API __declspec(dllimport)
#endif

WINDOW_FUNCTIONS_API int CalculateTotalHeightForClientHeight(HWND window, int desired_client_height);

WINDOW_FUNCTIONS_API int CalculateTotalWidthForClientWith(HWND window, int desired_client_width);

WINDOW_FUNCTIONS_API void MinimizeWindow(HWND window);

WINDOW_FUNCTIONS_API void ResizeToClientSize(HWND window, int new_width, int new_height);

WINDOW_FUNCTIONS_API void ResizeToTotalSize(HWND window, int new_width, int new_height);

WINDOW_FUNCTIONS_API bool WinIsDesktop(HWND window);

WINDOW_FUNCTIONS_API bool WinIsMaximized(HWND window);

WINDOW_FUNCTIONS_API bool WinIsMinimized(HWND window);

WINDOW_FUNCTIONS_API bool WinIsBring(HWND window);

WINDOW_FUNCTIONS_API bool WinIsOutOfScreen(HWND window);

WINDOW_FUNCTIONS_API bool WinIsTaskbar(HWND window);

WINDOW_FUNCTIONS_API bool WinIsProgramManager(HWND window);

WINDOW_FUNCTIONS_API bool WinIsTaskManager(HWND window);

WINDOW_FUNCTIONS_API bool WinBelongsToExecutable(HWND window, const char* programName);

WINDOW_FUNCTIONS_API bool WinIsZeroSized(HWND window);

#endif // INC_WINDOW_FUNCTIONS_H 