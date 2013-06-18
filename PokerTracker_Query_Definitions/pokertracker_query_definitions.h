#ifndef INC_POKERTRQCKERQUERYDEFINITIONS_H
#define INC_POKERTRQCKERQUERYDEFINITION_H

#ifdef POKERTRACKER_DLL_EXPORTS
#define POKERTRACKER_DLL_API extern "C" __declspec(dllexport)
#else
#define POKERTRACKER_DLL_API extern "C" __declspec(dllimport)
#endif

#include "atlstr.h"

POKERTRACKER_DLL_API	int		PT_DLL_GetNumberOfStats();
POKERTRACKER_DLL_API	CString PT_DLL_GetQueryDefinition(int stats_index, bool isomaha, bool istournament);
POKERTRACKER_DLL_API	CString PT_DLL_GetDescription(int stats_index);
POKERTRACKER_DLL_API	CString PT_DLL_GetBasicSymbolNameWithoutPTPrefix(int stats_index);
POKERTRACKER_DLL_API	bool	PT_DLL_IsBasicStat(int stats_index);
POKERTRACKER_DLL_API	bool	PT_DLL_IsPositionalPreflopStat(int stats_index);
POKERTRACKER_DLL_API	bool	PT_DLL_IsAdvancedStat(int stats_index);
POKERTRACKER_DLL_API	double	PT_DLL_GetStat(CString symbol_without_prefix, int chair);
POKERTRACKER_DLL_API	void	PT_DLL_SetStat(int stats_index, int chair, double value);
POKERTRACKER_DLL_API	void	PT_DLL_ClearPlayerStats(int chair);
POKERTRACKER_DLL_API	void	PT_DLL_ClearAllPlayerStats();

#endif INC_POKERTRQCKERQUERYDEFINITION_H