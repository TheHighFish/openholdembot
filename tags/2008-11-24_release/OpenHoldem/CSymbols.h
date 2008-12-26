#ifndef INC_CSYMBOLS_H
#define INC_CSYMBOLS_H

// Symbol structure
struct SSymbols 
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
	// Critical section used in public mutators and private shared variable writes
	static CRITICAL_SECTION	cs_symbols;

public:
	// public functions
	CSymbols();
	~CSymbols();
	void ResetSymbolsFirstTime(void);
	void CalcSymbols(void);
	void CalcTime(void);
	void CalcProbabilities(void);
	void CalcPrimaryFormulas(const bool final_answer);
	void CalcSecondaryFormulas(void);
	void UpdateAutoplayerInfo(void);
	const double GetSymbolVal(const char *a, int *e);
	const double CalcPokerval(const HandVal handval, const int ncards, double *pcbits, const int pcard0, const int pcard1);
	const void GetCardstring(char *c, const unsigned int c0, const unsigned int c1);
	const double IsHand(const char *a, int *e=NULL);
	const int GetSiteId (void);

public:
	// public accessors
	const bool			user_chair_confirmed() { return _user_chair_confirmed; }
	const double		bigbet() { return _bigbet; }
	const double		f$alli() { return _f$alli; }
	const double		f$swag() { return _f$swag; }
	const double		f$rais() { return _f$rais; }
	const double		f$call() { return _f$call; }
	const double		f$play() { return _f$play; }
	const double		f$prefold() { return _f$prefold; }
	const double		f$delay() { return _f$delay; }
	const double		f$chat() { return _f$chat; }
	const sprw1326		*prw1326() { return &_prw1326; }
	const SSymbols		*sym() { return &_sym; }
	const CArray <CString, CString> *logsymbols_collection() { return &_logsymbols_collection; }
	const CArray <CString, CString> *symboltrace_collection() { return &_symboltrace_collection; }
	const double		stacks_at_hand_start(const int n) { if (n>=0 && n<=9) return _stacks_at_hand_start[n]; else return 0.; }

public:
#define ENT EnterCriticalSection(&cs_symbols);
#define LEA LeaveCriticalSection(&cs_symbols);
	// public mutators
	void	set_reset_stakes(const bool b) { ENT _reset_stakes = b; LEA }
	void	set_sym_playing(const bool b) { ENT _sym.playing = b; LEA }
	void	set_elapsedautohold(time_t t) { ENT _elapsedautohold = t; LEA }
	sprw1326 *set_prw1326() { return &_prw1326; }
	CArray <CString, CString> *set_logsymbols_collection() { return &_logsymbols_collection; }
	CArray <CString, CString> *set_symboltrace_collection() { return &_symboltrace_collection; }

#undef ENT
#undef LEA

private:
	// private variables - use public accessors and public mutators to address these
	SSymbols	_sym;
	bool		_user_chair_confirmed;
	double		_f$alli, _f$swag, _f$rais, _f$call, _f$play, _f$prefold, _f$chat, _f$delay;
	double		_bigbet;
	bool		_reset_stakes;							// set to true on new hand or on change in title bar text
	double		_stacks_at_hand_start[10];				// Used in ICM calculator - ICM needs stacks at beginning of hand
	time_t		_elapsedautohold;						// The time since autoplayer acted
	CArray <CString, CString>   _logsymbols_collection; // Used to track the log$ symbols
	CArray <CString, CString>   _symboltrace_collection;// Used to trace function execution
	sprw1326	_prw1326;								//prwin 1326 data structure Matrix 2008-04-29

private:
	// private functions and variables - not available via accessors or mutators
	const double Chair$(const char *a);
	const double Chairbit$(const char *a);
	void ResetSymbolsNewHand(void);
	void ResetSymbolsNewRound(void);
	void ResetSymbolsEveryCalc(void);
	bool CalcUserChair(void);
	void CalcStakes(void);
	void CalcBetBalanceStack(void);
	void CalcPlayersFriendsOpponents(void);
	void CalcChipamtsLimits(void);
	void CalcNumbets(void);
	void CalcFlags(void);
	void CalcAutoplayer(void);
	void CalcRoundsPositions(void);
	void CalcPokerValues(void);
	void CalcUnknownCards(void);
	void CalcHandTests(void);
	void CalcPocketTests(void);
	void CalcListTests(void);
	void CalcNhands(void);
	void CalcHandrank(void);
	void CalcFlushesStraightsSets(void);
	void CalcRankbits(void);
	void CalcHistory(void);
	void CalcStatistics(void);
	void CalcRunRon(void);


	HandVal		_phandval[4], _chandval[4];	// for ishandup and ishandupcommon symbol calcs
	time_t		_elapsedhold;				// The time we "sat down"
	time_t		_elapsedhandhold;			// The time since start of last hand

	static double _dealerchair_last;
	static double _handnumber_last;
	static int _br_last;
	static unsigned int _player_card_last[2];
} *p_symbols;

#endif /* INC_CSYMBOLS_H */