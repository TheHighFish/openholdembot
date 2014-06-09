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
#include "CSymbolEngineRandom.h"

#include "MagicNumbers.h"
#include "NumericalFunctions.h"

CSymbolEngineRandom *p_symbol_engine_random = NULL;

CSymbolEngineRandom::CSymbolEngineRandom()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	//
	// This engine does not use any other engines.
}

CSymbolEngineRandom::~CSymbolEngineRandom()
{}

void CSymbolEngineRandom::InitOnStartup()
{
	ResetOnConnection();
}

void CSymbolEngineRandom::ResetOnConnection()
{
	ResetOnHandreset();
}

void CSymbolEngineRandom::ResetOnHandreset()
{
	_randomhand      = Random_0_1();
	_randomheartbeat = k_undefined;

	ResetOnNewRound();
}

void CSymbolEngineRandom::ResetOnNewRound()
{
	_randomround = Random_0_1();
}

void CSymbolEngineRandom::ResetOnMyTurn()
{}

void CSymbolEngineRandom::ResetOnHeartbeat()
{
	_randomheartbeat = Random_0_1();
}

double CSymbolEngineRandom::Random_0_1()
{
	double result = ((double) rand() / (double) RAND_MAX);
	AssertRange(result, 0, 1);
	return result;
}

bool CSymbolEngineRandom::EvaluateSymbol(const char *name, double *result, bool log /* = false */)
{
	if (memcmp(name, "random", 6) == 0)
	{
		if (memcmp(name, "randomheartbeat", 15)==0 && strlen(name)==15)
		{
			*result = randomheartbeat();
		}
		else if (memcmp(name, "randomhand", 10)==0 && strlen(name)==10)
		{
			*result = randomhand();
		}
		else if (memcmp(name, "randomround", 11)==0 && strlen(name)==11)
		{
			*result = randomround();
		}
		else if (memcmp(name, "random", 6) == 0 && strlen(name)==6)
		{
			*result = random();
		}
		else
		{
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}
	// Symbol of a different symbol-engine
	return false;
}

CString CSymbolEngineRandom::SymbolsProvided() {
  //!!
  return " todo";
}