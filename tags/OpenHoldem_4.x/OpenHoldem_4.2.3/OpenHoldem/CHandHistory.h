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

#ifndef _INC_CHANDHISTORY_H
#define _INC_CHANDHISTORY_H
 
#include <fstream>
#include "MagicNumbers.h"

using namespace std;

extern class CHandHistory
{
public:
	CHandHistory();
	~CHandHistory();
public:
	//~CHandHistory(); 
	void MakeHistory();
	const phistory		*history() { return &_history; }

private:
	const string setDate();
	const bool isBigBlind(const int i);
	const bool isShowdown();
	const bool cardsDealt();
	const bool isPlaying(const int i);
	const bool hasMucked(const int i);
	void UpdateSymbols();
	void SetPreviousActions();
	void roundStart();
	void checkBetround();
	void ResetPostflopHistory();
	void scanPlayerChanges();
	void processShowdown();
	void GetPCstring(char *c, unsigned int c0, unsigned int c1);
	void GetBCstring(char *c, unsigned int c0);
	void checkSeats(const int i, int j);
	void WriteHistory();
	void SearchForAllinPlayers(const int i);
	void resetVars();
	void SetAction(int pnum, int action, double amount, int betround);
	void ReconstructHand(bool constested);
	void HandleNextAction();
	void HandleDealingPhase();
	void HandleShowdown(bool constested);
	void HandleNewBetround();
	void HandleUncontestedPot();
	void HandleContestedPot();
	void CalculateTotalPot();
	void CreateHandHistoryFile();
private:
	fstream outfile;
	string playerName[k_max_number_of_players];
	string handText[k_max_number_of_players];
	bool allin[k_max_number_of_players];				//True if player is allin
	bool SBfound;
	// Betrounds are numbered 1..4. 
	// That's why we use 1 additional element and leave 0 unused
	bool allChecks[k_number_of_betrounds + 1];
	bool flopSeen;
	bool turnSeen;
	bool riverSeen;
	bool showdownSeen;
	bool roundStarted;
	bool showdownReady;
	bool newRoundFlag;
	bool passChecks;			//Whether checks have been passed over or not
	double middleBet[k_max_number_of_players];
	double prevpot;
	// Betrounds are numbered 1..4. 
	// That's why we use 1 additional element and leave 0 unused
	double bet[k_number_of_betrounds + 1];
	double maxBet;				//Maximum bet on table
	int pCardsSeen;
	int postflopstart;			//Starting seat after flop
	long int gameNumber;
	int prevround;				//Betround in previous scrape
	int prevdealerchair;		//Dealer chair in previous scrape
	char card_global[5][5];
	phistory _history;			//phistory data structure Demonthus 2010-01-23
	double calculatedPot;
	int nplayersin;
	int next_chair_to_look_for_actions;
	bool potbetinto;
	bool hasFolded[k_max_number_of_players];;					
private:
	int nchairs;
	int betround;
	int userchair;
	int players_playing_bits;
	double pot;
	int bblind;
	int dealerchair;
} *p_handhistory;

#endif // _INC_CHANDHISTORY_H