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
#include "CVirtualSymbolEngine.h"

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

