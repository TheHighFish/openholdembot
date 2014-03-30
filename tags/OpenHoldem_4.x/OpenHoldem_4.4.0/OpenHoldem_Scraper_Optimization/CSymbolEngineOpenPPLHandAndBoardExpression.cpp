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
#include "CSymbolEngineOpenPPLHandAndBoardExpression.h"

#include "CSymbolEngineCards.h"
#include "CSymbolEnginePokerval.h"
#include "CardFunctions.h"
#include "StringFunctions.h"

CSymbolEngineOpenPPLHandAndBoardExpression *p_symbol_engine_open_ppl_hand_and_board_expression = NULL;


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
{}

void CSymbolEngineOpenPPLHandAndBoardExpression::ResetOnHeartbeat()
{}

bool CSymbolEngineOpenPPLHandAndBoardExpression::ProcessQuery(CString hand_or_board_expression)
{
	// First check, if hand$ or board$ and/or Suited
	// At the same time remove the unnecessary parts of the expression
	bool is_hand_expression   = false;
	bool is_board_expression  = false;
	bool is_suited_expression = false;

	if (hand_or_board_expression.Left(5) == "hand$")
	{
		is_hand_expression = true;
		hand_or_board_expression = CStringRemoveLeft(
			hand_or_board_expression, 5);
	}
	else if (hand_or_board_expression.Left(6) == "board$")
	{
		is_board_expression = true;
		hand_or_board_expression = CStringRemoveLeft(
			hand_or_board_expression, 6);
	}
	assert(is_hand_expression || is_board_expression);

	if (hand_or_board_expression.Right(6).MakeLower() == "suited")
	{
		is_suited_expression;
		hand_or_board_expression = CStringRemoveRight(
			hand_or_board_expression, 6);
	}

	// Ranks in the expression (to be searched)
	int rankbits_to_be_searched = CardStringToRankbits(((char*)hand_or_board_expression.GetString()));
	// Cards available
	int rankbits_available = 0;
	if (is_hand_expression)
	{
		// rankbitsplayer is unfortunatelly defined as hand + common.
		// There is no symbol for the rankbits of our 2 card-hand.
		// So we build it here
		SetBit(&rankbits_available, p_symbol_engine_pokerval->rankhiplayer());
		SetBit(&rankbits_available, p_symbol_engine_pokerval->rankloplayer());
		if (is_suited_expression && !p_symbol_engine_cards->issuited())
		{
			// No suited ranks available
			rankbits_available = 0;
		}
	}
	else
	{
		if (is_suited_expression)
		{
			rankbits_available = p_symbol_engine_pokerval->rankbitscommon();
		}
		else
		{
			rankbits_available = p_symbol_engine_pokerval->srankbitscommon();
		}
	}
	
	// Check ranks in expression against available ranks
	if ((rankbits_to_be_searched & rankbits_available) != rankbits_to_be_searched)
	{
		return false;
	}

	// !!! TODO: multiple ranks like board$ATT, board$333

	// Cards with individual suits
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
					return false;
				}
			}
			else
			{
				if (!IsCardInCollection(icard_with_specific_suit,
					p_symbol_engine_cards->$$cc(0), p_symbol_engine_cards->$$cc(1),
					p_symbol_engine_cards->$$cc(2), p_symbol_engine_cards->$$cc(3),
					p_symbol_engine_cards->$$cc(4)))
				{
					return false;
				}
			}
		}	
	}
	return true;
}

