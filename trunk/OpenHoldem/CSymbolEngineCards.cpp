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
	// but luckily this does not create name symbol-dependency.
	// We leave the file ""CSymbolEnginePokerval.h" here out to avoid name circular dependency.
	assert(p_symbol_engine_userchair != NULL);
	// Set up some suit masks
	CardMaskCreateCMAllCardsOfOfSuit(&heartsCards,   Suit_HEARTS);
	CardMaskCreateCMAllCardsOfOfSuit(&diamondsCards, Suit_DIAMONDS);
	CardMaskCreateCMAllCardsOfOfSuit(&spadesCards,   Suit_SPADES);
	CardMaskCreateCMAllCardsOfOfSuit(&clubsCards,    Suit_CLUBS);
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
		write_log(preferences.debug_symbolengine(), "[CSymbolEngineCards] Resetting $$pcX, $$psX, $$prX\n");
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
{
	__TRACE
}

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

	if (!p_scraper_access->UserHasKnownCards())
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

void CSymbolEngineCards::CardMaskCreateCMAllCardsOfOfSuit(
	CardMask *card_mask_of_suit, int suit)
{
	CardMask_RESET(*card_mask_of_suit);
	for (int i=Rank_2; i<=Rank_ACE; i++)
	{
		CardMask_SET(*card_mask_of_suit, StdDeck_MAKE_CARD(i, suit));
	}
}

int CSymbolEngineCards::GetNumberOfCardsForSuit(
	CardMask cards, CardMask card_mask_of_suit)
{
	CardMask suittestCards;
	CardMask_AND(suittestCards, cards, card_mask_of_suit);
	int n = StdDeck_numCards(suittestCards);
	write_log(preferences.debug_symbolengine(), "[CSymbolEngineCards] found %i cards of the same suit\n", n);
	return n;
}

int CSymbolEngineCards::GetDominantSuit(CardMask cards, int* max_cards_of_same_suit)
{
	int n_clubs, n_diamonds, n_hearts, n_spades;
	n_clubs    = GetNumberOfCardsForSuit(cards, clubsCards);
	n_diamonds = GetNumberOfCardsForSuit(cards, diamondsCards);
	n_hearts   = GetNumberOfCardsForSuit(cards, heartsCards);
	n_spades   = GetNumberOfCardsForSuit(cards, spadesCards);
	write_log(preferences.debug_symbolengine(), "[CSymbolEngineCards] found %i clubs, %i diamonds, %i hearts and %i spades\n",
		n_clubs, n_diamonds, n_hearts, n_spades);
	if ((n_clubs >= n_diamonds) && (n_clubs >= n_hearts) && (n_clubs >= n_spades))
	{
		*max_cards_of_same_suit = n_clubs;
		return WH_SUIT_CLUBS;
	}
	else if ((n_diamonds >= n_hearts) && (n_diamonds >= n_spades))
	{
		*max_cards_of_same_suit = n_diamonds;
		return WH_SUIT_DIAMONDS;
	}
	else if (n_hearts >= n_spades)
	{
		*max_cards_of_same_suit = n_hearts;
		return WH_SUIT_HEARTS;
	}
	else
	{
		*max_cards_of_same_suit = n_spades;
		return WH_SUIT_SPADES;
	}
}

