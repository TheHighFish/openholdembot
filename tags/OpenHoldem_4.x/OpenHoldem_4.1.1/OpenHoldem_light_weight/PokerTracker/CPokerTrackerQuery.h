class CPokerTrackerQuery
{
public:
	CPokerTrackerQuery(CString name,
		CString query, 
		CString description,
		bool is_basic_stat,
		bool is_positional_preflop_stat,
		bool is_advanced_stat);
	~CPokerTrackerQuery();
public:
	CString GetName();
	// GetQuery does automatically adapt for tournaments if necessary
	CString GetQuery(bool is_tournament);
	CString GetDescription();
	bool IsBasicStat();
	bool IsPositionalPreflopStat();
	bool IsAdvancedStat();
private:
	CString _name;
	CString _query; 
	CString _description;
	bool _is_basic_stat;
	bool _is_positional_preflop_stat;
	bool _is_advanced_stat;
};