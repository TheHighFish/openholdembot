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
#include "CPlayer.h"
#include "CScrapedMoney.h"
#include "CTableTitle.h"
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
  Card *CommonCards(int common_card_index);
  Card *TurnCard();
  Card *RiverCard();
  int  NumberOfCommunityCards();
 public:
  CTableTitle *TableTitle();
 public:
  bool set_pot(int sidepot_index, CString new_value);
  void ResetPots();
 public:
  // !!! To do: proper encapsulation
  CLimitInfo _s_limit_info;
 private:
  CPlayer _players[kNumberOfPlayerEntries];
  Card    _common_cards[kNumberOfCommunityCards];
  CScrapedMoney _pot[kMaxNumberOfPots];
 private:
  CTableTitle _table_title;
 private:
  
 private:
};

extern CTableState *p_table_state;

#endif INC_CTABLESTATE_H