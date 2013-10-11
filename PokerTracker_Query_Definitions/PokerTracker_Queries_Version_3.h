//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*****************************************************************************
//
// Purpose:
//
//*****************************************************************************

#ifndef INC_POKERTRACKER_QUERIES_VERSION_3_H
#define INC_POKERTRACKER_QUERIES_VERSION_3_H

#define PT3_QUERY_SUPPORT__ICON							(TRUE)
#define PT3_QUERY_SUPPORT__NB_HANDS						(TRUE)
#define PT3_QUERY_SUPPORT__PFR							(TRUE)
#define PT3_QUERY_SUPPORT__PREFLOP_AGGR_FACTOR			(TRUE)
#define PT3_QUERY_SUPPORT__FLOP_AGGR_FACTOR				(TRUE)
#define PT3_QUERY_SUPPORT__TURN_AGGR_FACTOR				(TRUE)
#define PT3_QUERY_SUPPORT__RIVER_AGGR_FACTOR			(TRUE)
#define PT3_QUERY_SUPPORT__OVERALL_AGGR_FACTOR			(TRUE)
#define PT3_QUERY_SUPPORT__AGGR_FACTOR_NO_PREFLOP		(TRUE)
#define PT3_QUERY_SUPPORT__SEEN_FLOP_PCT				(TRUE)

#define PT3_QUERY_SUPPORT__SEEN_TURN_PCT				(TRUE)
#define PT3_QUERY_SUPPORT__SEEN_RIVER_PCT				(TRUE)
#define PT3_QUERY_SUPPORT__VPIP							(TRUE)
#define PT3_QUERY_SUPPORT__RFI							(TRUE)
#define PT3_QUERY_SUPPORT__PREFLOP_CALLED_RAISE			(TRUE)
#define PT3_QUERY_SUPPORT__STEAL_ATTEMPT				(TRUE)
#define PT3_QUERY_SUPPORT__WTSD							(TRUE)
#define PT3_QUERY_SUPPORT__WSSD							(TRUE)
#define PT3_QUERY_SUPPORT__FOLD_BB_TO_STEAL				(TRUE)
#define PT3_QUERY_SUPPORT__FOLD_SB_TO_STEAL				(TRUE)

#define PT3_QUERY_SUPPORT__FLOP_CBET					(TRUE)
#define PT3_QUERY_SUPPORT__OVERALL_FOLD_TO_3B			(TRUE)
#define PT3_QUERY_SUPPORT__PREFLOP_FOLD_TO_3B			(TRUE)
#define PT3_QUERY_SUPPORT__FLOP_FOLD_TO_3B				(TRUE)
#define PT3_QUERY_SUPPORT__TURN_FOLD_TO_3B				(TRUE)
#define PT3_QUERY_SUPPORT__RIVER_FOLD_TO_3B				(TRUE)
#define PT3_QUERY_SUPPORT__FLOP_FOLD_TO_CBET			(TRUE)
#define PT3_QUERY_SUPPORT__TURN_FOLD_TO_CBET			(TRUE)
#define PT3_QUERY_SUPPORT__RIVER_FOLD_TO_CBET			(TRUE)


