#include "Board.h"
#include <assert.h>
#include <iomanip>
#include <iostream>

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
	cout << "(srankbits & " 
		 // hexadecimal output to cout or a stream according to
		 // http://www.math.uni-bayreuth.de/~rbaier/lectures/c++_intro/html/node51.html
		 << hex << showbase << user_defined_rankbits
		 << " == " 
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
	cout << "(f$prime_coded_board_ranks % (1";
	for (int i=5; i<board_expression.GetLength(); i++)
	{
		char next_card = board_expression.MakeLower()[i];
		// Process cards, skip the rest (blanks, etc.)
		switch (next_card)
		{
			case '2': cout << " * f$PrimeCode_2"; break;
			case '3': cout << " * f$PrimeCode_3"; break;
			case '4': cout << " * f$PrimeCode_4"; break;
			case '5': cout << " * f$PrimeCode_5"; break;
			case '6': cout << " * f$PrimeCode_6"; break;
			case '7': cout << " * f$PrimeCode_7"; break;
			case '8': cout << " * f$PrimeCode_8"; break;
			case '9': cout << " * f$PrimeCode_9"; break;
			case 't': cout << " * f$PrimeCode_T"; break;
			case 'j': cout << " * f$PrimeCode_J"; break;
			case 'q': cout << " * f$PrimeCode_Q"; break;
			case 'k': cout << " * f$PrimeCode_K"; break;
			case 'a': cout << " * f$PrimeCode_A"; break;
			default: break;
		}
	}
	cout << ") == 0)";
}