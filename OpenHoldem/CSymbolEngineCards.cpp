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

#include "stdafx.h"
#include "CSymbolEngineCards.h"

#include <assert.h>
#include "CBetroundCalculator.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CSymbolEnginePokerval.h"
#include "CSymbolEngineUserchair.h"
#include "inlines/eval.h"
#include "..\CTransform\CTransform.h"
#include "MagicNumbers.h"
#include "Numericalfunctions.h"

CSymbolEngineCards *p_symbol_engine_cards = NULL;

CSymbolEngineCards::CSymbolEngineCards()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	//
	// We use 2 times the function p_symbol_engine_pokerval->CalculatePokerval,
	// but luckily this does not create a symbol-dependency.
	// We leave the file ""CSymbolEnginePokerval.h" here out to avoid a circular dependency.
	assert(p_symbol_engine_userchair != NULL);
}

CSymbolEngineCards::~CSymbolEngineCards()
{}

void CSymbolEngineCards::InitOnStartup()
{
	ResetOnConnection();
}

void CSymbolEngineCards::ResetOnConnection()
{
	ResetOnHandreset();
}

void CSymbolEngineCards::ResetOnHandreset()
{
	// hand tests
	for (int i=0; i<k_number_of_cards_per_player; i++)
	{
		_$$pc[i] = WH_NOCARD;
		_$$pr[i] = 0;
		_$$ps[i] = 0;
	}

	for (int i=0; i<k_number_of_community_cards; i++)
	{
		_$$cc[i] = WH_NOCARD;
		_$$cs[i] = 0;
		_$$cr[i] = 0;
	}
	// pocket tests
	_ispair = 0;
	_issuited = 0;
	_isconnector = 0;

	// common cards
	_nouts = 0;
	_ncommoncardsknown = 0;
	_ncommoncardsknown = 0;

	// (un)known cards
	_ncardsknown = 0;
	_ncardsunknown = 0;
	_ncardsbetter = 0;

	// nhands
	_nhands = 0;
	_nhandshi = 0;
	_nhandslo = 0;
	_nhandsti = 0;

	// flushes straights sets
	_nsuited = 0;
	_nsuitedcommon = 0;
	_tsuit = 0;
	_tsuitcommon = 0;
	_nranked = 0;
	_nrankedcommon = 0;
	_trank = 0;
	_trankcommon = 0;
	_nstraight = 0;
	_nstraightcommon = 0;
	_nstraightflush = 0;
	_nstraightflushcommon     = 0;
	_nstraightfill            = k_cards_needed_for_straight;
	_nstraightfillcommon      = k_cards_needed_for_straight;
	_nstraightflushfill       = k_cards_needed_for_flush;
	_nstraightflushfillcommon = k_cards_needed_for_flush;
}

void CSymbolEngineCards::ResetOnNewRound()
{}

void CSymbolEngineCards::ResetOnMyTurn()
{}

void CSymbolEngineCards::ResetOnHeartbeat()
{
	CalcPocketTests();
	CalculateCommonCards();
	CalculateHandTests();
	CalcFlushesStraightsSets();
	CalcUnknownCards();
}

void CSymbolEngineCards::CalcPocketTests()
{
	_ispair      = false;
	_issuited    = false;
	_isconnector = false;

	if (!p_scraper_access->UserHasCards())
	{
		return;
	}

	if (StdDeck_RANK(p_scraper->card_player(USER_CHAIR, 0)) 
			== StdDeck_RANK(p_scraper->card_player(USER_CHAIR, 1)))
	{
		_ispair = true;															
	}
	if (StdDeck_SUIT(p_scraper->card_player(USER_CHAIR, 0)) 
		== StdDeck_SUIT(p_scraper->card_player(USER_CHAIR, 1)))
	{
		_issuited = true;														
	}
	if (abs(int(StdDeck_RANK(p_scraper->card_player(USER_CHAIR, 0))  
		- StdDeck_RANK(p_scraper->card_player(USER_CHAIR, 1)))) == 1)
	{
		_isconnector = true;														
	}
}

/*void CSymbolEngineCards::CardMaskCreateCMAllCardsOfOfSuit(*CardMask card_mask_of_suit, int suit)
{
	CardMask_RESET(card_mask_of_suit);
	for (int i=Rank_2; i<=Rank_Ace; i++)
	{
		CardMask_SET(card_mask_of_suit, StdDeck_MAKE_CARD(i, suit)
	}
}*/

