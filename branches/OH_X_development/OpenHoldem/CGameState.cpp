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
#include "CScrapedActionInterface.h"
#include "CPreferences.h"
#include "CSymbolEngineTableLimits.h"
#include "CTableState.h"
#include "MagicNumbers.h"
#include "Numericalfunctions.h"

CGameState			*p_game_state = NULL;

CGameState::CGameState() {
	_state_index = 0;
}

CGameState::~CGameState() {
}

void CGameState::CaptureState() {
	bool				    playing = true;
	unsigned char		card = CARD_NOCARD;

	// figure out if I am playing
	int sym_chair = SYM->p_symbol_engine_userchair()->userchair();
	if (!SYM->p_symbol_engine_userchair()->userchair_confirmed()) 	{
		playing = false;
	}
	else if (!p_table_state->User()->HasKnownCards())	{
		playing = false;
	}

	// Poker window title
	char title[MAX_WINDOW_TITLE];
	GetWindowText(p_autoconnector->attached_hwnd(), title, MAX_WINDOW_TITLE);

	strncpy_s(_state[_state_index&0xff].m_title, 64, title, _TRUNCATE);
	_state[_state_index&0xff].m_title[63] = '\0';

	// Pot information
	for (int i=0; i<k_max_number_of_players; i++) {
		_state[_state_index&0xff].m_pot[i] = p_table_state->_pot[i];
  }
	// Common cards
	for (int i=0; i<k_number_of_community_cards; i++)	{
    int common_card = p_table_state->_common_cards[i].GetValue();
    write_log(theApp.p_preferences()->debug_dll_extension(), 
      "[CGameState] Common card %i = %i\n", i, common_card);
		_state[_state_index&0xff].m_cards[i] = common_card;
	}

	// playing, posting, dealerchair
	int sym_dealerchair = SYM->p_symbol_engine_dealerchair()->dealerchair();
	bool sym_isautopost = SYM->p_symbol_engine_autoplayer()->isautopost();
	_state[_state_index&0xff].m_is_playing = playing;
	_state[_state_index&0xff].m_is_posting = sym_isautopost;
	_state[_state_index&0xff].m_fillerbits = 0;
	_state[_state_index&0xff].m_fillerbyte = 0;
	_state[_state_index&0xff].m_dealer_chair = sym_dealerchair;

	// loop through all 10 player chairs
	for (int i=0; i<k_max_number_of_players; i++) {
    // player name, balance, currentbet
    strncpy_s(_state[_state_index&0xff].m_player[i].m_name, 16, p_table_state->_players[i]._name.GetString(), _TRUNCATE);
    _state[_state_index&0xff].m_player[i].m_balance = p_table_state->_players[i]._balance;
		_state[_state_index&0xff].m_player[i].m_currentbet = SYM->p_symbol_engine_chip_amounts()->currentbet(i);

		// player cards
		for (int j=0; j<k_number_of_cards_per_player; j++) {
      Card player_card = p_table_state->_players[i]._hole_cards[j];
      int card = player_card.GetValue();
      write_log(theApp.p_preferences()->debug_dll_extension(),
        "[CGameState] Plazer card [%i][%i] = %i\n", i, j, card);
			_state[_state_index&0xff].m_player[i].m_cards[j] = card;
		}

		// player name known, balance known
		_state[_state_index&0xff].m_player[i].m_name_known = p_table_state->_players[i].IsGoodPlayername();
		_state[_state_index&0xff].m_player[i].m_balance_known = true;
		_state[_state_index&0xff].m_player[i].m_fillerbits = 0;
		_state[_state_index&0xff].m_player[i].m_fillerbyte = 0;
	}
  _state_index++;
}

void CGameState::DumpState(void) {
	write_log(theApp.p_preferences()->debug_alltherest(), "[CGameState] m_ndx: %d\n", _state_index);
	write_log(theApp.p_preferences()->debug_alltherest(), "[CGameState] _title: %s\n", _state[(_state_index)&0xff].m_title);
	write_log(theApp.p_preferences()->debug_alltherest(), "[CGameState] _pot: %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n", 
    _state[(_state_index)&0xff].m_pot[0], _state[(_state_index)&0xff].m_pot[1],
		_state[(_state_index)&0xff].m_pot[2], _state[(_state_index)&0xff].m_pot[3], 
    _state[(_state_index)&0xff].m_pot[4], _state[(_state_index)&0xff].m_pot[5], 
    _state[(_state_index)&0xff].m_pot[6], _state[(_state_index)&0xff].m_pot[7],
		_state[(_state_index)&0xff].m_pot[8], _state[(_state_index)&0xff].m_pot[9]);
	write_log(theApp.p_preferences()->debug_alltherest(), "[CGameState] _cards: %d %d %d %d %d\n", 
    _state[(_state_index)&0xff].m_cards[0], _state[(_state_index)&0xff].m_cards[1],
		_state[(_state_index)&0xff].m_cards[2], _state[(_state_index)&0xff].m_cards[3], 
    _state[(_state_index)&0xff].m_cards[4]);
	write_log(theApp.p_preferences()->debug_alltherest(), "[CGameState] _is_playing: %d\n", _state[(_state_index)&0xff].m_is_playing);
	write_log(theApp.p_preferences()->debug_alltherest(), "[CGameState] _is_posting: %d\n", _state[(_state_index)&0xff].m_is_posting);
	write_log(theApp.p_preferences()->debug_alltherest(), "[CGameState] _dealer_chair: %d\n", _state[(_state_index)&0xff].m_dealer_chair);
	for (int i=0; i<k_max_number_of_players; i++) {
		write_log(theApp.p_preferences()->debug_alltherest(), "[CGameState] _player[%d].m_name:%s  ", i, _state[(_state_index)&0xff].m_player[i].m_name);
		write_log(theApp.p_preferences()->debug_alltherest(), "[CGameState] _balance:%.2f  ", _state[(_state_index)&0xff].m_player[i].m_balance);
		write_log(theApp.p_preferences()->debug_alltherest(), "[CGameState] _currentbet:%.2f  ", _state[(_state_index)&0xff].m_player[i].m_currentbet);
		write_log(theApp.p_preferences()->debug_alltherest(), "[CGameState] _cards:%d/%d  ", _state[(_state_index)&0xff].m_player[i].m_cards[0],
			_state[(_state_index)&0xff].m_player[i].m_cards[1]);
		write_log(theApp.p_preferences()->debug_alltherest(), "[CGameState] _name_known:%d  ", _state[(_state_index)&0xff].m_player[i].m_name_known);
		write_log(theApp.p_preferences()->debug_alltherest(), "[CGameState] _balance_known:%d\n", _state[(_state_index)&0xff].m_player[i].m_balance_known);
	}
}