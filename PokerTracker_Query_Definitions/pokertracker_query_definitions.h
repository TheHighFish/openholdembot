#pragma once
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

#ifdef POKERTRACKER_DLL_EXPORTS
#define POKERTRACKER_DLL_API extern "C" __declspec(dllexport)
#else
#define POKERTRACKER_DLL_API extern "C" __declspec(dllimport)
#endif

#include "atlstr.h"

POKERTRACKER_DLL_API	int		  PT_DLL_GetNumberOfStats();
POKERTRACKER_DLL_API	CString PT_DLL_GetDescription(int stats_index);
POKERTRACKER_DLL_API	CString PT_DLL_GetBasicSymbolNameWithoutPTPrefix(int stats_index);
POKERTRACKER_DLL_API	bool	  PT_DLL_IsBasicStat(int stats_index);
POKERTRACKER_DLL_API	bool	  PT_DLL_IsPositionalPreflopStat(int stats_index);
POKERTRACKER_DLL_API	bool	  PT_DLL_IsAdvancedStat(int stats_index);
POKERTRACKER_DLL_API	double	PT_DLL_GetStat(CString symbol_name, int chair);
POKERTRACKER_DLL_API	void	  PT_DLL_SetStat(int stats_index, int chair, double value);
POKERTRACKER_DLL_API	bool	  PT_DLL_IsValidSymbol(CString symbol_name);
POKERTRACKER_DLL_API	void	  PT_DLL_ClearPlayerStats(int chair);
POKERTRACKER_DLL_API	void	  PT_DLL_ClearAllPlayerStats();
POKERTRACKER_DLL_API	CString PT_DLL_GetQuery(int stats_index, 
												bool isomaha, bool istournament,
												int site_id, CString player_name,
                        double big_blind);