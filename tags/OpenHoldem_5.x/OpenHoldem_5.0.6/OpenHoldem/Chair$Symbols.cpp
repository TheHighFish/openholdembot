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

#include "stdafx.h"
#include "Chair$Symbols.h"

#include "CScraper.h"
#include "MagicNumbers.h"
#include "..\CTablemap\CTablemap.h"
#include "CTableState.h"

double Chair$(const char *name)
{
	for (int i=0; i<p_tablemap->nchairs(); i++)
	{
		if (p_table_state->_players[i]._name.Find(&name[0]) != k_not_found)
			return i;
	}
	return k_undefined;
}

double Chairbit$(const char *name)
{
	int chair = Chair$(name);
	int bits  = 1 << chair;
	return bits;
}