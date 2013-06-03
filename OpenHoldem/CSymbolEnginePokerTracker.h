#ifndef INC_CSYMBOLENGINEPOKERTRACKER_H
#define INC_CSYMBOLENGINEPOKERTRACKER_H

#include "CVirtualSymbolEngine.h"

const int k_max_number_of_supported_pokertracker_stats = 64;

struct SPlayerStats 
{
	char			scraped_name[k_max_length_of_playername];
	char			pt_name[k_max_length_of_playername];
	bool			found;
	double			stat[k_max_number_of_supported_pokertracker_stats];
	int				t_elapsed[k_max_number_of_supported_pokertracker_stats]; //??? necessary
	int				skipped_updates;           //??? nec?
};

extern SPlayerStats _player_stats[k_max_number_of_players];

extern class CSymbolEnginePokerTracker: public CVirtualSymbolEngine
{
public:
	CSymbolEnginePokerTracker();
	~CSymbolEnginePokerTracker();
public:
	// Mandatory reset-functions
	void InitOnStartup();
	void ResetOnConnection();
	void ResetOnHandreset();
	void ResetOnNewRound();
	void ResetOnMyTurn();
	void ResetOnHeartbeat();
public:
	// Public accessors	
	double	ProcessQuery(const char *s);
private:
	void	CheckForChangedPlayersOncePerHeartbeatAndSymbolLookup();
	void	WarnAboutInvalidPTSymbol(CString s);
	void	ClearSeatStats(int chair, bool clearNameAndFound);
	void	ClearAllStatsOfChangedPlayers();
	void	ClearAllStats();
private:
	bool	check_for_identity_of_players_executed_this_heartbeat;
} *p_symbol_engine_pokertracker;

#endif INC_CSYMBOLENGINEPOKERTRACKER_H