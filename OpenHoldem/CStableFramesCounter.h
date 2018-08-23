//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#ifndef _INC_CSTABLEFRAMESCOUNTER_H
#define _INC_CSTABLEFRAMESCOUNTER_H

#include "CSpaceOptimizedGlobalObject.h"

class CStableFramesCounter : public CSpaceOptimizedGlobalObject {
 public:
	CStableFramesCounter();
	~CStableFramesCounter();
 public:
	int	NumberOfStableFrames() { return _NumberOfStableFrames; }
	int	UpdateNumberOfStableFrames();
 public:
  // After an autoplayer-action we expect a change in table-state.
  // Therefore we reset the counter to zero after actions
  // to avoid multiple clicks within a short frame of time
  // if the casino does not update its GUI fast enough.
  void UpdateOnAutoplayerAction();
 private:
	void Reset();
	void SaveCurrentState();
 private:
	unsigned int _NumberOfStableFrames;
	bool         _isReset;
 private:
	// variables to keep the last game-state.
	unsigned int	_myturnbitslast;
	unsigned int	_card_common_last[kNumberOfCommunityCards];
	unsigned int	_card_player_last[kMaxNumberOfPlayers][kMaxNumberOfCardsPerPlayer];
	bool          _dealer_last[kMaxNumberOfPlayers];
	double        _playerbalance_last[kMaxNumberOfPlayers];
	double			  _playerbet_last[kMaxNumberOfPlayers];
};

extern CStableFramesCounter *p_stableframescounter;

#endif // _INC_CSTABLEFRAMESCOUNTER_H
