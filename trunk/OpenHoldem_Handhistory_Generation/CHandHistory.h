#include <fstream>
#include "CPokerAction.h"
#include "CSymbols.h"

using namespace std;

class CHandHistory
{
public:
	CHandHistory();
	void makeHistory();

private:
	void updateSymbols();
	void setPreviousActions();
	string setDate();
	bool isShowdown();
	void roundStart();
	void checkBetround();
	void scanPlayerChanges();
    void GetPCstring(char *c, unsigned int c0, unsigned int c1);
	void GetBCstring(char *c, unsigned int c0);
	int DealPosition (const int chairnum);

	ofstream outfile;
	CPokerAction action;
	string splayername[10];
	bool allChecks[4];
	bool alreadySet[50];
	char playername[16];
	double currentbetx[10];
	double playerbalance[10];
	double potplayer;
	double sblind;
	double bblind;
	char card_common[5][5];
	char card_player[10][5];
	int prevplayerbalance[10];
	int prevbetx[10];
	int seatsPlaying[10];
	int ac_dealpos[10];
	int playersplayingbits[10];
	int nplayersactive;
	int raischair;
	int dealerchair;
	int betround;
	int postflopstart;
	int cbits;
	int gameNumber;
	int sblindpos;
	int bblindpos;
	int userchair;
	int passChecks;
	int maxBet;
	int whosturn;
	int prevround;
	int prevdealerchair;
};