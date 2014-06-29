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

#include "stdafx.h"
#include "CHandHistory.h"

#include <tchar.h> 
#include <Windows.h>
#include <sstream>
#include <iomanip>
#include "CBetroundCalculator.h"
#include "CGameState.h"
#include "CHandresetDetector.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CSessionCounter.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineDealerchair.h"
#include "CSymbolEnginePokerAction.h"
#include "CSymbolEngineRaisersCallers.h"
#include "CSymbolEngineTableLimits.h"
#include "CTableState.h"
#include "enumerate.h"
#include "inlines/eval.h"
#include "inlines/eval_type.h"
#include "MagicNumbers.h"
#include "NumericalFunctions.h"
#include "OpenHoldem.h"
#include "poker_defs.h"

using namespace std;

CHandHistory *p_handhistory = NULL;

// !! searching from -5 to 5 maybe?
// !! But what if we have less than 10 chairs or the user/dealer sits not in nr. 5?
const int WTF_means_minus_five = -5; 

const int k_betround_showdown = k_betround_river + 1;

// OK
CHandHistory::CHandHistory()
{
	return; //!! disabled

	CreateHandHistoryFile();
	// Attempt to create a random starting game number
	unsigned long seconds = time(NULL);
	// Attention! Might be not yet initialized
	gameNumber = (100000000 * p_sessioncounter->session_id()) + seconds/8; 


	newRoundFlag = false;
	for (int i = 0; i<k_max_number_of_players; i++)
	{
		_history.chair[i].currentBalance = 0;
	}
}

// Done
CHandHistory::~CHandHistory()
{
	outfile.close();
}

// Checked
void CHandHistory::MakeHistory()
{
	MessageBox(0, "The hand-history-generator is work in progress.\n"
		"It is nearly complete, but some things are not perfect.\n"
		"Unfortunatelly its creator left the project in the mean-time.\n"
		"\n"
		"You are very welcome to have a look at the sources\n"
		"and suggest some improvements.\n"
		"\n"
		"For the moment this feature has to be disabled.\n",
		"Hand-history generator", 0);
	return;
	write_log(preferences.debug_handhistory(), "[CHandHistory] MakeHistory()\n");
	CreateHandHistoryFile();//!! Bad
	UpdateSymbols();
	WriteHistory();
	SetPreviousActions();
	outfile.close(); //!! Bad
}

// Checked
void CHandHistory::WriteHistory()
{
	write_log(preferences.debug_handhistory(), "[CHandHistory] WriteHistory()\n");
	int	betround    = p_betround_calculator->betround();
	int	dealerchair = p_symbol_engine_dealerchair->dealerchair();

	// Precondition: New round
	if (p_handreset_detector->IsHandreset() && betround == k_betround_preflop)
	{
		newRoundFlag = true;
	}
	// Precondition: New round flag has been set and cards dealt
	if (newRoundFlag && cardsDealt())
	{
		roundStart();
	}
	checkBetround();
	// Precondition: Cards have been dealt and the round summary has not
	// been printed
	if (showdownSeen == false && roundStarted)
	{
		scanPlayerChanges(); 
	}
	if (isShowdown())
	{
		processShowdown();
	}
}

// Done
void CHandHistory::UpdateSymbols()
{
	// !! Attention!
	// It seems that these pointers can be undefined the very first time
	// Result: crashes!
	// Better turn everything into a "symbol engine"
	nchairs   = p_tablemap->nchairs();
	betround  = p_betround_calculator->betround();
	userchair = p_symbol_engine_userchair->userchair();
	players_playing_bits = p_symbol_engine_active_dealt_playing->playersplayingbits(); 
	pot = p_symbol_engine_chip_amounts->pot();
	bblind =      p_symbol_engine_tablelimits->bblind();
	userchair =   p_symbol_engine_userchair->userchair();
	dealerchair = p_symbol_engine_dealerchair->dealerchair();

	//Set individual player variables
	for (int i=0; i<nchairs; i++)	{
    if ((p_table_state->_players[i].hole_cards[0].IsKnownCard())
			  || ((i != userchair) && (userchair != 0))) {
			GetPCstring(_history.chair[i].card_player, 
        p_table_state->_players[i].hole_cards[0].GetValue(),
        p_table_state->_players[i].hole_cards[1].GetValue());
		}

		_history.chair[i].playersPlayingBits = IsBitSet(players_playing_bits, i);

		// If the player is playing, update the symbol. 
		// This condition gets used for muck detection.
		if (_history.chair[i].playersPlayingBits)
		{
			_history.chair[i].currentBet = p_symbol_engine_chip_amounts->currentbet(i);
		}
		_history.chair[i].currentBalance = p_symbol_engine_chip_amounts->balance(i);

		playerName[i] = p_scraper->player_name(i).GetString();
	}
}

