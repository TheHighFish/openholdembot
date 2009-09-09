// Heuristic occlusion-check for rebuy.

#include "stdafx.h"
//#include "Globals.h"
#include "CGameState.h"
#include "COcclusionCheck.h"
#include "CSymbols.h"
//#include "OpenHoldemSymbols.h"
//#include "WhUser.h"

COcclusionCheck::COcclusionCheck()
{}

COcclusionCheck::~COcclusionCheck()
{}

bool COcclusionCheck::UserChairKnown()
{
	return (p_symbols->user_chair_confirmed());
}

bool COcclusionCheck::UserBalanceNonZero()
{
	return true;
	/*
	int userchair = p_symbols->sym()->userchair;
	return (UserChairKnown() && (p_game_state->m_player[userchair].m_balance_known == true)
		&& (p_game_state->m_player[userchair].m_balance > 0.0));
		*/
}

bool COcclusionCheck::UserNameKnown()
{
	return true;
	/*
	int userchair = p_symbols->sym()->userchair;
	return (UserChairKnown() && (p_game_state->m_player[userchair].m_name_known == true)
		&& (p_game_state->m_player[userchair].m_name != ""));
		*/
}

bool COcclusionCheck::AnyOpponentNameKnown()
{
	return true;
	/*
	for (int i=0; i<=9; i++)
	{
		if ((i != userchair) && (p_game_state->m_player[i].m_name_known == true)
			&& (strncmp(p_game_state->m_player[i].m_name, "", 0) != 0))
		{
			return true;
		}
	}
	return false;
	*/
}

bool COcclusionCheck::AnyApponentBalanceNonZero()
{
	return true;
	/*
	for (int i=0; i<=9; i++)
	{
		if ((i != userchair) && (p_game_state->m_player[i].m_balance_known == true)
			&& (p_game_state->m_player[i].m_balance > 0.0))
		{
			return true;
		}
	}
	return false;
	*/
}

bool COcclusionCheck::UserBalanceOccluded()
{
	return true;
	/*
	// Result:
	//  * true:  probably occlusion.
	//  * false: probably no occlusion.
	if (!UserChairKnown())
	{
		// Userchair not known; 
		// Probably cards occluded.
		return true;
	}
	else if (UserBalanceNonZero())
	{
		// Balance known (non-zero).
		// Probably not occluded.
		return false;
	}
	else if (UserNameKnown())
	{
		// Username known, but balance zero.
		// Username (and probably balance) not occluded.
		return false;
	}
	else if (AnyOpponentNameKnown())
	{
		// Other name(s) known, but username not.
		// Username (and probably balance) occluded.
		return true;
	}
	else if(AnyApponentBalanceNonZero())
	{
		// No names known, at least one other balance known (non-zeor).
		// TM does probably not support names, no occlusion.
		return false;
	}
	else
	{
		// No names known, no other balances known or all zero.
		// Probably full table occluded.
		return true;
	}
	*/
}
