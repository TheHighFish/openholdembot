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

// One fake-entry for the case of unknown user-chair
// This way we can avoid the handling of special cases
// and the danger of NULL-pointers for the User()-function.
const int kNumberOfPlayerEntries        = k_max_number_of_players + 1;
const int kFakeEntryForUnknownUserchair = k_max_number_of_players;

class CTableState {
 public:
  CTableState();
  ~CTableState();
 public:
  CPlayer *User();
 public:
  Card    _common_cards[k_number_of_community_cards];
  CPlayer _players[kNumberOfPlayerEntries];
  char		_title[MAX_WINDOW_TITLE];
  double	_pot[k_max_number_of_pots];
};

extern CTableState *p_table_state;

#endif INC_CTABLESTATE_H