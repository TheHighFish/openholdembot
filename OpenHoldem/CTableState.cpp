//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Game-state object for the table
//   (common cards and players)
//   Actually it should be names CGameSate,
//   but there already is a symbol engine with that name. 
//
//******************************************************************************

#include "stdafx.h"
#include "CTableState.h"

#include "CSymbolEngineUserchair.h"
#include "NumericalFunctions.h"

CTableState *p_table_state = NULL;

CTableState::CTableState() {
  Reset();
}

CTableState::~CTableState() {
}

void CTableState::Reset() {
  _SCI.Reset();
  _s_limit_info.Reset();
  for (int i=0; i<k_number_of_community_cards; ++i) {
    _common_cards[i].ClearValue();
  }
  for (int i=0; i<kNumberOfPlayerEntries; ++i) {
    _players[i].Reset();
  }
  for (int i=0; i<k_max_number_of_pots; ++i) {
    _pot[i] = 0.0;
  }
  _title[0] = '\0';
  _title_last[0] = '\0';
}

CPlayer *CTableState::User() {
  if (SYM->p_symbol_engine_userchair() == NULL) {
    return &_players[kFakeEntryForUnknownUserchair];
  }
  if (!SYM->p_symbol_engine_userchair()->userchair_confirmed()) {
    return &_players[kFakeEntryForUnknownUserchair];
  }
  int userchair = SYM->p_symbol_engine_userchair()->userchair();
  AssertRange(userchair, 0, k_last_chair);
  return &_players[userchair];
}