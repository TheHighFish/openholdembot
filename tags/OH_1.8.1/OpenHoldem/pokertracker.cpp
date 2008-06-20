#include "StdAfx.h"
#include <process.h>
#include <comdef.h>

#include "pokertracker.h"
#include "debug.h"
#include "levenshtein.h"
#include "symbols.h"
#include "registry.h"
#include "GameState.h"
#include "global.h"

CWinThread			*h_pokertracker_thread=NULL;
bool				pokertracker_thread_alive=false;
PokerTracker		PT;

UINT __cdecl pokertracker_thread(LPVOID pParam)
{
#ifdef SEH_ENABLE
	// Set exception handler
	SetUnhandledExceptionFilter(MyUnHandledExceptionFilter);
#endif

#ifdef SEH_ENABLE
	try {
#endif
		int			i, j;

		while (pokertracker_thread_alive) 
		{

			if (!PT.connected)  
				PT.connect();

			if (PQstatus(PT.pgconn) == CONNECTION_OK && PT.connected) 
			{
				for (i=0; i<=9; i++) 
				{
					if (symbols.sym.issittingin || symbols.sym.isppro || symbols.sym.ismanual) 
					{
						if (PT.checkname(i)) 
						{

							for (j=pt_min; j<=pt_max; j++) 
							{
								PT.update_stat(i, j);
							}
						}
					}
				}
			}

			for (i=0; i<PT.update_delay && pokertracker_thread_alive; i++)
				Sleep(1000);
		}

		PT.disconnect();

		write_log("Stopped Poker Tracker thread.\n");

		AfxEndThread(0);

		return 0;

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("::pokertracker_thread : \n"); 
		throw;
	}
#endif
}

PokerTracker::PokerTracker() 
{
#ifdef SEH_ENABLE
	// Set exception handler
	SetUnhandledExceptionFilter(MyUnHandledExceptionFilter);
#endif

#ifdef SEH_ENABLE
	try {
#endif

		int				i,j;
		Registry		reg;

		InitializeCriticalSection(&PT.pt_cs);

		reg.read_reg();

		disable = reg.pt_disable;
		ip_addr = reg.pt_ip_addr;
		port = reg.pt_port;
		user = reg.pt_user;
		pass = reg.pt_pass;
		dbname = reg.pt_dbname;
		update_delay = reg.pt_updatedelay;
		cache_refresh = reg.pt_cacherefresh;

		conn_str = "host=" + ip_addr;
		conn_str += " port=" + port;
		conn_str += " user=" + user;
		conn_str += " password=" + pass;
		conn_str += " dbname=" + dbname;

		for (i=0; i<=9; i++) 
		{
			for (j=pt_min; j<=pt_max; j++) 
			{
				player_stats[i].stat[j] = -1.0 ;
				player_stats[i].t_elapsed[j] = -1 ;
			}
			player_stats[i].found = false ;
			strcpy (player_stats[i].pt_name, "") ;
			strcpy (player_stats[i].scraped_name, "") ;
		}

		connected = false;

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("PokerTracker::constructor : \n"); 
		throw;
	}
#endif
}

PokerTracker::~PokerTracker() 
{
#ifdef SEH_ENABLE
	try {
#endif

		DeleteCriticalSection(&PT.pt_cs);

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("PokerTracker::destructor : \n"); 
		throw;
	}
#endif
}