void CSymbolEngineCards::CalcFlushesStraightsSets()
{
	int				i = 0, j = 0, n = 0;
	CardMask		plCards = {0}, comCards = {0};
	CardMask		heartsCards = {0}, diamondsCards = {0}, clubsCards = {0}, spadesCards = {0}, suittestCards = {0};
	int				max = 0;
	unsigned int	strbits = 0;

	_tsuit   = 0;
	_nsuited = 0;
	_tsuitcommon   = 0;
	_nsuitedcommon = 0;
	_nranked = 0;
	_trank = 0;
	_nrankedcommon = 0;
	_trankcommon   = 0;
	_nstraight     = 0;
	_nstraightfill = k_cards_needed_for_straight;
	_nstraightcommon     = 0;
	_nstraightfillcommon = k_cards_needed_for_straight;
	_nstraightflush     = 0;
	_nstraightflushfill = k_cards_needed_for_straight;
	_nstraightflushcommon     = 0;
	_nstraightflushfillcommon = k_cards_needed_for_straight;

	// Set up some suit masks
/*	CardMaskCreateCMAllCardsOfOfSuit(&heartsCards);
	CardMaskCreateCMAllCardsOfOfSuit(&diamondsCards);
	CardMaskCreateCMAllCardsOfOfSuit(&spadesCards);
	CardMaskCreateCMAllCardsOfOfSuit(&clubsCards);*/
	
	// player cards
	CardMask_RESET(plCards);
	for (int i=0; i<k_number_of_cards_per_player; i++)
	{
		int card = p_scraper->card_player(USER_CHAIR, i);
		if (p_scraper_access->IsKnownCard(card))
		{
			CardMask_SET(plCards, card);
		}
	}

	// common cards
	CardMask_RESET(comCards);
	for (int i=0; i<k_number_of_community_cards; i++)
	{
		int card = p_scraper->card_common(i);
		if (p_scraper_access->IsKnownCard(card))
		{
			CardMask_SET(comCards, p_scraper->card_common(i));
			CardMask_SET(plCards, p_scraper->card_common(i));
		}
	}

	// nsuited, tsuit
	max = 0;
	CardMask_AND(suittestCards, plCards, spadesCards);
	n = StdDeck_numCards(suittestCards);
	write_log(preferences.debug_symbolengine(), "[CSymbolEngineCards] found %i player cards of spades\n", n);
	if ( n>max && n>0)
	{
		max = n;
		_tsuit = WH_SUIT_SPADES;
	}
	CardMask_AND(suittestCards, plCards, heartsCards);
	n = StdDeck_numCards(suittestCards);
	write_log(preferences.debug_symbolengine(), "[CSymbolEngineCards] found %i player cards of hearts\n", n);
	if ( n>max && n>0)
	{
		max = n;
		_tsuit = WH_SUIT_HEARTS;
	}
	CardMask_AND(suittestCards, plCards, diamondsCards);
	n = StdDeck_numCards(suittestCards);
	write_log(preferences.debug_symbolengine(), "[CSymbolEngineCards] found %i player cards of diamonds\n", n);
	if ( n>max && n>0)
	{
		max = n;
		_tsuit = WH_SUIT_DIAMONDS;
	}
	CardMask_AND(suittestCards, plCards, clubsCards);
	n = StdDeck_numCards(suittestCards);
	write_log(preferences.debug_symbolengine(), "[CSymbolEngineCards] found %i player cards of clubs\n", n);
	if ( n>max && n>0)
	{
		max = n;
		_tsuit = WH_SUIT_CLUBS;												
	}
	_nsuited = max;															

	// nsuitedcommon, tsuitcommon
	max = 0;
	CardMask_AND(suittestCards, comCards, spadesCards);
	n = StdDeck_numCards(suittestCards);
	write_log(preferences.debug_symbolengine(), "[CSymbolEngineCards] found %i common cards of spades\n", n);
	if ( n>max && n>0)
	{
		max = n;
		_tsuitcommon = WH_SUIT_SPADES;
	}
	CardMask_AND(suittestCards, comCards, heartsCards);
	n = StdDeck_numCards(suittestCards);
	write_log(preferences.debug_symbolengine(), "[CSymbolEngineCards] found %i common cards of hearts\n", n);
	if ( n>max && n>0)
	{
		max = n;
		_tsuitcommon = WH_SUIT_HEARTS;
	}
	CardMask_AND(suittestCards, comCards, diamondsCards);
	n = StdDeck_numCards(suittestCards);
	write_log(preferences.debug_symbolengine(), "[CSymbolEngineCards] found %i common cards of diamonds\n", n);
	if ( n>max && n>0)
	{
		max = n;
		_tsuitcommon = WH_SUIT_DIAMONDS;
	}
	CardMask_AND(suittestCards, comCards, clubsCards);
	n = StdDeck_numCards(suittestCards);
	write_log(preferences.debug_symbolengine(), "[CSymbolEngineCards] found %i common cards of clubs\n", n);
	if ( n>max && n>0)
	{
		max = n;
		_tsuitcommon = WH_SUIT_CLUBS;												
	}
	_nsuitedcommon = max;															

	// nranked, trank
	max = 0;
	for (int i=12; i>=0; i--)
	{
		n = CardMask_CARD_IS_SET(plCards, i+(Rank_COUNT*0)) +
			CardMask_CARD_IS_SET(plCards, i+(Rank_COUNT*1)) +
			CardMask_CARD_IS_SET(plCards, i+(Rank_COUNT*2)) +
			CardMask_CARD_IS_SET(plCards, i+(Rank_COUNT*3));
		if (n>max && n>0)
		{
			max = n;
			_trank = i + 2;															// trank
		}
	}
	_nranked = max;																// nranked

	// nrankedcommon, trankcommon
	max = 0;
	for (int i=12; i>=0; i--)
	{
		n = CardMask_CARD_IS_SET(comCards, i+(Rank_COUNT*0)) +
			CardMask_CARD_IS_SET(comCards, i+(Rank_COUNT*1)) +
			CardMask_CARD_IS_SET(comCards, i+(Rank_COUNT*2)) +
			CardMask_CARD_IS_SET(comCards, i+(Rank_COUNT*3));
		if (n>max && n>0)
		{
			max = n;
			_trankcommon = i + 2;													// trankcommon
		}
	}
	_nrankedcommon = max;															// nrankedcommon

	// nstraight, nstraightfill
	strbits = 0;
	for (int i=0; i<Rank_COUNT; i++)
	{
		if (CardMask_CARD_IS_SET(plCards, (Rank_COUNT*0)+i) ||
			CardMask_CARD_IS_SET(plCards, (Rank_COUNT*1)+i) ||
			CardMask_CARD_IS_SET(plCards, (Rank_COUNT*2)+i) ||
			CardMask_CARD_IS_SET(plCards, (Rank_COUNT*3)+i))
		{
			strbits |= (1<<(i+2));
		}
	}
	if (CardMask_CARD_IS_SET(plCards, (Rank_COUNT*0)+Rank_ACE) ||
		CardMask_CARD_IS_SET(plCards, (Rank_COUNT*1)+Rank_ACE) ||
		CardMask_CARD_IS_SET(plCards, (Rank_COUNT*2)+Rank_ACE) ||
		CardMask_CARD_IS_SET(plCards, (Rank_COUNT*3)+Rank_ACE))
	{
		strbits |= (1<<1);
	}

	// Checking for T-low-strsight down to Ace-low-straight
	for (int i=10; i>=1; i--)
	{
		if (((strbits>>i)&0x1f) == 0x1f)
		{
			_nstraight = (_nstraight<5 ? 5 : _nstraight);
		}
		else if (((strbits>>i)&0x1e) == 0x1e ||
				 ((strbits>>i)&0x0f) == 0x0f)
		{
			_nstraight = _nstraight<4 ? 4 : _nstraight;
		}
		else if (((strbits>>i)&0x1c) == 0x1c ||
				 ((strbits>>i)&0x0e) == 0x0e ||
				 ((strbits>>i)&0x07) == 0x07)
		{
			_nstraight = _nstraight<3 ? 3 : _nstraight;
		}
		else if (((strbits>>i)&0x18) == 0x18 ||
				 ((strbits>>i)&0x0c) == 0x0c ||
				 ((strbits>>i)&0x06) == 0x06 ||
				 ((strbits>>i)&0x3) == 0x03)
		{
			_nstraight = _nstraight<2 ? 2 : _nstraight;
		}
		else 
		{
			_nstraight = _nstraight<1 ? 1 : _nstraight;					// nstraight
		}

		n = bitcount(((strbits>>i)&0x1f));
		if (5-n < _nstraightfill)
			{
				_nstraightfill = 5-n;													// nstraightfill
			}
		}

		// nstraightcommon, nstraightfillcommon
		if (_ncommoncardsknown < 1)
		{
			_nstraightfillcommon = 5;
		}
		else
		{
			strbits = 0;
			for (int i=0; i<Rank_COUNT; i++)
			{
				if (CardMask_CARD_IS_SET(comCards, (Rank_COUNT*0)+i) ||
					CardMask_CARD_IS_SET(comCards, (Rank_COUNT*1)+i) ||
					CardMask_CARD_IS_SET(comCards, (Rank_COUNT*2)+i) ||
					CardMask_CARD_IS_SET(comCards, (Rank_COUNT*3)+i))
				{
					strbits |= (1<<(i+2));
				}
			}
			if (CardMask_CARD_IS_SET(comCards, (Rank_COUNT*0)+Rank_ACE) ||
				CardMask_CARD_IS_SET(comCards, (Rank_COUNT*1)+Rank_ACE) ||
				CardMask_CARD_IS_SET(comCards, (Rank_COUNT*2)+Rank_ACE) ||
				CardMask_CARD_IS_SET(comCards, (Rank_COUNT*3)+Rank_ACE))
			{
				strbits |= (1<<1);
			}

			// Checking for T-low-strsight down to Ace-low-straight
			for (int i=10; i>=1; i--)
			{
				if (((strbits>>i)&0x1f) == 0x1f)
				{
				_nstraightcommon = _nstraightcommon<5 ? 5 : _nstraightcommon;
				}
				else if (((strbits>>i)&0x1e) == 0x1e ||
						 ((strbits>>i)&0x0f) == 0x0f)
				{
				_nstraightcommon = _nstraightcommon<4 ? 4 : _nstraightcommon;
				}
				else if (((strbits>>i)&0x1c) == 0x1c ||
						 ((strbits>>i)&0x0e) == 0x0e ||
						 ((strbits>>i)&0x07) == 0x07)
				{
				_nstraightcommon = _nstraightcommon<3 ? 3 : _nstraightcommon;
				}
				else if (((strbits>>i)&0x18) == 0x18 ||
						 ((strbits>>i)&0x0c) == 0x0c ||
						 ((strbits>>i)&0x06) == 0x06 ||
						 ((strbits>>i)&0x03) == 0x03)
				{
				_nstraightcommon = _nstraightcommon<2 ? 2 : _nstraightcommon;
				}
				else
				{
				_nstraightcommon = _nstraightcommon<1 ? 1 : _nstraightcommon; // nstraightcommon
				}

				n = bitcount(((strbits>>i)&0x1f));
			if (5-n < _nstraightfillcommon)	
				{
				_nstraightfillcommon = 5-n;										// nstraightfillcommon
				}
			}
		}

		// nstraightflush, nstraightflushfill
		for (int j=0; j<k_number_of_suits_per_deck; j++)
		{
			strbits = 0;
			for (int i=0; i<Rank_COUNT; i++)
			{
				if (CardMask_CARD_IS_SET(plCards, (Rank_COUNT*j)+i))
				{
					strbits |= (1<<(i+2));
				}
			}
			if (CardMask_CARD_IS_SET(plCards, (Rank_COUNT*j)+Rank_ACE))
			{
				strbits |= (1<<1);
			}
			// Checking for T-low-strsight down to Ace-low-straight
			for (int i=10; i>=1; i--)
			{
				if (((strbits>>i)&0x1f) == 0x1f)
				{
					_nstraightflush = _nstraightflush<5 ? 5 : _nstraightflush;
				}
				else if (((strbits>>i)&0x1e) == 0x1e ||
						 ((strbits>>i)&0x0f) == 0x0f)
				{
					_nstraightflush = _nstraightflush<4 ? 4 : _nstraightflush;
				}
				else if (((strbits>>i)&0x1c) == 0x1c ||
						 ((strbits>>i)&0x0e) == 0x0e ||
						 ((strbits>>i)&0x07) == 0x07)
				{
					_nstraightflush = _nstraightflush<3 ? 3 : _nstraightflush;
				}
				else if (((strbits>>i)&0x18) == 0x18 ||
						 ((strbits>>i)&0x0c) == 0x0c ||
						 ((strbits>>i)&0x06) == 0x06 ||
						 ((strbits>>i)&0x03) == 0x03)
				{
					_nstraightflush = _nstraightflush<2 ? 2 : _nstraightflush;
				}
				else
				{
					_nstraightflush = _nstraightflush<1 ? 1 : _nstraightflush;	
				}

				n = bitcount(((strbits>>i)&0x1f));
				if (k_cards_needed_for_flush-n < _nstraightflushfill)
				{
					_nstraightflushfill = k_cards_needed_for_flush - n;
				}
			}
		}

		// nstraightflushcommon, nstraightflushfillcommon
		if (_ncommoncardsknown < 1)
		{
			_nstraightflushfillcommon = k_cards_needed_for_straight;
		}
		else
		{
			for (int j=0; j<=3; j++)
			{
				strbits = 0;
				for (int i=0; i<Rank_COUNT; i++)
				{
					if (CardMask_CARD_IS_SET(comCards, (Rank_COUNT*j)+i))
					{
						strbits |= (1<<(i+2));
					}
				}
				if (CardMask_CARD_IS_SET(comCards, (Rank_COUNT*j)+Rank_ACE))
				{
					strbits |= (1<<1);
			}

			for (int i=10; i>=1; i--)
			{
				if (((strbits>>i)&0x1f) == 0x1f)
				{
					_nstraightflushcommon = _nstraightflushcommon<5 ? 5 : _nstraightflushcommon;
				}
				else if (((strbits>>i)&0x1e) == 0x1e ||
						 ((strbits>>i)&0x0f) == 0x0f)
				{
					_nstraightflushcommon = _nstraightflushcommon<4 ? 4 : _nstraightflushcommon;
				}
				else if (((strbits>>i)&0x1c) == 0x1c ||
						 ((strbits>>i)&0x0e) == 0x0e ||
						 ((strbits>>i)&0x07) == 0x07)
				{
					_nstraightflushcommon = _nstraightflushcommon<3 ? 3 : _nstraightflushcommon;
				}
				else if (((strbits>>i)&0x18) == 0x18 ||
						 ((strbits>>i)&0x0c) == 0x0c ||
						 ((strbits>>i)&0x06) == 0x06 ||
						 ((strbits>>i)&0x03) == 0x03)
				{
					_nstraightflushcommon = _nstraightflushcommon<2 ? 2 : _nstraightflushcommon;
				}
				else
				{
					_nstraightflushcommon = _nstraightflushcommon<1 ? 1 : _nstraightflushcommon;		// nstraightflushcommon
				}
				n = bitcount(((strbits>>i)&0x1f));
				if (5-n < _nstraightflushfillcommon )
				{
					_nstraightflushfillcommon = 5-n;							
				}
			}
		}
	}
}

