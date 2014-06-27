//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Scraping the poker-table and providing access to the scraped data.
//  As the CScraper is low-level and quite large we created 
//  an interface SCraperAccess that provides higher-level accessors
//  like "UserHasCards()".
//  Better use that interface to access scraper-data whenever possible.
//
//******************************************************************************

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
  return 0; //!!!
}

int Card::GetOpenHoldemRank() {
  return 0; //!!!
}

int Card::GetOpenHoldemSuit() {
  return 0; //!!!
}

int Card::GetStdDeckRank() {
  return 0; //!!!
}

int Card::GetStdDeckSuit() {
  return 0; //!!!
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
  return (_value == CARD_NOCARD);
}
   
bool Card::IsAnyCard() {
  return (IsKnownCard() || IsCardBack());
}
