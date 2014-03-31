#include "StdAfx.h"
#include "CPokerTrackerThread.h"

#include <assert.h>
#include <process.h>
#include <comdef.h>
#include "CPreferences.h"
#include "CGameState.h"
#include "CLevDistance.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineRaisersCallers.h"
#include "CSymbolEngineTime.h"
#include "CSymbolEngineUserchair.h"
#include "CSymbols.h"
#include "MagicNumbers.h"
#include "..\CTablemap\CTablemap.h"
#include "MagicNumbers.h"
#include "OH_MessageBox.h"
#include "PokerTracker_Queries_Version_3.h"
#include "StringFunctions.h"

CPokerTrackerThread	*p_pokertracker_thread = NULL;
CPokerTrackerLookup pt_lookup;

CPokerTrackerLookup::CPokerTrackerLookup()
{
	_pt3_siteid.clear();
	
	// Documentation about PT3 sited_IDs:
	// http://www.pokertracker.com/forums/viewtopic.php?f=18&t=20169&p=95629
	// All sitenames and networknames have to be in LOWER-CASES!
	_pt3_siteid.insert(std::pair<CString, int> ("stars", 100));
	_pt3_siteid.insert(std::pair<CString, int> ("party", 200));
	_pt3_siteid.insert(std::pair<CString, int> ("fulltilt", 300));
	_pt3_siteid.insert(std::pair<CString, int> ("ipoker", 400));
	_pt3_siteid.insert(std::pair<CString, int> ("everest", 500));
	_pt3_siteid.insert(std::pair<CString, int> ("ongame", 600));
	_pt3_siteid.insert(std::pair<CString, int> ("boss", 700));
	_pt3_siteid.insert(std::pair<CString, int> ("cereus", 800));
	_pt3_siteid.insert(std::pair<CString, int> ("pacific", 900));
	_pt3_siteid.insert(std::pair<CString, int> ("b2b", 1000));
	_pt3_siteid.insert(std::pair<CString, int> ("microgaming", 1100));
	_pt3_siteid.insert(std::pair<CString, int> ("cake", 1200));
	_pt3_siteid.insert(std::pair<CString, int> ("bodog", 1300));
	_pt3_siteid.insert(std::pair<CString, int> ("betfair", 1400));
	_pt3_siteid.insert(std::pair<CString, int> ("cryptologic", 1500));
	_pt3_siteid.insert(std::pair<CString, int> ("ultimate", 1600));
	_pt3_siteid.insert(std::pair<CString, int> ("absolute", 1700));
	_pt3_siteid.insert(std::pair<CString, int> ("wpex", 1800));
	_pt3_siteid.insert(std::pair<CString, int> ("tribeca", 1900));
	// 2000 not (yet) supported, whatever it is
	_pt3_siteid.insert(std::pair<CString, int> ("merge", 2100));
	_pt3_siteid.insert(std::pair<CString, int> ("winamax", 2200));
	_pt3_siteid.insert(std::pair<CString, int> ("everleaf", 2300));
	_pt3_siteid.insert(std::pair<CString, int> ("yatahay", 2400));
}

CPokerTrackerLookup::~CPokerTrackerLookup()
{
}

const int CPokerTrackerLookup::GetSiteId()
{
	CString network = p_tablemap->network();

	// No longer requiring an exact match for manualmode,
	// but treating it like a normal casino.
	// http://www.maxinmontreal.com/forums/viewtopic.php?f=114&t=12158&p=108712#p108712
	std::map<CString, int>::const_iterator lookup, end;

	// Is s$sitename one of the supported PT sites?  Return the proper site_id for db queries.
	// PT version 3 only
	lookup = _pt3_siteid.begin();
	end = _pt3_siteid.end();

	while (lookup!=end)
	{
		CString	sym = "sitename$" + lookup->first;
		int e = SUCCESS;

		if (p_symbols->GetSymbolVal(sym.MakeLower().GetString(), &e))
			return lookup->second;

		lookup++;
	}

	// Is s$network one of the supported PT sites?  Return the proper site_id for db queries.
	// PT version 3 only
	lookup = _pt3_siteid.begin();
	end = _pt3_siteid.end();

	while (lookup!=end)
	{
		CString	sym = "network$" + lookup->first;
		int e = SUCCESS;

		if (p_symbols->GetSymbolVal(sym.MakeLower().GetString(), &e))
			return lookup->second;

		lookup++;
	}
	return -1 ;
}


CPokerTrackerThread::CPokerTrackerThread()
{
	// Initialize variables
	_pt_thread = NULL;

	_conn_str.Format("host=%s port=%s user=%s password=%s dbname='%s'",  
		prefs.pt_ip_addr(), prefs.pt_port(), prefs.pt_user(), prefs.pt_pass(), prefs.pt_dbname());

	ClearAllStats();

	_connected = false;
	_m_stop_thread = NULL;
	_m_wait_thread = NULL;
	SetStatGroups();
	SetStatTypes();
	_pgconn = NULL; 
}

void CPokerTrackerThread::SetStatGroups()
{
	/* Ring symbols */
	_m_statGroup[pt_icon] = pt_group_advanced;
	_m_statGroup[pt_hands] = pt_group_basic;
	_m_statGroup[pt_pfr] = pt_group_basic;
	_m_statGroup[pt_aggp] = pt_group_basic;
	_m_statGroup[pt_aggf] = pt_group_basic;
	_m_statGroup[pt_aggt] = pt_group_advanced;
	_m_statGroup[pt_aggr] = pt_group_basic;
	_m_statGroup[pt_aggtot] = pt_group_basic;
	_m_statGroup[pt_aggtotnopf] = pt_group_basic;
	_m_statGroup[pt_floppct] = pt_group_basic;
	_m_statGroup[pt_turnpct] = pt_group_advanced;
	_m_statGroup[pt_riverpct] = pt_group_advanced;
	_m_statGroup[pt_vpip] = pt_group_basic;
	_m_statGroup[pt_pf_rfi] = pt_group_basic;
	_m_statGroup[pt_pf_cr] = pt_group_basic;
	_m_statGroup[pt_pfats] = pt_group_basic;
	_m_statGroup[pt_wsdp] = pt_group_advanced;
	_m_statGroup[pt_wssd] = pt_group_advanced;
	_m_statGroup[pt_fbbts] = pt_group_basic;
	_m_statGroup[pt_fsbts] = pt_group_basic;
	_m_statGroup[pt_cbetflop] = pt_group_advanced;
	_m_statGroup[pt_f3bettot] = pt_group_advanced;
	_m_statGroup[pt_f3betpflop] = pt_group_basic;
	_m_statGroup[pt_f3betflop] = pt_group_advanced;
	_m_statGroup[pt_f3betturn] = pt_group_advanced;
	_m_statGroup[pt_f3betriver] = pt_group_advanced;
	_m_statGroup[pt_fcbetflop] = pt_group_advanced;
	_m_statGroup[pt_fcbetturn] = pt_group_advanced;
	_m_statGroup[pt_fcbetriver] = pt_group_advanced;

	/* Tournament symbols */
	_m_statGroup[ptt_icon] = pt_group_advanced;
	_m_statGroup[ptt_hands] = pt_group_basic;
	_m_statGroup[ptt_pfr] = pt_group_basic;
	_m_statGroup[ptt_aggp] = pt_group_basic;
	_m_statGroup[ptt_aggf] = pt_group_basic;
	_m_statGroup[ptt_aggt] = pt_group_advanced;
	_m_statGroup[ptt_aggr] = pt_group_basic;
	_m_statGroup[ptt_aggtot] = pt_group_basic;
	_m_statGroup[ptt_aggtotnopf] = pt_group_basic;
	_m_statGroup[ptt_floppct] = pt_group_basic;
	_m_statGroup[ptt_turnpct] = pt_group_advanced;
	_m_statGroup[ptt_riverpct] = pt_group_advanced;
	_m_statGroup[ptt_vpip] = pt_group_basic;
	_m_statGroup[ptt_pf_rfi] = pt_group_basic;
	_m_statGroup[ptt_pf_cr] = pt_group_basic;
	_m_statGroup[ptt_pfats] = pt_group_basic;
	_m_statGroup[ptt_wsdp] = pt_group_advanced;
	_m_statGroup[ptt_wssd] = pt_group_advanced;
	_m_statGroup[ptt_fbbts] = pt_group_basic;
	_m_statGroup[ptt_fsbts] = pt_group_basic;
}

