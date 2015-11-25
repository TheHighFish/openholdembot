//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose:
//
//*******************************************************************************

#ifndef INC_CSYMBOLENGINECALLERS_H
#define INC_CSYMBOLENGINECALLERS_H

#include "CVirtualSymbolEngine.h"
#include "CBetroundCalculator.h"
#include "NumericalFunctions.h"

class CSymbolEngineCallers: public CVirtualSymbolEngine {
 public:
	CSymbolEngineCallers();
	~CSymbolEngineCallers();
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
	CString SymbolsProvided();;
 public:
	int callbits(int betround) {
		if ((betround >= kBetroundPreflop)
			  && (betround <= kBetroundRiver)) {
			return _callbits[betround];
		}	else		{
			return kUndefined;
		}
	}
 public:
	int nopponentscalling()   { return _nopponentscalling; }
  int lastcaller_chair()    { return _lastcaller_chair;  }
  int firstcaller_chair()   { return _firstcaller_chair; }
 private:
	void CalculateCallers();
 private:
  int FirstPossibleCaller();
  int LastPossibleCaller();
  double FirstPossibleRaisersBet();
 private:
	int _nopponentscalling;
  int _firstcaller_chair;
  int _lastcaller_chair;
 private:
	// Indices 1..4 are for the betrounds preflop..river.
	// Index 0 is unused.
	int _callbits[kNumberOfBetrounds + 1]; 
 private:
  int _nchairs;
};

extern CSymbolEngineCallers *p_symbol_engine_callers;

#endif INC_CSYMBOLENGINECALLERS_H