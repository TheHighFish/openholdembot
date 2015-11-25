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

#ifndef _INC_CSTABLEFRAMESCOUNTER_H
#define _INC_CSTABLEFRAMESCOUNTER_H

class CStableFramesCounter {
 public:
	CStableFramesCounter();
	~CStableFramesCounter();
 public:
	unsigned int	NumberOfStableFrames() {return _NumberOfStableFrames;}
	unsigned int	UpdateNumberOfStableFrames();
 public:
  // After an autoplayer-action we expect a change in table-state.
  // Therefore we reset the counter to zero after actions
  // to avoid multiple clicks within a short frame of time
  // if the casino does not update its GUI fast enough.
  void ResetOnAutoplayerAction();
 private:
	void Reset();
	void SaveCurrentState();
 private:
	unsigned int _NumberOfStableFrames;
	bool			    _isReset;
 private:
	// variables to keep the last game-state.
	unsigned int	_myturnbitslast;
	unsigned int	_card_common_last[kNumberOfCommunityCards];
	unsigned int	_card_player_last[kMaxNumberOfPlayers][kNumberOfCardsPerPlayer];
	bool          _dealer_last[kMaxNumberOfPlayers];
	double        _playerbalance_last[kMaxNumberOfPlayers];
	double			  _playerbet_last[kMaxNumberOfPlayers];
};

extern CStableFramesCounter *p_stableframescounter;

#endif // _INC_CSTABLEFRAMESCOUNTER_H