double PokerTracker::process_query (const char * s) 
{
#ifdef SEH_ENABLE
	try {
#endif

		if (PQstatus(pgconn) != CONNECTION_OK || !connected)  return 0.0;

		if      (memcmp(s,"pt_iconlastr",12)==0)		{ return getstat(game_state.lastraised(s[12]-'0'), pt_icon); }
		else if (memcmp(s,"pt_icon",7)==0)				{ return getstat(s[7]-'0', pt_icon); }
		else if (memcmp(s,"pt_pfr",6)==0)				{ return getstat(s[6]-'0', pt_pfr); }
		else if (memcmp(s,"pt_aggtotnopf",13)==0)		{ return getstat(s[13]-'0', pt_aggtotnopf); }
		else if (memcmp(s,"pt_aggtot",9)==0)			{ return getstat(s[9]-'0', pt_aggtot); }
		else if (memcmp(s,"pt_aggp",7)==0)				{ return getstat(s[7]-'0', pt_aggp); }
		else if (memcmp(s,"pt_aggf",7)==0)				{ return getstat(s[7]-'0', pt_aggf); }
		else if (memcmp(s,"pt_aggt",7)==0)				{ return getstat(s[7]-'0', pt_aggt); }
		else if (memcmp(s,"pt_aggr",7)==0)				{ return getstat(s[7]-'0', pt_aggr); }
		else if (memcmp(s,"pt_floppct",10)==0)			{ return getstat(s[10]-'0', pt_floppct); }
		else if (memcmp(s,"pt_turnpct",10)==0)			{ return getstat(s[10]-'0', pt_turnpct); }
		else if (memcmp(s,"pt_riverpct",11)==0)			{ return getstat(s[11]-'0', pt_riverpct); }
		else if (memcmp(s,"pt_vpip",7)==0)				{ return getstat(s[7]-'0', pt_vpip); }
		else if (memcmp(s,"pt_hands",8)==0)				{ return getstat(s[8]-'0', pt_hands); }
		else if (memcmp(s,"pt_pf_rfi",9)==0)			{ return getstat(s[9]-'0', pt_pf_rfi); }
		else if (memcmp(s,"pt_pf_cr",8)==0)				{ return getstat(s[8]-'0', pt_pf_cr); }
		else if (memcmp(s,"pt_pfats",8)==0)				{ return getstat(s[8]-'0', pt_pfats); }
		else if (memcmp(s,"pt_wsdp",7)==0)				{ return getstat(s[7]-'0', pt_wsdp); }
		else if (memcmp(s,"pt_wssd",7)==0)				{ return getstat(s[7]-'0', pt_wssd); }
		else if (memcmp(s,"pt_fbbts",8)==0)				{ return getstat(s[8]-'0', pt_fbbts); }
		else if (memcmp(s,"pt_fsbts",8)==0)				{ return getstat(s[8]-'0', pt_fsbts); }

		else if (memcmp(s,"pt_ricon",8)==0)				{ return getstat( symbols.sym.raischair, pt_icon); }
		else if (memcmp(s,"pt_rpfr",7)==0)				{ return getstat( symbols.sym.raischair, pt_pfr); }
		else if (memcmp(s,"pt_raggtotnopf",14)==0)		{ return getstat( symbols.sym.raischair, pt_aggtotnopf); }
		else if (memcmp(s,"pt_raggtot",10)==0)			{ return getstat( symbols.sym.raischair, pt_aggtot); }
		else if (memcmp(s,"pt_raggp",8)==0)				{ return getstat( symbols.sym.raischair, pt_aggp); }
		else if (memcmp(s,"pt_raggf",8)==0)				{ return getstat( symbols.sym.raischair, pt_aggf); }
		else if (memcmp(s,"pt_raggt",8)==0)				{ return getstat( symbols.sym.raischair, pt_aggt); }
		else if (memcmp(s,"pt_raggr",8)==0)				{ return getstat( symbols.sym.raischair, pt_aggr); }
		else if (memcmp(s,"pt_rfloppct",11)==0)			{ return getstat( symbols.sym.raischair, pt_floppct); }
		else if (memcmp(s,"pt_rturnpct",11)==0)			{ return getstat( symbols.sym.raischair, pt_turnpct); }
		else if (memcmp(s,"pt_rriverpct",12)==0)		{ return getstat( symbols.sym.raischair, pt_riverpct); }
		else if (memcmp(s,"pt_rvpip",8)==0)				{ return getstat( symbols.sym.raischair, pt_vpip); }
		else if (memcmp(s,"pt_rhands",9)==0)			{ return getstat( symbols.sym.raischair, pt_hands); }
		else if (memcmp(s,"pt_rpf_rfi",10)==0)			{ return getstat( symbols.sym.raischair, pt_pf_rfi); }
		else if (memcmp(s,"pt_rpf_cr",9)==0)			{ return getstat( symbols.sym.raischair, pt_pf_cr); }
		else if (memcmp(s,"pt_rpfats",9)==0)			{ return getstat( symbols.sym.raischair, pt_pfats); }
		else if (memcmp(s,"pt_rwsdp",8)==0)				{ return getstat( symbols.sym.raischair, pt_wsdp); }
		else if (memcmp(s,"pt_rwssd",8)==0)				{ return getstat( symbols.sym.raischair, pt_wssd); }
		else if (memcmp(s,"pt_rfbbts",9)==0)			{ return getstat( symbols.sym.raischair, pt_fbbts); }
		else if (memcmp(s,"pt_rfsbts",9)==0)			{ return getstat( symbols.sym.raischair, pt_fsbts); }

		else if (memcmp(s,"ptt_iconlastr",13)==0)		{ return getstat(game_state.lastraised(s[13]-'0'), ptt_icon); }
		else if (memcmp(s,"ptt_icon",8)==0)				{ return getstat(s[8]-'0', ptt_icon); }
		else if (memcmp(s,"ptt_pfr",7)==0)				{ return getstat(s[7]-'0', ptt_pfr); }
		else if (memcmp(s,"ptt_aggtotnopf",14)==0)		{ return getstat(s[14]-'0', ptt_aggtotnopf); }
		else if (memcmp(s,"ptt_aggtot",10)==0)			{ return getstat(s[10]-'0', ptt_aggtot); }
		else if (memcmp(s,"ptt_aggp",8)==0)				{ return getstat(s[8]-'0', ptt_aggp); }
		else if (memcmp(s,"ptt_aggf",8)==0)				{ return getstat(s[8]-'0', ptt_aggf); }
		else if (memcmp(s,"ptt_aggt",8)==0)				{ return getstat(s[8]-'0', ptt_aggt); }
		else if (memcmp(s,"ptt_aggr",8)==0)				{ return getstat(s[8]-'0', ptt_aggr); }
		else if (memcmp(s,"ptt_floppct",11)==0)			{ return getstat(s[11]-'0', ptt_floppct); }
		else if (memcmp(s,"ptt_turnpct",11)==0)			{ return getstat(s[11]-'0', ptt_turnpct); }
		else if (memcmp(s,"ptt_riverpct",12)==0)		{ return getstat(s[12]-'0', ptt_riverpct); }
		else if (memcmp(s,"ptt_vpip",8)==0)				{ return getstat(s[8]-'0', ptt_vpip); }
		else if (memcmp(s,"ptt_hands",9)==0)			{ return getstat(s[9]-'0', ptt_hands); }
		else if (memcmp(s,"ptt_pf_rfi",10)==0)			{ return getstat(s[10]-'0', ptt_pf_rfi); }
		else if (memcmp(s,"ptt_pf_cr",9)==0)			{ return getstat(s[9]-'0', ptt_pf_cr); }
		else if (memcmp(s,"ptt_pfats",9)==0)			{ return getstat(s[9]-'0', ptt_pfats); }
		else if (memcmp(s,"ptt_wsdp",8)==0)				{ return getstat(s[8]-'0', ptt_wsdp); }
		else if (memcmp(s,"ptt_wssd",8)==0)				{ return getstat(s[8]-'0', ptt_wssd); }
		else if (memcmp(s,"ptt_fbbts",9)==0)			{ return getstat(s[9]-'0', ptt_fbbts); }
		else if (memcmp(s,"ptt_fsbts",9)==0)			{ return getstat(s[9]-'0', ptt_fsbts); }

		else if (memcmp(s,"ptt_ricon",9)==0)			{ return getstat( symbols.sym.raischair, ptt_icon); }
		else if (memcmp(s,"ptt_rpfr",8)==0)				{ return getstat( symbols.sym.raischair, ptt_pfr); }
		else if (memcmp(s,"ptt_raggtotnopf",15)==0)		{ return getstat( symbols.sym.raischair, ptt_aggtotnopf); }
		else if (memcmp(s,"ptt_raggtot",11)==0)			{ return getstat( symbols.sym.raischair, ptt_aggtot); }
		else if (memcmp(s,"ptt_raggp",9)==0)			{ return getstat( symbols.sym.raischair, ptt_aggp); }
		else if (memcmp(s,"ptt_raggf",9)==0)			{ return getstat( symbols.sym.raischair, ptt_aggf); }
		else if (memcmp(s,"ptt_raggt",9)==0)			{ return getstat( symbols.sym.raischair, ptt_aggt); }
		else if (memcmp(s,"ptt_raggr",9)==0)			{ return getstat( symbols.sym.raischair, ptt_aggr); }
		else if (memcmp(s,"ptt_rfloppct",12)==0)		{ return getstat( symbols.sym.raischair, ptt_floppct); }
		else if (memcmp(s,"ptt_rturnpct",12)==0)		{ return getstat( symbols.sym.raischair, ptt_turnpct); }
		else if (memcmp(s,"ptt_rriverpct",13)==0)		{ return getstat( symbols.sym.raischair, ptt_riverpct); }
		else if (memcmp(s,"ptt_rvpip",9)==0)			{ return getstat( symbols.sym.raischair, ptt_vpip); }
		else if (memcmp(s,"ptt_rhands",10)==0)			{ return getstat( symbols.sym.raischair, ptt_hands); }
		else if (memcmp(s,"ptt_rpf_rfi",11)==0)			{ return getstat( symbols.sym.raischair, ptt_pf_rfi); }
		else if (memcmp(s,"ptt_rpf_cr",10)==0)			{ return getstat( symbols.sym.raischair, ptt_pf_cr); }
		else if (memcmp(s,"ptt_rpfats",10)==0)			{ return getstat( symbols.sym.raischair, ptt_pfats); }
		else if (memcmp(s,"ptt_rwsdp",8)==0)			{ return getstat( symbols.sym.raischair, ptt_wsdp); }
		else if (memcmp(s,"ptt_rwssd",8)==0)			{ return getstat( symbols.sym.raischair, ptt_wssd); }
		else if (memcmp(s,"ptt_rfbbts",9)==0)			{ return getstat( symbols.sym.raischair, ptt_fbbts); }
		else if (memcmp(s,"ptt_rfsbts",9)==0)			{ return getstat( symbols.sym.raischair, ptt_fsbts); }

		else return 0.0;
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("PokerTracker::process_query : \n"); 
		throw;
	}
#endif
}

