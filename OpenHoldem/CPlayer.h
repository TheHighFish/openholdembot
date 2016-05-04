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
 public:
  void CheckPlayerCardsForConsistency();
  CString Cards();
 public:
  bool IsAllin();
  // Having to post both blinds at once happens at some casinos
  // http://www.maxinmontreal.com/forums/viewtopic.php?f=297&t=18948
  // http://www.maxinmontreal.com/forums/viewtopic.php?f=297&t=18948
  // This does not count as raising.
  bool PostingBothBlinds();
 public:
  // Public accessors
  CString name()       { return _name; }
  double  balance()    { return _balance; }
  double  bet()        { return _bet; }
  int     colourcode() { return _colourcode; }
 public:
  // Public accessors
  Card*   hole_cards(int first_or_second_0_1) { 
    assert(first_or_second_0_1 >= 0);
    assert(first_or_second_0_1 < kNumberOfCardsPerPlayer);
    return &_hole_cards[first_or_second_0_1]; 
  }
 public:
  // Public accessors
  bool seated() { return _seated; }
  bool active() { return _active; }
  bool dealer() { return _dealer; }
 public:
  // Public setters (must only be used by CScraper)
  void set_name(CString new_name)         { _name = new_name; }
  void set_balance(double new_balance)    { _balance = new_balance; }
  void set_bet(double new_bet)            { _bet = new_bet; }
  void set_colourcode(int new_colourcode) { _colourcode = new_colourcode; }
 public:
  // Public setters (must only be used by CScraper)
  void set_seated(bool is_seated) { _seated = is_seated; }
  void set_active(bool is_active) { _active = is_active; }
  void set_dealer(bool is_dealer) { _dealer = is_dealer; }
 private:
  CString _name;
  double  _balance;
  double  _bet;
  Card    _hole_cards[kNumberOfCardsPerPlayer];
  int     _colourcode;
 private:
  bool _seated;
  bool _active;
  bool _dealer;
};

#endif INC_CPLAYER_H
