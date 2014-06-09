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
#include "CSymbolEngineTime.h"

#include "NumericalFunctions.h"

CSymbolEngineTime *p_symbol_engine_time = NULL;

CSymbolEngineTime::CSymbolEngineTime() {
  // The values of some symbol-engines depend on other engines.
  // As the engines get later called in the order of initialization
  // we assure correct ordering by checking if they are initialized.
  //
  // This engine does not use any other engines.
  }

CSymbolEngineTime::~CSymbolEngineTime() {
}

void CSymbolEngineTime::InitOnStartup() {
  // time
  _elapsed      = 0;
  _elapsedhand  = 0;
  _elapsedauto  = 0;
  _elapsedtoday = 0;
  time(&_elapsedhold);
  time(&_elapsedhandhold);
  time(&_elapsedautohold);
}

void CSymbolEngineTime::ResetOnConnection() {
  time(&_elapsedhold);
  time(&_elapsedhandhold);
  time(&_elapsedautohold);
}

void CSymbolEngineTime::ResetOnHandreset() {
  time(&_elapsedhandhold);
}

void CSymbolEngineTime::ResetOnNewRound() {
}

void CSymbolEngineTime::ResetOnMyTurn() {
}

void CSymbolEngineTime::ResetOnHeartbeat() {
  // current time
  time_t t_now_time;
  time(&t_now_time);										

  // midnight time
  tm s_midnight_time = {0};
  localtime_s(&s_midnight_time, &t_now_time);
  s_midnight_time.tm_hour = 0;
  s_midnight_time.tm_min = 0;
  s_midnight_time.tm_sec = 0;
  time_t t_midnight_time = mktime(&s_midnight_time);
  _elapsedtoday = t_now_time - t_midnight_time;

  // time symbols
  _elapsed     = t_now_time - _elapsedhold;
  _elapsedhand = t_now_time - _elapsedhandhold;									
  _elapsedauto = t_now_time - _elapsedautohold;	

  assert(_elapsed < 1000000);					// Heuristic: about 300 hours up-time
  AssertRange(_elapsedhand, 0, _elapsed);	
  AssertRange(_elapsedauto, 0, _elapsed);
  AssertRange(_elapsedtoday, 0, (24 * 3600));
}	

void CSymbolEngineTime::ResetOnAutoPlayerAction() {
  time(&_elapsedautohold);
}

bool CSymbolEngineTime::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
  if (memcmp(name, "elapsed", 7)==0) {
	if (memcmp(name, "elapsed", 7)==0 && strlen(name)==7) {
      *result = elapsed();
	}
	else if (memcmp(name, "elapsedhand", 11)==0 && strlen(name)==11) {
      *result = elapsedhand();
    }
	else if (memcmp(name, "elapsedauto", 11)==0 && strlen(name)==11) {
      *result = elapsedauto();
	} else if (memcmp(name, "elapsedtoday", 12)==0 && strlen(name)==12)	{
      *result = elapsedtoday();
	} else {
	  // Invalid symbol
	   return false;
	}
	// Valid symbol
	return true;
  }
  // Symbol of name different symbol-engine
  return false;
}

CString CSymbolEngineTime::SymbolsProvided() {
  return "elapsed elapsedhand elapsedauto elapsedtoday";
}