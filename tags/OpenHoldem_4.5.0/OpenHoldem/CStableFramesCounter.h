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

#ifndef _INC_CSTABLEFRAMESCOUNTER_H
#define _INC_CSTABLEFRAMESCOUNTER_H

class CStableFramesCounter
{
public:
	CStableFramesCounter();
	~CStableFramesCounter();
public:
	unsigned int	NumberOfStableFrames() {return _NumberOfStableFrames;}
	unsigned int	UpdateNumberOfStableFrames();
private:
	void			Reset();
	void			SaveCurrentState();
private:
	unsigned int	_NumberOfStableFrames;
	bool			_isReset;
private:
	// variables to keep the last game-state.
	unsigned int	_myturnbitslast;
	unsigned int	_card_common_last[k_number_of_community_cards];
	unsigned int	_card_player_last[k_max_number_of_players][k_number_of_cards_per_player];
	bool			_dealer_last[k_max_number_of_players];
	double			_playerbalance_last[k_max_number_of_players];
	double			_playerbet_last[k_max_number_of_players];
};

extern CStableFramesCounter *p_stableframescounter;

#endif // _INC_CSTABLEFRAMESCOUNTER_H
