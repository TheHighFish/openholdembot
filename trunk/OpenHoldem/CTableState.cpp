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

CTableState *p_table_state = NULL;

CTableState::CTableState() {
  for (int i=0; i<k_number_of_community_cards; ++i) {
    _common_cards[i].ClearValue();
  }
  for (int i=0; i<k_max_number_of_players; ++i) {
    _players[i].Reset();
  }
}

CTableState::~CTableState() {
}