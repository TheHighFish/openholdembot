#include "StdAfx.h"
#include "CPokerTrackerThread.h"

#include <assert.h>
#include <process.h>
#include <comdef.h>
#include "CSymbols.h"
#include "CPreferences.h"
#include "CGameState.h"
#include "CLevDistance.h"
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
}

CPokerTrackerLookup::~CPokerTrackerLookup()
{
}

const int CPokerTrackerLookup::GetSiteId()
{
	CString network = p_tablemap->network();

	if (p_symbols->sym()->isppro)
	{
		// Treat PokerPro-Server as PartyPoker.
		return 200;
	}
	else
	{
		// If we are using manual mode, we expect an exact match on the lookup
		if (p_symbols->sym()->ismanual)
		{
			std::map<CString, int>::const_iterator lookup, end;

			// Lookup site-id for version 3
			{
				lookup = _pt3_siteid.find(network.GetString());
				end    = _pt3_siteid.end();
			}

			if (lookup==end)
				return -1;
			else
				return lookup->second;
		}

		// If this is a regular scraped table, a match is valid for a substring match if sitename or network
		else
		{
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
		}
	}
	return -1 ;
}


CPokerTrackerThread::CPokerTrackerThread()
{
	// Initialize variables
	_pt_thread = NULL;

	_conn_str.Format("host=%s port=%s user=%s password=%s dbname='%s'",  
		prefs.pt_ip_addr(), prefs.pt_port(), prefs.pt_user(), prefs.pt_pass(), prefs.pt_dbname());

	for (int i=0; i<k_max_number_of_players; i++)
	{
		for (int j=pt_min; j<=pt_max; j++)
		{
			_player_stats[i].stat[j] = -1.0 ;
			_player_stats[i].t_elapsed[j] = -1 ;
		}
		_player_stats[i].found = false ;
		strcpy_s(_player_stats[i].pt_name, k_max_length_of_playername, "") ;
		strcpy_s(_player_stats[i].scraped_name, k_max_length_of_playername, "") ;
	}

	_connected = false;
	_m_stop_thread = NULL;
	_m_wait_thread = NULL;
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

		write_log_pokertracker(1, "Started Poker Tracker thread.\n");
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

		write_log_pokertracker(1, "Stopped Poker Tracker thread.\n");
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
	OH_MessageBox(error_message, "Error", 0);
}

const double CPokerTrackerThread::ProcessQuery (const char * s)
{
	int		sym_raischair = (int) p_symbols->sym()->raischair;

	if (!_connected || PQstatus(_pgconn) != CONNECTION_OK)
	{
		OH_MessageBox("Not connected to PokerTracker database.\n"
			"Can't use PokerTracker symbols.", "Error", 0);
		return -1.0;
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
		write_log_pokertracker(1, "PostgreSQL DB opened successfully <%s/%s/%s>\n", 
			prefs.pt_ip_addr(), 
			prefs.pt_port(), 
			prefs.pt_dbname());

		_connected = true;
	}
	else
	{
		write_log_pokertracker(1, "ERROR opening PostgreSQL DB: %s\n\n", PQerrorMessage(_pgconn));
		PQfinish(_pgconn);
		_connected = false;
	}
}

void CPokerTrackerThread::Disconnect(void)
{
	_connected = false;

	if (PQstatus(_pgconn) == CONNECTION_OK)
		PQfinish(_pgconn);
}

//!!!
#define k_min_chair_number 0
#define k_max_chair_number 9

