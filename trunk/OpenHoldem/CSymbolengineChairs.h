//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Various chair symbols, ecept dealerchair, userchair and raischair,
//   which are of special importance and get handled by other symbol-engines.
//
//******************************************************************************

#ifndef INC_CSYMBOLENGINECHAIRS_H
#define INC_CSYMBOLENGINECHAIRS_H

#include "CVirtualSymbolEngine.h"

class CSymbolEngineChairs: public CVirtualSymbolEngine {
 public:
	CSymbolEngineChairs();
	~CSymbolEngineChairs();
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
 private:
  void CalculateOpponentHeadsupChair();
  void CalculateSmallBlindChair();
  void CalculateBigBlindChair();
  void CalculateCutOffChair();
  void CalculateFirstCallerChair();
  void CalculateLastCallerChair();
  void CalculateFirstRaiserChair();
 private:
  int GetChairByDealposition(int dealposition);
 private:
  int _opponent_headsup_chair;
  int _smallblind_chair;
  int _bigblind_chair;
  int _cutoff_chair;
  int _firstcaller_chair;
  int _lastcaller_chair;
  int _firstraiser_chair;
};

extern CSymbolEngineChairs *p_symbol_engine_chairs;

#endif INC_CSYMBOLENGINEUSERCHAIR_H