//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*****************************************************************************
//
// Purpose: Providing functions (f$init_once_per_hand, etc.)
//   to deal with memory-symbols and other initializations (DLL maybe)
//   and get rid of the old advice "put it into f$alli".
//
// Not really a symbol-engine, but easy to implement with this concept.
//
//*****************************************************************************

#ifndef INC_CSYMBOLENGINEINIFUNCTIONS_H
#define INC_CSYMBOLENGINEINIFUNCTIONS_H

#include "CVirtualSymbolEngine.h"

#include "CGrammar.h"

enum 
{
	k_init_on_startup,
	k_init_on_connection,
	k_init_on_handreset,
	k_init_on_new_round,
	k_init_on_my_turn,
	k_init_on_heartbeat,
	// (last one + 1) = number of ini-functions
	k_number_of_ini_functions
};

const CString k_ini_function_names[k_number_of_ini_functions] =
{
	"f$init_on_startup",
	"f$init_on_connection",
	"f$init_on_handreset",
	"f$init_on_new_round",
	"f$init_on_my_turn",
	"f$init_on_heartbeat",
};

extern class CSymbolEngineIniFunctions: public CVirtualSymbolEngine
{
public:
	CSymbolEngineIniFunctions();
	~CSymbolEngineIniFunctions();
public:
	// Mandatory reset-functions
	void InitOnStartup();
	void ResetOnConnection();
	void ResetOnHandreset();
	void ResetOnNewRound();
	void ResetOnMyTurn();
	void ResetOnHeartbeat();

private:
	CGrammar gram;
	int dummy_e;
} *p_symbol_engine_ini_functions;

#endif INC_CSYMBOLENGINEINIFUNCTIONS_H
