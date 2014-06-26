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

#ifndef INC_CSYMBOLENGINEVERSUS_H
#define INC_CSYMBOLENGINEVERSUS_H

#include "CVirtualSymbolEngine.h"

const int card0_offset[52] = { 0, 62475, 123725, 183750, 242550, 300125, 356475, 411600, 465500, 518175, 569625,
	619850, 668850, 716625, 763175, 808500, 852600, 895475, 937125, 977550, 1016750,
	1054725, 1091475, 1127000, 1161300, 1194375, 1226225, 1256850, 1286250, 1314425,
	1341375, 1367100, 1391600, 1414875, 1436925, 1457750, 1477350, 1495725, 1512875,
	1528800, 1543500, 1556975, 1569225, 1580250, 1590050, 1598625, 1605975, 1612100,
	1617000, 1620675, 1623125, 1624350};

class CSymbolEngineVersus: public CVirtualSymbolEngine {
 public:
  CSymbolEngineVersus();
  ~CSymbolEngineVersus();
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
  bool EvaluateVersusHandListSymbol(const char *name, double *result, bool log = false);
  CString SymbolsProvided();
  bool VersusBinLoaded()  { return !_versus_bin_not_loaded; }
 public:
  bool GetCounts();
 private:
  void DoCalc(const CardMask plCards, const CardMask oppCards, const CardMask comCards, 
	  unsigned int *wintemp, unsigned int *tietemp, unsigned int *lostemp);
  void ClearWinTieLosData();
  inline bool CheckForLoadedVersusBin();
  void ErrorInvalidSymbol(CString name);
private:
  int	_versus_fh;
  bool _versus_bin_not_loaded;
  CCritSec m_critsec;
 private:
  int				_nhands, _nwin, _ntie, _nlos, _nhandshi, _nhandsti, _nhandslo;
	double		_vsprwin, _vsprtie, _vsprlos;
	double		_vsprwinhi, _vsprtiehi, _vsprloshi;
	double		_vsprwinti, _vsprtieti, _vsprlosti;
	double		_vsprwinlo, _vsprtielo, _vsprloslo;
	int				_nhandshinow, _nhandstinow, _nhandslonow;
	double		_vsprwinhinow, _vsprtiehinow, _vsprloshinow;
	double		_vsprwintinow, _vsprtietinow, _vsprlostinow;
	double		_vsprwinlonow, _vsprtielonow, _vsprloslonow;
 private:
  // Number of winning/tieing/losing outcomes of my current hand 
  // at the current board against a concrete holding.
  // Precomputed against all possible opponents card combinations.
  int _n_win_against_hand[k_number_of_cards_per_deck][k_number_of_cards_per_deck];
  int _n_tie_against_hand[k_number_of_cards_per_deck][k_number_of_cards_per_deck];
  int _n_los_against_hand[k_number_of_cards_per_deck][k_number_of_cards_per_deck];
 private:
  
  unsigned int card_player[2];
  unsigned int card_common[5];
  unsigned int pcard[2];
};

extern CSymbolEngineVersus *p_symbol_engine_versus;

#endif INC_CSYMBOLENGINEVERSUS_H