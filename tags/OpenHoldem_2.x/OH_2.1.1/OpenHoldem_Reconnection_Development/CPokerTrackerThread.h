#ifndef INC_CPOKERTRACKERTHREAD_H
#define INC_CPOKERTRACKERTHREAD_H

#include "libpq-fe.h"
#include <map>

extern class CPokerTrackerLookup
{
public:
	// public functions
	CPokerTrackerLookup();
	~CPokerTrackerLookup();
	const int GetSiteId();

private:
	// private functions and variables - not available via accessors or mutators
	std::map<CString, int>	_pt2_siteid;
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
		char		scraped_name[30];
		char		pt_name[30];
		bool		found;
		double		stat[50];
		int			t_elapsed[50];
	} _player_stats[10];

	HANDLE			_m_stop_thread;
	HANDLE			_m_wait_thread;

	CWinThread		*_pt_thread;

} *p_pokertracker_thread;

// These are for PokerTracker version 2 stats
char * const stat_str[] = {
	"pt_icon", "pt_hands", "pt_pfr", "pt_aggp", "pt_aggf", "pt_aggt", "pt_aggr", 
	"pt_aggtot", "pt_aggtotnopf", "pt_floppct", "pt_turnpct", "pt_riverpct", "pt_vpip", "pt_pf_rfi",
	"pt_pf_cr", "pt_pfats",	"pt_wsdp", "pt_wssd", "pt_fbbts", "pt_fsbts",
	"ptt_icon", "ptt_hands", "ptt_pfr", "ptt_aggp", "ptt_aggf", "ptt_aggt", "ptt_aggr",
	"ptt_aggtot", "ptt_aggtotnopf", "ptt_floppct", "ptt_turnpct", "ptt_riverpct", "ptt_vpip", "ptt_pf_rfi",
	"ptt_pf_cr", "ptt_pfats", "ptt_wsdp", "ptt_wssd", "ptt_fbbts", "ptt_fsbts"
};


