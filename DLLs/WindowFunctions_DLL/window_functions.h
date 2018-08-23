//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
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

#include <windows.h>

WINDOW_FUNCTIONS_API int CalculateTotalHeightForClientHeight(HWND window, int desired_client_height);
WINDOW_FUNCTIONS_API int CalculateTotalWidthForClientWidth(HWND window, int desired_client_width);
WINDOW_FUNCTIONS_API void CascadeSingleWindow(HWND window, int cascade_position);
WINDOW_FUNCTIONS_API void GetWindowSize(HWND window, int *width, int* height);
WINDOW_FUNCTIONS_API void MinimizeWindow(HWND window);
WINDOW_FUNCTIONS_API void MoveWindow(HWND window, int x, int y);
WINDOW_FUNCTIONS_API void MoveWindowToTopLeft(HWND window);
WINDOW_FUNCTIONS_API void ResizeToClientSize(HWND window, int new_width, int new_height);
WINDOW_FUNCTIONS_API void ResizeToTotalSize(HWND window, int new_width, int new_height);
WINDOW_FUNCTIONS_API void TileSingleWindow(HWND this_window, HWND *null_terminated_list_of_other_windows);
WINDOW_FUNCTIONS_API bool WinBelongsToExecutable(HWND window, const char* program_name);
WINDOW_FUNCTIONS_API void WinGetTitle(HWND window, char *title);
WINDOW_FUNCTIONS_API bool WinIsDesktop(HWND window);
WINDOW_FUNCTIONS_API bool WinIsMaximized(HWND window);
WINDOW_FUNCTIONS_API bool WinIsMinimized(HWND window);
WINDOW_FUNCTIONS_API bool WinIsBring(HWND window);
WINDOW_FUNCTIONS_API bool WinIsOutOfScreen(HWND window);
WINDOW_FUNCTIONS_API bool WinIsTaskbar(HWND window);
WINDOW_FUNCTIONS_API bool WinIsProgramManager(HWND window);
WINDOW_FUNCTIONS_API bool WinIsTaskManager(HWND window);
WINDOW_FUNCTIONS_API bool WinIsZeroSized(HWND window);

// Errors and warnings.
// Get displayed (or not) depending on settings
WINDOW_FUNCTIONS_API void MessageBox_Error_Warning(const char* Message, const char*  Title = "Error");
// Interactive messages.
// Get always displayed, even if they are not critical.
// They are either a result of the users action 
// or we need request some direct input.
WINDOW_FUNCTIONS_API int MessageBox_Interactive(const char*  Message, const char*  Title, int Flags);
// MessageBox for the msgbox$MESSAGE-command of OH-script
WINDOW_FUNCTIONS_API void MessageBox_OH_Script_Messages(const char*  message);

#endif // INC_WINDOW_FUNCTIONS_H 