int GetRankFromCard(int scraper_card);
int GetSuitFromCard(int scraper_card);

void CSymbolEngineCards::CalculateHandTests()
{
	// Player cards
	for (int i=0; i<k_number_of_cards_per_player; i++)
	{
		int card = p_scraper->card_player(USER_CHAIR, i);
		if (p_scraper_access->IsKnownCard(card))
		{
			int rank = GetRankFromCard(p_scraper->card_player(USER_CHAIR, i));
			int suit = GetSuitFromCard(p_scraper->card_player(USER_CHAIR, i));

			AssertRange(rank, 0, k_rank_ace);
			AssertRange(suit, 0, WH_SUIT_SPADES);

			_$$pc[i] = (rank<<4) | suit;								  
			_$$pr[i] = rank;						
			_$$ps[i] = suit;	
		}
	}
	// Common cards
	for (int i=0; i<k_number_of_community_cards; i++)
	{
		int card = p_scraper->card_common(i);
		if (p_scraper_access->IsKnownCard(card))
		{
			int rank = GetRankFromCard(p_scraper->card_common(i));
			int suit = GetSuitFromCard(p_scraper->card_common(i));

			_$$cc[i] = (rank<<4) | suit;								  
			_$$cr[i] = rank;						
			_$$cs[i] = suit;							
		}
	}
}

