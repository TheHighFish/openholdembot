//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose: Game-state object for the table
//   (common cards and players)
//   Actually it should be names CGameSate,
//   but there already is a symbol engine with that name. 
//
//*******************************************************************************

#ifndef INC_CTABLESTATE_H
#define INC_CTABLESTATE_H

#include "Card.h"
#include "CPlayer.h"
#include "CScrapedActionInterface.h"
#include "MagicNumbers.h"
#include "SLimitInfo.h"

// One fake-entry for the case of unknown user-chair
// This way we can avoid the handling of special cases
// and the danger of NULL-pointers for the User()-function.
const int kNumberOfPlayerEntries        = kMaxNumberOfPlayers + 1;
const int kFakeEntryForUnknownUserchair = kMaxNumberOfPlayers;

class CTableState {
 public:
  CTableState();
  ~CTableState();
 public:
  void Reset();
 public:
  CPlayer *User();
  CPlayer *Player(int chair);
  double	Pot(int sidepot_index);
 public:
  void set_pot(int sidepot_index, double new_value);
 public: //!!!!
  CLimitInfo _s_limit_info;
  CScrapedActionInterface _SCI;
 public:
  char		_title[MAX_WINDOW_TITLE];
  char		_title_last[MAX_WINDOW_TITLE]; 
  Card    _common_cards[kNumberOfCommunityCards];
 private:
  CPlayer _players[kNumberOfPlayerEntries];
  double	_pot[kMaxNumberOfPots];
};

extern CTableState *p_table_state;

#endif INC_CTABLESTATE_H