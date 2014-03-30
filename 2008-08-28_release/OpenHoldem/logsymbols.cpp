#include "stdafx.h"

#include "logsymbols.h"
#include "CSymbols.h"

LogSymbols::LogSymbols()
{
    __SEH_SET_EXCEPTION_HANDLER
}

double	LogSymbols::process_query(const char * pquery, int *e)
{
    __SEH_HEADER

    if (strlen(pquery) > 4)
    {
        bool exists = false;

        for (int i=0; i<p_symbols->logsymbols_collection()->GetCount(); i++)
        {
			if (p_symbols->logsymbols_collection()->GetAt(i) == pquery+4)
            {
                exists = true;
                break;
            }
        }

        if (!exists)
        {
			EnterCriticalSection(&p_symbols->cs_symbols);
            p_symbols->set_logsymbols_collection()->Add(pquery+4);
			LeaveCriticalSection(&p_symbols->cs_symbols);
        }

        return 1.0;
    }

    *e = ERR_INVALID_SYM;
    return 0.0;


    __SEH_LOGFATAL("LogSymbols::process_query\n");
}