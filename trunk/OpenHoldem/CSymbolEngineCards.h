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

#ifndef INC_CSYMBOLENGINECARDS_H
#define INC_CSYMBOLENGINECARDS_H

#include "CVirtualSymbolEngine.h"

extern class CSymbolEngineCards: public CVirtualSymbolEngine
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
	bool ispair()			{ return _ispair; }
	bool issuited()			{ return _issuited; }		
	bool isconnector()		{ return _isconnector; }
public:
	bool IsHand(const char *a, int *e);

public:
	//COMMON CARDS
	int ncommoncardsknown()		{ return _ncommoncardsknown; }

	//(UN)KNOWN CARDS
	int nouts()					{ return _nouts; }
	int ncardsknown()			{ return _ncardsknown; }
	int ncardsunknown()			{ return _ncardsunknown; }
	int ncardsbetter()			{ return _ncardsbetter; }

	//FLUSHES SETS STRAIGHTS
	int nsuited()					{ return _nsuited; }
	int nsuitedcommon()				{ return _nsuitedcommon; }
	int tsuit()						{ return _tsuit; }
	int tsuitcommon()				{ return _tsuitcommon; }
	int nranked()					{ return _nranked; }
	int nrankedcommon()				{ return _nrankedcommon; }
	int trank()						{ return _trank; }
	int trankcommon()				{ return _trankcommon; }
	int nstraight()					{ return _nstraight; }
	int nstraightcommon()			{ return _nstraightcommon; }
	int nstraightfill()				{ return _nstraightfill; }
	int nstraightfillcommon()		{ return _nstraightfillcommon; }
	int nstraightflush()				{ return _nstraightflush; }
	int nstraightflushcommon()		{ return _nstraightflushcommon; }
	int nstraightflushfill()			{ return _nstraightflushfill; }
	int nstraightflushfillcommon()	{ return _nstraightflushfillcommon; }

	//HAND TESTS
	int $$pc(int index)		{ return _$$pc[index]; }
	int $$pr(int index)		{ return _$$pr[index]; }
	int $$ps(int index)		{ return _$$ps[index]; }
	int $$cc(int index)		{ return _$$cc[index]; }
	int $$cr(int index)		{ return _$$cr[index]; }
	int $$cs(int index)		{ return _$$cs[index]; }

private:
	void CalcPocketTests();
	void CalcFlushesStraightsSets();
	void CalculateHandTests();
	void CalculateCommonCards();
	void CalcUnknownCards();
private:
	void CardMaskCreateCMAllCardsOfOfSuit(CardMask *card_mask_of_suit, int suit);
	int  GetNumberOfCardsForSuit(CardMask cards, CardMask card_mask_of_suit);
	int  GetDominantSuit(CardMask cards, int* max_cards_of_same_suit);
private:
	//POCKET TESTS
	bool _ispair;
	bool _issuited;
	bool _isconnector;

	//COMMON CARDS
	int _ncommoncardsknown;

	//(UN)KNOWN CARDS
	int _nouts;
	int _ncardsknown;
	int _ncardsunknown;
	int _ncardsbetter;

	//NHANDS
	int _nhands;
	int _nhandshi;
	int _nhandslo;
	int _nhandsti;

	//FLUSHES SETS STRAIGHTS
	int _nsuited;
	int _nsuitedcommon;
	int _tsuit;
	int _tsuitcommon;
	int _nranked;
	int _nrankedcommon;
	int _trank;
	int _trankcommon;
	int _nstraight;
	int _nstraightcommon;
	int _nstraightfill;
	int _nstraightfillcommon;
	int _nstraightflush;
	int _nstraightflushcommon;
	int _nstraightflushfill;
	int _nstraightflushfillcommon;

	//HAND TESTS
	int _$$pc[k_number_of_cards_per_player];
	int _$$pr[k_number_of_cards_per_player];
	int _$$ps[k_number_of_cards_per_player];
	int _$$cc[k_number_of_community_cards];
	int _$$cr[k_number_of_community_cards];
	int _$$cs[k_number_of_community_cards];

private:
	 // Card masks
	CardMask heartsCards, diamondsCards, clubsCards, spadesCards;
} *p_symbol_engine_cards;

#endif INC_CSYMBOLENGINECARDS_H