// Checked
void CHandHistory::roundStart()
{
	resetVars();
	// Records if there is no small blind
	if (p_symbol_engine_chip_amounts->currentbet(_history.sblindpos) == 0)
	{
		// !! What if the BB occupies this seat?
		SBfound = false;
	}
	else
	{
		_history.chair[_history.sblindpos].totalIn[0] = p_symbol_engine_tablelimits->sblind();
	}

	//Assign put value of bblind in total pot
	_history.chair[_history.bblindpos].totalIn[0] = bblind;

	for (int i = 0; i<nchairs; i++)
	{
		if (isPlaying(i))
		{
			_history.chair[i].startBalance = _history.chair[i].currentBalance + _history.chair[i].currentBet;
			if (_history.chair[i].startBalance >= 0)
			{
				_history.chair[i].dealt = true;
			}
		}
	}

	for (int i=0; i<nchairs; i++)
	{
		if (_history.chair[i].currentBet == bblind
			&& i != (_history.bblindpos+1)%nchairs //??
			&& i != _history.bblindpos)
		{
			_history.chair[i].postBlind = true;
			_history.chair[i].totalIn[0] = bblind;
		}
	}
	roundStarted = true;
}

// Checked
void CHandHistory::checkBetround()
{
	char			card_common[k_number_of_community_cards][5]; // WTF is 5?

	for (int i = 0;i<k_number_of_community_cards;i++)	//Set board card letters
    GetBCstring(card_common[i], p_table_state->_common_cards[i].GetValue());

	for (int i=0; i<k_number_of_community_cards; i++)
	{
		if (card_common[i][1]!= NULL)
		{
			for (int j = 0;j<5;j++)
			{
				card_global[i][j] = card_common[i][j];
			}
		}
	}

	//Precondition: Has iterated through all players OR bblind has checked and hasn't been run and flop cards visible
	if ((_history.whosturn == ((_history.last_player_to_act+1)%nchairs)||allChecks[0])
			&&flopSeen==false
			&&showdownReady==false
			&&showdownSeen==false
		&&card_common[0][1]!= NULL&&card_common[1][1]!= NULL&&card_common[2][1]!= NULL)
	{
		if (allChecks[0]) 
		{
			SetAction(_history.bblindpos, 2, 0, 1);
		}
		flopSeen = true;
		ResetPostflopHistory();
	}
	//Precondition: Has iterated through all players OR players checked and hasn't been run and turn card visible
	if ((_history.whosturn == ((_history.last_player_to_act+1)%nchairs)||allChecks[1])
		&&flopSeen==true
		&&turnSeen==false
		&&showdownReady==false
		&&showdownSeen==false
		&&card_common[3][1]!= NULL) //??
	{
		turnSeen = true;
		ResetPostflopHistory();
	}
	//Precondition: Has iterated through all players OR players checked and hasn't been run and river card visible
	if ((_history.whosturn == ((_history.last_player_to_act+1)%nchairs)||allChecks[2])
		&&turnSeen==true
		&&riverSeen==false
		&&showdownReady==false
		&&showdownSeen==false
		&&card_common[4][1]!= NULL)
	{
		riverSeen = true;
		ResetPostflopHistory();
	}
	//Precondition: Has iterated through all players, hasn't been run, and is showdown
	if ((_history.whosturn == ((_history.last_player_to_act+1)%nchairs)||allChecks[3])
		&&riverSeen==true
		&&showdownReady==false
		&&showdownSeen&&isShowdown())
	{
		showdownReady = true;
	}
}

