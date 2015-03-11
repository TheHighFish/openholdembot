//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#include "stdafx.h"
#include "CSymbolEnginePokerval.h"

#include <assert.h>
#include "CardFunctions.h"
#include "CBetroundCalculator.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CScrapedActionInterface.h"
#include "CSymbolEngineCards.h"
#include "CTableState.h"
#include "CSymbolEngineUserchair.h"
#include "..\CTransform\CTransform.h"
#include "inlines/eval.h"
#include "MagicNumbers.h"
#include "NumericalFunctions.h"

CSymbolEnginePokerval *p_symbol_engine_pokerval = NULL;

CSymbolEnginePokerval::CSymbolEnginePokerval()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	assert(p_symbol_engine_cards != NULL);
	assert(p_symbol_engine_tablelimits != NULL);
	assert(p_symbol_engine_userchair != NULL);
}

CSymbolEnginePokerval::~CSymbolEnginePokerval()
{}

void CSymbolEnginePokerval::InitOnStartup()
{}

void CSymbolEnginePokerval::ResetOnConnection()
{
	ResetOnHandreset();
}

void CSymbolEnginePokerval::ResetOnHandreset()
{
	for (int i=0; i<k_number_of_betrounds; i++)
	{
		_phandval[i] = 0;
		_chandval[i] = 0;
	}
	nCards  = 0;
	handval = 0;
}

void CSymbolEnginePokerval::ResetOnNewRound()
{}

void CSymbolEnginePokerval::ResetOnMyTurn() {
}

void CSymbolEnginePokerval::ResetOnHeartbeat()
{
	_ishistraight = false;
	_hand_type = 0;
	_isroyalflush = false;
	_ishandup = false;
	_ishandupcommon = false;
	_ishipair = 0;
	_islopair = 0;
	_ismidpair = 0;
	_ishistraight = 0;
	_ishiflush = 0;
	_rankbitsplayer = 0;
	_rankbitscommon = 0;
	_rankbitspoker = 0;
	_srankbitsplayer = 0;
	_srankbitscommon = 0;
	_srankbitspoker = 0;

	CalculateRankBits();
	CalcPokerValues();
}


void CSymbolEnginePokerval::CalcPokerValues()
{
	CardMask		Cards = {0};
	int				dummy = 0;

	///////////////////////////////////////////////////////////////////
	// pokerval
	nCards = 0;
	CardMask_RESET(Cards);
	for (int i=0; i<k_number_of_cards_per_player; i++)
	{
		if (p_table_state->User()->HasKnownCards())
		{
      CardMask_SET(Cards, p_table_state->User()->_hole_cards[i].GetValue());
			nCards++;
		}
	}

	for (int i=0; i<k_number_of_community_cards; i++)
	{
		// common cards
    Card card = p_table_state->_common_cards[i];
    if (card.IsKnownCard())
		{
      CardMask_SET(Cards, card.GetValue());
			nCards++;
		}
	}

	handval = Hand_EVAL_N(Cards, nCards);

	_pcbits = 0;
	_pokerval = CalculatePokerval(handval, nCards, &_pcbits,				
    p_table_state->User()->_hole_cards[0].GetValue(), 
    p_table_state->User()->_hole_cards[1].GetValue());

	write_log(preferences.debug_symbolengine(), "[CSymbolEnginePokerval] handval = %i\n", handval);
	write_log(preferences.debug_symbolengine(), "[CSymbolEnginePokerval] pokerval = %i\n", _pokerval);
	write_log(preferences.debug_symbolengine(), "[CSymbolEnginePokerval] nCards = %i\n", nCards);
	write_log(preferences.debug_symbolengine(), "[CSymbolEnginePokerval] pcbits = %i\n", _pcbits);

	_phandval[BETROUND-1] = _pokerval & 0xff000000; 

	if (BETROUND > k_betround_preflop
		&& _phandval[BETROUND-1] > _phandval[BETROUND-2])
	{
		_ishandup = true;														
	}

	CalculateHandType(); 

	///////////////////////////////////////////////////////////////////
	// pokervalplayer
	nCards = 0;
	CardMask_RESET(Cards);
	for (int i=0; i<k_number_of_cards_per_player; i++)
	{
		// player cards
		if (p_table_state->User()->HasKnownCards()) {
			CardMask_SET(Cards, p_table_state->User()->_hole_cards[i].GetValue());
			nCards++;
		}
	}
	handval = Hand_EVAL_N(Cards, nCards);

	_pokervalplayer = CalculatePokerval(handval, nCards, &dummy, CARD_NOCARD, CARD_NOCARD);


	///////////////////////////////////////////////////////////////////
	// pokervalcommon
	nCards = 0;
	CardMask_RESET(Cards);
	for (int i=0; i<k_number_of_community_cards; i++)
	{
		// common cards
    Card card = p_table_state->_common_cards[i];
    if (card.IsKnownCard())
		{
      CardMask_SET(Cards, card.GetValue());
			nCards++;
		}
	}
	handval = Hand_EVAL_N(Cards, nCards);

	_pokervalcommon = CalculatePokerval(handval, nCards, &dummy, CARD_NOCARD, CARD_NOCARD); 

		_chandval[BETROUND-1] = _pokervalcommon & 0xff000000; 
		if (BETROUND > k_betround_preflop 
			&& _chandval[BETROUND-1] > _chandval[BETROUND-2])
		{
			_ishandupcommon = true;
		}
}


