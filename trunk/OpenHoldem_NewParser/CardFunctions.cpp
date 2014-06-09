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
#include "CardFunctions.h"

#include <assert.h>
#include "MagicNumbers.h"
#include "NumericalFunctions.h"

int RankCharacterToRankNumber(char rank)
{
	switch (rank)
	{
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		case 't': 
		case 'T': return 10;
		case 'j':
		case 'J': return 11;
		case 'q':
		case 'Q': return 12;
		case 'k':
		case 'K': return 13;
		case 'a':
		case 'A': // Ace high
			return 14;
		default: 
			// Just accept silently
			// We might get input like "Ac3d" or "KJTsuited"
			assert(false);
			return -1;
	}
}

int SuitCharacterToSuitNumber(char suit)
{
	switch (suit)
	{
		case 'c': 
		case 'C': return 0;
		case 'd':
		case 'D': return 1;
		case 'h':
		case 'H': return 2;
		case 's':
		case 'S': return 3;
		default: // Fail silently
			assert(false);
			return -1;
	}
}

int RankAndSuitToCardNumber(int rank, int suit)
{
	// !! Completely bad unnamed constands
	// * std_deck
	// * WH-constants 1..4
	// * DLL-constants 0..3 (need to define names somewhere)
	assert(rank >= k_rank_ace_low);
	assert(rank <= k_rank_ace);  
	assert(suit >= 0);
	assert(suit < 4);  
	return ((rank << 4) + suit);
}

int CardStringToCardNumber(char* single_card)
{
	assert(strlen(single_card) == 2);
	int rank = RankCharacterToRankNumber(single_card[0]);
	int suit = SuitCharacterToSuitNumber(single_card[1]);
	return RankAndSuitToCardNumber(rank, suit);
}

int CardStringToRankbits(char* one_or_more_cards_cases_and_suits_dont_matter)
{
	int rankbits = 0;
	int length = strlen(one_or_more_cards_cases_and_suits_dont_matter);
	for (int i = 0; i<length; i++)
	{
		char next_character = one_or_more_cards_cases_and_suits_dont_matter[i];
		int rank = RankCharacterToRankNumber(next_character);
		SetBit(&rankbits, rank);
		if (next_character == 'a' || next_character == 'A')
		{
			SetBit(&rankbits, k_rank_ace_low);
		}	
	}
	return rankbits;
}

bool IsCardInCollection(int card, int col_card_0, int col_card_1,
						int opt_col_card_2, 
						int opt_col_card_3, 
						int opt_col_card_4)
{
	return ((card == col_card_0) ||  (card == col_card_1)
		|| (card == opt_col_card_2) || (card == opt_col_card_3)
		|| (card == opt_col_card_4));
}