void CPokerTrackerThread::SetRingStatsState(bool enabled)
{
	for (int i = pt_ring_min; i <= pt_ring_max; ++i)
	{	
		_m_enabled_stats[i] = enabled;
	}
}
void CPokerTrackerThread::SetTourneyStatsState(bool enabled)
{
	for (int i = pt_tourney_min; i <= pt_tourney_max; ++i)
	{	
		_m_enabled_stats[i] = enabled;
	}
}

void CPokerTrackerThread::SetStatTypes()
{
	int i;
	/* Ring symbols */
	for (i = pt_ring_min; i <= pt_ring_max; ++i)
	{	
		_m_stat_type[i] = pt_statType_Ring;
	}
	
	/* Tournament symbols */
	for (i = pt_tourney_min; i <= pt_tourney_max; ++i)
	{	
		_m_stat_type[i] = pt_statType_Tourney;
	}
}


CPokerTrackerThread::~CPokerTrackerThread()
{
	StopThread();
}

void CPokerTrackerThread::StartThread()
{
	if (_pt_thread == NULL)
	{
		// Create events
		_m_stop_thread = CreateEvent(0, TRUE, FALSE, 0);
		_m_wait_thread = CreateEvent(0, TRUE, FALSE, 0);

		_pt_thread = AfxBeginThread(PokertrackerThreadFunction, this);

		write_log_pokertracker(prefs.debug_pokertracker(), "Started Poker Tracker thread.\n");
	}
}

void CPokerTrackerThread::StopThread()
{
	if (_pt_thread)
	{
		// Trigger thread to stop
		::SetEvent(_m_stop_thread);

		// Wait until thread finished
		::WaitForSingleObject(_m_wait_thread, INFINITE);

		_pt_thread = NULL;

		Disconnect();

		write_log_pokertracker(prefs.debug_pokertracker(), "Stopped Poker Tracker thread.\n");
	}

	// Close handles
	if (_m_stop_thread)
	{
		::CloseHandle(_m_stop_thread);
		_m_stop_thread = NULL;
	}
	if (_m_wait_thread)
	{
		::CloseHandle(_m_wait_thread);
		_m_wait_thread = NULL;
	}
}

void CPokerTrackerThread::WarnAboutInvalidPTSymbol(CString s)
{
	CString error_message = "Error: Invalid PT-symbol: " + s;
	OH_MessageBox_Error_Warning(error_message, "Error");
}

