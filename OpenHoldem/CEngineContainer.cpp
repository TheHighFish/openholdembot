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
#include "CEngineContainer.h"

#include <assert.h>
#include "CBetroundCalculator.h"
#include "CHandresetDetector.h"
#include "CPreferences.h"
#include "CScraperAccess.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineBlinds.h"
#include "CSymbolEngineCards.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineDealerchair.h"
#include "CSymbolEngineHandrank.h"
#include "CSymbolEngineHistory.h"
#include "CSymbolEngineIniFunctions.h"
#include "CSymbolEngineIsOmaha.h"
#include "CSymbolEngineIsTournament.h"
#include "CSymbolEngineLists.h"
#include "CSymbolEnginePokerTracker.h"
#include "CSymbolEnginePokerval.h"
#include "CSymbolEnginePositions.h"
#include "CSymbolEnginePrwin.h"
#include "CSymbolEngineRaisersCallers.h"
#include "CSymbolEngineRandom.h"
#include "CSymbolEngineReplayFrameController.h"
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

void CEngineContainer::AddSymbolEngine(CVirtualSymbolEngine *new_symbol_engine)
{
	assert(_number_of_symbol_engines_loaded < k_max_number_of_symbol_engines);
	_symbol_engines[_number_of_symbol_engines_loaded] = new_symbol_engine;
	++_number_of_symbol_engines_loaded;
}

void CEngineContainer::CreateSymbolEngines()
{
	write_log(preferences.debug_engine_container(), "[EngineContainer] Going to create symbol engines\n");
	CreateSpecialSymbolEngines();

	_number_of_symbol_engines_loaded = 0;
	// Some symbols to be calculated depend on symbols of other engines.
	// The engines inserted first will be called first later.
	// But we assure correct ordering by assertions in the constructors of the engines.

	// CSymbolEngineTableLimits
	p_symbol_engine_tablelimits = new CSymbolEngineTableLimits ();
	AddSymbolEngine(p_symbol_engine_tablelimits);
	// CSymbolEngineReplayFrameController
	p_symbol_engine_replayframe_controller = new CSymbolEngineReplayFrameController();
	AddSymbolEngine(p_symbol_engine_replayframe_controller);
	// CSymbolEngineTime
	p_symbol_engine_time = new CSymbolEngineTime();
	AddSymbolEngine(p_symbol_engine_time);
	// CSymbolEngineDealerchair
	p_symbol_engine_dealerchair = new CSymbolEngineDealerchair();
	AddSymbolEngine(p_symbol_engine_dealerchair);
	// CSymbolEngineUserchair
	p_symbol_engine_userchair = new CSymbolEngineUserchair();
	AddSymbolEngine(p_symbol_engine_userchair);
	// CSymbolEngineAutoplayer
	p_symbol_engine_autoplayer = new CSymbolEngineAutoplayer();
	AddSymbolEngine(p_symbol_engine_autoplayer);
	// CSymbolEngineLists
	p_symbol_engine_lists = new CSymbolEngineLists();
	AddSymbolEngine(p_symbol_engine_lists);
	// CSymbolEngineRandom
	p_symbol_engine_random = new CSymbolEngineRandom();
	AddSymbolEngine(p_symbol_engine_random);
	// CSymbolEngineActiveDealtPlaying
	p_symbol_engine_active_dealt_playing = new CSymbolEngineActiveDealtPlaying();
	AddSymbolEngine(p_symbol_engine_active_dealt_playing);
	// CSymbolEnginePositions
	p_symbol_engine_positions = new CSymbolEnginePositions();
	AddSymbolEngine(p_symbol_engine_positions);
	// CSymbolEngineBlinds
	p_symbol_engine_blinds = new CSymbolEngineBlinds();
	AddSymbolEngine(p_symbol_engine_blinds);
	// CSymbolEngineChipAmounts
	p_symbol_engine_chip_amounts = new CSymbolEngineChipAmounts();
	AddSymbolEngine(p_symbol_engine_chip_amounts);
	// CSymbolEngineRaisersCallers
	p_symbol_engine_raisers_callers = new CSymbolEngineRaisersCallers();
	AddSymbolEngine(p_symbol_engine_raisers_callers);
	// CSymbolEnginePokerTracker
	p_symbol_engine_pokertracker = new CSymbolEnginePokerTracker;
	AddSymbolEngine(p_symbol_engine_pokertracker);
	// CSymbolEngineIsTournament
	p_symbol_engine_istournament = new CSymbolEngineIsTournament();
	AddSymbolEngine(p_symbol_engine_istournament);
	// CSymbolEngineCards
	p_symbol_engine_cards = new CSymbolEngineCards();
	AddSymbolEngine(p_symbol_engine_cards);
	// CSymbolEngineIsOmaha
	p_symbol_engine_isomaha = new CSymbolEngineIsOmaha();
	AddSymbolEngine(p_symbol_engine_isomaha);
	// CSymbolEnginePokerval
	p_symbol_engine_pokerval = new CSymbolEnginePokerval();
	AddSymbolEngine(p_symbol_engine_pokerval);
	// CSymbolEngineHistory
	p_symbol_engine_history = new CSymbolEngineHistory();
	AddSymbolEngine(p_symbol_engine_history);
	// CSymbolEnginePrwin
	p_symbol_engine_prwin = new CSymbolEnginePrwin();
	AddSymbolEngine(p_symbol_engine_prwin);
	// CSymbolEngineHandrank
	p_symbol_engine_handrank = new CSymbolEngineHandrank();
	AddSymbolEngine(p_symbol_engine_handrank);
	// CSymbolEngineIniFunctions
	// "depends" on all other engines,
	// as it can only be called after all symbols have been initialized.
	p_symbol_engine_ini_functions = new CSymbolEngineIniFunctions();
	AddSymbolEngine(p_symbol_engine_ini_functions);
	// CSymbols
	// Deals with symbol-lookups and depends on all the other ones.
	// Therefore it has to be the very last one.
	p_symbols = new CSymbols();
	AddSymbolEngine(p_symbols);
	write_log(preferences.debug_engine_container(), "[EngineContainer] All symbol engines created\n");
}

