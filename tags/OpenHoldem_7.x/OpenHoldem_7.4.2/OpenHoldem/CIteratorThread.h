//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: PrWin-simulation
//
//******************************************************************************

#ifndef INC_CITERATORTHREAD_H
#define INC_CITERATORTHREAD_H

#include "inlines/eval.h"
#include "..\CCritSec\CCritSec.h"

extern sprw1326	_prw1326;	// prwin 1326 data structure Matrix 2008-04-29

class CIteratorThread {
 public:
	// public functions
	CIteratorThread();
	~CIteratorThread();
 public:
  // Public accessors()
  double prwin()   { return _prwin; }
  double prtie()   { return _prtie; }
  double prlos()   { return _prlos; }
 public:
  static bool IteratorThreadComplete() { return _iterations_calculated >= _iterations_required; }
  bool IteratorThreadWorking()    { return ((_iterations_calculated > 0) && (_iterations_calculated < _iterations_required)); }
  int  IteratorThreadProgress()   { return _iterations_calculated; }
 public:
	void StartPrWinComputationsIfNeeded();
  void StartIteratorThread();
	void set_prw1326_useme(const int i)	{ _prw1326.useme = i;}
	const	sprw1326 *prw1326()	          { return &_prw1326; }
 public:
  // For the DLL "cmd$recalc"
  void RestartPrWinComputations();
#undef ENT
 private:
	// private functions and variables - not available via accessors or mutators
	static UINT IteratorThreadFunction(LPVOID pParam);
	static void AdjustPrwinVariablesIfNecessary();
	static void StandardDealingAlgorithm(int nopponents);
	static void SwapDealingAlgorithmForMoreThan13Opponents(int nopponents);
	static void StandardDealingAlgorithmForUpTo13Opponents(int nopponents);
	static void EnhancedDealingAlgorithm();
	static bool UseEnhancedPrWin();
	static int  GetRandomCard();
 private:
	static void UpdateIteratorVarsForDisplay();
	static void ResetIteratorVars();
	static void ResetGlobalVariables();
 private:
	void InitIteratorLoop();
	void InitHandranktTableForPrwin();
	void CloneVanillaChairToAllOtherChairs();
	void InitNumberOfIterations();
 private:
	// variables for iterator loop
	CardMask		_plCards, _comCards;
	HANDLE			_m_stop_thread;
	HANDLE			_m_wait_thread;
 private:
  static int _iterations_calculated;
  static int _iterations_required;
  static int _nopponents;;
 private:
  static double _prwin, _prtie, _prlos;
};

extern CIteratorThread *p_iterator_thread;

#endif //INC_CITERATORTHREAD_H
