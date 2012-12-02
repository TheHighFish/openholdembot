#ifndef INC_CSYMBOLENGINERAISERSCALLERS_H
#define INC_CSYMBOLENGINERAISERSCALLERS_H

#include "CVirtualSymbolEngine.h"
#include "..\CTransform\CTransform.h"

class CSymbolEngineRaisersCallers: public CVirtualSymbolEngine
{
public:
	CSymbolEngineRaisersCallers();
	~CSymbolEngineRaisersCallers();
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
	int raischair()	{ return _raischair; }
	int raisbits(int betround)
	{
		if ((betround >= k_betround_preflop)
			&& (betround <= k_betround_river))
		{
			return _raisbits[betround];
		}
		else
		{
			return k_undefined;
		}
	}
public:
	int foldbits(int betround)
	{
		if ((betround >= k_betround_preflop)
			&& (betround <= k_betround_river))
		{
			return _foldbits[betround];
		}
		else
		{
			return k_undefined;
		}
	}
public:
	int nplayerscallshort()		{ return _nplayerscallshort; }
	int nopponentsbetting()		{ return _nopponentsbetting; }
	int nopponentsfolded()		{ return _nopponentsfolded; }
	int nopponentschecking()	{ return _nopponentschecking; }
private:
	void CalculateNOpponentsCheckingBettingFolded();
private:
	int _raischair;
	int _nplayerscallshort;
	int _nopponentsbetting;
	int _nopponentsfolded;
	int _nopponentschecking;
private:
	int _raisbits[k_number_of_betrounds + 1];
	int _foldbits[k_number_of_betrounds + 1];

};

#endif INC_CSYMBOLENGINERAISERSCALLERS_H