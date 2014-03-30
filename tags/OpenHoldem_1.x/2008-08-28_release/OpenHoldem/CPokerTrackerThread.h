#ifndef INC_CPOKERTRACKERTHREAD_H
#define INC_CPOKERTRACKERTHREAD_H

#include "libpq-fe.h"

struct SPlayerStats 
{
	char		scraped_name[30];
	char		pt_name[30];
	bool		found;
	double		stat[50];
	int			t_elapsed[50];
};

extern class CPokerTrackerThread 
{
public:
	// Critical section used in public mutators and private shared variable writes
	static CRITICAL_SECTION		cs_pokertracker;

public:
	// public functions
	CPokerTrackerThread();
	~CPokerTrackerThread();
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
	SPlayerStats	_player_stats[10];
	HANDLE			_m_stop_thread;
	HANDLE			_m_wait_thread;

} *p_pokertracker_thread;

char * const networkid[] = {"", "", "Stars", "Party", "Ultimate", "Absolute", "Microgaming", "Ongame",
	"Cryptologic", "Pacific", "", "FullTilt", "B2B", "Tribeca", "Worldpex", "iPoker", "Tain", "Bodog",
	"Everest", "Boss", "Betfair"} ;

char * const stat_str[] = {
	"pt_icon", "pt_hands", "pt_pfr", "pt_aggp", "pt_aggf", "pt_aggt", "pt_aggr", 
	"pt_aggtot", "pt_aggtotnopf", "pt_floppct", "pt_turnpct", "pt_riverpct", "pt_vpip", "pt_pf_rfi",
	"pt_pf_cr", "pt_pfats",	"pt_wsdp", "pt_wssd", "pt_fbbts", "pt_fsbts",
	"ptt_icon", "ptt_hands", "ptt_pfr", "ptt_aggp", "ptt_aggf", "ptt_aggt", "ptt_aggr",
	"ptt_aggtot", "ptt_aggtotnopf", "ptt_floppct", "ptt_turnpct", "ptt_riverpct", "ptt_vpip", "ptt_pf_rfi",
	"ptt_pf_cr", "ptt_pfats", "ptt_wsdp", "ptt_wssd", "ptt_fbbts", "ptt_fsbts"
} ;

