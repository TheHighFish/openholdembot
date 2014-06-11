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

#include "stdafx.h"
#include "CSymbolEngineMemorySymbols.h"

//?? autoplayer-trace??

CSymbolEngineMemorySymbols *p_symbol_engine_memory_symbols = NULL;

CSymbolEngineMemorySymbols::CSymbolEngineMemorySymbols() {
}

CSymbolEngineMemorySymbols::~CSymbolEngineMemorySymbols() {
}

void CSymbolEngineMemorySymbols::InitOnStartup() {
}

void CSymbolEngineMemorySymbols::ResetOnConnection() {
  _memory_symbols.clear();
}

void CSymbolEngineMemorySymbols::ResetOnHandreset() {
}

void CSymbolEngineMemorySymbols::ResetOnNewRound() {
}

void CSymbolEngineMemorySymbols::ResetOnMyTurn() {
}

void CSymbolEngineMemorySymbols::ResetOnHeartbeat() {
}

void CSymbolEngineMemorySymbols::Store(CString command) {
  assert(command.Left(6) == "me_st_");
  CString command_without_prefix = command.Mid(6);
  double result = k_undefined_zero;  
  // Get the name of the symbol, up to the next underscore
  int position_of_first_underscore = command_without_prefix.Find('_');
  CString symbol_name = command_without_prefix.Left(position_of_first_underscore);
  // Get the right hand value after the underscore
  CString right_hand_value = command_without_prefix.Mid(position_of_first_underscore + 1);
  // !!! Eval
  MessageBox(0, right_hand_value, "Thou shall evaluate", 0);
  _memory_symbols[symbol_name] = result;
}

void CSymbolEngineMemorySymbols::Increment(CString command) {
  assert(command.Left(7) == "me_inc_");
  CString symbol_name = command.Mid(7);
  ++_memory_symbols[symbol_name];
}

double CSymbolEngineMemorySymbols::Recall(CString command) {
  assert(command.Left(6) == "me_re_");
  CString symbol_name = command.Mid(6);
  return _memory_symbols[symbol_name];
}

bool CSymbolEngineMemorySymbols::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
  // memory-commands
  // "name" = query
  if (memcmp(name, "me_", 3) == 0) {
    if (memcmp(name, "me_st_", 6) == 0) {  
      Store(name);
      *result = k_undefined_zero;
      return true;
    } else if (memcmp(name, "me_re_", 6) == 0) {
      *result = Recall(name);
      return true;
    } else if (memcmp(name, "me_inc_", 7) == 0) {
      Increment(name);
      *result = k_undefined_zero;
      return true;
    } else {
    // Looks like a memory-command, but is invalid
    return false;
    }
  }
  // Not a memory symbol
  return false;
}

/*#define ENT CSLock lock(m_critsec);
	// private functions and variables - not available via accessors or mutators
	bool RightValueIsNumber(char first_character_of_right_value);
	bool RightValueIsMemorySymbol(const char *value);
	bool RightValueIsFunction(const char *value);
	void StoreValue(const char * pquery, CEvalInfoFunction **logCallingFunction, int *e);
	double RetrieveValue(const char * pquery, int *e);

    CCritSec			m_critsec;

    
bool CMemory::RightValueIsNumber(char first_character_of_right_value)
{
	// Number constants inside memory expressions are quite limited.
	// They are always integer or floats in "simple" decimal format,
	// even without a minus sign.
	// so they always start with 0..9
	return isdigit(first_character_of_right_value);
}

bool CMemory::RightValueIsMemorySymbol(const char *value)
{
	// Right hand values starting with "me_re" are memory_symbols,
	// e.g. used like "me_st_OldValue_me_re_CurrentValue".
	CString cst_value = value;
	return (cst_value.Left(6) == "me_re_");
}

bool CMemory::RightValueIsFunction(const char *value)
{
	CString cst_value = value;
	return (cst_value.Left(2) == "f$");
}

void CMemory::StoreValue(const char *pquery, CEvalInfoFunction **logCallingFunction, int *e)
{
	// Get the name of the symbol, up to the next underscore
	if (strstr(var, "_") != NULL)
		var[strstr(var, "_")-var] = '\0';
	strcpy_s(value, k_max_length_of_memory_symbols, &pquery[6 + strlen(var) + 1]);


	// if we didn't find the var, put it in a new slot


	if (RightValueIsNumber(value[0]))
	{
		if (strstr(value, "_") != NULL)
		{
			// Number with "decimal point"
			// Get it right and replace it.
			value[strstr(value, "_")  -value]='.';
		}

		set_var_value(index, atof(value));
		set_var_name(index, var);
	}
	else if (RightValueIsMemorySymbol(value))
	{
		result = RetrieveValue(value, e);
		set_var_value(index, result);
		set_var_name(index, var);
	}
	else if (RightValueIsFunction(value))
	{
		result = p_function_collection->Evaluate(value);

		set_var_value(index, result);
		set_var_name(index, var);
	}
	else
	{
		// Must be a normal symbol
		p_engine_container->EvaluateSymbol(value, &result);
		set_var_value(index, result);
		set_var_name(index, var);
	}
}
*/