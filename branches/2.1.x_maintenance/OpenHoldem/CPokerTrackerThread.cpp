#include "StdAfx.h"
#include <process.h>
#include <comdef.h>

#include "CSymbols.h"
#include "CPokerTrackerThread.h"
#include "CPreferences.h"
#include "CGameState.h"
#include "CLevDistance.h"
#include "..\CTablemap\CTablemap.h"

CPokerTrackerThread	*p_pokertracker_thread = NULL;
CPokerTrackerLookup pt_lookup;

CPokerTrackerLookup::CPokerTrackerLookup()
{
	_pt2_siteid.clear();
	_pt3_siteid.clear();
	
	// Documentation about PT3 sited_IDs:
	// http://www.pokertracker.com/forums/viewtopic.php?f=18&t=20169&p=95629
	// All sitenames and networknames have to be in LOWER-CASES!
	_pt2_siteid.insert(std::pair<CString, int> ("stars", 2));
	_pt2_siteid.insert(std::pair<CString, int> ("party", 3));
	_pt2_siteid.insert(std::pair<CString, int> ("ultimate", 4));
	_pt2_siteid.insert(std::pair<CString, int> ("absolute", 5));
	_pt2_siteid.insert(std::pair<CString, int> ("microgaming", 6));
	_pt2_siteid.insert(std::pair<CString, int> ("ongame", 7));
	_pt2_siteid.insert(std::pair<CString, int> ("cryptologic", 8));
	_pt2_siteid.insert(std::pair<CString, int> ("pacific", 9));
	_pt2_siteid.insert(std::pair<CString, int> ("fulltilt", 11));
	_pt2_siteid.insert(std::pair<CString, int> ("b2b", 12));
	_pt2_siteid.insert(std::pair<CString, int> ("tribeca", 13));
	_pt2_siteid.insert(std::pair<CString, int> ("worldpex", 14));
	_pt2_siteid.insert(std::pair<CString, int> ("ipoker", 15));
	_pt2_siteid.insert(std::pair<CString, int> ("tain", 16));
	_pt2_siteid.insert(std::pair<CString, int> ("bodog", 17));
	_pt2_siteid.insert(std::pair<CString, int> ("everest", 18));
	_pt2_siteid.insert(std::pair<CString, int> ("boss", 19));
	_pt2_siteid.insert(std::pair<CString, int> ("betfair", 20));

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
	CString version = prefs.pt_version();
	CString network = p_tablemap->network();

	// Input parameter "version" needs to be "2" or "3" for PT version 2 or PT version 3, respectively
	if (version!="2" && version!="3")
		return -1;

	if (p_symbols->sym()->isppro)
	{
		return 3;
	}
	else
	{
		// If we are using manual mode, we expect an exact match on the lookup
		if (p_symbols->sym()->ismanual)
		{
			std::map<CString, int>::const_iterator lookup, end;

			if (version=="3")
			{
				lookup = _pt3_siteid.find(network.GetString());
				end = _pt3_siteid.end();
			}
			else
			{
				lookup = _pt2_siteid.find(network.GetString());
				end = _pt2_siteid.end();
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

			//Is s$sitename one of the supported PT sites?  Return the proper site_id for db queries.
			if (version=="3")
			{
				lookup = _pt3_siteid.begin();
				end = _pt3_siteid.end();
			}
			else
			{
				lookup = _pt2_siteid.begin();
				end = _pt2_siteid.end();
			}

			while (lookup!=end)
			{
				CString	sym = "sitename$" + lookup->first;
				int e = SUCCESS;

				if (p_symbols->GetSymbolVal(sym.MakeLower().GetString(), &e))
					return lookup->second;

				lookup++;
			}

			//Is s$network one of the supported PT sites?  Return the proper site_id for db queries.
			if (version=="3")
			{
				lookup = _pt3_siteid.begin();
				end = _pt3_siteid.end();
			}
			else
			{
				lookup = _pt2_siteid.begin();
				end = _pt2_siteid.end();
			}

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
	int				i = 0, j = 0;

	// Initialize variables
	_pt_thread = NULL;

	_conn_str.Format("host=%s port=%s user=%s password=%s dbname='%s'",  
		prefs.pt_ip_addr(), prefs.pt_port(), prefs.pt_user(), prefs.pt_pass(), prefs.pt_dbname());

	for (i=0; i<=9; i++)
	{
		for (j=pt_min; j<=pt_max; j++)
		{
			_player_stats[i].stat[j] = -1.0 ;
			_player_stats[i].t_elapsed[j] = -1 ;
		}
		_player_stats[i].found = false ;
		strcpy_s(_player_stats[i].pt_name, 30, "") ;
		strcpy_s(_player_stats[i].scraped_name, 30, "") ;
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

const double CPokerTrackerThread::ProcessQuery (const char * s)
{
	int		sym_raischair = (int) p_symbols->sym()->raischair;

	if (!_connected || PQstatus(_pgconn) != CONNECTION_OK)  
		return 0.0;

	if	    (memcmp(s,"pt_iconlastr",12)==0)		return GetStat(p_game_state->LastRaised(s[12]-'0'), pt_icon);
	else if (memcmp(s,"pt_icon",7)==0)				return GetStat(s[7]-'0', pt_icon);
	else if (memcmp(s,"pt_pfr",6)==0)				return GetStat(s[6]-'0', pt_pfr);
	else if (memcmp(s,"pt_aggtotnopf",13)==0)		return GetStat(s[13]-'0', pt_aggtotnopf);
	else if (memcmp(s,"pt_aggtot",9)==0)			return GetStat(s[9]-'0', pt_aggtot);
	else if (memcmp(s,"pt_aggp",7)==0)				return GetStat(s[7]-'0', pt_aggp);
	else if (memcmp(s,"pt_aggf",7)==0)				return GetStat(s[7]-'0', pt_aggf);
	else if (memcmp(s,"pt_aggt",7)==0)				return GetStat(s[7]-'0', pt_aggt);
	else if (memcmp(s,"pt_aggr",7)==0)				return GetStat(s[7]-'0', pt_aggr);
	else if (memcmp(s,"pt_floppct",10)==0)			return GetStat(s[10]-'0', pt_floppct);
	else if (memcmp(s,"pt_turnpct",10)==0)			return GetStat(s[10]-'0', pt_turnpct);
	else if (memcmp(s,"pt_riverpct",11)==0)			return GetStat(s[11]-'0', pt_riverpct);
	else if (memcmp(s,"pt_vpip",7)==0)				return GetStat(s[7]-'0', pt_vpip);
	else if (memcmp(s,"pt_hands",8)==0)				return GetStat(s[8]-'0', pt_hands);
	else if (memcmp(s,"pt_pf_rfi",9)==0)			return GetStat(s[9]-'0', pt_pf_rfi);
	else if (memcmp(s,"pt_pf_cr",8)==0)				return GetStat(s[8]-'0', pt_pf_cr);
	else if (memcmp(s,"pt_pfats",8)==0)				return GetStat(s[8]-'0', pt_pfats);
	else if (memcmp(s,"pt_wsdp",7)==0)				return GetStat(s[7]-'0', pt_wsdp);
	else if (memcmp(s,"pt_wssd",7)==0)				return GetStat(s[7]-'0', pt_wssd);
	else if (memcmp(s,"pt_fbbts",8)==0)				return GetStat(s[8]-'0', pt_fbbts);
	else if (memcmp(s,"pt_fsbts",8)==0)				return GetStat(s[8]-'0', pt_fsbts);

	else if (memcmp(s,"pt_ricon",8)==0)				return GetStat(sym_raischair, pt_icon);
	else if (memcmp(s,"pt_rpfr",7)==0)				return GetStat(sym_raischair, pt_pfr);
	else if (memcmp(s,"pt_raggtotnopf",14)==0)		return GetStat(sym_raischair, pt_aggtotnopf);
	else if (memcmp(s,"pt_raggtot",10)==0)			return GetStat(sym_raischair, pt_aggtot);
	else if (memcmp(s,"pt_raggp",8)==0)				return GetStat(sym_raischair, pt_aggp);
	else if (memcmp(s,"pt_raggf",8)==0)				return GetStat(sym_raischair, pt_aggf);
	else if (memcmp(s,"pt_raggt",8)==0)				return GetStat(sym_raischair, pt_aggt);
	else if (memcmp(s,"pt_raggr",8)==0)				return GetStat(sym_raischair, pt_aggr);
	else if (memcmp(s,"pt_rfloppct",11)==0)			return GetStat(sym_raischair, pt_floppct);
	else if (memcmp(s,"pt_rturnpct",11)==0)			return GetStat(sym_raischair, pt_turnpct);
	else if (memcmp(s,"pt_rriverpct",12)==0)		return GetStat(sym_raischair, pt_riverpct);
	else if (memcmp(s,"pt_rvpip",8)==0)				return GetStat(sym_raischair, pt_vpip);
	else if (memcmp(s,"pt_rhands",9)==0)			return GetStat(sym_raischair, pt_hands);
	else if (memcmp(s,"pt_rpf_rfi",10)==0)			return GetStat(sym_raischair, pt_pf_rfi);
	else if (memcmp(s,"pt_rpf_cr",9)==0)			return GetStat(sym_raischair, pt_pf_cr);
	else if (memcmp(s,"pt_rpfats",9)==0)			return GetStat(sym_raischair, pt_pfats);
	else if (memcmp(s,"pt_rwsdp",8)==0)				return GetStat(sym_raischair, pt_wsdp);
	else if (memcmp(s,"pt_rwssd",8)==0)				return GetStat(sym_raischair, pt_wssd);
	else if (memcmp(s,"pt_rfbbts",9)==0)			return GetStat(sym_raischair, pt_fbbts);
	else if (memcmp(s,"pt_rfsbts",9)==0)			return GetStat(sym_raischair, pt_fsbts);

	else if (memcmp(s,"ptt_iconlastr",13)==0)		return GetStat(p_game_state->LastRaised(s[13]-'0'), ptt_icon);
	else if (memcmp(s,"ptt_icon",8)==0)				return GetStat(s[8]-'0', ptt_icon);
	else if (memcmp(s,"ptt_pfr",7)==0)				return GetStat(s[7]-'0', ptt_pfr);
	else if (memcmp(s,"ptt_aggtotnopf",14)==0)		return GetStat(s[14]-'0', ptt_aggtotnopf);
	else if (memcmp(s,"ptt_aggtot",10)==0)			return GetStat(s[10]-'0', ptt_aggtot);
	else if (memcmp(s,"ptt_aggp",8)==0)				return GetStat(s[8]-'0', ptt_aggp);
	else if (memcmp(s,"ptt_aggf",8)==0)				return GetStat(s[8]-'0', ptt_aggf);
	else if (memcmp(s,"ptt_aggt",8)==0)				return GetStat(s[8]-'0', ptt_aggt);
	else if (memcmp(s,"ptt_aggr",8)==0)				return GetStat(s[8]-'0', ptt_aggr);
	else if (memcmp(s,"ptt_floppct",11)==0)			return GetStat(s[11]-'0', ptt_floppct);
	else if (memcmp(s,"ptt_turnpct",11)==0)			return GetStat(s[11]-'0', ptt_turnpct);
	else if (memcmp(s,"ptt_riverpct",12)==0)		return GetStat(s[12]-'0', ptt_riverpct);
	else if (memcmp(s,"ptt_vpip",8)==0)				return GetStat(s[8]-'0', ptt_vpip);
	else if (memcmp(s,"ptt_hands",9)==0)			return GetStat(s[9]-'0', ptt_hands);
	else if (memcmp(s,"ptt_pf_rfi",10)==0)			return GetStat(s[10]-'0', ptt_pf_rfi);
	else if (memcmp(s,"ptt_pf_cr",9)==0)			return GetStat(s[9]-'0', ptt_pf_cr);
	else if (memcmp(s,"ptt_pfats",9)==0)			return GetStat(s[9]-'0', ptt_pfats);
	else if (memcmp(s,"ptt_wsdp",8)==0)				return GetStat(s[8]-'0', ptt_wsdp);
	else if (memcmp(s,"ptt_wssd",8)==0)				return GetStat(s[8]-'0', ptt_wssd);
	else if (memcmp(s,"ptt_fbbts",9)==0)			return GetStat(s[9]-'0', ptt_fbbts);
	else if (memcmp(s,"ptt_fsbts",9)==0)			return GetStat(s[9]-'0', ptt_fsbts);

	else if (memcmp(s,"ptt_ricon",9)==0)			return GetStat(sym_raischair, ptt_icon);
	else if (memcmp(s,"ptt_rpfr",8)==0)				return GetStat(sym_raischair, ptt_pfr);
	else if (memcmp(s,"ptt_raggtotnopf",15)==0)		return GetStat(sym_raischair, ptt_aggtotnopf);
	else if (memcmp(s,"ptt_raggtot",11)==0)			return GetStat(sym_raischair, ptt_aggtot);
	else if (memcmp(s,"ptt_raggp",9)==0)			return GetStat(sym_raischair, ptt_aggp);
	else if (memcmp(s,"ptt_raggf",9)==0)			return GetStat(sym_raischair, ptt_aggf);
	else if (memcmp(s,"ptt_raggt",9)==0)			return GetStat(sym_raischair, ptt_aggt);
	else if (memcmp(s,"ptt_raggr",9)==0)			return GetStat(sym_raischair, ptt_aggr);
	else if (memcmp(s,"ptt_rfloppct",12)==0)		return GetStat(sym_raischair, ptt_floppct);
	else if (memcmp(s,"ptt_rturnpct",12)==0)		return GetStat(sym_raischair, ptt_turnpct);
	else if (memcmp(s,"ptt_rriverpct",13)==0)		return GetStat(sym_raischair, ptt_riverpct);
	else if (memcmp(s,"ptt_rvpip",9)==0)			return GetStat(sym_raischair, ptt_vpip);
	else if (memcmp(s,"ptt_rhands",10)==0)			return GetStat(sym_raischair, ptt_hands);
	else if (memcmp(s,"ptt_rpf_rfi",11)==0)			return GetStat(sym_raischair, ptt_pf_rfi);
	else if (memcmp(s,"ptt_rpf_cr",10)==0)			return GetStat(sym_raischair, ptt_pf_cr);
	else if (memcmp(s,"ptt_rpfats",10)==0)			return GetStat(sym_raischair, ptt_pfats);
	else if (memcmp(s,"ptt_rwsdp",8)==0)			return GetStat(sym_raischair, ptt_wsdp);
	else if (memcmp(s,"ptt_rwssd",8)==0)			return GetStat(sym_raischair, ptt_wssd);
	else if (memcmp(s,"ptt_rfbbts",9)==0)			return GetStat(sym_raischair, ptt_fbbts);
	else if (memcmp(s,"ptt_rfsbts",9)==0)			return GetStat(sym_raischair, ptt_fsbts);

	else return 0.0;
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

bool CPokerTrackerThread::CheckName (int m_chr)
{
	int			i = 0;
	char		oh_scraped_name[30] = {0}, best_name[30] = {0}, likename[100] = {0};
	bool		result = false, ok_scrape = false;

	if (m_chr<0 || m_chr>9)
		return false;

	if (p_game_state->state((p_game_state->state_index()-1)&0xff)->m_player[m_chr].m_name_known == 0)
		return false;

	strcpy_s(oh_scraped_name, 30, p_game_state->state((p_game_state->state_index()-1)&0xff)->m_player[m_chr].m_name);

	// Check for bad name scrape
	for (i=0; i<(int) strlen(oh_scraped_name); i++)
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
	//if (_player_stats[m_chr].found && strcmp(_player_stats[m_chr].scraped_name, oh_scraped_name)==0)
	//	return true;

	// We think we have the name, but it has changed since we last checked...reset stats for this
	// chair and search again
	if (_player_stats[m_chr].found && strcmp(_player_stats[m_chr].scraped_name, oh_scraped_name)!=0)
	{
		for (i=pt_min; i<=pt_max; i++)
		{
			_player_stats[m_chr].stat[i] = -1.0 ;
			_player_stats[m_chr].t_elapsed[i] = -1 ;
		}

		_player_stats[m_chr].found = false ;
		strcpy_s(_player_stats[m_chr].pt_name, 30, "") ;
		strcpy_s(_player_stats[m_chr].scraped_name, 30, "") ;
	}

	// We have not found the name in PT, go find it
	// First see if we can find the exact scraped name
	result = QueryName(oh_scraped_name, oh_scraped_name, best_name);

	// Query with "%n%a%m%e%"
	if (!result)
	{
		likename[0]='%';
		for (i=0; i<(int) strlen(oh_scraped_name); i++)
		{
			likename[i*2+1]=oh_scraped_name[i];
			likename[i*2+2]='%';
		}

		likename[(i-1)*2+3]='\0';
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
		strcpy_s(_player_stats[m_chr].pt_name, 30, best_name);
		strcpy_s(_player_stats[m_chr].scraped_name, 30, oh_scraped_name);
	}
	else
	{
		_player_stats[m_chr].found = false ;
		strcpy_s(_player_stats[m_chr].pt_name, 30, "");
		strcpy_s(_player_stats[m_chr].scraped_name, 30, "");
	}

	return result;
}

double CPokerTrackerThread::GetStat (int m_chr, PT_Stats stat)
{
	double		x = 0.;

	if (m_chr<0 || m_chr>9)
		return 0.0;

	return _player_stats[m_chr].stat[stat];
}

double CPokerTrackerThread::UpdateStat (int m_chr, int stat)
{
	PGresult	*res = NULL;
	char		strQry[2000] = {0}, strQry1[2000] = {0}, strQry2[2000] = {0};
	const char	*n = NULL;
	double		result = -1.0;
	char		siteidstr[5] = {0};
	int			siteid = 0;

	int sym_elapsed = (int) p_symbols->sym()->elapsed;

	//No more unnecessary queries when we don't even have a siteid to check
	siteid = pt_lookup.GetSiteId();
	if (siteid == -1)
		return result;

	if (!_connected || PQstatus(_pgconn) != CONNECTION_OK)
		return result;

	if (m_chr<0 || m_chr>9 || stat<pt_min || stat>pt_max)
		return result;

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
		if (prefs.pt_version()=="3")
			strcpy_s(strQry, 2000, query_str3[stat]);
		else
			strcpy_s(strQry, 2000, query_str2[stat]);

		// Insert the player name in the query string
		strcpy_s(strQry1, 2000, strQry);  // move the query into temp str 1
		while ((n=strstr(strQry1, "%SCREENNAME%"))!=NULL) // find the token in temp str 1
		{
			strcpy_s(strQry2, 2000, strQry1);  // move the query into temp str 2
			strQry2[n-strQry1]='\0';  // cut off temp str 2 at the beginning of the token
			strcat_s(strQry2, 2000, _player_stats[m_chr].pt_name);  // append the player name to temp str 2
			strcat_s(strQry2, 2000, n+12); // append the portion of temp str 1 after the token to temp str 2
			strcpy_s(strQry, 2000, strQry2); // move temp str 2 into the original query
			strcpy_s(strQry1, 2000, strQry);  // move the query into temp str 1
		}

		// Insert the site id in the query string
		sprintf_s(siteidstr, 5, "%d", siteid);
		strcpy_s(strQry1, 2000, strQry);  // move the query into temp str 1
		while ((n=strstr(strQry1, "%SITEID%"))!=NULL)   // find the token in temp str 1
		{
			strcpy_s(strQry2, 2000, strQry1);  // move the query into temp str 2
			strQry2[n-strQry1]='\0';  // cut off temp str 2 at the beginning of the token
			strcat_s(strQry2, 2000, siteidstr);  // append the site id to temp str 2
			strcat_s(strQry2, 2000, n+8); // append the portion of temp str 1 after the token to temp str 2
			strcpy_s(strQry, 2000, strQry2); // move temp str 2 into the original query
			strcpy_s(strQry1, 2000, strQry);  // move the query into temp str 1
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
	char		strQry[1000] = {0};
	int			i = 0, lev_dist = 0, bestLD = 0, bestLDindex = 0;
	PGresult	*res = NULL;
	char		siteidstr[5] = {0};
	bool		result = false;
	CLevDistance	myLD;
	int			siteid = 0;
	static int	_last_siteid = -1;

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

	if (strlen(query_name)==0)
		return false;

	sprintf_s(siteidstr, 5, "%d", siteid);

	// PT version 3 name query
	if (prefs.pt_version()=="3")
	{
		strcpy_s(strQry, 1000, "SELECT player_name FROM player WHERE player_name like '");
		strcat_s(strQry, 1000, query_name);
		strcat_s(strQry, 1000, "' AND id_site=");
		strcat_s(strQry, 1000, siteidstr);
	}
	// PT version 2 name query
	else
	{
		strcpy_s(strQry, 1000, "SELECT screen_name FROM players WHERE screen_name like '");
		strcat_s(strQry, 1000, query_name);
		strcat_s(strQry, 1000, "' AND main_site_id=");
		strcat_s(strQry, 1000, siteidstr);
	}

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
		strcpy_s(best_name, 30, PQgetvalue(res, 0, 0));
		result = true;
	}

	// We got more than one tuple, figure the Levenshtein distance for all of them
	// and return the best
	else if ((PQntuples(res) > 1))
	{
		bestLD = 999;
		for (i=0; i<PQntuples(res); i++)
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
			strcpy_s(best_name, 30, PQgetvalue(res, bestLDindex, 0));
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
	int		i = 0, j = 0;

	for (i=0; i<=9; i++)
	{
		for (j=pt_min; j<=pt_max; j++)
		{
			_player_stats[i].stat[j] = -1.0;
			_player_stats[i].t_elapsed[j] = -1;
		}

		_player_stats[i].found = false ;
		strcpy_s(_player_stats[i].pt_name, 30, "");
		strcpy_s(_player_stats[i].scraped_name, 30, "");
	}
}

UINT CPokerTrackerThread::PokertrackerThreadFunction(LPVOID pParam)
{
	CPokerTrackerThread *pParent = static_cast<CPokerTrackerThread*>(pParam);

	int		i = 0, j = 0;

	bool sym_issittingin = (bool) p_symbols->sym()->issittingin;
	bool sym_isppro = (bool) p_symbols->sym()->isppro;
	bool sym_ismanual = (bool) p_symbols->sym()->ismanual;

	while (::WaitForSingleObject(pParent->_m_stop_thread, 0) != WAIT_OBJECT_0)
	{

		if (!pParent->_connected)
			pParent->Connect();

		if (pParent->_connected && PQstatus(pParent->_pgconn) == CONNECTION_OK)
		{
			for (i=0; i<=9; i++)
			{
				if (sym_issittingin || sym_isppro || sym_ismanual)
				{
					if (pParent->CheckName(i))
					{

						for (j=pt_min; j<=pt_max; j++)
						{
							pParent->UpdateStat(i, j);
						}
					}
				}
			}
		}

		for (i=0; i < prefs.pt_update_delay() && 
				  ::WaitForSingleObject(pParent->_m_stop_thread, 0)!=WAIT_OBJECT_0; i++)
			Sleep(1000);
	}

	// Set event
	::SetEvent(pParent->_m_wait_thread);
	return 0;
}