const double CPokerTrackerThread::ProcessQuery (const char * s)
{
	int		sym_raischair = p_symbol_engine_raisers_callers->raischair();

	if (!_connected || PQstatus(_pgconn) != CONNECTION_OK)
	{
		if (!p_symbol_engine_userchair->userchair_confirmed())
		{
			// We are not yet seated.
			// Symbol-lookup happens, because of Formula-validation.
			// Not a problem, if we do not yet have a DB-connection.
			// Don't throw a warning here.
		}
		else
		{
			// We are seated and playing.
			// Serious problem, if we do not have a DB-connection.
			OH_MessageBox_Error_Warning("Not connected to PokerTracker database.\n"
				"Can't use PokerTracker symbols.", "ERROR");
		}
		return k_undefined;
	}

	// ATTENTION!
	//  
	// Be very careful, if a string is a prefix of multiple symbols,
	// e.g. "pt_vpip" is a prefix of both "pt_vpipX" and "pt_vpipsbX".
	// Take care to handle those cases correctly!

	// PokerTracker ring game symbols for the raise-chair
	if (StringAIsPrefixOfStringB("pt_r", s)
		&& !StringAIsPrefixOfStringB("pt_riverpct", s))
	{
		if		(StringIsExactMatch("pt_ricon", s))			return GetStat(sym_raischair, pt_icon);
		else if (StringIsExactMatch("pt_rpfr", s))			return GetStat(sym_raischair, pt_pfr);
		else if (StringIsExactMatch("pt_raggtotnopf", s))	return GetStat(sym_raischair, pt_aggtotnopf);
		else if (StringIsExactMatch("pt_raggtot", s))		return GetStat(sym_raischair, pt_aggtot);
		else if (StringIsExactMatch("pt_raggp", s))			return GetStat(sym_raischair, pt_aggp);
		else if (StringIsExactMatch("pt_raggf", s))			return GetStat(sym_raischair, pt_aggf);
		else if (StringIsExactMatch("pt_raggt", s))			return GetStat(sym_raischair, pt_aggt);
		else if (StringIsExactMatch("pt_raggr", s))			return GetStat(sym_raischair, pt_aggr);
		else if (StringIsExactMatch("pt_rfloppct", s))		return GetStat(sym_raischair, pt_floppct);
		else if (StringIsExactMatch("pt_rturnpct", s))		return GetStat(sym_raischair, pt_turnpct);
		else if (StringIsExactMatch("pt_rriverpct", s))		return GetStat(sym_raischair, pt_riverpct);
		else if (StringIsExactMatch("pt_rvpip", s))			return GetStat(sym_raischair, pt_vpip);
		else if (StringIsExactMatch("pt_rhands", s))		return GetStat(sym_raischair, pt_hands);
		else if (StringIsExactMatch("pt_rpf_rfi", s))		return GetStat(sym_raischair, pt_pf_rfi);
		else if (StringIsExactMatch("pt_rpf_cr", s))		return GetStat(sym_raischair, pt_pf_cr);
		else if (StringIsExactMatch("pt_rpfats", s))		return GetStat(sym_raischair, pt_pfats);
		else if (StringIsExactMatch("pt_rwsdp", s))			return GetStat(sym_raischair, pt_wsdp);
		else if (StringIsExactMatch("pt_rwssd", s))			return GetStat(sym_raischair, pt_wssd);
		else if (StringIsExactMatch("pt_rfbbts", s))		return GetStat(sym_raischair, pt_fbbts);
		else if (StringIsExactMatch("pt_rfsbts", s))		return GetStat(sym_raischair, pt_fsbts);

		else if (StringIsExactMatch("pt_rcbetflop", s))		return GetStat(sym_raischair, pt_cbetflop);
		else if (StringIsExactMatch("pt_rf3bettot", s))		return GetStat(sym_raischair, pt_f3bettot);
		else if (StringIsExactMatch("pt_rf3betpflop", s))	return GetStat(sym_raischair, pt_f3betpflop);
		else if (StringIsExactMatch("pt_rf3betflop", s))	return GetStat(sym_raischair, pt_f3betflop);
		else if (StringIsExactMatch("pt_rf3betturn", s))	return GetStat(sym_raischair, pt_f3betturn);
		else if (StringIsExactMatch("pt_rf3betriver", s))	return GetStat(sym_raischair, pt_f3betriver);
		else if (StringIsExactMatch("pt_rfcbetflop", s))	return GetStat(sym_raischair, pt_fcbetflop);
		else if (StringIsExactMatch("pt_rfcbetturn", s))	return GetStat(sym_raischair, pt_fcbetturn);
		else if (StringIsExactMatch("pt_rfcbetriver",s))	return GetStat(sym_raischair, pt_fcbetriver);
		else
		{
			// Invalid ring game symbol
			WarnAboutInvalidPTSymbol(s);
			return -1.0;
		}
	}
	// PokerTracker ring game symbols for chair X
	else if (StringAIsPrefixOfStringB("pt_", s))
	{
		if	    (StringAIsPrefixOfStringB("pt_iconlastr", s))		return GetStat(p_game_state->LastRaised(s[12]-'0'), pt_icon);
		else if (StringAIsPrefixOfStringB("pt_icon", s))			return GetStat(s[7]-'0', pt_icon);
		else if (StringAIsPrefixOfStringB("pt_pfr",s ))				return GetStat(s[6]-'0', pt_pfr);
		else if (StringAIsPrefixOfStringB("pt_aggtotnopf", s))		return GetStat(s[13]-'0', pt_aggtotnopf);
		else if (StringAIsPrefixOfStringB("pt_aggtot", s))			return GetStat(s[9]-'0', pt_aggtot);
		else if (StringAIsPrefixOfStringB("pt_aggp", s))			return GetStat(s[7]-'0', pt_aggp);
		else if (StringAIsPrefixOfStringB("pt_aggf", s))			return GetStat(s[7]-'0', pt_aggf);
		else if (StringAIsPrefixOfStringB("pt_aggt", s))			return GetStat(s[7]-'0', pt_aggt);
		else if (StringAIsPrefixOfStringB("pt_aggr", s))			return GetStat(s[7]-'0', pt_aggr);
		else if (StringAIsPrefixOfStringB("pt_floppct", s))			return GetStat(s[10]-'0', pt_floppct);
		else if (StringAIsPrefixOfStringB("pt_turnpct", s))			return GetStat(s[10]-'0', pt_turnpct);
		else if (StringAIsPrefixOfStringB("pt_riverpct", s))		return GetStat(s[11]-'0', pt_riverpct);
		else if (StringAIsPrefixOfStringB("pt_vpip", s))			return GetStat(s[7]-'0', pt_vpip);
		else if (StringAIsPrefixOfStringB("pt_hands", s))			return GetStat(s[8]-'0', pt_hands);
		else if (StringAIsPrefixOfStringB("pt_pf_rfi", s))			return GetStat(s[9]-'0', pt_pf_rfi);
		else if (StringAIsPrefixOfStringB("pt_pf_cr", s))			return GetStat(s[8]-'0', pt_pf_cr);
		else if (StringAIsPrefixOfStringB("pt_pfats", s))			return GetStat(s[8]-'0', pt_pfats);
		else if (StringAIsPrefixOfStringB("pt_wsdp", s))			return GetStat(s[7]-'0', pt_wsdp);
		else if (StringAIsPrefixOfStringB("pt_wssd",s))				return GetStat(s[7]-'0', pt_wssd);
		else if (StringAIsPrefixOfStringB("pt_fbbts", s))			return GetStat(s[8]-'0', pt_fbbts);
		else if (StringAIsPrefixOfStringB("pt_fsbts", s))			return GetStat(s[8]-'0', pt_fsbts);

		else if (StringAIsPrefixOfStringB("pt_cbetflop", s))		return GetStat(s[11]-'0', pt_cbetflop);
		else if (StringAIsPrefixOfStringB("pt_f3bettot", s))		return GetStat(s[11]-'0', pt_f3bettot);
		else if (StringAIsPrefixOfStringB("pt_f3betpflop", s))		return GetStat(s[15]-'0', pt_f3betpflop);
		else if (StringAIsPrefixOfStringB("pt_f3betflop", s))		return GetStat(s[12]-'0', pt_f3betflop);
		else if (StringAIsPrefixOfStringB("pt_f3betturn", s))		return GetStat(s[12]-'0', pt_f3betturn);
		else if (StringAIsPrefixOfStringB("pt_f3betriver", s))		return GetStat(s[13]-'0', pt_f3betriver);
		else if (StringAIsPrefixOfStringB("pt_fcbetflop", s))		return GetStat(s[12]-'0', pt_fcbetflop);
		else if (StringAIsPrefixOfStringB("pt_fcbetturn", s))		return GetStat(s[12]-'0', pt_fcbetturn);
		else if (StringAIsPrefixOfStringB("pt_fcbetriver", s))		return GetStat(s[12]-'0', pt_fcbetriver);
		else
		{
			// Invalid ring game symbol
			WarnAboutInvalidPTSymbol(s);
			return -1.0;
		}
	}
	// Poker Tracker tournament symbols for raise-chair
	else if (StringAIsPrefixOfStringB("ptt_r", s)
		&& !StringAIsPrefixOfStringB("ptt_riverpct", s))
	{
		if		(StringIsExactMatch("ptt_ricon", s))		return GetStat(sym_raischair, ptt_icon);
		else if (StringIsExactMatch("ptt_rpfr", s))			return GetStat(sym_raischair, ptt_pfr);
		else if (StringIsExactMatch("ptt_raggtotnopf", s))	return GetStat(sym_raischair, ptt_aggtotnopf);
		else if (StringIsExactMatch("ptt_raggtot", s))		return GetStat(sym_raischair, ptt_aggtot);
		else if (StringIsExactMatch("ptt_raggp", s))		return GetStat(sym_raischair, ptt_aggp);
		else if (StringIsExactMatch("ptt_raggf", s))		return GetStat(sym_raischair, ptt_aggf);
		else if (StringIsExactMatch("ptt_raggt", s))		return GetStat(sym_raischair, ptt_aggt);
		else if (StringIsExactMatch("ptt_raggr", s))		return GetStat(sym_raischair, ptt_aggr);
		else if (StringIsExactMatch("ptt_rfloppct", s))		return GetStat(sym_raischair, ptt_floppct);
		else if (StringIsExactMatch("ptt_rturnpct", s))		return GetStat(sym_raischair, ptt_turnpct);
		else if (StringIsExactMatch("ptt_rriverpct", s))	return GetStat(sym_raischair, ptt_riverpct);
		else if (StringIsExactMatch("ptt_rvpip", s))		return GetStat(sym_raischair, ptt_vpip);
		else if (StringIsExactMatch("ptt_rhands", s))		return GetStat(sym_raischair, ptt_hands);
		else if (StringIsExactMatch("ptt_rpf_rfi", s))		return GetStat(sym_raischair, ptt_pf_rfi);
		else if (StringIsExactMatch("ptt_rpf_cr", s))		return GetStat(sym_raischair, ptt_pf_cr);
		else if (StringIsExactMatch("ptt_rpfats", s))		return GetStat(sym_raischair, ptt_pfats);
		else if (StringIsExactMatch("ptt_rwsdp", s))		return GetStat(sym_raischair, ptt_wsdp);
		else if (StringIsExactMatch("ptt_rwssd", s))		return GetStat(sym_raischair, ptt_wssd);
		else if (StringIsExactMatch("ptt_rfbbts", s))		return GetStat(sym_raischair, ptt_fbbts);
		else if (StringIsExactMatch("ptt_rfsbts", s))		return GetStat(sym_raischair, ptt_fsbts);
		else
		{
			// Invalid tournament symbol
			WarnAboutInvalidPTSymbol(s);
			return -1.0;
		}
	}
	// Poker Tracker tournament symbols for chair x
	else if (StringIsExactMatch("ptt_", s))
	{
		if		(StringAIsPrefixOfStringB("ptt_iconlastr", s))		return GetStat(p_game_state->LastRaised(s[13]-'0'), ptt_icon);
		else if (StringAIsPrefixOfStringB("ptt_icon", s))			return GetStat(s[8]-'0', ptt_icon);
		else if (StringAIsPrefixOfStringB("ptt_pfr", s))			return GetStat(s[7]-'0', ptt_pfr);
		else if (StringAIsPrefixOfStringB("ptt_aggtotnopf", s))		return GetStat(s[14]-'0', ptt_aggtotnopf);
		else if (StringAIsPrefixOfStringB("ptt_aggtot", s))			return GetStat(s[10]-'0', ptt_aggtot);
		else if (StringAIsPrefixOfStringB("ptt_aggp", s))			return GetStat(s[8]-'0', ptt_aggp);
		else if (StringAIsPrefixOfStringB("ptt_aggf", s))			return GetStat(s[8]-'0', ptt_aggf);
		else if (StringAIsPrefixOfStringB("ptt_aggt", s))			return GetStat(s[8]-'0', ptt_aggt);
		else if (StringAIsPrefixOfStringB("ptt_aggr", s))			return GetStat(s[8]-'0', ptt_aggr);
		else if (StringAIsPrefixOfStringB("ptt_floppct", s))		return GetStat(s[11]-'0', ptt_floppct);
		else if (StringAIsPrefixOfStringB("ptt_turnpct", s))		return GetStat(s[11]-'0', ptt_turnpct);
		else if (StringAIsPrefixOfStringB("ptt_riverpct", s))		return GetStat(s[12]-'0', ptt_riverpct);
		else if (StringAIsPrefixOfStringB("ptt_vpip", s))			return GetStat(s[8]-'0', ptt_vpip);
		else if (StringAIsPrefixOfStringB("ptt_hands", s))			return GetStat(s[9]-'0', ptt_hands);
		else if (StringAIsPrefixOfStringB("ptt_pf_rfi", s))			return GetStat(s[10]-'0', ptt_pf_rfi);
		else if (StringAIsPrefixOfStringB("ptt_pf_cr", s))			return GetStat(s[9]-'0', ptt_pf_cr);
		else if (StringAIsPrefixOfStringB("ptt_pfats", s))			return GetStat(s[9]-'0', ptt_pfats);
		else if (StringAIsPrefixOfStringB("ptt_wsdp", s))			return GetStat(s[8]-'0', ptt_wsdp);
		else if (StringAIsPrefixOfStringB("ptt_wssd", s))			return GetStat(s[8]-'0', ptt_wssd);
		else if (StringAIsPrefixOfStringB("ptt_fbbts", s))			return GetStat(s[9]-'0', ptt_fbbts);
		else if (StringAIsPrefixOfStringB("ptt_fsbts", s))			return GetStat(s[9]-'0', ptt_fsbts);
		else
		{
			// Invalid tournament symbol
			WarnAboutInvalidPTSymbol(s);
			return -1.0;
		}
	}
	else
	{
		// Completely invalid symbol
		WarnAboutInvalidPTSymbol(s);
		return -1.0;
	}
}

