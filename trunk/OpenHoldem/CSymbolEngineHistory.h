//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose:
//
//***************************************************************************** 

#ifndef INC_CSYMBOLENGINEHISTORY_H
#define INC_CSYMBOLENGINEHISTORY_H

#include "CVirtualSymbolEngine.h"
#include "CBetroundCalculator.h"
#include "CSymbolEngineUserChair.h"

extern class CSymbolEngineHistory: public CVirtualSymbolEngine
{
public:
	CSymbolEngineHistory();
	~CSymbolEngineHistory();
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
	bool DidAct()	
	{
		if (!p_symbol_engine_userchair->userchair_confirmed())
		{
			return false;
		}
		// Not considering fold or allin, because the game would be over.
		return (didchec(BETROUND) || didcall(BETROUND) 
			|| didswag(BETROUND) || didrais(BETROUND));
	}
public:
	int nplayersround(int betround)		{ return _nplayersround[betround]; }
	int botslastaction(int betround)	{ return _botslastaction[betround]; }
	double nbetsround(int betround)		{ return _nbetsround[betround]; }
	int didchec(int betround)			{ return _autoplayer_actions[betround][k_autoplayer_function_check]; }
	int didcall(int betround)			{ return _autoplayer_actions[betround][k_autoplayer_function_call]; }
	int didrais(int betround)			{ return _autoplayer_actions[betround][k_autoplayer_function_raise]; }
	int didswag(int betround)			{ return _autoplayer_actions[betround][k_autoplayer_function_betsize]; }
	int prevaction()					{ return _prevaction; }
public:
	void RegisterAction(int autoplayer_action_code);
private:
	void SetPrevaction(int autoplayer_action_code);
	void CalculateHistory();
private:
	int _prevaction;
private:
	// Element 0 is unused
	// Elements 1..4 are for the 4 betrounds
	int _nplayersround[k_number_of_betrounds + 1];		
	int _botslastaction[k_number_of_betrounds + 1];		
	double _nbetsround[k_number_of_betrounds + 1];
	// Autoplayer-actions for the 4 bet-rounds
	// First dimension: betround; element 0 is unused again.
	// Second dimension: only some actions are really used,
	//   but it does not hurt to have a simple and extensible interface.
	//   "betpot" gets treated as swag, etc.
	int _autoplayer_actions[k_number_of_betrounds + 1][k_autoplayer_function_fold];
} *p_symbol_engine_history;

#endif INC_CSYMBOLENGINEHISTORY_H