void PokerTracker::connect(void) 
{
#ifdef SEH_ENABLE
	try {
#endif
		pgconn = PQconnectdb(conn_str.GetString());

		if (PQstatus(pgconn) == CONNECTION_OK) 
		{
			write_log("PostgreSQL DB opened successfully <%s/%s/%s>\n", ip_addr, port, dbname);
			connected = true;
		} 
		else 
		{
			write_log("ERROR opening PostgreSQL DB: %s\n\n", PQerrorMessage(pgconn));
			PQfinish(pgconn);
			connected = false;
		}
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("PokerTracker::connect : \n"); 
		throw;
	}
#endif
}

void PokerTracker::disconnect(void) 
{
#ifdef SEH_ENABLE
	try {
#endif

		if (PQstatus(pgconn) == CONNECTION_OK) 
			PQfinish(pgconn);

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("PokerTracker::disconnect : \n"); 
		throw;
	}
#endif
}

int	PokerTracker::getsiteid (void) 
{
#ifdef SEH_ENABLE
	try {
#endif
		int			i, e;
		CString		sym;

		if (symbols.sym.isppro) 
		{ 
			return 3; 
		} 
		else 
		{
			if (symbols.sym.ismanual)
			{
				for (i=0; i<=20; i++) 
				{
						if ((global.mm_network == networkid[i]) && strlen(networkid[i])) 
							return i;
				}
			}
			else
			{
				//Is s$sitename one of the supported PT sites?  Return the proper site_id for db queries.
				for (i=0; i<=20; i++) 
				{
						sym.Format("sitename$%s", networkid[i]);
						if (symbols.GetSymbolVal(sym.MakeLower().GetString(), &e) && strlen(networkid[i])) 
							return i;
				}
				//Is s$network one of the supported PT sites?  Return the proper site_id for db queries.
				for (i=0; i<=20; i++) 
				{
						sym.Format("network$%s", networkid[i]);
						if (symbols.GetSymbolVal(sym.MakeLower().GetString(), &e) && strlen(networkid[i])) 
							return i;
				}
			}
		}
		return -1 ;
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("PokerTracker::getsiteid : \n"); 
		throw;
	}
#endif
}

