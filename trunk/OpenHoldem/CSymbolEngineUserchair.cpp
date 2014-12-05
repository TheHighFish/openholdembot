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
#include "CSymbolEngineUserchair.h"

#include "CBetroundCalculator.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CStringMatch.h"
#include "CTableState.h"
#include "MagicNumbers.h"

CSymbolEngineUserchair *p_symbol_engine_userchair = NULL;

CSymbolEngineUserchair::CSymbolEngineUserchair()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	//
	// This engine does not use any other engines.
}

CSymbolEngineUserchair::~CSymbolEngineUserchair()
{}

void CSymbolEngineUserchair::InitOnStartup()
{
	ResetOnConnection();
}

void CSymbolEngineUserchair::ResetOnConnection()
{
	_userchair = k_undefined;
}

void CSymbolEngineUserchair::ResetOnHandreset()
{
}

void CSymbolEngineUserchair::ResetOnNewRound()
{}

void CSymbolEngineUserchair::ResetOnMyTurn()
{}

void CSymbolEngineUserchair::ResetOnHeartbeat()
{
	if (!userchair_confirmed() || (p_scraper_access->NumberOfVisibleButtons() > 0))
	{
		CalculateUserChair();
	}
}

bool CSymbolEngineUserchair::IsNotShowdown() {
  int num_buttons_enabled = p_scraper_access->NumberOfVisibleButtons();
  if (num_buttons_enabled >= k_min_buttons_needed_for_my_turn) return true;
  if (p_betround_calculator->betround() < k_betround_river) return true;
  return false;
}

void CSymbolEngineUserchair::CalculateUserChair() {
	if (userchair_confirmed() && p_table_state->User()->HasKnownCards()) {
		write_log(preferences.debug_symbolengine(),
			"[CSymbolEngineUserchair] CalculateUserChair() Known cards for known chair. Keeping userchair as is\n");
	}	else {
		// Either not confirmed or no known cards when it is my turn
		// Looking for known cards and new chair
		for (int i=0; i<p_tablemap->nchairs(); i++)
		{
			if (p_table_state->_players[i].HasKnownCards() && IsNotShowdown()) {
				_userchair = i;
				write_log(preferences.debug_symbolengine(),
					"[CSymbolEngineUserchair] CalculateUserChair() Setting userchair to %d\n",
					_userchair);
				return;
			}
		}
		write_log(preferences.debug_symbolengine(),
			"[CSymbolEngineUserchair] CalculateUserChair() Userchair not found, because no cards found. Keeping as is\n");
	}
}

bool CSymbolEngineUserchair::EvaluateSymbol(const char *name, double *result, bool log /* = false */)
{
  FAST_EXIT_ON_OPENPPL_SYMBOLS(name);
	if (memcmp(name, "userchair", 9)==0 && strlen(name)==9)
	{
		*result = userchair();
		return true;
	}
	// Symbol of a different symbol-engine
	return false;
}

CString CSymbolEngineUserchair::SymbolsProvided() {
  return "userchair ";
}
