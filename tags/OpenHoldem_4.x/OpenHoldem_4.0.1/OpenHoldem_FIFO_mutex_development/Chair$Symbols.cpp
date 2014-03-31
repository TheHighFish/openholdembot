#include "stdafx.h"
#include "Chair$Symbols.h"

#include "CScraper.h"
#include "MagicNumbers.h"
#include "..\CTablemap\CTablemap.h"

double Chair$(const char *name)
{
	for (int i=0; i<p_tablemap->nchairs(); i++)
	{
		if (p_scraper->player_name(i).Find(&name[0]) != k_not_found)
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