#ifndef INC_SYMBOLS_H
#define INC_SYMBOLS_H

#include "poker_defs.h"
#include "structs_defines.h"

// Symbol structure
struct S_symbols 
{
	//General
	double ismanual;
	double isppro;
	double site;
	double nchairs;
	double isbring;
	double session;
	double handnumber;
	double version;

	//PROFILE
	double swagdelay;
	double allidelay;
	double swagtextmethod;
	double potmethod;
	double activemethod;

	//FORMULA FILE
	double rake;
	double nit;
	double bankroll;

	//LIMITS
	double bblind;
	double sblind;
	double ante;
	double lim;
	double isnl;
	double ispl;
	double isfl;
	double sraiprev;
	double sraimin;
	double sraimax;
	double istournament;

	//HAND RANK
	double handrank;
	double handrank169;
	double handrank2652;
	double handrank1326;
	double handrank1000;
	double handrankp;

	//CHAIRS
	double chair;
	double userchair;
	double dealerchair;
	double raischair;

	//ROUND&POSITIONS
	double betround;
	double br;
	double betposition;
	double dealposition;
	double callposition;
	double seatposition;
	double dealpositionrais;
	double betpositionrais;
	double originaldealposition; //Matrix 2008-05-09

	//PROBABILITIES
	double prwin;
	double prlos;
	double prtie;
	double random;
	double randomhand;
	double randomround[5];				// "randomround" is held in element 4, round specific in elements 0-3

	//STATISTICS
	double callror;
	double raisror;
	double srairor;
	double alliror;
	double callmean;
	double raismean;
	double sraimean;
	double allimean;
	double callvariance;
	double raisvariance;
	double sraivariance;
	double allivariance;

	//P FORMULA
	double defcon;
	double isdefmode;
	double isaggmode;

	//CHIP AMOUNTS
	double balance[11];				// "balance" is held in element 10, chair specific in elements 0-9
	double stack[10];				// chair specific in elements 0-9
	double currentbet[11];			// "currentbet" is held in element 10, chair specific in elements 0-9
	double call;
	double bet[5];					// "bet" is held in element 4, round specific in elements 0-3
	double pot;
	double potcommon;
	double potplayer;
	double callshort;
	double raisshort;

	//NUMBER OF BETS
	double nbetstocall;
	double nbetstorais;
	double ncurrentbets;
	double ncallbets;
	double nraisbets;

	//LIST TESTS
	double islistcall;
	double islistrais;
	double islistalli;
	double islist[MAX_HAND_LISTS];
	double isemptylistcall;
	double isemptylistrais;
	double isemptylistalli;

	//LIST NUMBERS
	double nlistmax;
	double nlistmin;

	//POKER VALUES
	double pokerval;
	double pokervalplayer;
	double pokervalcommon;
	double pcbits;
	double npcbits;

	//POKER VALUE CONSTANTS
	double hicard;
	double onepair;
	double twopair;
	double threeofakind;
	double straight;
	double flush;
	double fullhouse;
	double fourofakind;
	double straightflush;
	double royalflush;
	double fiveofakind;

	//HAND TESTS
	double $$pc[2];
	double $$pr[2];
	double $$ps[2];
	double $$cc[5];
	double $$cs[5];
	double $$cr[5];
	double ishandup;
	double ishandupcommon;
	double ishicard;
	double isonepair;
	double istwopair;
	double isthreeofakind;
	double isstraight;
	double isflush;
	double isfullhouse;
	double isfourofakind;
	double isstraightflush;
	double isroyalflush;
	double isfiveofakind;

	//POCKET TESTS
	double ispair;
	double issuited;
	double isconnector;

	//POCKET/COMMON TESTS
	double ishipair;
	double islopair;
	double ismidpair;
	double ishistraight;
	double ishiflush;

	//PLAYERS FRIENDS OPPONENTS
	double nopponents;
	double nopponentsmax;
	
	double nplayersseated;
	double nplayersactive;
	double nplayersdealt;
	double nplayersplaying;
	double nplayersblind;

	double nfriendsseated;
	double nfriendsactive;
	double nfriendsdealt;
	double nfriendsplaying;
	double nfriendsblind;
	
	double nopponentsseated;
	double nopponentsactive;
	double nopponentsdealt;
	double nopponentsplaying;
	double nopponentsblind;

	double nopponentschecking;
	double nopponentscalling;
	double nopponentsraising;
	double nopponentsbetting;
	double nopponentsfolded;

	double nplayerscallshort;
	double nchairsdealtright;
	double nchairsdealtleft;
	
	double playersseatedbits;
	double playersactivebits;
	double playersdealtbits;
	double playersplayingbits;
	double playersblindbits;

	double opponentsseatedbits;
	double opponentsactivebits;
	double opponentsdealtbits;
	double opponentsplayingbits;
	double opponentsblindbits;
	double bblindbits; //prwin change

	double friendsseatedbits;
	double friendsactivebits;
	double friendsdealtbits;
	double friendsplayingbits;
	double friendsblindbits;

	//FLAGS
	double fmax;
	double f[10];
	double fbits;

	//COMMON CARDS
	double ncommoncardspresent;
	double ncommoncardsknown;
	double nflopc;

	//(UN)KNOWN CARDS
	double nouts;
	double ncardsknown;
	double ncardsunknown;
	double ncardsbetter;

	//NHANDS
	double nhands;
	double nhandshi;
	double nhandslo;
	double nhandsti;
	double prwinnow;
	double prlosnow;

