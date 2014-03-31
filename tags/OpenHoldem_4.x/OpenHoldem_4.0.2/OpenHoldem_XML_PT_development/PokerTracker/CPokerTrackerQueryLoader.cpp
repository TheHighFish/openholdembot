#include "stdafx.h"
#include "CPokerTrackerQueryLoader.h"

#include "CPokerTrackerQuery.h"
#include "CPokerTrackerQueryCollection.h"

CPokerTrackerQueryLoader::CPokerTrackerQueryLoader()
{
	LoadAllQueriesFromXMLFile();
}

CPokerTrackerQueryLoader::~CPokerTrackerQueryLoader()
{}

void CPokerTrackerQueryLoader::LoadAllQueriesFromXMLFile()
{
	// Demo
	// The data for the new query should be loaded from an XML-file.
	CPokerTrackerQuery *new_query = new CPokerTrackerQuery(
		"pt_vpip", 
		"SELECT (case when (count(*)!=0) then cast(sum(case when flg_vpip then 1 else 0 end) as real) / count(*) else (-1) end) as result FROM   player as P, holdem_hand_player_statistics as S WHERE  S.id_player = P.id_player AND P.player_name like '%SCREENNAME%' AND P.id_site=%SITEID%",
		"Poker Tracker VP$IP for chair x",
		true,
		false,
		false,
		true);
	p_pokertracker_query_collection->AddQuery(new_query);
}