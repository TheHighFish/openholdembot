//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose: Providing functions (f$init_once_per_hand, etc.)
//   to deal with memory-symbols and other initializations (DLL maybe)
//   and get rid of the old advice "put it into f$alli". 
//
// Not really a symbol-engine, but easy to implement with this concept.
//
//***************************************************************************** 

#include "stdafx.h"
#include "CSymbolEngineIniFunctions.h"

#include <assert.h>
#include "CFormula.h"
#include "CGrammar.h"
#include "CPreferences.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineBlinds.h"
#include "CSymbolEngineCards.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineDealerchair.h"
#include "CSymbolEngineHandrank.h"
#include "CSymbolEngineHistory.h"
#include "CSymbolEngineIsOmaha.h"
#include "CSymbolEngineIsTournament.h"
#include "CSymbolEngineLists.h"
#include "CSymbolEnginePokerTracker.h"
#include "CSymbolEnginePokerval.h"
#include "CSymbolEnginePositions.h"
#include "CSymbolEnginePrwin.h"
#include "CSymbolEngineRaisersCallers.h"
#include "CSymbolEngineRandom.h"
#include "CSymbolEngineTime.h"
#include "CSymbolEngineUserchair.h"
#include "MagicNumbers.h"

// !!! Temporary disabled as we get crashes in release and optimized mode,
// !!! not in debug-mode unfortunatelly.
// !!! We expect that problem to disappear with the new parser and evaluator soon.

CSymbolEngineIniFunctions *p_symbol_engine_ini_functions = NULL;

// We can't evaluate ini-functions if no formula is loaded.
// This was no problem on startup, but caused crashes
// when we tried to load a new formula.
// http://www.maxinmontreal.com/forums/viewtopic.php?f=110&t=16788&start=30
#define RETURN_IF_LOADING_NEW_FORMULA if ((p_formula == NULL) || (p_formula->IsParsing())) return;


CSymbolEngineIniFunctions::CSymbolEngineIniFunctions()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	//
	// CSymbolEngineIniFunctions() "depends" on all other engines,
	// as it can only be called after all symbols have been initialized.
	assert(p_symbol_engine_active_dealt_playing != NULL);
	assert(p_symbol_engine_autoplayer != NULL);
	assert(p_symbol_engine_blinds != NULL);
	assert(p_symbol_engine_cards != NULL);
	assert(p_symbol_engine_chip_amounts != NULL);
	assert(p_symbol_engine_dealerchair != NULL);
	assert(p_symbol_engine_handrank != NULL);
	assert(p_symbol_engine_history != NULL);
	assert(p_symbol_engine_isomaha != NULL);
	assert(p_symbol_engine_istournament != NULL);
	assert(p_symbol_engine_lists != NULL);
	assert(p_symbol_engine_pokertracker != NULL);
	assert(p_symbol_engine_pokerval != NULL);
	assert(p_symbol_engine_positions != NULL);
	assert(p_symbol_engine_prwin != NULL);
	assert(p_symbol_engine_raisers_callers != NULL);
	assert(p_symbol_engine_random != NULL);
	assert(p_symbol_engine_time != NULL);
	assert(p_symbol_engine_userchair != NULL);
}

CSymbolEngineIniFunctions::~CSymbolEngineIniFunctions()
{}

void CSymbolEngineIniFunctions::InitOnStartup()
{
	RETURN_IF_LOADING_NEW_FORMULA
	return;//!!!

	int dummy_e = SUCCESS;
	write_log(preferences.debug_symbolengine(), "[Symbolengine] CSymbolEngineIniFunctions::InitOnStartup()\n");
	ResetOnConnection();
	gram.CalcF$symbol(p_formula, k_standard_function_names[k_init_on_startup], 
		&dummy_e);
}

void CSymbolEngineIniFunctions::ResetOnConnection()
{
	RETURN_IF_LOADING_NEW_FORMULA
	return;//!!!
		
	int dummy_e = SUCCESS;
	write_log(preferences.debug_symbolengine(), "[Symbolengine] CSymbolEngineIniFunctions::ResetOnConnection()\n");
	write_log(preferences.debug_symbolengine(), "[Symbolengine] evaluating %s %s\n",
		p_formula->formula_name(), k_standard_function_names[k_init_on_connection]);
	//gram.CalcF$symbol(p_formula, k_standard_function_names[k_init_on_connection], 
	//	&dummy_e);
	write_log(preferences.debug_symbolengine(), "[Symbolengine] CSymbolEngineIniFunctions::ResetOnConnection() completed\n");
}

void CSymbolEngineIniFunctions::ResetOnHandreset()
{
	RETURN_IF_LOADING_NEW_FORMULA
	return;//!!!
	int dummy_e = SUCCESS;
	write_log(preferences.debug_symbolengine(), "[Symbolengine] CSymbolEngineIniFunctions::ResetOnHandreset()\n");
	gram.CalcF$symbol(p_formula, k_standard_function_names[k_init_on_handreset], 
		&dummy_e);
}

void CSymbolEngineIniFunctions::ResetOnNewRound()
{
	RETURN_IF_LOADING_NEW_FORMULA
	return; //!!!	
	int dummy_e = SUCCESS;
	write_log(preferences.debug_symbolengine(), "[Symbolengine] CSymbolEngineIniFunctions::ResetOnNewRound()\n");
	gram.CalcF$symbol(p_formula, k_standard_function_names[k_init_on_new_round], 
		&dummy_e);
}

void CSymbolEngineIniFunctions::ResetOnMyTurn()
{
	RETURN_IF_LOADING_NEW_FORMULA
	return;//!!!
	int dummy_e = SUCCESS;
	write_log(preferences.debug_symbolengine(), "[Symbolengine] CSymbolEngineIniFunctions::ResetOnMyTurn()\n");
	gram.CalcF$symbol(p_formula, k_standard_function_names[k_init_on_my_turn], 
		&dummy_e);
}

void CSymbolEngineIniFunctions::ResetOnHeartbeat()
{
	RETURN_IF_LOADING_NEW_FORMULA
	return;//!!!
	int dummy_e = SUCCESS;
	write_log(preferences.debug_symbolengine(), "[Symbolengine] CSymbolEngineIniFunctions::ResetOnHeartbeat()\n");
	write_log(preferences.debug_symbolengine(), "[Symbolengine] CSymbolEngineIniFunctions::ResetOnHeartbeat() evaluating %s\n",
		k_standard_function_names[k_init_on_heartbeat]);
	gram.CalcF$symbol(p_formula, (char *)k_standard_function_names[k_init_on_heartbeat], 
		&dummy_e);
	write_log(preferences.debug_symbolengine(), "[Symbolengine] CSymbolEngineIniFunctions::ResetOnHeartbeat() error-code: %i\n",
		dummy_e);
}