void CPokerTrackerThread::Connect(void)
{
	_pgconn = PQconnectdb(_conn_str.GetString());

	if (PQstatus(_pgconn) == CONNECTION_OK)
	{
		write_log_pokertracker(prefs.debug_pokertracker(), "PostgreSQL DB opened successfully <%s/%s/%s>\n", 
			prefs.pt_ip_addr(), 
			prefs.pt_port(), 
			prefs.pt_dbname());

		_connected = true;
	}
	else
	{
		write_log_pokertracker(prefs.debug_pokertracker(), "ERROR opening PostgreSQL DB: %s\n\n", PQerrorMessage(_pgconn));
		PQfinish(_pgconn);
		_connected = false;
		_pgconn = NULL;
	}
}

void CPokerTrackerThread::Disconnect(void)
{
	if(_pgconn)
    {
      if (PQstatus(_pgconn) == CONNECTION_OK)
        {
          PQfinish(_pgconn);
        }
    }
	_pgconn = NULL;
	_connected = false;
}



/* When running this function, chair is the chair to IGNORE 
   That's because this function is running while GetStatsForChair is running,
   And we wouldn't like to interrupt its order and ability to detect name changes
   In the seat it's getting stats for*/ 
void CPokerTrackerThread::ReportSeatChanges(int chair)
{
	int i;
	bool nameChanged;
	char currentScrapeName[k_max_length_of_playername];
	write_log_pokertracker(3, "ReportSeatChanges: started\n");
	for (i = k_min_chair_number; i < k_max_chair_number; ++i)
	{
		if (i != chair)
		{
			memcpy(currentScrapeName, _player_stats[i].scraped_name, k_max_length_of_playername);
			CheckName(i, nameChanged);
			if (nameChanged)
			{
				/* Scrapped name got changed. Clear stats for that chair */
				write_log_pokertracker(2, "ReportSeatChanges: chair [%d]: new player sat down in chair! oldscrape[%s] newscrape[%s].\n", i, currentScrapeName, _player_stats[i].scraped_name);
				/* Clear stats but leave the new name intact */
				ClearSeatStats(i, false);
			}
		}
	}
}

/*Returns true if found an appropriate name in the DB for chr, or false if 
  it did not found such name. Also changes nameChanged if the name was found but 
  changed since the last time we've called CheckName function */
