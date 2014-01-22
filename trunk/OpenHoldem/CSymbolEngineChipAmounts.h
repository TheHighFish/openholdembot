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

#ifndef INC_CSYMBOLENGINECHIPAMOUNTS_H
#define INC_CSYMBOLENGINECHIPAMOUNTS_H

#include "CVirtualSymbolEngine.h"
#include "assert.h"
#include "CSymbolEngineUserChair.h"
#include "CSymbolEngineTableLimits.h"
#include "MagicNumbers.h"

extern class CSymbolEngineChipAmounts: public CVirtualSymbolEngine
{
public:
	CSymbolEngineChipAmounts();
	~CSymbolEngineChipAmounts();
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
	double maxbalance()					{ return _maxbalance; }
	double balanceatstartofsession()	{ return _balanceatstartofsession; }
	double balance(int player)	
	{
		assert((player >= 0) || (player == k_undefined));
		assert(player < k_max_number_of_players);
		if (player == k_undefined)
		{
			return k_undefined_zero;
		}
		return _balance[player];
	}
public:
	double stack(int nth_best)
	{
		assert(nth_best >= 0);
		assert(nth_best < k_max_number_of_players);
		return _stack[nth_best];
	}

	double stacks_at_hand_start(const int chair) 
	{ 
		assert((chair >= 0) || (chair == k_undefined));
		assert(chair < k_max_number_of_players);
		if (chair == k_undefined)
		{
			return 0;
		}
		return _stacks_at_hand_start[chair]; 
	}

	double currentbet(int player)	
	{
		assert((player >= 0) || (player == k_undefined));
		assert(player < k_max_number_of_players);
		if (player == k_undefined)
		{
			return 0;
		}
		return _currentbet[player];
	}

	double ncurrentbets()
	{
		if (p_symbol_engine_tablelimits->bet() == 0)
		{
			return 0;
		}
		return (currentbet(p_symbol_engine_userchair->userchair()) / p_symbol_engine_tablelimits->bet());
	}
public:
	double pot()			{ return _pot; }
	double potcommon()		{ return _potcommon; }
	double potplayer()		{ return _potplayer; }
public:
	double sraiprev()		{ return _sraiprev; }
	double sraimin()		{ return _sraimin; }
	double sraimax()		{ return _sraimax; }
public:
	double call()			{ return _call;			}
	double nbetstocall()	{ return _nbetstocall;	}
	double nbetstorais()	{ return _nbetstorais; 	}
	double ncallbets()		{ return _ncallbets;	}
	double nraisbets()		{ return _nraisbets;	}
private:
	// private setters
	void SetBalance(const int player, const double d);
	void SetMaxBalanceConditionally(const double d);
	void SetBalanceAtStartOfSessionConditionally(const double d);
private:
	void CalculateBalances();
	void CalculateStacks();
	void CalculateCurrentbets();
	void CalculatePots();
	void CalculateAmountsToCallToRaise();
	void CalculateBetsToCallToRaise();
private:
	double Largestbet();
private:
	double _balance[k_max_number_of_players];				
	double _maxbalance;
	double _balanceatstartofsession;
	double _stack[k_max_number_of_players];
	double _currentbet[k_max_number_of_players];	
private:
	// Used in ICM calculator - ICM needs stacks at beginning of hand
	double _stacks_at_hand_start[k_max_number_of_players];	
	double _pot;
	double _potcommon;
	double _potplayer;
private:
	double _sraiprev;
	double _sraimin;
	double _sraimax;
private:
	double _call;
	double _nbetstocall;
	double _nbetstorais;
	double _ncallbets;
	double _nraisbets;
private:
} *p_symbol_engine_chip_amounts;

#endif INC_CSYMBOLENGINECHIPAMOUNTS_H