// Array of PokerTracker version 2 queries, needs to be in same order as stat_str enum
char * const query_str2[] = {

// RING STATISTICS
/* PT2 RING query to get icon */
"SELECT treeview_icon FROM players WHERE main_site_id=%SITEID% \
 AND screen_name = '%SCREENNAME%'",

/* PT2 RING query to get number of hands in the database */
"SELECT COUNT(*) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 RING query to get pre flop raise % */
"SELECT (case when (count(*)!=0) \
			  then ( cast(sum(pre_flop_raise_n) as real) / cast(count(*) as real) ) \
			  else (-1) \
		 end) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 RING query to get pre-flop aggression factor */
"SELECT (case when (SUM(pcall)!=0) \
			  then ( cast(SUM(praise) as real) / cast(SUM(pcall) as real) ) \
			  else (-1) \
		 end) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 RING query to get flop aggression factor */
"SELECT (case when (SUM(fcall)!=0) \
			  then ( (cast(SUM(fraise) as real) + cast(SUM(fbet) as real)) / cast(SUM(fcall) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 RING query to get turn aggression factor */
"SELECT (case when (SUM(tcall)!=0) \
			  then ( (cast(SUM(traise) as real) + cast(SUM(tbet) as real)) / cast(SUM(tcall) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 RING query to get river aggression factor */
"SELECT (case when (SUM(rcall)!=0) \
			  then ( (cast(SUM(rraise) as real) + cast(SUM(rbet) as real)) / cast(SUM(rcall) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 RING query to get total aggression factor */
"SELECT (case when (call_pct!=0) \
			  then ( (raise_pct+bet_pct)/call_pct ) \
			  else (-1) \
		 end) as result \
 FROM \
 ( \
	SELECT (case when (poss_actions!=0) then ( raise_actions/poss_actions ) else (0) end) as raise_pct, \
		   (case when (poss_actions!=0) then ( bet_actions/poss_actions ) else (0) end) as bet_pct, \
		   (case when (poss_actions!=0) then ( call_actions/poss_actions ) else (0) end) as call_pct \
	FROM \
	( \
	   SELECT cast(SUM(ppossible_actions) as real) + cast(SUM(fpossible_actions) as real) + cast(SUM(tpossible_actions) as real) + cast(SUM(rpossible_actions) as real) as poss_actions, \
			  cast(SUM(praise) as real) + cast(SUM(fraise) as real) + cast(SUM(traise) as real) + cast(SUM(rraise) as real) as raise_actions, \
			  cast(SUM(fbet) as real) + cast(SUM(tbet) as real) + cast(SUM(rbet) as real) as bet_actions, \
			  cast(SUM(pcall) as real) + cast(SUM(fcall) as real) + cast(SUM(tcall) as real) + cast(SUM(rcall) as real) as call_actions \
	   FROM   game_players, players \
	   WHERE  game_players.player_id = players.player_id AND \
			  screen_name like '%SCREENNAME%'  AND \
			  players.main_site_id=%SITEID% \
	) as foo \
 ) as bar ",

/* PT2 RING query to get total aggression factor without preflop */
"SELECT (case when (call_pct!=0) \
			  then ( (raise_pct+bet_pct)/call_pct ) \
			  else (-1) \
		 end) as result \
 FROM \
 ( \
	SELECT (case when (poss_actions!=0) then ( raise_actions/poss_actions ) else (0) end) as raise_pct, \
		   (case when (poss_actions!=0) then ( bet_actions/poss_actions ) else (0) end) as bet_pct, \
		   (case when (poss_actions!=0) then ( call_actions/poss_actions ) else (0) end) as call_pct \
	FROM \
	( \
	   SELECT cast(SUM(fpossible_actions) as real) + cast(SUM(tpossible_actions) as real) + cast(SUM(rpossible_actions) as real) as poss_actions, \
			  cast(SUM(fraise) as real) + cast(SUM(traise) as real) + cast(SUM(rraise) as real) as raise_actions, \
			  cast(SUM(fbet) as real) + cast(SUM(tbet) as real) + cast(SUM(rbet) as real) as bet_actions, \
			  cast(SUM(fcall) as real) + cast(SUM(tcall) as real) + cast(SUM(rcall) as real) as call_actions \
	   FROM   game_players, players \
	   WHERE  game_players.player_id = players.player_id AND \
			  screen_name like '%SCREENNAME%'  AND \
			  players.main_site_id=%SITEID% \
	) as foo \
 ) as bar ",

/* PT2 RING query to get percentage of flops seen */
"SELECT (case when (COUNT(*)!=0) \
			  then ( cast(SUM(saw_flop_n) as real) / cast(COUNT(*) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 RING query to get percentage of turns seen */
"SELECT (case when (COUNT(*)!=0) \
			  then ( ( cast( (SELECT COUNT(*) \
							  FROM   game_players, players \
							  WHERE  game_players.player_id = players.player_id AND \
									 screen_name like '%SCREENNAME%'  AND \
									 players.main_site_id=%SITEID% AND \
									 (when_folded IS NULL OR \
									  when_folded='After turn' OR \
									  when_folded='After river') ) as real) \
							  / cast (COUNT(*) as real) ) ) \
			  else (-1) \
		 end ) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 RING query to get percentage of rivers seen */
"SELECT (case when (COUNT(*)!=0) \
			  then ( ( cast( (SELECT COUNT(*) \
							  FROM   game_players, players \
							  WHERE  game_players.player_id = players.player_id AND \
									 screen_name like '%SCREENNAME%'  AND \
									 players.main_site_id=%SITEID% AND \
									 (when_folded IS NULL OR \
									 when_folded='After river') ) as real) \
									 / cast (COUNT(*) as real) ) ) \
			  else (-1) \
		 end ) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 RING query to get vpip */
"SELECT (case when (COUNT(*)!=0) \
			  then ( cast(SUM(vol_put_money_in_pot) as real) / cast(COUNT(*) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",
	
/* PT2 RING query to get pre-flop raise first in pct */
"SELECT (case when (COUNT(*)!=0) \
			  then ( cast(SUM(raised_first_pf) as real) / cast(COUNT(*) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 RING query to get pre-flop called raise pct */
"SELECT (case when (COUNT(*)!=0) \
			  then ( cast(SUM(limp_call_reraise_pf) as real) / cast(COUNT(*) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 RING query to get attempted steal percentage */
"SELECT (case when (count(*) !=0) \
			  then ( cast(sum(attempted_steal) as real)/cast(sum(chance_to_steal) as real)) \
			  else (-1) \
		 end) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 RING query to get "Went to SD %" */
"SELECT (case when (sum(saw_flop_n)!=0) \
			  then ( cast(sum(went_to_showdown_n) as real) / cast(sum(saw_flop_n) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 RING query to get "Won $ At SD %" */
"SELECT (case when (sum(went_to_showdown_n)!=0) \
			  then ( cast(sum( case when went_to_showdown_n = 1 then won_hand else 0 end ) as real) / cast(sum(went_to_showdown_n) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 RING query to get "Folded Big Blind to steal" */
"SELECT (case when (sum(case when steal_attempted = 1 AND big_blind_n = 1 then 1 else 0 end) !=0) \
			  then ( cast(sum( case when steal_attempted = 1 AND folded_to_steal_attempt = 1 AND big_blind_n = 1 then 1 else 0 end ) as real) / \
					 cast(sum( case when steal_attempted = 1 AND big_blind_n = 1 then 1 else 0 end) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 RING query to get "Folded Small Blind to steal" */
"SELECT (case when (sum(case when steal_attempted = 1 AND small_blind_n = 1 then 1 else 0 end) !=0) \
			  then ( cast(sum( case when steal_attempted = 1 AND folded_to_steal_attempt = 1 AND small_blind_n = 1 then 1 else 0 end ) as real) / \
					 cast(sum( case when steal_attempted = 1 AND small_blind_n = 1 then 1 else 0 end) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",




// TOURNAMENT STATISTICS
/* PT2 TOURNAMENT query to get icon */
"SELECT treeview_icon FROM players WHERE main_site_id=%SITEID% \
 AND screen_name = '%SCREENNAME%'",

/* PT2 TOURNAMENT query to get number of hands in the database */
"SELECT COUNT(*) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 TOURNAMENT query to get pre flop raise % */
"SELECT (case when (count(*)!=0) \
			  then ( cast(sum(pre_flop_raise_n) as real) / cast(count(*) as real) ) \
			  else (-1) \
		 end) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 TOURNAMENT query to get pre-flop aggression factor */
"SELECT (case when (SUM(pcall)!=0) \
			  then ( cast(SUM(praise) as real) / cast(SUM(pcall) as real) ) \
			  else (-1) \
		 end) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 TOURNAMENT query to get flop aggression factor */
"SELECT (case when (SUM(fcall)!=0) \
			  then ( (cast(SUM(fraise) as real) + cast(SUM(fbet) as real)) / cast(SUM(fcall) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 TOURNAMENT query to get turn aggression factor */
"SELECT (case when (SUM(tcall)!=0) \
			  then ( (cast(SUM(traise) as real) + cast(SUM(tbet) as real)) / cast(SUM(tcall) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 TOURNAMENT query to get river aggression factor */
"SELECT (case when (SUM(rcall)!=0) \
			  then ( (cast(SUM(rraise) as real) + cast(SUM(rbet) as real)) / cast(SUM(rcall) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 TOURNAMENT query to get total aggression factor */
"SELECT (case when (call_pct!=0) \
			  then ( (raise_pct+bet_pct)/call_pct ) \
			  else (-1) \
		 end) as result \
 FROM \
 ( \
	SELECT (case when (poss_actions!=0) then ( raise_actions/poss_actions ) else (0) end) as raise_pct, \
		   (case when (poss_actions!=0) then ( bet_actions/poss_actions ) else (0) end) as bet_pct, \
		   (case when (poss_actions!=0) then ( call_actions/poss_actions ) else (0) end) as call_pct \
	FROM \
	( \
	   SELECT cast(SUM(ppossible_actions) as real) + cast(SUM(fpossible_actions) as real) + cast(SUM(tpossible_actions) as real) + cast(SUM(rpossible_actions) as real) as poss_actions, \
			  cast(SUM(praise) as real) + cast(SUM(fraise) as real) + cast(SUM(traise) as real) + cast(SUM(rraise) as real) as raise_actions, \
			  cast(SUM(fbet) as real) + cast(SUM(tbet) as real) + cast(SUM(rbet) as real) as bet_actions, \
			  cast(SUM(pcall) as real) + cast(SUM(fcall) as real) + cast(SUM(tcall) as real) + cast(SUM(rcall) as real) as call_actions \
	   FROM   tourney_game_players, players \
	   WHERE  tourney_game_players.player_id = players.player_id AND \
			  screen_name like '%SCREENNAME%'  AND \
			  players.main_site_id=%SITEID% \
	) as foo \
 ) as bar ",

/* PT2 TOURNAMENT query to get total aggression factor without preflop */
"SELECT (case when (call_pct!=0) \
			  then ( (raise_pct+bet_pct)/call_pct ) \
			  else (-1) \
		 end) as result \
 FROM \
 ( \
	SELECT (case when (poss_actions!=0) then ( raise_actions/poss_actions ) else (0) end) as raise_pct, \
		   (case when (poss_actions!=0) then ( bet_actions/poss_actions ) else (0) end) as bet_pct, \
		   (case when (poss_actions!=0) then ( call_actions/poss_actions ) else (0) end) as call_pct \
	FROM \
	( \
	   SELECT cast(SUM(fpossible_actions) as real) + cast(SUM(tpossible_actions) as real) + cast(SUM(rpossible_actions) as real) as poss_actions, \
			  cast(SUM(fraise) as real) + cast(SUM(traise) as real) + cast(SUM(rraise) as real) as raise_actions, \
			  cast(SUM(fbet) as real) + cast(SUM(tbet) as real) + cast(SUM(rbet) as real) as bet_actions, \
			  cast(SUM(fcall) as real) + cast(SUM(tcall) as real) + cast(SUM(rcall) as real) as call_actions \
	   FROM   tourney_game_players, players \
	   WHERE  tourney_game_players.player_id = players.player_id AND \
			  screen_name like '%SCREENNAME%'  AND \
			  players.main_site_id=%SITEID% \
	) as foo \
 ) as bar ",

/* PT2 TOURNAMENT query to get percentage of flops seen */
"SELECT (case when (COUNT(*)!=0) \
			  then ( cast(SUM(saw_flop_n) as real) / cast(COUNT(*) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 TOURNAMENT query to get percentage of turns seen */
"SELECT (case when (COUNT(*)!=0) \
			  then ( ( cast( (SELECT COUNT(*) \
							  FROM   tourney_game_players, players \
							  WHERE  tourney_game_players.player_id = players.player_id AND \
									 screen_name like '%SCREENNAME%'  AND \
									 players.main_site_id=%SITEID% AND \
									 (when_folded IS NULL OR \
									  when_folded='After turn' OR \
									  when_folded='After river') ) as real) \
							  / cast (COUNT(*) as real) ) ) \
			  else (-1) \
		 end ) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 TOURNAMENT query to get percentage of rivers seen */
"SELECT (case when (COUNT(*)!=0) \
			  then ( ( cast( (SELECT COUNT(*) \
							  FROM   tourney_game_players, players \
							  WHERE  tourney_game_players.player_id = players.player_id AND \
									 screen_name like '%SCREENNAME%'  AND \
									 players.main_site_id=%SITEID% AND \
									 (when_folded IS NULL OR \
									 when_folded='After river') ) as real) \
									 / cast (COUNT(*) as real) ) ) \
			  else (-1) \
		 end ) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 TOURNAMENT query to get vpip */
"SELECT (case when (COUNT(*)!=0) \
			  then ( cast(SUM(vol_put_money_in_pot) as real) / cast(COUNT(*) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",
	
/* PT2 TOURNAMENT query to get pre-flop raise first in pct */
"SELECT (case when (COUNT(*)!=0) \
			  then ( cast(SUM(raised_first_pf) as real) / cast(COUNT(*) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 TOURNAMENT query to get pre-flop called raise pct */
"SELECT (case when (COUNT(*)!=0) \
			  then ( cast(SUM(limp_call_reraise_pf) as real) / cast(COUNT(*) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 TOURNAMENT query to get attempted steal percentage */
"SELECT (case when (count(*) !=0) \
			  then ( cast(sum(attempted_steal) as real)/cast(sum(chance_to_steal) as real)) \
			  else (-1) \
		 end) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 TOURNAMENT query to get "Went to SD %" */
"SELECT (case when (sum(saw_flop_n)!=0) \
			  then ( cast(sum(went_to_showdown_n) as real) / cast(sum(saw_flop_n) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 TOURNAMENT query to get "Won $ At SD %" */
"SELECT (case when (sum(went_to_showdown_n)!=0) \
			  then ( cast(sum( case when went_to_showdown_n = 1 then won_hand else 0 end ) as real) / cast(sum(went_to_showdown_n) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 TOURNAMENT query to get "Folded Big Blind to steal" */
"SELECT (case when (sum(case when steal_attempted = 1 AND big_blind_n = 1 then 1 else 0 end) !=0) \
			  then ( cast(sum( case when steal_attempted = 1 AND folded_to_steal_attempt = 1 AND big_blind_n = 1 then 1 else 0 end ) as real) / \
					 cast(sum( case when steal_attempted = 1 AND big_blind_n = 1 then 1 else 0 end) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* PT2 TOURNAMENT query to get "Folded Small Blind to steal" */
"SELECT (case when (sum(case when steal_attempted = 1 AND small_blind_n = 1 then 1 else 0 end) !=0) \
			  then ( cast(sum( case when steal_attempted = 1 AND folded_to_steal_attempt = 1 AND small_blind_n = 1 then 1 else 0 end ) as real) / \
					 cast(sum( case when steal_attempted = 1 AND small_blind_n = 1 then 1 else 0 end) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%"
};



