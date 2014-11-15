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

#include "stdafx.h"
#include "CopenHoldemStatusbar.h"

#include "CAutoplayerFunctions.h"
#include "CAutoplayerTrace.h"
#include "CEngineContainer.h"
#include "CFunctionCollection.h"
#include "CGameState.h"
#include "CIteratorThread.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineHandrank.h"
#include "CSymbolEnginePrwin.h"
#include "CSymbolEngineUserchair.h"
#include "CTableState.h"
#include "MagicNumbers.h"
#include "StringFunctions.h"

COpenHoldemStatusbar *p_openholdem_statusbar = NULL;

COpenHoldemStatusbar::COpenHoldemStatusbar(CWnd *main_window){
	_main_window = main_window;
  InitAdvancedStatusbar();
}

COpenHoldemStatusbar::~COpenHoldemStatusbar() {
}

void COpenHoldemStatusbar::InitAdvancedStatusbar() {
	_status_bar.Create(_main_window);
	_status_bar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	_status_bar.SetPaneInfo(0, ID_INDICATOR_STATUS_ACTION, NULL, 100);
	_status_bar.SetPaneInfo(1, ID_INDICATOR_STATUS_PLCARDS, NULL, 30);
	_status_bar.SetPaneInfo(2, ID_INDICATOR_STATUS_COMCARDS, NULL, 75);
	_status_bar.SetPaneInfo(3, ID_INDICATOR_STATUS_POKERHAND, NULL, 65);	
	_status_bar.SetPaneInfo(4, ID_INDICATOR_STATUS_HANDRANK, NULL, 55);
	_status_bar.SetPaneInfo(5, ID_INDICATOR_STATUS_PRWIN, NULL, 62);
	_status_bar.SetPaneInfo(6, ID_INDICATOR_STATUS_NOPP, NULL, 15);
	_status_bar.SetPaneInfo(7, ID_INDICATOR_STATUS_NIT, SBPS_STRETCH, 90);
}

void COpenHoldemStatusbar::GetWindowRect(RECT *statusbar_position)
{
	_status_bar.GetWindowRect(statusbar_position);
}

void COpenHoldemStatusbar::OnUpdateStatusbar() {
	ComputeCurrentStatus(); 
  _status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_ACTION), _status_action);
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_PLCARDS), _status_plcards);
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_COMCARDS), _status_comcards);
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_POKERHAND), _status_pokerhand);
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_HANDRANK), _status_handrank);
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_PRWIN), _status_prwin);
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_NOPP), _status_nopp);
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_NIT), _status_nit);
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
	 
  if (p_table_state->User()->HasKnownCards()) {
		for (int i=0; i<k_number_of_cards_per_player; i++) {	
			// This condition got already checked: "playing"
      Card card = p_table_state->User()->_hole_cards[i];
      // Assertion removeed, because the scraper runs in a different thread.
			// assert(card.IsKnownCard()); 
		  _status_plcards.Append(card.ToString());
      CardMask_SET(Cards, card.GetValue());
			nCards++;
		}
		_status_nopp.Format("%d", p_symbol_engine_prwin->nopponents_for_prwin());
	}	else 	{
		for (int i=0; i<k_number_of_cards_per_player; i++) {
			if (p_table_state->User()->HasKnownCards())
			{
				Card card = p_table_state->User()->_hole_cards[i];
				_status_plcards.Append(card.ToString());
        CardMask_SET(Cards, card.GetValue());
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
    Card card = p_table_state->_common_cards[i];
		if (card.IsKnownCard())
		{
			_status_comcards.Append(card.ToString());
			CardMask_SET(Cards, card.GetValue());
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
		  && p_table_state->User()->HasKnownCards()) {
		_status_prwin.Format("%d/%d/%d", 
			(int) (p_iterator_thread->prwin()*1000), 
			(int) (p_iterator_thread->prtie()*1000),
			(int) (p_iterator_thread->prlos()*1000));
		double iterations;
		p_engine_container->EvaluateSymbol("f$prwin_number_of_iterations", &iterations);
		_status_nit.Format("%d/%s", 
			p_iterator_thread->IteratorThreadProgress(),
			Number2CString(iterations));
	}	else {
		_status_prwin = "0/0/0";
		// No iteratrions without userchair or cards
		_status_nit.Format("0");
	}
	// action
  if (p_function_collection->EvaluateAutoplayerFunction(k_standard_function_prefold)) {
		_status_action = "Pre-fold";
	}
	else if (p_symbol_engine_userchair->userchair_confirmed() 
      && p_iterator_thread->IteratorThreadComplete()) {
		if (!p_symbol_engine_autoplayer->isfinalanswer())	{
      _status_action = "N/A";
    } else {
      _status_action = p_autoplayer_trace->BestAction();
    }
	} else if (p_symbol_engine_prwin->nopponents_for_prwin()==0) {
		_status_action = "Idle (f$prwin_number_of_opponents==0)";
  } else {
		_status_action = "Thinking";
  }
}
