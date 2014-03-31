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

#ifndef INC_TABLE_POSITIONER_H
#define INC_TABLE_POSITIONER_H

extern class CTablePositioner
{
public:
	CTablePositioner();
	~CTablePositioner();
public:
	// To be called once after connection
	void PositionMyWindow();
	// To be called once per heartbeat
	void AlwaysKeepPositionIfEnabled();
private:
	void PositionMyWindow(HWND *list_of_tables);
	bool TryRightSideOfTable(HWND HWND_of_potential_neighbour_table);
	bool TryBottomSideOfTable(HWND HWND_of_potential_neighbour_table);
	bool TryPosition(int left_x, int top_y);
	bool PotentialNewPositionOverlapsTable(int left_x, int top_y, HWND table_to_check_for_overlapping);
	void MoveToBottomRight();
	void MoveWindowToItsPosition();
private:
	int _number_of_tables;
	HWND *HWNDs_of_child_windows;
	int _table_size_x;
	int _table_size_y;
	int _new_left_x;
	int _new_top_y;
	RECT _desktop_rectangle;
} *p_table_positioner;

#endif // INC_TABLE_POSITIONER_H