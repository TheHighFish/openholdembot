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
  bool IsAllin();
  // Having to poat both blinds at once happens at some casinos
  // www.
  // This does not count as raising.
  bool PostingBothBlinds();
 public:
  CString _name;
  double  _balance;
  double  _bet;
  Card    _hole_cards[kNumberOfCardsPerPlayer];
  int     _colourcode;
 public:
  bool _seated;
  bool _active;
  bool _dealer;
};

#endif INC_CPLAYER_H
