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

#ifndef INC_CPLAYER_H
#define INC_CPLAYER_H

#include "Card.h"

class CPlayer {
 public:
  CPlayer();
  ~CPlayer();
 public:
  void Reset(); 
 public:
  bool HasAnyCards();
  bool HasKnownCards();
 public:
  void CheckPlayerCardsForConsistency();
  CString Cards();
 public:
  CString _name;
  double  _balance;
  Card    _hole_cards[k_number_of_cards_per_player];

};

#endif INC_CPLAYER_H
