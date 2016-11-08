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

#ifndef INC_CSYMBOLENGINERAISERS_H
#define INC_CSYMBOLENGINERAISERS_H

#include "CVirtualSymbolEngine.h"
#include "CBetroundCalculator.h"
#include "NumericalFunctions.h"

class CSymbolEngineRaisers: public CVirtualSymbolEngine {
 friend class CSymbolEngineCallers;
 public:
	CSymbolEngineRaisers();
	~CSymbolEngineRaisers();
 public:
	// Mandatory reset-functions
	void InitOnStartup();
	void UpdateOnConnection();
	void UpdateOnHandreset();
	void UpdateOnNewRound();
	void UpdateOnMyTurn();
	void UpdateOnHeartbeat();
 public:
	// Public accessors
	bool EvaluateSymbol(const char *name, double *result, bool log = false);
	CString SymbolsProvided();
 public:
	int raischair()	{ return _raischair; }
	int firstraiser_chair()      { return _firstraiser_chair; }
	int raisbits(int betround) {
		if ((betround >= kBetroundPreflop)
			  && (betround <= kBetroundRiver)) {
			return _raisbits[betround];
		}	else {
			return kUndefined;
		}
	}
 public:
	int nopponentstruelyraising()	{ return _nopponentstruelyraising; }
 protected:
  // Also used by CSymbolEngineCallers
  // Return values are NOT normalized to 0..(nchairs-1)
  // because this is more convenient 
  // for the exit-condition of the for-loop.
  // that searches for raisers (callers).
  // Normalization happens then inside the loop
  int ChairInFrontOfFirstPossibleActor();
  int FirstPossibleActor();
  int LastPossibleActor();	
 private:
	void CalculateRaisers();
 private:
	double RaisersBet();
	double MinimumStartingBetCurrentOrbit();
 private:
  int LastRaised(const int round);
 private:
	int _raischair;
	int _firstraiser_chair;
	int _nopponentstruelyraising;
 private:
	// Indices 1..4 are for the betrounds preflop..river.
	// Index 0 is unused.
	int _raisbits[kNumberOfBetrounds + 1];
  int _lastraised[kNumberOfBetrounds + 1]; 
};

extern CSymbolEngineRaisers *p_symbol_engine_raisers;

#endif INC_CSYMBOLENGINERAISERS_H