// Array of queries, needs to be in same order as pt_stats enum
char * const query_str[] = {

// RING STATISTICS
/* query to get icon */
"SELECT treeview_icon FROM players WHERE main_site_id=%SITEID% \
 AND screen_name = '%SCREENNAME%'",

/* query to get number of hands in the database */
"SELECT COUNT(*) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* query to get pre flop raise % */
"SELECT (case when (count(*)!=0) \
			  then ( cast(sum(pre_flop_raise_n) as real) / cast(count(*) as real) ) \
			  else (-1) \
		 end) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* query to get pre-flop aggression factor */
"SELECT (case when (SUM(pcall)!=0) \
			  then ( cast(SUM(praise) as real) / cast(SUM(pcall) as real) ) \
			  else (-1) \
		 end) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* query to get flop aggression factor */
"SELECT (case when (SUM(fcall)!=0) \
			  then ( (cast(SUM(fraise) as real) + cast(SUM(fbet) as real)) / cast(SUM(fcall) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* query to get turn aggression factor */
"SELECT (case when (SUM(tcall)!=0) \
			  then ( (cast(SUM(traise) as real) + cast(SUM(tbet) as real)) / cast(SUM(tcall) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* query to get river aggression factor */
"SELECT (case when (SUM(rcall)!=0) \
			  then ( (cast(SUM(rraise) as real) + cast(SUM(rbet) as real)) / cast(SUM(rcall) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* query to get total aggression factor */
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

/* query to get total aggression factor without preflop */
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

/* query to get percentage of flops seen */
"SELECT (case when (COUNT(*)!=0) \
			  then ( cast(SUM(saw_flop_n) as real) / cast(COUNT(*) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* query to get percentage of turns seen */
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

/* query to get percentage of rivers seen */
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

/* query to get vpip */
"SELECT (case when (COUNT(*)!=0) \
			  then ( cast(SUM(vol_put_money_in_pot) as real) / cast(COUNT(*) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",
	
/* query to get pre-flop raise first in pct */
"SELECT (case when (COUNT(*)!=0) \
			  then ( cast(SUM(raised_first_pf) as real) / cast(COUNT(*) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* query to get pre-flop called raise pct */
"SELECT (case when (COUNT(*)!=0) \
			  then ( cast(SUM(limp_call_reraise_pf) as real) / cast(COUNT(*) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* query to get attempted steal percentage */
"SELECT (case when (count(*) !=0) \
			  then ( cast(sum(attempted_steal) as real)/cast(sum(chance_to_steal) as real)) \
			  else (-1) \
		 end) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* query to get "Went to SD %" */
"SELECT (case when (sum(saw_flop_n)!=0) \
			  then ( cast(sum(went_to_showdown_n) as real) / cast(sum(saw_flop_n) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* query to get "Won $ At SD %" */
"SELECT (case when (sum(went_to_showdown_n)!=0) \
			  then ( cast(sum( case when went_to_showdown_n = 1 then won_hand else 0 end ) as real) / cast(sum(went_to_showdown_n) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* query to get "Folded Big Blind to steal" */
"SELECT (case when (sum(case when steal_attempted = 1 AND big_blind_n = 1 then 1 else 0 end) !=0) \
			  then ( cast(sum( case when steal_attempted = 1 AND folded_to_steal_attempt = 1 AND big_blind_n = 1 then 1 else 0 end ) as real) / \
					 cast(sum( case when steal_attempted = 1 AND big_blind_n = 1 then 1 else 0 end) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   game_players, players \
 WHERE  game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* query to get "Folded Small Blind to steal" */
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
/* query to get icon */
"SELECT treeview_icon FROM players WHERE main_site_id=%SITEID% \
 AND screen_name = '%SCREENNAME%'",

/* query to get number of hands in the database */
"SELECT COUNT(*) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* query to get pre flop raise % */
"SELECT (case when (count(*)!=0) \
			  then ( cast(sum(pre_flop_raise_n) as real) / cast(count(*) as real) ) \
			  else (-1) \
		 end) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* query to get pre-flop aggression factor */
"SELECT (case when (SUM(pcall)!=0) \
			  then ( cast(SUM(praise) as real) / cast(SUM(pcall) as real) ) \
			  else (-1) \
		 end) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* query to get flop aggression factor */
"SELECT (case when (SUM(fcall)!=0) \
			  then ( (cast(SUM(fraise) as real) + cast(SUM(fbet) as real)) / cast(SUM(fcall) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* query to get turn aggression factor */
"SELECT (case when (SUM(tcall)!=0) \
			  then ( (cast(SUM(traise) as real) + cast(SUM(tbet) as real)) / cast(SUM(tcall) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* query to get river aggression factor */
"SELECT (case when (SUM(rcall)!=0) \
			  then ( (cast(SUM(rraise) as real) + cast(SUM(rbet) as real)) / cast(SUM(rcall) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* query to get total aggression factor */
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

/* query to get total aggression factor without preflop */
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

/* query to get percentage of flops seen */
"SELECT (case when (COUNT(*)!=0) \
			  then ( cast(SUM(saw_flop_n) as real) / cast(COUNT(*) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* query to get percentage of turns seen */
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

/* query to get percentage of rivers seen */
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

/* query to get vpip */
"SELECT (case when (COUNT(*)!=0) \
			  then ( cast(SUM(vol_put_money_in_pot) as real) / cast(COUNT(*) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",
	
/* query to get pre-flop raise first in pct */
"SELECT (case when (COUNT(*)!=0) \
			  then ( cast(SUM(raised_first_pf) as real) / cast(COUNT(*) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* query to get pre-flop called raise pct */
"SELECT (case when (COUNT(*)!=0) \
			  then ( cast(SUM(limp_call_reraise_pf) as real) / cast(COUNT(*) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* query to get attempted steal percentage */
"SELECT (case when (count(*) !=0) \
			  then ( cast(sum(attempted_steal) as real)/cast(sum(chance_to_steal) as real)) \
			  else (-1) \
		 end) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* query to get "Went to SD %" */
"SELECT (case when (sum(saw_flop_n)!=0) \
			  then ( cast(sum(went_to_showdown_n) as real) / cast(sum(saw_flop_n) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* query to get "Won $ At SD %" */
"SELECT (case when (sum(went_to_showdown_n)!=0) \
			  then ( cast(sum( case when went_to_showdown_n = 1 then won_hand else 0 end ) as real) / cast(sum(went_to_showdown_n) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* query to get "Folded Big Blind to steal" */
"SELECT (case when (sum(case when steal_attempted = 1 AND big_blind_n = 1 then 1 else 0 end) !=0) \
			  then ( cast(sum( case when steal_attempted = 1 AND folded_to_steal_attempt = 1 AND big_blind_n = 1 then 1 else 0 end ) as real) / \
					 cast(sum( case when steal_attempted = 1 AND big_blind_n = 1 then 1 else 0 end) as real)  ) \
			  else (-1) \
		 end) as result \
 FROM   tourney_game_players, players \
 WHERE  tourney_game_players.player_id = players.player_id AND \
		screen_name like '%SCREENNAME%'  AND \
		players.main_site_id=%SITEID%",

/* query to get "Folded Small Blind to steal" */
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

#endif //INC_CPOKERTRACKERTHREAD_H