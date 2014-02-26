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

#ifndef INC_CSYMBOLENGINEPOKERVAL_H
#define INC_CSYMBOLENGINEPOKERVAL_H

#include "CVirtualSymbolEngine.h"
#include "assert.h"
#include "CSymbolEngineUserChair.h"
#include "CSymbolEngineTableLimits.h"
#include "MagicNumbers.h"
#include "NumericalFunctions.h"

class CSymbolEnginePokerval: public CVirtualSymbolEngine
{
public:
	CSymbolEnginePokerval();
	~CSymbolEnginePokerval();
public:
	// Mandatory reset-functions
	void InitOnStartup();
	void ResetOnConnection();
	void ResetOnHandreset();
	void ResetOnNewRound();
	void ResetOnMyTurn();
	void ResetOnHeartbeat();
public:
	int CalculatePokerval(HandVal hv, int n, int *pcb, int card0, int card1);
public:
	// Public accessors
	bool EvaluateSymbol(const char *name, double *result);
	CString IdentifiersProvided();;
public:
	// Pokerval
	int pokerval()			{ return _pokerval; }
	int pokervalplayer()	{ return _pokervalplayer; }
	int pokervalcommon()	{ return _pokervalcommon; }
	int pcbits()			{ return _pcbits; }
	int npcbits()			{ return bitcount(_pcbits); }
	// Hand tests
	bool ishandup()			{ return _ishandup; }
	bool ishandupcommon()	{ return _ishandupcommon; }
	bool ishicard()			{ return _hand_type == HandType_NOPAIR; }
	bool isonepair()		{ return _hand_type == HandType_ONEPAIR; }
	bool istwopair()		{ return _hand_type == HandType_TWOPAIR; }
	bool isthreeofakind()	{ return _hand_type == HandType_TRIPS; }
	bool isstraight()		{ return _hand_type == HandType_STRAIGHT; }
	bool isflush()			{ return _hand_type == HandType_FLUSH; }
	bool isfullhouse()		{ return _hand_type == HandType_FULLHOUSE; }
	bool isfourofakind()	{ return _hand_type == HandType_QUADS; }
	bool isstraightflush()	{ return _hand_type == HandType_STFLUSH; }
	bool isroyalflush()		{ return _isroyalflush; }
	// Pocket/common tests
	int ishipair()		{ return _ishipair; }
	int islopair()		{ return _islopair; }
	int ismidpair()		{ return _ismidpair; }
	int ishistraight()	{ return _ishistraight; }
	int ishiflush()		{ return _ishiflush; }
	// Bits
	int rankbits()			{ return _rankbitsplayer | _rankbitscommon; }
	int rankbitsplayer()	{ return _rankbitsplayer; }
	int rankbitscommon()	{ return _rankbitscommon; }
	int rankbitspoker()		{ return _rankbitspoker; }
	int srankbits()			{ return _srankbitsplayer | _srankbitscommon; }
	int srankbitsplayer()	{ return _srankbitsplayer; }
	int srankbitscommon()	{ return _srankbitscommon; }
	int srankbitspoker()	{ return _srankbitspoker; }
	//RANK HI
	int rankhi()			{ return GetRankHi(rankbits()); }
	int rankhicommon()		{ return GetRankHi(_rankbitscommon); }
	int rankhiplayer()		{ return GetRankHi(_rankbitsplayer); }
	int rankhipoker()		{ return GetRankHi(_rankbitspoker);  }
	int srankhi()			{ return GetRankHi(srankbits()); }
	int srankhicommon()		{ return GetRankHi(_srankbitscommon); }
	int srankhiplayer()		{ return GetRankHi(_srankbitsplayer); }
	int srankhipoker()		{ return GetRankHi(_srankbitspoker);  }
	//RANK LO
	int ranklo()			{ return GetRankLo(rankbits()); }
	int ranklocommon()		{ return GetRankLo(_rankbitscommon); }
	int rankloplayer()		{ return GetRankLo(_rankbitsplayer); }
	int ranklopoker()		{ return GetRankLo(_rankbitspoker); }
	int sranklo()			{ return GetRankLo(srankbits()); }
	int sranklocommon()		{ return GetRankLo(_srankbitscommon); }
	int srankloplayer()		{ return GetRankLo(_srankbitsplayer); }
	int sranklopoker()		{ return GetRankLo(_srankbitspoker); }
public:
	CString HandType();
private:
	void CalculateRankBits();
	void CalcPokerValues();
	void CalculateHandType();
	bool IsHigherStraightPossible(HandVal handval);
private:
	int GetRankHi(int rankbits);
	int GetRankLo(int rankbits);
	void SetRankBit(int* rankbits, int rank);
private:
	int _pokerval;
	int _pokervalplayer;
	int _pokervalcommon;
	int _pcbits;
	int _npcbits;
private:
	int _hand_type;
	bool _isroyalflush;
	bool _ishandup;
	bool _ishandupcommon;
private:
	int _ishipair;
	int _islopair;
	int _ismidpair;
	int _ishistraight;
	int _ishiflush;
private:
	int _rankbitsplayer;
	int _rankbitscommon;
	int _rankbitspoker;
private:
	int _srankbitsplayer;
	int _srankbitscommon;
	int _srankbitspoker;
private:
	// for ishandup and ishandupcommon symbol calcs
	HandVal	_phandval[k_number_of_betrounds];
	HandVal	_chandval[k_number_of_betrounds];	 
private:
	int nCards;
	HandVal	handval; 
};

extern CSymbolEnginePokerval *p_symbol_engine_pokerval;

#endif INC_CSYMBOLENGINEPOKERVAL_H