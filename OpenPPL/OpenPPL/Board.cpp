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


#include "Board.h"
#include <assert.h>
#include <cctype> 
#include <iomanip>
#include <iostream>
#include "ErrorMessages.h"
#include "Main.h"

using namespace std;

void generate_code_for_hand_or_board_expression(CString hand_or_board_expression)
{
	// Precondition:
	//   Input looks like:
	//     "hand = AA"
	//     "hand = AT Suited"
	//     "Hand = A"
	//     "hand = A Suited"
	//     "Hand = AcTd"
	//     "Hand = AcT
	//     "Board = ..."
	// 
	// We translate this to something like "board$AKT"
	
	// First: remove superfluous spaces
	hand_or_board_expression.Replace("BOARD", "board");
	hand_or_board_expression.Replace("Board", "board");
	hand_or_board_expression.Replace("HAND", "hand");
	hand_or_board_expression.Replace("Hand", "hand");
	hand_or_board_expression.Replace("\t", "");
	hand_or_board_expression.Replace("   ", "");
	hand_or_board_expression.Replace("   ", "");
	hand_or_board_expression.Replace("  ", "");
	hand_or_board_expression.Replace(" ", "");
	hand_or_board_expression.Replace("=", "$");
	hand_or_board_expression.Replace("Su", "Su");
	hand_or_board_expression.Replace("su", "Su");
	hand_or_board_expression.Replace("SU", "SU");
	current_output << hand_or_board_expression;
}
