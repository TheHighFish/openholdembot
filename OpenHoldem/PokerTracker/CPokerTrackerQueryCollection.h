#include "CPokerTrackerQuery.h"

const unsigned int k_max_pt_queries = 256;

class CPokerTrackerQueryCollection
{
public:
	CPokerTrackerQueryCollection();
	~CPokerTrackerQueryCollection();
public:
	void AddQuery(CPokerTrackerQuery *new_query);
	int  GetNumberOfPokerTrackerQueries();
	CPokerTrackerQuery* GetPokerTrackerQuery(unsigned int index);
private:
	void DeleteAllQueries();
private:
	unsigned int number_of_queries;
	CPokerTrackerQuery* pt_queries[k_max_pt_queries];
};