bool PokerTracker::checkname (int m_chr) 
{
#ifdef SEH_ENABLE
	try {
#endif
		int			i;
		char		oh_scraped_name[30], best_name[30], likename[100];
		bool		result, ok_scrape=false;

		if (m_chr<0 || m_chr>9)  
			return false;

		if (global.state[(global.state_index-1)&0xff].m_player[m_chr].m_name_known == 0)  
			return false;

		strcpy(oh_scraped_name, global.state[(global.state_index-1)&0xff].m_player[m_chr].m_name);

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
		if (player_stats[m_chr].found && strcmp(player_stats[m_chr].scraped_name, oh_scraped_name)==0)
			return true; 
		
		// We think we have the name, but it has changed since we last checked...reset stats for this
		// chair and search again
		if (player_stats[m_chr].found && strcmp(player_stats[m_chr].scraped_name, oh_scraped_name)!=0) 
		{
			for (i=pt_min; i<=pt_max; i++) 
			{
				player_stats[m_chr].stat[i] = -1.0 ;
				player_stats[m_chr].t_elapsed[i] = -1 ;
			}

			player_stats[m_chr].found = false ;
			strcpy (player_stats[m_chr].pt_name, "") ;
			strcpy (player_stats[m_chr].scraped_name, "") ;
		} 

		// We have not found the name in PT, go find it
		// First see if we can find the exact scraped name
		result = queryname(oh_scraped_name, oh_scraped_name, best_name);

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
			result = queryname(likename, oh_scraped_name, best_name);
		}

		// Query with "%"
		if (!result) 
		{
			result = queryname("%", oh_scraped_name, best_name);
		}

		if (result) 
		{	 
			player_stats[m_chr].found = true;
			strcpy(player_stats[m_chr].pt_name, best_name);
			strcpy(player_stats[m_chr].scraped_name, oh_scraped_name);
		}
		else 
		{
			player_stats[m_chr].found = false ;
			strcpy (player_stats[m_chr].pt_name, "") ;
			strcpy (player_stats[m_chr].scraped_name, "") ;
		} 

		return result;
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("PokerTracker::checkname : \n"); 
		throw;
	}
