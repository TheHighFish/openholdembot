#ifndef INC_CFORMULA_H
#define INC_CFORMULA_H

#include "UPDialog.h"
#include "..\CCritSec\CCritSec.h"

typedef	boost::spirit::tree_parse_info<const char *, int_factory_t>		tpi_type;

struct SHandList 
{
	CString		list;
	CString		list_text;
};

struct SFunction 
{
	CString		func;
	CString		func_text;
	bool		dirty;
	tpi_type	tpi;
	bool		fresh;
	double		cache;
};

struct SFormula 
{
	CArray <SFunction, SFunction> mFunction;

	double dBankroll, dDefcon, dRake, dNit;

	CArray <SHandList, SHandList> mHandList;

	bool	inlist[1000][13][13];  // list number, rank0/rank1 - rank0>=rank1 == suited, rank0<rank1 == unsuited
};

class CFormula;
struct	sData 
{
	// For parsing progress dialog
	bool		all_parsed;
	HWND		calling_hwnd;
	CFormula	*pParent;
	bool		disable_msgbox;
};

extern class CFormula 
{
public:
	// public functions
	CFormula();
	~CFormula();
	void ClearFormula();
	void SetDefaultBot();
	void ReadFormulaFile(CArchive& ar, bool ignoreFirstLine, bool disable_msgbox);
	void WriteFormula(CArchive& ar, bool use_new_OHF_style);
	void CreateHandListMatrices();
	bool ParseAllFormula(HWND hwnd, bool disable_msgbox);
	void CheckForDefaultFormulaEntries();
	void MarkCacheStale();
	void ParseHandList(const CString &list_text, bool inlist[13][13]);
	void CopyFormulaFrom(CFormula *f);
	const char *GetFunctionText(const char *name);
 private:
 	bool DoesFormulaAlreadyExist(const CString new_name);
	bool DoesHandlistAlreadyExist(const CString new_name);

public:
	// public accessors
	const SFormula * formula() { return &_formula; }
	const CString formula_name() { return _formula_name; }

public:
#define ENT CSLock lock(m_critsec);
	// public mutators
	void set_formula_name(const CString s) { ENT _formula_name = s; }
	void set_func_func(const int i, const CString s) { ENT if (i>=0 && i<_formula.mFunction.GetSize()) _formula.mFunction[i].func = s; }
	void set_func_cache(const int i, const double d) { ENT if (i>=0 && i<_formula.mFunction.GetSize()) _formula.mFunction[i].cache = d; }
	void set_func_fresh(const int i, const bool b) { ENT if (i>=0 && i<_formula.mFunction.GetSize()) _formula.mFunction[i].fresh = b; }
	void set_func_dirty(const int i, const bool b) { ENT if (i>=0 && i<_formula.mFunction.GetSize()) _formula.mFunction[i].dirty = b; }
	void set_func_text(const int i, const CString s) { ENT if (i>=0 && i<_formula.mFunction.GetSize()) _formula.mFunction[i].func_text = s; }
	void set_func_add(const SFunction f) { ENT _formula.mFunction.Add(f); }
	void set_func_remove(const int i) { ENT if (i>=0 && i<_formula.mFunction.GetSize()) _formula.mFunction.RemoveAt(i,1); }
	void set_list_list(const int i, const CString s) { ENT if (i>=0 && i<_formula.mHandList.GetSize()) _formula.mHandList[i].list = s; }
	void set_list_text(const int i, const CString s) { ENT if (i>=0 && i<_formula.mHandList.GetSize()) _formula.mHandList[i].list_text = s; }
	void set_list_add(const SHandList h) { ENT _formula.mHandList.Add(h); }
	void set_list_remove(const int i) { ENT if (i>=0 && i<_formula.mHandList.GetSize()) _formula.mHandList.RemoveAt(i,1); }
	//void set_list_removeall() { ENT _formula.mHandList.RemoveAll(); }
	void set_bankroll(const double d) { ENT _formula.dBankroll = d; }
	void set_defcon(const double d) { ENT _formula.dDefcon = d; }
	void set_rake(const double d) { ENT _formula.dRake = d; }
	void set_nit(const double d) { ENT _formula.dNit = d; }
	tpi_type * set_tpi(const int i) { if (i>=0 && i<_formula.mFunction.GetSize()) return &_formula.mFunction[i].tpi; else return NULL; }
#undef ENT

private:
	// private variables - use public accessors and public mutators to address these
	SFormula		_formula;
	CString			_formula_name;

private:
	// private functions and variables - not available via accessors or mutators
	const int CardIdentHelper(const char c);
	static bool ParseLoop(const CUPDUPDATA* pCUPDUPData);

	CCritSec		m_critsec;

} *p_formula;


