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
#include "CAutoplayerTrace.h"

#include "CAutoplayerFunctions.h"
#include "CBetroundCalculator.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineHandrank.h"
#include "CSymbolEnginePokerval.h"
#include "CSymbolEnginePrwin.h"

#include "CSymbolEngineUserchair.h"

#define ENT CSLock lock(m_critsec);

CAutoplayerTrace *p_autoplayer_trace = NULL;

CAutoplayerTrace::CAutoplayerTrace() {
  Clear();
}

CAutoplayerTrace::~CAutoplayerTrace()
{}

void CAutoplayerTrace::Clear() {
  ENT
  _indentation = 0;
  _number_of_log_lines = 0;
  _symboltrace_collection.RemoveAll();
}

int CAutoplayerTrace::Add(CString symbol) {
  ENT
  // This function for symbols without value is for f$functions only.
  // The value will be backpatched later.
  assert(symbol.Left(2) == "f$");
  CString new_message;
  new_message.Format("%s%s = ",
    Indentation(), symbol);
  _symboltrace_collection.Add(new_message);
  _number_of_log_lines++;
  return (_number_of_log_lines - 1); 
}

void CAutoplayerTrace::Add(CString symbol, double value) {
  ENT
  CString new_message;
  if (symbol.Left(2) == "f$") {
    // Function with known value a priori
    new_message.Format("%s%s = %.3f {cached]",
      Indentation(), symbol, value);
  } else {
    // "Normal" symbol
    new_message.Format("%s%s = %.3f",
      Indentation(), symbol, value); 
  }
  _symboltrace_collection.Add(new_message);
  _number_of_log_lines++;
}

void CAutoplayerTrace::BackPatchValue(int index, double value) {
  assert(index >= 0);
  assert(index < _number_of_log_lines);
  // Already done:
  // Indentation, symbol, " = "
  CString complete_message;
  complete_message.Format("%s%.3f", 
    _symboltrace_collection.GetAt(index),
    value);
  _symboltrace_collection.SetAt(index, complete_message);
}

void CAutoplayerTrace::Indent(bool more) {
  if (more) {
    _indentation += 2;
  } else {
    _indentation -= 2;
  }
  assert(_indentation >= 0);
}

CString CAutoplayerTrace::Indentation() {
  assert(_indentation >= 0);
  CString format;
  format.Format("%%%ds", _indentation);
  CString indentation;
  indentation.Format(format, "");
  return indentation;
}

void CAutoplayerTrace::Print(const char *action_taken) {
  if (!preferences.trace_enabled()) {
    return;
  }
  if (!log_fp) {
    return;
  }
  CSLock lock(log_critsec);
  LogBasicInfo(action_taken);
  LogAutoPlayerTrace();
  fflush(log_fp);
  Clear();
}

void CAutoplayerTrace::LogLogSymbols() {
  // Logging of log$symbols
  if (!preferences.log_symbol_enabled()) {
    return;
  }
  int max_log = _logsymbols_collection.GetCount();
  if (max_log <= 0) {
    return;
  }
  if (max_log > preferences.log_symbol_max_log()) {
    max_log = preferences.log_symbol_max_log();
  }
  write_log(k_always_log_basic_information, "*** log$ (Total: %d | Showing: %d)\n", 
    _logsymbols_collection.GetCount(), max_log);
  for (int i=0; i<max_log; i++) {
    write_log(k_always_log_basic_information, "***     %s\n", 
      _logsymbols_collection.GetAt(i));
  }
}

