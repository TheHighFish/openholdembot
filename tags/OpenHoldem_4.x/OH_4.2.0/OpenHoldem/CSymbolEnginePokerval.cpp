#include "stdafx.h"
#include "CSymbolEnginePokerval.h"

#include <assert.h>
#include "CBetroundCalculator.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CSymbolEngineCards.h"
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

void CSymbolEnginePokerval::ResetOnMyTurn()
{}

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
		if (p_scraper_access->UserHasCards())
		{
			CardMask_SET(Cards, p_scraper->card_player(USER_CHAIR, i));
			nCards++;
		}
	}

	for (int i=0; i<k_number_of_community_cards; i++)
	{
		// common cards
		if (p_scraper->card_common(i) != CARD_BACK &&
			p_scraper->card_common(i) != CARD_NOCARD)
		{
			CardMask_SET(Cards, p_scraper->card_common(i));
			nCards++;
		}
	}

	handval = Hand_EVAL_N(Cards, nCards);

	_pcbits = 0;
	_pokerval = CalculatePokerval(handval, nCards, &_pcbits,				
	  	p_scraper->card_player(USER_CHAIR, 0), 
		p_scraper->card_player(USER_CHAIR, 1));

	write_log(prefs.debug_symbolengine(), "[CSymbolEnginePokerval] handval = %i\n", handval);
	write_log(prefs.debug_symbolengine(), "[CSymbolEnginePokerval] pokerval = %i\n", _pokerval);
	write_log(prefs.debug_symbolengine(), "[CSymbolEnginePokerval] nCards = %i\n", nCards);
	write_log(prefs.debug_symbolengine(), "[CSymbolEnginePokerval] pcbits = %i\n", _pcbits);

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
		if (p_scraper->card_player(USER_CHAIR, i) != CARD_BACK 
			&& p_scraper->card_player(USER_CHAIR, i) != CARD_NOCARD 
			&& !CardMask_CARD_IS_SET(Cards, p_scraper->card_player(USER_CHAIR, i)) )
		{
			CardMask_SET(Cards, p_scraper->card_player(USER_CHAIR, i));
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
		if (p_scraper->card_common(i) != CARD_BACK 
			&& p_scraper->card_common(i) != CARD_NOCARD 
			&& !CardMask_CARD_IS_SET(Cards, p_scraper->card_common(i)) )
		{
			CardMask_SET(Cards, p_scraper->card_common(i));
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
	write_log(prefs.debug_symbolengine(), "[CSymbolEnginePokerval::CalculateHandType] handval = %i\n", handval);
	write_log(prefs.debug_symbolengine(), "[CSymbolEnginePokerval::CalculateHandType] handtype = %i\n", _hand_type);

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
		// If we have a pocket King, and it's the second best card in our flush
		else if (StdDeck_RANK(HandVal_SECOND_CARD(handval)) == 11 && (_pcbits & 0x8))
		{
			_ishiflush = true;
		}
		// If we have a pocket Queen, and it's the third best card in our flush
		else if (StdDeck_RANK(HandVal_THIRD_CARD(handval)) == 10 && (_pcbits & 0x4))
		{
			_ishiflush = true;
		}
		// If we have a pocket Jack, and it's the fourth best card in our flush
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
		// hi lo med pair
		// Only user cards...
		if (nCards == 2)
		{
			_ishipair = true;
		}
		// at least flop seen...
		else if (nCards >= 5)
		{
			if ((int)(StdDeck_RANK(HandVal_TOP_CARD(handval))) >= rankhicommon())
			{
				_ishipair = true;										
			}
			else if ((int)(StdDeck_RANK(HandVal_TOP_CARD(handval))) < rankhicommon()
				&& (int)(StdDeck_RANK(HandVal_TOP_CARD(handval))) > ranklocommon())
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

	// player cards
	for (int i=0; i<k_number_of_cards_per_player; i++)
	{
		if (p_scraper->card_player(USER_CHAIR, i) != CARD_BACK && 
			p_scraper->card_player(USER_CHAIR, i) != CARD_NOCARD)
		{
			CardMask_SET(plCards, p_scraper->card_player(USER_CHAIR, i));
			CardMask_SET(plcomCards, p_scraper->card_player(USER_CHAIR, i));
		}
	}

	// common cards
	for (int i=0; i<k_number_of_community_cards; i++)
	{
		if (p_scraper->card_common(i) != CARD_BACK && 
			p_scraper->card_common(i) != CARD_NOCARD)
		{
			CardMask_SET(comCards, p_scraper->card_common(i));
			CardMask_SET(plcomCards, p_scraper->card_common(i));
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

	int tsuitcommon = p_symbol_engine_cards->tsuitcommon();
	comsuit = (tsuitcommon==WH_SUIT_CLUBS ? Suit_CLUBS :
		tsuitcommon==WH_SUIT_DIAMONDS ? Suit_DIAMONDS :
		tsuitcommon==WH_SUIT_HEARTS ? Suit_HEARTS :
		tsuitcommon==WH_SUIT_SPADES ? Suit_SPADES : 0);

	int tsuit = p_symbol_engine_cards->tsuit();
	plcomsuit = (tsuit==WH_SUIT_CLUBS ? Suit_CLUBS :
		tsuit==WH_SUIT_DIAMONDS ? Suit_DIAMONDS :
		tsuit==WH_SUIT_HEARTS ? Suit_HEARTS :
		tsuit==WH_SUIT_SPADES ? Suit_SPADES : 0);

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
		if (p_scraper_access->IsKnownCard(p_scraper->card_common(i)))
		{
			CardMask_SET(comCards, p_scraper->card_common(i));
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

	// If we have a straight flush or flush, figure out the suit
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
			if (p_scraper->card_player(USER_CHAIR, i) != CARD_BACK && 
				p_scraper->card_player(USER_CHAIR, i) != CARD_NOCARD)
			{
				CardMask_SET(Cards, p_scraper->card_player(USER_CHAIR, i));
			}
		}

		for (int i=0; i<k_number_of_community_cards; i++)
		{
			if (p_scraper->card_common(i) != CARD_BACK && 
				p_scraper->card_common(i) != CARD_NOCARD)
			{
				CardMask_SET(Cards, p_scraper->card_common(i));
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