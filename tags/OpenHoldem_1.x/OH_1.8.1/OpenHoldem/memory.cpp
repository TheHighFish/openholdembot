#include "stdafx.h"

#include "memory.h"
#include "symbols.h"
#include "debug.h"
#include "global.h"
#include "grammar.h"

Memory		memory;

Memory::Memory() 
{
#ifdef SEH_ENABLE
	try {
#endif
	int			i;

		var_count=-1;

		for (i=0; i<512; i++) 
		{
			strcpy(var_name[i], "");
			var_value[i]=0.0;
		}

#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("Memory::constructor\n"); 
		throw; 
	}
#endif
}


double Memory::process_query (const char * pquery, int *e) 
{
#ifdef SEH_ENABLE
	try {
#endif

		if (memcmp(pquery,"me_st_", 6)==0) 
		{ 
			*e = SUCCESS;
			store_value(pquery, e); 
			return 0.0; 
		}

		if (memcmp(pquery,"me_re_", 6)==0) 
		{ 
			*e = SUCCESS;
			return retrieve_value(pquery, e); 
		} 

		*e = ERR_INVALID_SYM;
		return 0.0;

#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("Memory::process_query > %s\n", pquery); 
		throw; 
	}
#endif
}


void Memory::store_value(const char * pquery, int *e) 
{
#ifdef SEH_ENABLE
	try {
#endif
		int			i, index=0;
		char		var[512], value[512];
		double		result;

		strcpy(var, &pquery[6]);

		if (strstr(var, "_")!=NULL) 
			var[strstr(var, "_")-var]='\0';

		strcpy(value, &pquery[6+strlen(var)+1]);

		// see if we already have this variable name
		index=var_count+1;
		for (i=0; i<=var_count; i++) 
		{
			if (strlen(var)==strlen(var_name[i]) && memcmp(var, var_name[i], strlen(var))==0) 
			{ 
				index=i; 
			}
		}

		// if we didn't find the var, put it in a new slot
		if (index==var_count+1)
			var_count++;

		//store the value
		if (memcmp(value,"f$",2)==0) 
		{
			*e = SUCCESS;
			result = calc_f$symbol(&global.formula, value, e);

			if (*e==SUCCESS)
			{
				var_value[index] = result;
				strcpy(var_name[index], var);
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

			var_value[index] = atof(value);

			strcpy(var_name[index], var);
		}
		
			
#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("Memory::store_value > %s\n", pquery); 
		throw; 
	}
#endif
}

double Memory::retrieve_value(const char * pquery, int *e) 
{
#ifdef SEH_ENABLE
	try {
#endif
		int			i;
		char		var[512];

		strcpy(var, &pquery[6]);

		for (i=0; i<=var_count; i++) 
		{
			if (strlen(var)==strlen(var_name[i]) && memcmp(var, var_name[i], strlen(var))==0) 
			{ 
				return var_value[i]; 
			}
		}

		*e = ERR_INVALID_EXPR;
		return 0.0;

#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("Memory::retrieve_value > %s\n", pquery); 
		throw; 
	}
#endif
}