void CEngineContainer::DestroyAllSymbolEngines()
{
	write_log(preferences.debug_engine_container(), "[EngineContainer] Going to destroy all symbol engines\n");
	for (int i=0; i<_number_of_symbol_engines_loaded; i++)
	{
		write_log(preferences.debug_engine_container(), "[EngineContainer] Going to delete symbol engine %i\n", i);
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
	p_symbol_engine_tablelimits->CalcTableLimits();

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
	write_log(preferences.debug_engine_container(), "[EngineContainer] Reset on connection\n");
	for (int i=0; i<_number_of_symbol_engines_loaded; i++)
	{
		_symbol_engines[i]->ResetOnConnection();
	}
}

void CEngineContainer::ResetOnHandreset()
{
	write_log(preferences.debug_engine_container(), "[EngineContainer] Reset on handreset\n");
	for (int i=0; i<_number_of_symbol_engines_loaded; i++)
	{
		_symbol_engines[i]->ResetOnHandreset();
	}
}

void CEngineContainer::ResetOnNewRound()
{
	write_log(preferences.debug_engine_container(), "[EngineContainer] Reset on new round\n");
	for (int i=0; i<_number_of_symbol_engines_loaded; i++)
	{
		_symbol_engines[i]->ResetOnNewRound();
	}
}

void CEngineContainer::ResetOnMyTurn()
{
	write_log(preferences.debug_engine_container(), "[EngineContainer] Reset on my turn\n");
	for (int i=0; i<_number_of_symbol_engines_loaded; i++)
	{
		_symbol_engines[i]->ResetOnMyTurn();
	}
}

void CEngineContainer::ResetOnHeartbeat()
{
	write_log(preferences.debug_engine_container(), "[EngineContainer] Reset on heartbeat\n");
	for (int i=0; i<_number_of_symbol_engines_loaded; i++)
	{
		_symbol_engines[i]->ResetOnHeartbeat();
	}
}

