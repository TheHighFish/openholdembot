//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#ifndef INC_TABLE_POSITIONER_H
#define INC_TABLE_POSITIONER_H

class CTablePositioner {
 public:
	CTablePositioner();
	~CTablePositioner();
 public:
  // To be called once after connection
  void CTablePositioner::PositionMyWindow();
	// To be called once per heartbeat
	void AlwaysKeepPositionIfEnabled();
  void ResizeToTargetSize();
 private:
	int _number_of_tables;
	HWND *HWNDs_of_child_windows;
	int _table_size_x;
	int _table_size_y;
	RECT _desktop_rectangle;
  RECT _table_position;
};

extern CTablePositioner *p_table_positioner;

#endif // INC_TABLE_POSITIONER_H