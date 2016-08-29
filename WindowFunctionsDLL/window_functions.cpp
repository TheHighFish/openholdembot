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

// Needs to be defined here, before #include "window_functions.h"
// to generate proper export- and inport-definitions
#define WINDOW_FUNCTIONS_EXPORTS

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#include <assert.h>
#include <math.h>
#include <windows.h>
#include "window_functions.h"
#include "..\OpenHoldem\MagicNumbers.h"
#include "Psapi.h"

RECT desktop_dimensions;
bool desktop_dimensions_calculated = false;

void MinimizeWindow(HWND window) {
  // http://msdn.microsoft.com/en-us/library/windows/desktop/ms633548%28v=vs.85%29.aspx
  ShowWindow(window, SW_MINIMIZE);
}

bool WinCalculateDesktopDimensions() {
  if (!GetWindowRect(GetDesktopWindow(), &desktop_dimensions)) {
    return false;
  }
  desktop_dimensions_calculated = true;
  return true;
}

bool WinIsBring(HWND window) {
  return WinBelongsToExecutable(window, "bring.exe");
}

bool WinIsDesktop(HWND window) {
  // http://msdn.microsoft.com/en-us/library/windows/desktop/ms633504%28v=vs.85%29.aspx
  return (GetDesktopWindow() == window || GetShellWindow() == window);
}

bool WinIsMaximized(HWND window) {
  return ::IsZoomed(window);
}

bool WinIsMinimized(HWND window) {
  return ::IsIconic(window);
}

bool WinIsOutOfScreen(HWND window) {
  if (!desktop_dimensions_calculated) {
    WinCalculateDesktopDimensions();
  }
  RECT rect;
  if (!GetWindowRect(window, &rect)) {
    return false;
  }
  return((rect.right < 0)
    || (rect.bottom < 0)
    || (rect.top > desktop_dimensions.bottom)
    || (rect.left > desktop_dimensions.right));
}

bool WinIsTaskbar(HWND window) {
  // Good way to find and detect the taskbar.
  // This should work with Windows XP and Windows 7
  HWND hShellWnd = ::FindWindow("Shell_TrayWnd", NULL);
  return window == hShellWnd;
}

bool WinIsProgramManager(HWND window) {
  return WinBelongsToExecutable(window, "progman.exe");
}

bool WinIsTaskManager(HWND window) {
  return WinBelongsToExecutable(window, "taskmgr.exe");
}

bool WinBelongsToExecutable(HWND window, const char* program_name) {
  if (window) {
    DWORD dwProcessId = 0;
    DWORD dwThreadId = GetWindowThreadProcessId(window, &dwProcessId);
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
      PROCESS_VM_READ, FALSE, dwProcessId);
    if (hProcess) {
      bool match = false;
      char nameProc[MAX_PATH];
      if (GetProcessImageFileName(hProcess, nameProc, MAX_PATH)) {
        match = strstr(nameProc, program_name) != NULL;
      }
      CloseHandle(hProcess);
      return match;
    }
  }
  return false;
}

bool WinIsZeroSized(HWND window) {
  RECT rect;
  if (!GetWindowRect(window, &rect)) {
    return false;
  }
  return ((rect.right <= rect.left)
    || (rect.bottom <= rect.top));
}

int CalculateTotalWidthForClientWidth(HWND window, int desired_client_width) {
  assert(desired_client_width > 0);
  RECT old_client_size;
  GetClientRect(window, &old_client_size);
  int old_client_width = old_client_size.right - old_client_size.left;
  RECT old_position;
  GetWindowRect(window, &old_position);
  int new_total_width = old_position.right - old_position.left
    + desired_client_width - old_client_width;
  return new_total_width;
}

int CalculateTotalHeightForClientHeight(HWND window, int desired_client_height) {
  assert(desired_client_height > 0);
  RECT old_client_size;
  GetClientRect(window, &old_client_size);
  int old_client_height = old_client_size.bottom - old_client_size.top;
  RECT old_position;
  GetWindowRect(window, &old_position);
  int new_total_height = old_position.bottom - old_position.top
    + desired_client_height - old_client_height;
  return new_total_height;
}

void ResizeToClientSize(HWND window, int new_width, int new_height) {
  assert(new_width > 0);
  assert(new_height > 0);
  assert(window != NULL);
  int new_total_width = CalculateTotalWidthForClientWidth(
    window, new_width);
  int new_total_height = CalculateTotalHeightForClientHeight(
    window, new_height);
  ResizeToTotalSize(window, new_total_width, new_total_height);
}

void ResizeToTotalSize(HWND window, int new_width, int new_height) {
  assert(new_width > 0);
  assert(new_height > 0);
  assert(window != NULL);
  RECT old_position;
  GetWindowRect(window, &old_position);
  if (old_position.left < 0) {
    old_position.left = 0;
  }
  if (old_position.top < 0) {
    old_position.top = 0;
  }
  MoveWindow(window,
    old_position.left, old_position.top,
    new_width, new_height,
    true);	// true = Redraw the table.
            // Update shared mem !!!!
}

void WinGetTitle(HWND window, char *title) {
  GetWindowText(window, title, MAX_WINDOW_TITLE);
}