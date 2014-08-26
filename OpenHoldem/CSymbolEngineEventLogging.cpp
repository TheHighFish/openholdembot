//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*****************************************************************************
//
// Purpose: Not really a symbol-engine, 
//   but a good way to get important events logged.
//
//*****************************************************************************

#include "stdafx.h"
#include "CSymbolEngineEventLogging.h"

#include "CAutoconnector.h"
#include "CHandresetDetector.h"
#include "CScraper.h"
#include "CSymbolEngineDealerchair.h"
#include "CSymbolEngineUserchair.h"
#include "CTableState.h"
#include "CVersionInfo.h"

CSymbolEngineEventLogging *p_symbol_engine_event_logging = NULL;

CSymbolEngineEventLogging::CSymbolEngineEventLogging()
{}

CSymbolEngineEventLogging::~CSymbolEngineEventLogging()
{}

void CSymbolEngineEventLogging::InitOnStartup()
{}

void CSymbolEngineEventLogging::ResetOnConnection()
{}

void CSymbolEngineEventLogging::ResetOnHandreset()
{
	int player_card_cur[2] = {0};
  player_card_cur[0] = p_table_state->User()->_hole_cards[0].GetValue();
	player_card_cur[1] = p_table_state->User()->_hole_cards[1].GetValue();

  char title[512] = "--- No title specified ---";
	GetWindowText(p_autoconnector->attached_hwnd(), title, 512);
	write_log(k_always_log_basic_information, 
		"\n"
		"==============================================\n"
		"%s"					// Version info already contains a newline 
		"HAND RESET (num: %s dealer: %d cards: %s): %s\n",
		p_version_info->GetVersionInfo(),
		p_handreset_detector->GetHandNumber(),
		p_symbol_engine_dealerchair->dealerchair(), 
    p_table_state->User()->Cards(), title);
}

void CSymbolEngineEventLogging::ResetOnNewRound()
{}

void CSymbolEngineEventLogging::ResetOnMyTurn()
{}

void CSymbolEngineEventLogging::ResetOnHeartbeat()
{}

bool CSymbolEngineEventLogging::EvaluateSymbol(const char *name, double *result, bool log /* = false */)
{
	// We don't provide any symbols
	return false;
}
