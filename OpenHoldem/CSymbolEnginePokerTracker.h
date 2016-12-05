//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#ifndef INC_CSYMBOLENGINEPOKERTRACKER_H
#define INC_CSYMBOLENGINEPOKERTRACKER_H

#include "CVirtualSymbolEngine.h"

class CSymbolEnginePokerTracker: public CVirtualSymbolEngine {
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
	bool EvaluateSymbol(const char *name, double *result, bool log = false);
	CString SymbolsProvided();
 public:
  int PlayerIcon(const int chair);
 private:
	void	CheckForChangedPlayersOncePerHeartbeatAndSymbolLookup();
	void	WarnAboutInvalidPTSymbol(CString s);
	bool	IsOldStylePTSymbol(CString s);
	void	ClearSeatStats(int chair, bool clearNameAndFound);
	void	ClearAllStatsOfChangedPlayers();
	void	ClearAllStats();
 private:
	bool	check_for_identity_of_players_executed_this_heartbeat;
};

extern CSymbolEnginePokerTracker *p_symbol_engine_pokertracker;

#endif INC_CSYMBOLENGINEPOKERTRACKER_H