#endif
}

double PokerTracker::getstat (int m_chr, PT_Stats stat) 
{
#ifdef SEH_ENABLE
	try {
#endif
		double		x;

		if (!pokertracker_thread_alive) 
			return 0.0;

		if (m_chr<0 || m_chr>9)
			return 0.0;

		EnterCriticalSection(&pt_cs);
		x = player_stats[m_chr].stat[stat];
		LeaveCriticalSection(&pt_cs);

		return x;

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("PokerTracker::getstat : \n"); 
		throw;
	}
#endif
}

double PokerTracker::update_stat (int m_chr, int stat) 
{
#ifdef SEH_ENABLE
	try {
#endif
		PGresult	*res;
		char		strQry[2000], strQry1[2000], strQry2[2000];
		const char*	n;
		double		result = -1.0;
		char		siteidstr[5];
		int			siteid;

		//No more unnecessary queries when we don't even have a siteid to check
		siteid = getsiteid();
		if (siteid == -1)  
			return result;

		if (PQstatus(pgconn) != CONNECTION_OK || !connected)  
			return result;

		if (m_chr<0 || m_chr>9 || stat<pt_min || stat>pt_max)  
			return result;

		// If we already have stats cached for the player, the timeout has not expired,
		// return the value from the cache...
		if (symbols.sym.elapsed-player_stats[m_chr].t_elapsed[stat] < PT.cache_refresh &&
			 player_stats[m_chr].t_elapsed[stat] != -1 &&
			 player_stats[m_chr].stat[stat] != -1)
		{
				 result = player_stats[m_chr].stat[stat];
		}

		// ...otherwise query the database
		else 
		{
			// get query string for the requested statistic
			strcpy(strQry, query_str[stat]);

			// Insert the player name in the query string
			strcpy(strQry1, strQry);  // move the query into temp str 1
			while ((n=strstr(strQry1, "%SCREENNAME%"))!=NULL) // find the token in temp str 1
			{ 
				strcpy(strQry2, strQry1);  // move the query into temp str 2
				strQry2[n-strQry1]='\0';  // cut off temp str 2 at the beginning of the token
				strcat(strQry2, player_stats[m_chr].pt_name);  // append the player name to temp str 2
				strcat(strQry2, n+12); // append the portion of temp str 1 after the token to temp str 2
				strcpy(strQry, strQry2); // move temp str 2 into the original query
				strcpy(strQry1, strQry);  // move the query into temp str 1
			}

			// Insert the site id in the query string
			sprintf(siteidstr, "%d", siteid);
			strcpy(strQry1, strQry);  // move the query into temp str 1
			while ((n=strstr(strQry1, "%SITEID%"))!=NULL)   // find the token in temp str 1
			{
				strcpy(strQry2, strQry1);  // move the query into temp str 2
				strQry2[n-strQry1]='\0';  // cut off temp str 2 at the beginning of the token
				strcat(strQry2, siteidstr);  // append the site id to temp str 2
				strcat(strQry2, n+8); // append the portion of temp str 1 after the token to temp str 2
				strcpy(strQry, strQry2); // move temp str 2 into the original query
				strcpy(strQry1, strQry);  // move the query into temp str 1
			}

			// Do the query against the PT database
			try	
			{
				// See if we can find the player name in the database
				write_log("Querying %s for m_chr %d: %s\n", stat_str[stat], m_chr, strQry);
				res = PQexec(pgconn, strQry);
			}
			catch(_com_error &e) 
			{
				write_log("ERROR\n");
				write_log(_T("\tCode = %08lx\n"), e.Error());
				write_log(_T("\tCode meaning = %s\n"), e.ErrorMessage());
				_bstr_t bstrSource(e.Source());
				_bstr_t bstrDescription(e.Description());
				write_log(_T("\tSource = %s\n"), (LPCTSTR) bstrSource);
				write_log(_T("\tDescription = %s\n"), (LPCTSTR) bstrDescription);
				write_log(_T("\tQuery = [%s]\n"), strQry);
			}

			// Check query return code
			if (PQresultStatus(res) != PGRES_TUPLES_OK)	
			{
				switch (PQresultStatus(res)) 
				{
				case PGRES_COMMAND_OK:
					write_log("PGRES_COMMAND_OK: %s [%s]\n", PQerrorMessage(pgconn), strQry); break;
				case PGRES_EMPTY_QUERY:
					write_log("PGRES_EMPTY_QUERY: %s [%s]\n", PQerrorMessage(pgconn), strQry); break;
				case PGRES_BAD_RESPONSE:
					write_log("PGRES_BAD_RESPONSE: %s [%s]\n", PQerrorMessage(pgconn), strQry); break;
				case PGRES_COPY_OUT:
					write_log("PGRES_COPY_OUT: %s [%s]\n", PQerrorMessage(pgconn), strQry); break;
				case PGRES_COPY_IN:
					write_log("PGRES_COPY_IN: %s [%s]\n", PQerrorMessage(pgconn), strQry); break;
				case PGRES_NONFATAL_ERROR:
					write_log("PGRES_NONFATAL_ERROR: %s [%s]\n", PQerrorMessage(pgconn), strQry); break;
				case PGRES_FATAL_ERROR:
					write_log("PGRES_FATAL_ERROR: %s [%s]\n", PQerrorMessage(pgconn), strQry); break;
				default:
					write_log("GENERIC ERROR: %s [%s]\n", PQerrorMessage(pgconn), strQry); break;
				}
			}
			else 
			{
				if (PQgetisnull(res,0,0) != 1) 
				{ 
					result = atof(PQgetvalue(res,0,0));
					write_log("Query %s for m_chr %d success: %f\n", stat_str[stat], m_chr, result);
				}
			}

			PQclear(res);

			// update cache with new values
			EnterCriticalSection(&pt_cs);
			player_stats[m_chr].stat[stat] = result;
			player_stats[m_chr].t_elapsed[stat] = symbols.sym.elapsed;
			LeaveCriticalSection(&pt_cs);
		}

		return result;

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("PokerTracker::update_stat : \n"); 
		throw;
	}
#endif
}

