//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
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
  player_card_cur[0] = p_table_state->_players[USER_CHAIR].hole_cards[0].GetValue();
	player_card_cur[1] = p_table_state->_players[USER_CHAIR].hole_cards[1].GetValue();

  //!!! ToLogString
	char card0[10];
	char card1[10];
	// log new hand
	if (player_card_cur[0]==CARD_NOCARD || player_card_cur[1]==CARD_NOCARD)
	{
		strcpy_s(card0, 10, "NONE");
		strcpy_s(card1, 10, "");
	}
	else if (player_card_cur[0]==CARD_BACK || player_card_cur[1]==CARD_BACK)
	{
		strcpy_s(card0, 10, "BACKS");
		strcpy_s(card1, 10, "");
	}
	else
	{
		StdDeck_cardToString(player_card_cur[0], card0);
		StdDeck_cardToString(player_card_cur[1], card1);
	}
	char title[512] = "--- No title specified ---";
	GetWindowText(p_autoconnector->attached_hwnd(), title, 512);
	write_log(k_always_log_basic_information, 
		"\n"
		"==============================================\n"
		"%s"					// Version info already contains a newline 
		"HAND RESET (num: %s dealer: %d cards: %s%s): %s\n",
		p_version_info->GetVersionInfo(),
		p_handreset_detector->GetHandNumber(),
		p_symbol_engine_dealerchair->dealerchair(), 
		card0, card1, title);
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