void CSymbolEnginePokerval::CalculateHandType()
{
	_hand_type = HandVal_HANDTYPE(handval);
	write_log(preferences.debug_symbolengine(), "[CSymbolEnginePokerval::CalculateHandType] handval = %i\n", handval);
	write_log(preferences.debug_symbolengine(), "[CSymbolEnginePokerval::CalculateHandType] handtype = %i\n", _hand_type);

	if  (isstraightflush() 
		&& StdDeck_RANK(HandVal_TOP_CARD(handval)) == Rank_ACE)
	{
		_isroyalflush = true;
	}
	else if (isflush())
	{
		if (StdDeck_RANK(HandVal_TOP_CARD(handval)) == 12 && (_pcbits & 0x10))
		{
			_ishiflush = true;
		}
		// If we have name pocket King, and it's the second best card in our flush
		else if (StdDeck_RANK(HandVal_SECOND_CARD(handval)) == 11 && (_pcbits & 0x8))
		{
			_ishiflush = true;
		}
		// If we have name pocket Queen, and it's the third best card in our flush
		else if (StdDeck_RANK(HandVal_THIRD_CARD(handval)) == 10 && (_pcbits & 0x4))
		{
			_ishiflush = true;
		}
		// If we have name pocket Jack, and it's the fourth best card in our flush
		else if (StdDeck_RANK(HandVal_FOURTH_CARD(handval)) == 9 && (_pcbits & 0x2))
		{
			_ishiflush = true;
		}																	
	}
	else if (isstraight())
	{							
		if (!IsHigherStraightPossible(handval))
		{
			_ishistraight = true;
		}
	}
	else if (HandVal_HANDTYPE(handval) == HandType_ONEPAIR)
	{
		// hi lo mid pair
		// Only user cards...
		if (nCards == 2)
		{
			_ishipair = true;
		}
		// at least flop seen...
		else if (nCards >= 5)
		{
			if ((int)(StdDeck_RANK(HandVal_TOP_CARD(handval))) 
				>= OpenHoldemRankToStdDeckLibraryRank(rankhicommon()))
			{
				_ishipair = true;										
			}
			else if ((int)(StdDeck_RANK(HandVal_TOP_CARD(handval))) 
					< OpenHoldemRankToStdDeckLibraryRank(rankhicommon())
				&& (int)(StdDeck_RANK(HandVal_TOP_CARD(handval))) 
					> OpenHoldemRankToStdDeckLibraryRank(ranklocommon()))
			{
				_ismidpair = true;
			}
			else
			{
				_islopair = true;
			}
		}
	}
}

CString CSymbolEnginePokerval::HandType()
{
	if (ishicard())
	{ 
		return "hcard"; 
	}
	else if (isonepair())
	{ 
		return "1pair"; 
	}
	else if (istwopair())
	{ 
		return "2pair"; 
	}
	else if (isthreeofakind())
	{ 
		return "3kind"; 
	}
	else if (isstraight())
	{ 
		return "strai"; 
	}
	else if (isflush())
	{ 
		return "flush"; 
	}
	else if (isfullhouse())
	{ 
		return "fullh"; 
	}
	else if (isfourofakind())
	{ 
		return "4kind"; 
	}
	else if (isstraightflush())
	{ 
		return "strfl"; 
	}
	else if (isroyalflush())
	{ 
		return "royfl"; 
	}
	else
	{
		assert(false);
		return "err";
	}
}

void CSymbolEnginePokerval::CalculateRankBits()
{
	int				_rank = 0, suit = 0, plcomsuit = 0, comsuit = 0;
	CardMask		plCards = {0}, comCards = {0}, plcomCards = {0};

	CardMask_RESET(plCards);
	CardMask_RESET(comCards);
	CardMask_RESET(plcomCards);

	_rankbitsplayer  = 0;
	_rankbitscommon  = 0;
	_rankbitspoker   = 0;
	_srankbitsplayer = 0;
	_srankbitscommon = 0;
	_srankbitspoker  = 0;

	int tsuitcommon = SYM->p_symbol_engine_cards()->tsuitcommon();
	comsuit = (tsuitcommon==OH_SUIT_CLUBS ? Suit_CLUBS :
		tsuitcommon==OH_SUIT_DIAMONDS ? Suit_DIAMONDS :
		tsuitcommon==OH_SUIT_HEARTS ? Suit_HEARTS :
		tsuitcommon==OH_SUIT_SPADES ? Suit_SPADES : 0);

	int tsuit = SYM->p_symbol_engine_cards()->tsuit();
	plcomsuit = (tsuit==OH_SUIT_CLUBS ? Suit_CLUBS :
		tsuit==OH_SUIT_DIAMONDS ? Suit_DIAMONDS :
		tsuit==OH_SUIT_HEARTS ? Suit_HEARTS :
		tsuit==OH_SUIT_SPADES ? Suit_SPADES : 0);

	// player cards
	for (int i=0; i<k_number_of_cards_per_player; i++) {
		if (p_table_state->User()->HasKnownCards()) {
      int card = p_table_state->User()->_hole_cards[i].GetValue();
			CardMask_SET(plCards, card);
			CardMask_SET(plcomCards, card);
		}
	}

	// common cards
	for (int i=0; i<k_number_of_community_cards; i++) {
    Card card = p_table_state->_common_cards[i];
    if (card.IsKnownCard())
		{
      CardMask_SET(comCards, card.GetValue());
			CardMask_SET(plcomCards, card.GetValue());
		}
	}

	for (suit=StdDeck_Suit_FIRST; suit<=StdDeck_Suit_LAST; suit++)
	{
		for (_rank=StdDeck_Rank_LAST; _rank>=StdDeck_Rank_FIRST; _rank--)
		{
			if (CardMask_CARD_IS_SET(plCards, StdDeck_MAKE_CARD(_rank, suit)))
			{
				SetRankBit(&_rankbitsplayer, _rank);
			}
			if (CardMask_CARD_IS_SET(comCards, StdDeck_MAKE_CARD(_rank, suit)))
			{
				SetRankBit(&_rankbitscommon, _rank);
			}
		}
	}
	_rankbitspoker = (1<<((_pokerval>>16)&0xf))
		| (1<<((_pokerval>>12)&0xf)) 
		| (1<<((_pokerval>>8)&0xf)) 
		| (1<<((_pokerval>>4)&0xf)) 
		| (1<<((_pokerval>>0)&0xf)); 
	// Take care about ace (low bit)
	_rankbitspoker = _rankbitspoker + ((_rankbitspoker&0x4000) ? (1<<1) : 0); 

	for (_rank=StdDeck_Rank_LAST; _rank>=StdDeck_Rank_FIRST; _rank--)
	{
		if (CardMask_CARD_IS_SET(plCards, StdDeck_MAKE_CARD(_rank, plcomsuit)))
		{
			SetRankBit(&_srankbitsplayer, _rank);
		}
		if (CardMask_CARD_IS_SET(comCards, StdDeck_MAKE_CARD(_rank, comsuit)))
		{
			SetRankBit(&_srankbitscommon, _rank);
		}
	}

	_srankbitspoker =														
			(CardMask_CARD_IS_SET(plcomCards, StdDeck_MAKE_CARD(((_pokerval>>16)&0xf)-2, plcomsuit)) ?
			 (1<<((_pokerval>>16)&0xf)) : 0) |
			(CardMask_CARD_IS_SET(plcomCards, StdDeck_MAKE_CARD(((_pokerval>>12)&0xf)-2, plcomsuit)) ?
			 (1<<((_pokerval>>12)&0xf)) : 0) |
			(CardMask_CARD_IS_SET(plcomCards, StdDeck_MAKE_CARD(((_pokerval>>8)&0xf)-2, plcomsuit)) ?
			 (1<<((_pokerval>>8)&0xf)) : 0) |
			(CardMask_CARD_IS_SET(plcomCards, StdDeck_MAKE_CARD(((_pokerval>>4)&0xf)-2, plcomsuit)) ?
			 (1<<((_pokerval>>4)&0xf)) : 0) |
			(CardMask_CARD_IS_SET(plcomCards, StdDeck_MAKE_CARD(((_pokerval>>0)&0xf)-2, plcomsuit)) ?
		 (1<<((_pokerval>>0)&0xf)) : 0);
	_srankbitspoker += ((_srankbitspoker&0x4000) ? (1<<1) : 0); //ace	

	write_log(preferences.debug_symbolengine(), "[CSymbolEnginePokerval::CalculateHandType] rankbitsplayer  = %i\n", _rankbitsplayer);
	write_log(preferences.debug_symbolengine(), "[CSymbolEnginePokerval::CalculateHandType] rankbitscommon  = %i\n", _rankbitscommon);
	write_log(preferences.debug_symbolengine(), "[CSymbolEnginePokerval::CalculateHandType] rankbitspoker   = %i\n", _rankbitspoker);
	write_log(preferences.debug_symbolengine(), "[CSymbolEnginePokerval::CalculateHandType] srankbitsplayer = %i\n", _srankbitsplayer);
	write_log(preferences.debug_symbolengine(), "[CSymbolEnginePokerval::CalculateHandType] srankbitscommon = %i\n", _srankbitscommon);
	write_log(preferences.debug_symbolengine(), "[CSymbolEnginePokerval::CalculateHandType] srankbitspoker  = %i\n", _srankbitspoker);

	AssertRange(_rankbitsplayer,  0, k_rankbits_all_cards_111_111_111_111_110);
	AssertRange(_rankbitscommon,  0, k_rankbits_all_cards_111_111_111_111_110); 
	AssertRange(_rankbitspoker,   0, k_rankbits_all_cards_111_111_111_111_110);  
	AssertRange(_srankbitsplayer, 0, k_rankbits_all_cards_111_111_111_111_110);  
	AssertRange(_srankbitscommon, 0, k_rankbits_all_cards_111_111_111_111_110); 
	AssertRange(_srankbitspoker,  0, k_rankbits_all_cards_111_111_111_111_110); 	
}


