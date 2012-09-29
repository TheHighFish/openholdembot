#ifndef INC_CSYMBOLS_H
#define INC_CSYMBOLS_H

#include <assert.h>
#include "CTableLimits.h"
#include "MagicNumbers.h"

// Symbol structure
struct SSymbols 
{
	//General
	double ismanual;
	double site;
	double nchairs;
	double isbring;
	double session;
	double version;

	//FORMULA FILE
	double nit;

	// TABLE-LIMITS 
	// Accessors could become part of CSymbols again,
	// in case we chance it to a class in the future
	// The logic got isolated in CTableLimits.
	// double bblind() 
	// double sblind() 
	// double ante()   
	// double lim()    
	// double isnl()   
	// double ispl()   
	// double isfl()   

	//LIMITS
	double sraiprev;
	double sraimin;
	double sraimax;
	double istournament;

	//HAND RANK
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
	// For internal use only: raischair_previous_frame
	double raischair_previous_frame;

	//ROUND&POSITIONS
	double betround;
	double betposition;
	double dealposition;
	double callposition;
	double seatposition;
	double dealpositionrais;
	double betpositionrais;
	double originaldealposition;

	//PROBABILITIES
	double prwin;
	double prlos;
	double prtie;

	// RANDOM
	double randomheartbeat;
	double randomhand;
	double randomround[5];				// "randomround" is held in element 4, round specific in elements 0-3

	//CHIP AMOUNTS
	double balance[11];				// "balance" is held in element 10, chair specific in elements 0-9
	double maxbalance;
	double originalbalance;
	double stack[10];				// chair specific in elements 0-9
	double currentbet[11];			// "currentbet" is held in element 10, chair specific in elements 0-9
	double call;
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
	double islist[MAX_HAND_LISTS];

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

	//PLAYERS, OPPONENTS
	double nopponents;
	double nopponentsmax;
	
	double nplayersseated;
	double nplayersactive;
	double nplayersdealt;
	double nplayersplaying;
	double nplayersblind;
	
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

	// CALLBITS, RAISBITS etc.
	// To avoid confusion we use bits 1..4, bit 0 is unused.
	int raisbits[k_number_of_betrounds+1];
	int callbits[k_number_of_betrounds+1];
	int foldbits[k_number_of_betrounds+1];

	//FLAGS
	double fmax;
	double f[k_number_of_flags];
	double fbits;

	//COMMON CARDS
	double ncommoncardspresent;
	double ncommoncardsknown;

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
	double botslastaction[5];		// "botslastaction" s held in element 4, round specific in elements 0-3
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

	double vs$nhandshinow;
	double vs$nhandstinow;
	double vs$nhandslonow;
	double vs$prwinhinow;
	double vs$prtiehinow;
	double vs$prloshinow;
	double vs$prwintinow;
	double vs$prtietinow;
	double vs$prlostinow;
	double vs$prwinlonow;
	double vs$prtielonow;
	double vs$prloslonow;

	bool   playing; //2008-03-25 Matrix
};

