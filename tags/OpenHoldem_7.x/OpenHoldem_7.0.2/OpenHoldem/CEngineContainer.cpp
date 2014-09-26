//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#include "stdafx.h"
#include "CEngineContainer.h"

#include <assert.h>
#include "CAutoplayerTrace.h"
#include "CBetroundCalculator.h"
#include "CFormulaParser.h"
#include "CFunctionCollection.h"
#include "CHandresetDetector.h"
#include "CParseErrors.h"
#include "CPreferences.h"
#include "CScraperAccess.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineBlinds.h"
#include "CSymbolEngineCards.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineDealerchair.h"
#include "CSymbolEngineEventLogging.h"
#include "CSymbolEngineHandrank.h"
#include "CSymbolEngineHistory.h"
#include "CSymbolEngineICM.h"
#include "CSymbolEngineIniFunctions.h"
#include "CSymbolEngineIsOmaha.h"
#include "CSymbolEngineIsTournament.h"
#include "CSymbolEngineMemorySymbols.h"
#include "CSymbolEngineOpenPPL.h"
#include "CSymbolEngineOpenPPLHandAndBoardExpression.h"
#include "CSymbolEngineOpenPPLUserVariables.h"
#include "CSymbolEnginePokerAction.h"
#include "CSymbolEnginePokerTracker.h"
#include "CSymbolEnginePokerval.h"
#include "CSymbolEnginePositions.h"
#include "CSymbolEnginePrwin.h"
#include "CSymbolEngineRaisersCallers.h"
#include "CSymbolEngineRandom.h"
#include "CSymbolEngineReplayFrameController.h"
#include "CSymbolEngineTime.h"
#include "CSymbolEngineUserchair.h"
#include "CSymbolEngineVariousDataLookup.h"
#include "CSymbolEngineVersus.h"
#include "UnknownSymbols.h"

CEngineContainer *p_engine_container = NULL;
  CEngineContainer::CEngineContainer() {
  write_log(preferences.debug_engine_container(), "[EngineContainer] CEngineContainer()\n");
  CreateSymbolEngines();
  // First initialization is the same as on a new connection
  ResetOnConnection();
  // But we want to initialize later again on every connection
  _reset_on_connection_executed = false;
  write_log(preferences.debug_engine_container(), "[EngineContainer] CEngineContainer() finished\n");
}

CEngineContainer::~CEngineContainer() {
  DestroyAllSymbolEngines();
}

void CEngineContainer::CreateSpecialSymbolEngines() {
  // Some engines are "special", because we need to call them up-front,
  // e.g. to detect a hand-reset.
  // So they work slightly different and also get their own initialization.
  p_betround_calculator = new CBetroundCalculator();	
}

void CEngineContainer::AddSymbolEngine(CVirtualSymbolEngine *new_symbol_engine) {
  assert(_number_of_symbol_engines_loaded < k_max_number_of_symbol_engines);
  _symbol_engines[_number_of_symbol_engines_loaded] = new_symbol_engine;
  ++_number_of_symbol_engines_loaded;
}