int CSymbolEnginePokerval::GetRankHi(int rankbits)
{
	for (int i=k_rank_ace; i>=2; i--)
	{
		if (IsBitSet(rankbits, i))
		{
			return i;
		}
	}
	return 0;
}

int CSymbolEnginePokerval::GetRankLo(int rankbits)
{
	for (int i=2; i<=k_rank_ace; i++)
	{
		if (IsBitSet(rankbits, i))
		{
			return i;
		}
	}
	return 0;
}

void CSymbolEnginePokerval::SetRankBit(int* rankbits, int rank)
{
	*rankbits |= (1 << (rank + 2));		
	if (rank == Rank_ACE)
	{
		*rankbits |= (1<<1);
	}
}

bool CSymbolEnginePokerval::IsHigherStraightPossible(HandVal handval)
{
	assert((HandVal_HANDTYPE(handval) == HandType_STRAIGHT));
	if (StdDeck_RANK(HandVal_TOP_CARD(handval)) == StdDeck_Rank_ACE)
	{
		return false;												
	}
	// Otherwise:
	// Check for all higher 5-card-straight,
	// if only 2 or less are missing
	int n_shift = 10;
	int _rank = 0, suit = 0;
	unsigned int _rankbits_common = 0;

	CardMask      comCards = {0};
	CardMask_RESET(comCards);

	// common cards
	for (int i=0; i<k_number_of_community_cards; i++)
	{
    Card card = p_table_state->_common_cards[i];
    if (card.IsKnownCard())
		{
      CardMask_SET(comCards, card.GetValue());
		}
	}
	for (suit=StdDeck_Suit_FIRST; suit<=StdDeck_Suit_LAST; suit++)
	{
		for (_rank=StdDeck_Rank_LAST; _rank>=StdDeck_Rank_FIRST; _rank--)
		{
			if (CardMask_CARD_IS_SET(comCards, StdDeck_MAKE_CARD(_rank, suit)))
			{
				_rankbits_common |= (1<<(_rank+2));
				if (_rank == Rank_ACE)
				{
					_rankbits_common |= (1<<1);
				}
			}
		}
	}
	int current_top_end = StdDeck_RANK(HandVal_TOP_CARD(handval))+2;

	for (int k=14; k>current_top_end; k--)
	{   	
		if (bitcount((_rankbits_common>>n_shift)&0x1f) >= 3)
			return true;

		n_shift--;
	}
	return false;
}

