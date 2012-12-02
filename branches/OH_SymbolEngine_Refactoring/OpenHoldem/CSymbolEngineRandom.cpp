#include "stdafx.h"
#include "CSymbolEngineRandom.h"

#include "MagicNumbers.h"

CSymbolEngineRandom::CSymbolEngineRandom()
{}

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
	_randomround     = k_undefined;
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
	return ((double) rand() / (double) RAND_MAX);
}


