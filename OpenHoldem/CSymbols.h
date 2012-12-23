#ifndef INC_CSYMBOLS_H
#define INC_CSYMBOLS_H

#include <assert.h>
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineBlinds.h"
#include "CSymbolEngineCards.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineDealerchair.h"
#include "CSymbolEngineHandrank.h"
#include "CSymbolEngineHistory.h"
#include "CSymbolEngineLists.h"
#include "CSymbolEnginePokerval.h"
#include "CSymbolEnginePositions.h"
#include "CSymbolEnginePrwin.h"
#include "CSymbolEngineRaisersCallers.h"
#include "CSymbolEngineRandom.h"
#include "CSymbolEngineTime.h"
#include "CSymbolEngineUserchair.h"
#include "CTableLimits.h"
#include "MagicNumbers.h"


extern class CSymbols 
{
public:
	// public functions
	CSymbols();
	~CSymbols();
	void ResetSymbolsFirstTime(void);
	void CalcSymbols(void);
	void CalcProbabilities(void);
	void RecordPrevAction(const ActionConstant action);
	const double GetSymbolVal(const char *a, int *e);
	const double IsHand(const char *a, int *e=NULL);
	bool	DidAct(void);

public:
	// public accessors
	const double		bigbet()	{ return p_tablelimits->bbet(); }
	const CArray <CString, CString> *logsymbols_collection() { return &_logsymbols_collection; }
	const CArray <CString, CString> *symboltrace_collection() { return &_symboltrace_collection; }

public:
#define ENT CSLock lock(m_critsec);
	// public mutators
	void	logsymbols_collection_add(const char *s)	{ ENT _logsymbols_collection.Add(s);}
	void	logsymbols_collection_removeall()			{ ENT _logsymbols_collection.RemoveAll();}
	void	symboltrace_collection_add(const char *s)	{ ENT _symboltrace_collection.Add(s);}
	void	symboltrace_collection_removeall()			{ ENT _symboltrace_collection.RemoveAll();}
#undef ENT
private:
	// private variables - use public accessors and public mutators to address these
	
	CArray <CString, CString>   _logsymbols_collection; // Used to track the log$ symbols
	CArray <CString, CString>   _symboltrace_collection;// Used to trace function execution

private:
	// private functions and variables - not available via accessors or mutators
	void ResetSymbolsNewHand(void);
	void ResetSymbolsEveryCalc(void);
private:
	CCritSec	m_critsec;
private:
	int _betround; 
	int _userchair;
} *p_symbols;

#endif /* INC_CSYMBOLS_H */
