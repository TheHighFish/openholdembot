//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Source code:           https://github.com/OpenHoldem/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#include "StdAfx.h"
#include "CTablePositioner.h"

#include "CAutoConnector.h"
#include "CPreferences.h"
#include "CSessionCounter.h"
#include "CSharedMem.h"
#include "../CTablemap/CTableMapAccess.h"
#include "MagicNumbers.h"
#include "WinDef.h"
#include "..\WindowFunctionsDLL\window_functions.h"
#include "Winuser.h"





void MoveWindow(HWND window, int x, int y) {
  RECT desktop_rectangle;
  SystemParametersInfo(SPI_GETWORKAREA, NULL, &desktop_rectangle, NULL);
  write_log(preferences.debug_table_positioner(), "[CTablePositioner] MoveWindowToItsPosition()\n");
  // Consistancy checks
  if (x < 0
    || x >= desktop_rectangle.right
    || y < 0
    || y >= desktop_rectangle.bottom) {
    // Values out of sane range
    write_log(preferences.debug_table_positioner(), "[CTablePositioner] Values out of sane range; probably not yet initialized\n");
    write_log(preferences.debug_table_positioner(), "[CTablePositioner] _new_left_x   = %i\n", x);
    write_log(preferences.debug_table_positioner(), "[CTablePositioner] _new_top_y    = %i\n", y);
    return;
  }
  RECT current_position;
  GetWindowRect(p_autoconnector->attached_hwnd(), &current_position);
  int width = current_position.right - current_position.left;
  int height = current_position.bottom - current_position.top;
  const bool kRedrawWindow = true;
  MoveWindow(p_autoconnector->attached_hwnd(), x, y, width, height, kRedrawWindow);
}

// To be called once per heartbeat
void CTablePositioner::AlwaysKeepPositionIfEnabled() {
  if (!preferences.table_positioner_always_keep_position()
    || (p_autoconnector->attached_hwnd() == NULL)) {
    write_log(preferences.debug_table_positioner(), "[CTablePositioner] AlwaysKeepPositionIfEnabled() disabled or not connected\n");
    return;
  }
  RECT current_position;
  GetWindowRect(p_autoconnector->attached_hwnd(), &current_position);
  if ((current_position.left == _table_position.left)
    && (current_position.top == _table_position.top)) {
    write_log(preferences.debug_table_positioner(), "[CTablePositioner] AlwaysKeepPositionIfEnabled() position is good\n");
  }
  else {
    write_log(preferences.debug_table_positioner(), "[CTablePositioner] AlwaysKeepPositionIfEnabled() restoring old position\n");
    //!!!!!MoveWindowToItsPosition();
  }
}

void CTablePositioner::ResizeToTargetSize() {
  int width;
  int height;
  p_tablemap_access->GetClientSize("targetsize", &width, &height);
  if (width <= 0 || height <= 0) {
    write_log(preferences.debug_table_positioner(), "[CTablePositioner] target size <= 0\n");
    return;
  }
  ResizeToClientSize(p_autoconnector->attached_hwnd(), width, height);
}

void MoveWindowToTopLeft(HWND window) {
  write_log(preferences.debug_table_positioner(), "[CTablePositioner] MoveToTopLeft()\n");
  MoveWindow(window, 0, 0);
}

void CascadeWindow(HWND window, int cascade_position) {
  write_log(preferences.debug_table_positioner(), "[CTablePositioner] PositionMyWindow() Cascading window %i to position %i\n",
    window, cascade_position);
  const int kCascadedDeltaX = 30;
  const int kCascadedDeltaY = 20;
  // We don't use the position (0, 0) because it is reserved for the lobby
  int x = (cascade_position + 1) * kCascadedDeltaX;
  int y = (cascade_position + 1) * kCascadedDeltaY;
  MoveWindow(window, x, y);
}

void GetTableSize(HWND window, int *width, int* height) {
  if (window == NULL) {
    return;
  }
  if (!IsWindow(window)) {
    return;
  }
  RECT window_rect;
  GetWindowRect(window, &window_rect);
  *width = window_rect.right - window_rect.left;
  *height = window_rect.bottom - window_rect.top;
}





CTablePositioner *p_table_positioner = NULL;

CTablePositioner::CTablePositioner() {
  SystemParametersInfo(SPI_GETWORKAREA, NULL, &_desktop_rectangle, NULL);
}

CTablePositioner::~CTablePositioner()
{}

// To be called once after connection
void CTablePositioner::PositionMyWindow() {		
	// Build list of poker tables (child windows)
	// Use the shared memory (auto-connector) for that. 
	HWNDs_of_child_windows = p_sharedmem->GetDenseListOfConnectedPokerWindows();
	_number_of_tables = p_sharedmem->SizeOfDenseListOfAttachedPokerWindows();
  GetTableSize(p_autoconnector->attached_hwnd(),
    &_table_size_x, &_table_size_y);
  if (_number_of_tables <= 0)	{
    write_log(preferences.debug_table_positioner(), "[CTablePositioner] PositionMyWindow() No connected tables. going to return.\n");
		// Do nothing if there are 0 tables connected.
		// Actually an empty list of tables consists of only NULLs,
		// but if MicroSofts table-arranging functions see a NULL
		// they will arrange all windows at the desktop.
		// That's not what we want.
		return;
	}
  if (p_tablemap->islobby()) {
    write_log(preferences.debug_table_positioner(), "[CTablePositioner] PositionMyWindow() Going to handle the lobby...\n", _number_of_tables);
    MoveWindowToTopLeft(p_autoconnector->attached_hwnd());
  }	else if (preferences.table_positioner_options() == k_position_tables_tiled) {
		write_log(preferences.debug_table_positioner(), "[CTablePositioner] PositionMyWindow() Going to tile %d windows...\n", _number_of_tables);
		// Tiling windows: http://msdn.microsoft.com/en-us/library/windows/desktop/ms633554(v=vs.85).aspx
		// Unfortunatelly this fucntion had 2 disadvantages:
		//   * it allows small overlaps
		//   * it resizes all full-screen-windows back to default
    // but this seems to be acceptable
    // http://www.maxinmontreal.com/forums/viewtopic.php?f=111&t=18399
		TileWindows(
			NULL,				// Parent; NULL = whole desktop
			MDITILE_HORIZONTAL,	// How; either MDITILE_HORIZONTAL or MDITILE_VERTICAL
			NULL,				// Target area; NULL = parent window, here desktop
			_number_of_tables,
			HWNDs_of_child_windows);
	}	else if (preferences.table_positioner_options() == k_position_tables_cascaded) {
		write_log(preferences.debug_table_positioner(), "[CTablePositioner] PositionMyWindow() Going to cascade %d windows...\n", _number_of_tables);
		// Cascading windows: http://msdn.microsoft.com/en-us/library/windows/desktop/ms632674(v=vs.85).aspx
		/*CascadeWindows(
			NULL,				// Parent; NULL = whole desktop
			NULL,				// How; NULL means: order specified in the lpKids array
			NULL,				// Target area; NULL = parent window, here desktop
			_number_of_tables,
			HWNDs_of_child_windows);*/
    CascadeWindow(p_autoconnector->attached_hwnd(), p_sessioncounter->session_id());
	}	else {
		// preferences.table_positioner_options() == k_position_tables_never
		write_log(preferences.debug_table_positioner(), "[CTablePositioner] PositionMyWindow() Not doing anything because of preferences.\n");
	}
}

