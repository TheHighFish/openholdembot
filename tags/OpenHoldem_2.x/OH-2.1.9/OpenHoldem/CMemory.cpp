#include "stdafx.h"

#include "CMemory.h"

#include "CAutoplayer.h"
#include "CSymbols.h"
#include "CFormula.h"
#include "CGrammar.h"

CMemory		*p_memory = NULL;

CMemory::CMemory()
{
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
	if (memcmp(pquery, "me_st_", 6)==0)
	{
		*e = SUCCESS;
		StoreValue(pquery, logCallingFunction, e);
		return 0.0;
	}

	if (memcmp(pquery, "me_re_", 6)==0)
	{
		*e = SUCCESS;
		return RetrieveValue(pquery, e);
	}

	*e = ERR_INVALID_SYM;
	return 0.0;
}


void CMemory::StoreValue(const char *pquery, CEvalInfoFunction **logCallingFunction, int *e)
{
	// !!! Bad code
	// !!! Not sure, if var and value can be so long
	// !!! or if it should be restricted to k_max_length_of_memory_symbols
	char		var[512] = {0}, value[512] = {0};
	double		result = 0.0;
	CGrammar	gram;

	// Copy without "me_st_"
	strcpy_s(var, 512, &pquery[6]);

	if (strstr(var, "_") != NULL)
		var[strstr(var, "_")-var] = '\0';

	strcpy_s(value, 512, &pquery[6+strlen(var)+1]);

	// see if we already have this variable name
	int next_free_slot = _var_count + 1;
	int index = next_free_slot;
	for (int i=0; i<=_var_count; i++)
	{
		if (strlen(var)==strlen(_var_name[i]) && memcmp(var, _var_name[i], strlen(var))==0)
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
			MessageBox(0, "Too many memory symbols.\n"
						  "Stopping autoplayer.", 
						  "Error", 0);
			p_autoplayer->set_autoplayer_engaged(false);
			return;
		}
		set_var_count(next_free_slot);
	}

	//store the value
	if (memcmp(value, "f$" , 2) == 0)
	{
		*e = SUCCESS;
		result = gram.DoCalcF$symbol(p_formula, value, logCallingFunction, logCallingFunction!=NULL, e);

		if (*e==SUCCESS)
		{
			set_var_value(index, result);
			set_var_name(index, var);
		}
		else
		{
			*e = ERR_INVALID_EXPR;
		}

	}
	else
	{
		if (strstr(value, "_")!=NULL)
			value[strstr(value, "_")-value]='.';

		set_var_value(index, atof(value));
		set_var_name(index, var);
	}
}

double CMemory::RetrieveValue(const char * pquery, int *e)
{
	int			i = 0;
	char		var[512] = {0};

	strcpy_s(var, 512, &pquery[6]);

	for (i=0; i<=_var_count; i++)
	{
		if (strlen(var)==strlen(_var_name[i]) && memcmp(var, _var_name[i], strlen(var))==0)
		{
			return _var_value[i];
		}
	}
	*e = ERR_INVALID_EXPR;
	return 0.0;
}
