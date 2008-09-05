#ifndef INC_CFORMULA_H
#define INC_CFORMULA_H

#include "UPDialog.h"

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
	boost::spirit::tree_parse_info<const char *, int_factory_t>	tpi;
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

extern class CFormula 
{
public:
	// Critical section used in public mutators and private shared variable writes
	static CRITICAL_SECTION	cs_formula;

public:
	// public functions
	CFormula();
	~CFormula();
	void ClearFormula();
	void SetDefaultBot();
	void ReadFormulaFile(CArchive& ar, bool ignoreFirstLine);
	void WriteFormula(CArchive& ar);
	void CreateHandListMatrices();
	bool ParseAllFormula(HWND hwnd);
	void CheckForDefaultFormulaEntries();
	void MarkCacheStale();
	void ParseHandList(const CString &list_text, bool inlist[13][13]);

public:
	// public accessors
	const SFormula * formula() { return &_formula; }
	const CString formula_name() { return _formula_name; }

public:
#define ENT EnterCriticalSection(&cs_formula);
#define LEA LeaveCriticalSection(&cs_formula);
	// public mutators
	void set_formula_name(const CString s) { ENT _formula_name = s; LEA }
	SFormula * set_formula() { return &_formula; }

#undef ENT
#undef LEA

private:
	// private variables - use public accessors and public mutators to address these
	SFormula		_formula;
	CString			_formula_name;

private:
	// private functions and variables - not available via accessors or mutators
	static bool ParseLoop(const CUPDUPDATA* pCUPDUPData);
	const int CardIdentHelper(const char c);

} *p_formula;

void CopyFormula(const SFormula *f, SFormula *t);

struct	sData 
{
	// For parsing progress dialog
	bool		all_parsed;
	HWND		calling_hwnd;
	CFormula	*pParent;
};


//
// Default bot specification follows
//
static const char * defaultCSnotes = "\r\nDefault demo bot for OpenHoldem.\r\n\
Add your formula notes and comments here.";

static const char * defaultCSalli = "\
   0                                 // default is: do not alli\r\n\
   || [ prwin >= 1.00 ]              // dead nuts\r\n\
   || [ br==1 && islistalli ]        // listed opening hand\r\n\
";

static const char * defaultCSswag = "\
   0                                 // default is: no stated wagers";

static const char * defaultCSsrai = "\
   0                                 // default is: no stated raises";

static const char * defaultCSrais = "\
   0                                 // default is: do not rais\r\n\
   || [ prwin >= 1.00 ]              // dead nuts\r\n\
\r\n\
   || [ f$evrais > f$evcall && f$evrais>pot/2 ]  // majority share\r\n\
   || [ br==1 && islistrais ]        // listed opening hand\r\n\
\r\n\
   || [ f0                           // f0 button flag is on\r\n\
        && br==1                     // preflop\r\n\
        && isemptylistrais           // rais list is empty\r\n\
        && handrank2652<=2652        // my hand has\r\n\
           * (1.0 - e**(ln(0.50)/    // 50% chance hand better than all\r\n\
//           nopponentsdealt         // tight (static)\r\n\
             nopponents              // medium (p formula)\r\n\
//           nopponentsplaying       // loose (dynamic)\r\n\
             ))\r\n\
           * betposition/nplayersplaying // dynamic betting position percentage\r\n\
//         * dealposition/nplayersdealt  // static betting position percentage\r\n\
      ]\r\n\
";

static const char * defaultCScall = "\
   0                                 // default is: do not call\r\n\
   || [ prwin >= 1.00 ]              // dead nuts\r\n\
   || [ call  <= 0.00 ]              // zero call amount (check)\r\n\
   || [ f$evcall > pot / nplayersdealt ]     // equal share\r\n\
   || [ br==1 && islistcall ]        // listed opening hand\r\n\
\r\n\
   || [!f0                           // f0 flag button is off\r\n\
        && br==1                     // preflop\r\n\
        && isemptylistcall           // call list is empty\r\n\
        && prwin >= 1/               // fair chances against\r\n\
             nopponentsdealt         // loose (static)\r\n\
//           nopponents              // medium (p formula)\r\n\
//           nopponentsplaying       // tight (dynamic)\r\n\
      ]\r\n\
\r\n\
   || [ f0                           // f0 flag button is on\r\n\
        && br==1                     // preflop\r\n\
        && isemptylistcall           // call list is empty\r\n\
        && handrank2652<=2652        // fair chances against\r\n\
           * (1-e**(ln(0.50)/        // 50% chance hand better than all\r\n\
//           nopponentsdealt         // static and tight\r\n\
             nopponents              // p formula and defense level\r\n\
//           nopponentsplaying       // dynamic and loose\r\n\
             ))\r\n\
      ]\r\n\
";

