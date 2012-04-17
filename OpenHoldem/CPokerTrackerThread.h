#ifndef INC_CPOKERTRACKERTHREAD_H
#define INC_CPOKERTRACKERTHREAD_H

#include "libpq-fe.h"
#include <map>
#include "MagicNumbers.h"
#include "PokerTracker_Queries_Version_3.h"

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
	void StartThread();
	void StopThread();
	const double ProcessQuery(const char *s);

private:
	// private functions and variables - not available via accessors or mutators
	static UINT PokertrackerThreadFunction(LPVOID pParam);
	void Connect(void);
	void Disconnect(void);
	bool CheckName (const int m_chair);
	double GetStat (const int m_chair, const PT_Stats stat);	
	double UpdateStat (const int m_chair, const int stat);
	void ClearStats(void);
	bool QueryName (const char * query_name, const char * scraped_name, char * best_name);

	CString			_conn_str;
	bool			_connected;
	PGconn *		_pgconn;

	struct SPlayerStats 
	{
		char		scraped_name[k_max_length_of_playername];
		char		pt_name[k_max_length_of_playername];
		bool		found;
		double		stat[k_number_of_pokertracker_stats];
		int			t_elapsed[k_number_of_pokertracker_stats];
	} _player_stats[k_max_number_of_players];

	HANDLE			_m_stop_thread;
	HANDLE			_m_wait_thread;

	CWinThread		*_pt_thread;

} *p_pokertracker_thread;

#endif //INC_CPOKERTRACKERTHREAD_H