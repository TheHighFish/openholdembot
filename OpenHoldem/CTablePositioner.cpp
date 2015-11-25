//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose:
//
//*******************************************************************************

#include "StdAfx.h"
#include "CTablePositioner.h"

#include "CAutoConnector.h"
#include "CPreferences.h"
#include "CSharedMem.h"
#include "../CTablemap/CTableMapAccess.h"
#include "MagicNumbers.h"
#include "WinDef.h"
#include "Winuser.h"

CTablePositioner *p_table_positioner = NULL;

CTablePositioner::CTablePositioner()
{}

CTablePositioner::~CTablePositioner()
{}

// To be called once after connection
void CTablePositioner::PositionMyWindow() {		
	// Build list of poker tables (child windows)
	// Use the shared memory (auto-connector) for that. 
	HWNDs_of_child_windows	= p_sharedmem->GetDenseListOfConnectedPokerWindows();
	_number_of_tables		= p_sharedmem->SizeOfDenseListOfAttachedPokerWindows();

	if (_number_of_tables <= 0)	{
		// Do nothing if there are 0 tables connected.
		// Actually an empty list of tables consists of only NULLs,
		// but if MicroSofts table-arranging functions see a NULL
		// they will arrange all windows at the desktop.
		// That's not what we want.
		return;
	}
	if (preferences.table_positioner_options() == k_position_tables_tiled) {
		write_log(preferences.debug_table_positioner(), "[CTablePositioner] PositionMyWindow() Going to tile %d windows...\n", _number_of_tables);
		PositionMyWindow(HWNDs_of_child_windows);
		// Tiling windows: http://msdn.microsoft.com/en-us/library/windows/desktop/ms633554(v=vs.85).aspx
		// Unfortunatelly this fucntion had 2 disadvantages:
		//   * it allows small overlaps
		//   * it resizes all full-screen-windows back to default
		// Therefore we use our own TileWindows-function now.
		/*
		TileWindows(
			NULL,				// Parent; NULL = whole desktop
			MDITILE_HORIZONTAL,	// How; either MDITILE_HORIZONTAL or MDITILE_VERTICAL
			NULL,				// Target area; NULL = parent window, here desktop
			_number_of_tables,
			HWNDs_of_child_windows);
		*/
	}	else if (preferences.table_positioner_options() == k_position_tables_cascaded) {
		write_log(preferences.debug_table_positioner(), "[CTablePositioner] PositionMyWindow() Going to cascade %d windows...\n", _number_of_tables);
		// Cascading windows: http://msdn.microsoft.com/en-us/library/windows/desktop/ms632674(v=vs.85).aspx
		CascadeWindows(
			NULL,				// Parent; NULL = whole desktop
			NULL,				// How; NULL means: order specified in the lpKids array
			NULL,				// Target area; NULL = parent window, here desktop
			_number_of_tables,
			HWNDs_of_child_windows);
	}	else {
		// preferences.table_positioner_options() == k_position_tables_never
		write_log(preferences.debug_table_positioner(), "[CTablePositioner] PositionMyWindow() Not doing anything because of preferences.\n");
	}
}

