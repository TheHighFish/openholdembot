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
#include "CHandresetDetector.h"

#include <assert.h>
#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineDealerchair.h"
#include "CSymbolEngineTableLimits.h"
#include "CSymbolEngineUserchair.h"
#include "CTableState.h"
#include "..\CTablemap\CTablemap.h"
#include "MagicNumbers.h"
#include "StringFunctions.h"

CHandresetDetector *p_handreset_detector = NULL;

CHandresetDetector::CHandresetDetector() {
  //!!!!
	write_log(preferences.debug_handreset_detector(), "[CHandresetDetector] Executing constructor\n");
	playercards[0] = CARD_UNDEFINED;
	playercards[1] = CARD_UNDEFINED;
	dealerchair = k_undefined;
	handnumber = "";
	_is_handreset_on_this_heartbeat = false;
}

CHandresetDetector::~CHandresetDetector() {
}

CString CHandresetDetector::GetHandNumber() {
	return handnumber;
}

void CHandresetDetector::CalculateIsHandreset() {
	int handresetemethods_that_fired_this_heartbeat =
      (IsHandresetByDealerChair() ? 1 : 0)
    + (IsHandresetByUserCards() ? 1 : 0)
    + (IsHandresetByHandNumber() ? 1 : 0)
    + (IsHandresetByCommunityCards() ? 1 : 0)
    + (IsHandresetByPotsize() ? 1 : 0)
    + (IsHandresetByNopponentsplaying() ? 1 : 0)
    + (IsHandresetByIncreasingBalance() ? 1 : 0);
  int handresetemethods_that_fired_the_last_three_heartbeats =
    handresetemethods_that_fired_this_heartbeat; // TODO!!!
	_is_handreset_on_this_heartbeat = 
    (handresetemethods_that_fired_the_last_three_heartbeats >= 2);
}

bool CHandresetDetector::IsHandresetByDealerChair() {
	bool ishandreset = (IsValidDealerChair(dealerchair) && (dealerchair != last_dealerchair));
	write_log(preferences.debug_handreset_detector(), "[CHandresetDetector] Handreset by dealerchair: %s\n",
		Bool2CString(ishandreset));
	return ishandreset;
}

bool CHandresetDetector::IsHandresetByUserCards() {
	bool ishandreset = (p_table_state->User()->HasKnownCards()
		&& (playercards[0] != last_playercards[0]) 
		&& (playercards[1] != last_playercards[1]));
	write_log(preferences.debug_handreset_detector(), "[CHandresetDetector] Handreset by cards: %s\n",
		Bool2CString(ishandreset));
	return ishandreset;
}

bool CHandresetDetector::IsHandresetByHandNumber() {
	bool ishandreset = (IsValidHandNumber(handnumber) && (handnumber != last_handnumber));
	write_log(preferences.debug_handreset_detector(), "[CHandresetDetector] Handreset by handnumber: %s\n",
		Bool2CString(ishandreset));
	return ishandreset;
}

bool CHandresetDetector::IsValidHandNumber(CString handnumber) {
	int length_of_handnumber = handnumber.GetLength();
	int max_length_of_handnumber = p_tablemap->HandNumberMaxExpectedDigits();
	int min_length_of_handnumber = p_tablemap->HandNumberMinExpectedDigits(); 

	if ((min_length_of_handnumber > 0) && (max_length_of_handnumber > 0)) {
		return ((length_of_handnumber >= min_length_of_handnumber)
			&& (length_of_handnumber <= max_length_of_handnumber));
	}	else {
		// No extra requirements for handnumber: always valid
		return true;
	}
}

bool CHandresetDetector::IsValidDealerChair(int dealerchair) {
	// Dealerchair should be -1, if not found (occlusion).
	return ((dealerchair >= 0) && (dealerchair < p_tablemap->nchairs()));
}

bool CHandresetDetector::IsHandresetByCommunityCards() {
  bool ishandreset = ((_community_cards < _last_community_cards)
    && (_community_cards == 0));
  write_log(preferences.debug_handreset_detector(), "[CHandresetDetector] Handreset by community cards: %s\n",
		Bool2CString(ishandreset));
  return ishandreset;
}

