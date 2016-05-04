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
#include "CVirtualSymbolEngine.h"

#include "CFormulaParser.h"
#include "CScraperAccess.h"
#include "OH_MessageBox.h"

CVirtualSymbolEngine::CVirtualSymbolEngine()
{}

CVirtualSymbolEngine::~CVirtualSymbolEngine()
{}

void CVirtualSymbolEngine::ResetOnConnection()
{}

void CVirtualSymbolEngine::ResetOnHandreset()
{}

void CVirtualSymbolEngine::ResetOnNewRound()
{}

void CVirtualSymbolEngine::ResetOnMyTurn()
{}

void CVirtualSymbolEngine::ResetOnHeartbeat()
{}

bool CVirtualSymbolEngine::EvaluateSymbol(const char *name, double *result, bool log /* = false */)
{
	// We don't provide any symbols
	return false;
}

CString CVirtualSymbolEngine::SymbolsProvided() {
  // Default for symbol-engines that don't provide any symbols
  return "";
}

void CVirtualSymbolEngine::WarnIfSymbolRequiresMyTurn(CString name) {
  if (p_formula_parser->IsParsing()) {
    // No error-message while parsing,
    // as we only verify existence, 
    // but don't care about the result.
    return;
  }
  if (!p_scraper_access->IsMyTurn()) {
    CString error_message;
    error_message.Format("%s%s%s%s%s%s%s%s",
      "The symbol \"", name, "\"\n",
      "requires my turn and is currently undefined.\n",
      "\n",
      "(This error might also be caused by derived OpenPPL-symbols\n",
      "like Raises, Calls, CallsSinceLastRaise, ...\n",
      "which use basic OpenHoldem-symbols.)");
    OH_MessageBox_Error_Warning(error_message, "Warning");
  }
}

CString CVirtualSymbolEngine::RangeOfSymbols(CString format_string, int first, int last) {
  CString result;
  CString next_symbol;
  assert(last >= first);
  for (int i=first; i<=last; ++i) {
    next_symbol.Format(format_string, i);
    result += next_symbol;
    result += " ";
  }
  return result;
}