void CEngineContainer::CreateSymbolEngines() {
  write_log(preferences.debug_engine_container(), "[EngineContainer] Going to create symbol engines\n");
  CreateSpecialSymbolEngines();

  _number_of_symbol_engines_loaded = 0;
  // Some symbols to be calculated depend on symbols of other engines.
  // The engines inserted first will be called first later.
  // But we assure correct ordering by assertions in the constructors of the engines.

  // CFunctionCollection
  p_function_collection = new CFunctionCollection;
  AddSymbolEngine(p_function_collection);
  // CSymbolEngineUserchair
  p_symbol_engine_userchair = new CSymbolEngineUserchair();
  AddSymbolEngine(p_symbol_engine_userchair);
  // CSymbolEngineDealerchair
  p_symbol_engine_dealerchair = new CSymbolEngineDealerchair();
  AddSymbolEngine(p_symbol_engine_dealerchair);
  // CSymbolEngineEventLogging
  p_symbol_engine_event_logging = new CSymbolEngineEventLogging();
  AddSymbolEngine(p_symbol_engine_event_logging);
  // CSymbolEngineTableLimits
  p_symbol_engine_tablelimits = new CSymbolEngineTableLimits ();
  AddSymbolEngine(p_symbol_engine_tablelimits);
  // CSymbolEngineReplayFrameController
  p_symbol_engine_replayframe_controller = new CSymbolEngineReplayFrameController();
  AddSymbolEngine(p_symbol_engine_replayframe_controller);
  // CSymbolEngineTime
  p_symbol_engine_time = new CSymbolEngineTime();
  AddSymbolEngine(p_symbol_engine_time);
  // CSymbolEngineAutoplayer
  p_symbol_engine_autoplayer = new CSymbolEngineAutoplayer();
  AddSymbolEngine(p_symbol_engine_autoplayer);
  // CSymbolEngineRandom
  p_symbol_engine_random = new CSymbolEngineRandom();
  AddSymbolEngine(p_symbol_engine_random);
  // CSymbolEngineVersus
  p_symbol_engine_versus = new CSymbolEngineVersus;
  AddSymbolEngine(p_symbol_engine_versus);
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
  // CSymbolEngineICM
  p_symbol_engine_icm = new CSymbolEngineICM();
  AddSymbolEngine(p_symbol_engine_icm);
  // CSymbolEngineRaisersCallers
  p_symbol_engine_raisers_callers = new CSymbolEngineRaisersCallers();
  AddSymbolEngine(p_symbol_engine_raisers_callers);
  // CSymbolEnginePokerTracker
  p_symbol_engine_pokertracker = new CSymbolEnginePokerTracker;
  AddSymbolEngine(p_symbol_engine_pokertracker);
  // CSymbolEnginePokerActio
  p_symbol_engine_poker_action = new CSymbolEnginePokerAction();
  AddSymbolEngine(p_symbol_engine_poker_action);
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
  // CSymbolEngineOpenPPLHandAndBoardExpression
  p_symbol_engine_open_ppl_hand_and_board_expression = new CSymbolEngineOpenPPLHandAndBoardExpression();
  AddSymbolEngine(p_symbol_engine_open_ppl_hand_and_board_expression);
  // CSymbolEngineHistory
  p_symbol_engine_history = new CSymbolEngineHistory();
  AddSymbolEngine(p_symbol_engine_history);
  // CSymbolEnginePrwin
  p_symbol_engine_prwin = new CSymbolEnginePrwin();
  AddSymbolEngine(p_symbol_engine_prwin);
  // CSymbolEngineHandrank
  p_symbol_engine_handrank = new CSymbolEngineHandrank();
  AddSymbolEngine(p_symbol_engine_handrank);
  // CSymbolEngineMemorySymbols
  p_symbol_engine_memory_symbols = new CSymbolEngineMemorySymbols;
  AddSymbolEngine(p_symbol_engine_memory_symbols);
  // CSymbolEngineIniFunctions
  // "depends" on all other engines,
  // as it can only be called after all symbols have been initialized.
  p_symbol_engine_ini_functions = new CSymbolEngineIniFunctions();
  AddSymbolEngine(p_symbol_engine_ini_functions);
  // CSymbolEngineVariousDataLookup
  // Deals with symbol-lookups and depends on all the other OH-script ones.
  // Therefore it has to be the last OH-script one.
  p_symbol_engine_various_data_lookup = new CSymbolEngineVariousDataLookup;
  AddSymbolEngine(p_symbol_engine_various_data_lookup);
  // OpenPPL-symbol-engines
  p_symbol_engine_openppl_user_variables = new CSymbolEngineOpenPPLUserVariables;
  AddSymbolEngine(p_symbol_engine_openppl_user_variables);
  // CSymbolEngineOpenPPL triigers calculation of history-symbols
  // and therefore has to be the very last openPPL-symbol-engine
  p_symbol_engine_open_ppl = new CSymbolEngineOpenPPL;
  AddSymbolEngine(p_symbol_engine_open_ppl);
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
	write_log(preferences.debug_engine_container(), "[EngineContainer] All symbol engines successfully destroyed\n");
}

void CEngineContainer::DestroyAllSpecialSymbolEngines()
{
	delete p_betround_calculator;
}