bool PokerTracker::queryname (char * query_name, char * scraped_name, char * best_name) 
{
#ifdef SEH_ENABLE
	try {
#endif
		char		strQry[1000] = "";
		int			i, lev_dist, bestLD, bestLDindex;
		PGresult	*res;
		char		siteidstr[5];
		bool		result=false;
		LDistance	myLD;
		int siteid;

		//No more unnecessary queries when we don't even have a siteid to check
		siteid = getsiteid();
		if (siteid == -1)  return false;

		// siteid has changed -- we're using ManualMode
		if (siteid != last_siteid)
		{
			clearstats();
			last_siteid = siteid;
		}

		if (PQstatus(pgconn) != CONNECTION_OK || !connected)  
			return false;

		if (strlen(query_name)==0)  
			return false;

		sprintf(siteidstr, "%d", siteid);

		strcpy(strQry, "SELECT screen_name FROM players WHERE screen_name like '");
		strcat(strQry, query_name);
		strcat(strQry, "' AND main_site_id=");
		strcat(strQry, siteidstr);

		try	
		{ 
			res = PQexec(pgconn, strQry); 
		}
		catch(_com_error &e) 
		{
			write_log("Postgres Query error:\n");
			write_log("\tCode = %08lx\n", e.Error());
			write_log("\tCode meaning = %s\n", e.ErrorMessage());
			_bstr_t bstrSource(e.Source());
			_bstr_t bstrDescription(e.Description());
			write_log("\tSource = %s\n", (LPCTSTR) bstrSource);
			write_log("\tDescription = %s\n", (LPCTSTR) bstrDescription);
			write_log("\tQuery = [%s]\n", strQry);
		}

		// We got nothing, return false
		if (PQntuples(res) == 0) 
		{
			result = false;
		}

		// If we get one tuple, all is good - return the one name
		if (PQntuples(res) == 1) 
		{
			strcpy(best_name, PQgetvalue(res, 0, 0));
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
				strcpy(best_name, PQgetvalue(res, bestLDindex, 0));
				result = true;
			}
			else 
			{
				result = false;
			}
		}

		PQclear(res);
		return result; 
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("PokerTracker::queryname : \n"); 
		throw;
	}
#endif
}
void PokerTracker::clearstats (void)
{
#ifdef SEH_ENABLE
	try {
#endif
	int		i,j;

	for (i=0; i<=9; i++) 
	{
		for (j=pt_min; j<=pt_max; j++) 
		{
			player_stats[i].stat[j] = -1.0 ;
			player_stats[i].t_elapsed[j] = -1 ;
		}

		player_stats[i].found = false ;
		strcpy (player_stats[i].pt_name, "") ;
		strcpy (player_stats[i].scraped_name, "") ;
	}

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("PokerTracker::cleardata : \n"); 
		throw;
	}
#endif
}