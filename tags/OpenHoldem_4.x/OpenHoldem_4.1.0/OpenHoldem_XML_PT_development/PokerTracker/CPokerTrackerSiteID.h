#ifndef INC_CPOKERTRACKERSITEID_H
#define INC_CPOKERTRACKERSITEID_H

#include <map>

extern class CPokerTrackerSiteID
{
public:
	// public functions
	CPokerTrackerSiteID();
	~CPokerTrackerSiteID();
public:
	int GetSiteId();
private:
	// private functions and variables - not available via accessors or mutators
	std::map<CString, int>	_pt3_siteid;
} *p_pokertracker_site_id;

#endif INC_CPOKERTRACKERSITEID_H