// OK
void CHandHistory::ResetPostflopHistory()
{
	_history.whosturn = postflopstart;
	_history.last_player_to_act = WTF_means_minus_five; 
	passChecks = false;
	maxBet = 0;
	for (int i=0; i<nchairs; i++)
	{
		_history.chair[i].actionCount = 0;
		middleBet[i] = 0;
		_history.chair[i].prevBet = 0;
	}
}

// Checked
void CHandHistory::scanPlayerChanges()
{
	double			bblind = p_symbol_engine_tablelimits->bblind();
	double			potplayer = p_symbol_engine_chip_amounts->potplayer();
	int				betround  = p_betround_calculator->betround();
	int				nchairs   = p_tablemap->nchairs();
	int				raischair = p_symbol_engine_raisers_callers->raischair();

	//Iterates through all chairs
	for (int i=0; i<nchairs; i++) 
	{
		//Precondition: It is the player's turn, and they are not past the
		//last to act
		if (_history.whosturn == i
			&&_history.whosturn != (_history.last_player_to_act+1)%nchairs)
		{
			//Precondition: Player is playing
			if (_history.chair[i].playersPlayingBits)
			{
				//Prevents loops during first betround if _history.utg called and dealer checked
				if (betround == 1
					&& i == _history.bblindpos
					&& _history.last_player_to_act == WTF_means_minus_five)
				{
					_history.last_player_to_act = _history.bblindpos;
				}
				/* 
				Detects previous checks; if it is not preflop, and amount in the
				the player pot is not zero, checks have not been passed and someone has
				raised, then set the turn to the raiser
				*/
				if (betround != 1
					&& potplayer != 0
					&& !passChecks  
					&& raischair != postflopstart) 
				{
					_history.whosturn = raischair;
					passChecks = true;
				}
				//Pass players who are allin
				if (allin[i])
				{
					_history.whosturn = (_history.whosturn+1)%nchairs;
				}
				//Precondition: Player has raised
				else if (_history.chair[i].currentBet > maxBet)
				{
					//If there were players who checked, output checks
					if (betround != 1
						&&i != postflopstart&&allChecks[betround-1])
					{
						int j = postflopstart;
						checkSeats(i, j);
					}
					double temp = maxBet;
					maxBet = _history.chair[i].currentBet;

					if (p_symbol_engine_tablelimits->gametype() != k_gametype_FL)
					{
						SearchForAllinPlayers(i);
					}

					if (allin[i])
					{
						SetAction(i, 3, (_history.chair[i].currentBet-_history.chair[i].prevBet), betround);
					}
					else if (temp == 0)
					{
						SetAction(i, 3, _history.chair[i].currentBet, betround);
					}
					else 
					{
						SetAction(i, 3, (_history.chair[i].currentBet-_history.chair[i].totalIn[betround-1]), betround);
					}

					_history.last_player_to_act = (i-1)%nchairs;	//Set _history.last_player_to_act to seat behind raiser
					if (_history.last_player_to_act == -1)
					{
						_history.last_player_to_act = nchairs-1;
					}
					_history.whosturn = (_history.whosturn+1)%nchairs;	//Increment _history.whosturn
					allChecks[betround-1] = false;
					passChecks = true;
				} 
				//Precondition: Player has called
				else if ((_history.chair[i].currentBet == maxBet 
					&& maxBet != 0
					&& !isBigBlind(i)
					&& !_history.chair[i].postBlind 
					&& !_history.chair[i].currentBet == bblind
					&& !betround == 1)
					||
					(prevround != betround
					&&betround != 1 
					&& maxBet != 0))
				{
					if (!p_symbol_engine_tablelimits->isfl())
					{
						SearchForAllinPlayers(i);
					}

					if (allin[i])
						SetAction(i, 2, _history.chair[i].prevBalance, prevround);
					else
					{
						int betround = prevround -1;
						if (betround > k_betround_turn)
						{
							betround = k_betround_turn;
						}
						SetAction(i, 2, (maxBet-_history.chair[i].totalIn[betround]), prevround);
					}

					_history.whosturn = (_history.whosturn+1)%nchairs;	//Increment _history.whosturn
				}
				/* 
				Attempts to check players who have posted the blind early.
				Precondition: Betround one, player posted blinds early, and no one
				has raised
				*/
				else if (betround == 1
					&& _history.chair[i].postBlind
					&& maxBet == bblind)
				{
					int next_to_act = WTF_means_minus_five;	//Next to act, initialized to safe value
					int count = 0;
					while (next_to_act < 0) //While next to act hasn't been found
					{
						int j = (i+count)%nchairs;
						if (isPlaying(j))
						{
							next_to_act = j;	//If player is playing, set next to act
						}
						else
						{
							count++;
						}
					}
					if (_history.chair[next_to_act].currentBet != 0)
					{
						SetAction( i, 2, 0, betround);
						_history.whosturn = (_history.whosturn+1)%nchairs;	//Increment _history.whosturn
					}
				}
			}
			//Precondition: Player has folded
			else if (!hasMucked(i))
			{
				//If they have not been reported as folding, output a fold
				if (_history.chair[i].seatIsPlaying&&isPlaying(i))
				{
					SetAction( i, 1, 0, prevround);
				}
				_history.chair[i].seatIsPlaying = false;
				_history.whosturn = (_history.whosturn+1)%nchairs;	//Increment _history.whosturn
				nplayersin = 0;
				for (int i = 0; i<nchairs; i++)
				{
					if (_history.chair[i].seatIsPlaying) 
					{
						nplayersin++;
					}
				}
				//If there is only one player left, run outputUncontested function
				if (nplayersin == 1)
				{
					for (int j=0; j<nchairs; j++)
					{
						if (_history.chair[j].seatIsPlaying)
						{
							SetAction(j, 4, 0, betround);
							if(showdownSeen == false)
							{
								ReconstructHand(false);
							}
						}
					}
				}
			}
		}
	}
}

