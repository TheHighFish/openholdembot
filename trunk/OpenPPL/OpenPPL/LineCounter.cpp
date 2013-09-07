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


#include "Linecounter.h"

long int line_counter = 1;

void IncLineCounter()
{
	line_counter++;
}

int GetLineCount()
{
	return line_counter;
}