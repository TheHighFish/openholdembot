#ifndef INC_CSYMBOLENGINEHANDRANK_H
#define INC_CSYMBOLENGINEHANDRANK_H

#include "CVirtualSymbolEngine.h"

extern class CSymbolEngineHandrank: public CVirtualSymbolEngine
{
public:
	CSymbolEngineHandrank();
	~CSymbolEngineHandrank();
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
	double handrank169()      { return _handrank169; }
	double handrank2652()     { return _handrank2652; }
	double handrank1326()     { return _handrank1326; }
	double handrank1000()     { return _handrank1000; }
	double handrankp()        { return _handrankp; }
private:
	void GetCardstring(char *c, unsigned int c0, unsigned int c1);
private:
	double _handrank169;
	double _handrank2652;
	double _handrank1326;
	double _handrank1000;
	double _handrankp;
} *p_symbol_engine_handrank;

#endif INC_CSYMBOLENGINEHANDRANK_H