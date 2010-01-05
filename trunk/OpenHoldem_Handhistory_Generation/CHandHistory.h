#ifndef _INC_CHANDHISTORY_H
#define _INC_CHANDHISTORY_H

#include <fstream>

#include "CPokerAction.h"

using namespace std;

extern class CHandHistory
{
public:
	CHandHistory();
	//~CHandHistory();
	void makeHistory();

private:
	const string setDate(void);
	const string findLimit(void);
	const int DealPosition (const int chairnum);
	const double getSB(const double i);
	const bool NothingChanged(void);
	const bool isBigBlind(const int i);
	const bool isShowdown(void);
	const bool cardsDealt(void);
	const bool isPlaying(const int i);
	const bool hasMucked(const int i);
	void updateSymbols(void);
	void setPreviousActions(void);
	void roundStart(void);
	void checkBetround(void);
	void scanPlayerChanges(void);
	void processShowdown(void);
	void showdownResults(void);
    void GetPCstring(char *c, unsigned int c0, unsigned int c1);
	void GetBCstring(char *c, unsigned int c0);
	void outputUncontested(const int j);
	void checkSeats(const int i, int j);
	void writeHistory(void);
	void potUpdate(const int i);
	void resetVars(void);

private:
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
	string handText[10];
	double currentbetx[10];
	double prevprevbetx[10];
	double playerbalance[10];
	double prevplayerbalance[10];
	double pot;
	double prevbetx[10];
	double prevpot;
	double bet[4];
	double maxBet;		//Maximum bet on table
	char card_player[10][5];
	int pCardsSeen;
	int ac_dealpos[10];
	int playersplayingbits[10];
	int playersdealtbits[10];
	int postflopstart;	//Starting seat after flop
	long int gameNumber;
	int sblindpos;
	int bblindpos;
	int passChecks;	//Whether checks have been passed over or not
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
	double multipot[4];
} *p_handhistory;

#endif // _INC_CHANDHISTORY_H