static const char * defaultCSprefold = "\
   0                                  // default is: do not pre-fold any cards.\r\n";

static const char * defaultCSdelay = "\
   0                                  // default is: do not delay the action.\r\n";

static const char * defaultCSchat = "\
   0                                  // default is: do not chat.\r\n";

static const char * defaultCSP = "// Number of opponents for the prwin simulation.\r\n\r\n\
(nopponentsplaying<=0) ? 1 :\r\n\
\r\n\
nopponentsplaying + defcon*nopponentsfolded\r\n\
";

static const char * defaultCSplay = "//------------------------------------------------------------------------\r\n\
//    DISABLED RULE(S)\r\n\
!0                       ? -1 :  // 0=disabled 1=enabled \r\n\
!f9                      ? -1 :  // enabled when f9 is pressed \r\n\
(elapsed%4)              ? -1 :  // disabled 3 out of 4 seconds \r\n\
\r\n\
\r\n\
//------------------------------------------------------------------------\r\n\
//    LEAVE RULE\r\n\
0                                // 0=off 1=on \r\n\
&& issittingout                  // i am sitting out \r\n\
&& elapsedhand >= 600            // 10 minutes since end of last hand \r\n\
&& elapsedauto >= 600            // 10 minutes since autoplayer has acted \r\n\
\r\n\
                         ? -2 :  // leave the table \r\n\
\r\n\
//------------------------------------------------------------------------\r\n\
//    SITIN RULE\r\n\
0                                // 0=off 1=on \r\n\
&& issittingout                  // i am sitting out \r\n\
&& (br==2 || br==3)              // it is the flop or turn \r\n\
&& nopponentsdealt>=6            // 6 or more opponents \r\n\
&& nchairsdealtright==2          // i am about to be big blind \r\n\
\r\n\
                         ?  1 :  // sitin \r\n\
\r\n\
//------------------------------------------------------------------------\r\n\
//    SITOUT RULE\r\n\
0                                // 0=off 1=on \r\n\
&& issittingin                   // i am sitting in \r\n\
&& (br==2 || br==3)              // it is the flop or turn \r\n\
&& nopponentsdealt<=5            // 5 or less opponents \r\n\
\r\n\
                         ?  0 :  // sitout \r\n\
\r\n\
//------------------------------------------------------------------------\r\n\
-1 // disabled\r\n\
";

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
static const char * defaultCSevrais = "// User defined function f$evrais for the demo bot.\r\n\r\n\
0                                // start with zero\r\n\
\r\n\
// DEBITS HERE\r\n\
\r\n\
- call                           // calling cost\r\n\
- bet                            // raising cost\r\n\
\r\n\
// CREDITS HERE\r\n\
\r\n\
+  (\r\n\
\r\n\
      pot                        // current pot\r\n\
\r\n\
   + (call+bet*2)                // my call my bet and a call (minimum)\r\n\
\r\n\
   + (raisshort-call-bet*2)      // additional callers\r\n\
      * 0.00                     //   0% (tight)\r\n\
//    * 0.50                     //  50% (guess)\r\n\
//    * 1.00                     // 100% (loose)\r\n\
\r\n\
   )\r\n\
\r\n\
   * (prwin+prtie/2)             // adjust for winning chances\r\n\
   * (1+rake)                    // adjust for rake\r\n\
";

static const char * defaultCSevcall = "// User defined function f$evcall for the demo bot.\r\n\r\n\
0                                // start with zero\r\n\
\r\n\
// DEBITS HERE\r\n\
\r\n\
- call                           // calling cost\r\n\
\r\n\
\r\n\
// CREDITS HERE\r\n\
\r\n\
+  (\r\n\
\r\n\
      pot                        // current pot\r\n\
\r\n\
   +  call                       // my call (minimum)\r\n\
\r\n\
   + (callshort-call)            // additional callers\r\n\
      * 0.00                     //   0% (tight)\r\n\
//    * 0.50                     //  50% (guess)\r\n\
//    * 1.00                     // 100% (loose)\r\n\
\r\n\
   )\r\n\
\r\n\
   * (prwin+prtie/2)             // adjust for winning chances\r\n\
   * (1+rake)                    // adjust for rake\r\n\
";

static const double defaultdBankroll = 0.0;

static const double defaultdDefcon = 0.0;

static const double defaultdRake = 0.0;

static const double defaultdNit = 1000.0;

#endif //INC_CFORMULA_H