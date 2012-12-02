#ifndef INC_CSYMBOLS_H
#define INC_CSYMBOLS_H

#include <assert.h>
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineBlinds.h"
#include "CSymbolEngineCards.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineDealerchair.h"
#include "CSymbolEngineHandrank.h"
#include "CSymbolEngineHistory.h"
#include "CSymbolEngineLists.h"
#include "CSymbolEnginePokerval.h"
#include "CSymbolEnginePositions.h"
#include "CSymbolEnginePrwin.h"
#include "CSymbolEngineRaisersCallers.h"
#include "CSymbolEngineRandom.h"
#include "CSymbolEngineTime.h"
#include "CSymbolEngineUserchair.h"
#include "CTableLimits.h"
#include "MagicNumbers.h"


// Symbol structure
struct SSymbols 
{
	//General
	double nchairs;


	//FORMULA FILE
	double nit;

	//CHAIRS
	double raischair;
	// For internal use only: raischair_previous_frame
	double raischair_previous_frame;

	//ROUND&POSITIONS
	double betround;
	double betposition;
	double dealposition;
	double callposition;
	double dealpositionrais;
	double betpositionrais;
	double originaldealposition;

	//PROBABILITIES
	double prwin;
	double prlos;
	double prtie;

	//CHIP AMOUNTS		
	double call;

	//NUMBER OF BETS
	double nbetstocall;
	double nbetstorais;
	double ncallbets;
	double nraisbets;

	//LIST TESTS
	double islist[MAX_HAND_LISTS];

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

	//PLAYERS, OPPONENTS
	double nopponents;
	
	double nplayersseated;
	double nplayersdealt;
	double nplayersplaying;
	
	double nopponentsseated;
	double nopponentsdealt;
	double nopponentsplaying;

	double nopponentschecking;
	double nopponentscalling;
	double nopponentsraising;
	double nopponentsbetting;
	double nopponentsfolded;

	double nplayerscallshort;
	double nchairsdealtright;
	double nchairsdealtleft;
	
	double playersseatedbits;
	
	double playersdealtbits;
	double playersplayingbits;
	

	double opponentsseatedbits;
	double opponentsdealtbits;
	double opponentsplayingbits;

	// CALLBITS, RAISBITS etc.
	// To avoid confusion we use bits 1..4, bit 0 is unused.
	int raisbits[k_number_of_betrounds+1];
	int callbits[k_number_of_betrounds+1];
	int foldbits[k_number_of_betrounds+1];

	//COMMON CARDS
	double ncommoncardspresent;

	//(UN)KNOWN CARDS
	double nouts;
	
	double ncardsbetter;

	

	//NHANDS ???
	double prwinnow;
	double prlosnow;

	//RANK BITS
	double rankbits;
	double rankbitscommon;
	double rankbitsplayer;
	double rankbitspoker;
	double srankbits;
	double srankbitscommon;
	double srankbitsplayer;
	double srankbitspoker;

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

	// All symbol mutators below
	// general
	void	set_sym_nchairs(const double d) { ENT _sym.nchairs = d;}


	
private:
	bool	IsHigherStraightPossible(HandVal	handval);

	
private:
	// private variables - use public accessors and public mutators to address these
	SSymbols	_sym;
	bool		_user_chair_confirmed;
	
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
	void CalcPlayersOpponents(void);
	void CalcChipamtsLimits(void);
	void CalcNumbets(void);
	void CalcAutoplayer(void);
	void CalcPositionsUserchair(void);
	void CalcPositionsNonUserchair(void);
	void CalcPokerValues(void);
	void CalcUnknownCards(void);
	void CalcHandTests(void);
	void CalcPocketTests(void);
	void CalcListTests(void);
	void CalcNhands(void);
	void CalcFlushesStraightsSets(void);
	void CalcRankbits(void);
	void CalcHistory(void);

	// For internal use only: raischair_previous_frame
	void set_sym_raischair_previous_frame(const double d) { _sym.raischair_previous_frame = d;}

	
	static int _br_last;

	CCritSec			m_critsec;
private:
	CSymbolEngineRandom			symbol_engine_random;
	CSymbolEngineTime			symbol_engine_time;
	CSymbolEngineAutoplayer		symbol_engine_autoplayer;
	CSymbolEngineDealerchair	symbol_engine_dealerchair;	// Should be very first, and then handreset-detector
	CSymbolEngineUserchair		symbol_engine_userchair;
	CSymbolHistory				symbol_engine_history;
	CSymbolEngineCards.h		symbol_engine_cards;		// depends on user-chair
	CSymbolEngineLists			symbol_engine_lists;		// depends on user-chair
	CSymbolEngineActiveDealtPlaying		symbol_engine_active_dealt_playing; // depends on user-chair
	CSymbolEnginePokerval		symbol_engine_pokerval;		// depends on user-chair
	CSymbolEngineHandrank		symbol_engine_handrank;		// depends on user-chair and nopponents
	CSymbolEnginePrwin			symbol_engine_prwin;		// depends on user-chair and nopponents
	CSymbolEngineRaisersCallers symbol_engine_raisers_callers;	// depends on user-chair and dealerchair
	CSymbolEngineChipAmounts	symbol_engine_chip_amounts;	// depends on user-chair and nopponents and playersplayingbits and raischair
	CSymbolEnginePositions		symbol_engine_positions;	// depends on user-chair and dealer and playersdealtbits
	CSymbolEngineBlinds			symbol_engine_blinds;		// depends on dealerchair and playersplayingbits and positions
	// prwin at the very end, as the prwin-functions may use all other symbols
} *p_symbols;

#endif /* INC_CSYMBOLS_H */


//!!!
double ncardsknown;
	double ncommoncardsknown;