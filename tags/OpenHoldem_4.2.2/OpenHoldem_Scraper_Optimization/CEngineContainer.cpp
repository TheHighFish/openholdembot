#include "stdafx.h"
#include "CEngineContainer.h"

#include <assert.h>
#include "CBetroundCalculator.h"
#include "CHandresetDetector.h"
#include "CScraperAccess.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineBlinds.h"
#include "CSymbolEngineCards.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineDealerchair.h"
#include "CSymbolEngineHandrank.h"
#include "CSymbolEngineHistory.h"
#include "CSymbolEngineLists.h"
#include "CSymbolEnginePokerval.h"
#include "CSymbolEnginePositions.h"
#include "CSymbolEnginePrwin.h"
#include "CSymbolEngineRaisersCallers.h"
#include "CSymbolEngineRandom.h"
#include "CSymbolEngineTime.h"
#include "CSymbolEngineUserchair.h"
#include "CSymbols.h"

CEngineContainer *p_engine_container = NULL;

CEngineContainer::CEngineContainer()
{
	CreateSymbolEngines();
	// First initialization is the same as on a new connection
	ResetOnConnection();
}

CEngineContainer::~CEngineContainer()
{
	DestroyAllSymbolEngines();
}

void CEngineContainer::CreateSpecialSymbolEngines()
{
	// Some engines are "special", because we need to call them up-front,
	// e.g. to detect a hand-reset.
	// So they work slightly different and also get their own initialization.
	p_betround_calculator = new CBetroundCalculator();	
}

void CEngineContainer::CreateSymbolEngines()
{
	CreateSpecialSymbolEngines();

	_number_of_symbol_engines_loaded = 0;
	// Some symbols to be calculated depend on symbols of other engines.
	// The engines inserted first will be called first later.
	// But we assure correct ordering by assertions in the constructors of the engines.

	// CSymbolEngineTime
	p_symbol_engine_time = new CSymbolEngineTime();
	_symbol_engines[_number_of_symbol_engines_loaded] = p_symbol_engine_time;
	_number_of_symbol_engines_loaded++;
	// CSymbolEngineDealerchair
	p_symbol_engine_dealerchair= new CSymbolEngineDealerchair();
	_symbol_engines[_number_of_symbol_engines_loaded] = p_symbol_engine_dealerchair;
	_number_of_symbol_engines_loaded++;
	// CSymbolEngineUserchair
	p_symbol_engine_userchair= new CSymbolEngineUserchair();
	_symbol_engines[_number_of_symbol_engines_loaded] = p_symbol_engine_userchair;
	_number_of_symbol_engines_loaded++;
	// CSymbolEngineAutoplayer
	p_symbol_engine_autoplayer = new CSymbolEngineAutoplayer();
	_symbol_engines[_number_of_symbol_engines_loaded] = p_symbol_engine_autoplayer;
	_number_of_symbol_engines_loaded++;
	// CSymbolEngineLists
	p_symbol_engine_lists = new CSymbolEngineLists();
	_symbol_engines[_number_of_symbol_engines_loaded] = p_symbol_engine_lists;
	_number_of_symbol_engines_loaded++;
	// CSymbolEngineRandom
	p_symbol_engine_random = new CSymbolEngineRandom();
	_symbol_engines[_number_of_symbol_engines_loaded] = p_symbol_engine_random;
	_number_of_symbol_engines_loaded++;
	// CSymbolEngineActiveDealtPlaying
	p_symbol_engine_active_dealt_playing = new CSymbolEngineActiveDealtPlaying();
	_symbol_engines[_number_of_symbol_engines_loaded] = p_symbol_engine_active_dealt_playing;
	_number_of_symbol_engines_loaded++;
	// CSymbolEnginePositions
	p_symbol_engine_positions= new CSymbolEnginePositions();
	_symbol_engines[_number_of_symbol_engines_loaded] = p_symbol_engine_positions;
	_number_of_symbol_engines_loaded++;
	// CSymbolEngineBlinds
	p_symbol_engine_blinds = new CSymbolEngineBlinds();
	_symbol_engines[_number_of_symbol_engines_loaded] = p_symbol_engine_blinds;
	_number_of_symbol_engines_loaded++;
	// CSymbolEngineChipAmounts
	p_symbol_engine_chip_amounts = new CSymbolEngineChipAmounts();
	_symbol_engines[_number_of_symbol_engines_loaded] = p_symbol_engine_chip_amounts;
	_number_of_symbol_engines_loaded++;
	// CSymbolEngineRaisersCallers
	p_symbol_engine_raisers_callers = new CSymbolEngineRaisersCallers();
	_symbol_engines[_number_of_symbol_engines_loaded] = p_symbol_engine_raisers_callers;
	_number_of_symbol_engines_loaded++;
	// CSymbolEngineCards
	p_symbol_engine_cards = new CSymbolEngineCards();
	_symbol_engines[_number_of_symbol_engines_loaded] = p_symbol_engine_cards;
	_number_of_symbol_engines_loaded++;
	// CSymbolEnginePokerval
	p_symbol_engine_pokerval = new CSymbolEnginePokerval();
	_symbol_engines[_number_of_symbol_engines_loaded] = p_symbol_engine_pokerval;
	_number_of_symbol_engines_loaded++;
	// CSymbolEngineHistory
	p_symbol_engine_history = new CSymbolEngineHistory();
	_symbol_engines[_number_of_symbol_engines_loaded] = p_symbol_engine_history;
	_number_of_symbol_engines_loaded++;
	// CSymbolEnginePrwin
	p_symbol_engine_prwin = new CSymbolEnginePrwin();
	_symbol_engines[_number_of_symbol_engines_loaded] = p_symbol_engine_prwin;
	_number_of_symbol_engines_loaded++;
	// CSymbolEngineHandrank
	p_symbol_engine_handrank = new CSymbolEngineHandrank();
	_symbol_engines[_number_of_symbol_engines_loaded] = p_symbol_engine_handrank;
	_number_of_symbol_engines_loaded++;
	// Csymbols
	// Deals with symbol-lookups and depends on all the other ones.
	// Therefore it should has to be the very last one.
	p_symbols = new CSymbols();
	_symbol_engines[_number_of_symbol_engines_loaded] = p_symbols;
	_number_of_symbol_engines_loaded++;

	// Avoiding overflows
	assert(_number_of_symbol_engines_loaded < k_max_number_of_symbol_engines);
}

