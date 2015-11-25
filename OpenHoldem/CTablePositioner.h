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

#ifndef INC_TABLE_POSITIONER_H
#define INC_TABLE_POSITIONER_H

class CTablePositioner {
 public:
	CTablePositioner();
	~CTablePositioner();
 public:
	// To be called once after connection
	void PositionMyWindow();
	// To be called once per heartbeat
	void AlwaysKeepPositionIfEnabled();
  void ResizeToTargetSize();
 private:
	void PositionMyWindow(HWND *list_of_tables);
	bool TryLeftSideOfTable(HWND HWND_of_potential_neighbour_table);
	bool TryTopSideOfTable(HWND HWND_of_potential_neighbour_table);
	bool TryBottomRightPosition();
	bool TryPosition(int left_x, int top_y);
	void MoveToTopLeft();
	void MoveWindowToItsPosition();
 private:
  void ResizeToClientSize(int new_width, int new_height);
  void ResizeToTotalSize(int new_width, int new_height);
 private:
	int _number_of_tables;
	HWND *HWNDs_of_child_windows;
	int _table_size_x;
	int _table_size_y;
	int _new_left_x;
	int _new_top_y;
	RECT _desktop_rectangle;
};

extern CTablePositioner *p_table_positioner;

#endif // INC_TABLE_POSITIONER_H