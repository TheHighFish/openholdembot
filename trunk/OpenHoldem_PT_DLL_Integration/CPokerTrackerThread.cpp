#include "StdAfx.h"
#include "CPokerTrackerThread.h"

#include <assert.h>
#include <process.h>
#include <comdef.h>
#include "CAutoConnector.h"
#include "CGameState.h"
#include "CLevDistance.h"
#include "CPreferences.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineIsTournament.h"
#include "CSymbolEnginePokerTracker.h"
#include "CSymbolEngineTime.h"
#include "CSymbolEngineUserchair.h"
#include "CSymbols.h"
#include "MagicNumbers.h"
#include "..\CTablemap\CTablemap.h"
#include "MagicNumbers.h"
#include "PokerTracker_Queries_Version_3.h"

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

CString CPokerTrackerThread::CreateConnectionString(
	const CString ip_address, const CString port, const CString username,
	const CString password, const CString DB_name)
{
	CString result;
	result.Format("host=%s port=%s user=%s password=%s dbname='%s'",  
		ip_address, port, username, password, DB_name);
	write_log(prefs.debug_pokertracker(), "[PokerTracker] Connection-string: %s\n", result);
	return result;
}


CPokerTrackerThread::CPokerTrackerThread()
{
	// Initialize variables
	_pt_thread = NULL;

	_conn_str = CreateConnectionString(prefs.pt_ip_addr(), 
		prefs.pt_port(), prefs.pt_user(), prefs.pt_pass(), prefs.pt_dbname());

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
}

void CPokerTrackerThread::SetRingStatsState(bool enabled)
{
	for (int i = pt_min; i <= pt_max; ++i)
	{	
		_m_enabled_stats[i] = enabled;
	}
}

void CPokerTrackerThread::SetStatTypes()
{
	/* Ring symbols */
	for (int i = pt_min; i <= pt_max; ++i)
	{	
		_m_stat_type[i] = pt_statType_Ring;
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

		write_log(prefs.debug_pokertracker(), "Started PokerTracker-thread.\n");
	}
	else
	{
		write_log(prefs.debug_pokertracker(), "PokerTracker-thread already running.\n");
	}
}