bool CPokerTrackerThread::CheckName (int m_chr)
{
	char		oh_scraped_name[k_max_length_of_playername] = {0}; 
	char		best_name[k_max_length_of_playername] = {0};
	char		likename[k_max_length_of_playername] = {0};
	bool		result = false, ok_scrape = false;

	assert(m_chr >= k_min_chair_number); 
	assert(m_chr <= k_max_chair_number);

	if (p_game_state->state((p_game_state->state_index()-1)&0xff)->m_player[m_chr].m_name_known == 0)
		return false;

	strcpy_s(oh_scraped_name, k_max_length_of_playername, p_game_state->state((p_game_state->state_index()-1)&0xff)->m_player[m_chr].m_name);

	// Check for bad name scrape
	for (int i=0; i<(int) strlen(oh_scraped_name); i++)
	{
		if (oh_scraped_name[i]!='l' && oh_scraped_name[i]!='i' && oh_scraped_name[i]!='.' && oh_scraped_name[i]!=',')
		{
			ok_scrape = true;
			i = strlen(oh_scraped_name);
		}
	}
	if (!ok_scrape)
		return false;

	// We already have the name, and it has not changed since we last checked, so do nothing
	//if (_player_stats[m_chr].found && strcmp(_player_stats[m_chr].scraped_name, oh_scraped_name))
	//	return true;

	// We think we have the name, but it has changed since we last checked...reset stats for this
	// chair and search again
	if (_player_stats[m_chr].found && strcmp(_player_stats[m_chr].scraped_name, oh_scraped_name)!=0)
	{
		for (int i=pt_min; i<=pt_max; i++)
		{
			_player_stats[m_chr].stat[i] = -1.0 ;
			_player_stats[m_chr].t_elapsed[i] = -1 ;
		}

		_player_stats[m_chr].found = false ;
		strcpy_s(_player_stats[m_chr].pt_name, k_max_length_of_playername, "") ;
		strcpy_s(_player_stats[m_chr].scraped_name, k_max_length_of_playername, "") ;
	}

	// We have not found the name in PT, go find it
	// First see if we can find the exact scraped name
	result = QueryName(oh_scraped_name, oh_scraped_name, best_name);

	// Query with "%n%a%m%e%"
	if (!result)
	{
		likename[0]='%';
		for (int i=0; i<(int) strlen(oh_scraped_name); i++)
		{
			likename[i*2+1]=oh_scraped_name[i];
			likename[i*2+2]='%';
		}
		likename[strlen(oh_scraped_name)*2+1]='\0';
		result = QueryName(likename, oh_scraped_name, best_name);
	}

	// Query with "%"
	if (!result)
	{
		result = QueryName("%", oh_scraped_name, best_name);
	}

	if (result)
	{
		_player_stats[m_chr].found = true;
		strcpy_s(_player_stats[m_chr].pt_name, k_max_length_of_playername, best_name);
		strcpy_s(_player_stats[m_chr].scraped_name, k_max_length_of_playername, oh_scraped_name);
	}
	else
	{
		_player_stats[m_chr].found = false ;
		strcpy_s(_player_stats[m_chr].pt_name, k_max_length_of_playername, "");
		strcpy_s(_player_stats[m_chr].scraped_name, k_max_length_of_playername, "");
	}

	return result;
}

double CPokerTrackerThread::GetStat (int m_chr, PT_Stats stat)
{
	assert(m_chr >= k_min_chair_number); 
	assert(m_chr <= k_max_chair_number);

	return _player_stats[m_chr].stat[stat];
}

