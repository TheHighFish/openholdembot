#ifndef INC_CSYMBOLENGINEISTOURNAMENT_H
#define INC_CSYMBOLENGINEISTOURNAMENT_H

#include "CVirtualSymbolEngine.h"

extern class CSymbolEngineIsTournament: public CVirtualSymbolEngine
{
public:
	CSymbolEngineIsTournament();
	~CSymbolEngineIsTournament();
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
	bool istournament();
private:
	void TryToDetectTournament();
	bool BetsAndBalancesAreTournamentLike();
	bool AntesPresent();
	bool TitleStringLooksLikeTournament();
private:
	bool _is_tournament;
	bool _decision_locked;
} *p_symbol_engine_istournament;

#endif INC_CSYMBOLENGINEISTOURNAMENT_H