// Checked
void CHandHistory::SetPreviousActions()
{
	prevdealerchair = dealerchair;
	prevround = betround;
	prevpot = pot;

	if (isShowdown())
	{
		prevround = k_betround_showdown;
	}

	for (int i=0; i<nchairs; i++) 
	{
		if (_history.chair[i].currentBalance != 0)
		{
			_history.chair[i].prevBalance = _history.chair[i].currentBalance;
		}
		if (middleBet[i] != _history.chair[i].currentBet)
		{
			_history.chair[i].prevBet = middleBet[i];
		}
		middleBet[i] = _history.chair[i].currentBet;
	}
}
const string CHandHistory::setDate()
{
	SYSTEMTIME			st;

	GetLocalTime(&st);
	stringstream ss;
	string s;
	ss << st.wYear << "-" << setfill ('0') << setw (2) 
		<< st.wMonth << "-" << setfill ('0') << setw (2) 
		<< st.wDay << " " << setfill ('0') << setw (2) 
		<< st.wHour << ":" << setfill ('0') << setw (2) 
		<< st.wMinute << ":" << setfill ('0') << setw (2) 
		<< st.wSecond << " ";
	s = ss.str();
	return s;
}

// OK
const bool CHandHistory::isShowdown()
{
	//If a player other than the user has cards showing, it is showdown
	for (int i = 0;i<nchairs;i++)
	{
		if (_history.chair[i].card_player[1] !=  NULL
			&& (i != userchair || userchair == 0)
			&& _history.chair[i].playersPlayingBits
			&& betround == 4)
		{
			return true;
		}
	}
	return false;
}

// !! Duplicate somewhere?
void CHandHistory::GetPCstring(char *c, unsigned int c0, unsigned int c1)
{
	char		card0[10] = {0}, card1[10] = {0};

	// figure out the card string to search for
	if (StdDeck_RANK(c0) >= StdDeck_RANK(c1))
	{
		StdDeck_cardToString(c0, card0);
		StdDeck_cardToString(c1, card1);
	}
	else
	{
		StdDeck_cardToString(c1, card0);
		StdDeck_cardToString(c0, card1);
	}
	c[0] = card0[0];
	c[1] = card0[1];
	c[2] = card1[0];
	c[3] = card1[1];

	c[4] = '\0';
}

