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

	ofstream outfile;
	CPokerAction action;
	string splayername[10];
	HandVal handval[10];	//Holds hand value for all players
	CardMask hand[10];		//Holds CardMask of hand for all players
	bool allChecks[4];
	bool betroundSet[7];	//Checks if statements have already been run
	bool cardsSeen[10];		//Increments as player's cards are seen at showdown
	bool seatsPlaying[10];
	char playername[16];
	string handText[10];
	double currentbetx[10];
	double playerbalance[10];
	double potplayer;
	double sblind;
	double bblind;
	double pot;
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
	int postflopstart;	//Starting seat after flop
	int cbits;		//players playing bits
	int dbits;		//players dealt bits
	int gameNumber;
	int sblindpos;
	int bblindpos;
	int userchair;
	int passChecks;	//Whether checks have been passed over or not
	int maxBet;		//Maximum bet on table
	int whosturn;	//Turn determinant; used to move sequentially through seats
	int prevround;	//Betround in previous scrape
	int prevdealerchair;	//Dealer chair in previous scrape
	int lpta; //Last player to act on round
} *p_handhistory;

#endif // _INC_CHANDHISTORY_H