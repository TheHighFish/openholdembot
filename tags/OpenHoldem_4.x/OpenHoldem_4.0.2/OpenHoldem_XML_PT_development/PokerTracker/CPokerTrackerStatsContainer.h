#ifndef INC_CPOKERTRACKERSTATSCONTAINER_H
#define INC_CPOKERTRACKERSTATSCONTAINER_H

extern class CPokerTrackerStatsContainer
{
public:
	CPokerTrackerStatsContainer();
	~CPokerTrackerStatsContainer();
public:
	void CreateStat(CString stats_name);
	void ResetAllStats();
	void ResetStatsForPlayer(int chair);
	void SetStat(CString name, int chair, double new_value);
	double GetStat(CString name, int Chair);
	void WarnAboutInvalidPTSymbol(CString s);
} *p_pokertracker_stats_container;

#endif