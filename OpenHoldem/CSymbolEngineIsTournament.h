//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*****************************************************************************
//
// Purpose: Detecting if we play a tournament, especially
//   to enable / disable automatic blind-locking (stability) 
//
//*****************************************************************************

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
	bool istournament()		{ return _istournament == true; }
private:
	void TryToDetectTournament();
	bool BetsAndBalancesAreTournamentLike();
	bool AntesPresent();
	bool TitleStringLooksLikeTournament();
private:
	// Internally we have an extra state, k_undefined.  But that's
	// not exposed in the API as it's used internally to avoid locking
	// the istournament decision too quickly.
	int _istournament;
	bool _decision_locked;
} *p_symbol_engine_istournament;

#endif INC_CSYMBOLENGINEISTOURNAMENT_H