// Done
void CHandHistory::GetBCstring(char *c, unsigned int c0)
{
	char		card0[10] = {0};

	// figure out the card string to search for
	StdDeck_cardToString(c0, card0);

	c[0] = card0[0];
	c[1] = card0[1];
	// !! What is with c[2]?
	c[3] = '\0';
}

// Done
void CHandHistory::processShowdown()
{
	write_log(preferences.debug_handhistory(), "[CHandHistory] ProcessShowdown()\n");
	int	nchairs         = p_tablemap->nchairs();
	int	nplayersplaying	= p_symbol_engine_active_dealt_playing->nplayersplaying();

	for (int i=0; i<nchairs; i++)
	{
		//Precondition: Player is playing, their cards haven't been processed yet
		//and the cards are visible
		if (_history.chair[i].playersPlayingBits != 0
			&& _history.chair[i].cardsSeen == false 
			&& _history.chair[i].card_player[1]!= NULL)
		{
			stringstream ss;
			ss << playerName[i] << ": Shows [ "
				<< _history.chair[i].card_player[1]
				<< _history.chair[i].card_player[0] << " "
				<< _history.chair[i].card_player[3]
				<< _history.chair[i].card_player[2] << " ]";
			handText[i] = ss.str()+" ";	//Stores hand output to be displayed later

			//Creates a complete hand mask for each player
			for (int j=0; j<k_number_of_cards_per_player; ++j) {
				CardMask_SET(_history.chair[i].hand,
          p_table_state->_players[i].hole_cards[j].GetValue());
      }
			for (int j=0;j<k_number_of_community_cards; j++) {
				CardMask_SET(_history.chair[i].hand,
          p_table_state->_common_cards[j].GetValue());
      }
			_history.chair[i].handval = Hand_EVAL_N(_history.chair[i].hand, 7);	//Evaluate cards
			pCardsSeen++;
			_history.chair[i].cardsSeen = true;	
		}
	}
	if(showdownReady && showdownSeen == false)
	{
		// Precondition: All players' hole cards have been seen and
		// the showdown results haven't been displayed
		if (pCardsSeen == nplayersplaying)
		{
			ReconstructHand(true);
		}
	}
}

// Done
const bool CHandHistory::isBigBlind(const int i)
{
	int	betround = p_betround_calculator->betround();

	//Determines if chair passed is the big blind
	return(_history.chair[i].currentBet == bet[betround-1]
		&& _history.chair[i].ac_dealpos == 2
		&& betround == k_betround_preflop);
}

// Checked
const bool CHandHistory::cardsDealt()
{
	int				playersdealt = 0;

	//If any cardbacks are showing, cards have been dealt
	for (int i=0; i<nchairs; ++i)
	{
    if (p_table_state->_players[i].HasKnownCards()) {
			playersdealt++;
		}
	}
	// !! What if it is headsup? Then only one player has cardbacks
	return (playersdealt>= 2);
}

// Semi-OK
const bool CHandHistory::isPlaying(const int chair)
{
	int	dbits = p_symbol_engine_active_dealt_playing->playersdealtbits();
	bool playersdealtbits = IsBitSet(dbits, chair);

	return (playersdealtbits
		&&_history.chair[chair].currentBalance >= 0); // !! What if he is allin?
		
}

// OK
const bool CHandHistory::hasMucked(const int chair)
{
	int	betround =  p_betround_calculator->betround();

	//Precondition: Betround is 4, player has called and has not folded
	// !! 3 means here: river and 4 means showdown
	return (betround == k_betround_river
		&& (_history.chair[chair].currentBet == maxBet
			|| _history.chair[chair].currentBalance == (_history.chair[chair].prevBalance - maxBet))
		&& _history.chair[chair].seatIsPlaying);
}

// !! Check For What?
// !! Why pass the loop-variable j as in-parameters?
void CHandHistory::checkSeats(const int i, int j)
{
	do
	{
		if (_history.chair[j].playersPlayingBits)
		{
			// !! What means 2?
			SetAction(j, 2, 0, prevround);
		}
		j = (j+1)%nchairs;
	}
	while(j!= i);
}

