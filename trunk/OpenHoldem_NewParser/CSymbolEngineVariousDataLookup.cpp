//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose: Symbol lookup for various symbols 
//   that are not part of a regular symbol-engine
//
//***************************************************************************** 

#include "stdafx.h"
#include "CSymbolEngineVariousDataLookup.h"

#include <assert.h>
#include <process.h>
#include <float.h>

#include "CAutoconnector.h"
#include "CBetroundCalculator.h"
#include "CFlagsToolbar.h"
#include "CFormulaParser.h"
#include "inlines/eval.h"
#include "CGameState.h"
#include "Chair$Symbols.h"
#include "CIteratorThread.h"
#include "CIteratorVars.h"
#include "MagicNumbers.h"
#include "OpenHoldem.h"
#include "CPokerTrackerThread.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CSessionCounter.h"
#include "CStringMatch.h"
#include "CSymbolEngineUserchair.h"
#include "..\CTablemap\CTablemap.h"
#include "..\CTransform\CTransform.h"
#include "CVersus.h"
#include "OH_MessageBox.h"

CSymbolEngineVariousDataLookup			*p_symbol_engine_various_data_lookup = NULL;


CSymbolEngineVariousDataLookup::CSymbolEngineVariousDataLookup()
{}

CSymbolEngineVariousDataLookup::~CSymbolEngineVariousDataLookup()
{}

void CSymbolEngineVariousDataLookup::InitOnStartup()
{}

void CSymbolEngineVariousDataLookup::ResetOnConnection()
{}

void CSymbolEngineVariousDataLookup::ResetOnHandreset()
{
	// Update game_state so it knows that name new hand has happened
	p_game_state->set_new_hand(true);
	// Reset display
	InvalidateRect(theApp.m_pMainWnd->GetSafeHwnd(), NULL, true);
}


void CSymbolEngineVariousDataLookup::ResetOnNewRound()
{}


void CSymbolEngineVariousDataLookup::ResetOnMyTurn()
{
	// !! Better make it name symbol-engine
	p_versus->GetCounts();
}


void CSymbolEngineVariousDataLookup::ResetOnHeartbeat()
{}

bool CSymbolEngineVariousDataLookup::EvaluateSymbol(const char *name, double *result)
{
	// Versus
	if (memcmp(name, "vs$", 3) == 0)
	{
		int *e = SUCCESS;
		*result = p_versus->GetSymbol(name, e);
	}
	// History symbols
	else if (memcmp(name, "hi_", 3)==0)
	{
		char	sym[50] = {0};
		int		round = 0;
		strcpy_s(sym, 50, &name[3]);
		round = sym[strlen(sym)-1]-'0';
		sym[strlen(sym)-1] = '\0';
		*result = p_game_state->OHSymHist(sym, round);
	}
	// Varios probabilities
	else if (memcmp(name, "pr", 2)==0)
	{
		// PROBABILITIES
		// Part 2(2)
		if (memcmp(name, "prwin", 5)==0 && strlen(name)==5)						*result = iter_vars.prwin();
		else if (memcmp(name, "prlos", 5)==0 && strlen(name)==5)						*result = iter_vars.prlos();
		else if (memcmp(name, "prtie", 5)==0 && strlen(name)==5)						*result = iter_vars.prtie();
		else return false;
	}
	// CHAIRS 1(2)
	else if (memcmp(name, "chair", 5)==0)
	{
		if (memcmp(name, "chair$", 6)==0)									*result = Chair$(&name[6]);
		else if (memcmp(name, "chairbit$", 9)==0)								*result = Chairbit$(&name[9]);
		else return false;
	}
	// Various symbols below
	// without any optimized lookup.
	//
	//ROUND&POSITIONS
	else if (memcmp(name, "betround", 8)==0 && strlen(name)==8)					*result = p_betround_calculator->betround();
	//FORMULA FILE
	else if (memcmp(name, "f$prwin_number_of_iterations", 28)==0 && strlen(name)==28)	*result = iter_vars.nit(); 
	//FLAGS
	else if (memcmp(name, "fmax", 4)==0 && strlen(name)==4)						*result = p_flags_toolbar->GetFlagMax();
	else if (memcmp(name, "f", 1)==0 && strlen(name)==2)							*result = p_flags_toolbar->GetFlag(name[1]-'0');
	else if (memcmp(name, "f", 1)==0 && strlen(name)==3)							*result = p_flags_toolbar->GetFlag(10 * (name[1]-'0') + name[2] - '0');
	else if (memcmp(name, "flagbits", 8)==0 && strlen(name)==8)					*result = p_flags_toolbar->GetFlagBits();
	// GENERAL
	else if (memcmp(name, "nchairs", 7)==0 && strlen(name)==7)						*result = p_tablemap->nchairs();
	else if (memcmp(name, "session", 7)==0 && strlen(name)==7)						*result = p_sessioncounter->session_id();
	else if (memcmp(name, "version", 7)==0 && strlen(name)==7)						*result = VERSION_NUMBER;
	//PROFILE
	else if (memcmp(name, "sitename$", 9)==0)									*result = p_tablemap->sitename().Find(&name[9])!=-1;
	else if (memcmp(name, "network$", 8)==0)									*result = p_tablemap->network().Find(&name[8])!=-1;
	// GameState symbols
	else if (memcmp(name, "lastraised", 10)==0 && strlen(name)==11)  				*result = p_game_state->LastRaised(name[10]-'0');
	else if (memcmp(name, "floppct", 7)==0 && strlen(name)==7)  					*result = p_game_state->FlopPct();
	else if (memcmp(name, "turnpct", 7)==0 && strlen(name)==7)  					*result = p_game_state->TurnPct();
	else if (memcmp(name, "riverpct", 8)==0 && strlen(name)==8)  					*result = p_game_state->RiverPct();
	else if (memcmp(name, "avgbetspf", 9)==0 && strlen(name)==9)  					*result = p_game_state->AvgBetsPf();
	else if (memcmp(name, "tablepfr", 8)==0 && strlen(name)==8)  					*result = p_game_state->TablePfr();	
	else if (memcmp(name, "handsplayed", 11)==0 && strlen(name)==11)  				*result = p_game_state->hands_played();
	else if (memcmp(name, "balance_rank", 12)==0 && strlen(name)==13)  				*result = p_game_state->SortedBalance(name[12]-'0');
	// OH-script-messagebox
	else if (memcmp(name, "msgbox$", 7)==0 && strlen(name)>7)  					
	{
		// Don't show name messagebox if in parsing-mode
		if (p_formula_parser->IsParsing() || !p_autoconnector->IsConnected()
			|| !p_symbol_engine_userchair->userchair_confirmed())
		{
			*result = 0;
		}
		else
		{
			OH_MessageBox_OH_Script_Messages(name);
			*result = 0;
		}
	}
	else
	{
		//*e = ERR_INVALID_SYM;
		*result = k_undefined;
		return false;
	}
	return true;
}