int CSymbolEnginePokerval::CalculatePokerval(HandVal hv, int n, int *pcb, int card0, int card1)
{
	double			pv = 0.;
	int				i = 0, j = 0, k = 0, max = 0, c = 0, flush_suit = 0; //Matrix 2008-06-28
	int				bits = 0;
	CardMask		Cards = {0}, heartsCards = {0}, diamondsCards = {0}, clubsCards = {0}, spadesCards = {0}, suittestCards = {0};

	// If we have name straight flush or flush, figure out the suit
	flush_suit = -1;
	if (HandVal_HANDTYPE(hv)==HandType_STFLUSH || HandVal_HANDTYPE(hv)==HandType_FLUSH)
	{
		// Set up some suit masks
		CardMask_RESET(heartsCards);
		CardMask_SET(heartsCards, StdDeck_MAKE_CARD(Rank_2, Suit_HEARTS));
		CardMask_SET(heartsCards, StdDeck_MAKE_CARD(Rank_3, Suit_HEARTS));
		CardMask_SET(heartsCards, StdDeck_MAKE_CARD(Rank_4, Suit_HEARTS));
		CardMask_SET(heartsCards, StdDeck_MAKE_CARD(Rank_5, Suit_HEARTS));
		CardMask_SET(heartsCards, StdDeck_MAKE_CARD(Rank_6, Suit_HEARTS));
		CardMask_SET(heartsCards, StdDeck_MAKE_CARD(Rank_7, Suit_HEARTS));
		CardMask_SET(heartsCards, StdDeck_MAKE_CARD(Rank_8, Suit_HEARTS));
		CardMask_SET(heartsCards, StdDeck_MAKE_CARD(Rank_9, Suit_HEARTS));
		CardMask_SET(heartsCards, StdDeck_MAKE_CARD(Rank_TEN, Suit_HEARTS));
		CardMask_SET(heartsCards, StdDeck_MAKE_CARD(Rank_JACK, Suit_HEARTS));
		CardMask_SET(heartsCards, StdDeck_MAKE_CARD(Rank_QUEEN, Suit_HEARTS));
		CardMask_SET(heartsCards, StdDeck_MAKE_CARD(Rank_KING, Suit_HEARTS));
		CardMask_SET(heartsCards, StdDeck_MAKE_CARD(Rank_ACE, Suit_HEARTS));

		CardMask_RESET(diamondsCards);
		CardMask_SET(diamondsCards, StdDeck_MAKE_CARD(Rank_2, Suit_DIAMONDS));
		CardMask_SET(diamondsCards, StdDeck_MAKE_CARD(Rank_3, Suit_DIAMONDS));
		CardMask_SET(diamondsCards, StdDeck_MAKE_CARD(Rank_4, Suit_DIAMONDS));
		CardMask_SET(diamondsCards, StdDeck_MAKE_CARD(Rank_5, Suit_DIAMONDS));
		CardMask_SET(diamondsCards, StdDeck_MAKE_CARD(Rank_6, Suit_DIAMONDS));
		CardMask_SET(diamondsCards, StdDeck_MAKE_CARD(Rank_7, Suit_DIAMONDS));
		CardMask_SET(diamondsCards, StdDeck_MAKE_CARD(Rank_8, Suit_DIAMONDS));
		CardMask_SET(diamondsCards, StdDeck_MAKE_CARD(Rank_9, Suit_DIAMONDS));
		CardMask_SET(diamondsCards, StdDeck_MAKE_CARD(Rank_TEN, Suit_DIAMONDS));
		CardMask_SET(diamondsCards, StdDeck_MAKE_CARD(Rank_JACK, Suit_DIAMONDS));
		CardMask_SET(diamondsCards, StdDeck_MAKE_CARD(Rank_QUEEN, Suit_DIAMONDS));
		CardMask_SET(diamondsCards, StdDeck_MAKE_CARD(Rank_KING, Suit_DIAMONDS));
		CardMask_SET(diamondsCards, StdDeck_MAKE_CARD(Rank_ACE, Suit_DIAMONDS));

		CardMask_RESET(spadesCards);
		CardMask_SET(spadesCards, StdDeck_MAKE_CARD(Rank_2, Suit_SPADES));
		CardMask_SET(spadesCards, StdDeck_MAKE_CARD(Rank_3, Suit_SPADES));
		CardMask_SET(spadesCards, StdDeck_MAKE_CARD(Rank_4, Suit_SPADES));
		CardMask_SET(spadesCards, StdDeck_MAKE_CARD(Rank_5, Suit_SPADES));
		CardMask_SET(spadesCards, StdDeck_MAKE_CARD(Rank_6, Suit_SPADES));
		CardMask_SET(spadesCards, StdDeck_MAKE_CARD(Rank_7, Suit_SPADES));
		CardMask_SET(spadesCards, StdDeck_MAKE_CARD(Rank_8, Suit_SPADES));
		CardMask_SET(spadesCards, StdDeck_MAKE_CARD(Rank_9, Suit_SPADES));
		CardMask_SET(spadesCards, StdDeck_MAKE_CARD(Rank_TEN, Suit_SPADES));
		CardMask_SET(spadesCards, StdDeck_MAKE_CARD(Rank_JACK, Suit_SPADES));
		CardMask_SET(spadesCards, StdDeck_MAKE_CARD(Rank_QUEEN, Suit_SPADES));
		CardMask_SET(spadesCards, StdDeck_MAKE_CARD(Rank_KING, Suit_SPADES));
		CardMask_SET(spadesCards, StdDeck_MAKE_CARD(Rank_ACE, Suit_SPADES));

		CardMask_RESET(clubsCards);
		CardMask_SET(clubsCards, StdDeck_MAKE_CARD(Rank_2, Suit_CLUBS));
		CardMask_SET(clubsCards, StdDeck_MAKE_CARD(Rank_3, Suit_CLUBS));
		CardMask_SET(clubsCards, StdDeck_MAKE_CARD(Rank_4, Suit_CLUBS));
		CardMask_SET(clubsCards, StdDeck_MAKE_CARD(Rank_5, Suit_CLUBS));
		CardMask_SET(clubsCards, StdDeck_MAKE_CARD(Rank_6, Suit_CLUBS));
		CardMask_SET(clubsCards, StdDeck_MAKE_CARD(Rank_7, Suit_CLUBS));
		CardMask_SET(clubsCards, StdDeck_MAKE_CARD(Rank_8, Suit_CLUBS));
		CardMask_SET(clubsCards, StdDeck_MAKE_CARD(Rank_9, Suit_CLUBS));
		CardMask_SET(clubsCards, StdDeck_MAKE_CARD(Rank_TEN, Suit_CLUBS));
		CardMask_SET(clubsCards, StdDeck_MAKE_CARD(Rank_JACK, Suit_CLUBS));
		CardMask_SET(clubsCards, StdDeck_MAKE_CARD(Rank_QUEEN, Suit_CLUBS));
		CardMask_SET(clubsCards, StdDeck_MAKE_CARD(Rank_KING, Suit_CLUBS));
		CardMask_SET(clubsCards, StdDeck_MAKE_CARD(Rank_ACE, Suit_CLUBS));


		CardMask_RESET(Cards);
		for (int i=0; i<k_number_of_cards_per_player; i++)
		{
			if (p_table_state->User()->HasKnownCards())
			{
        CardMask_SET(Cards, p_table_state->User()->_hole_cards[i].GetValue());
			}
		}

		for (int i=0; i<k_number_of_community_cards; i++)
		{
      Card card = p_table_state->_common_cards[i];
      if (card.IsKnownCard()) {
        CardMask_SET(Cards, card.GetValue());
			}
		}

		max = 0;
		CardMask_AND(suittestCards, Cards, spadesCards);
		c = StdDeck_numCards(suittestCards);
		if ( c>max && c>0)
		{
			max = c;
			flush_suit = Suit_SPADES;
		}
		CardMask_AND(suittestCards, Cards, heartsCards);
		c = StdDeck_numCards(suittestCards);
		if ( c>max && c>0)
		{
			max = c;
			flush_suit = Suit_HEARTS;
		}
		CardMask_AND(suittestCards, Cards, diamondsCards);
		c = StdDeck_numCards(suittestCards);
		if ( c>max && c>0)
		{
			max = c;
			flush_suit = Suit_DIAMONDS;
		}
		CardMask_AND(suittestCards, Cards, clubsCards);
		c = StdDeck_numCards(suittestCards);
		if ( c>max && c>0)
		{
			max = c;
			flush_suit = Suit_CLUBS;
		}
	}

	// Now figure out the pokerval and pcbits
	switch HandVal_HANDTYPE(hv)
	{
	case HandType_STFLUSH:
		pv += k_pokerval_straightflush;
		pv += (HandVal_TOP_CARD(hv)+2-0)<<16;
		pv += (HandVal_TOP_CARD(hv)+2-1)<<12;
		pv += (HandVal_TOP_CARD(hv)+2-2)<<8;
		pv += (HandVal_TOP_CARD(hv)+2-3)<<4;
		pv += (HandVal_TOP_CARD(hv)+2-4)<<0;

		for (int i=0; i<k_number_of_community_cards; i++)
		{
			j = StdDeck_RANK(card0);	//Matrix 2008-06-28
			k = StdDeck_RANK(card1);
			if ( (j == HandVal_TOP_CARD(hv)-i &&
					StdDeck_SUIT(card0) == flush_suit)
					||
					k == HandVal_TOP_CARD(hv)-i &&
					StdDeck_SUIT(card1) == flush_suit)
			{
				bits |= (1<<(4-i));
			}
			// In straight evaluation an Ace can appear in hv as either 0x0c or 0xff. 
			// We need to do an ugly test for both cases.

			if ((j == 12) || (k == 12))  //Matrix 2008-10-14 !!KLUDGE ALERT!!
			{
				if (j == 12) 
				{
					j=-1;
				}
				if(k == 12)
				{
					k=-1;
				}
				if ((j == HandVal_TOP_CARD(hv)-i && StdDeck_SUIT(card0) == flush_suit)
					||	k == HandVal_TOP_CARD(hv)-i && StdDeck_SUIT(card1) == flush_suit)
				{
					bits |= (1<<(4-i));
				}
			}
		}
		break;

	case HandType_QUADS:
		pv += k_pokerval_fourofakind;
		pv += (HandVal_TOP_CARD(hv)+2)<<16;
		pv += (HandVal_TOP_CARD(hv)+2)<<12;
		pv += (HandVal_TOP_CARD(hv)+2)<<8;
		pv += (HandVal_TOP_CARD(hv)+2)<<4;
		if (n>=5)  pv += (HandVal_SECOND_CARD(hv)+2)<<0;

		// Either hole card is used in quads
		if ( (StdDeck_RANK(card0) == HandVal_TOP_CARD(hv)) !=
				(StdDeck_RANK(card1) == HandVal_TOP_CARD(hv)) )
		{
			bits |= (1<<4);
		}
		// Both hole cards are used in quads
		else if (StdDeck_RANK(card0) == HandVal_TOP_CARD(hv) &&
				 StdDeck_RANK(card1) == HandVal_TOP_CARD(hv))
		{
			bits |= (1<<4);
			bits |= (1<<3);
		}
		// Either hole card is used as kicker
		if (n>=5 &&
				(StdDeck_RANK(card0) == HandVal_SECOND_CARD(hv) ||
				 StdDeck_RANK(card1) == HandVal_SECOND_CARD(hv)))
		{
			bits |= (1<<0);
		}
		break;

	case HandType_FULLHOUSE:
		pv += k_pokerval_fullhouse;
		pv += (HandVal_TOP_CARD(hv)+2)<<16;
		pv += (HandVal_TOP_CARD(hv)+2)<<12;
		pv += (HandVal_TOP_CARD(hv)+2)<<8;
		pv += (HandVal_SECOND_CARD(hv)+2)<<4;
		pv += (HandVal_SECOND_CARD(hv)+2)<<0;

		// Either hole card is used in top of boat
		if ( (StdDeck_RANK(card0) == HandVal_TOP_CARD(hv)) !=
				(StdDeck_RANK(card1) == HandVal_TOP_CARD(hv)) )
		{
			bits |= (1<<4);
		}
		// Both hole cards are used in top of boat
		else if (StdDeck_RANK(card0) == HandVal_TOP_CARD(hv) &&
				 StdDeck_RANK(card1) == HandVal_TOP_CARD(hv))
		{
			bits |= (1<<4);
			bits |= (1<<3);
		}
		// Either hole card is used in bottom of boat
		if ( (StdDeck_RANK(card0) == HandVal_SECOND_CARD(hv))  !=
				(StdDeck_RANK(card1) == HandVal_SECOND_CARD(hv)) )
		{
			bits |= (1<<1);
		}
		// Both hole cards are used in bottom of boat
		else if (StdDeck_RANK(card0) == HandVal_SECOND_CARD(hv) &&
				 StdDeck_RANK(card1) == HandVal_SECOND_CARD(hv))
		{
			bits |= (1<<1);
			bits |= (1<<0);
		}
		break;

	case HandType_FLUSH:
		pv += k_pokerval_flush;
		pv += (HandVal_TOP_CARD(hv)+2)<<16;
		pv += (HandVal_SECOND_CARD(hv)+2)<<12;
		pv += (HandVal_THIRD_CARD(hv)+2)<<8;
		pv += (HandVal_FOURTH_CARD(hv)+2)<<4;
		pv += (HandVal_FIFTH_CARD(hv)+2)<<0;

		if ( (StdDeck_RANK(card0) == HandVal_TOP_CARD(hv) &&
				StdDeck_SUIT(card0) == flush_suit)
				||
				(StdDeck_RANK(card1) == HandVal_TOP_CARD(hv) &&
				 StdDeck_SUIT(card1) == flush_suit) )
		{
			bits |= (1<<4);
		}
		if ( (StdDeck_RANK(card0) == HandVal_SECOND_CARD(hv) &&
				StdDeck_SUIT(card0) == flush_suit)
				||
				(StdDeck_RANK(card1) == HandVal_SECOND_CARD(hv) &&
				 StdDeck_SUIT(card1) == flush_suit) )
		{
			bits |= (1<<3);
		}
		if ( (StdDeck_RANK(card0) == HandVal_THIRD_CARD(hv) &&
				StdDeck_SUIT(card0) == flush_suit)
				||
				(StdDeck_RANK(card1) == HandVal_THIRD_CARD(hv) &&
				 StdDeck_SUIT(card1) == flush_suit) )
		{
			bits |= (1<<2);
		}
		if ( (StdDeck_RANK(card0) == HandVal_FOURTH_CARD(hv) &&
				StdDeck_SUIT(card0) == flush_suit)
				||
				(StdDeck_RANK(card1) == HandVal_FOURTH_CARD(hv) &&
				 StdDeck_SUIT(card1) == flush_suit) )
		{
			bits |= (1<<1);
		}
		if ( (StdDeck_RANK(card0) == HandVal_FIFTH_CARD(hv) &&
				StdDeck_SUIT(card0) == flush_suit)
				||
				(StdDeck_RANK(card1) == HandVal_FIFTH_CARD(hv) &&
				 StdDeck_SUIT(card1) == flush_suit) )
		{
			bits |= (1<<0);
		}
		break;

	case HandType_STRAIGHT:
		pv += k_pokerval_straight;
		pv += (HandVal_TOP_CARD(hv)+2-0)<<16;
		pv += (HandVal_TOP_CARD(hv)+2-1)<<12;
		pv += (HandVal_TOP_CARD(hv)+2-2)<<8;
		pv += (HandVal_TOP_CARD(hv)+2-3)<<4;
		pv += (HandVal_TOP_CARD(hv)+2-4)<<0;
		for (int i=0; i<k_number_of_community_cards; i++)
		{
			j = StdDeck_RANK(card0);	//Matrix 2008-06-28
			k = StdDeck_RANK(card1);
			if (j == HandVal_TOP_CARD(hv)-i ||
					k == HandVal_TOP_CARD(hv)-i)
			{
				bits |= (1<<(4-i));
			}
			if((j==12)||(k==12))  //Matrix 2008-10-14 !!KLUDGE ALERT!!
			{
				if(j==12)j=-1;
				if(k==12)k=-1;
				if (j == HandVal_TOP_CARD(hv)-i ||
					k == HandVal_TOP_CARD(hv)-i)
					{
						bits |= (1<<(4-i));
					}
			}

		}
		break;

	case HandType_TRIPS:
		pv += k_pokerval_threeofakind;
		pv += (HandVal_TOP_CARD(hv)+2)<<16;
		pv += (HandVal_TOP_CARD(hv)+2)<<12;
		pv += (HandVal_TOP_CARD(hv)+2)<<8;
		if (n>=4)  pv += (HandVal_SECOND_CARD(hv)+2)<<4;
		if (n>=5)  pv += (HandVal_THIRD_CARD(hv)+2)<<0;

		// either hole card used in trips
		if ( (StdDeck_RANK(card0) == HandVal_TOP_CARD(hv)) !=
				(StdDeck_RANK(card1) == HandVal_TOP_CARD(hv)) )
		{
			bits |= (1<<4);
		}
		// both hole cards used in trips
		else if (StdDeck_RANK(card0) == HandVal_TOP_CARD(hv) &&
				 StdDeck_RANK(card1) == HandVal_TOP_CARD(hv))
		{
			bits |= (1<<4);
			bits |= (1<<3);
		}
		// either hole card is first kicker
		if (n>=4 &&
				(StdDeck_RANK(card0) == HandVal_SECOND_CARD(hv) ||
				 StdDeck_RANK(card1) == HandVal_SECOND_CARD(hv)))
		{
			bits |= (1<<1);
		}
		// either hole card is second kicker
		if (n>=5 &&
				(StdDeck_RANK(card0) == HandVal_THIRD_CARD(hv) ||
				 StdDeck_RANK(card1) == HandVal_THIRD_CARD(hv)))
		{
			bits |= (1<<1);
		}
		break;

	case HandType_TWOPAIR:
		pv += k_pokerval_twopair;
		pv += (HandVal_TOP_CARD(hv)+2)<<16;
		pv += (HandVal_TOP_CARD(hv)+2)<<12;
		pv += (HandVal_SECOND_CARD(hv)+2)<<8;
		pv += (HandVal_SECOND_CARD(hv)+2)<<4;
		if (n>=5)  pv += (HandVal_THIRD_CARD(hv)+2)<<0;

		// either hole card used in top pair
		if ( (StdDeck_RANK(card0) == HandVal_TOP_CARD(hv)) !=
				(StdDeck_RANK(card1) == HandVal_TOP_CARD(hv)) )
		{
			bits |= (1<<4);
		}
		// both hole cards used in top pair
		else if (StdDeck_RANK(card0) == HandVal_TOP_CARD(hv) &&
				 StdDeck_RANK(card1) == HandVal_TOP_CARD(hv))
		{
			bits |= (1<<4);
			bits |= (1<<3);
		}
		// either hole card used in bottom pair
		if ( (StdDeck_RANK(card0) == HandVal_SECOND_CARD(hv)) !=
				(StdDeck_RANK(card1) == HandVal_SECOND_CARD(hv)) )
		{
			bits |= (1<<2);
		}
		// both hole cards used in bottom pair
		else if (StdDeck_RANK(card0) == HandVal_SECOND_CARD(hv) &&
				 StdDeck_RANK(card1) == HandVal_SECOND_CARD(hv))
		{
			bits |= (1<<2);
			bits |= (1<<1);
		}
		// either hole card kicker
		if (n>=5 &&
				(StdDeck_RANK(card0) == HandVal_THIRD_CARD(hv) ||
				 StdDeck_RANK(card1) == HandVal_THIRD_CARD(hv)))
		{
			bits |= (1<<0);
		}
		break;

	case HandType_ONEPAIR:
		pv += k_pokerval_onepair;
		pv += (HandVal_TOP_CARD(hv)+2)<<16;
		pv += (HandVal_TOP_CARD(hv)+2)<<12;
		if (n>=3)  pv += (HandVal_SECOND_CARD(hv)+2)<<8;
		if (n>=4)  pv += (HandVal_THIRD_CARD(hv)+2)<<4;
		if (n>=5)  pv += (HandVal_FOURTH_CARD(hv)+2)<<0;

		// either hole card used in pair
		if ( (StdDeck_RANK(card0) == StdDeck_RANK(HandVal_TOP_CARD(hv))) !=
				(StdDeck_RANK(card1) == StdDeck_RANK(HandVal_TOP_CARD(hv))) )
		{
			bits |= (1<<4);
		}
		// both hole cards used in pair
		else if (StdDeck_RANK(card0) == StdDeck_RANK(HandVal_TOP_CARD(hv)) &&
				 StdDeck_RANK(card1) == StdDeck_RANK(HandVal_TOP_CARD(hv)))
		{
			bits |= (1<<4);
			bits |= (1<<3);
		}
		// either hole card used as first kicker
		if (n>=3 &&
				(StdDeck_RANK(card0) == HandVal_SECOND_CARD(hv) ||
				 StdDeck_RANK(card1) == HandVal_SECOND_CARD(hv)))
		{
			bits |= (1<<2);
		}
		// either hole card used as second kicker
		if (n>=4 &&
				(StdDeck_RANK(card0) == HandVal_THIRD_CARD(hv) ||
				 StdDeck_RANK(card1) == HandVal_THIRD_CARD(hv)))
		{
			bits |= (1<<1);
		}
		// either hole card used as third kicker
		if (n>=5 &&
				(StdDeck_RANK(card0) == HandVal_FOURTH_CARD(hv) ||
				 StdDeck_RANK(card1) == HandVal_FOURTH_CARD(hv)))
		{
			bits |= (1<<0);
		}
		break;

	case HandType_NOPAIR:
		if (n>=1)  pv += (HandVal_TOP_CARD(hv)+2)<<16;
		if (n>=2)  pv += (HandVal_SECOND_CARD(hv)+2)<<12;
		if (n>=3)  pv += (HandVal_THIRD_CARD(hv)+2)<<8;
		if (n>=4)  pv += (HandVal_FOURTH_CARD(hv)+2)<<4;
		if (n>=5)  pv += (HandVal_FIFTH_CARD(hv)+2)<<0;

		// either hole card used as top card
		if (n>=1 &&
				(StdDeck_RANK(card0) == HandVal_TOP_CARD(hv) ||
				 StdDeck_RANK(card1) == HandVal_TOP_CARD(hv) ))
		{
			bits |= (1<<4);
		}
		// either hole card used as first kicker
		if (n>=2 &&
				(StdDeck_RANK(card0) == HandVal_SECOND_CARD(hv) ||
				 StdDeck_RANK(card1) == HandVal_SECOND_CARD(hv) ))
		{
			bits |= (1<<3);
		}
		// either hole card used as second kicker
		if (n>=3 &&
				(StdDeck_RANK(card0) == HandVal_THIRD_CARD(hv) ||
				 StdDeck_RANK(card1) == HandVal_THIRD_CARD(hv) ))
		{
			bits |= (1<<2);
		}
		// either hole card used as third kicker
		if (n>=3 &&
				(StdDeck_RANK(card0) == HandVal_FOURTH_CARD(hv) ||
				 StdDeck_RANK(card1) == HandVal_FOURTH_CARD(hv) ))
		{
			bits |= (1<<1);
		}
		// either hole card used as fourth kicker
		if (n>=3 &&
				(StdDeck_RANK(card0) == HandVal_FIFTH_CARD(hv) ||
				 StdDeck_RANK(card1) == HandVal_FIFTH_CARD(hv) ))
		{
			bits |= (1<<0);
		}
		break;
	}

	*pcb = bits;
	return pv;
}

