#include "stdafx.h"
#include "CSymbolEngineHistory.h"

CSymbolEngineHistory::CSymbolEngineHistory()
{}

CSymbolEngineHistory::~CSymbolEngineHistory()
{}

void CSymbolEngineHistory::InitOnStartup()
{}

void CSymbolEngineHistory::ResetOnConnection()
{}

void CSymbolEngineHistory::ResetOnHandreset()
{
	// Element 0 is unused
	for (int i=0; i<(k_number_of_betrounds+1); i++)
	{
		_nplayersround[i] = 0;
		_nbetsround[i] = 0;
		_didchec[i] = 0;
		_didcall[i] = 0;
		_didrais[i] = 0;
		_didswag[i] = 0;
	}
}

void CSymbolEngineHistory::ResetOnNewRound()
{}

void CSymbolEngineHistory::ResetOnMyTurn()
{}

void CSymbolEngineHistory::ResetOnHeartbeat()
{}


void CSymbols::CalcHistory(void)
{
	double		maxbet = 0.;

	if (_sym.nplayersround[(int) _sym.betround-1]==0)
	{
		set_sym_nplayersround((int) _sym.betround-1, _sym.nplayersplaying);					// nplayersroundx
	}
	set_sym_nplayersround(4, _sym.nplayersround[(int) _sym.betround-1]);						// nplayersround

	maxbet = 0;
	for (int i=0; i<p_tablemap->nchairs(); i++)
	{
		// Be careful: in some cases it might be that a user folds,
		// but "Fold" gets displayed where formerly his bet got displayed.
		// This may lead to ugly mis-scrapes, that's why he have to check
		// if the user is still playing.
		// (http://www.maxinmontreal.com/forums/viewtopic.php?f=111&t=10929)
		if ((_sym.currentbet[i] > maxbet)
			&& (((int(_sym.playersplayingbits) >> i) & 1) == 1))
		{
			maxbet = _sym.currentbet[i];
		}
	}

	maxbet /= (p_tablelimits->bet()==0 ? 1 : p_tablelimits->bet());
	if (maxbet > _sym.nbetsround[(int) _sym.betround-1])
	{
		set_sym_nbetsround((int) _sym.betround-1, maxbet);									// nbetsroundx
	}
	set_sym_nbetsround(4, maxbet);	// nbetsround
}