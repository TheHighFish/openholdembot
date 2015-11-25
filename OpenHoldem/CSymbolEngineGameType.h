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

#ifndef INC_CSYMBOLENGINEGAMETYPE_H
#define INC_CSYMBOLENGINEGAMETYPE_H

#include "MagicNumbers.h"
#include "CVirtualSymbolEngine.h"

class CSymbolEngineGameType: public CVirtualSymbolEngine {
 public:
	CSymbolEngineGameType();
	~CSymbolEngineGameType();
	// public mutators
 public:
	void InitOnStartup();
	void ResetOnConnection();
	void ResetOnHandreset();
	void ResetOnNewRound();
	void ResetOnMyTurn();
	void ResetOnHeartbeat();
	void CalcTableLimits();
 public:
	bool EvaluateSymbol(const char *name, double *result, bool log = false);
  CString SymbolsProvided();
 public:
	int gametype()	{ return _gametype; }	// former p_symbol_engine_various_data_lookup->sym()->lim
	bool isnl()			{ return (gametype() == kGametypeNL); }
	bool ispl()			{ return (gametype() == kGametypePL); }
	bool isfl()			{ return (gametype() == kGametypeFL); }
 public:
	CString GetGameTypeAsString();
	// private data members
 private:
	int		 _gametype;
};

extern CSymbolEngineGameType *p_symbol_engine_gametype;

#endif // INC_CSYMBOLENGINEGAMETYPE_H
