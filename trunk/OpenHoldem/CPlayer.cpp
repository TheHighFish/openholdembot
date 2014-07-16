//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*****************************************************************************
//
// Purpose:
//
//*****************************************************************************

#include "stdafx.h"
#include "CPlayer.h"

CPlayer::CPlayer() {
}

CPlayer::~CPlayer() {
}

void CPlayer::Reset() {
  _name = "";
  _balance = 0.0;
  for (int i=0; i<k_number_of_cards_per_player; ++i) {
    _hole_cards[i].ClearValue();
  }
}

bool CPlayer::HasAnyCards() {
  return (_hole_cards[0].IsAnyCard() && _hole_cards[1].IsAnyCard());
}

bool CPlayer::HasKnownCards() {
  return (_hole_cards[0].IsKnownCard() && _hole_cards[1].IsKnownCard());
}

void CPlayer::CheckPlayerCardsForConsistency() {
  if (HasKnownCards() && (_hole_cards[0].GetValue() == _hole_cards[1].GetValue())) {
    // Something "known", not "nocard" and not "undefined",
    // but clearly wrong (duplicate card).
    // Probably mis-scraped cardbacks
    _hole_cards[0].SetValue(CARD_BACK);
    _hole_cards[1].SetValue(CARD_BACK);
  }
}