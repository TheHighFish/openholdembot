#ifndef INC_CSYMBOLENGINEPRWIN_H
#define INC_CSYMBOLENGINEPRWIN_H

#include "CVirtualSymbolEngine.h"
#include "assert.h"
#include "CSymbolEngineUserChair.h"
#include "CTableLimits.h"
#include "MagicNumbers.h"

class CSymbolEnginePrwin: public CVirtualSymbolEngine
{
public:
	CSymbolEnginePrwin();
	~CSymbolEnginePrwin();
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
	int nhands()		{ return (_nhandshi + _nhandslo + _nhandsti); }
	int nhandshi()		{ return _nhandshi; }
	int nhandslo()		{ return _nhandslo; }
	int nhandsti()		{ return _nhandsti; }
	double prwinnow()	{ return _prwinnow; }
	double prlosnow()	{ return _prlosnow; }
private:
	void CalculateProbabilities();
	void RestartIteratorThread();
	void CalculateNhands();
private:
	int _nhandshi;
	int _nhandslo;
	int _nhandsti;
	double _prwinnow;
	double _prlosnow;
private:
	int nopponents; //!!!
	int userchair; //!!!
};

#endif INC_CSYMBOLENGINEPRWIN_H
