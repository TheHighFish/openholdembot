#ifndef INC_CSYMBOLENGINEDEALERCHAIR_H
#define INC_CSYMBOLENGINEDEALERCHAIR_H

#include "CVirtualSymbolEngine.h"

#define DEALER_CHAIR p_symbol_engine_dealerchair->dealerchair()

extern class CSymbolEngineDealerchair: public CVirtualSymbolEngine
{
public:
	CSymbolEngineDealerchair();
	~CSymbolEngineDealerchair();
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
	inline int dealerchair()			{ return _dealerchair; }
private:
	void CalculateDealerChair();
private:
	int _dealerchair;
} *p_symbol_engine_dealerchair;

#endif INC_CSYMBOLENGINEUSERCHAIR_H