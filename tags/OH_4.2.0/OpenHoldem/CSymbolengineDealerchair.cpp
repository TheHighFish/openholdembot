#include "stdafx.h"
#include "CSymbolEngineDealerchair.h"

#include "CPreferences.h"
#include "CScraper.h"
#include "MagicNumbers.h"

CSymbolEngineDealerchair *p_symbol_engine_dealerchair = NULL;

CSymbolEngineDealerchair::CSymbolEngineDealerchair()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	//
	// This engine does not use any other engines.
}

CSymbolEngineDealerchair::~CSymbolEngineDealerchair()
{}

void CSymbolEngineDealerchair::InitOnStartup()
{
	ResetOnConnection();
}

void CSymbolEngineDealerchair::ResetOnConnection()
{
	_dealerchair = k_undefined;
}

void CSymbolEngineDealerchair::ResetOnHandreset()
{}

void CSymbolEngineDealerchair::ResetOnNewRound()
{}

void CSymbolEngineDealerchair::ResetOnMyTurn()
{}

void CSymbolEngineDealerchair::ResetOnHeartbeat()
{
	write_log(prefs.debug_symbolengine(), "nchairs: %d\n", 
		p_tablemap->nchairs());
	for (int i=0; i<p_tablemap->nchairs(); i++)
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