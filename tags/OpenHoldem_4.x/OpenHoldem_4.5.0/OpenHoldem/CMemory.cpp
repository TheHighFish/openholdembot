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

#include "stdafx.h"
#include "CMemory.h"

#include "CAutoplayer.h"
#include "CEngineContainer.h"
#include "CFormula.h"
#include "CGrammar.h"
#include "CPreferences.h"
#include "OH_MessageBox.h"

CMemory		*p_memory = NULL;

CMemory::CMemory()
{
	write_log(preferences.debug_memorysymbols(), "[CMemory] Clearing all symbols\n");
	_var_count = -1;
	for (int i=0; i<k_max_number_of_memory_symbols; i++)
	{
		strcpy_s(_var_name[i], k_max_length_of_memory_symbols, "");
		_var_value[i] = 0.0;
	}
}

CMemory::~CMemory()
{
}

const double CMemory::ProcessQuery(const char * pquery, CEvalInfoFunction **logCallingFunction, int *e)
{
	if (memcmp(pquery, "me_st_", 6) == 0)
	{
		*e = SUCCESS;
		StoreValue(pquery, logCallingFunction, e);
		write_log(preferences.debug_memorysymbols(), "[CMemory] Storing symbol: %s\n", pquery);
		return 0.0;
	}

	if (memcmp(pquery, "me_re_", 6)==0)
	{
		*e = SUCCESS;
		write_log(preferences.debug_memorysymbols(), "[CMemory] Querying symbol: %s\n", pquery);
		return RetrieveValue(pquery, e);
	}

	*e = ERR_INVALID_SYM;
	return 0.0;
}


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
	char		var[k_max_length_of_memory_symbols] = {0};
	char		value[k_max_length_of_memory_symbols] = {0};
	double		result = 0.0;
	CGrammar	gram;

	// Copy without "me_st_"
	strcpy_s(var, k_max_length_of_memory_symbols, &pquery[6]);

	// Get the name of the symbol, up to the next underscore
	if (strstr(var, "_") != NULL)
		var[strstr(var, "_")-var] = '\0';
	strcpy_s(value, k_max_length_of_memory_symbols, &pquery[6 + strlen(var) + 1]);

	// see if we already have this variable name
	int next_free_slot = _var_count + 1;
	int index = next_free_slot;
	for (int i=0; i<=_var_count; i++)
	{
		if (strlen(var)==strlen(_var_name[i]) && memcmp(var, _var_name[i], strlen(var)) == 0)
		{
			index = i;
			break;
		}
	}

	// if we didn't find the var, put it in a new slot
	if (index == next_free_slot)
	{
		// Check for memory overflow
		if (next_free_slot >= k_max_number_of_memory_symbols)
		{
			OH_MessageBox_Error_Warning("Too many memory symbols.\n"
						  "Stopping autoplayer.", 
						  "Error");
			p_autoplayer->EngageAutoplayer(false);
			return;
		}
		write_log(preferences.debug_memorysymbols(), "[CMemory] Storing new value in a new slot\n");
		set_var_count(next_free_slot);
	}
	else
	{
		write_log(preferences.debug_memorysymbols(), "[CMemory] Overwriting existing symbol with new value\n");
	}

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
		*e = SUCCESS;
		result = gram.DoCalcF$symbol(p_formula, value, logCallingFunction, logCallingFunction!=NULL, e);

		if (*e == SUCCESS)
		{
			set_var_value(index, result);
			set_var_name(index, var);
		}
		else
		{
			*e = ERR_INVALID_EXPR;
			// And decreasing the var_count again,
			// otherwise we get a reach the maximum number of symbols soon,
			// despite we didn't save anything.
			set_var_count(_var_count - 1);
		}
	}
	else
	{
		// Must be a normal symbol
		p_engine_container->EvaluateSymbol(value, &result);
		set_var_value(index, result);
		set_var_name(index, var);
	}
}

double CMemory::RetrieveValue(const char * pquery, int *e)
{
	char		var[k_max_length_of_memory_symbols] = {0};

	strcpy_s(var, k_max_length_of_memory_symbols, &pquery[6]);

	for (int i=0; i<=_var_count; i++)
	{
		if (strlen(var)==strlen(_var_name[i]) && memcmp(var, _var_name[i], strlen(var))==0)
		{
			write_log(preferences.debug_memorysymbols(), "[CMemory] Value found: %.3f\n");
			return _var_value[i];
		}
	}
	write_log(preferences.debug_memorysymbols(), "[CMemory] Value not found. Returning 0.\n");
	return 0;
}