// CHECKED
// !! Seems like it could be completely removed
void CHandHistory::SearchForAllinPlayers(const int chair)
{
	int	betround = p_betround_calculator->betround();

	//Precondition: Player has gone allin, and this hasn't been caught yet
	if ((_history.chair[chair].currentBalance <= maxBet
			||_history.chair[chair].currentBalance == 0)
		&& allin[chair]) 
	{
		// !! allin will be set to true, only if it is true?
		allin[chair] = true;	
	}
}

// Done
void CHandHistory::resetVars()
{
	SBfound = true;
	newRoundFlag = false;
	flopSeen = false;
	turnSeen = false;
	riverSeen = false;
	showdownSeen = false;
	roundStarted = false;
	showdownReady = false;
	pCardsSeen = 0;
	prevround = 0;
	gameNumber++;
	maxBet = bblind;
	passChecks = false;
	_history.last_player_to_act = WTF_means_minus_five; 

	for (int i=k_betround_preflop; i<k_betround_river; i++)
	{
		allChecks[i] = true;
		bet[i] = p_symbol_engine_tablelimits->bet(i); 
	}
	for (int i=0; i<nchairs; i++)
	{
		_history.chair[i].ac_dealpos = p_symbol_engine_poker_action->DealPosition(i);
		if (_history.chair[i].ac_dealpos == 1)
		{
			_history.sblindpos = i;
		}
		else if (_history.chair[i].ac_dealpos == 2)
		{
			_history.bblindpos = i;
		}
		else if (_history.chair[i].ac_dealpos == 3)
		{
			_history.utg = i;
		}
		allin[i] = false;
		CardMask_RESET(_history.chair[i].hand);
		_history.chair[i].postBlind = false;
		_history.chair[i].seatIsPlaying = true;
		_history.chair[i].cardsSeen = false;
		_history.chair[i].dealt = false;
		_history.chair[i].handval = 0;
		handText[i] = "";
		middleBet[i] = 0;
		_history.chair[i].prevBet = 0;
		_history.chair[i].bets = 0;
		_history.chair[i].calls = 0;
		_history.chair[i].actionCount = 0;
		middleBet[i] = 0;
		for (int j=0; j<4 ;j++) // !! Betrounds are 1..4 
		{
			_history.chair[i].totalIn[j] = 0;
			for (int k=0; k<k_assumed_max_number_of_actions_per_player_and_round; k++) // !! WTF is 8?
			{
				_history.chair[i].bet[j][k] = 0;
				_history.chair[i].action[j][k] = 0;
			}
		}
	}
	next_chair_to_look_for_actions = _history.utg;
	_history.whosturn = _history.utg;
	postflopstart = _history.sblindpos;
}

// Done
void CHandHistory::SetAction(int pnum, int action, double amount, int betround)
{
	int				count = _history.chair[pnum].actionCount;
	betround--;

	if (betround > 3) // should be 1..4 !!
	{
		betround = 3;
	}
	else if (betround<0)
	{
		betround = 0;
	}

	if (action == 2)
	{
		_history.chair[pnum].calls++;
	}
	else if (action == 3)
	{
		_history.chair[pnum].bets++;
	}

	if (action!= 1 && amount!= 0)
	{
		if (count == 0)
		{
			_history.chair[pnum].totalIn[betround] = amount;
		}
		else
		{
			_history.chair[pnum].totalIn[betround] = maxBet;
		}
	}

	_history.chair[pnum].action[betround][count] = action;
	_history.chair[pnum].bet[betround][count] = amount;

	if (_history.chair[pnum].actionCount < k_assumed_max_number_of_actions_per_player_and_round)
	{
		_history.chair[pnum].actionCount++;
	}
	// !! Otherwise: too many action
}

