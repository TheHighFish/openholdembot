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
  void Reset(); 
 public:
  bool HasAnyCards();
  bool HasKnownCards();
  bool IsGoodPlayername();
 public:
  bool seated()   { return _seated; }
  bool active()   { return seated() && _active; }
  bool dealer()   { return _dealer; }
 public:
  void set_seated(bool b)     { _seated = b; }
  void set_active(bool b)     { _active = b; }
  void set_dealer(bool b)     { _dealer = b; }
 public:
  void CheckPlayerCardsForConsistency();
  CString Cards();
 public:
  CString _name;
  double  _balance;
  double  _bet;
  Card    _hole_cards[k_number_of_cards_per_player];
  int     _colourcode;
 private:
  bool _seated;
  bool _active;
  bool _dealer;
};

#endif INC_CPLAYER_H