void CAutoplayerTrace::LogBasicInfo(const char *action_taken) {
  char		nowtime[26];
  CString	pcards, comcards, temp, rank, pokerhand, bestaction;
  char		*card;
  CString	fcra_formula_status;
  int		userchair = p_symbol_engine_userchair->userchair();
  int		betround  = p_betround_calculator->betround();

  // player cards
  if (p_symbol_engine_userchair->userchair_confirmed()) {
    for (int i=0; i<=1; i++) {
      card = StdDeck_cardString(p_scraper->card_player(userchair, i));
      temp.Format("%s", card);
      pcards.Append(temp);
    }
  } else {
	pcards = "....";
  }
  // common cards
  comcards = "";
  if (betround >= k_betround_flop) {
    for (int i=0; i<k_number_of_flop_cards; i++) {
      if (p_scraper_access->IsKnownCard(p_scraper->card_common(i))) {
	      card = StdDeck_cardString(p_scraper->card_common(i));
	      temp.Format("%s", card);
	      comcards.Append(temp);
      }
    }
  }
  if (betround >= k_betround_turn) {
    card = StdDeck_cardString(p_scraper->card_common(3));
    temp.Format("%s", card);
    comcards.Append(temp);
  }
  if (betround >= k_betround_river) {
    card = StdDeck_cardString(p_scraper->card_common(4));
    temp.Format("%s", card);
    comcards.Append(temp);
  }
  comcards.Append("..........");
  comcards = comcards.Left(10);
  // Always use handrank169 here
  rank.Format("%.0f", p_symbol_engine_handrank->handrank169());
  // poker hand
  pokerhand = p_symbol_engine_pokerval->HandType();
  // best action
  // !! needs to be extended for betpot, etc.
  if (p_autoplayer_functions->f$alli())
    bestaction = "Allin";
  else if (p_autoplayer_functions->f$betsize() > 0)
    bestaction.Format("Raise to $%.2f", p_autoplayer_functions->f$betsize());				
  else if (p_autoplayer_functions->f$rais())
    bestaction = "Bet/Raise";
  else if (p_autoplayer_functions->f$call())
    bestaction = "Call/Check";
  else if (p_autoplayer_functions->f$prefold())
    bestaction = "Prefold";
  else
    bestaction = "Check/Fold";
  // fcra_seen
  CString fcra_seen = p_symbol_engine_autoplayer->GetFCKRAString();
  // fcra formula status
  fcra_formula_status.Format("%s%s%s%s%s",
	p_autoplayer_functions->f$fold() ? "F" : ".",
	p_autoplayer_functions->f$call() ? "C" : ".",
	p_autoplayer_functions->f$call() ? "K" : ".",
	p_autoplayer_functions->f$rais() ? "R" : ".",
	p_autoplayer_functions->f$alli() ? "A" : ".");
  // More verbose summary in the log
  // The old WinHoldem format was a complete mess
  fprintf(log_fp, get_time(nowtime));
  fprintf(log_fp, "**** Basic Info *********************************************\n");
  fprintf(log_fp, "  Version:       %s\n",    VERSION_TEXT); 
  fprintf(log_fp, "  Chairs:        %5d\n",   p_tablemap->nchairs());
  fprintf(log_fp, "  Userchair:     %5d\n",   userchair);
  fprintf(log_fp, "  Holecards:     %s\n",    pcards.GetString());
  fprintf(log_fp, "  Community:     %s\n",    comcards.GetString());
  fprintf(log_fp, "  Handrank:      %s\n",    rank.GetString());
  fprintf(log_fp, "  Hand:          %s\n",    pokerhand.GetString());
  fprintf(log_fp, "  My balance:    %9.2f\n", p_symbol_engine_chip_amounts->balance(userchair));
  fprintf(log_fp, "  My currentbet: %9.2f\n", p_symbol_engine_chip_amounts->currentbet(userchair)); 
  fprintf(log_fp, "  To call:       %9.2f\n", p_symbol_engine_chip_amounts->call());
  fprintf(log_fp, "  Pot:           %9.2f\n", p_symbol_engine_chip_amounts->pot());
  fprintf(log_fp, "  Big blind:     %9.2f\n", p_symbol_engine_tablelimits->bblind());
  fprintf(log_fp, "  Big bet (FL):  %9.2f\n", p_symbol_engine_tablelimits->bigbet());
  fprintf(log_fp, "  f$betsize:     %9.2f\n", p_autoplayer_functions->f$betsize());
  fprintf(log_fp, "  Formulas:      %s\n",    fcra_formula_status.GetString());
  fprintf(log_fp, "  Buttons:       %s\n",    fcra_seen.GetString());
  fprintf(log_fp, "  Best action:   %s\n",    bestaction.GetString());
  fprintf(log_fp, "  Action taken:  %s\n",    action_taken);
}

void CAutoplayerTrace::LogAutoPlayerTrace()
{
  if (!preferences.trace_enabled() 
      || (_symboltrace_collection.GetSize() <= 0)) {
    return;
  }
  write_log_nostamp(true, "***** Autoplayer Trace **************************************\n");
  for (int i=0; i<_symboltrace_collection.GetSize(); i++)
  {
	  write_log_nostamp(true, "%s\n", _symboltrace_collection.GetAt(i));
  }
}

CString CAutoplayerTrace::LogSymbolsForGUI() {
  CString temp, result;
  for (int i=0; i<min(5, _logsymbols_collection.GetCount()); i++) {
	temp.Format("  Log: %s\n", _logsymbols_collection.GetAt(i));
	result.Append(temp);
  }
  return result;
}

