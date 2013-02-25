#ifndef INC_CSYMBOLS_H
#define INC_CSYMBOLS_H

#include "CVirtualSymbolEngine.h"
#include <assert.h>
#include "MagicNumbers.h"


// Main symbol-class which provides the GetSymbolVal()-function.
// We treat it like the other symbol-engines, because it also
// contains some Reset-functions.
// They should be called after all other engines functions,
// i.e. this symbol-engine should be the last one to be added to the container.

extern class CSymbols: public CVirtualSymbolEngine
{
public:
	// public functions
	CSymbols();
	~CSymbols();
public:
	// Mandatory reset-functions
	void InitOnStartup();
	void ResetOnConnection();
	void ResetOnHandreset();
	void ResetOnNewRound();
	void ResetOnMyTurn();
	void ResetOnHeartbeat();
public:
	void RecordPrevAction(const ActionConstant action);
	double GetSymbolVal(const char *a, int *e);
	void LogHandReset();
public:
	// public accessors
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
	CCritSec	m_critsec;
private:
} *p_symbols;

#endif /* INC_CSYMBOLS_H */
