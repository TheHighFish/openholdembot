//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#ifndef INC_POKERTRACKER_QUERIES_VERSION_4_H
#define INC_POKERTRACKER_QUERIES_VERSION_4_H
//PREFLOP GENERAL STATS
#define PT4_QUERY_SUPPORT__ICON							(TRUE)
#define PT4_QUERY_SUPPORT__NB_HANDS					(TRUE)
#define PT4_QUERY_SUPPORT__VPIP							(TRUE)
#define PT4_QUERY_SUPPORT__PFR							(TRUE)
#define PT4_QUERY_SUPPORT__RFI							(TRUE)
#define PT4_QUERY_SUPPORT__WTSD							(TRUE)
#define PT4_QUERY_SUPPORT__WSSD							(TRUE)
#define PT4_QUERY_SUPPORT__PREFLOP_AGGR_FACTOR		(TRUE)
#define PT4_QUERY_SUPPORT__FLOP_AGGR_FACTOR				(TRUE)
#define PT4_QUERY_SUPPORT__TURN_AGGR_FACTOR				(TRUE)
#define PT4_QUERY_SUPPORT__RIVER_AGGR_FACTOR			(TRUE)
#define PT4_QUERY_SUPPORT__OVERALL_AGGR_FACTOR		(TRUE)
#define PT4_QUERY_SUPPORT__AGGR_FACTOR_NO_PREFLOP	(TRUE)
#define PT4_QUERY_SUPPORT__SEEN_FLOP_PCT			  	(TRUE)
#define PT4_QUERY_SUPPORT__SEEN_TURN_PCT			  	(TRUE)
#define PT4_QUERY_SUPPORT__SEEN_RIVER_PCT			  	(TRUE)
#define PT4_QUERY_SUPPORT__OVERALL_FOLD_TO_3B			(TRUE)
#define PT4_QUERY_SUPPORT__OVERALL_4B					(TRUE)
//PREFLOP OTHER STATS
#define PT4_QUERY_SUPPORT__PREFLOP_3B					    (TRUE)
#define PT4_QUERY_SUPPORT__PREFLOP_FOLD_TO_3B			(TRUE)
#define PT4_QUERY_SUPPORT__PREFLOP_CALLED_RAISE		(TRUE)
#define PT4_QUERY_SUPPORT__STEAL_ATTEMPT				  (TRUE)
#define PT4_QUERY_SUPPORT__FOLD_BB_TO_STEAL				(TRUE)
#define PT4_QUERY_SUPPORT__FOLD_SB_TO_STEAL				(TRUE)
#define PT4_QUERY_SUPPORT__3B_VS_STEAL            (TRUE)
#define PT4_QUERY_SUPPORT__BB_3B_VS_STEAL         (TRUE)
#define PT4_QUERY_SUPPORT__SB_3B_VS_STEAL         (TRUE)
#define PT4_QUERY_SUPPORT__PREFLOP_4B             (TRUE)
#define PT4_QUERY_SUPPORT__PREFLOP_FOLD_TO_4B           (TRUE)
//FLOP
#define PT4_QUERY_SUPPORT__FLOP_CBET              (TRUE)
#define PT4_QUERY_SUPPORT__FLOP_FOLD_TO_CBET			(TRUE)
#define PT4_QUERY_SUPPORT__RAISE_FLOP_CBET				(TRUE)
#define PT4_QUERY_SUPPORT__FLOP_FOLD_TO_3B				(TRUE)
#define PT4_QUERY_SUPPORT__FLOP_CHECK_RAISE             (TRUE)
#define PT4_QUERY_SUPPORT__FLOP_DONK					(TRUE)
//TURN
#define PT4_QUERY_SUPPORT__TURN_CBET					    (TRUE)
#define PT4_QUERY_SUPPORT__TURN_FOLD_TO_CBET			(TRUE)
#define PT4_QUERY_SUPPORT__TURN_FOLD_TO_3B				(TRUE)
#define PT4_QUERY_SUPPORT__TURN_CHECK_RAISE	      (TRUE)
#define PT4_QUERY_SUPPORT__TURN_CHECK_CALL	      (TRUE)
//RIVER
#define PT4_QUERY_SUPPORT__RIVER_FOLD_TO_3B				(TRUE)
#define PT4_QUERY_SUPPORT__RIVER_FOLD_TO_CBET			(TRUE)
#define PT4_QUERY_SUPPORT__RIVER_BET     				(TRUE)

