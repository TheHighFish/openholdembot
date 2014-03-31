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
#include "CopenHoldemStatusbar.h"

#include "CAutoplayerFunctions.h"
#include "CGameState.h"
#include "CIteratorThread.h"
#include "CIteratorVars.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineHandrank.h"
#include "CSymbolEnginePrwin.h"
#include "CSymbolEngineUserchair.h"
#include "CSymbols.h"
#include "MagicNumbers.h"


COpenHoldemStatusbar *p_openholdem_statusbar = NULL;

COpenHoldemStatusbar::COpenHoldemStatusbar(CWnd *main_window)
{
	_main_window = main_window;
	InitBasicStatusbar();
}

COpenHoldemStatusbar::~COpenHoldemStatusbar()
{}

void COpenHoldemStatusbar::InitBasicStatusbar()
{
	//is_basic_statusbar = true;
	//_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_GENERAL_HINT), "Do you need Help -> Problem Solver?");
	
	InitAdvancedStatusbar();
}

void COpenHoldemStatusbar::InitAdvancedStatusbar()
{
	_status_bar.Create(_main_window);
	_status_bar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	_status_bar.SetPaneInfo(0, ID_INDICATOR_STATUS_READY, NULL, 100);
	_status_bar.SetPaneInfo(1, ID_INDICATOR_STATUS_PLCARDS, NULL, 30);
	_status_bar.SetPaneInfo(2, ID_INDICATOR_STATUS_COMCARDS, NULL, 75);
	_status_bar.SetPaneInfo(3, ID_INDICATOR_STATUS_POKERHAND, NULL, 65);	
	_status_bar.SetPaneInfo(4, ID_INDICATOR_STATUS_HANDRANK, NULL, 55);
	_status_bar.SetPaneInfo(5, ID_INDICATOR_STATUS_PRWIN, NULL, 62);
	_status_bar.SetPaneInfo(6, ID_INDICATOR_STATUS_NOPP, NULL, 15);
	_status_bar.SetPaneInfo(7, ID_INDICATOR_STATUS_NIT, NULL, 90);
	_status_bar.SetPaneInfo(8, ID_INDICATOR_STATUS_ACTION, SBPS_STRETCH, 70);

	is_basic_statusbar = false;
}

void COpenHoldemStatusbar::SwitchToAdvancedStatusbarAfterFirstHand()
{
	if (!is_basic_statusbar)
	{
		// Already advanced statusbar
		return;
	}
	// Still basic statusbar for beginners
	if (p_game_state->hands_played() >= 1)
	{
		// Bot seems to play -> no beginners advice needed
		// -> switch to normal "advanced" status-bar.
		InitAdvancedStatusbar();
	}
}

void COpenHoldemStatusbar::GetWindowRect(RECT *statusbar_position)
{
	_status_bar.GetWindowRect(statusbar_position);
}

void COpenHoldemStatusbar::OnUpdateStatusbar()
{
	if (is_basic_statusbar)
	{
		// Basic statusbar for beginners with basic hints
		// We can't display detailed info.
		return;
	}
	// Update this info only for advanced statusbar
	ComputeCurrentStatus();
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_READY), "Ready"); 
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_PLCARDS), _status_plcards);
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_COMCARDS), _status_comcards);
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_POKERHAND), _status_pokerhand);
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_HANDRANK), _status_handrank);
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_PRWIN), _status_prwin);
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_NOPP), _status_nopp);
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_NIT), _status_nit);
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_ACTION), _status_action);
}