bool CPokerTrackerThread::CheckName(int chr, bool &nameChanged)
{
	char		oh_scraped_name[k_max_length_of_playername]; 
	char		best_name[k_max_length_of_playername];
	bool		result = false, ok_scrape = false;
	int			i;


	assert(chr >= k_min_chair_number); 
	assert(chr <= k_max_chair_number);
	
	memset(oh_scraped_name, 0, k_max_length_of_playername);
	memset(best_name, 0, k_max_length_of_playername);
	
	nameChanged = false;
	if (p_game_state->state((p_game_state->state_index()-1)&0xff)->m_player[chr].m_name_known == 0)
		return false;

	strcpy_s(oh_scraped_name, k_max_length_of_playername, p_game_state->state((p_game_state->state_index()-1)&0xff)->m_player[chr].m_name);

	// Check for bad name scrape
	int len = (int) strlen(oh_scraped_name);
	for (i = 0; i < len; ++i)
	{
		if (oh_scraped_name[i]!='l' && oh_scraped_name[i]!='i' && oh_scraped_name[i]!='.' && oh_scraped_name[i]!=',')
		{
			ok_scrape = true;
			break;
		}
	}
	if (!ok_scrape)
		return false;

	// We already have the name, and it has not changed since we last checked, so do nothing
	if (_player_stats[chr].found && 0 == strcmp(_player_stats[chr].scraped_name, oh_scraped_name))
		return true;
	
	nameChanged = true;


	// We have not found the name in PT, go find it
	// First see if we can find the exact scraped name
	result = FindName(oh_scraped_name, best_name);

	if (result)
	{
		SetPlayerName(chr, true, best_name, oh_scraped_name);
	}
	else
	{
		SetPlayerName(chr, false, "", "");
	}

	return result;
}

void CPokerTrackerThread::SetPlayerName(int chr, bool found, const char* pt_name, const char* scraped_name)
{
	_player_stats[chr].found = found;
	bool logResult = false;
	if (0 != memcmp(_player_stats[chr].pt_name, pt_name, k_max_length_of_playername) )
	{
		memcpy(_player_stats[chr].pt_name, pt_name, k_max_length_of_playername);
		logResult = true;
	}
	if (0 != memcmp(_player_stats[chr].scraped_name, scraped_name, k_max_length_of_playername) )
	{
		memcpy(_player_stats[chr].scraped_name, scraped_name, k_max_length_of_playername);
		logResult = true;
	}
	if (logResult)
	{
		write_log_pokertracker(2, "SetPlayerName[%d]: Done. ptname[%s] scrapedName[%s]\n", chr, _player_stats[chr].pt_name, _player_stats[chr].scraped_name);
	}
}


//Short Algorithm to query the DB for the best name. returns true if best name is populated.
bool CPokerTrackerThread::FindName(const char *oh_scraped_name, char *best_name)
{
	char		likename[k_max_length_of_playername];
	memset(likename, 0, k_max_length_of_playername);
	
	bool result = QueryName(oh_scraped_name, oh_scraped_name, best_name);

	if (!result)
	{
		likename[0]='%';
		int character_position = 0;
		for (int character_position=0; character_position<(int) strlen(oh_scraped_name); character_position++)
		int len = (int)strlen(oh_scraped_name);

		{
			likename[character_position*2+1]=oh_scraped_name[character_position];
			likename[character_position*2+2]='%';
		}

		likename[(character_position)*2 + 1]='\0';
		result = QueryName(likename, oh_scraped_name, best_name);
	}

	if (!result)
	{
		/*  Escalation #2: "%n%a%m%e%" not found in DB*/
		/*  Try query with "%" to get all names */
		result = QueryName("%", oh_scraped_name, best_name);
	}
	return result;
}

double CPokerTrackerThread::GetStat(int chr, PT_Stats stat)
{
	assert(chr >= k_min_chair_number); 
	assert(chr <= k_max_chair_number);

	return _player_stats[chr].stat[stat];

		
}