void CSymbolEngineCards::CalculateCommonCards()
{
	_ncommoncardsknown = 0;
	for (int i=0; i<k_number_of_community_cards; i++)
	{
		if (p_scraper->card_common(i) != CARD_NOCARD)
		{
			_ncommoncardsknown++;							
		}
	}
}


void CSymbolEngineCards::CalcUnknownCards()
{
	CardMask		stdCards = {0}, commonCards = {0};
	int				nstdCards = 0, ncommonCards = 0;
	HandVal			handval_std = 0, handval_std_plus1 = 0, handval_common_plus1 = 0;
	int				dummy = 0;

	write_log(preferences.debug_symbolengine(), "[CSymbolEngineCards] CalcUnknownCards()\n");
	CardMask_RESET(stdCards);
	CardMask_RESET(commonCards);

	for (int i=0; i<k_number_of_cards_per_player; i++)
	{
		// player cards
		int card = p_scraper->card_player(p_symbol_engine_userchair->userchair(), i);
		if (p_scraper_access->IsKnownCard(card))
		{
			CardMask_SET(stdCards, card);
			nstdCards++;
		}
	}
	for (int i=0; i<k_number_of_community_cards; i++)
	{
		// common cards
		int card = p_scraper->card_common(i);
		if (p_scraper_access->IsKnownCard(card))
		{
			CardMask_SET(stdCards, card);
			nstdCards++;
			CardMask_SET(commonCards, p_scraper->card_common(i));
		}
	}

	_ncardsknown = nstdCards;	
	_ncardsunknown = k_number_of_cards_per_deck - _ncardsknown;

	handval_std = Hand_EVAL_N(stdCards, nstdCards);
	_nouts = 0;
	_ncardsbetter = 0;

	if (p_symbol_engine_userchair->userchair_confirmed())
	{
		write_log(preferences.debug_symbolengine(), "[CSymbolEngineCards] userchair confirmed; calculating nouts...\n");
		// iterate through every unseen card and see what happens to our handvals
		for (int i=0; i<k_number_of_cards_per_deck; i++)
		{
			if (i!=p_scraper->card_player(p_symbol_engine_userchair->userchair(), 0)  
				&& i!=p_scraper->card_player(p_symbol_engine_userchair->userchair(), 1) 
				&& i!=p_scraper->card_common(0) 
				&& i!=p_scraper->card_common(1) 
				&& i!=p_scraper->card_common(2) 
				&& i!=p_scraper->card_common(3) 
				&& i!=p_scraper->card_common(4))
			{
				CardMask_SET(stdCards, i);
				handval_std_plus1 = Hand_EVAL_N(stdCards, nstdCards+1);
				CardMask_UNSET(stdCards, i);

				CardMask_SET(commonCards, i);
				handval_common_plus1 = Hand_EVAL_N(commonCards, ncommonCards+1);
				CardMask_UNSET(commonCards, i);

				if (BETROUND < k_betround_river 
					&& HandVal_HANDTYPE(handval_std_plus1) > HandVal_HANDTYPE(handval_std) 
					&& p_symbol_engine_pokerval->CalculatePokerval(handval_std_plus1, nstdCards+1, &dummy, CARD_NOCARD, CARD_NOCARD) > p_symbol_engine_pokerval->pokerval()
					&& HandVal_HANDTYPE(handval_std_plus1) > HandVal_HANDTYPE(handval_common_plus1))
				{
					_nouts++;										
				}

				if (p_symbol_engine_pokerval->CalculatePokerval(handval_common_plus1, ncommonCards+1, &dummy, CARD_NOCARD, CARD_NOCARD) > p_symbol_engine_pokerval->pokerval())
				{
					_ncardsbetter++;							
				}
			}
		}
	}
	write_log(preferences.debug_symbolengine(), "[CSymbolEngineCards] nouts: %i\n", _nouts);
	AssertRange(_ncardsknown,   0, k_number_of_cards_per_deck);
	AssertRange(_ncardsunknown, 0, k_number_of_cards_per_deck);
	AssertRange(_nouts,         0, k_number_of_cards_per_deck);
	AssertRange(_ncardsbetter,  0, k_number_of_cards_per_deck);
}

