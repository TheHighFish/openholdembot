#ifndef INC_POKERTRACKER_QUERIES_VERSION_3_H

#define INC_POKERTRACKER_QUERIES_VERSION_3_H


const int k_number_of_pokertracker_stats = 49; ///!!!!

const int k_max_lenght_of_stats_name = 15;	// 14 + 1 char for NULL

const int k_max_length_of_query = 2000;

const int k_max_length_of_site_id = 5;		// for a string with the number, not the name

// PokerTracker support
// This enum is used for both PT2 and PT3
enum PT_Stats 
{
	pt_icon,		pt_hands,		pt_pfr,			pt_aggp, 
	pt_aggf,		pt_aggt,		pt_aggr,		pt_aggtot, 
	pt_aggtotnopf,	pt_floppct,		pt_turnpct,		pt_riverpct, 
	pt_vpip,		pt_pf_rfi,		pt_pf_cr,		pt_pfats, 
	pt_wsdp,		pt_wssd,		pt_fbbts,		pt_fsbts,
	pt_cbetflop,	pt_f3bettot,	pt_f3betpflop,	pt_f3betflop,
	pt_f3betturn,	pt_f3betriver,	pt_fcbetflop,	pt_fcbetturn,
	pt_fcbetriver,	ptt_icon,		ptt_hands,		ptt_pfr,		
	ptt_aggp,		ptt_aggf,		ptt_aggt,		ptt_aggr,		
	ptt_aggtot,		ptt_aggtotnopf, ptt_floppct,	ptt_turnpct,	
	ptt_riverpct, 	ptt_vpip,		ptt_pf_rfi,		ptt_pf_cr,		
	ptt_pfats,		ptt_wsdp,		ptt_wssd,		ptt_fbbts,		
	ptt_fsbts, 
	// End of stats
	// Specification of minimu and maximum
	// These have to be adapted, when new stats are added.
	pt_min = pt_icon,
	pt_max = ptt_fsbts,
	pt_ring_min = pt_icon,
	pt_ring_max = pt_fcbetriver,
	pt_tourney_min = ptt_icon,
	pt_tourney_max = ptt_fsbts
};


enum PT_Groups
{
	pt_group_basic = 1,
	pt_group_advanced = 2,
	pt_group_positional = 3
};

enum PT_UpdateTypes
{
	pt_updateType_noUpdate = 0,
	pt_updateType_updateBasic = 1,
	pt_updateType_updateAll = 2
};

enum PT_StatTypes
{
	pt_statType_Ring = 0,
	pt_statType_Tourney = 1
};


// These are for PokerTracker version 3 stats
const CString stat_str[k_number_of_pokertracker_stats]/*[k_max_lenght_of_stats_name]*/ = {
	"pt_icon",			"pt_hands",			"pt_pfr",			"pt_aggp", 
	"pt_aggf",			"pt_aggt",			"pt_aggr",			"pt_aggtot", 
	"pt_aggtotnopf",	"pt_floppct",		"pt_turnpct",		"pt_riverpct", 
	"pt_vpip",			"pt_pf_rfi",		"pt_pf_cr",			"pt_pfats",	
	"pt_wsdp",			"pt_wssd",			"pt_fbbts",			"pt_fsbts",
	"pt_cbetflop",		"pt_f3bettot",		"pt_f3betpflop",	"pt_f3betflop",
	"pt_f3betturn",		"pt_f3betriver",	"pt_fcbetflop",		"pt_fcbetturn",
	"pt_fcbetriver",	"ptt_icon",			"ptt_hands",		"ptt_pfr",			
	"ptt_aggp",			"ptt_aggf",			"ptt_aggt",			"ptt_aggr",			
	"ptt_aggtot",		"ptt_aggtotnopf",	"ptt_floppct",		"ptt_turnpct",		
	"ptt_riverpct", 	"ptt_vpip",			"ptt_pf_rfi",		"ptt_pf_cr",		
	"ptt_pfats",		"ptt_wsdp",			"ptt_wssd",			"ptt_fbbts",		
	"ptt_fsbts"
};


