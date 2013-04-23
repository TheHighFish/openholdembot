#include "stdafx.h"
#include "CSymbolEngineLists.h"

#include "CBetroundCalculator.h"
#include "CFormula.h"
#include "CScraper.h"
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
{
	userchair = p_symbol_engine_userchair->userchair();
}

void CSymbolEngineLists::ResetOnNewRound()
{}

void CSymbolEngineLists::ResetOnMyTurn()
{
	// Calculate only preflop
	if (p_betround_calculator->betround() == k_betround_preflop)
	{
		CalculateLists();
	}
}

void CSymbolEngineLists::ResetOnHeartbeat()
{}

void CSymbolEngineLists::CalculateLists()
{

	unsigned int	card0, card1;

	if (StdDeck_RANK(p_scraper->card_player(userchair, 0)) 
		>= StdDeck_RANK(p_scraper->card_player(userchair, 1)))
	{
		card0 = p_scraper->card_player(userchair, 0);
		card1 = p_scraper->card_player(userchair, 1);
	}
	else
	{
		card0 = p_scraper->card_player(userchair, 1);
		card1 = p_scraper->card_player(userchair, 0);
	}

	int number_of_lists = p_formula->formula()->mHandList.GetSize();
	for (int i=0; i<number_of_lists; i++)
	{
		int listnum = atoi(p_formula->formula()->mHandList[i].list.Mid(4).GetString());

		if ((StdDeck_SUIT(card0)==StdDeck_SUIT(card1) && p_formula->formula()->inlist[listnum][StdDeck_RANK(card0)][StdDeck_RANK(card1)]) ||
			(StdDeck_SUIT(card0)!=StdDeck_SUIT(card1) && p_formula->formula()->inlist[listnum][StdDeck_RANK(card1)][StdDeck_RANK(card0)]))
		{
			SetIsList(listnum, true);													// islistxx
		}
		else
		{
			SetIsList(listnum, false);
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