//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose:
//
//*******************************************************************************

#include "stdafx.h"
#include "CPlayer.h"

#include "CSymbolEngineTableLimits.h"

CPlayer::CPlayer() {
}

CPlayer::~CPlayer() {
}

void CPlayer::Reset() {
  _name = "";
  _balance = 0.0;
  _bet = 0.0;
  for (int i=0; i<kNumberOfCardsPerPlayer; ++i) {
    _hole_cards[i].ClearValue();
  }
  _seated = false;
  _active = false;
  _dealer = false;
  _colourcode = kUndefinedZero;
}

bool CPlayer::HasAnyCards() {
  // We do no longer check for cardbacks,
	// but for cardbacks or cards.
	// This way we can play all cards face-up at PokerAcademy.
	// http://www.maxinmontreal.com/forums/viewtopic.php?f=111&t=13384
  return (_hole_cards[0].IsAnyCard() && _hole_cards[1].IsAnyCard());
}

CString CPlayer::Cards() {
	// log new hand
  if (HasKnownCards()) {
    CString result = _hole_cards[0].ToString() + _hole_cards[1].ToString();
    return result;
  }
  if (HasAnyCards()) {
    return "BACKS";
  }
  return "NONE";
}

bool CPlayer::HasKnownCards() {
  return (_hole_cards[0].IsKnownCard() && _hole_cards[1].IsKnownCard());
}

void CPlayer::CheckPlayerCardsForConsistency() {
  if (HasKnownCards() && (_hole_cards[0].GetValue() == _hole_cards[1].GetValue())) {
    // Identical cards, something clearly went wrong.
    // Something "known", not "nocard" and not "undefined",
    // but clearly wrong (duplicate card).
    // We assume, that we see something that differs from the background,
    // probably cardbacks, that sometimes get scraped as JJsuited or 88suited
    // by bad tablemaps.
    _hole_cards[0].SetValue(CARD_BACK);
    _hole_cards[1].SetValue(CARD_BACK);
  } 
}

bool CPlayer::IsAllin() {
  return (_seated && _active && HasAnyCards() && (_balance == 0));
}

bool CPlayer::PostingBothBlinds() {
  // We have to calculate in cents here, as IsApproximatellyEqual uses rounding internally.
  // http://www.maxinmontreal.com/forums/viewtopic.php?f=156&t=18743
  double bet_in_cents = 100 * _bet;
  double both_blinds_in_cents = 100 * (p_symbol_engine_tablelimits->sblind() + p_symbol_engine_tablelimits->bblind());
  return (_seated && _active && HasAnyCards() 
    && IsApproximatellyEqual(bet_in_cents, both_blinds_in_cents));
}