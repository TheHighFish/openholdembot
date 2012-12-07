#include "stdafx.h"
#include "CEngineContainer.h"

#include <assert.h>
#include "CBetroundCalculator.h"
#include "CHandresetDetector.h"


CEngineContainer::CEngineContainer()
{
	CreateSymbolEngines();
	// First initialization is the same as on a new connection
	ResetOnConnection();
}

CEngineContainer::~CEngineContainer()
{
	DestroyAllSymbolEngines();
}

void CEngineContainer::CreateSymbolEngines()
{
	_number_of_symbol_engines_loaded = 0;
	// Be careful about the order of engines.
	// Some symbols to be calculated depend on symbols of other engines.
	// The engines inserted first will be called first later.
	
	assert(_number_of_symbol_engines_loaded < k_max_number_of_symbol_engines);
}

void CEngineContainer::DestroyAllSymbolEngines()
{
	for (int i=0; i<_number_of_symbol_engines_loaded; i++)
	{
		_symbol_engines[i].~CVirtualSymbolEngine();
	}
	_number_of_symbol_engines_loaded = 0;
}

void CEngineContainer::CallSymbolEnginesToUpdateSymbolsIfNecessary()
{
	// ResetOnConnection() gets directly called by the auto-connector,
	// so we don't have to care about that.
	// We only need to care about:
	// * ResetOnHandreset()
	// * ResetOnNewRound()
	// * ResetOnMyTurn()
	if (p_handreset_detector->IsHandreset())
	{
		ResetOnHandreset();
	}
	if (p_betround_calculator->IsNewBetround())
	{
		ResetOnNewRound();
	}
	if (1) //!!!
	{
		ResetOnMyTurn();
	}
	// And finally ResetOnHeartbeat() gets always called.
	ResetOnHeartbeat();
}

void CEngineContainer::ResetOnConnection()
{
	for (int i=0; i<_number_of_symbol_engines_loaded; i++)
	{
		_symbol_engines[i].ResetOnConnection();
	}
}

void CEngineContainer::ResetOnHandreset()
{
	for (int i=0; i<_number_of_symbol_engines_loaded; i++)
	{
		_symbol_engines[i].ResetOnHandreset();
	}
}

void CEngineContainer::ResetOnNewRound()
{
	for (int i=0; i<_number_of_symbol_engines_loaded; i++)
	{
		_symbol_engines[i].ResetOnNewRound();
	}
}

void CEngineContainer::ResetOnMyTurn()
{
	for (int i=0; i<_number_of_symbol_engines_loaded; i++)
	{
		_symbol_engines[i].ResetOnMyTurn();
	}
}

void CEngineContainer::ResetOnHeartbeat()
{
	for (int i=0; i<_number_of_symbol_engines_loaded; i++)
	{
		_symbol_engines[i].ResetOnHeartbeat();
	}
}

