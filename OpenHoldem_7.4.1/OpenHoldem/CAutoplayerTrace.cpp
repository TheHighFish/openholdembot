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
#include "CAutoplayerTrace.h"

#include "CAutoplayerFunctions.h"
#include "CBetroundCalculator.h"
#include "CFunctionCollection.h"
#include "COHScriptObject.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineHandrank.h"
#include "CSymbolEnginePokerval.h"
#include "CSymbolEnginePrwin.h"
#include "CSymbolEngineUserchair.h"
#include "CTableState.h"

#define ENT CSLock lock(m_critsec);

CAutoplayerTrace *p_autoplayer_trace = NULL;

CAutoplayerTrace::CAutoplayerTrace() {
  Clear();
}

CAutoplayerTrace::~CAutoplayerTrace() {
  Clear();
}

void CAutoplayerTrace::Clear() {
  ENT 
  _indentation = 0;
  _number_of_log_lines = 0;
  _symboltrace_collection.RemoveAll();
  _already_logged_symbols.clear();
}

bool CAutoplayerTrace::SymbolNeedsToBeLogged(CString name) {
  // DLL and Perl, as there is no caching and value might change
  if (memcmp(name, "dll$", 4) == 0) return true;
  if (memcmp(name, "pl_", 3) == 0) return true;
  // Memory-store and recall-commands
  if (memcmp(name, "me_", 3) == 0) return true;
  // OpenPPL-user-variables might also change (once)
  // We don't care about multiple loggings of userchair here
  if (_memicmp(name, "user", 4) == 0) return true;
  // True random numbers that don't get cached,
  // i.e. OH-script "random" and OpenPPL "Random"
  if (memcmp(name, "random", 6) == 0) return true;
  if (memcmp(name, "Random", 6) == 0) return true;
  // Values that already got logged can be ignored
  if (_already_logged_symbols[name] == true) return false;
  // Everything else needs to be logged
  return true;
}

int CAutoplayerTrace::Add(CString symbol) {
  ENT
  write_log(preferences.debug_auto_trace(),
    "[CAutoplayerTrace] Add (%s, ...)\n", symbol);
  // This function for symbols without value is for functions only.
  // These functions are eitherpredefined (f$), userdefined (f$)
  // or OpenPPL (upper-cases).
  // The value will be backpatched later.
  assert(COHScriptObject::IsFunction(symbol)
    || COHScriptObject::IsOpenPPLSymbol(symbol));
  CString new_message;
  new_message.Format("%s%s = ",
    Indentation(), symbol);
  _symboltrace_collection.Add(new_message);
  // Nothing to do for _already_logged_symbols here,
  // as this Add()-function is for the first-time-evaluation
  // of f$functions.
  _number_of_log_lines++;
  return (_number_of_log_lines - 1); 
}

void CAutoplayerTrace::Add(CString symbol, double value) {
  ENT
  write_log(preferences.debug_auto_trace(),
    "[CAutoplayerTrace] Add (%s, %.3f)\n",
    symbol, value);
  if (!SymbolNeedsToBeLogged(symbol)) return;
  CString new_message;
  if (COHScriptObject::IsFunction(symbol)
      || COHScriptObject::IsOpenPPLSymbol(symbol)) {
    // Function with known value a priori
    new_message.Format("%s%s = %.3f [cached]",
      Indentation(), symbol, value);
  } else {
    // "Normal" symbol
    new_message.Format("%s%s = %.3f",
      Indentation(), symbol, value); 
  }
  _symboltrace_collection.Add(new_message);
  _already_logged_symbols[symbol] = true;
  _number_of_log_lines++;
}

void CAutoplayerTrace::BackPatchValueAndLine(
    int index, double value, int starting_line_of_function) {
  assert(index >= 0);
  assert(index < _number_of_log_lines);
  // Starting line should be > 0, but auto-generated missing 
  // vital functions like f$check can have line == 0.
  assert(starting_line_of_function >= 0);
  int executed_absolute_line = starting_line_of_function 
    + _last_evaluated_relative_line_number;
  // Already done:
  // Indentation, symbol, " = "
  CString complete_message;
  complete_message.Format("%s%.3f [Line %d/%d]", 
    _symboltrace_collection.GetAt(index),
    value,
    _last_evaluated_relative_line_number,
    executed_absolute_line);
  _symboltrace_collection.SetAt(index, complete_message);
}

void CAutoplayerTrace::Indent(bool more) {
  if (more) {
    _indentation += 2;
  } else {
    _indentation -= 2;
  }
  if (_indentation < 0) {
    // This could happen if we calculate and log prwin functions
    // and the heartbeat-threat resets the autoplayer-trace inbetween.
    // Most easy way to fix it: continue gracefully
    _indentation = 0;
  }
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
  CSLock lock(log_critsec);
  LogBasicInfo(action_taken);
  LogAutoPlayerTrace();
  Clear();
}

void CAutoplayerTrace::LogLogSymbols() {
  int max_log = _logsymbols_collection.GetCount();
  if (max_log <= 0) {
    return;
  }
  if (max_log > kMaxLogSymbolsForWhiteBox) {
    max_log = kMaxLogSymbolsForWhiteBox;
  }
  write_log(k_always_log_basic_information, "*** log$ (Total: %d | Showing: %d)\n", 
    _logsymbols_collection.GetCount(), max_log);
  for (int i=0; i<max_log; i++) {
    write_log(k_always_log_basic_information, "***     %s\n", 
      _logsymbols_collection.GetAt(i));
  }
}