void CPokerTrackerThread::StopThread()
{
	if (_pt_thread)
	{
		// Trigger thread to stop
		::SetEvent(_m_stop_thread);

		// Wait until thread finished
		::WaitForSingleObject(_m_wait_thread, k_max_time_to_wait_for_thread_to_shutdown);

		_pt_thread = NULL;

		Disconnect();

		write_log(prefs.debug_pokertracker(), "Stopped Poker Tracker thread.\n");
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

void CPokerTrackerThread::Connect(void)
{
	write_log(prefs.debug_pokertracker(), "Trying to open PostgreSQL DB...\n");
	_pgconn = PQconnectdb(_conn_str.GetString());

	if (PQstatus(_pgconn) == CONNECTION_OK)
	{
		write_log(prefs.debug_pokertracker(), "PostgreSQL DB opened successfully <%s/%s/%s>\n", 
			prefs.pt_ip_addr(), 
			prefs.pt_port(), 
			prefs.pt_dbname());

		_connected = true;
	}
	else
	{
		write_log(prefs.debug_pokertracker(), "ERROR opening PostgreSQL DB: %s\n\n", PQerrorMessage(_pgconn));
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


// !!! to be removed completely
/* When running this function, chair is the chair to IGNORE 
   That's because this function is running while GetStatsForChair is running,
   And we wouldn't like to interrupt its order and ability to detect name changes
   In the seat it's getting stats for*/ 
void CPokerTrackerThread::ReportSeatChanges(int chair)
{
	char currentScrapeName[k_max_length_of_playername];
	write_log(prefs.debug_pokertracker(), "ReportSeatChanges: started\n");
	for (int i = k_first_chair; i < k_last_chair; i++)
	{
		if (i != chair)
		{
			memcpy(currentScrapeName, _player_stats[i].scraped_name, k_max_length_of_playername);
			if (CheckIfNameHasChanged(i))
			{
				/* Scrapped name got changed. Clear stats for that chair */
				write_log(prefs.debug_pokertracker(), "ReportSeatChanges: chair [%d]: new player sat down in chair! oldscrape[%s] newscrape[%s].\n", i, currentScrapeName, _player_stats[i].scraped_name);
				/* Clear stats but leave the new name intact */
				//!!!ClearSeatStats(i, false);
			}
		}
	}
}
/* A scraped name is "bad" if it consists only of characters 
   like "l", "1", "i", "." and "," */
bool CPokerTrackerThread::NameLooksLikeBadScrape(char *oh_scraped_name)
{
	int len = (int) strlen(oh_scraped_name);
	for (int i=0; i<len; i++)
	{
		if (oh_scraped_name[i]    != 'l'
			&& oh_scraped_name[i] != '1'
			&& oh_scraped_name[i] != 'i' 
			&& oh_scraped_name[i] != '.' 
			&& oh_scraped_name[i] != ',')
		{
			return true;
		}
	}
	return false;
}

/* Returns true if found an appropriate name in the DB for chr, or false if 
   it did not found such name.*/
bool CPokerTrackerThread::CheckIfNameExistsInDB(int chair)
{
	char		oh_scraped_name[k_max_length_of_playername]; 
	char		best_name[k_max_length_of_playername];

	assert(chair >= k_first_chair); 
	assert(chair <= k_last_chair);
	
	memset(oh_scraped_name, 0, k_max_length_of_playername);
	memset(best_name, 0, k_max_length_of_playername);
	
	if (p_game_state->state((p_game_state->state_index()-1)&0xff)->m_player[chair].m_name_known == 0)
	{
		return false;
	}
	strcpy_s(oh_scraped_name, k_max_length_of_playername, p_game_state->state((p_game_state->state_index()-1)&0xff)->m_player[chair].m_name);

	if (NameLooksLikeBadScrape(oh_scraped_name))
	{
		return false;
	}
	
	// We already have the name, and it has not changed since we last checked, so do nothing
	if (_player_stats[chair].found && 0 == strcmp(_player_stats[chair].scraped_name, oh_scraped_name))
	{
		return true;
	}
	
	// We have not found the name in PT, go find it
	// First see if we can find the exact scraped name
	if (FindName(oh_scraped_name, best_name))
	{
		SetPlayerName(chair, true, best_name, oh_scraped_name);
		return true;
	}
	else
	{
		SetPlayerName(chair, false, "", "");
		return false;
	}
}

/* Returns true, if the name looks ok, but differs from the last one. */
bool CPokerTrackerThread::CheckIfNameHasChanged(int chair)
{
	char		oh_scraped_name[k_max_length_of_playername]; 
	char		best_name[k_max_length_of_playername];

	assert(chair >= k_first_chair); 
	assert(chair <= k_last_chair);
	
	memset(oh_scraped_name, 0, k_max_length_of_playername);
	memset(best_name, 0, k_max_length_of_playername);
	
	if (p_game_state->state((p_game_state->state_index()-1)&0xff)->m_player[chair].m_name_known == 0)
	{
		return false;
	}
	strcpy_s(oh_scraped_name, k_max_length_of_playername, p_game_state->state((p_game_state->state_index()-1)&0xff)->m_player[chair].m_name);

	if (NameLooksLikeBadScrape(oh_scraped_name))
	{
		return false;
	}

	// We already have the name, and it has not changed since we last checked, so do nothing
	if (_player_stats[chair].found 
		&& 0 == strcmp(_player_stats[chair].scraped_name, oh_scraped_name))
	{
		return false;
	}
	return true;
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
		write_log(prefs.debug_pokertracker(), "SetPlayerName[%d]: Done. ptname[%s] scrapedName[%s]\n", chr, _player_stats[chr].pt_name, _player_stats[chr].scraped_name);
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
	assert(chr >= k_first_chair); 
	assert(chr <= k_last_chair);

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

	assert(m_chr >= k_first_chair);
	assert(m_chr <= k_last_chair);
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
			write_log(prefs.debug_pokertracker(), "Querying %s for m_chr %d: %s\n", stat_str[stat], m_chr, strQry);
			res = PQexec(_pgconn, strQry);
		}
		catch (_com_error &e)
		{
			write_log(prefs.debug_pokertracker(), "ERROR\n");
			write_log(prefs.debug_pokertracker(), _T("\tCode = %08lx\n"), e.Error());
			write_log(prefs.debug_pokertracker(), _T("\tCode meaning = %s\n"), e.ErrorMessage());
			_bstr_t bstrSource(e.Source());
			_bstr_t bstrDescription(e.Description());
			write_log(prefs.debug_pokertracker(), _T("\tSource = %s\n"), (LPCTSTR) bstrSource);
			write_log(prefs.debug_pokertracker(), _T("\tDescription = %s\n"), (LPCTSTR) bstrDescription);
			write_log(prefs.debug_pokertracker(), _T("\tQuery = [%s]\n"), strQry);
		}
		
		updEnd = clock();
		duration = (int) ((double)(updEnd - updStart) / 1000);
		if (duration >= 3)
			write_log(prefs.debug_pokertracker(), "Query time in seconds: [%d]\n", duration);

		// Check query return code
		if (PQresultStatus(res) != PGRES_TUPLES_OK)
		{
			switch (PQresultStatus(res))
			{
			case PGRES_COMMAND_OK:
				write_log(prefs.debug_pokertracker(), "PGRES_COMMAND_OK: %s [%s]\n", PQerrorMessage(_pgconn), strQry);
				break;
			case PGRES_EMPTY_QUERY:
				write_log(prefs.debug_pokertracker(), "PGRES_EMPTY_QUERY: %s [%s]\n", PQerrorMessage(_pgconn), strQry);
				break;
			case PGRES_BAD_RESPONSE:
				write_log(prefs.debug_pokertracker(), "PGRES_BAD_RESPONSE: %s [%s]\n", PQerrorMessage(_pgconn), strQry);
				break;
			case PGRES_COPY_OUT:
				write_log(prefs.debug_pokertracker(), "PGRES_COPY_OUT: %s [%s]\n", PQerrorMessage(_pgconn), strQry);
				break;
			case PGRES_COPY_IN:
				write_log(prefs.debug_pokertracker(), "PGRES_COPY_IN: %s [%s]\n", PQerrorMessage(_pgconn), strQry);
				break;
			case PGRES_NONFATAL_ERROR:
				write_log(prefs.debug_pokertracker(), "PGRES_NONFATAL_ERROR: %s [%s]\n", PQerrorMessage(_pgconn), strQry);
				break;
			case PGRES_FATAL_ERROR:
				write_log(prefs.debug_pokertracker(), "PGRES_FATAL_ERROR: %s [%s]\n", PQerrorMessage(_pgconn), strQry);
				break;
			default:
				write_log(prefs.debug_pokertracker(), "GENERIC ERROR: %s [%s]\n", PQerrorMessage(_pgconn), strQry);
				break;
			}
		}
		else
		{
			if (PQgetisnull(res,0,0) != 1)
			{
				result = atof(PQgetvalue(res,0,0));
				write_log(prefs.debug_pokertracker(), "Query %s for m_chr %d success: %f\n", stat_str[stat], m_chr, result);
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
	double			Levenshtein_distance_matching_factor = 0.2;

	//No more unnecessary queries when we don't even have a siteid to check
	siteid = pt_lookup.GetSiteId();
	if (siteid == k_undefined)  return false;

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
		write_log(prefs.debug_pokertracker(), "Postgres Query error:\n");
		write_log(prefs.debug_pokertracker(), "\tCode = %08lx\n", e.Error());
		write_log(prefs.debug_pokertracker(), "\tCode meaning = %s\n", e.ErrorMessage());
		_bstr_t bstrSource(e.Source());
		_bstr_t bstrDescription(e.Description());
		write_log(prefs.debug_pokertracker(), "\tSource = %s\n", (LPCTSTR) bstrSource);
		write_log(prefs.debug_pokertracker(), "\tDescription = %s\n", (LPCTSTR) bstrDescription);
		write_log(prefs.debug_pokertracker(), "\tQuery = [%s]\n", strQry);
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

// Returns 1 for basic stats only, and 2 for all
int CPokerTrackerThread::GetUpdateType(int chr)
{
	if (_player_stats[chr].skipped_updates == k_advanced_stat_update_every)
	{
		write_log(prefs.debug_pokertracker(), "GetUpdateType: update type for chair [%d] is update ALL\n", chr);
		return pt_updateType_updateAll;
	}
	write_log(prefs.debug_pokertracker(), "GetUpdateType: update type for chair [%d] is update Basic only\n", chr);
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
	_m_handsStats = pt_hands;
	_m_min_hands_for_slower_update = k_min_hands_slower_updates_ring;
	SetRingStatsState(true);
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
	int			updateType;
	char        reason[100];
	int         updatedCount = 0;
	
	if (pParent->CheckIfNameExistsInDB(chair) == false)
	{
		/* Note that checkname fail just when starting, doesn't necessarily mean that there's no user
		   in that chair, but only that the scraper failed to find one. This could be due to lobby window
		   that hides poker window behind it. We make this check once, and if we are good, the update iteration
		   is good to go. if we are not, we assume that this seat is not taken. */ 
		write_log(prefs.debug_pokertracker(), "GetStatsForChair[%d] had been started.\n", chair);
		write_log(prefs.debug_pokertracker(), "GetStatsForChair[%d] had been skipped. Reason: [CheckName failed]\n", chair);
		return;
	}
//!!!	const char* playerscrapedName = pParent->GetPlayerScrapedName(chair);
//!!!	write_log(prefs.debug_pokertracker(), "GetStatsForChair[%d][%s] had been started.\n", chair, playerscrapedName);
	
	/* Check if there's a complete update cycle skipping for that chair */
	updateType = pParent->SkipUpdateForChair(chair, reason);
	if (updateType == pt_updateType_noUpdate)
	{
		write_log(prefs.debug_pokertracker(), "GetStatsForChair for chair [%d] had been skipped. Reason: [%s]\n", chair, reason);
		pParent->RecalcSkippedUpdates(chair);
		return;
	}

	/* Make sure all other seats contain the appropriate players */ 
	pParent->ReportSeatChanges(chair);  

	if (!pParent->_connected)
	{
		pParent->Connect();
	}
		
	if (pParent->_connected && PQstatus(pParent->_pgconn) == CONNECTION_OK)
	{
		if (p_autoconnector->IsConnected())
		{
			for (int i = pt_min; i <= pt_max; i++)
			{
				/* Every few iterations, we need to verify that the seats we already have stats on, 
			       did not change. This task is totally irrelevant for the current function
				   we're on, that is GetStatsForChair. But if we won't do this every now and then,
				   we might find ourselves updating stats for chair 1, for 1 minute, while the player
				   in chair 3 stood up and someone else replaced him. we cannot let this go unnoticed */
				pParent->ReportSeatChanges(chair);
			
				/* CheckName is necessary before each update.
				   There's a short interval between any two updates, and it's possible that the player
				   had stood up during the update process. But it also possible that the poker lobby was 
				   hiding our poker window, or some popup temporarily was over it, and that's why CheckName fails.
				   Since we cannot know which one caused checkname to fail, we would continue to update, as 
				   long as we have a found name, and as long as the name did't get changed. 
				   So what we do care about, is the situation were the name got replaced by another name,
				   in that case, we stop the update for the current chair  			   */ 

				if (pParent->IsFound(chair))
				{
					/* Verify therad_stop is false */ 
					if (LightSleep(0, pParent)) 
						return; 
					/* verify that name did not get changed */
					if (pParent->CheckIfNameHasChanged(i))
					{
						/* Name got changed while we search for stats for current chair
						   Clear stats for this seat and return                   */
						write_log(prefs.debug_pokertracker(), "GetStatsForChair chair [%d] had changed name getting stat for chair. Clearing stats for chair.\n", chair);
						/* Clear stats, but leave the new name intact */
						//!!!pParent->ClearSeatStats(chair, false);
						return;
					}
					if (!pParent->StatEnabled(i))
					{
						/* Skip disabled stats */
						write_log(prefs.debug_pokertracker(), "GetStatsForChair: Updating stats [%d] for chair [%d] had been skipped. Reason: [stat is disabled]\n", i, chair);
					}
					else if (pParent->SkipUpdateCondition(i, chair))
					{
						/* Updating stat i should be skipped this time */
						/* advanced/positional stats are updated every k_advanced_stat_update_every cycles */
//!!!						write_log(prefs.debug_pokertracker(), "GetStatsForChair: Updating stats [%d] for chair [%d] had been skipped. Reason: [advanced/positional stats cycle [%d of %d]]\n", i, chair, pParent->GetSkippedUpdates(chair) , k_advanced_stat_update_every);
					}
					else
					{
						/* Update... */
						write_log(prefs.debug_pokertracker(), "GetStatsForChair updating stats [%d] for chair [%d]...\n", i, chair);
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
					write_log(prefs.debug_pokertracker(), "GetStatsForChair for chair [%d] had been skipped. Reason: [user not found (user stood up?)]\n", chair);
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
	write_log(prefs.debug_pokertracker(), "Updates for chair [%d][%s] had been completed. Total [%d] updated stats\n", chair, _player_stats[chair].scraped_name, updatedCount);
}

bool CPokerTrackerThread::IsFound(int chair)
{
	return _player_stats[chair].found;
}


UINT CPokerTrackerThread::PokertrackerThreadFunction(LPVOID pParam)
{
	CPokerTrackerThread *pParent = static_cast<CPokerTrackerThread*>(pParam);
	int				iteration = 0;
	int				players;
	int				sleepTime;
	clock_t			iterStart, iterEnd;
	int				iterDurationMS;

	while (::WaitForSingleObject(pParent->_m_stop_thread, 0) != WAIT_OBJECT_0)
	{
		iterStart = clock();
		write_log(prefs.debug_pokertracker(), "PTthread iteration [%d] had started\n", ++iteration);
		pParent->SetHandsStat();
		if (!pParent->_connected)
		{
			pParent->Connect();
		}
	
		players = p_symbol_engine_active_dealt_playing->nopponentsplaying() 
			+ (p_symbol_engine_userchair->userchair_confirmed() ? 1 : 0); 
		write_log(prefs.debug_pokertracker(), "Players count is [%d]\n", players);
		
		//Define sleeptime for current ptrhead iteration
		if (players > 1)
		{
			sleepTime = (int) ((double)(/*prefs.pt_cache_refresh() !!*/ 30 * 1000) / (double)(pt_max * players));
			write_log(prefs.debug_pokertracker(), "sleepTime set to %d\n", sleepTime);
		}
		else
		{
			write_log(prefs.debug_pokertracker(), "Not enough players to justify iteration, sleeping 10 seconds...\n");
			LightSleep(10000, pParent);
			continue;
		}
		
		if (pParent->_connected && PQstatus(pParent->_pgconn) == CONNECTION_OK)
		{
			for (int chair = 0; chair < p_tablemap->nchairs(); ++chair)
			{
				GetStatsForChair(pParam, chair, sleepTime);
				/* Verify therad_stop is false */ 
				if (LightSleep(0, pParent)) 
					break; 
			}
		}
		iterEnd = clock();
		iterDurationMS = (int) ((double)(iterEnd - iterStart));
		write_log(prefs.debug_pokertracker(), "PTthread iteration [%d] had ended, duration time in ms: [%d]\n", ++iteration, iterDurationMS);
		if (iterDurationMS <= 10000)
		{
			write_log(prefs.debug_pokertracker(), "sleeping [%d] ms because iteration was too quick.\n", 10000 - iterDurationMS);
			if (LightSleep(10000 - iterDurationMS, pParent)) 
				break; 
		}
	}
	// Set event
	write_log(prefs.debug_pokertracker(), "PokertrackerThreadFunction: outside while loop...\n");
	::SetEvent(pParent->_m_wait_thread);
	return 0;
}

/*Sleeps but wakes up on stop thread event
We use this function since we never want the thread to ignore the stop_thread event while it's sleeping*/
int	CPokerTrackerThread::LightSleep(int sleepTime, CPokerTrackerThread *pParent)
{
	write_log(prefs.debug_pokertracker(), "LightSleep: called with sleepTime[%d]\n", sleepTime);
	if ( sleepTime > 0)
	{
		int iterations = 20;
		int sleepSlice = (int) ((double)sleepTime / (double)iterations);
		for (int i = 0; i < iterations; i++)
		{
			Sleep(sleepSlice);
			if (::WaitForSingleObject(pParent->_m_stop_thread, 0) == WAIT_OBJECT_0)
			{
				write_log(prefs.debug_pokertracker(), "LightSleep: _m_stop_thread signal received\n");
				return 1;
			}
		}
	}
	else
	{
		if (::WaitForSingleObject(pParent->_m_stop_thread, 0) == WAIT_OBJECT_0)
		{
			write_log(prefs.debug_pokertracker(), "LightSleep: _m_stop_thread signal received\n");
			return 1;
		}
	}
	return 0;
}