double CPokerTrackerThread::UpdateStat(int m_chr, int stat)
{
	PGresult	*res = NULL;
	char		strQry[k_max_length_of_query] = {0};
	char		strQry1[k_max_length_of_query] = {0};
	char		strQry2[k_max_length_of_query] = {0};
	const char	*n = NULL;
	double		result = -1.0;
	char		siteidstr[k_max_length_of_site_id] = {0};
	int			siteid = 0;
	clock_t		updStart, updEnd;
	int			duration;

	int sym_elapsed = p_symbol_engine_time->elapsed();

	//No more unnecessary queries when we don't even have a siteid to check
	siteid = pt_lookup.GetSiteId();
	if (siteid == k_undefined)
		return result;

	if (!_connected || PQstatus(_pgconn) != CONNECTION_OK)
		return result;

	assert(m_chr >= k_min_chair_number);
	assert(m_chr <= k_max_chair_number);
	assert(stat >= pt_min);
	assert(stat <= pt_max);

	/* TS 01/25/2011.  Update means update... we will not back off now :-) */
	
	// If we already have stats cached for the player, the timeout has not expired,
	// return the value from the cache...
	//if (sym_elapsed - _player_stats[m_chr].t_elapsed[stat] < prefs.pt_cache_refresh() &&
		//_player_stats[m_chr].t_elapsed[stat] != -1 &&
		//_player_stats[m_chr].stat[stat] != -1)
	//{
		//result = _player_stats[m_chr].stat[stat];
	//}

	// ...otherwise query the database
	//else
	{
		// get query string for the requested statistic
		strcpy_s(strQry, k_max_length_of_query, query_str3[stat]);

		// Insert the player name in the query string
		strcpy_s(strQry1, k_max_length_of_query, strQry);  // move the query into temp str 1
		while ((n=strstr(strQry1, "%SCREENNAME%"))!=NULL) // find the token in temp str 1
		{
			strcpy_s(strQry2, k_max_length_of_query, strQry1);  // move the query into temp str 2
			strQry2[n-strQry1]='\0';  // cut off temp str 2 at the beginning of the token
			strcat_s(strQry2, k_max_length_of_query, _player_stats[m_chr].pt_name);  // append the player name to temp str 2
			strcat_s(strQry2, k_max_length_of_query, n+12); // append the portion of temp str 1 after the token to temp str 2
			strcpy_s(strQry, k_max_length_of_query, strQry2); // move temp str 2 into the original query
			strcpy_s(strQry1, k_max_length_of_query, strQry);  // move the query into temp str 1
		}

		// Insert the site id in the query string
		sprintf_s(siteidstr, k_max_length_of_site_id, "%d", siteid);
		strcpy_s(strQry1, k_max_length_of_query, strQry);  // move the query into temp str 1
		while ((n=strstr(strQry1, "%SITEID%"))!=NULL)   // find the token in temp str 1
		{
			strcpy_s(strQry2, k_max_length_of_query, strQry1);  // move the query into temp str 2
			strQry2[n-strQry1]='\0';  // cut off temp str 2 at the beginning of the token
			strcat_s(strQry2, k_max_length_of_query, siteidstr);  // append the site id to temp str 2
			strcat_s(strQry2, k_max_length_of_query, n+8); // append the portion of temp str 1 after the token to temp str 2
			strcpy_s(strQry, k_max_length_of_query, strQry2); // move temp str 2 into the original query
			strcpy_s(strQry1, k_max_length_of_query, strQry);  // move the query into temp str 1
		}

		// Do the query against the PT database
		updStart = clock();
		try
		{
			// See if we can find the player name in the database
			write_log_pokertracker(prefs.debug_pokertracker(), "Querying %s for m_chr %d: %s\n", stat_str[stat], m_chr, strQry);
			res = PQexec(_pgconn, strQry);
		}
		catch (_com_error &e)
		{
			write_log_pokertracker(prefs.debug_pokertracker(), "ERROR\n");
			write_log_pokertracker(prefs.debug_pokertracker(), _T("\tCode = %08lx\n"), e.Error());
			write_log_pokertracker(prefs.debug_pokertracker(), _T("\tCode meaning = %s\n"), e.ErrorMessage());
			_bstr_t bstrSource(e.Source());
			_bstr_t bstrDescription(e.Description());
			write_log_pokertracker(prefs.debug_pokertracker(), _T("\tSource = %s\n"), (LPCTSTR) bstrSource);
			write_log_pokertracker(prefs.debug_pokertracker(), _T("\tDescription = %s\n"), (LPCTSTR) bstrDescription);
			write_log_pokertracker(prefs.debug_pokertracker(), _T("\tQuery = [%s]\n"), strQry);
		}
		
		updEnd = clock();
		duration = (int) ((double)(updEnd - updStart) / 1000);
		if (duration >= 3)
			write_log_pokertracker(2, "Query time in seconds: [%d]\n", duration);

		// Check query return code
		if (PQresultStatus(res) != PGRES_TUPLES_OK)
		{
			switch (PQresultStatus(res))
			{
			case PGRES_COMMAND_OK:
				write_log_pokertracker(prefs.debug_pokertracker(), "PGRES_COMMAND_OK: %s [%s]\n", PQerrorMessage(_pgconn), strQry);
				break;
			case PGRES_EMPTY_QUERY:
				write_log_pokertracker(prefs.debug_pokertracker(), "PGRES_EMPTY_QUERY: %s [%s]\n", PQerrorMessage(_pgconn), strQry);
				break;
			case PGRES_BAD_RESPONSE:
				write_log_pokertracker(prefs.debug_pokertracker(), "PGRES_BAD_RESPONSE: %s [%s]\n", PQerrorMessage(_pgconn), strQry);
				break;
			case PGRES_COPY_OUT:
				write_log_pokertracker(prefs.debug_pokertracker(), "PGRES_COPY_OUT: %s [%s]\n", PQerrorMessage(_pgconn), strQry);
				break;
			case PGRES_COPY_IN:
				write_log_pokertracker(prefs.debug_pokertracker(), "PGRES_COPY_IN: %s [%s]\n", PQerrorMessage(_pgconn), strQry);
				break;
			case PGRES_NONFATAL_ERROR:
				write_log_pokertracker(prefs.debug_pokertracker(), "PGRES_NONFATAL_ERROR: %s [%s]\n", PQerrorMessage(_pgconn), strQry);
				break;
			case PGRES_FATAL_ERROR:
				write_log_pokertracker(prefs.debug_pokertracker(), "PGRES_FATAL_ERROR: %s [%s]\n", PQerrorMessage(_pgconn), strQry);
				break;
			default:
				write_log_pokertracker(prefs.debug_pokertracker(), "GENERIC ERROR: %s [%s]\n", PQerrorMessage(_pgconn), strQry);
				break;
			}
		}
		else
		{
			if (PQgetisnull(res,0,0) != 1)
			{
				result = atof(PQgetvalue(res,0,0));
				write_log_pokertracker(prefs.debug_pokertracker(), "Query %s for m_chr %d success: %f\n", stat_str[stat], m_chr, result);
			}
		}

		PQclear(res);

		// update cache with new values
		_player_stats[m_chr].stat[stat] = result;
		_player_stats[m_chr].t_elapsed[stat] = sym_elapsed;
	}

	return result;
}

bool CPokerTrackerThread::QueryName(const char * query_name, const char * scraped_name, char * best_name)
{
	char			strQry[k_max_length_of_query] = {0};
	int				lev_dist = 0, bestLD = 0, bestLDindex = 0;
	PGresult		*res = NULL;
	char			siteidstr[k_max_length_of_site_id] = {0};
	bool			result = false;
	CLevDistance	myLD;
	int				siteid = 0;
	static int		_last_siteid = -1;
	double			Levenshtein_distance_matching_factor = 0.2;

	//No more unnecessary queries when we don't even have a siteid to check
	siteid = pt_lookup.GetSiteId();
	if (siteid == k_undefined)  return false;

	// siteid has changed -- we're using ManualMode
	if (siteid != _last_siteid)
	{
		ClearAllStats();
		_last_siteid = siteid;
	}

	if (!_connected || PQstatus(_pgconn) != CONNECTION_OK)
		return false;

	if (0 == strlen(query_name))
		return false;

	sprintf_s(siteidstr, k_max_length_of_site_id, "%d", siteid);

	// PT version 3 name query
	strcpy_s(strQry, k_max_length_of_query, "SELECT player_name FROM player WHERE player_name like '");
	strcat_s(strQry, k_max_length_of_query, query_name);
	strcat_s(strQry, k_max_length_of_query, "' AND id_site=");
	strcat_s(strQry, k_max_length_of_query, siteidstr);
	
	try
	{
		res = PQexec(_pgconn, strQry);
	}
	catch (_com_error &e)
	{
		write_log_pokertracker(prefs.debug_pokertracker(), "Postgres Query error:\n");
		write_log_pokertracker(prefs.debug_pokertracker(), "\tCode = %08lx\n", e.Error());
		write_log_pokertracker(prefs.debug_pokertracker(), "\tCode meaning = %s\n", e.ErrorMessage());
		_bstr_t bstrSource(e.Source());
		_bstr_t bstrDescription(e.Description());
		write_log_pokertracker(prefs.debug_pokertracker(), "\tSource = %s\n", (LPCTSTR) bstrSource);
		write_log_pokertracker(prefs.debug_pokertracker(), "\tDescription = %s\n", (LPCTSTR) bstrDescription);
		write_log_pokertracker(prefs.debug_pokertracker(), "\tQuery = [%s]\n", strQry);
	}

	// We got nothing, return false
	if (PQntuples(res) == 0)
	{
		result = false;
	}

	// If we get one tuple, all is good - return the one name
	if (PQntuples(res) == 1)
	{
		lev_dist = myLD.LD(scraped_name, PQgetvalue(res, 0, 0));

		if (lev_dist<=(int)strlen(PQgetvalue(res, 0, 0))*Levenshtein_distance_matching_factor )
		{
	     	strcpy_s(best_name, k_max_length_of_playername, PQgetvalue(res, 0, 0));
			result = true;
		}
		else {
			result = false;
		}
	}

	// We got more than one tuple, figure the Levenshtein distance for all of them
	// and return the best
	else if ((PQntuples(res) > 1))
	{
		bestLD = 999;
		for (int i=0; i<PQntuples(res); i++)
		{
			lev_dist = myLD.LD(scraped_name, PQgetvalue(res, i, 0));

			if (lev_dist<bestLD && lev_dist<(int)strlen(PQgetvalue(res, i, 0))*Levenshtein_distance_matching_factor ) 
			{
				bestLD = lev_dist;
				bestLDindex = i;
			}
		}
		if (bestLD != 999)
		{
			strcpy_s(best_name, k_max_length_of_playername, PQgetvalue(res, bestLDindex, 0));
			result = true;
		}
		else
		{
			result = false;
		}
	}

	PQclear(res);
	return result;
}


