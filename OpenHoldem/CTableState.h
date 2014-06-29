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

#ifndef INC_CTABLESTATE_H
#define INC_CTABLESTATE_H

#include "Card.h"
#include "MagicNumbers.h"
#include "CPlayer.h"

class CTableState {
 public:
  CTableState();
  ~CTableState();
 public:
  Card _common_cards[k_number_of_community_cards];
  CPlayer _players[k_max_number_of_players];
};

extern CTableState *p_table_state;

#endif INC_CTABLESTATE_H