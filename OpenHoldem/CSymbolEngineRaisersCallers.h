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

#ifndef INC_CSYMBOLENGINERAISERSCALLERS_H
#define INC_CSYMBOLENGINERAISERSCALLERS_H

#include "CVirtualSymbolEngine.h"
#include "CBetroundCalculator.h"
#include "NumericalFunctions.h"

class CSymbolEngineRaisersCallers: public CVirtualSymbolEngine {
 public:
	CSymbolEngineRaisersCallers();
	~CSymbolEngineRaisersCallers();
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
	int raischair()	{ return _raischair; }
	int raisbits(int betround) {
		if ((betround >= k_betround_preflop)
			  && (betround <= k_betround_river)) {
			return _raisbits[betround];
		}	else {
			return k_undefined;
		}
	}
 public:
	int foldbits(int betround) {
		if ((betround >= k_betround_preflop)
			  && (betround <= k_betround_river)) {
			return _foldbits[betround];
		}	else {
			return k_undefined;
		}
	}
 public:
	int callbits(int betround) {
		if ((betround >= k_betround_preflop)
			  && (betround <= k_betround_river)) {
			return _callbits[betround];
		}	else		{
			return k_undefined;
		}
	}
 public:
	int nplayerscallshort()			{ return _nplayerscallshort; }
	int nopponentstruelyraising()	{ return _nopponentstruelyraising; }
	int nopponentsbetting()			{ return _nopponentsbetting; }
	int nopponentsfolded()			{ return _nopponentsfolded; }
	int nopponentscalling()			{ return _nopponentscalling; }
	int nopponentschecking()		{ return _nopponentschecking; }
 private:
	void CalculateRaisers();
	void CalculateCallers();
	void CalculateFoldBits();
	void CalculateNOpponentsCheckingBettingFolded();
 private:
	int FirstPossibleRaiser();
	int LastPossibleRaiser();
 private:
	double RaisersBet();
	double LastOrbitsLastRaisersBet();
 private:
	int _raischair;
	int _nplayerscallshort;
	int _nopponentsbetting;
	int _nopponentstruelyraising;
	int _nopponentsfolded;
	int _nopponentscalling;
	int _nopponentschecking;
 private:
	// Indices 1..4 are for the betrounds preflop..river.
	// Index 0 is unused.
	int _raisbits[k_number_of_betrounds + 1];
	int _foldbits[k_number_of_betrounds + 1];
	int _callbits[k_number_of_betrounds + 1]; 
};

extern CSymbolEngineRaisersCallers *p_symbol_engine_raisers_callers;

#endif INC_CSYMBOLENGINERAISERSCALLERS_H