extern class CSymbols 
{
public:
	// public functions
	CSymbols();
	~CSymbols();
	void ResetSymbolsFirstTime(void);
	void CalcSymbols(void);
	void CalcTime(void);
	void CalcProbabilities(void);
	void RecordPrevAction(const ActionConstant action);
	const double GetSymbolVal(const char *a, int *e);
	const double CalcPokerval(const HandVal handval, const int ncards, double *pcbits, const int pcard0, const int pcard1);
	const void GetCardstring(char *c, const unsigned int c0, const unsigned int c1);
	const double IsHand(const char *a, int *e=NULL);
	bool	DidAct(void);

public:
	// public accessors
	const bool			user_chair_confirmed() { return _user_chair_confirmed; }
	const double		bigbet()	{ return p_tablelimits->bbet(); }
	const sprw1326		*prw1326()	{ return &_prw1326; }
	const CArray <CString, CString> *logsymbols_collection() { return &_logsymbols_collection; }
	const CArray <CString, CString> *symboltrace_collection() { return &_symboltrace_collection; }
	const double		stacks_at_hand_start(const int n) { if (n>=0 && n<=9) return _stacks_at_hand_start[n]; else return 0.; }
	const SSymbols		*sym() { return &_sym; }

public:
#define ENT CSLock lock(m_critsec);
	// public mutators
	void	set_user_chair_confirmed(const bool b) { ENT _user_chair_confirmed = b;}
	void	set_prw1326_useme(const int i)	{ ENT _prw1326.useme = i;}
	void	logsymbols_collection_add(const char *s)	{ ENT _logsymbols_collection.Add(s);}
	void	logsymbols_collection_removeall()			{ ENT _logsymbols_collection.RemoveAll();}
	void	symboltrace_collection_add(const char *s)	{ ENT _symboltrace_collection.Add(s);}
	void	symboltrace_collection_removeall()			{ ENT _symboltrace_collection.RemoveAll();}
	void	set_stacks_at_hand_start(const int i, const int d)	{ ENT if (i>=0 && i<=9) _stacks_at_hand_start[i] = d; }

	void	reset_elapsedautohold() 
	{ 
		ENT
		time_t my_time_t;
		time(&my_time_t);
		_elapsedautohold = my_time_t;
	}

	// All symbol mutators below
	// general
	void	set_sym_ismanual(const double d) { ENT _sym.ismanual = d;}
	void	set_sym_site(const double d) { ENT _sym.site = d;}
	void	set_sym_nchairs(const double d) { ENT _sym.nchairs = d;}
	void	set_sym_isbring(const double d) { ENT _sym.isbring = d;}
	void	set_sym_session(const double d) { ENT _sym.session = d;};
	void	set_sym_version(const double d) { ENT _sym.version = d;}

	// formula
	void	set_sym_nit(const double d) { ENT _sym.nit = d;}

	// limits
	void	set_sym_sraiprev(const double d) { ENT _sym.sraiprev = d;}
	void	set_sym_sraimin(const double d) { ENT _sym.sraimin = d;}
	void	set_sym_sraimax(const double d) { ENT _sym.sraimax = d;}
	void	set_sym_istournament(const double d) { ENT _sym.istournament = d;}

	// handrank
	void	set_sym_handrank169(const double d) { ENT _sym.handrank169 = d;}
	void	set_sym_handrank2652(const double d) { ENT _sym.handrank2652 = d;}
	void	set_sym_handrank1326(const double d) { ENT _sym.handrank1326 = d;}
	void	set_sym_handrank1000(const double d) { ENT _sym.handrank1000 = d;}
	void	set_sym_handrankp(const double d) { ENT _sym.handrankp = d;}

	// chairs
	void	set_sym_chair(const double d) { ENT _sym.chair = d;}
	void	set_sym_userchair(const double d) { ENT _sym.userchair = d;}
	void	set_sym_dealerchair(const double d) { ENT _sym.dealerchair = d;}
	void	set_sym_raischair(const double d) { ENT _sym.raischair = d;}

	// rounds positions
	void	set_sym_betround(const double d) { ENT _sym.betround = d;}
	void	set_sym_betposition(const double d) { ENT _sym.betposition = d;}
	void	set_sym_dealposition(const double d) { ENT _sym.dealposition = d;}
	void	set_sym_callposition(const double d) { ENT _sym.callposition = d;}
	void	set_sym_seatposition(const double d) { ENT _sym.seatposition = d;}
	void	set_sym_dealpositionrais(const double d) { ENT _sym.dealpositionrais = d;}
	void	set_sym_betpositionrais(const double d) { ENT _sym.betpositionrais = d;}
	void	set_sym_originaldealposition(const double d) { ENT _sym.originaldealposition = d;}

	// probabilities
	void	set_sym_prwin(const double d) { ENT _sym.prwin = d;}
	void	set_sym_prlos(const double d) { ENT _sym.prlos = d;}
	void	set_sym_prtie(const double d) { ENT _sym.prtie = d;}

	// random
	void	set_sym_randomheartbeat(const double d) { ENT _sym.randomheartbeat = d;}
	void	set_sym_randomhand(const double d) { ENT _sym.randomhand = d;}
	void	set_sym_randomround(const int i, const double d) { ENT _sym.randomround[i] = d;}

	// chip amounts
	void	reset_sym_maxbalance() { ENT _sym.maxbalance = 0; }
	void	reset_sym_originalbalance() { ENT _sym.originalbalance = 0; }

	void	set_sym_balance(const int i, const double d) 
	{
		ENT 
		if (i>=0 && i<=10) _sym.balance[i] = d;
		if (i == 10) 
		{
			set_sym_max_balance_conditionally(d);
			set_sym_originalbalance_conditionally(d);
		}
	}
	void	set_sym_stack(const int i, const double d) { ENT if (i>=0 && i<=9) _sym.stack[i] = d;}
	void	set_sym_currentbet(const int i, const double d) { ENT if (i>=0 && i<=10) _sym.currentbet[i] = d;}
	void	set_sym_call(const double d) { ENT _sym.call = d;}
	void	set_sym_pot(const double d) { ENT _sym.pot = d;}
	void	set_sym_potcommon(const double d) { ENT _sym.potcommon = d;}
	void	set_sym_potplayer(const double d) { ENT _sym.potplayer = d;}
	void	set_sym_callshort(const double d) { ENT _sym.callshort = d;}
	void	set_sym_raisshort(const double d) { ENT _sym.raisshort = d;}

	// number of bets
	void	set_sym_nbetstocall(const double d) { ENT _sym.nbetstocall = d;}
	void	set_sym_nbetstorais(const double d) { ENT _sym.nbetstorais = d;}
	void	set_sym_ncurrentbets(const double d) { ENT _sym.ncurrentbets = d;}
	void	set_sym_ncallbets(const double d) { ENT _sym.ncallbets = d;}
	void	set_sym_nraisbets(const double d) { ENT _sym.nraisbets = d;}

	// list tests
	void	set_sym_islist(const int i, const double d) { ENT if (i>=0 && i<MAX_HAND_LISTS) _sym.islist[i] = d;}

	// poker values
	void	set_sym_pokerval(const double d) { ENT _sym.pokerval = d;}
	void	set_sym_pokervalplayer(const double d) { ENT _sym.pokervalplayer = d;}
	void	set_sym_pokervalcommon(const double d) { ENT _sym.pokervalcommon = d;}
	void	set_sym_pcbits(const double d) { ENT _sym.pcbits = d;}
	void	set_sym_npcbits(const double d) { ENT _sym.npcbits = d;}

	// poker value constants
	void	set_sym_hicard(const double d) { ENT _sym.hicard = d;}
	void	set_sym_onepair(const double d) { ENT _sym.onepair = d;}
	void	set_sym_twopair(const double d) { ENT _sym.twopair = d;}
	void	set_sym_threeofakind(const double d) { ENT _sym.threeofakind = d;}
	void	set_sym_straight(const double d) { ENT _sym.straight = d;}
	void	set_sym_flush(const double d) { ENT _sym.flush = d;}
	void	set_sym_fullhouse(const double d) { ENT _sym.fullhouse = d;}
	void	set_sym_fourofakind(const double d) { ENT _sym.fourofakind = d;}
	void	set_sym_straightflush(const double d) { ENT _sym.straightflush = d;}
	void	set_sym_royalflush(const double d) { ENT _sym.royalflush = d;}
	void	set_sym_fiveofakind(const double d) { ENT _sym.fiveofakind = d;}

	// hand tests
	void	set_sym_$$pc(const int i, const double d) { ENT if (i>=0 && i<=1) _sym.$$pc[i] = d;}
	void	set_sym_$$pr(const int i, const double d) { ENT if (i>=0 && i<=1) _sym.$$pr[i] = d;}
	void	set_sym_$$ps(const int i, const double d) { ENT if (i>=0 && i<=1) _sym.$$ps[i] = d;}
	void	set_sym_$$cc(const int i, const double d) { ENT if (i>=0 && i<=4) _sym.$$cc[i] = d;}
	void	set_sym_$$cs(const int i, const double d) { ENT if (i>=0 && i<=4) _sym.$$cs[i] = d;}
	void	set_sym_$$cr(const int i, const double d) { ENT if (i>=0 && i<=4) _sym.$$cr[i] = d;}
	void	set_sym_ishandup(const double d) { ENT _sym.ishandup = d;}
	void	set_sym_ishandupcommon(const double d) { ENT _sym.ishandupcommon = d;}
	void	set_sym_ishicard(const double d) { ENT _sym.ishicard = d;}
	void	set_sym_isonepair(const double d) { ENT _sym.isonepair = d;}
	void	set_sym_istwopair(const double d) { ENT _sym.istwopair = d;}
	void	set_sym_isthreeofakind(const double d) { ENT _sym.isthreeofakind = d;}
	void	set_sym_isstraight(const double d) { ENT _sym.isstraight = d;}
	void	set_sym_isflush(const double d) { ENT _sym.isflush = d;}
	void	set_sym_isfullhouse(const double d) { ENT _sym.isfullhouse = d;}
	void	set_sym_isfourofakind(const double d) { ENT _sym.isfourofakind = d;}
	void	set_sym_isstraightflush(const double d) { ENT _sym.isstraightflush = d;}
	void	set_sym_isroyalflush(const double d) { ENT _sym.isroyalflush = d;}

	// pocket tests
	void	set_sym_ispair(const double d) { ENT _sym.ispair = d;}
	void	set_sym_issuited(const double d) { ENT _sym.issuited = d;}
	void	set_sym_isconnector(const double d) { ENT _sym.isconnector = d;}

	// pocket/common tests
	void	set_sym_ishipair(const double d) { ENT _sym.ishipair = d;}
	void	set_sym_islopair(const double d) { ENT _sym.islopair = d;}
	void	set_sym_ismidpair(const double d) { ENT _sym.ismidpair = d;}
	void	set_sym_ishistraight(const double d) { ENT _sym.ishistraight = d;}
	void	set_sym_ishiflush(const double d) { ENT _sym.ishiflush = d;}

	// players, opponents
	void	set_sym_nopponents(const double d) { ENT _sym.nopponents = d;}
	void	set_sym_nopponentsmax(const double d) { ENT _sym.nopponentsmax = d;}
	void	set_sym_nplayersseated(const double d) { ENT _sym.nplayersseated = d;}
	void	set_sym_nplayersactive(const double d) { ENT _sym.nplayersactive = d;}
	void	set_sym_nplayersdealt(const double d) { ENT _sym.nplayersdealt = d;}
	void	set_sym_nplayersplaying(const double d) { ENT _sym.nplayersplaying = d;}
	void	set_sym_nplayersblind(const double d) { ENT _sym.nplayersblind = d;}
	void	set_sym_nopponentsseated(const double d) { ENT _sym.nopponentsseated = d;}
	void	set_sym_nopponentsactive(const double d) { ENT _sym.nopponentsactive = d;}
	void	set_sym_nopponentsdealt(const double d) { ENT _sym.nopponentsdealt = d;}
	void	set_sym_nopponentsplaying(const double d) { ENT _sym.nopponentsplaying = d;}
	void	set_sym_nopponentsblind(const double d) { ENT _sym.nopponentsblind = d;}
	void	set_sym_nopponentschecking(const double d) { ENT _sym.nopponentschecking = d;}
	void	set_sym_nopponentscalling(const double d) { ENT _sym.nopponentscalling = d;}
	void	set_sym_nopponentsraising(const double d) { ENT _sym.nopponentsraising = d;}
	void	set_sym_nopponentsbetting(const double d) { ENT _sym.nopponentsbetting = d;}
	void	set_sym_nopponentsfolded(const double d) { ENT _sym.nopponentsfolded = d;}
	void	set_sym_nplayerscallshort(const double d) { ENT _sym.nplayerscallshort = d;}
	void	set_sym_nchairsdealtright(const double d) { ENT _sym.nchairsdealtright = d;}
	void	set_sym_nchairsdealtleft(const double d) { ENT _sym.nchairsdealtleft = d;}
	void	set_sym_playersseatedbits(const double d) { ENT _sym.playersseatedbits = d;}
	void	set_sym_playersactivebits(const double d) { ENT _sym.playersactivebits = d;}
	void	set_sym_playersdealtbits(const double d) { ENT _sym.playersdealtbits = d;}
	void	set_sym_playersplayingbits(const double d) { ENT _sym.playersplayingbits = d;}
	void	set_sym_playersblindbits(const double d) { ENT _sym.playersblindbits = d;}
	void	set_sym_opponentsseatedbits(const double d) { ENT _sym.opponentsseatedbits = d;}
	void	set_sym_opponentsactivebits(const double d) { ENT _sym.opponentsactivebits = d;}
	void	set_sym_opponentsdealtbits(const double d) { ENT _sym.opponentsdealtbits = d;}
	void	set_sym_opponentsplayingbits(const double d) { ENT _sym.opponentsplayingbits = d;}
	void	set_sym_opponentsblindbits(const double d) { ENT _sym.opponentsblindbits = d;}
	void	set_sym_bblindbits(const double d) { ENT _sym.bblindbits = d; };
	
	// callbits, raisbits, etc.
	void	set_sym_raisbits(const int i, const int betround) { ENT assert(betround >= k_betround_current); assert(betround <= k_betround_river); _sym.raisbits[betround] = i; }
	void	set_sym_callbits(const int i, const int betround) { ENT assert(betround >= k_betround_current); assert(betround <= k_betround_river); _sym.callbits[betround] = i; }
	void	set_sym_foldbits(const int i, const int betround) { ENT assert(betround >= k_betround_current); assert(betround <= k_betround_river); _sym.foldbits[betround] = i; }

	//flags
	void	set_sym_fmax(const double d) { ENT _sym.fmax = d;}
	void	set_sym_f(const int i, const double d) { ENT assert(i>=0); assert(i<k_number_of_flags); _sym.f[i] = d;}
	void	set_sym_fbits(const double d) { ENT _sym.fbits = d;}

	// common cards
	void	set_sym_ncommoncardspresent(const double d) { ENT _sym.ncommoncardspresent = d;}
	void	set_sym_ncommoncardsknown(const double d) { ENT _sym.ncommoncardsknown = d;}

	// (un)known cards
	void	set_sym_nouts(const double d) { ENT _sym.nouts = d;}
	void	set_sym_ncardsknown(const double d) { ENT _sym.ncardsknown = d;}
	void	set_sym_ncardsunknown(const double d) { ENT _sym.ncardsunknown = d;}
	void	set_sym_ncardsbetter(const double d) { ENT _sym.ncardsbetter = d;}

	// nhands
	void	set_sym_nhands(const double d) { ENT _sym.nhands = d;}
	void	set_sym_nhandshi(const double d) { ENT _sym.nhandshi = d;}
	void	set_sym_nhandslo(const double d) { ENT _sym.nhandslo = d;}
	void	set_sym_nhandsti(const double d) { ENT _sym.nhandsti = d;}
	void	set_sym_prwinnow(const double d) { ENT _sym.prwinnow = d;}
	void	set_sym_prlosnow(const double d) { ENT _sym.prlosnow = d;}

	// flushes straights sets
	void	set_sym_nsuited(const double d) { ENT _sym.nsuited = d;}
	void	set_sym_nsuitedcommon(const double d) { ENT _sym.nsuitedcommon = d;}
	void	set_sym_tsuit(const double d) { ENT _sym.tsuit = d;}
	void	set_sym_tsuitcommon(const double d) { ENT _sym.tsuitcommon = d;}
	void	set_sym_nranked(const double d) { ENT _sym.nranked = d;}
	void	set_sym_nrankedcommon(const double d) { ENT _sym.nrankedcommon = d;}
	void	set_sym_trank(const double d) { ENT _sym.trank = d;}
	void	set_sym_trankcommon(const double d) { ENT _sym.trankcommon = d;}
	void	set_sym_nstraight(const double d) { ENT _sym.nstraight = d;}
	void	set_sym_nstraightcommon(const double d) { ENT _sym.nstraightcommon = d;}
	void	set_sym_nstraightflush(const double d) { ENT _sym.nstraightflush = d;}
	void	set_sym_nstraightflushcommon(const double d) { ENT _sym.nstraightflushcommon = d;}
	void	set_sym_nstraightfill(const double d) { ENT _sym.nstraightfill = d;}
	void	set_sym_nstraightfillcommon(const double d) { ENT _sym.nstraightfillcommon = d;}
	void	set_sym_nstraightflushfill(const double d) { ENT _sym.nstraightflushfill = d;}
	void	set_sym_nstraightflushfillcommon(const double d) { ENT _sym.nstraightflushfillcommon = d;}

	// rank bits
	void	set_sym_rankbits(const double d) { ENT _sym.rankbits = d;}
	void	set_sym_rankbitscommon(const double d) { ENT _sym.rankbitscommon = d;}
	void	set_sym_rankbitsplayer(const double d) { ENT _sym.rankbitsplayer = d;}
	void	set_sym_rankbitspoker(const double d) { ENT _sym.rankbitspoker = d;}
	void	set_sym_srankbits(const double d) { ENT _sym.srankbits = d;}
	void	set_sym_srankbitscommon(const double d) { ENT _sym.srankbitscommon = d;}
	void	set_sym_srankbitsplayer(const double d) { ENT _sym.srankbitsplayer = d;}
	void	set_sym_srankbitspoker(const double d) { ENT _sym.srankbitspoker = d;}

	// rank hi
	void	set_sym_rankhi(const double d) { ENT _sym.rankhi = d;}
	void	set_sym_rankhicommon(const double d) { ENT _sym.rankhicommon = d;}
	void	set_sym_rankhiplayer(const double d) { ENT _sym.rankhiplayer = d;}
	void	set_sym_rankhipoker(const double d) { ENT _sym.rankhipoker = d;}
	void	set_sym_srankhi(const double d) { ENT _sym.srankhi = d;}
	void	set_sym_srankhicommon(const double d) { ENT _sym.srankhicommon = d;}
	void	set_sym_srankhiplayer(const double d) { ENT _sym.srankhiplayer = d;}
	void	set_sym_srankhipoker(const double d) { ENT _sym.srankhipoker = d;}

	// rank lo
	void	set_sym_ranklo(const double d) { ENT _sym.ranklo = d;}
	void	set_sym_ranklocommon(const double d) { ENT _sym.ranklocommon = d;}
	void	set_sym_rankloplayer(const double d) { ENT _sym.rankloplayer = d;}
	void	set_sym_ranklopoker(const double d) { ENT _sym.ranklopoker = d;}
	void	set_sym_sranklo(const double d) { ENT _sym.sranklo = d;}
	void	set_sym_sranklocommon(const double d) { ENT _sym.sranklocommon = d;}
	void	set_sym_srankloplayer(const double d) { ENT _sym.srankloplayer = d;}
	void	set_sym_sranklopoker(const double d) { ENT _sym.sranklopoker = d;}

	// time
	void	set_sym_elapsed(const double d) { ENT _sym.elapsed = d;}
	void	set_sym_elapsedhand(const double d) { ENT _sym.elapsedhand = d;}
	void	set_sym_elapsedauto(const double d) { ENT _sym.elapsedauto = d;}
	void	set_sym_elapsedtoday(const double d) { ENT _sym.elapsedtoday = d;}
	void	set_sym_elapsed1970(const double d) { ENT _sym.elapsed1970 = d;}

	// autoplayer
	void	set_sym_myturnbits(const double d) { ENT _sym.myturnbits = d;}
	void	set_sym_ismyturn(const double d) { ENT _sym.ismyturn = d;}
	void	set_sym_issittingin(const double d) { ENT _sym.issittingin = d;}
	void	set_sym_isautopost(const double d) { ENT _sym.isautopost = d;}
	void	set_sym_issittingout(const double d) { ENT _sym.issittingout = d;}
	void	set_sym_isfinalanswer(const double d) { ENT _sym.isfinalanswer = d;}

	// history
	// 0..3 are for flop..river, 4 is for the current round
	// candidate for refactoring
	void	set_sym_nplayersround(const int betround, const double d) { ENT if (betround>=0 && betround<=4) _sym.nplayersround[betround] = d;}
	void	set_sym_nbetsround(const int betround, const double d) { ENT if (betround>=0 && betround<=4) _sym.nbetsround[betround] = d;}
	void	set_sym_didchec(const int betround, const double d) { ENT if (betround>=0 && betround<=4) _sym.didchec[betround] = d;}
	void	set_sym_didcall(const int betround, const double d) { ENT if (betround>=0 && betround<=4) _sym.didcall[betround] = d;}
	void	set_sym_didrais(const int betround, const double d) { ENT if (betround>=0 && betround<=4) _sym.didrais[betround] = d;}
	void	set_sym_didswag(const int betround, const double d) { ENT if (betround>=0 && betround<=4) _sym.didswag[betround] = d;}
	void	set_sym_prevaction(const double d) { ENT _sym.prevaction = d;}
	void	set_sym_botslastaction(const int betround, const double d) { ENT if (betround>=0 && betround<=4) _sym.botslastaction[betround] = d;}

	//run$ ron$
	void	set_sym_ron$royfl(const double d) { ENT _sym.ron$royfl = d;}
	void	set_sym_ron$strfl(const double d) { ENT _sym.ron$strfl = d;}
	void	set_sym_ron$4kind(const double d) { ENT _sym.ron$4kind = d;}
	void	set_sym_ron$fullh(const double d) { ENT _sym.ron$fullh = d;}
	void	set_sym_ron$flush(const double d) { ENT _sym.ron$flush = d;}
	void	set_sym_ron$strai(const double d) { ENT _sym.ron$strai = d;}
	void	set_sym_ron$3kind(const double d) { ENT _sym.ron$3kind = d;}
	void	set_sym_ron$2pair(const double d) { ENT _sym.ron$2pair = d;}
	void	set_sym_ron$1pair(const double d) { ENT _sym.ron$1pair = d;}
	void	set_sym_ron$hcard(const double d) { ENT _sym.ron$hcard = d;}
	void	set_sym_ron$total(const double d) { ENT _sym.ron$total = d;}
	void	set_sym_ron$pokervalmax(const double d) { ENT _sym.ron$pokervalmax = d;}
	void	set_sym_ron$prnuts(const double d) { ENT _sym.ron$prnuts = d;}
	void	set_sym_ron$prbest(const double d) { ENT _sym.ron$prbest = d;}
	void	set_sym_run$royfl(const double d) { ENT _sym.run$royfl = d;}
	void	set_sym_run$strfl(const double d) { ENT _sym.run$strfl = d;}
	void	set_sym_run$4kind(const double d) { ENT _sym.run$4kind = d;}
	void	set_sym_run$fullh(const double d) { ENT _sym.run$fullh = d;}
	void	set_sym_run$flush(const double d) { ENT _sym.run$flush = d;}
	void	set_sym_run$strai(const double d) { ENT _sym.run$strai = d;}
	void	set_sym_run$3kind(const double d) { ENT _sym.run$3kind = d;}
	void	set_sym_run$2pair(const double d) { ENT _sym.run$2pair = d;}
	void	set_sym_run$1pair(const double d) { ENT _sym.run$1pair = d;}
	void	set_sym_run$hcard(const double d) { ENT _sym.run$hcard = d;}
	void	set_sym_run$total(const double d) { ENT _sym.run$total = d;}
	void	set_sym_run$pokervalmax(const double d) { ENT _sym.run$pokervalmax = d;}
	void	set_sym_run$prnuts(const double d) { ENT _sym.run$prnuts = d;}
	void	set_sym_run$prbest(const double d) { ENT _sym.run$prbest = d;}

	// vs$
	void	set_sym_vs$nhands(const double d) { ENT _sym.vs$nhands = d;}
	void	set_sym_vs$nhandshi(const double d) { ENT _sym.vs$nhandshi = d;}
	void	set_sym_vs$nhandsti(const double d) { ENT _sym.vs$nhandsti = d;}
	void	set_sym_vs$nhandslo(const double d) { ENT _sym.vs$nhandslo = d;}
	void	set_sym_vs$prwin(const double d) { ENT _sym.vs$prwin = d;}
	void	set_sym_vs$prtie(const double d) { ENT _sym.vs$prtie = d;}
	void	set_sym_vs$prlos(const double d) { ENT _sym.vs$prlos = d;}
	void	set_sym_vs$prwinhi(const double d) { ENT _sym.vs$prwinhi = d;}
	void	set_sym_vs$prtiehi(const double d) { ENT _sym.vs$prtiehi = d;}
	void	set_sym_vs$prloshi(const double d) { ENT _sym.vs$prloshi = d;}
	void	set_sym_vs$prwinti(const double d) { ENT _sym.vs$prwinti = d;}
	void	set_sym_vs$prtieti(const double d) { ENT _sym.vs$prtieti = d;}
	void	set_sym_vs$prlosti(const double d) { ENT _sym.vs$prlosti = d;}
	void	set_sym_vs$prwinlo(const double d) { ENT _sym.vs$prwinlo = d;}
	void	set_sym_vs$prtielo(const double d) { ENT _sym.vs$prtielo = d;}
	void	set_sym_vs$prloslo(const double d) { ENT _sym.vs$prloslo = d;}

	void	set_sym_vs$nhandshinow(const double d) { ENT _sym.vs$nhandshinow = d;}
	void	set_sym_vs$nhandstinow(const double d) { ENT _sym.vs$nhandstinow = d;}
	void	set_sym_vs$nhandslonow(const double d) { ENT _sym.vs$nhandslonow = d;}
	void	set_sym_vs$prwinhinow(const double d) { ENT _sym.vs$prwinhinow = d;}
	void	set_sym_vs$prtiehinow(const double d) { ENT _sym.vs$prtiehinow = d;}
	void	set_sym_vs$prloshinow(const double d) { ENT _sym.vs$prloshinow = d;}
	void	set_sym_vs$prwintinow(const double d) { ENT _sym.vs$prwintinow = d;}
	void	set_sym_vs$prtietinow(const double d) { ENT _sym.vs$prtietinow = d;}
	void	set_sym_vs$prlostinow(const double d) { ENT _sym.vs$prlostinow = d;}
	void	set_sym_vs$prwinlonow(const double d) { ENT _sym.vs$prwinlonow = d;}
	void	set_sym_vs$prtielonow(const double d) { ENT _sym.vs$prtielonow = d;}
	void	set_sym_vs$prloslonow(const double d) { ENT _sym.vs$prloslonow = d;}
	void	set_sym_playing(const bool b) { ENT _sym.playing = b;}
private:
	void	set_prevaction(const int i) { ENT _sym.prevaction = i; }
	void	set_didchec(const int n, const int i) { ENT assert(n>=0); assert(n<=4); _sym.didchec[n] = i; }
	void	set_didcall(const int n, const int i) { ENT assert(n>=0); assert(n<=4); _sym.didcall[n] = i; }
	void	set_didrais(const int n, const int i) { ENT assert(n>=0); assert(n<=4); _sym.didrais[n] = i; }
	void	set_didswag(const int n, const int i) { ENT assert(n>=0); assert(n<=4); _sym.didswag[n] = i; }
#undef ENT

private:
	void	InitHandranktTableForPrwin();
	bool	IsHigherStraightPossible(HandVal	handval);

	void	set_sym_max_balance_conditionally(const double d) 
	{ 
		// No ENT necessary and allowed, as we do call set_sym_maxbalance_conditionally
		// only inside set_sym_balance, which is already protected by the mutex!
		if (d > _sym.maxbalance) _sym.maxbalance = d;
	}

	void	set_sym_originalbalance_conditionally(const double d) 
	{ 
		// No ENT necessary and allowed, as we do call set_sym_originalbalance_conditionally
		// only inside set_sym_balance, which is already protected by the mutex!
		if (_sym.originalbalance <= 0) _sym.originalbalance = d;
	}
#undef ENT

private:
	// private variables - use public accessors and public mutators to address these
	SSymbols	_sym;
	bool		_user_chair_confirmed;
	
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
	void CalcBetBalanceStack(void);
	void CalcPlayersOpponents(void);
	void CalcChipamtsLimits(void);
	void CalcNumbets(void);
	void CalcFlags(void);
	void CalcAutoplayer(void);
	void CalcPositionsUserchair(void);
	void CalcPositionsNonUserchair(void);
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
	void CalcRunRon(void);

	// For internal use only: raischair_previous_frame
	void set_sym_raischair_previous_frame(const double d) { _sym.raischair_previous_frame = d;}

	HandVal		_phandval[4], _chandval[4];	// for ishandup and ishandupcommon symbol calcs
	time_t		_elapsedhold;				// The time we "sat down"
	time_t		_elapsedhandhold;			// The time since start of last hand

	static int _br_last;

	CCritSec			m_critsec;

} *p_symbols;

#endif /* INC_CSYMBOLS_H */