void CTablePositioner::PositionMyWindow(HWND *list_of_tables) {
	assert(p_autoconnector != NULL);
	if (!p_autoconnector->IsConnected()) 	{
		// This should not happen, as this function gets only called after connection
		// But it does not hurt to avoid trouble with p_autoconnector->attached_hwnd()
		return;
	}
	// Pre-calculate our table-size
	RECT current_position;
	GetWindowRect(p_autoconnector->attached_hwnd(), &current_position);
	_table_size_x = current_position.right - current_position.left;
	_table_size_y = current_position.bottom - current_position.top;
	// Get the desktop-size
	// http://stackoverflow.com/questions/8690619/how-to-get-screen-resolution-in-c
	// http://msdn.microsoft.com/en-us/library/windows/desktop/ms724947(v=vs.85).aspx
	SystemParametersInfo(SPI_GETWORKAREA,NULL,&_desktop_rectangle,NULL);
	write_log(preferences.debug_table_positioner(), "[CTablePositioner] PositionMyWindow() Connected to window %i\n", p_autoconnector->attached_hwnd());
	write_log(preferences.debug_table_positioner(), "[CTablePositioner] PositionMyWindow() Table size: %ix%i\n", _table_size_x, _table_size_y);
	write_log(preferences.debug_table_positioner(), "[CTablePositioner] PositionMyWindow() Desktop size: %ix%i\n", _desktop_rectangle.right, _desktop_rectangle.bottom); 
  if (p_tablemap->islobby()) {
	  assert(_number_of_tables == 1);	
		// No other tables at the moment, 
		// so we don't really have to care about overlaps.
		// Moving this table to the top-left (0, 0) however,
		// as the first "table" usually is the lobby
		// and mouse-movements at the lobby are more reliable
		// if relative lobby-coordinates are equal to absolute screen-coordinates
		// and we don't have to care if the right window is active.
		MoveToTopLeft();
		return;
	}
	// Thereafter always try the bottom-right-position first
	// and then try to dock at the left or top of known windows.
	// Main advantage: we have to search in 2 direction only
	// if the desktop gets empty again.
	if (TryBottomRightPosition())	{
		return;
	}
	for (int i=0; i<_number_of_tables; i++)	{
		HWND HWND_of_potential_neighbour_table = list_of_tables[i];
		write_log(preferences.debug_table_positioner(), "[CTablePositioner] PositionMyWindow() Trying neighbour-table %i\n", HWND_of_potential_neighbour_table);
		if (TryLeftSideOfTable(HWND_of_potential_neighbour_table)) {
			return;
		}
		if (TryTopSideOfTable(HWND_of_potential_neighbour_table))	{
			return;
		}
	}
	// If we couldn't position a table we move it to top-left
	// and hope that it occludes only the lobby 
	// and not multiple tables in the middle of the screen
	MoveToTopLeft();
}

bool CTablePositioner::TryLeftSideOfTable(HWND HWND_of_potential_neighbour_table) {
	RECT position_of_potential_neigbour_table;
	GetWindowRect(HWND_of_potential_neighbour_table, &position_of_potential_neigbour_table);
	write_log(preferences.debug_table_positioner(), "[CTablePositioner] TryLeftSideOfTable() Neighbours position: %i, %i, %i, %i\n", 
		position_of_potential_neigbour_table.left, position_of_potential_neigbour_table.top,
		position_of_potential_neigbour_table.right, position_of_potential_neigbour_table.bottom);
	write_log(preferences.debug_table_positioner(), "[CTablePositioner] TryLeftSideOfTable() Trying right side of neighbour\n");
	int my_left_x = position_of_potential_neigbour_table.left - _table_size_x;
	int my_top_y  = position_of_potential_neigbour_table.top;
	return TryPosition(my_left_x, my_top_y);
}

bool CTablePositioner::TryTopSideOfTable(HWND HWND_of_potential_neighbour_table) {
	RECT position_of_potential_neigbour_table;
	GetWindowRect(HWND_of_potential_neighbour_table, &position_of_potential_neigbour_table);
	write_log(preferences.debug_table_positioner(), "[CTablePositioner] TryTopSideOfTable() Trying top side of neighbour\n");
	int my_left_x = position_of_potential_neigbour_table.left;
	int my_top_y  = position_of_potential_neigbour_table.top - _table_size_y;
	return TryPosition(my_left_x, my_top_y);
}

bool CTablePositioner::TryBottomRightPosition() {
	int left_x = _desktop_rectangle.right  - _table_size_x + 1;
	int top_y  = _desktop_rectangle.bottom - _table_size_y + 1;
	write_log(preferences.debug_table_positioner(), "[CTablePositioner] TryBottomRightPosition()\n");
	return TryPosition(left_x, top_y);
}

bool CTablePositioner::TryPosition(int left_x, int top_y) {
	write_log(preferences.debug_table_positioner(), "[CTablePositioner] TryPosition() Trying position %i, %i\n",
		left_x, top_y);
  int right_x = left_x; //!!!
  int bottom_y = top_y;
  if (p_sharedmem->OverlapsAnyTable(left_x, top_y, right_x, bottom_y)) {
  	write_log(preferences.debug_table_positioner(), "[CTablePositioner] TryPosition() Candidate-position overlaps other table\n");
		return false;
	}
	// We have found a good position that does not overlap any other table
	// Move our connected table over there.
	_new_left_x = left_x;
	_new_top_y  = top_y;
	write_log(preferences.debug_table_positioner(), "[CTablePositioner] TryPosition() Found a free slot\n");
	write_log(preferences.debug_table_positioner(), "[CTablePositioner] TryPosition() Going to move the table to %i, %i\n",
		_new_left_x, _new_top_y);
	MoveWindowToItsPosition();
	return true;
}

