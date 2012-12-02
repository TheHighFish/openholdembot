#ifndef INC_CSYMBOLENGINECARDS_H
#define INC_CSYMBOLENGINECARDS_H

#include "CVirtualSymbolEngine.h"

class CSymbolEngineCards: public CVirtualSymbolEngine
{
public:
	CSymbolEngineCards();
	~CSymbolEngineCards();
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

	//POCKET TESTS
	double ispair()			{ return _ispair; }
	double issuited()		{ return _issuited; }		
	double isconnector()	{ return _isconnector; }

	//COMMON CARDS
	double ncommoncardspresent()	{ return _ncommoncardspresent; }
	double ncommoncardsknown()		{ return _ncommoncardsknown; }

	//(UN)KNOWN CARDS
	double nouts()					{ return _nouts; }
	double ncardsknown()			{ return _ncardsknown; }
	double ncardsunknown()			{ return _ncardsunknown; }
	double ncardsbetter()			{ return _ncardsbetter; }

	//FLUSHES SETS STRAIGHTS
	double nsuited()					{ return _nsuited; }
	double nsuitedcommon()				{ return _nsuitedcommon; }
	double tsuit()						{ return _tsuit; }
	double tsuitcommon()				{ return _tsuitcommon; }
	double nranked()					{ return _nranked; }
	double nrankedcommon()				{ return _nrankedcommon; }
	double trank()						{ return _trank; }
	double trankcommon()				{ return _trankcommon; }
	double nstraight()					{ return _nstraight; }
	double nstraightcommon()			{ return _nstraightcommon; }
	double nstraightfill()				{ return _nstraightfill; }
	double nstraightfillcommon()		{ return _nstraightfillcommon; }
	double nstraightflush()				{ return _nstraightflush; }
	double nstraightflushcommon()		{ return _nstraightflushcommon; }
	double nstraightflushfill()			{ return _nstraightflushfill; }
	double nstraightflushfillcommon()	{ return _nstraightflushfillcommon; }

	//HAND TESTS
	int $$pc(int index)		{ return _$$pc[index]; }
	int $$pr(int index)		{ return _$$pr[index]; }
	int $$ps(int index)		{ return _$$ps[index]; }
	int $$cc(int index)		{ return _$$cc[index]; }
	int $$cr(int index)		{ return _$$cr[index]; }
	int $$cs(int index)		{ return _$$cs[index]; }

private:
	bool BothPocketCardsKnown();
	void CalcPocketTests();
	void CalcFlushesStraightsSets();
	void CalculateHandTests();

private:
	//POCKET TESTS
	double _ispair;
	double _issuited;
	double _isconnector;

	//HAND TESTS
	double _ishandup;
	double _ishandupcommon;
	double _ishicard;
	double _isonepair;
	double _istwopair;
	double _isthreeofakind;
	double _isstraight;
	double _isflush;
	double _isfullhouse;
	double _isfourofakind;
	double _isstraightflush;
	double _isroyalflush;

	//COMMON CARDS
	double _ncommoncardspresent;
	double _ncommoncardsknown;

	//(UN)KNOWN CARDS
	double _nouts;
	double _ncardsknown;
	double _ncardsunknown;
	double _ncardsbetter;

	//NHANDS
	double _nhands;
	double _nhandshi;
	double _nhandslo;
	double _nhandsti;

	//FLUSHES SETS STRAIGHTS
	double _nsuited;
	double _nsuitedcommon;
	double _tsuit;
	double _tsuitcommon;
	double _nranked;
	double _nrankedcommon;
	double _trank;
	double _trankcommon;
	double _nstraight;
	double _nstraightcommon;
	double _nstraightfill;
	double _nstraightfillcommon;
	double _nstraightflush;
	double _nstraightflushcommon;
	double _nstraightflushfill;
	double _nstraightflushfillcommon;

	//HAND TESTS
	int _$$pc[k_number_of_cards_per_player];
	int _$$pr[k_number_of_cards_per_player];
	int _$$ps[k_number_of_cards_per_player];
	int _$$cc[k_number_of_community_cards];
	int _$$cr[k_number_of_community_cards];
	int _$$cs[k_number_of_community_cards];
private:
	int _userchair;
};

#endif INC_CSYMBOLENGINECARDS_H