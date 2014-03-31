#include "Board.h"
#include <assert.h>
#include <cctype> 
#include <iomanip>
#include <iostream>
#include "ErrorMessages.h"
#include "Main.h"

using namespace std;

void generate_code_for_suited_board(CString board_expression)
{
	assert(board_expression.Left(5).MakeLower() == "board");
	int user_defined_rankbits = 0;
	for (int i=5; i<board_expression.GetLength(); i++)
	{
		char next_card = board_expression.MakeLower()[i];
		// Process cards, skip the rest (blanks, etc.)
		switch (next_card)
		{
			case '2': user_defined_rankbits |= 0x0004; break;
			case '3': user_defined_rankbits |= 0x0008; break;
			case '4': user_defined_rankbits |= 0x0010; break;
			case '5': user_defined_rankbits |= 0x0020; break;
			case '6': user_defined_rankbits |= 0x0040; break;
			case '7': user_defined_rankbits |= 0x0080; break;
			case '8': user_defined_rankbits |= 0x0100; break;
			case '9': user_defined_rankbits |= 0x0200; break;
			case 't': user_defined_rankbits |= 0x0400; break;
			case 'j': user_defined_rankbits |= 0x0800; break;
			case 'q': user_defined_rankbits |= 0x1000; break;
			case 'k': user_defined_rankbits |= 0x2000; break;
			case 'a':
				{
					// Ace is both high and low.
					user_defined_rankbits |= 0x4002;
					break;
				}
			case 's': 
				{
					// Terminate of loop as soon as we reach the "s" of "suited"
					// We don't want to process the "t" as "Ten".
					// "Break" doesn't help here, so we use the super-cool...
					goto End_Of_Loop;
				}
			default: break;
		}
	}
End_Of_Loop:
	current_output << "([srankbitscommon & " 
		 // hexadecimal output to current_output or a stream according to
		 // http://www.math.uni-bayreuth.de/~rbaier/lectures/c++_intro/html/node51.html
		 << hex << showbase << user_defined_rankbits
		 << "] == " 
		 << hex << showbase << user_defined_rankbits
		 << ")";
}

// Prime-coded board ranks
// Cards may appear more than once, e.g. "board = ATT",
// therefore we can't work with rankbits, but do some prime-coding, 
// so that we can easily check for multiple cards.
//
//  Card:	2	3	4	5	6	7	8	9	T	J	Q	K	A
//	Coding:	2	3	5	7	11	13	17	19	23	29	31	37	41
//
// Example: prime-coding for ATT = 41 * 23 * 23 = 21689
// Therefore a board contains ATT, exactly if its prime-coding
// can be divided by 21689 without a reminder.

void generate_code_for_non_suited_board(CString board_expression)
{
	assert(board_expression.Left(5).MakeLower() == "board");
	current_output << "(f$prime_coded_board_ranks % (1";
	for (int i=5; i<board_expression.GetLength(); i++)
	{
		char next_card = board_expression.MakeLower()[i];
		// Process cards, skip the rest (blanks, etc.)
		switch (next_card)
		{
			case '2': current_output << " * f$PrimeCode_2"; break;
			case '3': current_output << " * f$PrimeCode_3"; break;
			case '4': current_output << " * f$PrimeCode_4"; break;
			case '5': current_output << " * f$PrimeCode_5"; break;
			case '6': current_output << " * f$PrimeCode_6"; break;
			case '7': current_output << " * f$PrimeCode_7"; break;
			case '8': current_output << " * f$PrimeCode_8"; break;
			case '9': current_output << " * f$PrimeCode_9"; break;
			case 't': current_output << " * f$PrimeCode_T"; break;
			case 'j': current_output << " * f$PrimeCode_J"; break;
			case 'q': current_output << " * f$PrimeCode_Q"; break;
			case 'k': current_output << " * f$PrimeCode_K"; break;
			case 'a': current_output << " * f$PrimeCode_A"; break;
			default: break;
		}
	}
	current_output << ") == 0)";
}

void generate_code_for_hand_expression(CString hand_expression)
{
	// Translates something like KQ to $KQ
	// and A Suited to $AXs
	int number_of_cards = 0;

	current_output << CString("$");
	int length = hand_expression.GetLength();
	for (int i=0; i<length; i++)
	{
		char next_char = toupper(hand_expression[i]);
		if (next_char >= '0' && next_char <= '9')
		{
			current_output << CString(next_char);
			number_of_cards++;
		}
		else if ((next_char == 'T') || (next_char == 'J')
			|| (next_char == 'Q') || (next_char == 'K') || (next_char == 'A'))
		{
			current_output << CString(next_char);
			number_of_cards++;
		}
		// Cards processed and end or keyword "suited" reached?
		else if (i >= (length - 1) || next_char == 'S')
		{
			// Fewer than 2 cards specified?
			// Then we have to generate code like "$AX Suited" or "$XX";
			if (number_of_cards == 1)
			{
				current_output << CString("X");
			}
			else if (number_of_cards == 0)
			{
				current_output << CString("XX");
			}
			// Suited?
			if (next_char == 'S')
			{
				current_output << CString("s");
				// Stop processing this part of the input,
				// as the rest is not relevant.
				break;
			}
		}
	}
	if (number_of_cards > 2)
	{
		ErrorMessage(k_error_too_many_cards_in_hand, hand_expression);
	}
}

bool IsSuit(char s)
{
	switch (tolower(s))
	{
		case 'c': return true;
		case 'd': return true;
		case 'h': return true;
		case 's': return true;
		default: return false;
	}
}

void generate_code_for_hand_expression_with_specific_suits(CString hand_expression)
{
	// Input: a expression like "AcJs" or "KdQ" or even only "Kc".

	// Remove superfluos spaces...
	hand_expression.Remove(' ');
	current_output << "[ /* " << hand_expression << " */ ";
	// ... and add 4 spaces at the end to avoid out-of-range-errors
	hand_expression += "    ";

	char first_card_rank  = toupper(hand_expression[0]);
	char first_card_suit  = tolower(hand_expression[1]);
	char second_card_rank = toupper(hand_expression[2]);
	char second_card_suit = tolower(hand_expression[3]);

	CString code_for_card_0 = "1";
	CString code_for_card_1 = "1";

	if (IsSuit(first_card_suit))
	{
		code_for_card_0 = CString("f$PrimeCode_") 
			+ CString(first_card_rank) + CString(first_card_suit);
	}
	if (IsSuit(second_card_suit))
	{
		code_for_card_1 = CString("f$PrimeCode_") 
			+ CString(second_card_rank) + CString(second_card_suit);
	}

	// For hands like AcT we first have to check AT, before we can check Ac.
	// Otherwise we would miss cards without suits in our second check.
	CString hand_expression_without_suits = CString(first_card_rank)
		+ CString(second_card_rank);
	generate_code_for_hand_expression(hand_expression_without_suits);
	current_output << " && ";

	// Now code for cards with specific suits
	current_output << CString("[f$prime_coded_hole_cards % [1 * ") 
		<< code_for_card_0 << CString(" * ") << code_for_card_1
		<< CString("] == 0]]");
}
