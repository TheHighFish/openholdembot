#ifndef INC_CSYMBOLENGINEPOKERTRACKER_H
#define INC_CSYMBOLENGINEPOKERTRACKER_H

#include "CVirtualSymbolEngine.h"

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
	bool	IsOldStylePTSymbol(CString s);
	void	ClearSeatStats(int chair, bool clearNameAndFound);
	void	ClearAllStatsOfChangedPlayers();
	void	ClearAllStats();
private:
	bool	check_for_identity_of_players_executed_this_heartbeat;
} *p_symbol_engine_pokertracker;

#endif INC_CSYMBOLENGINEPOKERTRACKER_H