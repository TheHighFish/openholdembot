//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: This module provides:
//     * a series of 256 gamestates for the DLL
//   In the past it also was responsible for
//     * history and action symbols
//     * table statistics
//     * creating a summary for the log (hand-history) 
//   But after refactoring this module creates just gamestates
//   and nothing else.
//
//******************************************************************************

#include "stdafx.h"
#include "CGameState.h"

#include "assert.h"
#include "CAutoConnector.h"
#include "CBetroundCalculator.h"
#include "CDllExtension.h"
#include "CEngineContainer.h"
#include "CHandresetDetector.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineCards.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineDealerchair.h"
#include "CSymbolEngineHistory.h"
#include "CSymbolEnginePrwin.h"
#include "CSymbolEngineTime.h"
#include "CSymbolEngineUserchair.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CPreferences.h"
#include "CSymbolEngineTableLimits.h"
#include "CTableState.h"
#include "MagicNumbers.h"
#include "Numericalfunctions.h"

CGameState			*p_game_state = NULL;

CGameState::CGameState() {
	state_index = 0;
}

CGameState::~CGameState() {
}

void CGameState::CaptureState() {
	bool				    playing = true;
	unsigned char		card = CARD_NOCARD;

	// figure out if I am playing
	int sym_chair = p_symbol_engine_userchair->userchair();
	if (!p_symbol_engine_userchair->userchair_confirmed()) 	{
		playing = false;
	}
	else if (!p_table_state->User()->HasKnownCards())	{
		playing = false;
	}

	// Poker window title
	char title[MAX_WINDOW_TITLE];
	GetWindowText(p_autoconnector->attached_hwnd(), title, MAX_WINDOW_TITLE);

	strncpy_s(state[state_index&0xff].m_title, 64, title, _TRUNCATE);
	state[state_index&0xff].m_title[63] = '\0';

	// Pot information
	for (int i=0; i<kMaxNumberOfPlayers; i++) {
		state[state_index&0xff].m_pot[i] = p_table_state->Pot(i);
  }
	// Common cards
	for (int i=0; i<kNumberOfCommunityCards; i++)	{
    int common_card = p_table_state->CommonCards(i)->GetValue();
     write_log(preferences.debug_dll_extension(), 
      "[CGameState] Common card %i = %i\n", i, common_card);
		state[state_index&0xff].m_cards[i] = common_card;
	}

	// playing, posting, dealerchair
	int sym_dealerchair = p_symbol_engine_dealerchair->dealerchair();
	bool sym_isautopost = p_symbol_engine_autoplayer->isautopost();
	state[state_index&0xff].m_is_playing = playing;
	state[state_index&0xff].m_is_posting = sym_isautopost;
	state[state_index&0xff].m_fillerbits = 0;
	state[state_index&0xff].m_fillerbyte = 0;
	state[state_index&0xff].m_dealer_chair = sym_dealerchair;

	// loop through all 10 player chairs
	for (int i=0; i<kMaxNumberOfPlayers; i++) {
    // player name, balance, currentbet
    strncpy_s(state[state_index&0xff].m_player[i].m_name, 16, p_table_state->Player(i)->name().GetString(), _TRUNCATE);
    state[state_index&0xff].m_player[i].m_balance = p_table_state->Player(i)->balance();
		state[state_index&0xff].m_player[i].m_currentbet = p_table_state->Player(i)->bet();

		// player cards
		for (int j=0; j<kNumberOfCardsPerPlayer; j++) {
      Card* player_card = p_table_state->Player(i)->hole_cards(j);
      int card = player_card->GetValue();
       write_log(preferences.debug_dll_extension(),
        "[CGameState] Player card [%i][%i] = %i\n", i, j, card);
			//!!!!!
       state[state_index&0xff].m_player[i].m_cards[j] = card;
		}

		// player name known, balance known
		state[state_index&0xff].m_player[i].m_name_known = p_scraper_access->IsGoodPlayername(i);
		state[state_index&0xff].m_player[i].m_balance_known = true;
		state[state_index&0xff].m_player[i].m_fillerbits = 0;
		state[state_index&0xff].m_player[i].m_fillerbyte = 0;
	}
  state_index++;
}

void CGameState::DumpState(void) {
	 write_log(preferences.debug_alltherest(), "[CGameState] m_ndx: %d\n", state_index);
	 write_log(preferences.debug_alltherest(), "[CGameState] _title: %s\n", state[(state_index)&0xff].m_title);
	 write_log(preferences.debug_alltherest(), "[CGameState] _pot: %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n", 
    state[(state_index)&0xff].m_pot[0], state[(state_index)&0xff].m_pot[1],
		state[(state_index)&0xff].m_pot[2], state[(state_index)&0xff].m_pot[3], 
    state[(state_index)&0xff].m_pot[4], state[(state_index)&0xff].m_pot[5], 
    state[(state_index)&0xff].m_pot[6], state[(state_index)&0xff].m_pot[7],
		state[(state_index)&0xff].m_pot[8], state[(state_index)&0xff].m_pot[9]);
	 write_log(preferences.debug_alltherest(), "[CGameState] _cards: %d %d %d %d %d\n", 
    state[(state_index)&0xff].m_cards[0], state[(state_index)&0xff].m_cards[1],
		state[(state_index)&0xff].m_cards[2], state[(state_index)&0xff].m_cards[3], 
    state[(state_index)&0xff].m_cards[4]);
	 write_log(preferences.debug_alltherest(), "[CGameState] _is_playing: %d\n", state[(state_index)&0xff].m_is_playing);
	 write_log(preferences.debug_alltherest(), "[CGameState] _is_posting: %d\n", state[(state_index)&0xff].m_is_posting);
	 write_log(preferences.debug_alltherest(), "[CGameState] _dealer_chair: %d\n", state[(state_index)&0xff].m_dealer_chair);
	for (int i=0; i<kMaxNumberOfPlayers; i++) {
		 write_log(preferences.debug_alltherest(), "[CGameState] _player[%d].m_name:%s  ", i, state[(state_index)&0xff].m_player[i].m_name);
		 write_log(preferences.debug_alltherest(), "[CGameState] _balance:%.2f  ", state[(state_index)&0xff].m_player[i].m_balance);
		 write_log(preferences.debug_alltherest(), "[CGameState] _currentbet:%.2f  ", state[(state_index)&0xff].m_player[i].m_currentbet);
		 write_log(preferences.debug_alltherest(), "[CGameState] _cards:%d/%d  ", state[(state_index)&0xff].m_player[i].m_cards[0],
			state[(state_index)&0xff].m_player[i].m_cards[1]);
		 write_log(preferences.debug_alltherest(), "[CGameState] _name_known:%d  ", state[(state_index)&0xff].m_player[i].m_name_known);
		 write_log(preferences.debug_alltherest(), "[CGameState] _balance_known:%d\n", state[(state_index)&0xff].m_player[i].m_balance_known);
	}
}