const int k_number_of_pokertracker_stats =  //PREFLOP GENERAL STATS
											(PT4_QUERY_SUPPORT__ICON ? 1 : 0) +
											(PT4_QUERY_SUPPORT__NB_HANDS ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__VPIP ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__PFR ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__RFI ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__WTSD ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__WSSD ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__PREFLOP_AGGR_FACTOR ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__FLOP_AGGR_FACTOR ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__TURN_AGGR_FACTOR ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__RIVER_AGGR_FACTOR ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__OVERALL_AGGR_FACTOR ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__AGGR_FACTOR_NO_PREFLOP ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__SEEN_FLOP_PCT ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__SEEN_TURN_PCT ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__SEEN_RIVER_PCT ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__OVERALL_FOLD_TO_3B ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__OVERALL_4B ? 1 : 0) + 
											//PREFLOP OTHER STATS
											(PT4_QUERY_SUPPORT__PREFLOP_3B ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__PREFLOP_FOLD_TO_3B ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__PREFLOP_CALLED_RAISE ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__STEAL_ATTEMPT ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__FOLD_BB_TO_STEAL ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__FOLD_SB_TO_STEAL ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__3B_VS_STEAL ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__BB_3B_VS_STEAL ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__SB_3B_VS_STEAL ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__PREFLOP_4B ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__PREFLOP_FOLD_TO_4B ? 1 : 0) + 
											//FLOP
											(PT4_QUERY_SUPPORT__FLOP_CBET ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__FLOP_FOLD_TO_CBET ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__RAISE_FLOP_CBET ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__FLOP_FOLD_TO_3B ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__FLOP_CHECK_RAISE ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__FLOP_DONK ? 1 : 0) + 
											//TURN
											(PT4_QUERY_SUPPORT__TURN_CBET ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__TURN_FOLD_TO_CBET ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__TURN_FOLD_TO_3B ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__TURN_CHECK_RAISE ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__TURN_CHECK_CALL ? 1 : 0) + 
											//RIVER
											(PT4_QUERY_SUPPORT__RIVER_FOLD_TO_3B ? 1 : 0) + 
											(PT4_QUERY_SUPPORT__RIVER_FOLD_TO_CBET ? 1 : 0)+
											(PT4_QUERY_SUPPORT__RIVER_BET ? 1 : 0);

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

// PokerTracker naming conventions: 
//   http://www.pokertracker.com/guides/PT3/tutorials/using-custom-reports-and-statistics#Naming-conventions

t_QueryDefinition query_definitions[k_number_of_pokertracker_stats] =
{
//TODO PT4!
#if PT4_QUERY_SUPPORT__ICON
	/* PT4 query to get icon */
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
// TESTED !
#if PT4_QUERY_SUPPORT__NB_HANDS
	/* PT4 query to get number of hands in the database */
	{
		// name
		"hands",	
		// description_for_editor
		"Poker Tracker number of hands that are in the database",
		// query
		"SELECT COUNT(*) as result \
		 FROM   player as P, %TYPE%_hand_player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
				S.id_gametype = %GAMETYPE% AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_basic
	},
#endif
// TESTED !
#if PT4_QUERY_SUPPORT__VPIP
	/* PT4 query to get vpip */
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
		FROM   player as P, %TYPE%_hand_player_statistics as S \
		WHERE  S.id_player = P.id_player AND \
			   S.id_gametype = %GAMETYPE% AND \
		       P.id_site = %SITEID% AND \
		       P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_basic
	},
#endif
// TESTED !
#if PT4_QUERY_SUPPORT__PFR
	/* PT4 query to get pre flop raise */
	{
		// name
		"pfr",			
		// description_for_editor
		"Poker Tracker pre-flop raise ",
		// query
		"SELECT (case when (count(*) = 0) \
		        then -1 \
				else (cast(sum(case when cnt_p_raise > 0 then 1 else 0 end) as real) / count(*)) \
				end) as result \
		 FROM   player as P, %TYPE%_hand_player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
		  	    S.id_gametype = %GAMETYPE% AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_basic
	},
#endif
#if PT4_QUERY_SUPPORT__RFI
	/* PT4 query to get pre-flop raise first*/
	{
		// name
		"preflop_raise_first",		
		// description_for_editor
		"Poker Tracker pre-flop raise first",
		// query
		"SELECT (case when count(*) = 0 then -1 else \
		        cast(sum(case when flg_p_first_raise then 1 else 0 end) as real) / count(*) end) as result \
		 FROM   player as P, %TYPE%_hand_player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
		 	    S.flg_p_open_opp AND \
				S.id_gametype = %GAMETYPE% AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_basic
	},
#endif
	
#if PT4_QUERY_SUPPORT__WTSD
	/* PT4 query to get Went to SD */
	{
		// name
		"wsdp",			
		// description_for_editor
		"Poker Tracker went to showdown",
		// query
		"SELECT (case when sum(case when flg_f_saw then 1 else 0 end) = 0 then -1 else \
		        cast(sum(case when flg_showdown then 1 else 0 end) as real) \
				/ sum(case when flg_f_saw then 1 else 0 end) end) as result \
		 FROM   player as P, %TYPE%_hand_player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
				S.id_gametype = %GAMETYPE% AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_basic
	},