//
// Default bot specification follows
//
static const char * defaultCSnotes = "\
Playbot3. Created by Matrix as an example of a prwin, \r\n\
pot odds bot with the following bells and whistles:\r\n\
Bet and call capping to limit pot chasing in marginal situations\r\n\
EV's modified by proportion of bulls, bears and limpers \r\n\
Autobet and Check Raise logic \r\n\
Options for random trimming of decision levels to reduce predictability \r\n\
Plenty of constants to trim to get the bot to play your way. \r\n\
\r\n\
EDITED 2005-10-30 to correct EV criteria and handrank oddity. \r\n\
\r\n\
EDITED 2009-05-31 by TheHighFish: \r\n\
  * conversion to new ohf-format \r\n\
  * comments for unused standard formulae \r\n\
  * put some symbols into the debug-tab to demonstrate its usage \r\n\
  * removed unused hand-lists";


static const char * defaultCSalli = "\
   0                                     // default is do not alli \r\n\
   || [ prwin >= 1.00 ]                  // dead nuts";


static const char * defaultCSswag = "\
// No stated wagers with a FL-bot.";


static const char * defaultCSsrai = "\
// No stated raises with a FL-bot.";

static const char * defaultCSrais = "\
   0 \r\n\
   ||    [ prwin >= 1.00 ]                  // dead nuts \r\n\
\r\n\
   || br==1 \r\n\
//round 1 logic \r\n\
   ?\r\n\
//bet-capping logic. \r\n\
//We initially used some random modifications to prevent the bot being profiled too accurately \r\n\
//prwin was modified by at max + or - 0.1 \r\n\
//This is of questionable value, and made the bot difficult to tune \r\n\
//e,g.   [(ncurrentbets==1)&&(prwin+((5-(10*randomround))/50)>0.4)] || \r\n\
   ( \r\n\
   [(ncurrentbets==3)&&(prwin>0.7)] ||  //change the >constants to taste \r\n\
   [(ncurrentbets==2)&&(prwin>0.55)] || \r\n\
   [(ncurrentbets==1)&&(prwin>0.4)] || //should be greater than have acted value \r\n\
   [ncurrentbets<1] \r\n\
   ) \r\n\
//end of bet-capping logic \r\n\
\r\n\
   &&\r\n\
\r\n\
//this determines if we want to raise at all pre-flop \r\n\
   ( \r\n\
   ncurrentbets<1 \r\n\
   ? \r\n\
//have not acted yet logic. Separate because prwin will probably increase through folds on first circuit \r\n\
   ( \r\n\
   handrank1000 < (300 - 20*nopponentsplaying)  //trim constants to taste \r\n\
   ) \r\n\
//end of have not acted yet logic \r\n\
   : \r\n\
//have acted logic \r\n\
   ( \r\n\
   prwin > 0.3 // constant should not be greater than the minimum possible prwin value which \r\n\
                  // can match the ncurrentbets==1 capping value \r\n\
   ) \r\n\
//end of have acted logic \r\n\
   ) \r\n\
// end of preflop raise logic \r\n\
   : \r\n\
//post-flop logic \r\n\
   ( \r\n\
//bet-capping logic. \r\n\
   ( \r\n\
   [(ncurrentbets==3)&&(prwin>0.8)] || //change >constants to taste \r\n\
   [(ncurrentbets==2)&&(prwin>0.6)] || \r\n\
   [(ncurrentbets==1)&&(prwin>0.5)] || \r\n\
   [ncurrentbets<1] || [ncurrentbets>=4] \r\n\
   ) \r\n\
//end of bet capping logic \r\n\
   && \r\n\
//check raise code - comment out active line if not wanted \r\n\
   !( \r\n\
   !ncurrentbets && [randomround > 0.75] && [prwin > 0.5] //adjust constants to taste, 0.75 = 1 in 4 \r\n\
   ) \r\n\
//end of check raise code \r\n\
   && \r\n\
//postflop raise logic \r\n\
   ( \r\n\
   0 \r\n\
   || [ f$evrais>f$evcall && f$evrais>(pot/3) ]  // guesswork \r\n\
   || \r\n\
//autobet logic - comment out active line if not wanted \r\n\
   ( \r\n\
   br==2 && didraisround1 && !nbetstocall \r\n\
   && [randomround > 0.5] && [prwin > 0.3] //adjust constants to taste \r\n\
   ) \r\n\
//end of autobet logic \r\n\
   ) \r\n\
//end of postflop raise logic \r\n\
   ) \r\n\
//end of f$rais";

