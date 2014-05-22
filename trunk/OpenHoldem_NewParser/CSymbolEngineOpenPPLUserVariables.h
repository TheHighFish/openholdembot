//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose: Data container for OpenPPL user-variables.
//   If they exist they evaluate to true.
//   The collection gets cleared every heartbeat.
//
//***************************************************************************** 

#ifndef INC_CSYMBOL_ENGINE_OPENPPL_USER_VARIABLES_H
#define INC_CSYMBOL_ENGINE_OPENPPL_USER_VARIABLES_H

class CSymbolEngineOpenPPLUserVariables
{
public:
	CSymbolEngineOpenPPLUserVariables();
	~CSymbolEngineOpenPPLUserVariables();
public:
	// Mandatory reset-functions
	void InitOnStartup();
	void ResetOnConnection();
	void ResetOnHandreset();
	void ResetOnNewRound();
	void ResetOnMyTurn();
	void ResetOnHeartbeat();
public:
	void Set(CString symbol);
	bool EvaluateSymbol(const char *name, double *result, bool log = false);
private:
	CMap<CString*, CString*, bool, bool> _user_variable_hashtable;
};

extern CSymbolEngineOpenPPLUserVariables *p_symbol_engine_openppl_user_variables;

#endif INC_CSYMBOL_ENGINE_OPENPPL_USER_VARIABLES_H