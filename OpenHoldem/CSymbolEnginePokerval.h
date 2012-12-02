#ifndef INC_CSYMBOLENGINEPOKERVAL_H
#define INC_CSYMBOLENGINEPOKERVAL_H

#include "CVirtualSymbolEngine.h"
#include "assert.h"
#include "CSymbolEngineUserChair.h"
#include "CTableLimits.h"
#include "..\CTransform\CTransform.h"
#include "MagicNumbers.h"

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
	// Public accessors
	// Pokerval
	double pokerval()			{ return _pokerval; }
	double pokervalplayer()		{ return _pokervalplayer; }
	double pokervalcommon()		{ return _pokervalcommon; }
	double pcbits()				{ return _pcbits; }
	double npcbits()			{ return bitcount(_pcbits); }
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
	double ishipair()		{ return _ishipair; }
	double islopair()		{ return _islopair; }
	double ismidpair()		{ return _ismidpair; }
	double ishistraight()	{ return _ishistraight; }
	double ishiflush()		{ return _ishiflush; }
	//RANK HI
	double rankhi()				{ return GetRankHi(_rankbitsplayer & _rankbitscommon); }
	double rankhicommon()		{ return GetRankHi(_rankbitscommon); }
	double rankhiplayer()		{ return GetRankHi(_rankbitsplayer); }
	double rankhipoker()		{ return GetRankHi(_rankbitspoker);  }
	double srankhi()			{ return GetRankHi(_srankbitsplayer & _srankbitscommon); }
	double srankhicommon()		{ return GetRankHi(_srankbitscommon); }
	double srankhiplayer()		{ return GetRankHi(_srankbitsplayer); }
	double srankhipoker()		{ return GetRankHi(_srankbitspoker);  }
	//RANK LO
	double ranklo()				{ return GetRankLo(_rankbitsplayer & _rankbitscommon); }
	double ranklocommon()		{ return GetRankLo(_rankbitscommon); }
	double rankloplayer()		{ return GetRankLo(_rankbitsplayer); }
	double ranklopoker()		{ return GetRankLo(_rankbitspoker); }
	double sranklo()			{ return GetRankLo(_srankbitsplayer & _srankbitscommon); }
	double sranklocommon()		{ return GetRankLo(_srankbitscommon); }
	double srankloplayer()		{ return GetRankLo(_srankbitsplayer); }
	double sranklopoker()		{ return GetRankLo(_srankbitspoker); }
private:
	void CalculateRankBits();
	void CalcPokerValues();
	void CalculateHandType();
	bool IsHigherStraightPossible(HandVal handval);
	double CalcPokerval(HandVal hv, int n, double *pcb, int card0, int card1);
private:
	int GetRankHi(int rankbits);
	int GetRankLo(int rankbits);
	void SetRankBit(int* rankbits, int rank);
private:
	double _pokerval;
	double _pokervalplayer;
	double _pokervalcommon;
	double _pcbits;
	double _npcbits;
private:
	int _hand_type;
	bool _isroyalflush;
	bool _ishandup;
	bool _ishandupcommon;
private:
	double _ishipair;
	double _islopair;
	double _ismidpair;
	double _ishistraight;
	double _ishiflush;
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
	HandVal		_phandval[4], _chandval[4];	
private:
	int userchair;
	int betround;
};

#endif INC_CSYMBOLENGINEPOKERVAL_H