void COpenHoldemStatusbar::ComputeCurrentStatus()
{
	CardMask	Cards;
	CString		temp;

	if (p_symbol_engine_userchair == NULL)
	{
		// Very early phase of initialization
		// Can't continue here.
		_status_action = "Not playing";
		return;
	}
	if (!p_symbol_engine_userchair->userchair_confirmed())
	{
		_status_action = "Not playing";
		return;
	}
	int userchair = p_symbol_engine_userchair->userchair();
	
	// Player cards
	CardMask_RESET(Cards);
	int nCards = 0;
	_status_plcards = "";
	 
	if (p_scraper_access->UserHasCards())
	{
		for (int i=0; i<k_number_of_cards_per_player; i++) 
		{
			// This condition got already checked: "playing"
			assert(p_scraper->card_player(userchair, i) != CARD_BACK);
			assert(p_scraper->card_player(userchair, i) != CARD_NOCARD); 


			char *card = StdDeck_cardString(p_scraper->card_player(userchair, i));
			temp.Format("%s ", card);
			_status_plcards.Append(temp);
			CardMask_SET(Cards, p_scraper->card_player(userchair, i));
			nCards++;
		}
		_status_nopp.Format("%d", p_symbol_engine_prwin->nopponents_for_prwin());
	}
	else 
	{
		for (int i=0; i<k_number_of_cards_per_player; i++) 
		{
			if (p_scraper_access->UserHasCards())
			{
				char *card = StdDeck_cardString(p_scraper_access->GetPlayerCards(userchair, i));
				temp.Format("%s ", card);
				_status_plcards.Append(temp);
				CardMask_SET(Cards, p_scraper_access->GetPlayerCards(userchair, i));
				nCards++;
			}
		}
		// Not playing, therefore no opponents to be considered for prwin.
		_status_nopp = "";
	}

	// Common cards
	_status_comcards = "";
	for (int i=0; i<k_number_of_community_cards; i++) 
	{
		if (p_scraper->card_common(i) != CARD_BACK && 
			p_scraper->card_common(i) != CARD_NOCARD) 
		{
			char *card = StdDeck_cardString(p_scraper->card_common(i));
			temp.Format("%s ", card);
			_status_comcards.Append(temp);
			CardMask_SET(Cards, p_scraper->card_common(i));
			nCards++;
		}
	}

	// poker hand
	HandVal hv = Hand_EVAL_N(Cards, nCards);
	char hvstring[100] = {0};
	HandVal_toString(hv, hvstring);
	_status_pokerhand = hvstring;
	_status_pokerhand = _status_pokerhand.Mid(0, _status_pokerhand.Find(" "));

	// Always use handrank169 here
	_status_handrank.Format("%.0f/169", p_symbol_engine_handrank->handrank169());

	// Always update prwin/nit
	if (p_symbol_engine_userchair->userchair_confirmed() 
		&& p_scraper_access->UserHasCards())
	{
		_status_prwin.Format("%d/%d/%d", 
			(int) (iter_vars.prwin()*1000), 
			(int) (iter_vars.prtie()*1000),
			(int) (iter_vars.prlos()*1000));
		int	e = SUCCESS;
		_status_nit.Format("%d/%d", 
			iter_vars.iterator_thread_progress(),
			(int) p_symbols->GetSymbolVal("f$prwin_number_of_iterations", &e));
	}
	else
	{
		_status_prwin = "0/0/0";
		// No iteratrions without userchair or cards
		_status_nit.Format("0");
	}

	// action
	if (p_autoplayer_functions->f$prefold())
	{
		_status_action = "Pre-fold";
	}
	else if (p_symbol_engine_userchair->userchair_confirmed() && iter_vars.iterator_thread_complete())
	{
		if (!p_symbol_engine_autoplayer->isfinalanswer())	_status_action = "N/A";
		else if (p_autoplayer_functions->f$alli())			_status_action = "Allin";
		else if (p_autoplayer_functions->f$betsize())		_status_action.Format("Betsize: %.2f", p_autoplayer_functions->f$betsize());
		else if (p_autoplayer_functions->f$rais())			_status_action = "Bet/Raise";
		else if (p_autoplayer_functions->f$call())			_status_action = "Call/Check";
		else  _status_action = "Fold/Check";
	}

	else if (p_symbol_engine_prwin->nopponents_for_prwin()==0)
		_status_action = "Idle (f$prwin_number_of_opponents==0)";
	else
		_status_action = "Thinking";
}
