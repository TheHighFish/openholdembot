//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Detecting handresets the reliable way,
//   requireing N seen handresetmethods within 3 heartbeats
//
//******************************************************************************

#ifndef INC_CHANDRESETDETECTOR_H
#define INC_CHANDRESETDETECTOR_H

#include "CSpaceOptimizedGlobalObject.h"

class CHandresetDetector : public CSpaceOptimizedGlobalObject {
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
  void ClearSeenHandResets();
 private:
	bool IsHandresetByDealerChair();
	bool IsHandresetByUserCards();
	bool IsHandresetByHandNumber();
	bool IsHandresetByCommunityCards();
	bool IsHandresetByPotsize();
	bool IsHandresetByNopponentsplaying();
	bool IsHandresetByIncreasingBalance();
  bool IsHandresetByNewSmallBlind();
  bool IsHandresetByChangingBlindLevel();
  bool IsHandresetByOHReplayFrameNumber();
 public:
  int hands_played() { return _hands_played; }
  int hands_played_headsup() { return _hands_played_headsup; }
 private:
	bool IsValidHandNumber(CString handnumber);
	bool IsValidDealerChair(int dealerchair);
  bool SmallBlindExists();
  int  BitVectorFiringHandresetMethods();
 private:
	void GetNewSymbolValues();
	void StoreOldValuesForComparisonOnNextHeartbeat();
 private:
  void UpdateHandsPlayedOnHandreset();
 private:
	int dealerchair;
	int last_dealerchair;
	int playercards[kMaxNumberOfCardsPerPlayer];
	int last_playercards[kMaxNumberOfCardsPerPlayer];
 private:
  double _potsize;
  double _last_potsize;
  int    _community_cards;
  int    _last_community_cards;
  int    _nopponentsplaying;
  int    _last_nopponentsplaying;
  double _balance[kMaxNumberOfPlayers];
  double _last_balance[kMaxNumberOfPlayers];
  double _bblind;
  double _last_bblind;
  bool   _small_blind_existed_last_hand;
  int    _ohreplay_framenumber;
  int    _last_ohreplay_framenumber;
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
  int _hands_played_headsup;
};

extern CHandresetDetector *p_handreset_detector;

#endif INC_CHANDRESETDETECTOR_H