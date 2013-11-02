//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*****************************************************************************
//
// Purpose: Detecting if we play a holdem or omaha,
//   needed e.g. for automatic adaption of PT-queries. 
//
//*****************************************************************************

#include "stdafx.h"
#include "CSymbolEngineIsOmaha.h"

#include <assert.h>
#include "CGameState.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "..\CTablemap\CTablemap.h"
#include "MagicNumbers.h"
#include "StringFunctions.h"

CSymbolEngineIsOmaha *p_symbol_engine_isomaha = NULL;

CSymbolEngineIsOmaha::CSymbolEngineIsOmaha()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
}

CSymbolEngineIsOmaha::~CSymbolEngineIsOmaha()
{}

void CSymbolEngineIsOmaha::InitOnStartup()
{
	ResetOnConnection();
}

void CSymbolEngineIsOmaha::ResetOnConnection()
{}

void CSymbolEngineIsOmaha::ResetOnHandreset()
{}

void CSymbolEngineIsOmaha::ResetOnNewRound()
{}

void CSymbolEngineIsOmaha::ResetOnMyTurn()
{}

void CSymbolEngineIsOmaha::ResetOnHeartbeat()
{}