// Done
void CHandHistory::ReconstructHand(bool constested)
{
	int				betround = 0;
	int				prevround = 0;

	nplayersin = 0;
	potbetinto = true;
	next_chair_to_look_for_actions = _history.utg;
	calculatedPot = 0;
	for (int i=0; i<nchairs; i++)
	{
		hasFolded[i] = false;
		_history.chair[i].actionCount = 0;
	}

	CalculateTotalPot();
	HandleDealingPhase();
	
	while(betround<4&&nplayersin>1)
	{
		HandleNextAction();

		if (prevround != betround)
		{
			potbetinto = false;
			HandleNewBetround();
		}
		prevround = betround;
	}
	HandleShowdown(constested);
	outfile.flush(); 
}

// Done
void CHandHistory::HandleNextAction()
{
	string name = playerName[next_chair_to_look_for_actions];
	int count   = _history.chair[next_chair_to_look_for_actions].actionCount;
	int action  = _history.chair[next_chair_to_look_for_actions].action[betround][count];
	double bet  = _history.chair[next_chair_to_look_for_actions].bet[betround][count];

	if (_history.chair[next_chair_to_look_for_actions].dealt)
	{
		if (action == 1) // !! Bad! Unnamed constants
		{
			outfile << name << ": Fold" << endl;
			hasFolded[next_chair_to_look_for_actions] = true;
			nplayersin--;
		}
		else if (action == 2)
			if (bet == 0)
				outfile << name << ": Check" << endl;
			else
				outfile << name << ": Call $" << bet << endl;
		else if (action == 3)
			if (!potbetinto)
			{
				outfile << name << ": Bet $" << bet << endl;
				potbetinto = true;
			}
			else
				outfile << name << ": Raise $" << bet << endl;
		else if (action == 0)
		{
			if (!hasFolded[next_chair_to_look_for_actions])
			{
				betround++;
				next_chair_to_look_for_actions = postflopstart;
				for (int i = 0;i<nchairs;i++) _history.chair[i].actionCount = 0;
			}
		}
	}
	_history.chair[next_chair_to_look_for_actions].actionCount++;
	next_chair_to_look_for_actions = (next_chair_to_look_for_actions + 1) % nchairs;
}

// Done
void CHandHistory::HandleNewBetround()
{
	if (betround == (k_betround_flop-1)) // !! betrounds should be 1..4, not 0..3
	{
		outfile << "*** FLOP *** [ "
			 << card_global[0][1] << card_global[0][0] << " "
			 << card_global[1][1] << card_global[1][0] << " "
			 << card_global[2][1] << card_global[2][0] << " ]" << endl;
	}
	else if (betround == (k_betround_turn-1))
	{
		outfile << "*** TURN *** [ "
			 << card_global[3][1] << card_global[3][0] << " ]" << endl;
	}
	else if (betround == (k_betround_river-1))
	{
		outfile << "*** RIVER *** [ "
			 << card_global[4][1] << card_global[4][0] << " ]" << endl;
	}
}

// Done
void CHandHistory::HandleUncontestedPot()
{
	for (int i = 0; i<nchairs ;i++)
	{
		if (_history.chair[i].seatIsPlaying)
		{
			outfile << playerName[i] << ": wins $" << calculatedPot << endl;
		}
	}
}

// Done
void CHandHistory::HandleContestedPot()
{
	for (int i=0; i<nchairs; i++)
	{
		if (handText[i]!= "")
		{
			outfile << handText[i] << endl;
		}
	}
	HandVal handhigh = {0};
	int highest = 0;
	for (int i = 0;i<nchairs;i++)
	{
		if (_history.chair[i].handval>handhigh)		//Find hand w/ highest value
		{
			handhigh = _history.chair[i].handval;
			highest = i;
		}
	}
	outfile << playerName[highest] << ": wins $" << calculatedPot << " from the main pot with ";
	int currentType = Hand_EVAL_TYPE(_history.chair[highest].hand,7);	//Find hand type
	switch(currentType)
	{
		case StdRules_HandType_STFLUSH: 
			outfile << "a Straight Flush." << endl; 
			break;
		case StdRules_HandType_QUADS: 
			outfile << "Quads." << endl; 
			break;
		case StdRules_HandType_FULLHOUSE: 
			outfile << "a Fullhouse." << endl; 
			break;
		case StdRules_HandType_FLUSH: 
			outfile << "a Flush." << endl; 
			break;
		case StdRules_HandType_STRAIGHT: 
			outfile << "a Straight." << endl; 
			break;
		case StdRules_HandType_TRIPS: 
			outfile << "Trips." << endl; 
			break;
		case StdRules_HandType_TWOPAIR: 
			outfile << "Two Pair." << endl; 
			break;
		case StdRules_HandType_ONEPAIR: 
			outfile << "One Pair." << endl; 
			break;
		case StdRules_HandType_NOPAIR: 
			outfile << "no Pair." << endl; 
			break;
	}
}