// Array of PokerTracker version 3 queries, needs to be in same order as stat_str enum
char * const query_str3[] = {

// RING STATISTICS
/* PT3 RING  query to get icon */
"SELECT val_icon \
 FROM player \
 WHERE id_site=%SITEID% AND \
 player_name = '%SCREENNAME%'",

/* PT3 RING  query to get number of hands in the database */
"SELECT COUNT(*) as result \
 FROM   holdem_hand_player_detail as D, player as P \
 WHERE  P.id_player=D.id_player AND \
		P.player_name like '%SCREENNAME%'  AND \
		P.id_site=%SITEID%",

/* PT3 RING  query to get pre flop raise % */
"SELECT (case when (count(*)!=0) \
              then ( cast(sum(case when cnt_p_raise >0 then 1 else 0 end) as real) / count(*)) \
              else (-1) \
         end) as result \
 FROM   player as P, holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 RING  query to get pre-flop aggression factor */
"SELECT (case when (count(*)!=0) \
              then ( cast(sum(cnt_p_raise) as real) / cast(sum(cnt_p_call) as real)) \
              else (-1) \
         end) as result \
 FROM   player as P, holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 RING  query to get flop aggression factor */
"SELECT (case when (count(*)!=0) \
              then ( (cast(sum(cnt_f_raise) as real) + sum(case when flg_f_bet then 1 else 0 end)) / cast(sum(cnt_f_call) as real)) \
              else (-1) \
         end) as result \
 FROM   player as P, holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 RING  query to get turn aggression factor */
"SELECT (case when (count(*)!=0) \
              then ( (cast(sum(cnt_t_raise) as real) + sum(case when flg_t_bet then 1 else 0 end)) / cast(sum(cnt_t_call) as real)) \
              else (-1) \
         end) as result \
 FROM   player as P, holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 RING  query to get river aggression factor */
"SELECT (case when (count(*)!=0) \
              then ( (cast(sum(cnt_r_raise) as real) + sum(case when flg_r_bet then 1 else 0 end)) / cast(sum(cnt_r_call) as real)) \
              else (-1) \
         end) as result \
 FROM   player as P, holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 RING  query to get total aggression factor */
"SELECT (case when (count(*)!=0) \
              then ( ( cast(sum(cnt_p_raise) as real) \
                       + sum(cnt_f_raise) \
					   + sum(case when flg_f_bet then 1 else 0 end) \
                       + sum(cnt_t_raise) \
					   + sum(case when flg_t_bet then 1 else 0 end) \
                       + sum(cnt_r_raise) \
					   + sum(case when flg_r_bet then 1 else 0 end) ) \
                     / ( cast(sum(cnt_p_call) as real) \
                         + sum(cnt_f_call) \
                         + sum(cnt_t_call) \
                         + sum(cnt_r_call) ) \
                   ) \
              else (-1) \
         end) as result \
 FROM   player as P, holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 RING  query to get total aggression factor without preflop */
"SELECT (case when (count(*)!=0) \
              then ( ( cast(sum(cnt_f_raise) + sum(case when flg_f_bet then 1 else 0 end) as real) \
                       + sum(cnt_t_raise) \
				       + sum(case when flg_t_bet then 1 else 0 end) \
                       + sum(cnt_r_raise) \
				       + sum(case when flg_r_bet then 1 else 0 end) ) \
                     / ( sum(cnt_f_call) \
                         + sum(cnt_t_call) \
                         + sum(cnt_r_call) ) \
                   ) \
              else (-1) \
         end) as result \
 FROM   player as P, holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 RING  query to get percentage of flops seen */
"SELECT (case when (count(*)!=0) \
             then cast(sum(case when flg_f_saw then 1 else 0 end) as real) / count(*) \
             else (-1) \
        end) as result \
 FROM   player as P, holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 RING  query to get percentage of turns seen */
"SELECT (case when (count(*)!=0) \
             then cast(sum(case when flg_t_saw then 1 else 0 end) as real) / count(*) \
             else (-1) \
        end) as result \
 FROM   player as P, holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 RING  query to get percentage of rivers seen */
"SELECT (case when (count(*)!=0) \
             then cast(sum(case when flg_r_saw then 1 else 0 end) as real) / count(*) \
             else (-1) \
        end) as result \
 FROM   player as P, holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 RING  query to get vpip */
"SELECT (case when (count(*)!=0) \
              then cast(sum(case when flg_vpip then 1 else 0 end) as real) / count(*) \
              else (-1) \
         end) as result \
 FROM   player as P, holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 RING  query to get pre-flop raise first in pct */
"SELECT (case when (count(*)!=0) \
              then cast(sum(case when flg_p_first_raise AND flg_p_open_opp then 1 else 0 end) as real) \
                   / sum(case when flg_p_open_opp then 1 else 0 end) \
              else (-1) \
         end) as result \
 FROM   player as P, holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 RING  query to get pre-flop called raise pct */
"SELECT (case when (count(*)!=0) \
              then cast(sum(case when flg_p_face_raise AND flg_p_fold = false AND cnt_p_raise=0 then 1 else 0 end) as real) \
                   / sum(case when flg_p_face_raise then 1 else 0 end) \
              else (-1) \
         end) as result \
 FROM   player as P, holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 RING  query to get attempted steal percentage */
"SELECT (case when (count(*)!=0) \
              then cast(sum(case when flg_steal_att then 1 else 0 end) as real) \
                   / sum(case when flg_steal_opp then 1 else 0 end) \
              else (-1) \
         end) as result \
 FROM   player as P, holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 RING  query to get Went to SD % */
"SELECT (case when (count(*)!=0) \
              then cast(sum(case when flg_showdown then 1 else 0 end) as real) \
                   / sum(case when flg_f_saw then 1 else 0 end) \
              else (-1) \
         end) as result \
 FROM   player as P, holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 RING  query to get Won $ At SD % */
"SELECT (case when (count(*)!=0) \
              then cast(sum(case when flg_showdown AND flg_won_hand then 1 else 0 end) as real) \
                   / sum(case when flg_showdown then 1 else 0 end) \
              else (-1) \
         end) as result \
 FROM   player as P, holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 RING  query to get Folded Big Blind to steal */
"SELECT (case when (count(*)!=0) \
              then cast(sum(case when flg_bb_steal_fold then 1 else 0 end) as real) \
                   / sum(case when flg_blind_def_opp AND flg_blind_b then 1 else 0 end) \
              else (-1) \
         end) as result \
 FROM   player as P, holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 RING  query to get Folded Small Blind to steal */
"SELECT (case when (count(*)!=0) \
              then cast(sum(case when flg_sb_steal_fold then 1 else 0 end) as real) \
                   / sum(case when flg_blind_def_opp AND flg_blind_s then 1 else 0 end) \
              else (-1) \
         end) as result \
 FROM   player as P, holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",



// TOURNAMENT STATISTICS
/* PT3 TOURNAMENT query to get icon */
"SELECT val_icon \
 FROM player \
 WHERE id_site=%SITEID% AND \
 player_name = '%SCREENNAME%'",

/* PT3 TOURNAMENT query to get number of hands in the database */
"SELECT COUNT(*) as result \
 FROM   tourney_holdem_hand_player_detail as D, player as P \
 WHERE  P.id_player=D.id_player AND \
		P.player_name like '%SCREENNAME%'  AND \
		P.id_site=%SITEID%",

/* PT3 TOURNAMENT query to get pre flop raise % */
"SELECT (case when (count(*)!=0) \
              then ( cast(sum(case when cnt_p_raise >0 then 1 else 0 end) as real) / count(*)) \
              else (-1) \
         end) as result \
 FROM   player as P, tourney_holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 TOURNAMENT query to get pre-flop aggression factor */
"SELECT (case when (count(*)!=0) \
              then ( cast(sum(cnt_p_raise) as real) / cast(sum(cnt_p_call) as real)) \
              else (-1) \
         end) as result \
 FROM   player as P, tourney_holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 TOURNAMENT query to get flop aggression factor */
"SELECT (case when (count(*)!=0) \
              then ( (cast(sum(cnt_f_raise) as real) + sum(case when flg_f_bet then 1 else 0 end)) / cast(sum(cnt_f_call) as real)) \
              else (-1) \
         end) as result \
 FROM   player as P, tourney_holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 TOURNAMENT query to get turn aggression factor */
"SELECT (case when (count(*)!=0) \
              then ( (cast(sum(cnt_t_raise) as real) + sum(case when flg_t_bet then 1 else 0 end)) / cast(sum(cnt_t_call) as real)) \
              else (-1) \
         end) as result \
 FROM   player as P, tourney_holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 TOURNAMENT query to get river aggression factor */
"SELECT (case when (count(*)!=0) \
              then ( (cast(sum(cnt_r_raise) as real) + sum(case when flg_r_bet then 1 else 0 end)) / cast(sum(cnt_r_call) as real)) \
              else (-1) \
         end) as result \
 FROM   player as P, tourney_holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 TOURNAMENT query to get total aggression factor */
"SELECT (case when (count(*)!=0) \
              then ( ( cast(sum(cnt_p_raise) as real) \
                       + sum(cnt_f_raise) \
					   + sum(case when flg_f_bet then 1 else 0 end) \
                       + sum(cnt_t_raise) \
					   + sum(case when flg_t_bet then 1 else 0 end) \
                       + sum(cnt_r_raise) \
					   + sum(case when flg_r_bet then 1 else 0 end) ) \
                     / ( cast(sum(cnt_p_call) as real) \
                         + sum(cnt_f_call) \
                         + sum(cnt_t_call) \
                         + sum(cnt_r_call) ) \
                   ) \
              else (-1) \
         end) as result \
 FROM   player as P, tourney_holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 TOURNAMENT query to get total aggression factor without preflop */
"SELECT (case when (count(*)!=0) \
              then ( ( cast(sum(cnt_f_raise) + sum(case when flg_f_bet then 1 else 0 end) as real) \
                       + sum(cnt_t_raise) \
				       + sum(case when flg_t_bet then 1 else 0 end) \
                       + sum(cnt_r_raise) \
				       + sum(case when flg_r_bet then 1 else 0 end) ) \
                     / ( sum(cnt_f_call) \
                         + sum(cnt_t_call) \
                         + sum(cnt_r_call) ) \
                   ) \
              else (-1) \
         end) as result \
 FROM   player as P, tourney_holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 TOURNAMENT query to get percentage of flops seen */
"SELECT (case when (count(*)!=0) \
             then cast(sum(case when flg_f_saw then 1 else 0 end) as real) / count(*) \
             else (-1) \
        end) as result \
 FROM   player as P, tourney_holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 TOURNAMENT query to get percentage of turns seen */
"SELECT (case when (count(*)!=0) \
             then cast(sum(case when flg_t_saw then 1 else 0 end) as real) / count(*) \
             else (-1) \
        end) as result \
 FROM   player as P, tourney_holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 TOURNAMENT query to get percentage of rivers seen */
"SELECT (case when (count(*)!=0) \
             then cast(sum(case when flg_r_saw then 1 else 0 end) as real) / count(*) \
             else (-1) \
        end) as result \
 FROM   player as P, tourney_holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 TOURNAMENT query to get vpip */
"SELECT (case when (count(*)!=0) \
              then cast(sum(case when flg_vpip then 1 else 0 end) as real) / count(*) \
              else (-1) \
         end) as result \
 FROM   player as P, tourney_holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 TOURNAMENT query to get pre-flop raise first in pct */
"SELECT (case when (count(*)!=0) \
              then cast(sum(case when flg_p_first_raise AND flg_p_open_opp then 1 else 0 end) as real) \
                   / sum(case when flg_p_open_opp then 1 else 0 end) \
              else (-1) \
         end) as result \
 FROM   player as P, tourney_holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 TOURNAMENT query to get pre-flop called raise pct */
"SELECT (case when (count(*)!=0) \
              then cast(sum(case when flg_p_face_raise AND flg_p_fold = false AND cnt_p_raise=0 then 1 else 0 end) as real) \
                   / sum(case when flg_p_face_raise then 1 else 0 end) \
              else (-1) \
         end) as result \
 FROM   player as P, tourney_holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 TOURNAMENT query to get attempted steal percentage */
"SELECT (case when (count(*)!=0) \
              then cast(sum(case when flg_steal_att then 1 else 0 end) as real) \
                   / sum(case when flg_steal_opp then 1 else 0 end) \
              else (-1) \
         end) as result \
 FROM   player as P, tourney_holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 TOURNAMENT query to get Went to SD % */
"SELECT (case when (count(*)!=0) \
              then cast(sum(case when flg_showdown then 1 else 0 end) as real) \
                   / sum(case when flg_f_saw then 1 else 0 end) \
              else (-1) \
         end) as result \
 FROM   player as P, tourney_holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 TOURNAMENT query to get Won $ At SD % */
"SELECT (case when (count(*)!=0) \
              then cast(sum(case when flg_showdown AND flg_won_hand then 1 else 0 end) as real) \
                   / sum(case when flg_showdown then 1 else 0 end) \
              else (-1) \
         end) as result \
 FROM   player as P, tourney_holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 TOURNAMENT query to get Folded Big Blind to steal */
"SELECT (case when (count(*)!=0) \
              then cast(sum(case when flg_bb_steal_fold then 1 else 0 end) as real) \
                   / sum(case when flg_blind_def_opp AND flg_blind_b then 1 else 0 end) \
              else (-1) \
         end) as result \
 FROM   player as P, tourney_holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 TOURNAMENT query to get Folded Small Blind to steal */
"SELECT (case when (count(*)!=0) \
              then cast(sum(case when flg_sb_steal_fold then 1 else 0 end) as real) \
                   / sum(case when flg_blind_def_opp AND flg_blind_s then 1 else 0 end) \
              else (-1) \
         end) as result \
 FROM   player as P, tourney_holdem_hand_player_statistics as S \
 WHERE  S.id_player = P.id_player AND \
        P.player_name like '%SCREENNAME%' AND \
		P.id_site=%SITEID%"

};

#endif //INC_CPOKERTRACKERTHREAD_H