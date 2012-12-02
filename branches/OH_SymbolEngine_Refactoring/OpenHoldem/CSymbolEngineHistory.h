#ifndef INC_CSYMBOLENGINEHISTORY_H
#define INC_CSYMBOLENGINEHISTORY_H

#include "CVirtualSymbolEngine.h"

class CSymbolEngineHistory: public CVirtualSymbolEngine
{
public:
	CSymbolEngineHistory();
	~CSymbolEngineHistory();
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
	bool DidAct()	
	{
		// Not considering fold or allin, because the game would be over.
		return (didchec() || didcall() || didswag() || didrais());
	}
public:
	// !!! range-check
	int nplayersround(int betround)		{ return _nplayersround[betround]; }
	int botslastaction(int betround)	{ return _botslastaction[betround]; }
	double nbetsround(int betround)		{ return _nbetsround[betround]; }
	bool didchec(int betround)			{ return _didchec[betround]; }
	bool didchec(int betround)			{ return _didchec[betround]; }
	bool didrais(int betround)			{ return _didrais[betround]; }
	bool didswag(int betround)			{ return _didswag[betround]; }
private:
	double _prevaction;
private:
	// Element 0 is unused
	// Elements 1..4 are for the 4 betrounds
	int _nplayersround[k_number_of_betrounds + 1];		
	int _botslastaction[k_number_of_betrounds + 1];		
	double _nbetsround[k_number_of_betrounds + 1];
	bool _didchec[k_number_of_betrounds + 1];
	bool _didcall[k_number_of_betrounds + 1];
	bool _didrais[k_number_of_betrounds + 1];
	bool _didswag[k_number_of_betrounds + 1];
};

#endif INC_CSYMBOLENGINEHISTORY_H


