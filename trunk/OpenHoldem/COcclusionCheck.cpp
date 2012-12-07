// Heuristic occlusion-check for rebuy.

#include "stdafx.h"
#include "COcclusionCheck.h"

#include "CGameState.h"
#include "CPreferences.h"
#include "CSCraper.h"
#include "CSymbolEngineChipAmounts.h"
#include "debug.h"


COcclusionCheck *p_occlusioncheck = NULL;


COcclusionCheck::COcclusionCheck()
{}

COcclusionCheck::~COcclusionCheck()
{}

bool COcclusionCheck::UserChairKnown()
{
	if (p_symbol_engine_userchair->userchair_confirmed())
	{
		return true;
	}
	else
	{
		write_log(prefs.debug_occlusionchecker(), "[COcclusionCheck] UserChairKnown: false\n");
		return false;
	}
}

bool COcclusionCheck::UserBalanceNonZero()
{
	int userchair = p_symbol_engine_userchair->userchair();
	if (UserChairKnown() 
		&& (p_symbol_engine_chip_amounts->balance(userchair) > 0))
	{
		return true;
	}
	else
	{
		write_log(prefs.debug_occlusionchecker(), "[COcclusionCheck] UserBalanceNonZero: false\n");
		return false;
	}
}

bool COcclusionCheck::UserNameKnown()
{	
	int Userchair = p_symbol_engine_userchair->userchair();
	if ((Userchair < 0) || (Userchair > 9))
	{
		write_log(prefs.debug_occlusionchecker(), "[COcclusionCheck] UserNameKnown: false; chair out of range\n");
		return false;
	}
	else if (UserChairKnown() && (p_scraper->player_name(Userchair) != ""))
	{	
		return true;
	}
	else
	{
		write_log(prefs.debug_occlusionchecker(), "[COcclusionCheck] UserNameKnown: false\n");
		return false;
	}
}

bool COcclusionCheck::AnyOpponentNameKnown()
{
	int Userchair = p_symbol_engine_userchair->userchair();
	for (int i=0; i<=9; i++)
	{
		if ((i != Userchair) && (p_scraper->player_name(i) != ""))
		{
			return true;
		}
	}
	write_log(prefs.debug_occlusionchecker(), "[COcclusionCheck] AnyOpponentNameKnown: false\n");
	return false;
}

bool COcclusionCheck::AnyApponentBalanceNonZero()
{
	int Userchair = p_symbol_engine_userchair->userchair();
	for (int i=0; i<=9; i++)
	{
		if ((i != Userchair) && (p_symbol_engine_chip_amounts->balance(i) > 0))
		{
			return true;
		}
	}
	write_log(prefs.debug_occlusionchecker(), "[COcclusionCheck] AnyApponentBalanceNonZero: false\n");
	return false;
}

bool COcclusionCheck::UserBalanceOccluded()
{
	// Result:
	//  * true:  probably occlusion.
	//  * false: probably no occlusion.
	if (!UserChairKnown())
	{
		// Userchair not known; 
		// Something went completely wrong.
		// For sure the balance is not known;
		// We treat the table as "occluded".
		write_log(prefs.debug_occlusionchecker(), "COcclusionCheck: user chair not known. Assuming occlusion\n");
		return true;
	}
	else if (UserBalanceNonZero())
	{
		// Balance known (non-zero).
		// Probably not occluded.
		write_log(prefs.debug_occlusionchecker(), "COcclusionCheck: user balance non-zero. Assuming all is ok\n");
		return false;
	}
	else if (UserNameKnown())
	{
		// Username known, but balance zero.
		// Username (and probably balance) not occluded.
		write_log(prefs.debug_occlusionchecker(), "COcclusionCheck: user name known. Assuming all is ok\n");
		return false;
	}
	else if (AnyOpponentNameKnown())
	{
		// Other name(s) known, but username not.
		// Username (and probably balance) occluded.
		write_log(prefs.debug_occlusionchecker(), "COcclusionCheck: user name and balance not known, but opponents. Assuming occlusion\n");
		return true;
	}
	else if(AnyApponentBalanceNonZero())
	{
		// No names known, at least one other balance known (non-zero).
		// TM does probably not support names, no occlusion.
		write_log(prefs.debug_occlusionchecker(), "[COcclusionCheck] TM does probably not support names, other balances known. Assuming no occulsion\n.");
		return false;
	}
	else
	{
		// No names known, no other balances known or all zero.
		// Probably full table occluded.
		write_log(prefs.debug_occlusionchecker(), "[COcclusionCheck] No names and balances known at all. Assuming occlusion.\n");
		return true;
	}
}
