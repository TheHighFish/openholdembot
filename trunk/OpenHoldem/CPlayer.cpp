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
#include "CPlayer.h"

CPlayer::CPlayer() {
}

CPlayer::~CPlayer() {
}

void CPlayer::Reset() {
  for (int i=0; i<k_number_of_cards_per_player; ++i) {
    hole_cards[i].ClearValue();
  }
}

bool CPlayer::HasAnyCards() {
  return (hole_cards[0].IsAnyCard() && hole_cards[1].IsAnyCard());
}

bool CPlayer::HasKnownCards() {
  return (hole_cards[0].IsKnownCard() && hole_cards[1].IsKnownCard());
}

void CPlayer::CheckPlayerCardsForConsistency() {
  if (HasKnownCards() && (hole_cards[0].GetValue() == hole_cards[1].GetValue())) {
    // Something "known", not "nocard" and not "undefined",
    // but clearly wrong (duplicate card).
    // Probably mis-scraped cardbacks
    hole_cards[0].SetValue(CARD_BACK);
    hole_cards[1].SetValue(CARD_BACK);
  }
}