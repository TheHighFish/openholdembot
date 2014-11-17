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

#ifndef INC_CHANDRESETDETECTOR_H
#define INC_CHANDRESETDETECTOR_H

#include "MagicNumbers.h"

class CHandresetDetector {
 public:
	CHandresetDetector();
	~CHandresetDetector();
 public:
	// OnNewHeartbeat(): to be called on every new heartbeat
	// BEFORE IsHandreset() gets called.
	void OnNewHeartbeat();
	bool IsHandreset()		{ return _is_handreset_on_this_heartbeat; }
 public:
	// Only for output in the log
	CString GetHandNumber();
 private: 
	void CalculateIsHandreset();
	bool IsHandresetByDealerChair();
	bool IsHandresetByUserCards();
	bool IsHandresetByHandNumber();
	bool IsHandresetByCommunityCards();
	bool IsHandresetByPotsize();
	bool IsHandresetByNopponentsplaying();
	bool IsHandresetByIncreasingBalance();
  bool IsHandresetByNewSmallBlind();
  bool IsHandresetByChangingBlindLevel();
public:
  int hands_played() { return _hands_played; }
 private:
	bool IsValidHandNumber(CString handnumber);
	bool IsValidDealerChair(int dealerchair);
  bool SmallBlindExists();
  int  BitVectorFiringHandresetMethods();
 private:
	void GetNewSymbolValues();
	void StoreOldValuesForComparisonOnNextHeartbeat();
 private:
	int dealerchair;
	int last_dealerchair;
	int playercards[k_number_of_cards_per_player];
	int last_playercards[k_number_of_cards_per_player];
 private:
  double _potsize;
  double _last_potsize;
  int    _community_cards;
  int    _last_community_cards;
  int    _nopponentsplaying;
  int    _last_nopponentsplaying;
  double _balance[k_max_number_of_players];
  double _last_balance[k_max_number_of_players];
  double _bblind;
  double _last_bblind;
  bool   _small_blind_existed_last_hand;
 private:
	// Handnumber should be a string, as
	//   * it may contain characters
	//   * its lengths my exceed the precision of double
	CString handnumber;
	CString last_handnumber;
 private:
   int _methods_firing_the_last_three_heartbeats[3];
 private:
	bool _is_handreset_on_this_heartbeat;
  int _hands_played;
};

extern CHandresetDetector *p_handreset_detector;

#endif INC_CHANDRESETDETECTOR_H