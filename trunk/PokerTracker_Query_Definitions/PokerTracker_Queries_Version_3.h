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

const int k_number_of_pokertracker_stats = 29;

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
	CString first_part_of_query;
	// The queries will be created on the fly,
	// for both ring-games and tournaments.
	// There will be an "infix" added, depending on game-type
	CString last_part_of_query;
	int stat_group;
	bool needs_infix_and_second_part;
};


t_QueryDefinition query_definitions[k_number_of_pokertracker_stats] =
{
	/* PT3 query to get icon */
	{
		// name
		"icon",			
		// description_for_editor
		"Poker Tracker auto-rate icon code",
		// first_part_of_query
		"SELECT val_icon \
		FROM player \
		WHERE id_site=%SITEID% AND \
		player_name = '%SCREENNAME%'",
		// last_part_of_query
		"",
		// stat_group
		pt_group_basic,
		// needs_infix_and_second_part
		false
	},
	/* PT3 query to get number of hands in the database */
	{
		// name
		"hands",		
		// description_for_editor
		"Poker Tracker number of hands that are in the database",
		// first_part_of_query
		"SELECT COUNT(*) as result FROM ",	
		// last_part_of_query
		"hand_player_detail as D, player as P",
		// stat_group
		pt_group_basic,
		// needs_infix_and_second_part
		true
	},
	/* PT3 query to get pre flop raise % */
	{
		// name
		"pfr",			
		// description_for_editor
		"Poker Tracker pre-flop raise percentage",
		// first_part_of_query
		"SELECT (case when (count(*)!=0) \
              then ( cast(sum(case when cnt_p_raise >0 then 1 else 0 end) as real) / count(*)) \
              else (-1) \
			end) as result \
		FROM   player as P,", 
		// last_part_of_query
		"hand_player_statistics as S",
		// stat_group
		pt_group_basic,
		// needs_infix_and_second_part
		true
	},
	/* PT3 query to get pre-flop aggression factor */
	{
		// name
		"aggp",
		// description_for_editor
		"Poker Tracker preflop aggression",
		// first_part_of_query
		"SELECT (case when (count(*)!=0) \
              then ( cast(sum(cnt_p_raise) as real) / cast(sum(cnt_p_call) as real)) \
              else (-1) \
			end) as result \
		FROM   player as P,", 
		// last_part_of_query
		"holdem_hand_player_statistics as S",
		// stat_group
		pt_group_basic,
		// needs_infix_and_second_part
		true
	},
	/* PT3 query to get flop aggression factor */
	{
		// name
		"aggf",			
		// description_for_editor
		"Poker Tracker flop aggression",
		// first_part_of_query
		"SELECT (case when (count(*)!=0) \
              then ( (cast(sum(cnt_f_raise) as real) + sum(case when flg_f_bet then 1 else 0 end)) / cast(sum(cnt_f_call) as real)) \
              else (-1) \
			end) as result \
		FROM   player as P,", 
		// last_part_of_query
		"hand_player_statistics as S",
		// stat_group
		pt_group_advanced,
		// needs_infix_and_second_part
		true
	},
	/* PT3 query to get turn aggression factor */
	{
		// name
		"aggt",			
		// description_for_editor
		"Poker Tracker turn aggression",
		// first_part_of_query
		"SELECT (case when (count(*)!=0) \
              then ( (cast(sum(cnt_t_raise) as real) + sum(case when flg_t_bet then 1 else 0 end)) / cast(sum(cnt_t_call) as real)) \
              else (-1) \
			end) as result \
		FROM   player as P,",
		// last_part_of_query
		"hand_player_statistics as S",
		// stat_group
		pt_group_advanced,
		// needs_infix_and_second_part
		true,
	},
	/* PT3 query to get river aggression factor */
	{
		// name
		"aggr",			
		// description_for_editor
		"Poker Tracker river aggression",
		// first_part_of_query
		"SELECT (case when (count(*)!=0) \
              then ( (cast(sum(cnt_r_raise) as real) + sum(case when flg_r_bet then 1 else 0 end)) / cast(sum(cnt_r_call) as real)) \
              else (-1) \
			end) as result \
		FROM   player as P,",
		// last_part_of_query
		"hand_player_statistics as S",
		// stat_group
		pt_group_advanced,
		// needs_infix_and_second_part
		true,
	},
	/* PT3 query to get total aggression factor */
	{
		// name
		"aggtot", 
		// description_for_editor
		"Poker Tracker total aggression",
		// first_part_of_query
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
		FROM   player as P,", 
		// last_part_of_query
		"hand_player_statistics as S",
		// stat_group
		pt_group_advanced,
		// needs_infix_and_second_part
		true,
	},
	/* PT3 query to get total aggression factor without preflop */
	{
		// name
		"aggtotnopf",	
		// description_for_editor
		"Poker Tracker total aggression excluding preflop",
		// first_part_of_query
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
		FROM   player as P,",
		// last_part_of_query
		 "hand_player_statistics as S",
		// stat_group
		pt_group_advanced,
		// needs_infix_and_second_part
		true,
	},
	/* PT3 query to get percentage of flops seen */
	{
		// name
		"floppct",		
		// description_for_editor
		"Poker Tracker saw flop percentage",
		// first_part_of_query
		"SELECT (case when (count(*)!=0) \
             then cast(sum(case when flg_f_saw then 1 else 0 end) as real) / count(*) \
             else (-1) \
        end) as result \
		FROM   player as P,", 
		// last_part_of_query
		"hand_player_statistics as S",
		// stat_group
		pt_group_advanced,
		// needs_infix_and_second_part
		true,
	},
	/* PT3 query to get percentage of turns seen */
	{
		// name
		"turnpct",		
		// description_for_editor
		"Poker Tracker saw turn percentage",
		// first_part_of_query
		"SELECT (case when (count(*)!=0) \
             then cast(sum(case when flg_t_saw then 1 else 0 end) as real) / count(*) \
             else (-1) \
        end) as result \
		FROM   player as P,", 
		// last_part_of_query
		"hand_player_statistics as S",
		// stat_group
		pt_group_advanced,
		// needs_infix_and_second_part
		true,
	},
	/* PT3 query to get percentage of rivers seen */
	{
		// name
		"riverpct", 
		// description_for_editor
		"Poker Tracker saw river percentage",
		// first_part_of_query
		"SELECT (case when (count(*)!=0) \
             then cast(sum(case when flg_r_saw then 1 else 0 end) as real) / count(*) \
             else (-1) \
        end) as result \
		FROM   player as P,", 
		// last_part_of_query
		"hand_player_statistics as S",
		// stat_group
		pt_group_advanced,
		// needs_infix_and_second_part
		true,
	},
	/* PT3 query to get vpip */
	{
		// name
		"vpip",			
		// description_for_editor
		"Poker Tracker VP$IP",
		// first_part_of_query
		"SELECT (case when (count(*)!=0) \
              then cast(sum(case when flg_vpip then 1 else 0 end) as real) / count(*) \
              else (-1) \
         end) as result \
		FROM   player as P,", 
		// last_part_of_query
		"hand_player_statistics as S",
		// stat_group
		pt_group_basic,
		// needs_infix_and_second_part
		true,
	},
	/* PT3 query to get pre-flop raise first in pct */
	{
		// name
		"pf_rfi",		
		// description_for_editor
		"Poker Tracker pre-flop raise first in percentage",
		// first_part_of_query
		"SELECT (case when (count(*)!=0) \
              then cast(sum(case when flg_p_first_raise AND flg_p_open_opp then 1 else 0 end) as real) \
                   / sum(case when flg_p_open_opp then 1 else 0 end) \
              else (-1) \
         end) as result \
		FROM   player as P,",
		// last_part_of_query
		"hand_player_statistics as S",
		// stat_group
		pt_group_basic,
		// needs_infix_and_second_part
		true,
	},
	/* PT3 query to get pre-flop called raise pct */
	{
		// name
		"pf_cr",		
		// description_for_editor
		"Poker Tracker pre-flop called raise percentage",
		// first_part_of_query
		"SELECT (case when (count(*)!=0) \
              then cast(sum(case when flg_p_face_raise AND flg_p_fold = false AND cnt_p_raise=0 then 1 else 0 end) as real) \
                   / sum(case when flg_p_face_raise then 1 else 0 end) \
              else (-1) \
         end) as result \
		FROM   player as P,",
		// last_part_of_query
		 "hand_player_statistics as S",
		// stat_group
		pt_group_advanced,
		// needs_infix_and_second_part
		true,
	},
	/* PT3 query to get attempted steal percentage */
	{
		// name
		"pfats",	
		// description_for_editor
		"Poker Tracker attempt to steal blinds",
		// first_part_of_query
		"SELECT (case when (count(*)!=0) \
              then cast(sum(case when flg_steal_att then 1 else 0 end) as real) \
                   / sum(case when flg_steal_opp then 1 else 0 end) \
              else (-1) \
         end) as result \
		FROM   player as P,",
		// last_part_of_query
		"hand_player_statistics as S",
		// stat_group
		pt_group_advanced,
		// needs_infix_and_second_part
		true,
	},
	/* PT3 query to get Went to SD % */
	{
		// name
		"wsdp",			
		// description_for_editor
		"Poker Tracker went to showdown percentage",
		// first_part_of_query
		"SELECT (case when (count(*)!=0) \
              then cast(sum(case when flg_showdown then 1 else 0 end) as real) \
                   / sum(case when flg_f_saw then 1 else 0 end) \
              else (-1) \
         end) as result \
		FROM   player as P,",
		// last_part_of_query
		"hand_player_statistics as S",
		// stat_group
		pt_group_basic,
		// needs_infix_and_second_part
		true,
	},
	/* PT3 query to get Won $ At SD % */
	{
		// name
		"wssd",			
		// description_for_editor
		"Poker Tracker won $ at showdown percentae",
		// first_part_of_query
		"SELECT (case when (count(*)!=0) \
              then cast(sum(case when flg_showdown AND flg_won_hand then 1 else 0 end) as real) \
                   / sum(case when flg_showdown then 1 else 0 end) \
              else (-1) \
         end) as result \
		FROM   player as P,",
		// last_part_of_query
		"holdem_hand_player_statistics as S",
		// stat_group
		pt_group_basic,
		// needs_infix_and_second_part
		true,
	},
	/* PT3 query to get Folded Big Blind to steal */
	{
		// name
		"fbbts",		
		// description_for_editor
		"Poker Tracker folded big blind to steal",
		// first_part_of_query
		"SELECT (case when (count(*)!=0) \
              then cast(sum(case when flg_bb_steal_fold then 1 else 0 end) as real) \
                   / sum(case when flg_blind_def_opp AND flg_blind_b then 1 else 0 end) \
              else (-1) \
         end) as result \
		FROM   player as P,",
		// last_part_of_query
		"hand_player_statistics as S",
		// stat_group
		pt_group_positional,
		// needs_infix_and_second_part
		true,
	},
	/* PT3 query to get Folded Small Blind to steal */
	{
		// name
		"fsbts",
		// description_for_editor
		"Poker Tracker folded small blind to steal",
		// first_part_of_query
		"SELECT (case when (count(*)!=0) \
              then cast(sum(case when flg_sb_steal_fold then 1 else 0 end) as real) \
                   / sum(case when flg_blind_def_opp AND flg_blind_s then 1 else 0 end) \
              else (-1) \
         end) as result \
		FROM   player as P,",
		// last_part_of_query
		"hand_player_statistics as S",
		// stat_group
		pt_group_positional,
		// needs_infix_and_second_part
		true,
	},
	/* PT3  query to get cbet on FLOP */
	{
		// name
		"cbetflop",		
		// description_for_editor
		"Poker Tracker cbet percentage",
		// first_part_of_query
		"SELECT (case when (count(*)!=0) \
			then cast(sum(case when S.flg_f_cbet = 't' then 1 else 0 end) as real) \
				/ cast(sum(case when S.flg_f_cbet_opp = 't' then 1 else 0 end) as real) \
			else (-1) end) as result \
		FROM	player as P,",
		// last_part_of_query
		"hand_player_statistics as S",
		// stat_group
		pt_group_advanced,
		// needs_infix_and_second_part
		true,
	},
	/* PT3  query to get TOTAL fold to 3 bet percentage */
	{
		// name
		"f3bettot",		
		// description_for_editor
		"Poker Tracker folded while facing 3bet total percentage",
		// first_part_of_query
		"SELECT (case when (count(*)!=0) then \
			(cast(sum(case when (S.enum_p_3bet_action = 'F' or S.enum_f_3bet_action = 'F' or S.enum_t_3bet_action = 'F' or S.enum_r_3bet_action = 'F') then 1 else 0 end) as real) / \
			cast(sum(case when  (S.flg_p_3bet_def_opp = 't' or S.flg_f_3bet_def_opp = 't' or S.flg_t_3bet_def_opp = 't' or S.flg_r_3bet_def_opp = 't') then 1 else 0 end) as real)) \
		else (-1) end) as result \
		FROM player as P,",
		// last_part_of_query
		"hand_player_statistics as S",
		// stat_group
		pt_group_advanced,
		// needs_infix_and_second_part
		true,
	},
	/* PT3  query to get PREFLOP fold to 3 bet percentage */
	{
		// name
		"f3betpflop",	
		// description_for_editor
		"Poker Tracker folded while facing 3bet preflop percentage",
		// first_part_of_query
		"select (case when (count(*)!=0) then \
			cast(sum(case when S.enum_p_3bet_action = 'F' then 1 else 0 end) as real) / \
			cast(sum(case when S.flg_p_3bet_def_opp = 't' then 1 else 0 end) as real) \
		else (-1) end) as result \
		FROM player as P,", 
		// last_part_of_query
		"hand_player_statistics as S",
		// stat_group
		pt_group_advanced,
		// needs_infix_and_second_part
		true,
	},
	/* PT3  query to get FLOP fold to 3 bet percentage */
	{
		// name
		"f3betflop",
		// description_for_editor
		"Poker Tracker folded while facing 3bet flop percentage",
		// first_part_of_query
		"select (case when (count(*)!=0) then \
			cast(sum(case when S.enum_f_3bet_action = 'F' then 1 else 0 end) as real) / \
			cast(sum(case when S.flg_f_3bet_def_opp = 't' then 1 else 0 end) as real) \
		else (-1) end) as result \
		FROM player as P,",
		// last_part_of_query
		"hand_player_statistics as S",
		// stat_group
		pt_group_advanced,
		// needs_infix_and_second_part
		true,
	},
	/* PT3  query to get TURN fold to 3 bet percentage */
	{
		// name
		"f3betturn",	
		// description_for_editor
		"Poker Tracker folded while facing 3bet turn percentage",
		// first_part_of_query
		"select (case when (count(*)!=0) then \
			cast(sum(case when S.enum_t_3bet_action = 'F' then 1 else 0 end) as real) / \
			cast(sum(case when S.flg_t_3bet_def_opp = 't' then 1 else 0 end) as real) \
		else (-1) end) as result \
		FROM player as P,", 
		// last_part_of_query
		"hand_player_statistics as S",
		// stat_group
		pt_group_advanced,
		// needs_infix_and_second_part
		true,
	},
	/* PT3  query to get RIVER fold to 3 bet percentage */
	{
		// name
		"f3betriver",	
		// description_for_editor
		"Poker Tracker folded while facing 3bet river percentage",
		// first_part_of_query
		"select (case when (count(*)!=0) then \
			cast(sum(case when S.enum_r_3bet_action = 'F' then 1 else 0 end) as real) / \
			cast(sum(case when S.flg_r_3bet_def_opp = 't' then 1 else 0 end) as real) \
		else (-1) end) as result \
		FROM player as P,", 
		// last_part_of_query
		"hand_player_statistics as S",
		// stat_group
		pt_group_advanced,
		// needs_infix_and_second_part
		true,
	},
	/* PT3  query to get FLOP fold to cbet percentage */
	{
		// name
		"fcbetflop",	
		// description_for_editor
		"Poker Tracker folded while facing cbet flop percentage",
		// first_part_of_query
		"select (case when (count(*)!=0) then \
			cast(sum(case when S.enum_f_cbet_action = 'F' then 1 else 0 end) as real) / \
			cast(sum(case when S.flg_f_cbet_def_opp = 't' then 1 else 0 end) as real) \
		else (-1) end) as result \
		FROM player as P,",
		// last_part_of_query
		"holdem_hand_player_statistics as S",
		// stat_group
		pt_group_advanced,
		// needs_infix_and_second_part
		true,
	},
	/* PT3  query to get TURN fold to cbet percentage */
	{
		// name
		"fcbetturn",
		// description_for_editor
		"Poker Tracker folded while facing cbet turn percentage",
		// first_part_of_query
		"select (case when (count(*)!=0) then \
			cast(sum(case when S.enum_t_cbet_action = 'F' then 1 else 0 end) as real) / \
			cast(sum(case when S.flg_t_cbet_def_opp = 't' then 1 else 0 end) as real) \
		else (-1) end) as result \
		FROM player as P,",
		// last_part_of_query
		"hand_player_statistics as S",
		// stat_group
		pt_group_advanced,
		// needs_infix_and_second_part
		true,
	},
	/* PT3  query to get RIVER fold to cbet percentage */
	{
		// name
		"fcbetriver",
		// description_for_editor
		"Poker Tracker folded while facing cbet river percentage",
		// first_part_of_query
		"select (case when (count(*)!=0) then \
			cast(sum(case when S.enum_r_cbet_action = 'F' then 1 else 0 end) as real) / \
			cast(sum(case when S.flg_r_cbet_def_opp = 't' then 1 else 0 end) as real) \
		else (-1) end) as result \
		FROM player as P,",
		// last_part_of_query
		"hand_player_statistics as S",
		// stat_group
		pt_group_advanced,
		// needs_infix_and_second_part
		true,
	}
};

#endif // INC_POKERTRACKER_QUERIES_VERSION_3_H