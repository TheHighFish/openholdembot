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

bool isInRange(int value, int first, int last) {
  if (value < first) {
    return false;
  }
  if (value > last) {
    return false;
  }
  return true;
}


// Tiling windows: http://msdn.microsoft.com/en-us/library/windows/desktop/ms633554(v=vs.85).aspx
// Unfortunatelly this fucntion had 2 disadvantages:
//   * it allows small overlaps
//   * it resizes all full-screen-windows back to default
// but this seems to be acceptable
// http://www.maxinmontreal.com/forums/viewtopic.php?f=111&t=18399
void TileSingleWindow(HWND this_window, HWND *null_terminated_list_of_other_windows) {
  // Keep other windows at current place.
  // Find a free space for this one.
  // Move itz to top left, if not possible.
  RECT desktop_rectangle;
  SystemParametersInfo(SPI_GETWORKAREA, NULL, &desktop_rectangle, NULL);
  int width, height;
  GetWindowSize(this_window, &width, &height);
  // First search a free slot, the dumb and reliable way.
  for (int x = 0; x < desktop_rectangle.right; ++x) {
    write_log(true, "Searching (%i, y)\n", x); //!!!!!
    for (int y = 0; y < desktop_rectangle.bottom; ++y) {
      write_log(true, "Searching (%i, %i)\n", x, y); //!!!!!
      // Look for a conflict with every other window
      int right_x = x + width - 1;
      int bottom_y = y + height - 1;
      assert(null_terminated_list_of_other_windows != NULL);
      // Create a copy of the pointer, we need the original later again
      HWND* list_of_windows = null_terminated_list_of_other_windows;
      while (*list_of_windows != NULL) {
        write_log(true, "Searching (%i, %i) and window %i\n", x, y, *list_of_windows);
        if (*list_of_windows == this_window) {
          // Window to be positioned might be in list of "other" windows
          // which we get from auto-connector. Ignore it
          ++list_of_windows;
          continue;
        }
        RECT other_window_position;
        GetWindowRect(*list_of_windows, &other_window_position);
        bool x_range_overlaps = false;
        bool y_range_overlaps = false;
        if (isInRange(x, other_window_position.left, other_window_position.right)) {
          x_range_overlaps = true;
        } else if (isInRange(right_x, other_window_position.left, other_window_position.right)) {
          x_range_overlaps = true;
        }
        if (isInRange(y, other_window_position.top, other_window_position.bottom)) {
          y_range_overlaps = true;
        } else if (isInRange(bottom_y, other_window_position.top, other_window_position.bottom)) {
          y_range_overlaps = true;
        }
        if (x_range_overlaps && y_range_overlaps) {
          write_log(true, "Overlap (%i, %i) by window %i\n", x, y, *list_of_windows); //!!!!!
          goto conflict_with_any_window_continue_with_next_x_or_y;
        }
        // No conflict with this window
        // Try next one
        write_log(true, "Advancing pointer\n");
        ++list_of_windows;
      }
      // End of list reached
      // No overlap found
      // Heureka!
      MoveWindow(this_window, x, y);
      return;
// GOTO-label to jump out of inner loop on conflict
    conflict_with_any_window_continue_with_next_x_or_y:
      ; //!!!!!
    }
  }
  // Failed
  // Move to top-left (lobby-position) to reduce overlaps and damage.
  MoveWindowToTopLeft(this_window);
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

CTablePositioner *p_table_positioner = NULL;

CTablePositioner::CTablePositioner() {
  SystemParametersInfo(SPI_GETWORKAREA, NULL, &_desktop_rectangle, NULL);
}

CTablePositioner::~CTablePositioner() {
}

// To be called once after connection
void CTablePositioner::PositionMyWindow() {		
	// Build list of poker tables (child windows)
	// Use the shared memory (auto-connector) for that. 
	HWNDs_of_child_windows = p_sharedmem->GetDenseListOfConnectedPokerWindows();
	_number_of_tables = p_sharedmem->SizeOfDenseListOfAttachedPokerWindows();
  GetWindowSize(p_autoconnector->attached_hwnd(),
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
  if (p_tablemap->islobby()) { // !!!!! to do: ispopup
    write_log(preferences.debug_table_positioner(), "[CTablePositioner] PositionMyWindow() Going to handle the lobby...\n", _number_of_tables);
    MoveWindowToTopLeft(p_autoconnector->attached_hwnd());
  }	else if (preferences.table_positioner_options() == k_position_tables_tiled) {
		write_log(preferences.debug_table_positioner(), "[CTablePositioner] PositionMyWindow() Going to tile %d windows...\n", _number_of_tables);	
    TileSingleWindow(p_autoconnector->attached_hwnd(), HWNDs_of_child_windows);
	}	else if (preferences.table_positioner_options() == k_position_tables_cascaded) {
		write_log(preferences.debug_table_positioner(), "[CTablePositioner] PositionMyWindow() Going to cascade %d windows...\n", _number_of_tables);
		// Cascading windows: http://msdn.microsoft.com/en-us/library/windows/desktop/ms632674(v=vs.85).aspx
		/*CascadeWindows(!!!!!
			NULL,				// Parent; NULL = whole desktop
			NULL,				// How; NULL means: order specified in the lpKids array
			NULL,				// Target area; NULL = parent window, here desktop
			_number_of_tables,
			HWNDs_of_child_windows);*/
    CascadeSingleWindow(p_autoconnector->attached_hwnd(), p_sessioncounter->session_id());
	}	else {
		// preferences.table_positioner_options() == k_position_tables_never
		write_log(preferences.debug_table_positioner(), "[CTablePositioner] PositionMyWindow() Not doing anything because of preferences.\n");
	}
}