void CEngineContainer::EvaluateAll()
{
	write_log(preferences.debug_engine_container(), "[EngineContainer] EvaluateAll()\n");
	if (!_reset_on_connection_executed)
	{
		write_log(preferences.debug_engine_container(), "[EngineContainer] Skipping as ResetOnConnection not yet executed.\n");
		write_log(preferences.debug_engine_container(), "[EngineContainer] Waiting for call by auto-connector-thread\n");
		// The problem with ResetOnConnection:
		// It will be called by another thread,
		// so the execution might be out of order.
		// Therefore we have to skip all other calculations
		// until OnConnection() got executed.
		return;
	}
	if (p_formula_parser == NULL) {
		// No formula loaded
		return;
	}
	if (p_formula_parser->IsParsing()) {
		// Not safe to evaluate anything
		return;
	}
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
	p_autoplayer_trace->Clear();
	for (int i=0; i<_number_of_symbol_engines_loaded; i++)
	{
		_symbol_engines[i]->ResetOnConnection();
	}
	_reset_on_connection_executed = true;
	write_log(preferences.debug_engine_container(), "[EngineContainer] Reset on connection finished\n");
}

void CEngineContainer::ResetOnDisconnection()
{
	write_log(preferences.debug_engine_container(), "[EngineContainer] Reset on disconnection\n");
	// Just to make sure that our connection-code
	// will be executed later in correct order
	_reset_on_connection_executed = false;
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
	__TRACE
	write_log(preferences.debug_engine_container(), "[EngineContainer] Reset on my turn\n");
	for (int i=0; i<_number_of_symbol_engines_loaded; i++)
	{
		__TRACE
		_symbol_engines[i]->ResetOnMyTurn();
	}
}

void CEngineContainer::ResetOnHeartbeat() {
  write_log(preferences.debug_engine_container(), "[EngineContainer] Reset on heartbeat\n");
  p_autoplayer_trace->Clear();
  for (int i=0; i<_number_of_symbol_engines_loaded; i++)
  {
	  _symbol_engines[i]->ResetOnHeartbeat();
  }
}

bool CEngineContainer::EvaluateSymbol(const char *name, 
                                      double *result, 
                                      bool log /* = false */) {
  write_log(preferences.debug_engine_container(), "[EngineContainer] EvaluateSymbol(%s)\n", name);
  if (IsOutdatedSymbol(name)) {
	*result = k_undefined;
	return false;
  }
  for (int i=0; i<_number_of_symbol_engines_loaded; i++) {
    if (_symbol_engines[i]->EvaluateSymbol(name, result, log)) {
      // Symbol successfully evaluated
      // Result already returned via result-pointer
      if (COHScriptObject::IsFunction(name) 
          || COHScriptObject::IsOpenPPLSymbol(name)) {
        // Nothing to do (see below)
        return true;
      }
      if (log) {
        // Log the symbol and its value
        // But only if it is a basic symbol and not a function
        // Functions receive special treatment (indentation, etc)
        write_log(preferences.debug_auto_trace(),
          "[EngineContainer] %s -> %.3f [evaluated]\n", name, *result);
        p_autoplayer_trace->Add(name, *result);
      }
      return true;
    }
  }
  // Unknown symbol
  if (p_formula_parser->IsParsing()) {
    // Generate a verbose error-message
    // with line number and code-snippet
    CParseErrors::ErrorUnknownIdentifier(name);
    // Don't change the result, which is a magic number
    // (ATM unused)
    return false;
  }
  else {
    // Error found during execution
    // Though we check the syntax, this can still happen
    // by gws-calls from Perl or a DLL, etc.
    WarnAboutUnknownSymbol(name);
    *result = k_undefined;
    return false;
  }
}

void CEngineContainer::BuildListOfSymbolsProvided() {
  write_log(preferences.debug_engine_container(), "[EngineContainer] Building list of symbols\n");
  _list_of_symbols = "";
  for (int i=0; i<_number_of_symbol_engines_loaded; ++i) {
    _list_of_symbols.Append(_symbol_engines[i]->SymbolsProvided());
    // Extra blank to avoid unexpected concatenation of symbols
    _list_of_symbols.Append(" ");
  }
  write_log(preferences.debug_engine_container(), "[EngineContainer] List of symbols \"%s\"\n",
    _list_of_symbols);
}