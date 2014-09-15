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

#ifndef INC_CSYMBOLENGINEHISTORY_H
#define INC_CSYMBOLENGINEHISTORY_H

#include "CVirtualSymbolEngine.h"
#include "CBetroundCalculator.h"
#include "CSymbolEngineUserChair.h"

class CSymbolEngineHistory: public CVirtualSymbolEngine
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
	bool EvaluateSymbol(const char *name, double *result, bool log = false);
  CString SymbolsProvided();
public:
	// Public accessors
	// Public accessors
	bool DidAct();	
	bool DidAct(int betround);
	
	bool DidActThisHand()
	{
		return DidAct(k_betround_preflop);
	}
public:
	int nplayersround(int betround)		{ return _nplayersround[betround]; }
	int botslastaction(int betround)	{ return _botslastaction[betround]; }
	double nbetsround(int betround)		{ return _nbetsround[betround]; }
  // Didchec can only be 0 or 1 poker-logically,
  // but the counter might be > 0 if the autoplayer did act more than once
  // or if the user scraped check- and call-buttons incorrectly.
	int didchec(int betround)	{ 
    return ((_autoplayer_actions[betround][k_autoplayer_function_check] > 0) ? 1:0) ; 
  }
	int didcall(int betround)			{ return _autoplayer_actions[betround][k_autoplayer_function_call]; }
	int didrais(int betround)			{ return _autoplayer_actions[betround][k_autoplayer_function_raise]; }
	int didswag(int betround)			{ return _autoplayer_actions[betround][k_autoplayer_function_betsize]; }
	int prevaction()			    		{ return _prevaction; }
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
};

extern CSymbolEngineHistory *p_symbol_engine_history;

#endif INC_CSYMBOLENGINEHISTORY_H


