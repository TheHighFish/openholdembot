#include "stdafx.h"
#include "CPokerTrackerStatsContainer.h"

#include <map>
#include "..\MagicNumbers.h"

CPokerTrackerStatsContainer *p_pokertracker_stats_container = NULL;

typedef double TStatsArray[k_max_number_of_players];

struct SSingleTableStat
{
	// single stat for all players at the table
	TStatsArray p_stats_array;
};


// Associative array to lookup the stats.
typedef std::map<CString, SSingleTableStat> TStatsMap;
TStatsMap StatsMap;


CPokerTrackerStatsContainer::CPokerTrackerStatsContainer()
{
	ResetAllStats();
}

CPokerTrackerStatsContainer::~CPokerTrackerStatsContainer()
{
	ResetAllStats();
}

void CPokerTrackerStatsContainer::CreateStat(CString stats_name)
{
	assert(stats_name.GetLength() > 0);
	TStatsArray *p_new_stats_array = (TStatsArray*) new(TStatsArray);
	SSingleTableStat *p_new_single_table_stat = new(SSingleTableStat);
	StatsMap.insert(std::pair<CString, SSingleTableStat>(stats_name, 
		*p_new_single_table_stat));
}

void CPokerTrackerStatsContainer::ResetAllStats()
{
	for (int i=k_min_chair_number; i<=k_min_chair_number; i++)
	{
		ResetStatsForPlayer(i);
	}
}

void CPokerTrackerStatsContainer::ResetStatsForPlayer(int chair)
{
	assert(chair >= k_min_chair_number);
	assert(chair <= k_max_chair_number);
}

void CPokerTrackerStatsContainer::SetStat(CString stats_name, int chair, double new_value)
{
	assert(chair >= k_min_chair_number);
	assert(chair <= k_max_chair_number);
	assert(stats_name.GetLength() > 0);

	TStatsMap::const_iterator stats_iterator = StatsMap.find(stats_name);
	// "Not found" should not happen, 
	// because only stats can be set, that have previously been created. 
	assert(stats_iterator != StatsMap.end());
	TStatsArray *stats_array = (TStatsArray*)(stats_iterator->second.p_stats_array);
	*stats_array[chair] = new_value;
}

double CPokerTrackerStatsContainer::GetStat(CString stats_name, int chair)
{
	assert(chair >= k_min_chair_number);
	assert(chair <= k_max_chair_number);
	assert(stats_name.GetLength() > 0);

	TStatsMap::const_iterator stats_iterator = StatsMap.find(stats_name);
	// "Not found" can happen (bad formula by user).
	if (stats_iterator == StatsMap.end())
	{
		WarnAboutInvalidPTSymbol(stats_name);
		return k_pokertracker_stat_undefined;
	}
	else 
	{
		return stats_iterator->second.p_stats_array[chair];
	}
}

void CPokerTrackerStatsContainer::WarnAboutInvalidPTSymbol(CString s)
{
	CString error_message = "Invalid PokerTracker-symbol: " + s;
	OH_MessageBox(error_message, "ERROR", 0);
}