void CEngineContainer::DestroyAllSymbolEngines()
{
	for (int i=0; i<_number_of_symbol_engines_loaded; i++)
	{
		delete _symbol_engines[i];
		_symbol_engines[i] = NULL;
	}
	_number_of_symbol_engines_loaded = 0;
}

void CEngineContainer::DestroyAllSpecialSymbolEngines()
{
	delete p_betround_calculator;
}

void CEngineContainer::CallSymbolEnginesToUpdateSymbolsIfNecessary()
{
	p_betround_calculator->OnNewHeartbeat();
	p_handreset_detector->OnNewHeartbeat();
	// table-limits depend on betround
	p_tablelimits->CalcTableLimits();

	// ResetOnConnection() gets directly called by the auto-connector,
	// so we don't have to care about that.
	// We only need to care about:
	// * ResetOnHandreset()
	// * ResetOnNewRound()
	// * ResetOnMyTurn()
	if (p_handreset_detector->IsHandreset())
	{
		ResetOnHandreset();
	}
	if (p_betround_calculator->IsNewBetround())
	{
		ResetOnNewRound();
	}
	if (p_scraper_access->IsMyTurn())
	{
		ResetOnMyTurn();
	}
	// And finally ResetOnHeartbeat() gets always called.
	ResetOnHeartbeat();
}

void CEngineContainer::ResetOnConnection()
{
	p_tablelimits->ResetOnConnection();
	for (int i=0; i<_number_of_symbol_engines_loaded; i++)
	{
		_symbol_engines[i]->ResetOnConnection();
	}
}

void CEngineContainer::ResetOnHandreset()
{
	for (int i=0; i<_number_of_symbol_engines_loaded; i++)
	{
		_symbol_engines[i]->ResetOnHandreset();
	}
}

void CEngineContainer::ResetOnNewRound()
{
	for (int i=0; i<_number_of_symbol_engines_loaded; i++)
	{
		_symbol_engines[i]->ResetOnNewRound();
	}
}

void CEngineContainer::ResetOnMyTurn()
{
	for (int i=0; i<_number_of_symbol_engines_loaded; i++)
	{
		_symbol_engines[i]->ResetOnMyTurn();
	}
}

void CEngineContainer::ResetOnHeartbeat()
{
	for (int i=0; i<_number_of_symbol_engines_loaded; i++)
	{
		_symbol_engines[i]->ResetOnHeartbeat();
	}
}