bool CHandresetDetector::IsHandresetByPotsize() {
  // Decreasing potsize and potsize is preflop-like
  bool ishandreset = ((_potsize < _last_potsize)
    && (_potsize >= 1 * BIG_BLIND)
    && (_potsize <  6 * BIG_BLIND));
  write_log(preferences.debug_handreset_detector(), "[CHandresetDetector] Handreset by potsize: %s\n",
		Bool2CString(ishandreset));
  return ishandreset;
}

bool CHandresetDetector::IsHandresetByNopponentsplaying() {
  bool ishandreset = (_nopponentsplaying > _last_nopponentsplaying);
  write_log(preferences.debug_handreset_detector(), "[CHandresetDetector] Handreset by nopponentsplaying: %s\n",
		Bool2CString(ishandreset));
  return ishandreset;
}

bool CHandresetDetector::IsHandresetByIncreasingBalance() {
  bool ishandreset = false;
  for (int i=0; i<k_max_number_of_players; ++i) {
    if ((_balance[i] > _last_balance[i])
        && (_last_balance[i] > 0)) {
      ishandreset = true;
      break;
    }
  }
  write_log(preferences.debug_handreset_detector(), "[CHandresetDetector] Handreset by increasing balance: %s\n",
		Bool2CString(ishandreset));
  return ishandreset;
}

void CHandresetDetector::GetNewSymbolValues() {
	assert(p_symbol_engine_dealerchair != NULL);
	if (IsValidDealerChair(p_symbol_engine_dealerchair->dealerchair()))	{
		dealerchair = p_symbol_engine_dealerchair->dealerchair();	
		write_log(preferences.debug_handreset_detector(), "[CHandresetDetector] Setting new dealerchair to [%i]\n", dealerchair);
	}
	if (IsValidHandNumber(p_scraper->s_limit_info()->handnumber))	{
		write_log(preferences.debug_handreset_detector(), "[CHandresetDetector] Setting handnumber to [%s]\n", handnumber);
		handnumber = p_scraper->s_limit_info()->handnumber;	
	}	else {
		write_log(preferences.debug_handreset_detector(), "[CHandresetDetector] Setting handnumber to [%s] was skipped. Reason: [digits number not in range]\n", handnumber);
	}
	assert(p_symbol_engine_userchair != NULL);
	int userchair = p_symbol_engine_userchair->userchair();
  _potsize = p_symbol_engine_chip_amounts->pot();
  _community_cards = p_scraper_access->NumberOfCommonCards();
  _nopponentsplaying = p_symbol_engine_active_dealt_playing->nopponentsplaying();
	for (int i=0; i<k_number_of_cards_per_player; i++) {
		if ((userchair >= 0) && (userchair < p_tablemap->nchairs())) {
      playercards[i] = p_table_state->_players[userchair]._hole_cards[i].GetValue();
		} else {
			playercards[i] = CARD_UNDEFINED;
		}
	}
  for (int i=0; i<k_max_number_of_players; ++i) {
    _balance[i] = p_table_state->_players[i]._balance;
  }
}

void CHandresetDetector::StoreOldValuesForComparisonOnNextHeartbeat() {
	last_dealerchair = dealerchair;
	write_log(preferences.debug_handreset_detector(), "[CHandresetDetector] Setting last dealerchair to [%i]\n", dealerchair);
	last_handnumber = handnumber;
  _last_potsize = _potsize;
  _last_community_cards = _community_cards;
  _last_nopponentsplaying = _nopponentsplaying;
	for (int i=0; i<k_number_of_cards_per_player; i++) {
		last_playercards[i] = playercards[i];
	}
}

void CHandresetDetector::OnNewHeartbeat() {
	if (p_symbol_engine_dealerchair == NULL) {
		// Very early initialization phase
		return;
	}
	GetNewSymbolValues();
	CalculateIsHandreset();
	StoreOldValuesForComparisonOnNextHeartbeat();
}

