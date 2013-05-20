// mousedll.cpp : Defines the entry point for the DLL application.
//

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#include <windows.h>
//!!!#include "pokertracker_query_definition.h"

#include <atlstr.h>
#include "..\OpenHoldem\NumericalFunctions.h"
#include "PokerTracker_Queries_Version_3.h"


int GetNumberOfStats()
{
	return k_number_of_pokertracker_stats;
}


// We create queries on the fly, 
// so that they are usable for both ring-games and tournaments 
const CString k_holdem_infix  = " holdem_";
const CString k_omaha_infix   = " omaha_";
const CString k_tournament_infix = " tourney_";

CString GetQueryDefinition(int index, bool isomaha, bool istournament)
{
	AssertRange(index, 0, (k_number_of_pokertracker_stats - 1));
	CString query = query_definitions[index].first_part_of_query;
	if (query_definitions[index].needs_infix_and_second_part)
	{
		if (istournament)
		{
			query += k_tournament_infix;
		}
		if (isomaha)
		{
			query += k_omaha_infix;
		}
		else
		{
			query += k_holdem_infix;
		}
		query += query_definitions[index].last_part_of_query;
	}
	return query;
}

CString GetDescription(int index)
{ 
	AssertRange(index, 0, (k_number_of_pokertracker_stats - 1));
	return query_definitions[index].description_for_editor; 
}

bool IsBasicStat(int index)
{ 
	return query_definitions[index].stat_group == pt_group_basic; 
}

bool IsPositionalPreflopStat(int index)
{ 
	return query_definitions[index].stat_group == pt_group_positional;
}

bool IsAdvancedStat(int index)
{ 
	return query_definitions[index].stat_group == pt_group_advanced; 
}