static const char * defaultCScall = "\
   0                                     // default is do not call \r\n\
   || [ prwin >= 1.00 ]                  // dead nuts \r\n\
   || [ call  <= 0.00 ]                  // zero call amount (check) \r\n\
   || br==1 \r\n\
//round 1 logic \r\n\
   ? \r\n\
//bet-capping logic. \r\n\
   ( \r\n\
   [handrank169 < 12] ||  //guard against a few top hands being wasted by prwin on a full table \r\n\
   [(ncurrentbets==3)&&(prwin>0.3)] ||  //change the >constants to taste \r\n\
   [(ncurrentbets==2)&&(prwin>0.25)] || \r\n\
   [(ncurrentbets==1)&&(prwin>0.2)] || //should be greater than have acted value \r\n\
   [ncurrentbets<1] || [ncurrentbets==4] \r\n\
   ) \r\n\
//end of bet-capping logic \r\n\
\r\n\
   && \r\n\
\r\n\
//this determines if we want to call at all pre-flop \r\n\
   ( \r\n\
   ncurrentbets<1 \r\n\
   ? \r\n\
//have not acted yet logic. Separate because prwin will probably increase through folds on first circuit \r\n\
   ( \r\n\
   handrank1000 < (400 - 10*nopponentsplaying)  //trim constants to taste \r\n\
//trim constants to taste \r\n\
   ) \r\n\
//end of have not acted yet logic \r\n\
   : \r\n\
//have acted logic \r\n\
   ( \r\n\
   prwin > 0.15 // constant should not be greater than the minimum possible prwin value which \r\n\
                  // can match the ncurrentbets==1 capping value \r\n\
   ) \r\n\
//end of have acted logic \r\n\
   ) \r\n\
// end of preflop call logic \r\n\
   : \r\n\
//post-flop logic \r\n\
   ( \r\n\
//bet-capping logic. \r\n\
   ( \r\n\
   [(ncurrentbets==3)&&(prwin>0.5)] || //change >constants to taste \r\n\
   [(ncurrentbets==2)&&(prwin>0.4)] || \r\n\
   [(ncurrentbets==1)&&(prwin>0.3)] || \r\n\
   [ncurrentbets<1] || [ncurrentbets>=4] \r\n\
   ) \r\n\
//end of bet capping logic \r\n\
   && \r\n\
//postflop call logic \r\n\
   ( \r\n\
   0 \r\n\
   || [ f$evcall>pot/(nplayersdealt+2) ]  // guesswork \r\n\
   ) \r\n\
//end of postflop calle logic \r\n\
   ) \r\n\
//end of f$call";

static const char * defaultCSprefold = "\
// Don't prefold anything \r\n\
0";

static const char * defaultCSrebuy = "\
// <=0 means: don't rebuy.\r\n\
//  >0 means: target-amount.\r\n\
// Default is: no rebuy.\r\n\
0";

static const char * defaultCSdelay = "\
// No delay to act. \r\n\
0";

static const char * defaultCSchat = "\
// No chat. \r\n\
0";

static const char * defaultCSP = "// Number of opponents for the prwin simulation.\r\n\r\n\
(nopponentsplaying==0) ? 0 : \r\n\
nopponentsplaying + defcon*nopponentsfolded";

