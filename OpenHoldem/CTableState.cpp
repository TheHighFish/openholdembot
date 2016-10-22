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
#include "CSymbolEngineActiveDealtPlaying.h"

CTableState *p_table_state = NULL;

CTableState::CTableState() {
  Reset();
}

CTableState::~CTableState() {
}

void CTableState::Reset() {
  _s_limit_info.Reset();
  for (int i=0; i<kNumberOfCommunityCards; ++i) {
    _common_cards[i].ClearValue();
  }
  for (int i=0; i<kNumberOfPlayerEntries; ++i) {
    _players[i].Reset();
  }
  for (int i=0; i<kMaxNumberOfPots; ++i) {
    _pot[i].Reset();
  }
  TableTitle()->Clear();
}

CPlayer *CTableState::User() {
  if (p_symbol_engine_userchair == NULL) {
    return &_players[kFakeEntryForUnknownUserchair];
  }
  if (!p_symbol_engine_userchair->userchair_confirmed()) {
    return &_players[kFakeEntryForUnknownUserchair];
  }
  int userchair = p_symbol_engine_userchair->userchair();
  AssertRange(userchair, 0, kLastChair);
  return &_players[userchair];
}

CPlayer *CTableState::Player(int chair) {
  AssertRange(chair, 0, kLastChair);
  return &_players[chair];
}

double CTableState::Pot(int sidepot_index) {
  AssertRange(sidepot_index, 0, kLastPot);
  return  _pot[sidepot_index].GetValue();
}

bool CTableState::set_pot(int sidepot_index, CString new_value) {
  AssertRange(sidepot_index, 0, kLastPot);
  return _pot[sidepot_index].SetValue(new_value);
}

void CTableState::ResetPots() {
  for (int j = 0; j < kMaxNumberOfPots; j++) {
    _pot[j].Reset();
  }
}

Card *CTableState::CommonCards(int common_card_index) {
  AssertRange(common_card_index, 0, kIndexOfLastCommunityCard);
  return &_common_cards[common_card_index];
}

Card *CTableState::TurnCard() {
  return CommonCards(kIndexOfTurnCard);
}

Card *CTableState::RiverCard() {
  return CommonCards(kIndexOfRiverCard);
}

int CTableState::NumberOfCommunityCards() {
  int result = 0;
  for (int i = 0; i < kNumberOfCommunityCards; ++i) {
    if (_common_cards[i].IsKnownCard()) {
      ++result;
    }
    else {
      break;
    }
  }
  return result;
}

CTableTitle *CTableState::TableTitle() {
  return &_table_title;
}

