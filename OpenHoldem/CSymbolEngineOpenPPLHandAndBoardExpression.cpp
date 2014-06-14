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
#include "CSymbolEngineOpenPPLHandAndBoardExpression.h"

#include "CSymbolEngineCards.h"
#include "CSymbolEnginePokerval.h"
#include "CardFunctions.h"
#include "CPreferences.h"
#include "StringFunctions.h"

CSymbolEngineOpenPPLHandAndBoardExpression *p_symbol_engine_open_ppl_hand_and_board_expression = NULL;

const int prime_coded_card_ranks[k_rank_ace+1] =
{
	1,	// rank = 0, unused, exists due to C++-definition
	1,	// rank = 1 = Ace-low, unused
	2,  // rank = 2
	3,  // rank = 3
	5,  // rank = 4
	7,  // rank = 5
	11, // rank = 6
	13, // rank = 7
	17, // rank = 8
	19, // rank = 9
	23, // rank = Ten
	29, // rank = Jack
	31, // rank = Queen
	37, // rank = King
	41, // rank = Ace-high
};

CSymbolEngineOpenPPLHandAndBoardExpression::CSymbolEngineOpenPPLHandAndBoardExpression()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	assert(p_symbol_engine_cards != NULL);
	assert(p_symbol_engine_pokerval != NULL);
}

CSymbolEngineOpenPPLHandAndBoardExpression::~CSymbolEngineOpenPPLHandAndBoardExpression()
{}

void CSymbolEngineOpenPPLHandAndBoardExpression::InitOnStartup()
{}

void CSymbolEngineOpenPPLHandAndBoardExpression::ResetOnConnection()
{}

void CSymbolEngineOpenPPLHandAndBoardExpression::ResetOnHandreset()
{}

void CSymbolEngineOpenPPLHandAndBoardExpression::ResetOnNewRound()
{}

void CSymbolEngineOpenPPLHandAndBoardExpression::ResetOnMyTurn()
{
	__TRACE
}

void CSymbolEngineOpenPPLHandAndBoardExpression::ResetOnHeartbeat()
{
	_prime_coded_hole_cards = PrimeCodedRanks(p_symbol_engine_cards->$$pc(0), 
		p_symbol_engine_cards->$$pc(1));
	_prime_coded_board_cards = PrimeCodedRanks(p_symbol_engine_cards->$$cc(0), 
		p_symbol_engine_cards->$$cc(1), p_symbol_engine_cards->$$cc(2), 
		p_symbol_engine_cards->$$cc(3),	p_symbol_engine_cards->$$cc(4));
	write_log(preferences.debug_symbolengine(), 
		"[CSymbolEngineOpenPPLHandAndBoardExpression] _prime_coded_hole_cards = %i\n",
		_prime_coded_hole_cards);
	write_log(preferences.debug_symbolengine(), 
		"[CSymbolEngineOpenPPLHandAndBoardExpression] _prime_coded_board_cards = %i\n",
		_prime_coded_board_cards);
}