const int k_number_of_pokertracker_stats =  (PT3_QUERY_SUPPORT__ICON ? 1 : 0) +
											(PT3_QUERY_SUPPORT__NB_HANDS ? 1 : 0) + 
											(PT3_QUERY_SUPPORT__PFR ? 1 : 0) + 
											(PT3_QUERY_SUPPORT__PREFLOP_AGGR_FACTOR ? 1 : 0) + 
											(PT3_QUERY_SUPPORT__FLOP_AGGR_FACTOR ? 1 : 0) + 
											(PT3_QUERY_SUPPORT__TURN_AGGR_FACTOR ? 1 : 0) + 
											(PT3_QUERY_SUPPORT__RIVER_AGGR_FACTOR ? 1 : 0) + 
											(PT3_QUERY_SUPPORT__OVERALL_AGGR_FACTOR ? 1 : 0) + 
											(PT3_QUERY_SUPPORT__AGGR_FACTOR_NO_PREFLOP ? 1 : 0) + 
											(PT3_QUERY_SUPPORT__SEEN_FLOP_PCT ? 1 : 0) + 
											(PT3_QUERY_SUPPORT__SEEN_TURN_PCT ? 1 : 0) + 
											(PT3_QUERY_SUPPORT__SEEN_RIVER_PCT ? 1 : 0) + 
											(PT3_QUERY_SUPPORT__VPIP ? 1 : 0) + 
											(PT3_QUERY_SUPPORT__RFI ? 1 : 0) + 
											(PT3_QUERY_SUPPORT__PREFLOP_CALLED_RAISE ? 1 : 0) + 
											(PT3_QUERY_SUPPORT__STEAL_ATTEMPT ? 1 : 0) + 
											(PT3_QUERY_SUPPORT__WTSD ? 1 : 0) + 
											(PT3_QUERY_SUPPORT__WSSD ? 1 : 0) + 
											(PT3_QUERY_SUPPORT__FOLD_BB_TO_STEAL ? 1 : 0) + 
											(PT3_QUERY_SUPPORT__FOLD_SB_TO_STEAL ? 1 : 0) + 
											(PT3_QUERY_SUPPORT__FLOP_CBET ? 1 : 0) + 
											(PT3_QUERY_SUPPORT__OVERALL_FOLD_TO_3B ? 1 : 0) + 
											(PT3_QUERY_SUPPORT__PREFLOP_FOLD_TO_3B ? 1 : 0) + 
											(PT3_QUERY_SUPPORT__FLOP_FOLD_TO_3B ? 1 : 0) + 
											(PT3_QUERY_SUPPORT__TURN_FOLD_TO_3B ? 1 : 0) + 
											(PT3_QUERY_SUPPORT__RIVER_FOLD_TO_3B ? 1 : 0) + 
											(PT3_QUERY_SUPPORT__FLOP_FOLD_TO_CBET ? 1 : 0) + 
											(PT3_QUERY_SUPPORT__TURN_FOLD_TO_CBET ? 1 : 0) + 
											(PT3_QUERY_SUPPORT__RIVER_FOLD_TO_CBET ? 1 : 0);

// PokerTracker support
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

typedef struct t_QueryDefinition
{
	CString name;
	CString description_for_editor;
	CString query;
	int stat_group;
};