// Array of PokerTracker version 3 queries, needs to be in same order as stat_str enum
const CString query_str3[k_number_of_pokertracker_stats] = {

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

/****************** KYO ADDED START # new stats: 9 ***************************************/
/* PT3 RING query to get cbet on FLOP */
"SELECT (case when (count(*)!=0) \
			then cast(sum(case when S.flg_f_cbet = 't' then 1 else 0 end) as real) \
				/ cast(sum(case when S.flg_f_cbet_opp = 't' then 1 else 0 end) as real) \
			else (-1) end) as result \
FROM	player as P, holdem_hand_player_statistics as S \
WHERE	S.id_player = P.id_player AND \
		P.player_name LIKE '%SCREENNAME%' AND \
		P.id_site=%SITEID%",

/* PT3 RING query to get TOTAL fold to 3 bet percentage */
"SELECT (case when (count(*)!=0) then \
			(cast(sum(case when (S.enum_p_3bet_action = 'F' or S.enum_f_3bet_action = 'F' or S.enum_t_3bet_action = 'F' or S.enum_r_3bet_action = 'F') then 1 else 0 end) as real) / \
			cast(sum(case when  (S.flg_p_3bet_def_opp = 't' or S.flg_f_3bet_def_opp = 't' or S.flg_t_3bet_def_opp = 't' or S.flg_r_3bet_def_opp = 't') then 1 else 0 end) as real)) \
		else (-1) end) as result \
		FROM player as P, holdem_hand_player_statistics as S \
		WHERE S.id_player = P.id_player \
		AND P.player_name LIKE '%SCREENNAME%'",

/* PT3 RING query to get PREFLOP fold to 3 bet percentage */
"select (case when (count(*)!=0) then \
			cast(sum(case when S.enum_p_3bet_action = 'F' then 1 else 0 end) as real) / \
			cast(sum(case when S.flg_p_3bet_def_opp = 't' then 1 else 0 end) as real) \
		else (-1) end) as result \
		FROM player as P, holdem_hand_player_statistics as S \
		WHERE S.id_player = P.id_player \
		AND P.player_name LIKE '%SCREENNAME%'", 

/* PT3 RING query to get FLOP fold to 3 bet percentage */
"select (case when (count(*)!=0) then \
			cast(sum(case when S.enum_f_3bet_action = 'F' then 1 else 0 end) as real) / \
			cast(sum(case when S.flg_f_3bet_def_opp = 't' then 1 else 0 end) as real) \
		else (-1) end) as result \
		FROM player as P, holdem_hand_player_statistics as S \
		WHERE S.id_player = P.id_player \
		AND P.player_name LIKE '%SCREENNAME%'", 

/* PT3 RING query to get TURN fold to 3 bet percentage */
"select (case when (count(*)!=0) then \
			cast(sum(case when S.enum_t_3bet_action = 'F' then 1 else 0 end) as real) / \
			cast(sum(case when S.flg_t_3bet_def_opp = 't' then 1 else 0 end) as real) \
		else (-1) end) as result \
		FROM player as P, holdem_hand_player_statistics as S \
		WHERE S.id_player = P.id_player \
		AND P.player_name LIKE '%SCREENNAME%'", 

/* PT3 RING query to get RIVER fold to 3 bet percentage */
"select (case when (count(*)!=0) then \
			cast(sum(case when S.enum_r_3bet_action = 'F' then 1 else 0 end) as real) / \
			cast(sum(case when S.flg_r_3bet_def_opp = 't' then 1 else 0 end) as real) \
		else (-1) end) as result \
		FROM player as P, holdem_hand_player_statistics as S \
		WHERE S.id_player = P.id_player \
		AND P.player_name LIKE '%SCREENNAME%'", 

/* PT RING query to get FLOP fold to cbet percentage */
"select (case when (count(*)!=0) then \
			cast(sum(case when S.enum_f_cbet_action = 'F' then 1 else 0 end) as real) / \
			cast(sum(case when S.flg_f_cbet_def_opp = 't' then 1 else 0 end) as real) \
		else (-1) end) as result \
		FROM player as P, holdem_hand_player_statistics as S \
		WHERE S.id_player = P.id_player \
		AND P.player_name LIKE '%SCREENNAME%'",

/* PT RING query to get TURN fold to cbet percentage */
"select (case when (count(*)!=0) then \
			cast(sum(case when S.enum_t_cbet_action = 'F' then 1 else 0 end) as real) / \
			cast(sum(case when S.flg_t_cbet_def_opp = 't' then 1 else 0 end) as real) \
		else (-1) end) as result \
		FROM player as P, holdem_hand_player_statistics as S \
		WHERE S.id_player = P.id_player \
		AND P.player_name LIKE '%SCREENNAME%'",

/* PT RING query to get RIVER fold to cbet percentage */
"select (case when (count(*)!=0) then \
			cast(sum(case when S.enum_r_cbet_action = 'F' then 1 else 0 end) as real) / \
			cast(sum(case when S.flg_r_cbet_def_opp = 't' then 1 else 0 end) as real) \
		else (-1) end) as result \
		FROM player as P, holdem_hand_player_statistics as S \
		WHERE S.id_player = P.id_player \
		AND P.player_name LIKE '%SCREENNAME%'",



/************************** KYO ADDED ENDS ***********************************************/

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

#endif // INC_POKERTRACKER_QUERIES_VERSION_3_H