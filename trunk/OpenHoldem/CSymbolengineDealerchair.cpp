#include "stdafx.h"
#include "CSymbolEngineDealerchair.h"

#include "CPreferences.h"
#include "CScraper.h"
#include "MagicNumbers.h"

CSymbolEngineDealerchair::CSymbolEngineDealerchair()
{}

CSymbolEngineDealerchair::~CSymbolEngineDealerchair()
{}

void CSymbolEngineDealerchair::InitOnStartup()
{
	ResetOnConnection();
}

void CSymbolEngineDealerchair::ResetOnConnection()
{
	_dealerchair = k_undefined;
	nchairs = p_tablemap->nchairs();
}

void CSymbolEngineDealerchair::ResetOnHandreset()
{}

void CSymbolEngineDealerchair::ResetOnNewRound()
{}

void CSymbolEngineDealerchair::ResetOnMyTurn()
{}

void CSymbolEngineDealerchair::ResetOnHeartbeat()
{
	write_log(prefs.debug_symbolengine(), "nchairs: %d\n", nchairs);
	for (int i=0; i < nchairs; i++)
	{
		if (p_scraper->dealer(i))
		{
			write_log(prefs.debug_symbolengine(), "Setting dealerchair to %d\n", i);
			_dealerchair = i;					
			break;
		}
	}
	// Otherwise: do nothing and keep the dealer as is.
	// Dealer-button probably just temporary occluded.
	// Do not reset, as this might cause a hand-reset.
}