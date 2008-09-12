#include "stdafx.h"

#include "CMemory.h"

#include "CSymbols.h"
#include "CFormula.h"
#include "CGrammar.h"

CMemory		*p_memory = NULL;

CMemory::CMemory()
{
	__SEH_SET_EXCEPTION_HANDLER

	int	i = 0;

	_var_count = -1;

	for (i=0; i<512; i++)
	{
		strcpy_s(_var_name[i], 64, "");
		_var_value[i] = 0.0;
	}
}

CMemory::~CMemory()
{
}

const double CMemory::ProcessQuery(const char * pquery, CEvalInfoFunction **logCallingFunction, int *e)
{
	if (memcmp(pquery,"me_st_", 6)==0)
	{
		*e = SUCCESS;
		StoreValue(pquery, logCallingFunction, e);
		return 0.0;
	}

	if (memcmp(pquery,"me_re_", 6)==0)
	{
		*e = SUCCESS;
		return RetrieveValue(pquery, e);
	}

	*e = ERR_INVALID_SYM;
	return 0.0;
}


void CMemory::StoreValue(const char * pquery, CEvalInfoFunction **logCallingFunction, int *e)
{
	int			i = 0, index = 0;
	char		var[512] = {0}, value[512] = {0};
	double		result = 0.;
	CGrammar	gram;

	strcpy_s(var, 512, &pquery[6]);

	if (strstr(var, "_")!=NULL)
		var[strstr(var, "_")-var]='\0';

	strcpy_s(value, 512, &pquery[6+strlen(var)+1]);

	// see if we already have this variable name
	index=_var_count+1;
	for (i=0; i<=_var_count; i++)
	{
		if (strlen(var)==strlen(_var_name[i]) && memcmp(var, _var_name[i], strlen(var))==0)
		{
			index=i;
		}
	}

	// if we didn't find the var, put it in a new slot
	if (index==_var_count+1)
		_var_count++;

	//store the value
	if (memcmp(value,"f$",2)==0)
	{
		*e = SUCCESS;
		result = gram.DoCalcF$symbol(p_formula, value, logCallingFunction, logCallingFunction!=NULL, e);

		if (*e==SUCCESS)
		{
			_var_value[index] = result;
			strcpy_s(_var_name[index], 64, var);
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

		_var_value[index] = atof(value);

		strcpy_s(_var_name[index], 64, var);
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
