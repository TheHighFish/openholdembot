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

#ifndef INC_CSymbolEngineMTTInfo_H
#define INC_CSymbolEngineMTTInfo_H

#include "CVirtualSymbolEngine.h"


class CSymbolEngineMTTInfo: public CVirtualSymbolEngine {
 public:
	CSymbolEngineMTTInfo();
	~CSymbolEngineMTTInfo();
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
	void set_mtt_number_entrants(const int i)   { _mtt_number_entrants = i;}
	void set_mtt_players_remaining(const int i) { _mtt_players_remaining = i;}
	void set_mtt_my_rank(const int i)           { _mtt_my_rank = i;}
	void set_mtt_paid_places(const int i)       { _mtt_paid_places = i;}
	void set_mtt_largest_stack(const double d)  { _mtt_largest_stack = d;}
	void set_mtt_average_stack(const double d)  { _mtt_average_stack = d;}
	void set_mtt_smallest_stack(const double d) { _mtt_smallest_stack = d;}
 private:
	// MTT regions
	int _mtt_number_entrants;
	int _mtt_players_remaining;
	int _mtt_my_rank;
	int _mtt_paid_places;
	double _mtt_largest_stack;
	double _mtt_average_stack;
	double _mtt_smallest_stack;
};

#endif INC_CSymbolEngineMTTInfo_H