	//FLUSHES SETS STRAIGHTS
	double nsuited;
	double nsuitedcommon;
	double tsuit;
	double tsuitcommon;
	double nranked;
	double nrankedcommon;
	double trank;
	double trankcommon;
	double nstraight;
	double nstraightcommon;
	double nstraightfill;
	double nstraightfillcommon;
	double nstraightflush;
	double nstraightflushcommon;
	double nstraightflushfill;
	double nstraightflushfillcommon;

	//RANK BITS
	double rankbits;
	double rankbitscommon;
	double rankbitsplayer;
	double rankbitspoker;
	double srankbits;
	double srankbitscommon;
	double srankbitsplayer;
	double srankbitspoker;

	//RANK HI
	double rankhi;
	double rankhicommon;
	double rankhiplayer;
	double rankhipoker;
	double srankhi;
	double srankhicommon;
	double srankhiplayer;
	double srankhipoker;

	//RANK LO
	double ranklo;
	double ranklocommon;
	double rankloplayer;
	double ranklopoker;
	double sranklo;
	double sranklocommon;
	double srankloplayer;
	double sranklopoker;

	//TIME
	double elapsed;
	double elapsedhand;
	double elapsedauto;
	double elapsedtoday;
	double elapsed1970;
	double clocks;
	double nclockspersecond;
	double ncps;

	//AUTOPLAYER
	double myturnbits;
	double ismyturn;
	double issittingin;
	double issittingout;
	double isautopost;
	double isfinalanswer;

	//HISTORY
	double nplayersround[5];		// "nplayersround" is held in element 4, round specific in elements 0-3
	double prevaction;
	double nbetsround[5];			// "nbetsround" is held in element 4, round specific in elements 0-3
	double didchec[5];				// "didchec" is held in element 4, round specific in elements 0-3
	double didcall[5];				// "didcall" is held in element 4, round specific in elements 0-3
	double didrais[5];				// "didrais" is held in element 4, round specific in elements 0-3
	double didswag[5];				// "didswag" is held in element 4, round specific in elements 0-3

	//run$ ron$
	double ron$royfl;
	double ron$strfl;
	double ron$4kind;
	double ron$fullh;
	double ron$flush;
	double ron$strai;
	double ron$3kind;
	double ron$2pair;
	double ron$1pair;
	double ron$hcard;
	double ron$total;
	double ron$pokervalmax;
	double ron$prnuts;
	double ron$prbest;
	double ron$clocks;
	double run$royfl;
	double run$strfl;
	double run$4kind;
	double run$fullh;
	double run$flush;
	double run$strai;
	double run$3kind;
	double run$2pair;
	double run$1pair;
	double run$hcard;
	double run$total;
	double run$pokervalmax;
	double run$prnuts;
	double run$prbest;
	double run$clocks;

	// Versus
	double vs$nhands;
	double vs$nhandshi;
	double vs$nhandsti;
	double vs$nhandslo;
	double vs$prwin;
	double vs$prtie;
	double vs$prlos;
	double vs$prwinhi;
	double vs$prtiehi;
	double vs$prloshi;
	double vs$prwinti;
	double vs$prtieti;
	double vs$prlosti;
	double vs$prwinlo;
	double vs$prtielo;
	double vs$prloslo;
	bool   playing; //2008-03-25 Matrix
};

extern class CSymbols 
{
public:
	CSymbols::CSymbols();
	void ResetSymbolsFirstTime(void);
	void CalcSymbols(void);
	double GetSymbolVal(const char *a, int *e);
	double calc_pokerval(HandVal handval, int ncards, double *pcbits, int pcard0, int pcard1);
	void calc_time(void);
	void calc_probabilities(void);
	void get_cardstring(char *c, unsigned int c0, unsigned int c1);
	double IsHand(const char *a);

	S_symbols	sym;
	bool		user_chair_confirmed;
	double		f$alli, f$swag, f$rais, f$call, f$play, f$prefold;
	//  2008.02.27 by THF
	double f$chat;

	//  2008.04.02 by Spektre
	int f$delay;                             // Autoplayer delay in milliseconds

	double		bigbet;

	bool		reset_stakes;				// set to true on new hand or on change in title bar text

	double		stacks_at_hand_start[10];	// Used in ICM calculator - ICM needs stacks at beginning of hand

	time_t	elapsedautohold;				// The time since autoplayer acted

    CArray <CString, CString>   logsymbols_collection;    // Used to track the log$ symbols
    CArray <CString, CString>   symboltrace_collection;    // Used to trace function execution

	sprw1326	prw1326;					//prwin 1326 data structure Matrix 2008-04-29

private:
	double Chair$(const char *a);
	double Chairbit$(const char *a);
	void ResetSymbolsNewHand(void);
	void ResetSymbolsNewRound(void);
	void ResetSymbolsEveryCalc(void);
	bool calc_userchair(void);
	void calc_stakes(void);
	void calc_betbalancestack(void);
	void calc_playersfriendsopponents(void);
	void calc_chipamts_limits(void);
	void calc_numbets(void);
	void calc_flags(void);
	void calc_autoplayer(void);
	void calc_roundspositions(void);
	void calc_pokervalues(void);
	void calc_unknowncards(void);
	void calc_handtests(void);
	void calc_pockettests(void);
	void calc_listtests(void);
	void calc_nhands(void);
	void calc_handrank(void);
	void calc_fl_str_set(void);
	void calc_rankbits(void);
	void calc_history(void);
	void calc_statistics(void);
	void calc_run_ron(void);


	HandVal		phandval[4], chandval[4];	// for ishandup and ishandupcommon symbol calcs
	time_t		elapsedhold;				// The time we "sat down"
	time_t		elapsedhandhold;			// The time since start of last hand

} symbols;


#endif /* INC_SYMBOLS_H */