// Done
void CHandHistory::HandleShowdown(bool constested)
{
	outfile << "*** SUMMARY ***" << endl;
	// !! Rake-structure: maximum / tourneys not handled
	outfile << "Total pot $" << calculatedPot << " Rake $"
		 << (k_hand_history_rake * calculatedPot) << endl;
	if (constested)
	{
		HandleContestedPot();	
	}
	else 
	{
		HandleUncontestedPot();
	}
}

// Done
void CHandHistory::CalculateTotalPot()
{
	calculatedPot = 0;
	for (int i=0; i<nchairs; i++)
	{
		for (int j = 0;j<4; j++)  // betrounds 1..4
		{
			calculatedPot += _history.chair[i].totalIn[j];
		}
	}
}

// Done
void CHandHistory::HandleDealingPhase()
{
	//------------------DEALER--------------------//
	outfile << endl;
	outfile << "GAME #" << gameNumber 
		<< ": Texas Hold'em " << p_symbol_engine_tablelimits->GetGametypeAsString() 
		<< " $" << bblind << "/$" << (bblind*2) << 	" " 
		<< setDate() << endl;
	outfile << "Table " << p_scraper->title() << endl;
	for (int i=0; i<nchairs; i++)
	{
		if (_history.chair[i].dealt)
		{
			outfile << "Seat " << (i+1) << ": " << playerName[i] << " ( $" 
				 << _history.chair[i].startBalance << " in chips)";
			if (i == dealerchair) 
			{
				outfile << " DEALER" << endl;
			}
			else 
			{
				outfile << endl;
			}
			nplayersin++;
		}
	}
	if (SBfound)
	{
		outfile << playerName[_history.sblindpos]
			 << ": Post SB $"
			 << p_symbol_engine_tablelimits->sblind() 
			 << endl;
	}
	outfile << playerName[_history.bblindpos] << ": Post BB $" << bblind << endl;
	for (int i=0; i<nchairs; i++)
	{
		if (_history.chair[i].postBlind
			&& i != _history.sblindpos
			&& i != _history.bblindpos)
		{
			outfile << playerName[i] << ": Post BB $" << bblind << endl;
		}
	}
	//------------------ACTIONS--------------------//
	outfile << "*** HOLE CARDS ***" << endl;
	if (_history.chair[userchair].dealt && userchair != 0)
	{
		outfile << "Dealt to " << playerName[userchair] 
			<< " [ " 
			<< _history.chair[userchair].card_player[1] 
			<< _history.chair[userchair].card_player[0]
			<< " " 
			<< _history.chair[userchair].card_player[3] 
			<< _history.chair[userchair].card_player[2] 
			<< " ]" << endl;
	}
}

// Done
void CHandHistory::CreateHandHistoryFile()
{
	if (!preferences.handhistory_generator_enable())
	{
		write_log(preferences.debug_handhistory(), "[CHandHistory] Disabled. Not creating file.\n");
		return;
	}
	write_log(preferences.debug_handhistory(), "[CHandHistory] Enabled. Creating file if needed.\n");
	// Append to (or create if it does not exist) a handhistory file
	// using the session id as the name
	CreateDirectory("handhistory", NULL);
	stringstream ss;
	ss << "handhistory/handhistory_" << p_sessioncounter->session_id() << ".txt";
	string s = ss.str() + " ";
	outfile.open(s.c_str(), fstream::app);
	outfile.precision(2);
	outfile.setf(ios::fixed,ios::floatfield);
}
