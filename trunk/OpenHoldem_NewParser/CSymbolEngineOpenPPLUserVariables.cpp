#include "stdafx.h"
#include "CSymbolEngineOpenPPLUserVariables.h"

#include "CPreferences.h"
#include "CStringHashtableTemplate.h"

CSymbolEngineOpenPPLUserVariables *p_symbol_engine_openppl_user_variables = NULL;

CSymbolEngineOpenPPLUserVariables::CSymbolEngineOpenPPLUserVariables()
{
	// Hash-table-size must be 
	// * a prime number
	// * if possible at least 20% larger than the number of entries
	_user_variable_hashtable.InitHashTable(1031);
}

CSymbolEngineOpenPPLUserVariables::~CSymbolEngineOpenPPLUserVariables()
{}

void CSymbolEngineOpenPPLUserVariables::InitOnStartup()
{}

void CSymbolEngineOpenPPLUserVariables::ResetOnConnection()
{}

void CSymbolEngineOpenPPLUserVariables::ResetOnHandreset()
{
	// All user-variables are for the current hand only 
	// and get deleted on hand-reset.
	write_log(preferences.debug_symbolengine_open_ppl(),
		"[CSymbolEngineOpenPPLUserVariables] Deleting all user variables on hand-reset\n");
	_user_variable_hashtable.RemoveAll();
}

void CSymbolEngineOpenPPLUserVariables::ResetOnNewRound()
{}

void CSymbolEngineOpenPPLUserVariables::ResetOnMyTurn()
{}

void CSymbolEngineOpenPPLUserVariables::ResetOnHeartbeat()
{}

void CSymbolEngineOpenPPLUserVariables::Set(CString symbol)
{
	write_log(preferences.debug_symbolengine_open_ppl(),
		"[CSymbolEngineOpenPPLUserVariables] Setting user-variable %s\n", symbol);
	_user_variable_hashtable.SetAt(&symbol, true);
}

bool CSymbolEngineOpenPPLUserVariables::EvaluateSymbol(const char *name, double *result)
{
	// !! conflict with userchhair
	if (memcmp(name, "user", 4) != 0)
	{
		// Not a user-variable
		return false;
	}
	if (memcmp(name+4, "chair", 5) == 0)
	{
		// Symbol "userchair", not a user-variable
		return false;
	}
	// Try to look it up
	bool dummy_result;
	if (_user_variable_hashtable.Lookup((CString*)name, dummy_result))
	{
		write_log(preferences.debug_symbolengine_open_ppl(),
			"[CSymbolEngineOpenPPLUserVariables] user-variable exists: %s\n", name);
		*result = double(true);
		return true;
	}
	write_log(preferences.debug_symbolengine_open_ppl(),
			"[CSymbolEngineOpenPPLUserVariables] user-variable does not exist: %s\n", name);
	*result = double(false);
	return true;
}