double CPokerTrackerThread::UpdateStat (int m_chr, int stat)
{
	PGresult	*res = NULL;
	char		strQry[k_max_length_of_query] = {0};
	char		strQry1[k_max_length_of_query] = {0};
	char		strQry2[k_max_length_of_query] = {0};
	const char	*n = NULL;
	double		result = -1.0;
	char		siteidstr[k_max_length_of_site_id] = {0};
	int			siteid = 0;

	int sym_elapsed = (int) p_symbols->sym()->elapsed;

	//No more unnecessary queries when we don't even have a siteid to check
	siteid = pt_lookup.GetSiteId();
	if (siteid == -1)
		return result;

	if (!_connected || PQstatus(_pgconn) != CONNECTION_OK)
		return result;

	assert(m_chr >= k_min_chair_number);
	assert(m_chr <= k_max_chair_number);
	assert(stat >= pt_min);
	assert(stat <= pt_max);

	// If we already have stats cached for the player, the timeout has not expired,
	// return the value from the cache...
	if (sym_elapsed - _player_stats[m_chr].t_elapsed[stat] < prefs.pt_cache_refresh() &&
		_player_stats[m_chr].t_elapsed[stat] != -1 &&
		_player_stats[m_chr].stat[stat] != -1)
	{
		result = _player_stats[m_chr].stat[stat];
	}

	// ...otherwise query the database
	else
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
		try
		{
			// See if we can find the player name in the database
			write_log_pokertracker(3, "Querying %s for m_chr %d: %s\n", stat_str[stat], m_chr, strQry);
			res = PQexec(_pgconn, strQry);
		}
		catch (_com_error &e)
		{
			write_log_pokertracker(3, "ERROR\n");
			write_log_pokertracker(3, _T("\tCode = %08lx\n"), e.Error());
			write_log_pokertracker(3, _T("\tCode meaning = %s\n"), e.ErrorMessage());
			_bstr_t bstrSource(e.Source());
			_bstr_t bstrDescription(e.Description());
			write_log_pokertracker(3, _T("\tSource = %s\n"), (LPCTSTR) bstrSource);
			write_log_pokertracker(3, _T("\tDescription = %s\n"), (LPCTSTR) bstrDescription);
			write_log_pokertracker(3, _T("\tQuery = [%s]\n"), strQry);
		}

		// Check query return code
		if (PQresultStatus(res) != PGRES_TUPLES_OK)
		{
			switch (PQresultStatus(res))
			{
			case PGRES_COMMAND_OK:
				write_log_pokertracker(3, "PGRES_COMMAND_OK: %s [%s]\n", PQerrorMessage(_pgconn), strQry);
				break;
			case PGRES_EMPTY_QUERY:
				write_log_pokertracker(3, "PGRES_EMPTY_QUERY: %s [%s]\n", PQerrorMessage(_pgconn), strQry);
				break;
			case PGRES_BAD_RESPONSE:
				write_log_pokertracker(3, "PGRES_BAD_RESPONSE: %s [%s]\n", PQerrorMessage(_pgconn), strQry);
				break;
			case PGRES_COPY_OUT:
				write_log_pokertracker(3, "PGRES_COPY_OUT: %s [%s]\n", PQerrorMessage(_pgconn), strQry);
				break;
			case PGRES_COPY_IN:
				write_log_pokertracker(3, "PGRES_COPY_IN: %s [%s]\n", PQerrorMessage(_pgconn), strQry);
				break;
			case PGRES_NONFATAL_ERROR:
				write_log_pokertracker(3, "PGRES_NONFATAL_ERROR: %s [%s]\n", PQerrorMessage(_pgconn), strQry);
				break;
			case PGRES_FATAL_ERROR:
				write_log_pokertracker(3, "PGRES_FATAL_ERROR: %s [%s]\n", PQerrorMessage(_pgconn), strQry);
				break;
			default:
				write_log_pokertracker(3, "GENERIC ERROR: %s [%s]\n", PQerrorMessage(_pgconn), strQry);
				break;
			}
		}
		else
		{
			if (PQgetisnull(res,0,0) != 1)
			{
				result = atof(PQgetvalue(res,0,0));
				write_log_pokertracker(2, "Query %s for m_chr %d success: %f\n", stat_str[stat], m_chr, result);
			}
		}

		PQclear(res);

		// update cache with new values
		_player_stats[m_chr].stat[stat] = result;
		_player_stats[m_chr].t_elapsed[stat] = sym_elapsed;
	}

	return result;
}

