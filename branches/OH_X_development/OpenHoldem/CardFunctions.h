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

#ifndef INC_CARDFUNCTIONS_H
#define INC_CARDFUNCTIONS_H

// Supporting macros
#define HIGH_NIBBLE(c)	(((c)>>4)&0x0F)
#define LOW_NIBBLE(c)	((c)&0x0F)

// Rank and suits as used by the DLL
#define RANK(c)			  ( ISKNOWN(c) ? HIGH_NIBBLE(c) : 0 )
#define SUIT(c)			  ( ISKNOWN(c) ? LOW_NIBBLE(c) : 0 )
#define ISCARDBACK(c)	((c) == CARD_BACK)
#define ISUNKNOWN(c)	((c) == CARD_UNDEFINED)
#define ISNOCARD(c)		((c) == CARD_NOCARD)
#define ISKNOWN(c)    (!ISCARDBACK(c) && !ISUNKNOWN(c) && !ISNOCARD(c))

int RankAndSuitToCardNumber(int rank, int suit);
int CardStringToCardNumber(char* single_card);
int CardStringToRankbits(char* one_or_more_cards_cases_and_suits_dont_matter);

bool IsRankString(CString rank);
bool IsSuitString(CString suit);

// Support for hand$xyz and board$xyz-symbols
// Checking if a card is in the hand or on the board
bool IsCardInCollection(int card, int col_card_0, int col_card_1,
						int opt_col_card_2 = CARD_UNDEFINED, 
						int opt_col_card_3 = CARD_UNDEFINED, 
						int opt_col_card_4 = CARD_UNDEFINED);

inline bool IsCardSuitCharacter(char c)
{
	return ((c == 'c') || c == 'd' || c == 'h' || c == 's'
		|| c == 'C' || c == 'D' || c == 'H' || c == 'S');
}

inline bool IsCardRankCharacter(char c)
{
	return (isdigit(c)  || c == 'T' || c == 'J'
		 || c == 'Q' || c == 'K' || c == 'A' || c == 't'
		 || c == 'j' || c == 'q' || c == 'k' || c == 'a');
}

inline int CardRankCharacterToCardRank(char rank)
{
	switch (rank)
	{
		case '2': return  2;
		case '3': return  3;
		case '4': return  4;
		case '5': return  5;
		case '6': return  6;
		case '7': return  7;
		case '8': return  8;
		case '9': return  9;
		case 'T': return 10;
		case 't': return 10;
		case 'J': return 11;
		case 'j': return 11;
		case 'Q': return 12;
		case 'q': return 12;
		case 'K': return 13;
		case 'k': return 13;
		case 'A': return 14;
		case 'a': return 14;
		default: 
          // Just accept silently
		  // We might get input like "Ac3d" or "KJTsuited"
		  assert(false);
		  return -1;
	}
}

inline int OpenHoldemRankToStdDeckLibraryRank(int rank)	{ return (rank - 2); }

#endif INC_CARDFUNCTIONS_H