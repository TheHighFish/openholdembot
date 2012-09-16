#ifndef _INC_CHANDHISTORY_H
#define _INC_CHANDHISTORY_H
 
#include <fstream>
#include "MagicNumbers.h"

using namespace std;

extern class CHandHistory
{
public:
	CHandHistory();
	//~CHandHistory(); 
	void MakeHistory();
	const phistory		*history() { return &_history; }

private:
	const string setDate(void);
	const string findLimit(void);
	const int DealPosition (const int chairnum);
	const double getSB(const double i);
	const bool isBigBlind(const int i);
	const bool isShowdown(void);
	const bool cardsDealt(void);
	const bool isPlaying(const int i);
	const bool hasMucked(const int i);
	void UpdateSymbols(void);
	void SetPreviousActions(void);
	void roundStart(void);
	void checkBetround(void);
	void scanPlayerChanges(void);
	void processShowdown(void);
	void GetPCstring(char *c, unsigned int c0, unsigned int c1);
	void GetBCstring(char *c, unsigned int c0);
	void checkSeats(const int i, int j);
	void WriteHistory(void);
	void checkAllin(const int i);
	void resetVars(void);
	void SetAction(int pnum, int action, double amount, int br);
	void ReconstructHand(bool constested);

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
	double pot;
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
	phistory    _history;		//phistory data structure Demonthus 2010-01-23

} *p_handhistory;

#endif // _INC_CHANDHISTORY_H