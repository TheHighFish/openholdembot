#ifndef _INC_CHANDHISTORY_H

#define _INC_CHANDHISTORY_H

#include <fstream>
#include <tchar.h>
#include <Windows.h>
#include <sstream>

#include "CPokerAction.h"
#include "CSymbols.h"
#include "CScraper.h"
#include "CGameState.h"
#include "CSessionCounter.h"

#include "poker_defs.h"
#include "enumerate.h"
#include "inlines/eval.h"
#include "inlines/eval_type.h"

using namespace std;

extern class CHandHistory
{
public:
	CHandHistory();
	//Main function, calls other functions based on table state
	void makeHistory();

private:
	//Updates needed symbols
	void updateSymbols();
	//Sets previous table variables
	void setPreviousActions();
	//Returns date for output in history
	string setDate();
	//Checks if there is a showdown
	bool isShowdown();
	//Runs at the beginning of each match; sets initial variables
	void roundStart();
	//Sets betround-dependent variables 
	void checkBetround();
	//Constantly scans the table for player changes, moving around
	//the table using whosturn
	void scanPlayerChanges();
	//Processes showdown info, including hands and hand values
	void processShowdown();
	//Gets a string readout of player cards
	void showdownResults();
    void GetPCstring(char *c, unsigned int c0, unsigned int c1);
	//Gets a string readout of board cards
	void GetBCstring(char *c, unsigned int c0);
	//Calculates ac_dealpos
	int DealPosition (const int chairnum);
	//Experimental, checks if the pot is uncontested
	void outputUncontested(int j);
	//Checks if passed chair is big blind 
	//(big blind pos, current bet=1, current betround=1)
	bool isBigBlind(int i);
	//Output table limit type
	string findLimit();
	bool cardsDealt();
	bool isPlaying(int i);
	bool hasMucked(int i);
	void checkSeats(int i, int j);
	double getSB(double i);
	void potUpdate(int i); //Beta function keeps track of multiple pots
	void resetVars();

	fstream outfile;
	CPokerAction action;
	string splayername[10];
	HandVal handval[10];	//Holds hand value for all players
	CardMask hand[10];		//Holds CardMask of hand for all players
	bool allChecks[4];
	bool betroundSet[9];	//Checks if statements have already been run
	/*
	TO BE UPDATED
	Because every poker game has specific events that must happen only once,
	and in order, I created an array of boolean variables to keep track
	betroundSet[0] - user hole cards
	betroundSet[1] - flop
	betroundSet[2] - turn
	betroundSet[3] - river
	betroundSet[4] - showdown
	betroundSet[5] - showdown cards displayed
	betroundSet[6] - uncontested pot
	betroundSet[7] - cards dealt
	betroundSet[8] - betround start
	*/
	bool cardsSeen[10];		//Increments as player's cards are seen at showdown
	bool seatsPlaying[10];
	bool newRoundFlag;
	bool postBlind[10];
	char playername[16];
	string handText[10];
	double currentbetx[10];
	double playerbalance[10];
	double potplayer;
	double bblind;
	double pot;
	double multipot[4];
	double prevpot;
	double bet[4];
	double rake;
	char card_common[5][5];
	char card_player[10][5];
	int nchairs;
	int pCardsSeen;
	int prevplayerbalance[10];
	int prevbetx[10];
	int ac_dealpos[10];
	int playersplayingbits[10];
	int playersdealtbits[10];
	int nplayersactive;
	int nplayersplaying;
	int raischair;
	int dealerchair;
	int betround;
	int utg;
	int postflopstart;	//Starting seat after flop
	int cbits;		//players playing bits
	int dbits;		//players dealt bits
	long int gameNumber;
	int sblindpos;
	int bblindpos;
	int userchair;
	int passChecks;	//Whether checks have been passed over or not
	int maxBet;		//Maximum bet on table
	int whosturn;	//Turn determinant; used to move sequentially through seats
	int prevround;	//Betround in previous scrape
	int prevdealerchair;	//Dealer chair in previous scrape
	int lpta; //Last player to act on round

	/*-------------Side Pots Variables----------------*/
	bool allin[10];		//True if player is allin
	bool allinflag[4];	//True if a player is allin in current betting round
	int potnum;		//Number of active pot
	double activepot[4];	//Holds pots, 0-Main, 1-3-Side pots
	double allincall[4];	//Amount to call allin raise
	double playerbet[10][4];

} *p_handhistory;

#endif // _INC_CHANDHISTORY_H