#endif
#if PT4_QUERY_SUPPORT__WSSD
	/* PT4 query to get Won $ At SD */
	{
		// name
		"wssd",			
		// description_for_editor
		"Poker Tracker won $ at showdown",
		// query
		"SELECT (case when count(*) = 0 then -1 else \
		        cast(sum(case when flg_won_hand then 1 else 0 end) as real) \
				/ count(*) end) as result \
		 FROM   player as P, %TYPE%_hand_player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
				flg_showdown AND \
				S.id_gametype = %GAMETYPE% AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_basic
	},
#endif
#if PT4_QUERY_SUPPORT__PREFLOP_AGGR_FACTOR
	/* PT4 query to get pre-flop aggression factor */
	{
		// name
		"preflop_aggr_factor",
		// description_for_editor
		"Poker Tracker preflop aggression",
		// query
		"SELECT (case when sum(S.cnt_p_call) = 0 \
		        then -1 \
				else cast(sum(S.cnt_p_raise) as real) / sum(S.cnt_p_call) \
				end) as result \
		 FROM   player as P, %TYPE%_hand_player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
				S.id_gametype = %GAMETYPE% AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_basic
	},
#endif
#if PT4_QUERY_SUPPORT__FLOP_AGGR_FACTOR
	/* PT4 query to get flop aggression factor */
	{
		// name
		"flop_aggr_factor",
		// description_for_editor
		"Poker Tracker flop aggression",
		// query
		"SELECT (case when sum(S.cnt_f_call) = 0 \
		        then -1 \
				else cast((sum(S.cnt_f_raise) + sum(case when S.flg_f_bet then 1 else 0 end)) as real) \
				/ sum(S.cnt_f_call) \
				end) as result \
		 FROM   player as P, %TYPE%_hand_player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
				S.id_gametype = %GAMETYPE% AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT4_QUERY_SUPPORT__TURN_AGGR_FACTOR
	/* PT4 query to get turn aggression factor */
	{
		// name
		"turn_aggr_factor",
		// description_for_editor
		"Poker Tracker turn aggression",
		// query
		"SELECT (case when sum(S.cnt_t_call) = 0 \
		        then -1 \
				else cast((sum(S.cnt_t_raise) + sum(case when S.flg_t_bet then 1 else 0 end)) as real) \
				/ sum(S.cnt_t_call) \
				end) as result \
		 FROM   player as P, %TYPE%_hand_player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
				S.id_gametype = %GAMETYPE% AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT4_QUERY_SUPPORT__RIVER_AGGR_FACTOR
	/* PT4 query to get river aggression factor */
	{
		// name
		"river_aggr_factor",			
		// description_for_editor
		"Poker Tracker river aggression",
		// query
		"SELECT (case when sum(S.cnt_r_call) = 0 \
		        then -1 \
				else cast((sum(S.cnt_r_raise) + sum(case when S.flg_r_bet then 1 else 0 end)) as real) \
				/ sum(S.cnt_r_call) \
				end) as result \
		 FROM   player as P, %TYPE%_hand_player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
				S.id_gametype = %GAMETYPE% AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT4_QUERY_SUPPORT__OVERALL_AGGR_FACTOR
	/* PT4 query to get total aggression factor */
	{
		// name
		"aggr_factor", 
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
			   FROM   player as P, %TYPE%_hand_player_statistics as S \
			   WHERE  S.id_player = P.id_player AND \
					  S.id_gametype = %GAMETYPE% AND \
		              P.id_site = %SITEID% AND \
		              P.player_name LIKE '%SCREENNAME%') foo",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT4_QUERY_SUPPORT__AGGR_FACTOR_NO_PREFLOP
	/* PT4 query to get total aggression factor without preflop */
	{
		// name
		"aggr_factor_without_preflop",	
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
			   FROM   player as P, %TYPE%_hand_player_statistics as S \
			   WHERE  S.id_player = P.id_player AND \
					  S.id_gametype = %GAMETYPE% AND \
		              P.id_site = %SITEID% AND \
		              P.player_name LIKE '%SCREENNAME%') foo",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT4_QUERY_SUPPORT__SEEN_FLOP_PCT
	/* PT4 query to get flops seen */
	{
		// name
		"flop_seen",		
		// description_for_editor
		"Poker Tracker saw flop",
		// query
		"SELECT (case when (count(*) = 0) \
		         then -1 \
				 else cast(sum(case when flg_f_saw then 1 else 0 end) as real) / count(*) \
				 end) as result \
		FROM   player as P, %TYPE%_hand_player_statistics as S \
		WHERE  S.id_player = P.id_player AND \
			   S.id_gametype = %GAMETYPE% AND \
		       P.id_site = %SITEID% AND \
		       P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT4_QUERY_SUPPORT__SEEN_TURN_PCT
	/* PT4 query to get turns seen */
	{
		// name
		"turn_seen",		
		// description_for_editor
		"Poker Tracker saw turn",
		// query
		"SELECT (case when (count(*) = 0) \
		         then -1 \
				 else cast(sum(case when flg_t_saw then 1 else 0 end) as real) / count(*) \
				 end) as result \
		FROM   player as P, %TYPE%_hand_player_statistics as S \
		WHERE  S.id_player = P.id_player AND \
			   S.id_gametype = %GAMETYPE% AND \
		       P.id_site = %SITEID% AND \
		       P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT4_QUERY_SUPPORT__SEEN_RIVER_PCT
	/* PT4 query to get rivers seen */
	{
		// name
		"river_seen", 
		// description_for_editor
		"Poker Tracker saw river",
		// query
		"SELECT (case when (count(*) = 0) \
		         then -1 \
				 else cast(sum(case when flg_r_saw then 1 else 0 end) as real) / count(*) \
				 end) as result \
		FROM   player as P, %TYPE%_hand_player_statistics as S \
		WHERE  S.id_player = P.id_player AND \
			   S.id_gametype = %GAMETYPE% AND \
		       P.id_site = %SITEID% AND \
		       P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT4_QUERY_SUPPORT__OVERALL_FOLD_TO_3B
	/* PT4  query to get TOTAL fold to 3 bet */
	{
		// name
		"fold_to_3bet",		
		// description_for_editor
		"Poker Tracker folded while facing 3bet total",
		// query
		"SELECT (case when sum(case when flg_p_3bet_def_opp or flg_f_3bet_def_opp or flg_t_3bet_def_opp or flg_r_3bet_def_opp then 1 else 0 end) = 0 then -1 else \
		        cast(sum(case when not enum_p_3bet_action = 'F' or enum_f_3bet_action = 'F' or enum_t_3bet_action ='F' or enum_r_3bet_action = 'F' then 1 else 0 end) as real) \
				/ sum(case when flg_p_3bet_def_opp or flg_f_3bet_def_opp or flg_t_3bet_def_opp or flg_r_3bet_def_opp then 1 else 0 end) end) as result \
		 FROM   player as P, %TYPE%_hand_player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
				S.id_gametype = %GAMETYPE% AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT4_QUERY_SUPPORT__OVERALL_4B
	/* PT4 query to get Overall 4B */
	{
		// name
		"4bet",
		// description_for_editor
		"Poker Tracker Overall 4B",
		// query
		"SELECT (case when count(*) = 0 then -1 else \
			cast(sum(case when (flg_p_4bet) then 1 else 0 end) as real) / count(*) end) as result \
        FROM   player as P, %TYPE%_hand_player_statistics as S \
        WHERE  S.id_player = P.id_player AND \
			S.id_gametype = %GAMETYPE% AND \
            P.id_site = %SITEID% AND \
            P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_positional
	},
#endif
#if PT4_QUERY_SUPPORT__PREFLOP_3B
	/* PT4  query to get PREFLOP 3 bet */
	{
		// name
		"preflop_3bet",	
		// description_for_editor
		"Poker Tracker 3bet preflop",
		// query
		"SELECT (case when sum(case when flg_p_3bet_opp then 1 else 0 end) = 0 then -1 else \
				cast(sum(case when flg_p_3bet then 1 else 0 end) as real) \
				/ sum(case when flg_p_3bet_opp then 1 else 0 end) end) as result \
		FROM  	player as P, %TYPE%_hand_player_statistics as S \
		WHERE 	S.id_player = P.id_player AND \
				S.id_gametype = %GAMETYPE% AND \
				P.id_site = %SITEID% AND \
				P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT4_QUERY_SUPPORT__PREFLOP_FOLD_TO_3B
	/* PT4  query to get PREFLOP fold to 3 bet */
	{
		// name
		"preflop_fold_to_3bet",	
		// description_for_editor
		"Poker Tracker folded while facing 3bet preflop",
		// query
		"SELECT (case when sum(case when flg_p_3bet_def_opp then 1 else 0 end) = 0 then -1 else \
		        cast(sum(case when enum_p_3bet_action = 'F' then 1 else 0 end) as real) \
				/ sum(case when flg_p_3bet_def_opp then 1 else 0 end) end) as result \
		 FROM   player as P, %TYPE%_hand_player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
				S.id_gametype = %GAMETYPE% AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT4_QUERY_SUPPORT__PREFLOP_CALLED_RAISE
	/* PT4 query to get pre-flop called raise */
	{
		// name
		"preflop_called_raise",		
		// description_for_editor
		"Poker Tracker pre-flop called raise",
		// query
		"SELECT (case when count(*) = 0 then -1 else \
		        cast(sum(case when not flg_p_fold AND cnt_p_raise = 0 then 1 else 0 end) as real) / count(*) end) as result \
		 FROM   player as P, %TYPE%_hand_player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
		 	    S.flg_p_face_raise AND \
				S.id_gametype = %GAMETYPE% AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT4_QUERY_SUPPORT__STEAL_ATTEMPT
	/* PT4 query to get attempted steal */
	{
		// name
		"preflop_attempt_steal",	
		// description_for_editor
		"Poker Tracker attempt to steal blinds",
		// query
		"SELECT (case when sum(case when flg_steal_opp then 1 else 0 end) = 0 then -1 else \
		        cast(sum(case when flg_steal_att then 1 else 0 end) as real) \
				/ sum(case when flg_steal_opp then 1 else 0 end) end) as result \
		 FROM   player as P, %TYPE%_hand_player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
				S.id_gametype = %GAMETYPE% AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT4_QUERY_SUPPORT__FOLD_BB_TO_STEAL
	/* PT4 query to get Folded Big Blind to steal */
	{
		// name
		"big_blind_fold_to_steal",		
		// description_for_editor
		"Poker Tracker folded big blind to steal",
		// query
		"SELECT (case when sum(case when flg_blind_def_opp AND flg_blind_b then 1 else 0 end) = 0 then -1 else \
		        cast(sum(case when flg_bb_steal_fold then 1 else 0 end) as real) \
				/ sum(case when flg_blind_def_opp AND flg_blind_b then 1 else 0 end) end) as result \
		 FROM   player as P, %TYPE%_hand_player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
				S.id_gametype = %GAMETYPE% AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_positional
	},
#endif
#if PT4_QUERY_SUPPORT__FOLD_SB_TO_STEAL
	/* PT4 query to get Folded Small Blind to steal */
	{
		// name
		"small_blind_fold_to_steal",
		// description_for_editor
		"Poker Tracker folded small blind to steal",
		// query
		"SELECT (case when sum(case when flg_blind_def_opp AND flg_blind_s then 1 else 0 end) = 0 then -1 else \
		        cast(sum(case when flg_sb_steal_fold then 1 else 0 end) as real) \
				/ sum(case when flg_blind_def_opp AND flg_blind_s then 1 else 0 end) end) as result \
		 FROM   player as P, %TYPE%_hand_player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
				S.id_gametype = %GAMETYPE% AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_positional
	},
#endif
#if PT4_QUERY_SUPPORT__3B_VS_STEAL
	/* PT4 query to get 3B vs. steal */
	{
		// name
		"preflop_3bet_vs_steal",
		// description_for_editor
		"Poker Tracker 3bet vs. steal",
		// query
		"SELECT (case when count(*) = 0 then -1 else \
			cast(sum(case when (flg_blind_def_opp AND flg_blind_b AND flg_p_3bet) then 1 else 0 end) as real) / count(*) end) as result \
		FROM   player as P, %TYPE%_hand_player_statistics as S \
		WHERE  S.id_player = P.id_player AND \
			S.id_gametype = %GAMETYPE% AND \
            P.id_site = %SITEID% AND \
            P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_positional
	},
#endif
#if PT4_QUERY_SUPPORT__BB_3B_VS_STEAL
	/* PT4 query to get BB 3B vs. steal */
	{
		// name
		"big_blind_3bet_vs_steal",
		// description_for_editor
		"Poker Tracker BB 3bet vs. steal",
		// query
		"SELECT (cast (sum(case when ((flg_blind_b=TRUE) AND (flg_blind_def_opp=TRUE) AND (cnt_p_raise>0)) then 1 else 0 end) as real)/ \
			cast (sum(case when ((flg_blind_b=TRUE) AND (flg_blind_def_opp=TRUE)) then 1 else 0 end) as real)) as result \
		FROM   player as P, %TYPE%_hand_player_statistics as S \
		WHERE  S.id_player = P.id_player AND \
			S.id_gametype = %GAMETYPE% AND \
            P.id_site = %SITEID% AND \
            P.player_name LIKE '%SCREENNAME%'",
      // stat_group
      pt_group_positional
	},
#endif
	#if PT4_QUERY_SUPPORT__SB_3B_VS_STEAL
	/* PT4 query to get SB 3B vs. steal */
	{
		// name
		"small_blind_3bet_vs_steal",
		// description_for_editor
		"Poker Tracker SB 3bet vs. steal",
		// query
		"SELECT (cast (sum(case when ((flg_blind_s=TRUE) AND (flg_blind_def_opp=TRUE) AND (cnt_p_raise>0)) then 1 else 0 end) as real)/ \
			 cast (sum(case when ((flg_blind_s=TRUE) AND (flg_blind_def_opp=TRUE)) then 1 else 0 end) as real)) as result \
		FROM   player as P, %TYPE%_hand_player_statistics as S \
		WHERE  S.id_player = P.id_player AND \
			S.id_gametype = %GAMETYPE% AND \
            P.id_site = %SITEID% AND \
            P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_positional
	},
#endif
#if PT4_QUERY_SUPPORT__PREFLOP_4B
	/* PT4 query to get Preflop 4B */
	{
		// name
		"preflop_4bet",
		// description_for_editor
		"Poker Tracker Preflop 4B",
		// query
		"SELECT (cast (sum(case when (flg_p_4bet=TRUE) then 1 else 0 end) as real)/ \
			cast (sum(case when (flg_p_4bet_opp=TRUE) then 1 else 0 end) as real)) as result \
        FROM   player as P, %TYPE%_hand_player_statistics as S \
        WHERE  S.id_player = P.id_player AND \
			S.id_gametype = %GAMETYPE% AND \
            P.id_site = %SITEID% AND \
            P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_positional
	},
#endif
#if PT4_QUERY_SUPPORT__PREFLOP_FOLD_TO_4B
	/* PT4 query to get Preflop fold to 4B */
	{
		// name
		"preflop_fold_to_4bet",
		// description_for_editor
		"Poker Tracker Preflop fold to 4B",
		// query
		"SELECT (cast (sum(case when (enum_p_4bet_action='F') then 1 else 0 end) as real)/ \
			cast (sum(case when (flg_p_4bet_def_opp=TRUE) then 1 else 0 end) as real)) as result \
        FROM   player as P, %TYPE%_hand_player_statistics as S \
        WHERE  S.id_player = P.id_player AND \
			S.id_gametype = %GAMETYPE% AND \
            P.id_site = %SITEID% AND \
            P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_positional
	},
#endif
#if PT4_QUERY_SUPPORT__FLOP_CBET
	/* PT4  query to get cbet on FLOP */
	{
		// name
		"flop_cbet",		
		// description_for_editor
		"Poker Tracker cbet",
		// query
		"SELECT (case when sum(case when flg_f_cbet_opp then 1 else 0 end) = 0 then -1 else \
		        cast(sum(case when flg_f_cbet then 1 else 0 end) as real) \
				/ sum(case when flg_f_cbet_opp then 1 else 0 end) end) as result \
		 FROM   player as P, %TYPE%_hand_player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
				S.id_gametype = %GAMETYPE% AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT4_QUERY_SUPPORT__FLOP_FOLD_TO_CBET
	/* PT4  query to get FLOP fold to cbet */
	{
		// name
		"flop_fold_to_cbet",	
		// description_for_editor
		"Poker Tracker folded while facing cbet flop",
		// query
		"SELECT (case when sum(case when flg_f_cbet_def_opp then 1 else 0 end) = 0 then -1 else \
		        cast(sum(case when enum_f_cbet_action = 'F' then 1 else 0 end) as real) \
				/ sum(case when flg_f_cbet_def_opp then 1 else 0 end) end) as result \
		 FROM   player as P, %TYPE%_hand_player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
				S.id_gametype = %GAMETYPE% AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT4_QUERY_SUPPORT__RAISE_FLOP_CBET
	/* PT4  query to get raise cbet on FLOP */
	{
		// name
		"flop_raise_cbet",		
		// description_for_editor
		"Poker Tracker raise flop cbet",
		// query
		"select (case when (count(*)!=0) then \
		cast(sum(case when S.enum_f_cbet_action = 'R' then 1 else 0 end) as real) / \
		cast(sum(case when S.flg_f_cbet_def_opp = 't' then 1 else 0 end) as real) \
		else (-1) end) as result \
		FROM   player as P,  %TYPE%_hand_player_statistics as S \
		WHERE  S.id_player = P.id_player AND \
			   S.id_gametype = %GAMETYPE% AND \
			   P.player_name LIKE '%SCREENNAME%'AND \
			   P.id_site=%SITEID%",
		pt_group_advanced
	},
#endif
#if PT4_QUERY_SUPPORT__FLOP_FOLD_TO_3B
	/* PT4  query to get FLOP fold to 3 bet */
	{
		// name
		"flop_fold_to_3bet",
		// description_for_editor
		"Poker Tracker folded while facing 3bet flop",
		// query
		"SELECT (case when sum(case when flg_f_3bet_def_opp then 1 else 0 end) = 0 then -1 else \
		        cast(sum(case when enum_f_3bet_action = 'F' then 1 else 0 end) as real) \
				/ sum(case when flg_f_3bet_def_opp then 1 else 0 end) end) as result \
		 FROM   player as P, %TYPE%_hand_player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
				S.id_gametype = %GAMETYPE% AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT4_QUERY_SUPPORT__FLOP_CHECK_RAISE
	/* PT4  query to get flop check-raise */
	{
		// name
		"flop_checkraise",		
		// description_for_editor
		"Poker Tracker flop check-raise",
		// query
		"SELECT (cast (sum(case when (flg_f_check_raise=TRUE) then 1 else 0 end) as real)/ \
			cast (sum (case when ((flg_f_check=TRUE) AND ((cnt_f_raise>0) OR (cnt_f_call>0) OR (flg_f_fold=TRUE))) then 1 else 0 end) as real)) as result \
		FROM   player as P, %TYPE%_hand_player_statistics as S \
		WHERE  S.id_player = P.id_player AND \
				S.id_gametype = %GAMETYPE% AND \
                P.id_site = %SITEID% AND \
                P.player_name LIKE '%SCREENNAME%'",
		pt_group_advanced
	},
#endif
#if PT4_QUERY_SUPPORT__FLOP_DONK
	/* PT4  query to get FLOP donk */
	{
		// name
		"flop_donkbet",
		// description_for_editor
		"Poker Tracker donk flop",
		// query
		"select (case when  (count(*)!=0)  then \
			(cast(sum(case when (S.flg_p_face_raise AND NOT(S.flg_p_3bet OR S.flg_p_4bet) AND S.flg_f_bet AND NOT(S.flg_f_cbet_opp) AND \
			((HSum.cnt_players > 2 and S.val_p_raise_aggressor_pos < S.position) or (HSum.cnt_players = 2 and S.flg_blind_b))) then 1 else 0 end) as real)/   \
			 cast(sum(case when (S.flg_p_face_raise AND NOT(S.flg_p_3bet OR S.flg_p_4bet) AND (S.flg_f_open_opp) AND NOT(S.flg_f_cbet_opp) AND \
			((HSum.cnt_players > 2 and S.val_p_raise_aggressor_pos < S.position) or (HSum.cnt_players = 2 and S.flg_blind_b))) then 1 else 0 end) as real)) \
			 else (0) end) as result \
			FROM  player as P, %TYPE%_hand_player_statistics as S, %TYPE%_hand_summary as HSum\
		WHERE S.id_player = P.id_player AND \
			HSum.id_hand = S.id_hand AND \
			S.id_gametype = %GAMETYPE% AND \
			P.player_name like '%SCREENNAME%' AND \
			P.id_site=%SITEID%",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT4_QUERY_SUPPORT__TURN_CBET
	/* PT4  query to get cbet on TURN */
	{
		// name
		"turn_cbet",		
		// description_for_editor
		"Poker Tracker turn cbet",
		// query
		"select (case when   (sum(case when flg_t_cbet_opp then 1 else 0 end) != 0)  then \
         cast(sum(case when S.flg_t_cbet = 't' then 1 else 0 end) as real) / \
		 cast(sum(case when S.flg_t_cbet_opp = 't' then 1 else 0 end) as real) \
		 else (0) end) as result \
		 FROM player as P, %TYPE%_hand_player_statistics as S \
		 WHERE S.id_player = P.id_player AND \
				S.id_gametype = %GAMETYPE% AND \
				P.player_name like '%SCREENNAME%' AND \
				P.id_site=%SITEID%",
		pt_group_advanced
	},
#endif
#if PT4_QUERY_SUPPORT__TURN_FOLD_TO_CBET
	/* PT4  query to get TURN fold to cbet */
	{
		// name
		"turn_fold_to_cbet",
		// description_for_editor
		"Poker Tracker folded while facing cbet turn",
		// query
		"SELECT (case when sum(case when flg_t_cbet_def_opp then 1 else 0 end) = 0 then -1 else \
		        cast(sum(case when enum_t_cbet_action = 'F' then 1 else 0 end) as real) \
				/ sum(case when flg_t_cbet_def_opp then 1 else 0 end) end) as result \
		 FROM   player as P, %TYPE%_hand_player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
				S.id_gametype = %GAMETYPE% AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT4_QUERY_SUPPORT__TURN_FOLD_TO_3B
	/* PT4  query to get TURN fold to 3 bet */
	{
		// name
		"turn_fold_to_3bet",	
		// description_for_editor
		"Poker Tracker folded while facing 3bet turn",
		// query
		"SELECT (case when sum(case when flg_t_3bet_def_opp then 1 else 0 end) = 0 then -1 else \
		        cast(sum(case when enum_t_3bet_action = 'F' then 1 else 0 end) as real) \
				/ sum(case when flg_t_3bet_def_opp then 1 else 0 end) end) as result \
		 FROM   player as P, %TYPE%_hand_player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
				S.id_gametype = %GAMETYPE% AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT4_QUERY_SUPPORT__TURN_CHECK_RAISE
	/* PT4  query to get TURN Check-Raise */
	{
		// name
		"turn_checkraise",	
		// description_for_editor
		"Poker Tracker turn Check-Raise",
		// query
		"select (case when  (count(*)!=0)  then \
			 cast(sum(case when S.flg_t_check_raise = 't' then 1 else 0 end) as real) / \
			cast(sum(case when (S.flg_t_check= 't' AND (S.cnt_t_raise>0 OR S.cnt_t_call>0 OR S.flg_t_fold= 't')) then 1 else 0 end) as real) \
			else (0) end) as result \
		FROM player as P, %TYPE%_hand_player_statistics as S \
		WHERE S.id_player = P.id_player AND \
				S.id_gametype = %GAMETYPE% AND \
				P.player_name like '%SCREENNAME%' AND \
				P.id_site=%SITEID%",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT4_QUERY_SUPPORT__TURN_CHECK_CALL
	/* PT4  query to get TURN Check-Call */
	{
		// name
		"turn_checkcall",	
		// description_for_editor
		"Poker Tracker turn Check-Call",
		// query
		"select (case when  (count(*)!=0)  then \
			cast(sum(case when (S.flg_t_check AND S.cnt_t_call>0) then 1 else 0 end) as real) / \
			cast(sum(case when (S.flg_t_check= 't' AND (S.cnt_t_raise>0 OR S.cnt_t_call>0 OR S.flg_t_fold= 't')) then 1 else 0 end) as real) \
			else (0) end) as result \
		FROM player as P, %TYPE%_hand_player_statistics as S \
		WHERE S.id_player = P.id_player AND \
				S.id_gametype = %GAMETYPE% AND \
				P.player_name like '%SCREENNAME%' AND \
				P.id_site=%SITEID%",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT4_QUERY_SUPPORT__RIVER_FOLD_TO_3B
	/* PT4  query to get RIVER fold to 3 bet */
	{
		// name
		"river_fold_to_3bet",	
		// description_for_editor
		"Poker Tracker folded while facing 3bet river",
		// query
		"SELECT (case when sum(case when flg_r_3bet_def_opp then 1 else 0 end) = 0 then -1 else \
		        cast(sum(case when enum_r_3bet_action = 'F' then 1 else 0 end) as real) \
				/ sum(case when flg_r_3bet_def_opp then 1 else 0 end) end) as result \
		 FROM   player as P, %TYPE%_hand_player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
				S.id_gametype = %GAMETYPE% AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT4_QUERY_SUPPORT__RIVER_FOLD_TO_CBET
	/* PT4  query to get RIVER fold to cbet */
	{
		// name
		"river_fold_to_cbet",
		// description_for_editor
		"Poker Tracker folded while facing cbet river",
		// query
		"SELECT (case when sum(case when flg_r_cbet_def_opp then 1 else 0 end) = 0 then -1 else \
		        cast(sum(case when enum_r_cbet_action = 'F' then 1 else 0 end) as real) \
				/ sum(case when flg_r_cbet_def_opp then 1 else 0 end) end) as result \
		 FROM   player as P, %TYPE%_hand_player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
				S.id_gametype = %GAMETYPE% AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
#if PT4_QUERY_SUPPORT__RIVER_BET
	/* PT4  query to get River bet */
	{
		// name
		"river_bet",
		// description_for_editor
		"Poker Tracker river bet",
		// query
		"SELECT (case when sum(case when flg_r_cbet_def_opp then 1 else 0 end) = 0 then -1 else \
		        cast(sum(case when enum_r_cbet_action = 'F' then 1 else 0 end) as real) \
				/ sum(case when flg_r_cbet_def_opp then 1 else 0 end) end) as result \
		 FROM   player as P, %TYPE%_hand_player_statistics as S \
		 WHERE  S.id_player = P.id_player AND \
				S.id_gametype = %GAMETYPE% AND \
		        P.id_site = %SITEID% AND \
		        P.player_name LIKE '%SCREENNAME%'",
		// stat_group
		pt_group_advanced
	},
#endif
};

#endif // INC_POKERTRACKER_QUERIES_VERSION_3_H