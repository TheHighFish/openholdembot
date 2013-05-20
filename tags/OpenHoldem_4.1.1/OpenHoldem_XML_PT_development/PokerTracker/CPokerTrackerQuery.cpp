#include "stdafx.h"
#include "CPokerTrackerQuery.h"

CPokerTrackerQuery::CPokerTrackerQuery(
	CString name,
	CString query, 
	CString description,
	bool is_basic_stat,
	bool is_positional_preflop_stat,
	bool is_advanced_stat,
	bool is_activated)
{
	_name = name;
	_query = query;
	_description = description;
	_is_basic_stat = is_basic_stat;
	_is_positional_preflop_stat = is_positional_preflop_stat;
	_is_advanced_stat = is_advanced_stat;
	_is_activated = is_activated;
}

CPokerTrackerQuery::~CPokerTrackerQuery()
{}



CString CPokerTrackerQuery::GetName()
{
	return _name;
}
	
CString CPokerTrackerQuery::GetQuery(bool is_tournament)
{
	return _query;
}
	
CString CPokerTrackerQuery::GetDescription()
{
	return _description;
}
	
bool CPokerTrackerQuery::IsBasicStat()
{
	return _is_basic_stat;
}
	
bool CPokerTrackerQuery::IsPositionalPreflopStat()
{
	return _is_positional_preflop_stat;
}
	
bool CPokerTrackerQuery::IsAdvancedStat()
{
	return _is_advanced_stat;
}
	
bool CPokerTrackerQuery::IsActivated()
{
	return _is_activated;
}
	