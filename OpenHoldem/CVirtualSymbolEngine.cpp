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

bool CVirtualSymbolEngine::EvaluateSymbol(const char *name, double *result)
{
	// We don't provide any symbols
	return false;
}

