//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose: Interface to the user.DLL
//
//*******************************************************************************

#include "stdafx.h"
#include "CSymbolEngineUserDLL.h"

#include "CEngineContainer.h"
#include "CFormulaParser.h"
#include "CFunctionCollection.h"
#include "CHandresetDetector.h"
#include "CIteratorThread.h"
#include "COcclusionCheck.h"
#include "CScraper.h"
#include "CSymbolengineVersus.h"
#include "..\Reference User DLL\user.h"

CSymbolEngineUserDLL *p_symbol_engine_formula_loading = NULL;

CSymbolEngineUserDLL::CSymbolEngineUserDLL() {
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	//
	// This engine does not use any other engines.
}

CSymbolEngineUserDLL::~CSymbolEngineUserDLL() {
}

void CSymbolEngineUserDLL::InitOnStartup() {
  DLLUpdateOnNewFormula();
}

void CSymbolEngineUserDLL::UpdateOnConnection() {
  DLLUpdateOnConnection();
}

void CSymbolEngineUserDLL::UpdateOnHandreset() {
  DLLUpdateOnHandreset();
}

void CSymbolEngineUserDLL::UpdateOnNewRound() {
  DLLUpdateOnNewRound();
}

void CSymbolEngineUserDLL::UpdateOnMyTurn() {
  DLLUpdateOnMyTurn();
}

void CSymbolEngineUserDLL::UpdateOnHeartbeat() {
  DLLUpdateOnHeartbeat();
}

bool CSymbolEngineUserDLL::EvaluateSymbol(const CString name, double *result, bool log /* = false */) {
  FAST_EXIT_ON_OPENPPL_SYMBOLS(name);
  if (memcmp(name, "dll$", 4) != 0) {
    // Symbol of a different symbol-engine
    return false;
  }
  *result = ProcessQuery(name);
	return true;
}

CString CSymbolEngineUserDLL::SymbolsProvided() {
  return " ";
}

//*******************************************************************************
//
// Exported functions, needed by the DLL
//
//*******************************************************************************

double __stdcall GetSymbol(const char* name_of_single_symbol__not_expression) {
  CString	str = "";
  str.Format("%s", name_of_single_symbol__not_expression);
  if (strcmp(str, "cmd$recalc") == 0) {
    // restart iterator thread
    p_iterator_thread->RestartPrWinComputations();
    // Recompute versus tables
    p_engine_container->symbol_engine_versus()->GetCounts();
    // Busy waiting until recalculation got finished.
    // Nothing better to do, as we already evaluate bot-logic,
    // so we can't continue with another heartbeat or something else.
    // http://www.maxinmontreal.com/forums/viewtopic.php?f=111&t=19012
    while (!p_iterator_thread->IteratorThreadComplete()) {
      Sleep(100);
    }
    return 0;
  }
  double result = kUndefined;
  p_engine_container->EvaluateSymbol(name_of_single_symbol__not_expression,
    &result,
    kAlwaysLogDLLMessages);
  return result;
}

void* __stdcall GetPrw1326() {
  assert(p_iterator_thread != NULL);
  return (void *)(p_iterator_thread->prw1326());
}

char* __stdcall GetHandnumber() {
  assert(p_handreset_detector->GetHandNumber().GetLength() < k_max_length_of_handnumber);
  static char handnumber_as_char_array[k_max_length_of_handnumber];
  strcpy_s(handnumber_as_char_array,
    k_max_length_of_handnumber,
    p_handreset_detector->GetHandNumber());
  return handnumber_as_char_array;
}

void __stdcall ParseHandList(const char* name_of_list, const char* list_body) {
  COHScriptList* p_new_list = new COHScriptList(name_of_list, list_body);
  p_formula_parser->ParseFormula(p_new_list);
  p_function_collection->Add(p_new_list);
}

char* __stdcall ScrapeTableMapRegion(char* p_region, int& p_returned_lengh) {
  CString result;
  bool success = p_scraper->EvaluateRegion(p_region, &result);
  if (success) {
    p_returned_lengh = result.GetLength() + 1;
    char* returnStr = static_cast<char*>(LocalAlloc(LPTR, p_returned_lengh));
    strcat(returnStr, result);
    return returnStr;
  }
  p_returned_lengh = kUndefined;
  return nullptr;
}

void __stdcall SendChatMessage(const char *msg) {
  SendChatMessage((char *)msg);
}

void __stdcall WriteLog(char* fmt, ...) {
  // Docu about ellipsis and variadic macro:
  // http://msdn.microsoft.com/en-us/library/ms177415(v=vs.80).aspx
  // http://stackoverflow.com/questions/1327854/how-to-convert-a-variable-argument-function-into-a-macro
  va_list args;
  va_start(args, fmt);
  write_log_vl(kAlwaysLogDLLMessages, fmt, args);
  va_end(args);
}



