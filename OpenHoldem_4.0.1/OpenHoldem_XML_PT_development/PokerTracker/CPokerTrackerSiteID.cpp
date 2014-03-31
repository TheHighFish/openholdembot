#include "stdafx.h"
#include "CPokerTrackerSiteID.h"

#include "..\CSymbols.h"
#include "..\CTablemap\CTablemap.h"

CPokerTrackerSiteID *p_pokertracker_site_id = NULL;

CPokerTrackerSiteID::CPokerTrackerSiteID()
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

CPokerTrackerSiteID::~CPokerTrackerSiteID()
{}

int CPokerTrackerSiteID::GetSiteId()
{
	CString network = p_tablemap->network();

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
			{
				return lookup->second;
			}
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
			{
				return lookup->second;
			}
			lookup++;
		}
	}
	return -1 ;
}