void CPokerTrackerThread::ClearSeatStats(int chr, bool clearNameAndFound)
{
	assert(chr >= k_min_chair_number); 
	assert(chr <= k_max_chair_number);
	int j;
	for (j = pt_min; j <= pt_max; ++j)
	{
		_player_stats[chr].stat[j] = -1.0;
		_player_stats[chr].t_elapsed[j] = -1;
	}
	if (clearNameAndFound)
	{
		_player_stats[chr].found = false;
		memset(_player_stats[chr].pt_name, 0, k_max_length_of_playername);
		memset(_player_stats[chr].scraped_name, 0, k_max_length_of_playername);
	}
	_player_stats[chr].skipped_updates = k_advanced_stat_update_every;
}

void CPokerTrackerThread::ClearAllStats()
{
	int i;
	for (i = 0; i <= 9; ++i)
	{
		ClearSeatStats(i);
	}
}

// Returns 1 for basic stats only, and 2 for all
int CPokerTrackerThread::GetUpdateType(int chr)
{
	if (_player_stats[chr].skipped_updates == k_advanced_stat_update_every)
	{
		write_log_pokertracker(3, "GetUpdateType: update type for chair [%d] is update ALL\n", chr);
		return pt_updateType_updateAll;
	}
	write_log_pokertracker(3, "GetUpdateType: update type for chair [%d] is update Basic only\n", chr);
	return pt_updateType_updateBasic;
}


void CPokerTrackerThread::RecalcSkippedUpdates(int chr)
{
	if (_player_stats[chr].skipped_updates == k_advanced_stat_update_every)
		_player_stats[chr].skipped_updates = 1;
	else
		++_player_stats[chr].skipped_updates;
}


int CPokerTrackerThread::SkipUpdateCondition(int stat, int chair)
{
	int statGroup = _m_statGroup[stat]; // puts in statgroup the group of this stat, that is basic/advanced/positional
	int result;
	int updateType = GetUpdateType(chair); // get the current update type, that is either basic or all

	if (updateType == pt_updateType_updateBasic && (statGroup == pt_group_advanced || statGroup == pt_group_positional))
		result = 1;
	else
		result = 0;
	return result;
}

void CPokerTrackerThread::SetHandsStat()
{
	int tourney = p_tablelimits->istournament();
	if (tourney)
	{
		_m_handsStats = ptt_hands;
		_m_min_hands_for_slower_update = k_min_hands_slower_updates_tourney;
		SetRingStatsState(false);
		SetTourneyStatsState(true);
	}
	else
	{
		_m_handsStats = pt_hands;
		_m_min_hands_for_slower_update = k_min_hands_slower_updates_ring;
		SetRingStatsState(true);
		SetTourneyStatsState(false);
	}
}

int CPokerTrackerThread::SkipUpdateForChair(int chair, char* reason)
{
	memset(reason,0,100);
	int userchair = p_symbol_engine_userchair->userchair();
	bool confirmed = p_symbol_engine_userchair->userchair_confirmed();
	if (userchair == chair && confirmed)
	{
		memcpy(reason, "User sits in this chair", 100);
		return pt_updateType_noUpdate;
	}
	
	int hands = (int)GetStat(chair, _m_handsStats);
	if (hands > _m_min_hands_for_slower_update)
	{
		if (GetUpdateType(chair) == pt_updateType_updateAll)
			return pt_updateType_updateAll;
		else
		{
			memcpy(reason, "User has lots of hands", 100);
			return pt_updateType_noUpdate;
		}
	}
	return pt_updateType_updateAll;
}


