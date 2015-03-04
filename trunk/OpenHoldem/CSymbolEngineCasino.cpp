//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Symbol lookup for various symbols 
//   that are not part of a regular symbol-engine
//
//******************************************************************************

#include "stdafx.h"
#include "CSymbolEngineCasino.h"

#include <assert.h>
#include <process.h>
#include <float.h>

#include "CAutoconnector.h"
#include "CAutoplayerTrace.h"
#include "inlines/eval.h"
#include "Chair$Symbols.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CSessionCounter.h"
#include "CStringMatch.h"
#include "..\CTablemap\CTablemap.h"
#include "..\CTransform\CTransform.h"
#include "MagicNumbers.h"
#include "OpenHoldem.h"
#include "OH_MessageBox.h"

CSymbolEngineCasino	*p_symbol_engine_casino = NULL;

CSymbolEngineCasino::CSymbolEngineCasino() {
  // The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	//
	// This engine does not use any other engines.
}

CSymbolEngineCasino::~CSymbolEngineCasino() {
}

void CSymbolEngineCasino::InitOnStartup() {
}

void CSymbolEngineCasino::ResetOnConnection() {
}

void CSymbolEngineCasino::ResetOnHandreset() {
  // Reset display
  InvalidateRect(theApp.m_pMainWnd->GetSafeHwnd(), NULL, true);
}

void CSymbolEngineCasino::ResetOnNewRound() {
}

void CSymbolEngineCasino::ResetOnMyTurn() {
}

bool CSymbolEngineCasino::ConnectedToManualMode() {
  const int k_max_length_of_classname = 50;
	char classname[k_max_length_of_classname] = "";
  GetClassName(p_autoconnector->attached_hwnd(), classname, k_max_length_of_classname);
  return (strcmp(classname, "OpenHoldemManualMode") == 0);
}

bool CSymbolEngineCasino::ConnectedToOHReplay() {
  const int k_max_length_of_classname = 50;
	char classname[k_max_length_of_classname] = "";
  GetClassName(p_autoconnector->attached_hwnd(), classname, k_max_length_of_classname);
  return (strcmp(classname, "OHREPLAY") == 0);
}

bool CSymbolEngineCasino::ConnectedToOfflineSimulation() {
  return (ConnectedToManualMode()
    || ConnectedToOHReplay()
    || ConnectedToDDPoker()
    || SitenameContainsCasinoIdentifier("pokeracademy")
    || SitenameContainsCasinoIdentifier("pokerth")
    || SitenameContainsCasinoIdentifier("pokersnowie"));
}

bool CSymbolEngineCasino::ConnectedToRealCasino() {
  return (!ConnectedToOfflineSimulation());
}

bool CSymbolEngineCasino::ConnectedToBring() {
  const int k_max_length_of_classname = 50;
	char classname[k_max_length_of_classname] = "";
  GetClassName(p_autoconnector->attached_hwnd(), classname, k_max_length_of_classname);
  return (strcmp(classname, "BRING") == 0);
}

bool CSymbolEngineCasino::ConnectedToDDPoker() {
  return SitenameContainsCasinoIdentifier("ddpoker");
}

bool CSymbolEngineCasino::SitenameContainsCasinoIdentifier(const char *casino) {
  CString sitename = p_tablemap->sitename();
  sitename.MakeLower();
  return (sitename.Find(casino) >= 0);
}

bool CSymbolEngineCasino::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
  FAST_EXIT_ON_OPENPPL_SYMBOLS(name);
  // CHAIRS 1(2)
  if (memcmp(name, "chair", 5)==0) {
    if (memcmp(name, "chair$", 6)==0)							*result = Chair$(&name[6]);
    else if (memcmp(name, "chairbit$", 9)==0)					*result = Chairbit$(&name[9]);
    else return false;
  }
  // GENERAL
  else if (memcmp(name, "nchairs", 7)==0 && strlen(name)==7)	*result = p_tablemap->nchairs();
  //PROFILE
  else if (memcmp(name, "sitename$", 9)==0)	*result = SitenameContainsCasinoIdentifier(&name[9]);
  else if (memcmp(name, "network$", 8)==0)	*result = p_tablemap->network().Find(&name[8])!=-1;
  else {
    *result = k_undefined;
    return false;
  }
  return true;
}

CString CSymbolEngineCasino::SymbolsProvided() {
  // This list includes some prefixes of symbols that can't be verified,
  // e.g. "dll$, pl_chair$, ....
  return "chair$ chairbit$ sitename$ network$ nchairs ";
}

void CSymbolEngineCasino::ResetOnHeartbeat()
{}