CString CAutoplayerTrace::BestAction() {
  for (int i=k_autoplayer_function_allin; i<=k_autoplayer_function_fold; ++i) {
    if (p_autoplayer_functions->GetAutoplayerFunctionValue(i)) {
      if (i == k_autoplayer_function_betsize) {
        // Special treatment for f$betsize
        // e.g. "f$betsize = 201.47"
        CString best_action;
        best_action.Format("%s = %.2f", k_standard_function_names[i],
          p_function_collection->EvaluateAutoplayerFunction(k_autoplayer_function_betsize));
        return best_action;
      }
      else {
        return k_standard_function_names[i];
      }
    }
  }
  // No action can happen if it is not our turn (best action in GUI)
  return "no action";
}

void CAutoplayerTrace::LogBasicInfo(const char *action_taken) {
  char		nowtime[26];
  CString	pcards, comcards, temp, rank, pokerhand;
  CString	fcra_formula_status;
  int		userchair = p_symbol_engine_userchair->userchair();
  int		betround  = p_betround_calculator->betround();

  // player cards
  if (p_symbol_engine_userchair->userchair_confirmed()) {
    for (int i=0; i<=1; i++) {
      Card card = p_table_state->User()->_hole_cards[i];
      pcards.Append(card.ToString());
    }
  } else {
	pcards = "....";
  }
  // common cards
  comcards = "";
  if (betround >= k_betround_flop) {
    for (int i=0; i<k_number_of_flop_cards; i++) {
      if (p_table_state->_common_cards[i].IsKnownCard()) {
        comcards.Append(p_table_state->_common_cards[i].ToString());
      }
    }
  }
  if (betround >= k_betround_turn) {
    comcards.Append(p_table_state->_common_cards[3].ToString());
  }
  if (betround >= k_betround_river) {
    comcards.Append(p_table_state->_common_cards[4].ToString());
  }
  comcards.Append("..........");
  comcards = comcards.Left(10);
  // Always use handrank169 here
  rank.Format("%.0f", p_symbol_engine_handrank->handrank169());
  // poker hand
  pokerhand = p_symbol_engine_pokerval->HandType();
  // fcra_seen
  CString fcra_seen = p_symbol_engine_autoplayer->GetFCKRAString();
  // fcra formula status
  fcra_formula_status.Format("%s%s%s%s%s",
	p_function_collection->EvaluateAutoplayerFunction(k_autoplayer_function_fold)  ? "F" : ".",
	p_function_collection->EvaluateAutoplayerFunction(k_autoplayer_function_call)  ? "C" : ".",
	p_function_collection->EvaluateAutoplayerFunction(k_autoplayer_function_check) ? "K" : ".",
	p_function_collection->EvaluateAutoplayerFunction(k_autoplayer_function_raise) ? "R" : ".",
	p_function_collection->EvaluateAutoplayerFunction(k_autoplayer_function_allin) ? "A" : ".");
  // More verbose summary in the log
  // The old WinHoldem format was a complete mess
  write_log_separator(true, "Basic Info");
  write_log(k_always_log_basic_information, "  Version:       %s\n",    VERSION_TEXT); 
  write_log(k_always_log_basic_information, "  Chairs:        %5d\n",   p_tablemap->nchairs());
  write_log(k_always_log_basic_information, "  Userchair:     %5d\n",   userchair);
  write_log(k_always_log_basic_information, "  Holecards:     %s\n",    pcards.GetString());
  write_log(k_always_log_basic_information, "  Community:     %s\n",    comcards.GetString());
  write_log(k_always_log_basic_information, "  Handrank:      %s\n",    rank.GetString());
  write_log(k_always_log_basic_information, "  Hand:          %s\n",    pokerhand.GetString());
  write_log(k_always_log_basic_information, "  My balance:    %9.2f\n", p_table_state->User()->_balance);
  write_log(k_always_log_basic_information, "  My currentbet: %9.2f\n", p_symbol_engine_chip_amounts->currentbet(userchair)); 
  write_log(k_always_log_basic_information, "  To call:       %9.2f\n", p_symbol_engine_chip_amounts->call());
  write_log(k_always_log_basic_information, "  Pot:           %9.2f\n", p_symbol_engine_chip_amounts->pot());
  write_log(k_always_log_basic_information, "  Big blind:     %9.2f\n", p_symbol_engine_tablelimits->bblind());
  write_log(k_always_log_basic_information, "  Big bet (FL):  %9.2f\n", p_symbol_engine_tablelimits->bigbet());
  write_log(k_always_log_basic_information, "  f$betsize:     %9.2f\n", p_function_collection->EvaluateAutoplayerFunction(k_autoplayer_function_betsize));
  write_log(k_always_log_basic_information, "  Formulas:      %s\n",    fcra_formula_status.GetString());
  write_log(k_always_log_basic_information, "  Buttons:       %s\n",    fcra_seen.GetString());
  write_log(k_always_log_basic_information, "  Best action:   %s\n",    BestAction().GetString());
  write_log(k_always_log_basic_information, "  Action taken:  %s\n",    action_taken);
  write_log_separator(true, "");
}

void CAutoplayerTrace::LogAutoPlayerTrace() {
  if (!preferences.trace_enabled() 
      || (_symboltrace_collection.GetSize() <= 0)) {
    return;
  }
  write_log_separator(true, "Autoplayer Trace");
  for (int i=0; i<_symboltrace_collection.GetSize(); ++i) {
	  write_log_nostamp(true, "%s\n", _symboltrace_collection.GetAt(i));
  }
  write_log_separator(true, "");
}

CString CAutoplayerTrace::LogSymbolsForGUI() {
  CString temp, result;
  for (int i=0; i<min(5, _logsymbols_collection.GetCount()); i++) {
	  temp.Format("  Log: %s\n", _logsymbols_collection.GetAt(i));
	  result.Append(temp);
  }
  return result;
}

