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

#ifndef INC_CSYMBOLENGINEAUTOPLAYER_H
#define INC_CSYMBOLENGINEAUTOPLAYER_H

#include "CVirtualSymbolEngine.h"
#include "assert.h"
#include "CSymbolEngineTableLimits.h"
#include "MagicNumbers.h"

class CSymbolEngineAutoplayer: public CVirtualSymbolEngine
{
public:
	CSymbolEngineAutoplayer();
	~CSymbolEngineAutoplayer();
public:
	// Mandatory reset-functions
	void InitOnStartup();
	void ResetOnConnection();
	void ResetOnHandreset();
	void ResetOnNewRound();
	void ResetOnMyTurn();
	void ResetOnHeartbeat();
public:
	// Public accessors
	bool EvaluateSymbol(const char *name, double *result, bool log = false);
  CString SymbolsProvided();
public:
	int  myturnbits()		{ return _myturnbits; }
	bool ismyturn()			{ return ((_myturnbits & k_my_turn_bits_fold_call_raise) != 0); }
	bool issittingin()		{ return _issittingin; }
	bool issittingout()		{ return !issittingin(); }
	bool isautopost()		{ return _isautopost; }
	bool isfinalanswer()	{ return _isfinalanswer; }
	bool isfinaltable()		{ return _isfinaltable; }
	bool isbring()			{ return _isbring; }
	bool ismanual()			{ return _ismanual; }
public:
	// Especially needed to start the PrWin-calculations
	bool IsFirstHeartbeatOfMyTurn();
	// Visible buttons
	CString GetFCKRAString();
private:
	void CalculateMyTurnBits();
	void CalculateSitInState();
	void CalculateFinalAnswer();
	void DetectSpecialConnectionLikeBringAndManualMode();
private:
	int  _myturnbits;
	bool _issittingin;
	bool _isautopost;
	bool _isfinalanswer; 
	bool _isfinaltable;  
	bool _isbring;
	bool _ismanual;
private:
	bool _last_myturnbits;
};

extern CSymbolEngineAutoplayer *p_symbol_engine_autoplayer;

#endif INC_CSYMBOLENGINEAUTOPLAYER_H