bool CSymbolEnginePokerval::EvaluateSymbol(const char *name, double *result, bool log /* = false */)
{
  FAST_EXIT_ON_OPENPPL_SYMBOLS(name);
	if (memcmp(name, "rankhi", 6) == 0)
	{
		if (memcmp(name, "rankhi", 6)==0 && strlen(name)==6)
		{
			*result = rankhi();
		}
		else if (memcmp(name, "rankhicommon", 12)==0 && strlen(name)==12)
		{
			*result = rankhicommon();
		}
		else if (memcmp(name, "rankhiplayer", 12)==0 && strlen(name)==12)
		{
			*result = rankhiplayer();
		}
		else if (memcmp(name, "rankhipoker", 11)==0 && strlen(name)==11)
		{
			*result = rankhipoker();
		}
		else
		{
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}
	else if (memcmp(name, "srankhi", 7) == 0)
	{
		if (memcmp(name, "srankhi", 7)==0 && strlen(name)==7)		
		{
			*result = srankhi();
		}
		else if (memcmp(name, "srankhicommon", 13)==0 && strlen(name)==13)
		{
			*result = srankhicommon();
		}
		else if (memcmp(name, "srankhiplayer", 13)==0 && strlen(name)==13)	
		{
			*result = srankhiplayer();
		}
		else if (memcmp(name, "srankhipoker", 12)==0 && strlen(name)==12)	
		{
			*result = srankhipoker();
		}
		else
		{
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}
	else if (memcmp(name, "ranklo", 6) == 0)
	{
		if (memcmp(name, "ranklo", 6)==0 && strlen(name)==6)
		{
			*result = ranklo();
		}
		else if (memcmp(name, "ranklocommon", 12)==0 && strlen(name)==12)
		{
			*result = ranklocommon();
		}
		else if (memcmp(name, "rankloplayer", 12)==0 && strlen(name)==12)
		{
			*result = rankloplayer();
		}
		else if (memcmp(name, "ranklopoker", 11)==0 && strlen(name)==11)	
		{
			*result = ranklopoker();
		}
		else
		{
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}
	else if (memcmp(name, "sranklo", 7) == 0)
	{
		if (memcmp(name, "sranklo", 7)==0 && strlen(name)==7)		
		{
			*result = sranklo();
		}
		else if (memcmp(name, "sranklocommon", 13)==0 && strlen(name)==13)	
		{
			*result = sranklocommon();
		}
		else if (memcmp(name, "srankloplayer", 13)==0 && strlen(name)==13)	
		{
			*result = srankloplayer();
		}
		else if (memcmp(name, "sranklopoker", 12)==0 && strlen(name)==12)	
		{
			*result = sranklopoker();
		}
		else 
		{
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}
	else if (memcmp(name, "pokerval", 8)==0)
	{
		if (memcmp(name, "pokerval", 8)==0 && strlen(name)==8)	
		{
			*result = pokerval();
		}
		else if (memcmp(name, "pokervalplayer", 14)==0 && strlen(name)==14)
		{
			*result = pokervalplayer();
		}
		else if (memcmp(name, "pokervalcommon", 14)==0 && strlen(name)==14)
		{
			*result = pokervalcommon();
		}
		else 
		{
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}
	else if (memcmp(name, "rankbits", 8)==0)
	{
		if (memcmp(name, "rankbits", 8)==0 && strlen(name)==8)
		{
			*result = rankbits();
		}
		else if (memcmp(name, "rankbitscommon", 14)==0 && strlen(name)==14)
		{
			*result = rankbitscommon();
		}
		else if (memcmp(name, "rankbitsplayer", 14)==0 && strlen(name)==14)
		{
			*result = rankbitsplayer();
		}
		else if (memcmp(name, "rankbitspoker", 13)==0 && strlen(name)==13)	
		{
			*result = rankbitspoker();
		}
		else 
		{
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}
	else if (memcmp(name, "srankbits", 9)==0)
	{
		if (memcmp(name, "srankbits", 9)==0 && strlen(name)==9)		
		{
			*result = srankbits();
		}
		else if (memcmp(name, "srankbitscommon", 15)==0 && strlen(name)==15)
		{
			*result = srankbitscommon();
		}
		else if (memcmp(name, "srankbitsplayer", 15)==0 && strlen(name)==15)
		{
			*result = srankbitsplayer();
		}
		else if (memcmp(name, "srankbitspoker", 14)==0 && strlen(name)==14)
		{
			*result = srankbitspoker();
		}
		else 
		{
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}
	else if (memcmp(name, "is", 2)==0)
	{
		if (memcmp(name, "ishandup", 8)==0 && strlen(name)==8)
		{
			*result = ishandup(); 
		}
		else if (memcmp(name, "ishandupcommon", 14)==0 && strlen(name)==14)
		{
			*result = ishandupcommon();
		}
		else if (memcmp(name, "ishicard", 8)==0 && strlen(name)==8)	
		{
			*result = ishicard();
		}
		else if (memcmp(name, "isonepair", 9)==0 && strlen(name)==9)	
		{
			*result = isonepair();
		}
		else if (memcmp(name, "istwopair", 9)==0 && strlen(name)==9)		
		{
			*result = istwopair();
		}
		else if (memcmp(name, "isthreeofakind", 14)==0 && strlen(name)==14)
		{
			*result = isthreeofakind();
		}
		else if (memcmp(name, "isstraight", 10)==0 && strlen(name)==10)	
		{
			*result = isstraight();
		}
		else if (memcmp(name, "isflush", 7)==0 && strlen(name)==7)			
		{
			*result = isflush();
		}
		else if (memcmp(name, "isfullhouse", 11)==0 && strlen(name)==11)	
		{
			*result = isfullhouse();
		}
		else if (memcmp(name, "isfourofakind", 13)==0 && strlen(name)==13)	
		{
			*result = isfourofakind();
		}
		else if (memcmp(name, "isstraightflush", 15)==0 && strlen(name)==15)
		{
			*result = isstraightflush();
		}
		else if (memcmp(name, "isroyalflush", 12)==0 && strlen(name)==12)
		{
			*result = isroyalflush();
		}
		else if (memcmp(name, "ishipair", 8)==0 && strlen(name)==8)	
		{
			*result = ishipair();
		}
		else if (memcmp(name, "islopair", 8)==0 && strlen(name)==8)	
		{
			*result = islopair();
		}
		else if (memcmp(name, "ismidpair", 9)==0 && strlen(name)==9)		
		{
			*result = ismidpair();
		}
		else if (memcmp(name, "ishistraight", 12)==0 && strlen(name)==12)
		{
			*result = ishistraight();
		}
		else if (memcmp(name, "ishiflush", 9)==0 && strlen(name)==9)	
		{
			*result = ishiflush();
		}
		else 
		{
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}
	// Various symbols without grouping
	else if (memcmp(name, "pcbits", 6)==0 && strlen(name)==6)	
	{
		*result = pcbits();
	}
	else if (memcmp(name, "npcbits", 7)==0 && strlen(name)==7)		
	{
		*result = npcbits();
	}
	else if (memcmp(name, "hicard", 6)==0 && strlen(name)==6)		
	{
		*result = k_pokerval_hicard;
	}
	else if (memcmp(name, "onepair", 7)==0 && strlen(name)==7)		
	{
		*result = k_pokerval_onepair;
	}
	else if (memcmp(name, "twopair", 7)==0 && strlen(name)==7)			
	{
		*result = k_pokerval_twopair;
	}
	else if (memcmp(name, "threeofakind", 12)==0 && strlen(name)==12)
	{
		*result = k_pokerval_threeofakind;
	}
	else if (memcmp(name, "straight", 8)==0 && strlen(name)==8)	
	{
		*result = k_pokerval_straight;
	}
	else if (memcmp(name, "flush", 5)==0 && strlen(name)==5)		
	{
		*result = k_pokerval_flush;
	}
	else if (memcmp(name, "fullhouse", 9)==0 && strlen(name)==9)	
	{
		*result = k_pokerval_fullhouse;
	}
	else if (memcmp(name, "fourofakind", 11)==0 && strlen(name)==11)	
	{
		*result = k_pokerval_fourofakind;
	}
	else if (memcmp(name, "straightflush", 13)==0 && strlen(name)==13)
	{
		*result = k_pokerval_straightflush;
	}
	else if (memcmp(name, "royalflush", 10)==0 && strlen(name)==10)	
	{
		*result = k_pokerval_royalflush;
	}
	else
	{
		// Symbol of a different symbol-engine
		return false;
	}
	// Valid symbol
	return true;
}

CString CSymbolEnginePokerval::SymbolsProvided() {
  return "rankhi rankhicommon rankhiplayer rankhipoker "
    "srankhi srankhicommon srankhiplayer srankhipoker "
    "ranklo ranklocommon rankloplayer ranklopoker "
    "sranklo sranklocommon srankloplayer sranklopoker "
    "pokerval pokervalplayer pokervalcommon "
    "rankbits rankbitscommon rankbitsplayer rankbitspoker "
    "srankbits srankbitscommon srankbitsplayer srankbitspoker "
    "ishandup ishandupcommon "
    "ishicard isonepair istwopair isthreeofakind isstraight "
    "isflush isfullhouse isfourofakind isstraightflush isroyalflush "
    "ishipair islopair ismidpair "
    "ishistraight ishiflush "
    "pcbits npcbits "
    "hicard onepair twopair threeofakind straight "
    "flush fullhouse fourofakind straightflush royalflush ";
}