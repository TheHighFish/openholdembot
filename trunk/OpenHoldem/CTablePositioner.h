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
	void PositionMyWindow();
} *p_table_positioner;

#endif // INC_TABLE_POSITIONER_H