t_QueryDefinition query_definitions[k_number_of_pokertracker_stats] =
{
#if PT3_QUERY_SUPPORT__ICON
	/* PT3 query to get icon */
	{
		// name
		"icon",
		// description_for_editor
		"Poker Tracker auto-rate icon code",
		// query
		"SELECT val_icon as result \
		 FROM   player \
		 WHERE  id_site = %SITEID% AND \
		        player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_basic
	},
#endif
#if PT3_QUERY_SUPPORT__NB_HANDS
	/* PT3 query to get number of hands in the database */
	{
		// name
		"hands",	
		// description_for_editor
		"Poker Tracker number of hands that are in the database",
		// query
		"SELECT COUNT(*) as result \
		 FROM   player as P, %GAMETYPE%player_detail as D \
		 WHERE  D.id_player = P.id_player AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_basic
	},
#endif
#if PT3_QUERY_SUPPORT__PFR
	/* PT3 query to get pre flop raise % */
	{
		// name
		"pfr",			
		// description_for_editor
		"Poker Tracker pre-flop raise percentage",
		// query
		"SELECT (case when (count(*) = 0) \
		        then -1 \
				else (cast(sum(case when cnt_p_raise > 0 then 1 else 0 end) as real) / count(*)) \
				end) as result \
		 FROM   player as P, %GAMETYPE%player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_basic
	},
#endif
#if PT3_QUERY_SUPPORT__PREFLOP_AGGR_FACTOR
	/* PT3 query to get pre-flop aggression factor */
	{
		// name
		"aggp",
		// description_for_editor
		"Poker Tracker preflop aggression",
		// query
		"SELECT (case when sum(S.cnt_p_call) = 0 \
		        then -1 \
				else cast(sum(S.cnt_p_raise) as real) / sum(S.cnt_p_call) \
				end) as result \
		 FROM   player as P, %GAMETYPE%player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_basic
	},
#endif
#if PT3_QUERY_SUPPORT__FLOP_AGGR_FACTOR
	/* PT3 query to get flop aggression factor */
	{
		// name
		"aggf",
		// description_for_editor
		"Poker Tracker flop aggression",
		// query
		"SELECT (case when sum(S.cnt_f_call) = 0 \
		        then -1 \
				else cast((sum(S.cnt_f_raise) + sum(case when S.flg_f_bet then 1 else 0 end)) as real) \
				/ sum(S.cnt_f_call) \
				end) as result \
		 FROM   player as P, %GAMETYPE%player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT3_QUERY_SUPPORT__TURN_AGGR_FACTOR
	/* PT3 query to get turn aggression factor */
	{
		// name
		"aggt",
		// description_for_editor
		"Poker Tracker turn aggression",
		// query
		"SELECT (case when sum(S.cnt_t_call) = 0 \
		        then -1 \
				else cast((sum(S.cnt_t_raise) + sum(case when S.flg_t_bet then 1 else 0 end)) as real) \
				/ sum(S.cnt_t_call) \
				end) as result \
		 FROM   player as P, %GAMETYPE%player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT3_QUERY_SUPPORT__RIVER_AGGR_FACTOR
	/* PT3 query to get river aggression factor */
	{
		// name
		"aggr",			
		// description_for_editor
		"Poker Tracker river aggression",
		// query
		"SELECT (case when sum(S.cnt_r_call) = 0 \
		        then -1 \
				else cast((sum(S.cnt_r_raise) + sum(case when S.flg_r_bet then 1 else 0 end)) as real) \
				/ sum(S.cnt_r_call) \
				end) as result \
		 FROM   player as P, %GAMETYPE%player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT3_QUERY_SUPPORT__OVERALL_AGGR_FACTOR
	/* PT3 query to get total aggression factor */
	{
		// name
		"aggtot", 
		// description_for_editor
		"Poker Tracker total aggression",
		// query
		"SELECT (case when B = 0 \
		        then -1 \
				else cast(A as real) / B \
				end) as result \
		 FROM (SELECT sum(cnt_p_raise) \
                       + sum(cnt_f_raise) \
					   + sum(case when flg_f_bet then 1 else 0 end) \
                       + sum(cnt_t_raise) \
					   + sum(case when flg_t_bet then 1 else 0 end) \
                       + sum(cnt_r_raise) \
					   + sum(case when flg_r_bet then 1 else 0 end) as A, \
		              sum(cnt_p_call) \
                       + sum(cnt_f_call) \
                       + sum(cnt_t_call) \
                       + sum(cnt_r_call) as B \
			   FROM   player as P, %GAMETYPE%player_statistics as S \
			   WHERE  S.id_player = P.id_player AND \
		              P.id_site = %SITEID% AND \
		              P.player_name LIKE '%SCREENNAME%') foo",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT3_QUERY_SUPPORT__AGGR_FACTOR_NO_PREFLOP
	/* PT3 query to get total aggression factor without preflop */
	{
		// name
		"aggtotnopf",	
		// description_for_editor
		"Poker Tracker total aggression excluding preflop",
		// query
		"SELECT (case when B = 0 \
		        then -1 \
				else cast(A as real) / B \
				end) as result \
		 FROM (SELECT sum(cnt_f_raise) \
					   + sum(case when flg_f_bet then 1 else 0 end) \
                       + sum(cnt_t_raise) \
					   + sum(case when flg_t_bet then 1 else 0 end) \
                       + sum(cnt_r_raise) \
					   + sum(case when flg_r_bet then 1 else 0 end) as A, \
		              sum(cnt_f_call) \
                       + sum(cnt_t_call) \
                       + sum(cnt_r_call) as B \
			   FROM   player as P, %GAMETYPE%player_statistics as S \
			   WHERE  S.id_player = P.id_player AND \
		              P.id_site = %SITEID% AND \
		              P.player_name LIKE '%SCREENNAME%') foo",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT3_QUERY_SUPPORT__SEEN_FLOP_PCT
	/* PT3 query to get percentage of flops seen */
	{
		// name
		"floppct",		
		// description_for_editor
		"Poker Tracker saw flop percentage",
		// query
		"SELECT (case when (count(*) = 0) \
		         then -1 \
				 else cast(sum(case when flg_f_saw then 1 else 0 end) as real) / count(*) \
				 end) as result \
		FROM   player as P, %GAMETYPE%player_statistics as S \
		WHERE  S.id_player = P.id_player AND \
		       P.id_site = %SITEID% AND \
		       P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT3_QUERY_SUPPORT__SEEN_TURN_PCT
	/* PT3 query to get percentage of turns seen */
	{
		// name
		"turnpct",		
		// description_for_editor
		"Poker Tracker saw turn percentage",
		// query
		"SELECT (case when (count(*) = 0) \
		         then -1 \
				 else cast(sum(case when flg_t_saw then 1 else 0 end) as real) / count(*) \
				 end) as result \
		FROM   player as P, %GAMETYPE%player_statistics as S \
		WHERE  S.id_player = P.id_player AND \
		       P.id_site = %SITEID% AND \
		       P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT3_QUERY_SUPPORT__SEEN_RIVER_PCT
	/* PT3 query to get percentage of rivers seen */
	{
		// name
		"riverpct", 
		// description_for_editor
		"Poker Tracker saw river percentage",
		// query
		"SELECT (case when (count(*) = 0) \
		         then -1 \
				 else cast(sum(case when flg_r_saw then 1 else 0 end) as real) / count(*) \
				 end) as result \
		FROM   player as P, %GAMETYPE%player_statistics as S \
		WHERE  S.id_player = P.id_player AND \
		       P.id_site = %SITEID% AND \
		       P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT3_QUERY_SUPPORT__VPIP
	/* PT3 query to get vpip */
	{
		// name
		"vpip",			
		// description_for_editor
		"Poker Tracker VP$IP",
		// query
		"SELECT (case when (count(*) = 0) \
		         then -1 \
				 else cast(sum(case when flg_vpip then 1 else 0 end) as real) / count(*) \
				 end) as result \
		FROM   player as P, %GAMETYPE%player_statistics as S \
		WHERE  S.id_player = P.id_player AND \
		       P.id_site = %SITEID% AND \
		       P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_basic
	},
#endif
#if PT3_QUERY_SUPPORT__RFI
	/* PT3 query to get pre-flop raise first in pct */
	{
		// name
		"pf_rfi",		
		// description_for_editor
		"Poker Tracker pre-flop raise first in percentage",
		// query
		"SELECT (case when count(*) = 0 then -1 else \
		        cast(sum(case when flg_p_first_raise then 1 else 0 end) as real) / count(*) end) as result \
		 FROM   player as P, %GAMETYPE%player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
		 	    S.flg_p_open_opp AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_basic
	},
#endif
#if PT3_QUERY_SUPPORT__PREFLOP_CALLED_RAISE
	/* PT3 query to get pre-flop called raise pct */
	{
		// name
		"pf_cr",		
		// description_for_editor
		"Poker Tracker pre-flop called raise percentage",
		// query
		"SELECT (case when count(*) = 0 then -1 else \
		        cast(sum(case when not flg_p_fold AND cnt_p_raise = 0 then 1 else 0 end) as real) / count(*) end) as result \
		 FROM   player as P, %GAMETYPE%player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
		 	    S.flg_p_face_raise AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT3_QUERY_SUPPORT__STEAL_ATTEMPT
	/* PT3 query to get attempted steal percentage */
	{
		// name
		"pfats",	
		// description_for_editor
		"Poker Tracker attempt to steal blinds",
		// query
		"SELECT (case when sum(case when flg_steal_opp then 1 else 0 end) = 0 then -1 else \
		        cast(sum(case when flg_steal_att then 1 else 0 end) as real) \
				/ sum(case when flg_steal_opp then 1 else 0 end) end) as result \
		 FROM   player as P, %GAMETYPE%player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT3_QUERY_SUPPORT__WTSD
	/* PT3 query to get Went to SD % */
	{
		// name
		"wsdp",			
		// description_for_editor
		"Poker Tracker went to showdown percentage",
		// query
		"SELECT (case when sum(case when flg_f_saw then 1 else 0 end) = 0 then -1 else \
		        cast(sum(case when flg_showdown then 1 else 0 end) as real) \
				/ sum(case when flg_f_saw then 1 else 0 end) end) as result \
		 FROM   player as P, %GAMETYPE%player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_basic
	},
#endif
#if PT3_QUERY_SUPPORT__WSSD
	/* PT3 query to get Won $ At SD % */
	{
		// name
		"wssd",			
		// description_for_editor
		"Poker Tracker won $ at showdown percentae",
		// query
		"SELECT (case when count(*) = 0 then -1 else \
		        cast(sum(case when flg_won_hand then 1 else 0 end) as real) \
				/ count(*) end) as result \
		 FROM   player as P, %GAMETYPE%player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
				flg_showdown AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_basic
	},
#endif
#if PT3_QUERY_SUPPORT__FOLD_BB_TO_STEAL
	/* PT3 query to get Folded Big Blind to steal */
	{
		// name
		"fbbts",		
		// description_for_editor
		"Poker Tracker folded big blind to steal",
		// query
		"SELECT (case when sum(case when flg_blind_def_opp AND flg_blind_b then 1 else 0 end) = 0 then -1 else \
		        cast(sum(case when flg_bb_steal_fold then 1 else 0 end) as real) \
				/ sum(case when flg_blind_def_opp AND flg_blind_b then 1 else 0 end) end) as result \
		 FROM   player as P, %GAMETYPE%player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_positional
	},
#endif
#if PT3_QUERY_SUPPORT__FOLD_SB_TO_STEAL
	/* PT3 query to get Folded Small Blind to steal */
	{
		// name
		"fsbts",
		// description_for_editor
		"Poker Tracker folded small blind to steal",
		// query
		"SELECT (case when sum(case when flg_blind_def_opp AND flg_blind_s then 1 else 0 end) = 0 then -1 else \
		        cast(sum(case when flg_sb_steal_fold then 1 else 0 end) as real) \
				/ sum(case when flg_blind_def_opp AND flg_blind_s then 1 else 0 end) end) as result \
		 FROM   player as P, %GAMETYPE%player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_positional
	},
#endif
#if PT3_QUERY_SUPPORT__FLOP_CBET
	/* PT3  query to get cbet on FLOP */
	{
		// name
		"cbetflop",		
		// description_for_editor
		"Poker Tracker cbet percentage",
		// query
		"SELECT (case when sum(case when flg_f_cbet_opp then 1 else 0 end) = 0 then -1 else \
		        cast(sum(case when flg_f_cbet then 1 else 0 end) as real) \
				/ sum(case when flg_f_cbet_opp then 1 else 0 end) end) as result \
		 FROM   player as P, %GAMETYPE%player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT3_QUERY_SUPPORT__OVERALL_FOLD_TO_3B
	/* PT3  query to get TOTAL fold to 3 bet percentage */
	{
		// name
		"f3bettot",		
		// description_for_editor
		"Poker Tracker folded while facing 3bet total percentage",
		// query
		"SELECT (case when sum(case when flg_p_3bet_def_opp or flg_f_3bet_def_opp or flg_t_3bet_def_opp or flg_r_3bet_def_opp then 1 else 0 end) = 0 then -1 else \
		        cast(sum(case when not enum_p_3bet_action = 'F' or enum_f_3bet_action = 'F' or enum_t_3bet_action ='F' or enum_r_3bet_action = 'F' then 1 else 0 end) as real) \
				/ sum(case when flg_p_3bet_def_opp or flg_f_3bet_def_opp or flg_t_3bet_def_opp or flg_r_3bet_def_opp then 1 else 0 end) end) as result \
		 FROM   player as P, %GAMETYPE%player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT3_QUERY_SUPPORT__PREFLOP_FOLD_TO_3B
	/* PT3  query to get PREFLOP fold to 3 bet percentage */
	{
		// name
		"f3betpflop",	
		// description_for_editor
		"Poker Tracker folded while facing 3bet preflop percentage",
		// query
		"SELECT (case when sum(case when flg_p_3bet_def_opp then 1 else 0 end) = 0 then -1 else \
		        cast(sum(case when enum_p_3bet_action = 'F' then 1 else 0 end) as real) \
				/ sum(case when flg_p_3bet_def_opp then 1 else 0 end) end) as result \
		 FROM   player as P, %GAMETYPE%player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT3_QUERY_SUPPORT__FLOP_FOLD_TO_3B
	/* PT3  query to get FLOP fold to 3 bet percentage */
	{
		// name
		"f3betflop",
		// description_for_editor
		"Poker Tracker folded while facing 3bet flop percentage",
		// query
		"SELECT (case when sum(case when flg_f_3bet_def_opp then 1 else 0 end) = 0 then -1 else \
		        cast(sum(case when enum_f_3bet_action = 'F' then 1 else 0 end) as real) \
				/ sum(case when flg_f_3bet_def_opp then 1 else 0 end) end) as result \
		 FROM   player as P, %GAMETYPE%player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT3_QUERY_SUPPORT__TURN_FOLD_TO_3B
	/* PT3  query to get TURN fold to 3 bet percentage */
	{
		// name
		"f3betturn",	
		// description_for_editor
		"Poker Tracker folded while facing 3bet turn percentage",
		// query
		"SELECT (case when sum(case when flg_t_3bet_def_opp then 1 else 0 end) = 0 then -1 else \
		        cast(sum(case when enum_t_3bet_action = 'F' then 1 else 0 end) as real) \
				/ sum(case when flg_t_3bet_def_opp then 1 else 0 end) end) as result \
		 FROM   player as P, %GAMETYPE%player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT3_QUERY_SUPPORT__RIVER_FOLD_TO_3B
	/* PT3  query to get RIVER fold to 3 bet percentage */
	{
		// name
		"f3betriver",	
		// description_for_editor
		"Poker Tracker folded while facing 3bet river percentage",
		// query
		"SELECT (case when sum(case when flg_r_3bet_def_opp then 1 else 0 end) = 0 then -1 else \
		        cast(sum(case when enum_r_3bet_action = 'F' then 1 else 0 end) as real) \
				/ sum(case when flg_r_3bet_def_opp then 1 else 0 end) end) as result \
		 FROM   player as P, %GAMETYPE%player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT3_QUERY_SUPPORT__FLOP_FOLD_TO_CBET
	/* PT3  query to get FLOP fold to cbet percentage */
	{
		// name
		"fcbetflop",	
		// description_for_editor
		"Poker Tracker folded while facing cbet flop percentage",
		// query
		"SELECT (case when sum(case when flg_f_cbet_def_opp then 1 else 0 end) = 0 then -1 else \
		        cast(sum(case when enum_f_cbet_action = 'F' then 1 else 0 end) as real) \
				/ sum(case when flg_f_cbet_def_opp then 1 else 0 end) end) as result \
		 FROM   player as P, %GAMETYPE%player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT3_QUERY_SUPPORT__TURN_FOLD_TO_CBET
	/* PT3  query to get TURN fold to cbet percentage */
	{
		// name
		"fcbetturn",
		// description_for_editor
		"Poker Tracker folded while facing cbet turn percentage",
		// query
		"SELECT (case when sum(case when flg_t_cbet_def_opp then 1 else 0 end) = 0 then -1 else \
		        cast(sum(case when enum_t_cbet_action = 'F' then 1 else 0 end) as real) \
				/ sum(case when flg_t_cbet_def_opp then 1 else 0 end) end) as result \
		 FROM   player as P, %GAMETYPE%player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT3_QUERY_SUPPORT__RIVER_FOLD_TO_CBET
	/* PT3  query to get RIVER fold to cbet percentage */
	{
		// name
		"fcbetriver",
		// description_for_editor
		"Poker Tracker folded while facing cbet river percentage",
		// query
		"SELECT (case when sum(case when flg_r_cbet_def_opp then 1 else 0 end) = 0 then -1 else \
		        cast(sum(case when enum_r_cbet_action = 'F' then 1 else 0 end) as real) \
				/ sum(case when flg_r_cbet_def_opp then 1 else 0 end) end) as result \
		 FROM   player as P, %GAMETYPE%player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
};

#endif // INC_POKERTRACKER_QUERIES_VERSION_3_H