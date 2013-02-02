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


