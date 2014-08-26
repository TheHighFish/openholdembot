//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*****************************************************************************
//
// Purpose: Scraping the poker-table and providing access to the scraped data.
//  As the CScraper is low-level and quite large we created 
//  an interface SCraperAccess that provides higher-level accessors
//  like "UserHasCards()".
//  Better use that interface to access scraper-data whenever possible.
//
//*****************************************************************************

#include "stdafx.h"
#include "Card.h"

Card::Card() {
  ClearValue();
}

Card::~Card() {
}

void Card::SetValue(int value){
  _value = value;
}

void Card::SetValue(int openholdem_rank, int openholdem_suit){
}

void Card::ClearValue(){
  _value = CARD_UNDEFINED;
}

int Card::GetValue() {
  return _value;
}

// StdDeck uses 0..12 for deuce..ace
// OpenHoldem uses 2..14
int Card::GetOpenHoldemRank() {
  if (IsKnownCard()) {
    return (GetStdDeckRank() + 2);
  } else {
    return k_undefined;
  }
}

// StdDeck uses 0..12 for deuce..ace
// OpenHoldem uses 2..14
int Card::GetStdDeckRank() {
  if (IsKnownCard()) {
    return StdDeck_RANK(_value);
  } else {
    return k_undefined;
  }
}

int Card::GetSuit() {
  if (IsKnownCard()) {
    return StdDeck_SUIT(_value);
  } else {
    return k_undefined;
  }
}

char Card::GetRankCharacter() {
  if (!IsKnownCard()) {
    return '?';
  }
  return StdDeck_rankChars[GetStdDeckRank()];
}

char Card::GetSuitCharacter(bool uppercase /* = false */) {
  if (!IsKnownCard()) {
    return '?';
  }
  char suit_character = StdDeck_suitChars[GetSuit()];
  if (uppercase) {
    return toupper(suit_character);
  }
  else {
    return suit_character;
  }
}

bool Card::IsKnownCard() {
  return (!IsCardBack()
    && !IsNoCard()
    && (_value != CARD_UNDEFINED));
}

bool Card::IsCardBack(){
  return (_value == CARD_BACK);
}

bool Card::IsNoCard() {
  return ((_value == CARD_NOCARD) || (_value == CARD_UNDEFINED));
}
   
bool Card::IsAnyCard() {
  // We do no longer check for cardbacks,
	// but for cardbacks or cards.
	// This way we can play all cards face-up at PokerAcademy.
	// http://www.maxinmontreal.com/forums/viewtopic.php?f=111&t=13384
  return (IsKnownCard() || IsCardBack());
}

CString Card::ToString() {
  return StdDeck_cardString(_value);  
}
