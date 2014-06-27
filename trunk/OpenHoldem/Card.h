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

#ifndef INC_CARD_H
#define INC_CARD_H

class Card {
 public:
  Card();
  ~Card();
 public:
   void SetValue(int value);
   void SetValue(int openholdem_rank, int openholdem_suit);
   void ClearValue();
 public:
   int GetValue();
   int GetOpenHoldemRank();
   int GetOpenHoldemSuit();
   int GetStdDeckRank();
   int GetStdDeckSuit();
 public:
   bool IsKnownCard();
   bool IsCardBack();
   bool IsNoCard();     // Empty, missing
   bool IsAnyCard();    // Card or cardback
 private:
   int _value;
};

#endif