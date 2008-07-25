#include "stdafx.h"

#include "logsymbols.h"
#include "symbols.h"
#include "debug.h"
#include "global.h"

double	LogSymbols::process_query(const char * pquery, int *e)
{
    __SEH_HEADER

    if (strlen(pquery) > 4)
    {
        bool exists = false;

        for (int i=0; i<symbols.logsymbols_collection.GetCount(); i++)
        {
            if (symbols.logsymbols_collection[i] == pquery+4)
            {
                exists = true;
                break;
            }
        }

        if (!exists)
        {
            symbols.logsymbols_collection.Add(pquery+4);
        }

        return 1.0;
    }

    *e = ERR_INVALID_SYM;
    return 0.0;


    __SEH_LOGFATAL("LogSymbols::process_query\n");

}