void CSymbolEngineCards::CalcFlushesStraightsSets()
{
	int				n = 0;
	CardMask		plCards = {0}, comCards = {0};
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
	
	// player cards
	CardMask_RESET(plCards);
	for (int i=0; i<k_number_of_cards_per_player; i++)
	{
		int card = p_scraper->card_player(USER_CHAIR, i);
		if (p_scraper_access->IsKnownCard(card))
		{
			write_log(preferences.debug_symbolengine(), "[CSymbolEngineCards] Setting card mask player: %i\n",
				card);
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
			write_log(preferences.debug_symbolengine(), "[CSymbolEngineCards] Setting card mask common (and player): %i\n",
				card);
			CardMask_SET(comCards, card);
			CardMask_SET(plCards, card);
		}
	}

	// nsuited, tsuit
	_tsuit = GetDominantSuit(plCards, &_nsuited);
	// nsuitedcommon, tsuitcommon
	_tsuitcommon = GetDominantSuit(comCards, &_nsuitedcommon);
	
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

			write_log(preferences.debug_symbolengine(), 
				"[CSymbolEngineCards] Card = %i\n",
				p_scraper->card_player(USER_CHAIR, i));
			write_log(preferences.debug_symbolengine(), 
				"[CSymbolEngineCards] Rank = %i\n", rank);
			write_log(preferences.debug_symbolengine(), 
				"[CSymbolEngineCards] Suit = %i\n", suit);

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

bool CSymbolEngineCards::IsHand(const char *name, int *e)
{
	int				cardrank[2] = {0}, temp;
	int				suited = 0;  //0=not specified, 1=suited, 2=offsuit
	int				cardcnt = 0;
	int				plcardrank[2] = {0}, plsuited = 0;

	if (strlen(name)<=1)
	{
		if (e)
			*e = ERR_INVALID_SYM;
		return false;
	}
	assert(name[0] == '$');

	// passed in symbol query
	for (int i=1; i<(int) strlen(name); i++)
	{
		if (name[i]>='2' && name[i]<='9')
			cardrank[cardcnt++] =  name[i] - '0';

		else if (name[i]=='T' || name[i]=='t')
			cardrank[cardcnt++] = 10;

		else if (name[i]=='J' || name[i]=='j')
			cardrank[cardcnt++] = 11;

		else if (name[i]=='Q' || name[i]=='q')
			cardrank[cardcnt++] = 12;

		else if (name[i]=='K' || name[i]=='k')
			cardrank[cardcnt++] = 13;

		else if (name[i]=='A' || name[i]=='name')
			cardrank[cardcnt++] = 14;

		else if (name[i]=='X' || name[i]=='x')
			cardrank[cardcnt++] = -1;

		else if (name[i]=='S' || name[i]=='s')
			suited=1;

		else if (name[i]=='O' || name[i]=='o')
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

	// one card passed in, or one with name wildcard
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

bool CSymbolEngineCards::EvaluateSymbol(const char *name, double *result)
{
	if (memcmp(name, "$", 1)==0)
	{
		if (memcmp(name, "$$pc", 4)==0)
		{
			*result = $$pc(name[4]-'0');
		}
		else if (memcmp(name, "$$pr", 4)==0)
		{
			*result = $$pr(name[4]-'0');
		}
		else if (memcmp(name, "$$ps", 4)==0)  
		{
			*result = $$ps(name[4]-'0');
		}
		else if (memcmp(name, "$$cc", 4)==0)  
		{
			*result = $$cc(name[4]-'0');
		}
		else if (memcmp(name, "$$cr", 4)==0)  
		{
			*result = $$cr(name[4]-'0');
		}
		else if (memcmp(name, "$$cs", 4)==0)  
		{
			*result = $$cs(name[4]-'0');
		}
		else if (memcmp(name, "$", 1)==0)  	
		{
			int *e = SUCCESS;
			*result = IsHand(name, e);
		}
		else
		{
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}
	else if (memcmp(name, "ncards", 6)==0)
	{
		if (memcmp(name, "ncardsknown", 11)==0 && strlen(name)==11)
		{
			*result = ncardsknown();
		}
		else if (memcmp(name, "ncardsunknown", 13)==0 && strlen(name)==13)	
		{
			*result = ncardsunknown();
		}
		else if (memcmp(name, "ncardsbetter", 12)==0 && strlen(name)==12)	
		{
			*result = ncardsbetter();
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
		if (memcmp(name, "ispair", 6)==0 && strlen(name)==6)
		{
			*result = ispair();
		}
		else if (memcmp(name, "issuited", 8)==0 && strlen(name)==8)
		{
			*result = issuited();
		}
		else if (memcmp(name, "isconnector", 11)==0 && strlen(name)==11)
		{
			*result = isconnector();
		}
		else
		{
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}
	else if (memcmp(name, "nsuited", 7)==0)
	{
		if (memcmp(name, "nsuited", 7)==0 && strlen(name)==7)	
		{
			*result = nsuited();
		}
		else if (memcmp(name, "nsuitedcommon", 13)==0 && strlen(name)==13)	
		{
			*result = nsuitedcommon();
		}
		else
		{
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}
	else if (memcmp(name, "tsuit", 5)==0)
	{
		if (memcmp(name, "tsuit", 5)==0 && strlen(name)==5)
		{
			*result = tsuit();
		}
		else if (memcmp(name, "tsuitcommon", 11)==0 && strlen(name)==11)
		{
			*result = tsuitcommon();
		}
		else
		{
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}
	else if (memcmp(name, "nstraight", 9)==0)
	{
		if (memcmp(name, "nstraight", 9)==0 && strlen(name)==9)	
		{
			*result = nstraight();
		}
		else if (memcmp(name, "nstraightcommon", 15)==0 && strlen(name)==15)	
		{
			*result = nstraightcommon();
		}
		else if (memcmp(name, "nstraightfill", 13)==0 && strlen(name)==13)		
		{
			*result = nstraightfill();
		}
		else if (memcmp(name, "nstraightfillcommon", 19)==0 && strlen(name)==19)	
		{
			*result = nstraightfillcommon();
		}
		else if (memcmp(name, "nstraightflush", 14)==0 && strlen(name)==14)		
		{
			*result = nstraightflush();
		}
		else if (memcmp(name, "nstraightflushcommon", 20)==0 && strlen(name)==20)
		{
			*result = nstraightflushcommon();
		}
		else if (memcmp(name, "nstraightflushfill", 18)==0 && strlen(name)==18)
		{
			*result = nstraightflushfill();
		}
		else if (memcmp(name, "nstraightflushfillcommon", 24)==0 && strlen(name)==24)
		{
			*result = nstraightflushfillcommon();
		}
		else
		{
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}
	else if (memcmp(name, "nranked", 7)==0)
	{
		if (memcmp(name, "nranked", 7)==0 && strlen(name)==7)	
		{
			*result = nranked();
		}
		else if (memcmp(name, "nrankedcommon", 13)==0 && strlen(name)==13)
		{
			*result = nrankedcommon();
		}
		else
		{
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}
	else if (memcmp(name, "trank", 5)==0)
	{
		if (memcmp(name, "trank", 5)==0 && strlen(name)==5)
		{
			*result = trank();
		}
		else if (memcmp(name, "trankcommon", 11)==0 && strlen(name)==11)
		{
			*result = trankcommon();
		}
		else if (memcmp(name, "nouts", 5)==0 && strlen(name)==5)						
		{
			*result = nouts();
		}
		else
		{
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}
	else if (memcmp(name, "ncommoncardsknown", 17)==0 && strlen(name)==17)	
	{
		*result = p_symbol_engine_cards->ncommoncardsknown();
		return true;
	}
	// Symbol of a different symbol-engine
	return false;
}