bool CSymbolEngineOpenPPLHandAndBoardExpression::EvaluateSymbol(const char *name, double *result, bool log /* = false */)
{
	// First check, if hand$ or board$ and/or Suited
	// At the same time remove the unnecessary parts of the expression
	if (memcmp(name, "hand$", 5) == 0)
	{
		is_hand_expression  = true;
		is_board_expression = false;
		hand_or_board_expression = name;
		write_log(preferences.debug_symbolengine(), 
			"[CSymbolEngineOpenPPLHandAndBoardExpression] Evaluating %s\n",
			hand_or_board_expression);
		hand_or_board_expression = CStringRemoveLeft(hand_or_board_expression, 5);
		prime_coded_available_ranks = _prime_coded_hole_cards;
	}
	else if	(memcmp(name, "board$", 6) == 0)
	{
		is_hand_expression  = false;
		is_board_expression = true;
		hand_or_board_expression = name;
		write_log(preferences.debug_symbolengine(), 
			"[CSymbolEngineOpenPPLHandAndBoardExpression] Evaluating %s\n",
			hand_or_board_expression);
		hand_or_board_expression = CStringRemoveLeft(hand_or_board_expression, 6);
		prime_coded_available_ranks = _prime_coded_board_cards;
	}
	else
	{
		// Quick exit on other symbols
		return false;
	}

	bool is_suited_expression = false;
	assert(is_hand_expression || is_board_expression);

	if (hand_or_board_expression.Right(6).MakeLower() == "suited")
	{
		write_log(preferences.debug_symbolengine(), 
			"[CSymbolEngineOpenPPLHandAndBoardExpression] Suited expression\n");
		is_suited_expression = true;
		hand_or_board_expression = CStringRemoveRight(
			hand_or_board_expression, 6);
	}
	// Checking ranks with potential multiplicity > 1, not caring about suits.
	// We do this at the very beginning, as this is a very quick test
	// and most real-world-use-cases will be false, so we get a fast exit.
	int prime_coded_search_expression = PrimeCodedRanks(hand_or_board_expression);
	if ((prime_coded_available_ranks % prime_coded_search_expression) != 0)
	{
		// Division without reminder not possible.
		// Therefore different primes in the search-expression
		// Therefore ranks that do not fit available ranks.
		write_log(preferences.debug_symbolengine(), 
			"[CSymbolEngineOpenPPLHandAndBoardExpression] No match, because ranks do not fit\n");
		*result = false;
		return true;
	}

	// This was super-elegant, but unfortunatelly there can be 
	// SUITED expressions, which we can only solve with srankbits.
	// Ranks in the expression (to be searched)
	if (is_suited_expression)
	{
		int rankbits_to_be_searched = CardStringToRankbits(((char*)hand_or_board_expression.GetString()));
		write_log(preferences.debug_symbolengine(), 
			"[CSymbolEngineOpenPPLHandAndBoardExpression] rank bits for %s = %i\n",
			hand_or_board_expression.GetString(), rankbits_to_be_searched);
		if (is_hand_expression)
		{
			// Suited hand-expression
			// Ranks already checked, there are only 2, this simplifies things
			if (!p_symbol_engine_cards->issuited())
			{
				// No suited ranks available
				*result = false;
				return true;
			}
		}
		else
		{
			// Suited board-expression
			int rankbits_available = p_symbol_engine_pokerval->srankbitscommon();
	
			// Check ranks in expression against available ranks
			if ((rankbits_to_be_searched & rankbits_available) != rankbits_to_be_searched)
			{
				write_log(preferences.debug_symbolengine(),
					"[CSymbolEngineOpenPPLHandAndBoardExpression] No match, because suited rankbits do not fit\n");
				*result = false;
				return true;
			}
		}
	}

	// Third case: cards with individual suits
	int length = hand_or_board_expression.GetLength();
	for (int i=0; i<(length-1); i++)
	{
		if (IsCardRankCharacter(hand_or_board_expression[i])
			&& IsCardSuitCharacter(hand_or_board_expression[i+1]))
		{
			CString card_with_specific_suit =
				CString(hand_or_board_expression[i])
				+ CString(hand_or_board_expression[i+1]);
			int icard_with_specific_suit = CardStringToCardNumber(
				((char*)card_with_specific_suit.GetString()));

			// Check if card is on the board or in the hand
			if (is_hand_expression)
			{
				if (!IsCardInCollection(icard_with_specific_suit,
					p_symbol_engine_cards->$$pc(0), p_symbol_engine_cards->$$pc(1)))
				{
					write_log(preferences.debug_symbolengine(),
						"[CSymbolEngineOpenPPLHandAndBoardExpression] No match, concrete hole cards do not fit\n");
					*result = false;
					return true;
				}
			}
			else
			{
				if (!IsCardInCollection(icard_with_specific_suit,
					p_symbol_engine_cards->$$cc(0), p_symbol_engine_cards->$$cc(1),
					p_symbol_engine_cards->$$cc(2), p_symbol_engine_cards->$$cc(3),
					p_symbol_engine_cards->$$cc(4)))
				{
					write_log(preferences.debug_symbolengine(),
						"[CSymbolEngineOpenPPLHandAndBoardExpression] No match, concrete board cards do not fit\n");
					*result = false;
					return true;
				}
			}
		}	
	}
	write_log(preferences.debug_symbolengine(),
		"[CSymbolEngineOpenPPLHandAndBoardExpression] successful match\n");
	*result = true;
	return true;
}

int CSymbolEngineOpenPPLHandAndBoardExpression::PrimeCodedRanks(int card_0,
	int card_1, int opt_card_2, int opt_card_3, int opt_card_4)
{
	int result = 1;
	int ranks[k_number_of_community_cards];

	ranks[0] = RANK(card_0);
	ranks[1] = RANK(card_1);
	ranks[2] = RANK(opt_card_2);
	ranks[3] = RANK(opt_card_3);
	ranks[4] = RANK(opt_card_4);

	for (int i=0; i<k_number_of_community_cards; i++)
	{
		assert(ranks[i] >= 0);
		assert(ranks[i] <= k_rank_ace);
		result *= prime_coded_card_ranks[ranks[i]];
	}
	return result;
}

int CSymbolEngineOpenPPLHandAndBoardExpression::PrimeCodedRanks(CString card_expression)
{
	int result = 1;
	int length = card_expression.GetLength();

	for (int i=0; i<length; i++)
	{
		char next_character = card_expression[i];
		if (!IsCardRankCharacter(next_character))
		{
			// The expression might contain suits,
			// so simply continue on unixpected characters
			continue;
		}
		int rank = CardRankCharacterToCardRank(next_character);
		result *= prime_coded_card_ranks[rank];
	}
	return result;
}

CString CSymbolEngineOpenPPLHandAndBoardExpression::SymbolsProvided() {
  return "hand$ board$";
}