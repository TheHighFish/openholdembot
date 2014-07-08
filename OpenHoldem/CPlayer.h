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

#ifndef INC_CPLAYER_H
#define INC_CPLAYER_H

#include "Card.h"

class CPlayer {
 public:
  CPlayer();
  ~CPlayer();
 public:
  void Reset(); //!!! LazyScraper!!! comflict?
 public:
  bool HasAnyCards();
  bool HasKnownCards();
 public:
  void CheckPlayerCardsForConsistency();
 public:
  CString _name;
  double  _balance;
  Card    _hole_cards[k_number_of_cards_per_player];

};

#endif INC_CPLAYER_H

/*!!!
bool CScraperAccess::PlayerHasCards(int player)
{
	assert(player >= 0);
	assert(player < p_tablemap->nchairs());
	// We do no longer check for cardbacks,
	// but for cardbacks or cards.
	// This way we can play all cards face-up at PokerAcademy.
	// http://www.maxinmontreal.com/forums/viewtopic.php?f=111&t=13384
	if (!IsPlayerActive(player))
	{
		return false;
	}
	if (p_table_state->_players[player, 0) == CARD_NOCARD 
		|| p_table_state->_players[player, 1) == CARD_NOCARD)
	{
		return false;
	}
	return true;
}
CheckPlayerCardsForConsistency(int chair) {
  AssertRange(chair, 0, k_max_chair_number);
  if (p_table_state->_players[chair].HasKnownCards
    && (p_table_state->_players[chair]._hole_cards[0].GetValue()
      == p_table_state->_players[chair]._hole_cards[1].GetValue())) {
    // Identical cards, something clearly went wrong.
    // We assume, that we see something that differs from the background,
    // probably cardbacks, that sometimes get scraped as JJsuited or 88suited
    // by bad tablemaps.
    _card_player[chair][0] = CARD_BACK;
    _card_player[chair][1] = CARD_BACK;
    write_log(preferences.debug_scraper(), 
		  "[CScraper] Bad cards for chair %i. Auto-correcting to card-backs\n", chair);
  }
}

*/