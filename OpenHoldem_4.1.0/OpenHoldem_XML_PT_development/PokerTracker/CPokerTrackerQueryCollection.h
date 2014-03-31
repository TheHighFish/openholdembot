#ifndef INC_CPOKERTRACKERQUERYCOLLECTION_H
#define INC_CPOKERTRACKERQUERYCOLLECTION_H

class CPokerTrackerQuery;

const unsigned int k_max_pt_queries = 256;

extern class CPokerTrackerQueryCollection
{
public:
	CPokerTrackerQueryCollection();
	~CPokerTrackerQueryCollection();
public:
	void AddQuery(CPokerTrackerQuery *new_query);
	int  GetNumberOfPokerTrackerQueries();
	CPokerTrackerQuery* GetPokerTrackerQuery(int index);
private:
	void DeleteAllQueries();
private:
	unsigned int number_of_queries;
	CPokerTrackerQuery* pt_queries[k_max_pt_queries];
} *p_pokertracker_query_collection;

#endif INC_CPOKERTRACKERQUERYCOLLECTION_H