bool CPokerTrackerThread::QueryName (const char * query_name, const char * scraped_name, char * best_name)
{
	char			strQry[k_max_length_of_query] = {0};
	int				lev_dist = 0, bestLD = 0, bestLDindex = 0;
	PGresult		*res = NULL;
	char			siteidstr[k_max_length_of_site_id] = {0};
	bool			result = false;
	CLevDistance	myLD;
	int				siteid = 0;
	static int		_last_siteid = -1;

	//No more unnecessary queries when we don't even have a siteid to check
	siteid = pt_lookup.GetSiteId();
	if (siteid == -1)  return false;

	// siteid has changed -- we're using ManualMode
	if (siteid != _last_siteid)
	{
		ClearStats();
		_last_siteid = siteid;
	}

	if (!_connected || PQstatus(_pgconn) != CONNECTION_OK)
		return false;

	if (strlen(query_name))
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
		write_log_pokertracker(3, "Postgres Query error:\n");
		write_log_pokertracker(3, "\tCode = %08lx\n", e.Error());
		write_log_pokertracker(3, "\tCode meaning = %s\n", e.ErrorMessage());
		_bstr_t bstrSource(e.Source());
		_bstr_t bstrDescription(e.Description());
		write_log_pokertracker(3, "\tSource = %s\n", (LPCTSTR) bstrSource);
		write_log_pokertracker(3, "\tDescription = %s\n", (LPCTSTR) bstrDescription);
		write_log_pokertracker(3, "\tQuery = [%s]\n", strQry);
	}

	// We got nothing, return false
	if (PQntuples(res) == 0)
	{
		result = false;
	}

	// If we get one tuple, all is good - return the one name
	if (PQntuples(res) == 1)
	{
		strcpy_s(best_name, k_max_length_of_playername, PQgetvalue(res, 0, 0));
		result = true;
	}

	// We got more than one tuple, figure the Levenshtein distance for all of them
	// and return the best
	else if ((PQntuples(res) > 1))
	{
		bestLD = 999;
		for (int i=0; i<PQntuples(res); i++)
		{
			lev_dist = myLD.LD(scraped_name, PQgetvalue(res, i, 0));

			if (lev_dist<bestLD && lev_dist<(int)strlen(PQgetvalue(res, i, 0))/2 )
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

void CPokerTrackerThread::ClearStats (void)
{
	for (int i=0; i<=9; i++)
	{
		for (int j=pt_min; j<=pt_max; j++)
		{
			_player_stats[i].stat[j] = -1.0;
			_player_stats[i].t_elapsed[j] = -1;
		}

		_player_stats[i].found = false ;
		strcpy_s(_player_stats[i].pt_name, k_max_length_of_playername, "");
		strcpy_s(_player_stats[i].scraped_name, k_max_length_of_playername, "");
	}
}

void CPokerTrackerThread::GetStatsForChair(LPVOID pParam, int chair)
{
	CPokerTrackerThread *pParent = static_cast<CPokerTrackerThread*>(pParam);

	bool sym_issittingin = (bool) p_symbols->sym()->issittingin;
	bool sym_isppro = (bool) p_symbols->sym()->isppro;
	bool sym_ismanual = (bool) p_symbols->sym()->ismanual;

	while (::WaitForSingleObject(pParent->_m_stop_thread, 0) != WAIT_OBJECT_0)
	{
		if (!pParent->_connected)
			pParent->Connect();

		if (pParent->_connected && PQstatus(pParent->_pgconn) == CONNECTION_OK)
		{
			if (sym_issittingin || sym_isppro || sym_ismanual)
			{
				if (pParent->CheckName(chair))
				{
					for (int i=pt_min; i<=pt_max; i++)
					{
						pParent->UpdateStat(chair, i);
					}
				}
			}
		}
	}
}


UINT CPokerTrackerThread::PokertrackerThreadFunction(LPVOID pParam)
{
	CPokerTrackerThread *pParent = static_cast<CPokerTrackerThread*>(pParam);

	while (::WaitForSingleObject(pParent->_m_stop_thread, 0) != WAIT_OBJECT_0)
	{
		if (!pParent->_connected)
			pParent->Connect();

		if (pParent->_connected && PQstatus(pParent->_pgconn) == CONNECTION_OK)
		{
			for (int chair=0; chair<k_max_number_of_players; chair++)
			{
				GetStatsForChair(pParam, chair);
			}
		}

		for (int i=0; i < prefs.pt_update_delay() && 
				  ::WaitForSingleObject(pParent->_m_stop_thread, 0)!=WAIT_OBJECT_0; i++)
			Sleep(1000);
	}
	// Set event
	::SetEvent(pParent->_m_wait_thread);
	return 0;
}