void CPokerTrackerThread::GetStatsForChair(LPVOID pParam, int chair, int sleepTime)
{
	CPokerTrackerThread *pParent = static_cast<CPokerTrackerThread*>(pParam);
	bool		nameChanged = false;
	bool		sym_issittingin = p_symbol_engine_autoplayer->issittingin();
	bool		sym_ismanual = (bool) p_symbol_engine_autoplayer->ismanual();
	int			i;
	int			updateType;
	char        reason[100];
	int         updatedCount = 0;
	
	if (pParent->CheckName(chair, nameChanged) == false)
	{
		/* Note that checkname fail just when starting, doesn't necessarily mean that there's no user
		   in that chair, but only that the scraper failed to find one. This could be due to lobby window
		   that hides poker window behind it. We make this check once, and if we are good, the update iteration
		   is good to go. if we are not, we assume that this seat is not taken. */ 
		write_log_pokertracker(2, "GetStatsForChair[%d] had been started.\n", chair);
		write_log_pokertracker(2, "GetStatsForChair[%d] had been skipped. Reason: [CheckName failed]\n", chair);
		return;
	}
	const char* playerscrapedName = pParent->GetPlayerScrapedName(chair);
	write_log_pokertracker(2, "GetStatsForChair[%d][%s] had been started.\n", chair, playerscrapedName);
	
	/* Check if there's a complete update cycle skipping for that chair */
	updateType = pParent->SkipUpdateForChair(chair, reason);
	if (updateType == pt_updateType_noUpdate)
	{
		write_log_pokertracker(2, "GetStatsForChair for chair [%d] had been skipped. Reason: [%s]\n", chair, reason);
		pParent->RecalcSkippedUpdates(chair);
		return;
	}

	/* Make sure all other seats contain the appropriate players */ 
	pParent->ReportSeatChanges(chair);  

	if (!pParent->_connected)
		pParent->Connect();
	
	
	if (pParent->_connected && PQstatus(pParent->_pgconn) == CONNECTION_OK)
	{
		if (sym_issittingin || sym_ismanual)
		{
			for (i = pt_min; i <= pt_max; ++i)
			{
				/* Every few iterations, we need to verify that the seats we already have stats on, 
			       did not change. This task is totally irrelevant for the current function
				   we're on, that is GetStatsForChair. But if we won't do this every now and then,
				   we might find ourselves updating stats for chair 1, for 1 minute, while the player
				   in chair 3 stood up and someone else replaced him. we cannot let this go unnoticed */
				if (i % 10 == 4) pParent->ReportSeatChanges(chair);
			
				/* CheckName is necessary before each update.
				   There's a short interval between any two updates, and it's possible that the player
				   had stood up during the update process. But it also possible that the poker lobby was 
				   hiding our poker window, or some popup temporarily was over it, and that's why CheckName fails.
				   Since we cannot know which one caused checkname to fail, we would continue to update, as 
				   long as we have a found name, and as long as the name did't get changed. 
				   So what we do care about, is the situation were the name got replaced by another name,
				   in that case, we stop the update for the current chair  			   */ 


				pParent->CheckName(chair, nameChanged);
				/* Note that Checkname might return false, with IsFound(chair) returning true.
				   When IsFound returns false the situation must be that we no longer have anyone
				   Sitting in that chair*/
				if (pParent->IsFound(chair))
				{
					/* Verify therad_stop is false */ 
					if (LightSleep(0, pParent)) 
						return; 
					/* verify that name did not get changed */
					if (i > pt_min && nameChanged)
					{
						/* Name got changed while we search for stats for current chair
						   Clear stats for this seat and return                   */
						write_log_pokertracker(2, "GetStatsForChair chair [%d] had changed name getting stat for chair. Clearing stats for chair.\n", chair);
						/* Clear stats, but leave the new name intact */
						pParent->ClearSeatStats(chair, false); 
						return;
					}
					if (!pParent->StatEnabled(i))
					{
						/* Skip disabled stats */
						write_log_pokertracker(3, "GetStatsForChair: Updating stats [%d] for chair [%d] had been skipped. Reason: [stat is disabled]\n", i, chair);
					}
					else if (pParent->SkipUpdateCondition(i, chair))
					{
						/* Updating stat i should be skipped this time */
						/* advanced/positional stats are updated every k_advanced_stat_update_every cycles */
						write_log_pokertracker(3, "GetStatsForChair: Updating stats [%d] for chair [%d] had been skipped. Reason: [advanced/positional stats cycle [%d of %d]]\n", i, chair, pParent->GetSkippedUpdates(chair) , k_advanced_stat_update_every);
					}
					else
					{
						/* Update... */
						write_log_pokertracker(3, "GetStatsForChair updating stats [%d] for chair [%d]...\n", i, chair);
						pParent->UpdateStat(chair, i);
						++updatedCount;
					}
					/* Sleep between two updates (even if skipped) */
					if (LightSleep(sleepTime, pParent)) 
						return;
				}
				else
				{
					/* We couldn't find any user sitting on that chair. Give message*/
					write_log_pokertracker(2, "GetStatsForChair for chair [%d] had been skipped. Reason: [user not found (user stood up?)]\n", chair);
					return;
				}
			}
		}
	}
	pParent->ReportUpdateComplete(updatedCount, chair);
	pParent->RecalcSkippedUpdates(chair);
}

void CPokerTrackerThread::ReportUpdateComplete(int updatedCount, int chair)
{
	write_log_pokertracker(2, "Updates for chair [%d][%s] had been completed. Total [%d] updated stats\n", chair, _player_stats[chair].scraped_name, updatedCount);
}

bool CPokerTrackerThread::IsFound(int chair)
{
	return _player_stats[chair].found;
}


UINT CPokerTrackerThread::PokertrackerThreadFunction(LPVOID pParam)
{
	CPokerTrackerThread *pParent = static_cast<CPokerTrackerThread*>(pParam);
	bool			sym_issittingin = p_symbol_engine_autoplayer->issittingin();
	bool			sym_ismanual = p_symbol_engine_autoplayer->ismanual();
	int				chr = 0;
	int				iteration = 0;
	int				players;
	int				sleepTime;
	bool			dummy;
	clock_t			iterStart, iterEnd;
	int				iterDurationMS;

	while (::WaitForSingleObject(pParent->_m_stop_thread, 0) != WAIT_OBJECT_0)
	{
		iterStart = clock();
		write_log_pokertracker(2, "PTthread iteration [%d] had started\n", ++iteration);
		pParent->SetHandsStat();
		if (!pParent->_connected)
			pParent->Connect();
	
		/* Count the number of players */ 
		players = 0;
		if (pParent->_connected && PQstatus(pParent->_pgconn) == CONNECTION_OK)
		{
			if (sym_issittingin || sym_ismanual)
			{
				for (chr = 0; chr < k_max_number_of_players; ++chr)
				{
					if (pParent->CheckName(chr, dummy))
					{
						++players;
					}
				}
			}
		}
		write_log_pokertracker(2, "Players count is [%d]\n", players);
		
		//Define sleeptime for current ptrhead iteration
		if (players > 1)
		{
			sleepTime = (int) ((double)(/*prefs.pt_cache_refresh() !!*/ 30 * 1000) / (double)(pt_max * players));
			write_log_pokertracker(2, "sleepTime set to %d\n", sleepTime);
		}
		else
		{
			write_log_pokertracker(2, "Not enough players to justify iteration, sleeping 10 seconds...\n");
			LightSleep(10000, pParent);
			continue;
		}
		
		if (pParent->_connected && PQstatus(pParent->_pgconn) == CONNECTION_OK)
		{
			for (chr = 0; chr < k_max_number_of_players; ++chr)
			{
				GetStatsForChair(pParam, chr, sleepTime);
				/* Verify therad_stop is false */ 
				if (LightSleep(0, pParent)) 
					break; 
			}
		}
		iterEnd = clock();
		iterDurationMS = (int) ((double)(iterEnd - iterStart));
		write_log_pokertracker(2, "PTthread iteration [%d] had ended, duration time in ms: [%d]\n", ++iteration, iterDurationMS);
		if (iterDurationMS <= 10000)
		{
			write_log_pokertracker(3, "sleeping [%d] ms because iteration was too quick.\n", 10000 - iterDurationMS);
			if (LightSleep(10000 - iterDurationMS, pParent)) 
				break; 
		}
	}
	// Set event
	write_log_pokertracker(3, "PokertrackerThreadFunction: outside while loop...\n");
	::SetEvent(pParent->_m_wait_thread);
	return 0;
}

/*Sleeps but wakes up on stop thread event
We use this function since we never want the thread to ignore the stop_thread event while it's sleeping*/
int	CPokerTrackerThread::LightSleep(int sleepTime, CPokerTrackerThread *pParent)
{
	write_log_pokertracker(3, "LightSleep: called with sleepTime[%d]\n", sleepTime);
	if ( sleepTime > 0)
	{
		int i = 0;
		int iterations = 20;
		int sleepSlice = (int) ((double)sleepTime / (double)iterations);
		for (i = 0; i < iterations; ++i)
		{
			Sleep(sleepSlice);
			if (::WaitForSingleObject(pParent->_m_stop_thread, 0) == WAIT_OBJECT_0)
			{
				write_log_pokertracker(3, "LightSleep: _m_stop_thread signal received\n");
				return 1;
			}
		}
	}
	else
	{
		if (::WaitForSingleObject(pParent->_m_stop_thread, 0) == WAIT_OBJECT_0)
		{
			write_log_pokertracker(3, "LightSleep: _m_stop_thread signal received\n");
			return 1;
		}
	}
	return 0;
}