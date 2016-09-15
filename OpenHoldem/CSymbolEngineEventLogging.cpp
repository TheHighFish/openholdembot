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
//******************************************************************************

#include "stdafx.h"
#include "CSymbolEngineEventLogging.h"

#include "CAutoconnector.h"
#include "CAutoplayerTrace.h"
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

void CSymbolEngineEventLogging::ResetOnConnection() {
  p_autoplayer_trace->Clear();
}

void CSymbolEngineEventLogging::ResetOnHandreset() {
  // Log a new connection, plus the version-info
  // (because of all the guys who report "bugs" of outdated versions)
  char title[512] = "--- title undefined ---";
  GetWindowText(p_autoconnector->attached_hwnd(), title, 512);
 write_log(k_always_log_basic_information,
    "\n"
    "==============================================\n"
    "%s"    // Version info already contains a newline
    "Table: %s\n"
    "==============================================\n"
    "HAND RESET\n"
    "==============================================\n",
    p_version_info->GetVersionInfo(), title);
}

void CSymbolEngineEventLogging::ResetOnNewRound()
{}

void CSymbolEngineEventLogging::ResetOnMyTurn()
{}

void CSymbolEngineEventLogging::ResetOnHeartbeat() {
  p_autoplayer_trace->Clear();
}

bool CSymbolEngineEventLogging::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
	// We don't provide any symbols
	return false;
}