bool CSymbolEngineCards::IsHand(const char *a, int *e)
{
	int				cardrank[2] = {0}, temp;
	int				suited = 0;  //0=not specified, 1=suited, 2=offsuit
	int				cardcnt = 0;
	int				plcardrank[2] = {0}, plsuited = 0;

	if (strlen(a)<=1)
	{
		if (e)
			*e = ERR_INVALID_SYM;
		return false;
	}
	assert(a[0] == '$');

	// passed in symbol query
	for (int i=1; i<(int) strlen(a); i++)
	{
		if (a[i]>='2' && a[i]<='9')
			cardrank[cardcnt++] =  a[i] - '0';

		else if (a[i]=='T' || a[i]=='t')
			cardrank[cardcnt++] = 10;

		else if (a[i]=='J' || a[i]=='j')
			cardrank[cardcnt++] = 11;

		else if (a[i]=='Q' || a[i]=='q')
			cardrank[cardcnt++] = 12;

		else if (a[i]=='K' || a[i]=='k')
			cardrank[cardcnt++] = 13;

		else if (a[i]=='A' || a[i]=='a')
			cardrank[cardcnt++] = 14;

		else if (a[i]=='X' || a[i]=='x')
			cardrank[cardcnt++] = -1;

		else if (a[i]=='S' || a[i]=='s')
			suited=1;

		else if (a[i]=='O' || a[i]=='o')
			suited=2;

		else
		{
			if (e)
				*e = ERR_INVALID_SYM;
			return false;
		}
	}

	if (!p_symbol_engine_userchair->userchair_confirmed())
		return false;

	// sort
	if (cardrank[1] > cardrank[0])
	{
		temp = cardrank[0];
		cardrank[0] = cardrank[1];
		cardrank[1] = temp;
	}

	// playercards
	plcardrank[0] = Deck_RANK(p_scraper->card_player(p_symbol_engine_userchair->userchair(), 0))+2;
	plcardrank[1] = Deck_RANK(p_scraper->card_player(p_symbol_engine_userchair->userchair(), 1))+2;
	if (plcardrank[1] > plcardrank[0])
	{
		SwapInts(&plcardrank[0], &plcardrank[1]);
	}
	if (Deck_SUIT(p_scraper->card_player(p_symbol_engine_userchair->userchair(), 0)) == 
		Deck_SUIT(p_scraper->card_player(p_symbol_engine_userchair->userchair(), 1)))
	{
		plsuited = 1;
	}
	else
	{
		plsuited = 0;
	}

	// check for non suited/offsuit match
	if (suited==1 && plsuited==0)
		return false;

	if (suited==2 && plsuited==1)
		return 0;

	// check for non rank match
	// two wildcards
	if (cardrank[0]==-1 && cardrank[1]==-1)
		return true;

	// one card passed in, or one with a wildcard
	if (cardrank[1]==0 || cardrank[1]==-1)
	{
		if (cardrank[0] != plcardrank[0] &&
				cardrank[0] != plcardrank[1])
		{
			return false;
		}
	}

	// two cards passed in
	else
	{
		if (cardrank[0]!=-1 && cardrank[0]!=plcardrank[0])
			return false;

		if (cardrank[1]!=-1 && cardrank[1]!=plcardrank[1])
			return false;
	}

	return true;
}

int GetRankFromCard(int scraper_card)
{
	// Hand-eval library uses 0 (smallest positive) for 2
	return StdDeck_RANK(scraper_card) + 2;
}

int GetSuitFromCard(int scraper_card)
{
	switch StdDeck_SUIT(scraper_card)
	{
		case StdDeck_Suit_CLUBS:  
			return WH_SUIT_CLUBS;
		case StdDeck_Suit_DIAMONDS: 
			return WH_SUIT_DIAMONDS;
		case StdDeck_Suit_HEARTS: 
			return WH_SUIT_HEARTS;
		case StdDeck_Suit_SPADES: 
			return WH_SUIT_SPADES;
	}
	return k_undefined;
}