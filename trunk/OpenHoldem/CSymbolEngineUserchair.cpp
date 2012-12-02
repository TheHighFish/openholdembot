#include "stdafx.h"
#include "CSymbolEngineUserchair.h"

#include "CScraper.h"
#include "CStringMatch.h"
#include "MagicNumbers.h"

CSymbolEngineUserchair::CSymbolEngineUserchair()
{}

CSymbolEngineUserchair::~CSymbolEngineUserchair()
{}

void CSymbolEngineUserchair::InitOnStartup()
{
	ResetOnConnection();
}

void CSymbolEngineUserchair::ResetOnConnection()
{
	_userchair = k_undefined;
	_userchair_locked = false;
}

void CSymbolEngineUserchair::ResetOnHandreset()
{
	// !!! TODO: option to reset it on every hand-reset 
	// (for table-changes in tournaments)
}

void CSymbolEngineUserchair::ResetOnNewRound()
{}

void CSymbolEngineUserchair::ResetOnMyTurn()
{}

void CSymbolEngineUserchair::ResetOnHeartbeat()
{
	if (!_userchair_locked)
	{
		CalculateUserChair();
	}
}

void CSymbolEngineUserchair::CalculateUserChair()
{
	// !!! To be refactored into ScraperAccess
	int num_buttons_enabled = 0;
	for (int i=0; i<k_max_number_of_players; i++)
	{
		CString button_label = p_scraper->button_label(i);

		if (p_scraper->GetButtonState(i) &&
			(p_string_match->IsStringFold(button_label) ||
			 p_string_match->IsStringCheck(button_label) ||
			 p_string_match->IsStringCall(button_label) ||
			 p_string_match->IsStringRaise(button_label) ||
			 p_string_match->IsStringAllin(button_label) ||
			 button_label.MakeLower() == "swag"))
		{
			num_buttons_enabled++;
		}
	}

	if (num_buttons_enabled >= k_min_buttons_needed_for_my_turn)
	{
		for (int i=0; i<p_tablemap->nchairs(); i++)
		{
			if (p_scraper->card_player(i, 0) != CARD_NOCARD && p_scraper->card_player(i, 0) != CARD_BACK &&
				p_scraper->card_player(i, 1) != CARD_NOCARD && p_scraper->card_player(i, 1) != CARD_BACK)
			{
				_userchair = i;
				_userchair_locked = true;
				return;
			}
		}
	}
}