static const char * defaultCSplay = "\
//------------------------------------------------------------------------ \r\n\
//    DISABLED RULE(S) \r\n\
!1                       ? -1 :  // 0=disabled 1=enabled \r\n\
!f9                      ? -1 :  // enabled when f9 is pressed \r\n\
//(elapsed%4)              ? -1 :  // disabled 3 out of 4 seconds \r\n\
\r\n\
\r\n\
//------------------------------------------------------------------------ \r\n\
//    LEAVE RULE \r\n\
0                                // 0=off 1=on \r\n\
&& issittingout                  // i am sitting out \r\n\
&& elapsedhand >= 600            // 10 minutes since end of last hand \r\n\
&& elapsedauto >= 600            // 10 minutes since autoplayer has acted \r\n\
\r\n\
                         ? -2 :  // leave the table \r\n\
\r\n\
//------------------------------------------------------------------------ \r\n\
//    SITIN RULE \r\n\
\r\n\
0                                // 0=off 1=on \r\n\
&& issittingout                  // i am sitting out \r\n\
//&& (br==2 || br==3)              // it is the flop or turn \r\n\
&& (nopponentsdealt >= 2)             // 6 or more opponents \r\n\
//&& nchairsdealtright==2          // i am about to be big blind \r\n\
\r\n\
                         ?  1 :  // sitin \r\n\
\r\n\
//------------------------------------------------------------------------ \r\n\
//    SITOUT RULE \r\n\
1                                // 0=off 1=on \r\n\
&& issittingin                   // i am sitting in \r\n\
&& (br==2 || br==3)              // it is the flop or turn \r\n\
&& (nopponentsdealt <= 1)             // 5 or less opponents \r\n\
\r\n\
                         ?  0 :  // sitout \r\n\
\r\n\
//------------------------------------------------------------------------ \r\n\
-1 // disabled"; 

static const char * defaultCStest = "";

static const char * defaultCSdebug = "                 = elapsed\r\n\
\r\n\
                 = sblind\r\n\
                 = bblind\r\n\
                 = bet\r\n\
\r\n\
                 = call\r\n\
                 = currentbet\r\n\
                 = balance\r\n\
\r\n\
                 = f$evcall\r\n\
                 = f$evrais\r\n\
\r\n\
                 = potplayer\r\n\
                 = potcommon\r\n\
                 = pot\r\n\
                 = rake\r\n\
\r\n\
                 = chair\r\n\
                 = handrank2652\r\n\
                 = pokerval\r\n\
                 = prwin\r\n\
                 = prlos\r\n\
                 = prtie\r\n\
                 = nopponents\r\n\
";

//
// User defined functions: f$evrais and f$evcall
//
static const char * defaultCSevrais = "\
0                                        // start with zero \r\n\
\r\n\
// DEBITS HERE \r\n\
\r\n\
- call                                   // calling cost \r\n\
- bet                                    // raising cost \r\n\
\r\n\
// CREDITS HERE \r\n\
\r\n\
+  ( \r\n\
\r\n\
      pot                                // current pot \r\n\
\r\n\
   + (call+bet*2)                        // my call my bet and a call (minimum) \r\n\
\r\n\
   + (raisshort-call-bet*2)              // additional callers \r\n\
      * 0.00                             //   0% (tight) \r\n\
//    * 0.50                             //  50% (guess) \r\n\
//    * 1.00                             // 100% (loose) \r\n\
\r\n\
   ) \r\n\
\r\n\
   * (prwin+prtie/2)                     // adjust for winning chances \r\n\
   * (1+rake)                            // adjust for rake \r\n\
   * (1-((nopponentsraising/nopponentsplaying)/3)) //adjust constant for aggressors \r\n\
   * (1+((nopponentschecking/nopponentsplaying)/5)) //adjust constant for lurkers \r\n\
   * (1-((nopponentscalling/nopponentsplaying)/10)) //adjust constant for limpers";
									  
static const char * defaultCSevcall = "\
0                                        // start with zero \r\n\
\r\n\
// DEBITS HERE \r\n\
\r\n\
- call                                   // calling cost \r\n\
\r\n\
\r\n\
// CREDITS HERE \r\n\
\r\n\
+  ( \r\n\
\r\n\
      pot                                // current pot \r\n\
\r\n\
   +  call                               // my call (minimum) \r\n\
\r\n\
   + (callshort-call)                    // additional callers \r\n\
      //* 0.00                             //   0% (tight) \r\n\
      * 0.50                             //  50% (guess) \r\n\
//    * 1.00                             // 100% (loose) \r\n\
\r\n\
   ) \r\n\
\r\n\
   * (prwin+prtie/2)                     // adjust for winning chances \r\n\
   * (1+rake)                            // adjust for rake \r\n\
   * (1-((nopponentsraising/nopponentsplaying)/4)) //adjust constant for aggressors \r\n\
   * (1+((nopponentschecking/nopponentsplaying)/10)) //adjust constant for lurkers \r\n\
   * (1-((nopponentscalling/nopponentsplaying)/20)) //adjust constant for waiters ";

static const double defaultdBankroll = 0.0;

static const double defaultdDefcon = 0.0;

static const double defaultdRake = 0.0;

static const double defaultdNit = 1000.0;

#endif //INC_CFORMULA_H