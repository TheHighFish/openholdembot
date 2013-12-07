//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose: PrWin-simulation
//
//***************************************************************************** 

#ifndef INC_CITERATORTHREAD_H
#define INC_CITERATORTHREAD_H

#include "inlines/eval.h"
#include "..\CCritSec\CCritSec.h"

extern sprw1326	_prw1326;	// prwin 1326 data structure Matrix 2008-04-29

extern class CIteratorThread
{
public:
	// public functions
	CIteratorThread();
	~CIteratorThread();
public:
	static void StopIteratorThread();
	void	StartIteratorThreadIfNeeded();
	void	RestartIteratorThread();
	void	set_prw1326_useme(const int i)	{ _prw1326.useme = i;}
	const	sprw1326 *prw1326()	            { return &_prw1326; }
#undef ENT
private:
	// private functions and variables - not available via accessors or mutators
	static UINT IteratorThreadFunction(LPVOID pParam);
	static void AdjustPrwinVariablesIfNecessary(CIteratorThread *pParent);
	static void StandardDealingAlgorithm(int nopponents);
	static void SwapDealingAlgorithmForMoreThan13Opponents(int nopponents);
	static void StandardDealingAlgorithmForUpTo13Opponents(int nopponents);
	static void EnhancedDealingAlgorithm();
	static bool UseEnhancedPrWin();
	static int  GetRandomCard();
private:
	static void UpdateIteratorVarsForDisplay(unsigned int nit);
	static void ResetIteratorVars();
	static void ResetGlobalVariables();
	static bool SimulationFinished(unsigned int nit);
private:
	void InitIteratorLoop(void);
	void InitHandranktTableForPrwin();
	void CloneVanillaChairToAllOtherChairs();
private:
	// variables for iterator loop
	CardMask		_plCards, _comCards;
	HANDLE			_m_stop_thread;
	HANDLE			_m_wait_thread;
} *p_iterator_thread;


#endif //INC_CITERATORTHREAD_H
