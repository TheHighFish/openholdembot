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
#include "CSymbolEngineLists.h"

#include "CBetroundCalculator.h"
#include "CScraper.h"
#include "CSymbolEngineHistory.h"
#include "CSymbolEngineUserchair.h"
#include "OH_MessageBox.h"

CSymbolEngineLists *p_symbol_engine_lists = NULL;

CSymbolEngineLists::CSymbolEngineLists()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	assert(p_symbol_engine_userchair != NULL);
}

CSymbolEngineLists::~CSymbolEngineLists()
{}

void CSymbolEngineLists::InitOnStartup()
{}

void CSymbolEngineLists::ResetOnConnection()
{}

void CSymbolEngineLists::ResetOnHandreset()
{}

void CSymbolEngineLists::ResetOnNewRound()
{}

void CSymbolEngineLists::ResetOnMyTurn()
{
}

void CSymbolEngineLists::ResetOnHeartbeat()
{
	// Calculate after hand-reset up to my first action
	// as lists might be used for f$prefold too.
	// http://www.maxinmontreal.com/forums/viewtopic.php?f=234&t=16810
	if (!p_symbol_engine_history->DidActThisHand())
	{
		CalculateLists();
	}
}

void CSymbolEngineLists::CalculateLists()
{

	unsigned int	card0, card1;

	if (StdDeck_RANK(p_scraper->card_player(USER_CHAIR, 0)) 
		>= StdDeck_RANK(p_scraper->card_player(USER_CHAIR, 1)))
	{
		card0 = p_scraper->card_player(USER_CHAIR, 0);
		card1 = p_scraper->card_player(USER_CHAIR, 1);
	}
	else
	{
		card0 = p_scraper->card_player(USER_CHAIR, 1);
		card1 = p_scraper->card_player(USER_CHAIR, 0);
	}

	//!!!int number_of_lists = p_formula->formula()->mHandList.GetSize();
	//for (int i=0; i<number_of_lists; i++)
	{
		//!!!int listnum = atoi(p_formula->formula()->mHandList[i].list.Mid(4).GetString());

		//!!!if ((StdDeck_SUIT(card0)==StdDeck_SUIT(card1) && p_formula->formula()->inlist[listnum][StdDeck_RANK(card0)][StdDeck_RANK(card1)]) ||
		//	(StdDeck_SUIT(card0)!=StdDeck_SUIT(card1) && p_formula->formula()->inlist[listnum][StdDeck_RANK(card1)][StdDeck_RANK(card0)]))
		{
			//SetIsList(listnum, true);													// islistxx
		}
		//!!!else
		{
			//SetIsList(listnum, false);
		}
	}
}

void CSymbolEngineLists::ErrorListNumberOutOfRange(int list)
{
	CString message;
	message.Format("List out of range.\n"
		"%i should be in the range [0..999]", list);
	OH_MessageBox_Error_Warning(message, "ERROR");
}

bool CSymbolEngineLists::EvaluateSymbol(const char *name, double *result)
{
	if (memcmp(name, "islist", 6) == 0)	
	{
		*result = IsList(atoi(name+6));
		return true;
	}
	// Symbol of name different symbol-engine
	return false;
}