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

#ifndef INC_CARDFUNCTIONS_H
#define INC_CARDFUNCTIONS_H

// Supporting macros
#define HIGH_NIBBLE(c)	(((c)>>4)&0x0F)
#define LOW_NIBBLE(c)	((c)&0x0F)

// Rank and suits as used by the DLL
#define RANK(c)			HIGH_NIBBLE(c)
#define SUIT(c)			LOW_NIBBLE(c)
#define ISCARDBACK(c)	(c==0xFF)
#define ISUNKNOWN(c)	(c==0)

int RankAndSuitToCardNumber(int rank, int suit);
int CardStringToCardNumber(char* single_card);
int CardStringToRankbits(char* one_or_more_cards_cases_and_suits_dont_matter);

// Support for hand$xyz and board$xyz-symbols
// Checking if a card is in the hand or on the board
bool IsCardInCollection(int card, int col_card_0, int col_card_1,
						int opt_col_card_2 = CARD_NOCARD, 
						int opt_col_card_3 = CARD_NOCARD, 
						int opt_col_card_4 = CARD_NOCARD);

inline bool IsCardSuitCharacter(char c)
{
	return ((c == 'c') || c == 'd' || c == 'h' || c == 's'
		|| c == 'C' || c == 'D' || c == 'H' || c == 'S');
}

inline bool IsCardRankCharacter(char c)
{
	return (isdigit(c)  || c == 'T' || c == 'J'
		 || c == 'Q' || c == 'K' || c == 'A' || c == 't'
		 || c == 'j' || c == 'q' || c == 'k' || c == 'A');
}

#endif INC_CARDFUNCTIONS_H