void CTablePositioner::MoveToTopLeft() {
	write_log(preferences.debug_table_positioner(), "[CTablePositioner] MoveToTopLeft()\n");
	_new_left_x = 0;
	_new_top_y  = 0;
	MoveWindowToItsPosition();
}

// Precondition: position and size defined
void CTablePositioner::MoveWindowToItsPosition() {
	write_log(preferences.debug_table_positioner(), "[CTablePositioner] MoveWindowToItsPosition()\n");
  // Consistancy checks
	if (_new_left_x      < 0 || _new_left_x   > _desktop_rectangle.right
	  	|| _new_top_y    < 0 || _new_top_y    > _desktop_rectangle.bottom
		  || _table_size_x < 1 || _table_size_x > _desktop_rectangle.right
		  || _table_size_y < 1 || _table_size_y > _desktop_rectangle.bottom) {
		// Values out of sane range
		write_log(preferences.debug_table_positioner(), "[CTablePositioner] Values out of sane range; probably not yet initialized\n");
		write_log(preferences.debug_table_positioner(), "[CTablePositioner] _new_left_x   = %i\n", _new_left_x);
		write_log(preferences.debug_table_positioner(), "[CTablePositioner] _new_top_y    = %i\n", _new_top_y);
		write_log(preferences.debug_table_positioner(), "[CTablePositioner] _table_size_x = %i\n", _table_size_x);
		write_log(preferences.debug_table_positioner(), "[CTablePositioner] _table_size_y = %i\n", _table_size_y);
		return;
	}
  MoveWindow(p_autoconnector->attached_hwnd(), 
		_new_left_x, _new_top_y, 
		_table_size_x, _table_size_y, 
		true);	// true = Redraw the table.
  // Update shared mem
  int right = _new_left_x + _table_size_x - 1;
  int bottom = _new_top_y + _table_size_y - 1;
  p_sharedmem->StoreTablePosition(_new_left_x, _new_top_y, 
    right, bottom);
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
	if ((current_position.left == _new_left_x)
		  && (current_position.top == _new_top_y)) {
		write_log(preferences.debug_table_positioner(), "[CTablePositioner] AlwaysKeepPositionIfEnabled() position is good\n");
	}	else {
		write_log(preferences.debug_table_positioner(), "[CTablePositioner] AlwaysKeepPositionIfEnabled() restoring old position\n");
		MoveWindowToItsPosition();
	}
}

void CTablePositioner::ResizeToTargetSize() {
  int width;
  int height;
  p_tablemap_access->SetClientSize("targetsize", &width, &height);
  if (width <= 0 || height <= 0) {
    write_log(preferences.debug_table_positioner(), "[CTablePositioner] target size <= 0\n");
    return;
  }
  ResizeToClientSize(width, height);
}

void CTablePositioner::ResizeToClientSize(int new_width, int new_height) {
  assert(new_width > 0);
  assert(new_height > 0);
  RECT old_client_size;
  GetClientRect(p_autoconnector->attached_hwnd(), &old_client_size);
  int old_width  = old_client_size.right - old_client_size.left;
  int old_height = old_client_size.bottom - old_client_size.top;
  write_log(preferences.debug_table_positioner(), "[CTablePositioner] current client size: %i, %i\n",
    old_width, old_height);
  write_log(preferences.debug_table_positioner(), "[CTablePositioner] target client size: %i, %i\n",
    new_width, new_height);
  if (old_width == new_width && old_height == new_height) return;
  RECT old_position;
  GetWindowRect(p_autoconnector->attached_hwnd(), &old_position);
  int new_total_width = old_position.right - old_position.left
    + new_width- old_width;
  int new_total_height = old_position.bottom - old_position.top
    + new_height - old_height;
  ResizeToTotalSize(new_total_width, new_total_height);
}

void CTablePositioner::ResizeToTotalSize(int new_width, int new_height) {
  write_log(preferences.debug_table_positioner(), 
    "[CTablePositioner] Resizing window to total size %i, %i. Keeping old position\n",
    new_width, new_height);
  RECT old_position;
  GetWindowRect(p_autoconnector->attached_hwnd(), &old_position);
  MoveWindow(p_autoconnector->attached_hwnd(), 
    old_position.left, old_position.top,
    new_width, new_height,
		true);	// true = Redraw the table.
  // Update shared mem
  int right = old_position.left + new_width - 1;
  int bottom = old_position.top * new_height - 1;
  p_sharedmem->StoreTablePosition(old_position.left, old_position.top,
    right, bottom);
}