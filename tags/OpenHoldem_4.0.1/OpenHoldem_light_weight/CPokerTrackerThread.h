#ifndef INC_CPOKERTRACKERTHREAD_H
#define INC_CPOKERTRACKERTHREAD_H

#include "libpq-fe.h"
#include <map>
#include "MagicNumbers.h"
#include "PokerTracker_Queries_Version_3.h"

#define k_max_length_of_playername 30
#define k_min_chair_number 0
#define k_max_chair_number 9
#define k_advanced_stat_update_every 5
#define k_min_hands_slower_updates_ring    1000
#define k_min_hands_slower_updates_tourney 1000


extern class CPokerTrackerLookup
{
public:
	// public functions
	CPokerTrackerLookup();
	~CPokerTrackerLookup();
	const int GetSiteId();

private:
	// private functions and variables - not available via accessors or mutators
	std::map<CString, int>	_pt3_siteid;
} pt_lookup;


extern class CPokerTrackerThread 
{
public:
	// public functions
						CPokerTrackerThread();
						~CPokerTrackerThread();
	void				StartThread();
	void				StopThread();
	const double		ProcessQuery(const char *s);

private:
	// private functions and variables - not available via accessors or mutators
	static void			GetStatsForChair(LPVOID pParam, int chair, int sleepTime);
	static UINT			PokertrackerThreadFunction(LPVOID pParam);
	static int			LightSleep(int sleepTime, CPokerTrackerThread * pParent);
	void				SetStatTypes();
	void				SetStatGroups();
	void				SetRingStatsState(bool enabled);
	void				SetTourneyStatsState(bool enabled);
	void				Connect(void);
	void				Disconnect(void);
	bool				CheckName(const int m_chair, bool &nameChange);
	double				GetStat(const int m_chair, const PT_Stats stat);	
	double				UpdateStat(const int m_chair, const int stat);
	void				ClearAllStats(void);
	void				ClearSeatStats(int m_chair, bool clearNameAndFound = true);
	bool				QueryName(const char * query_name, const char * scraped_name, char * best_name);
	bool				FindName(const char *scraped_name, char *best_name);
	int					GetUpdateType(int chair);
	void				WarnAboutInvalidPTSymbol(CString s);
	int					GetStatGroup(int stat);
	int					SkipUpdateCondition(int stat, int chair);
	int					SkipUpdateForChair(int chair, char* reason);
	void				RecalcSkippedUpdates(int chr);
	void				SetHandsStat();
	void				ReportSeatChanges(int chair);
	void				ReportUpdateComplete(int updatedCount, int chair);
	bool				StatEnabled(int stat){return _m_enabled_stats[stat];}
	void				SetPlayerName(int chr, bool found, const char* pt_name, const char* scraped_name);
	int					GetSkippedUpdates(int chr){return _player_stats[chr].skipped_updates;}
	bool				IsFound(int chair);
	const char*         GetPlayerScrapedName(int chair){return _player_stats[chair].scraped_name;}

	CString				_conn_str;
	bool				_connected;
	PGconn *			_pgconn;

	struct SPlayerStats 
	{
		char			scraped_name[k_max_length_of_playername];
		char			pt_name[k_max_length_of_playername];
		bool			found;
		double			stat[k_number_of_pokertracker_stats];
		int				t_elapsed[k_number_of_pokertracker_stats];
		int				skipped_updates;
	} _player_stats[k_max_number_of_players];
	
	bool                _m_enabled_stats[k_number_of_pokertracker_stats];
	PT_StatTypes		_m_stat_type[k_number_of_pokertracker_stats];
	PT_Stats			_m_handsStats;
	int					_m_min_hands_for_slower_update;

	int					_m_statGroup[pt_max];
	HANDLE				_m_stop_thread;
	HANDLE				_m_wait_thread;

	CWinThread			*_pt_thread;

} *p_pokertracker_thread;

#endif //INC_CPOKERTRACKERTHREAD_H