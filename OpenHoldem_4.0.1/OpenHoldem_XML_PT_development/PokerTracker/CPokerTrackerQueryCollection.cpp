#include "stdafx.h"
#include "CPokerTrackerQueryCollection.h"

#include "assert.h"
#include "CPokerTrackerQuery.h"
#include "..\OH_MessageBox.h"

CPokerTrackerQueryCollection *p_pokertracker_query_collection = NULL;

CPokerTrackerQueryCollection::CPokerTrackerQueryCollection()
{
	number_of_queries = 0;
}

CPokerTrackerQueryCollection::~CPokerTrackerQueryCollection()
{
	DeleteAllQueries();
}

void CPokerTrackerQueryCollection::AddQuery(CPokerTrackerQuery *new_query)
{
	pt_queries[number_of_queries] = new_query;
	number_of_queries++;
	if (number_of_queries >= k_max_pt_queries)
	{
		CString error_message;
		error_message.Format("Too many PokerTrackerQueries in XML-file.\n"
			"%i is the maximum.", k_max_pt_queries);
		OH_MessageBox(error_message, "ERROR", 0);
		PostQuitMessage(-1);
	}
}

int CPokerTrackerQueryCollection::GetNumberOfPokerTrackerQueries()
{
	return number_of_queries;
}

CPokerTrackerQuery* CPokerTrackerQueryCollection::GetPokerTrackerQuery(int index)
{
	assert(index >= 0);
	assert(index < GetNumberOfPokerTrackerQueries());
	return pt_queries[index];
}

void CPokerTrackerQueryCollection::DeleteAllQueries()
{
	for (int i=0; i<GetNumberOfPokerTrackerQueries(); i++)
	{
		CPokerTrackerQuery* next_query = GetPokerTrackerQuery(i);
		next_query->~CPokerTrackerQuery();
	}
	number_of_queries = 0;
}