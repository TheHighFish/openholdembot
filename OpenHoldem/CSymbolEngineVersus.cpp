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
#include "CSymbolEngineVersus.h"

CSymbolEngineVersus::CSymbolEngineVersus() {
}

CSymbolEngineVersus::~CSymbolEngineVersus() {
}

void CSymbolEngineVersus::InitOnStartup() {

}

void CSymbolEngineVersus::ResetOnConnection() {

}

void CSymbolEngineVersus::ResetOnHandreset() {

}

void CSymbolEngineVersus::ResetOnNewRound() {

}

void CSymbolEngineVersus::ResetOnMyTurn() {

}

void CSymbolEngineVersus::ResetOnHeartbeat() {

}

bool CSymbolEngineVersus::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
  return true; //!!
}

CString CSymbolEngineVersus::SymbolsProvided() {
  return ""; //!!
}