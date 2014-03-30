//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose:
//
//***************************************************************************** 

#ifndef INC_CENGINECONTAINER_H
#define INC_CENGINECONTAINER_H

#include "CVirtualSymbolEngine.h"

const int k_max_number_of_symbol_engines = 50;

extern class CEngineContainer
{
public:
	CEngineContainer();
	~CEngineContainer();
public:
	// both to be called by the auto-connector
	void ResetOnConnection();
	void ResetOnDisconnection();
public:
	void CallSymbolEnginesToUpdateSymbolsIfNecessary();
	bool EvaluateSymbol(const char *name, double *result);
public:
#define ENT CSLock lock(m_critsec);
	// public mutators
	void	logsymbols_collection_add(const char *s)	{ ENT _logsymbols_collection.Add(s);}
	void	logsymbols_collection_removeall()			{ ENT _logsymbols_collection.RemoveAll();}
	void	symboltrace_collection_add(const char *s)	{ ENT _symboltrace_collection.Add(s);}
	void	symboltrace_collection_removeall()			{ ENT _symboltrace_collection.RemoveAll();}
#undef ENT
	public:
	// public accessors
	const CArray <CString, CString> *logsymbols_collection() { return &_logsymbols_collection; }
	const CArray <CString, CString> *symboltrace_collection() { return &_symboltrace_collection; }
private:
	void CreateSpecialSymbolEngines();
	void AddSymbolEngine(CVirtualSymbolEngine *new_symbol_engine);
	void CreateSymbolEngines();
	void DestroyAllSymbolEngines();
	void DestroyAllSpecialSymbolEngines();
private:
	void ResetOnHandreset();
	void ResetOnNewRound();
	void ResetOnMyTurn();
	void ResetOnHeartbeat();
private:
	CVirtualSymbolEngine *_symbol_engines[k_max_number_of_symbol_engines]; 
	int _number_of_symbol_engines_loaded;
	bool _reset_on_connection_executed;
private:
	// private variables - use public accessors and public mutators to address these
	CArray <CString, CString>   _logsymbols_collection; // Used to track the log$ symbols
	CArray <CString, CString>   _symboltrace_collection;// Used to trace function execution
private:
	CCritSec m_critsec;
} *p_engine_container;

#endif INC_CENGINECONTAINER_H