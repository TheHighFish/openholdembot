//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Warning about unknown (erroneaous) and outdated symbols
//   Is not able to care about wrong function names; this has to be handled
//   by CFormula::WarnAboutOutdatedConcepts().
//
//******************************************************************************

#include "stdafx.h"
#include "UnknownSymbols.h"

#include "OH_MessageBox.h"

char *title_outdated_symbol = "ERROR: outdated symbol";

char *title_unknown_symbol = "ERROR: unknown symbol";

char *outdated_symbols_br_ncps_nflopc_chair =
  "The following symbols got removed from the code-base:\n"
  "  * br (betround)\n"
  "  * ncps (nclockspersecond)\n"
  "  * nflopc (ncommoncardsknown)\n"
  "  * chair (userchair)\n"
  "  * oppdealt (nopponentsdealt)\n"
  "because duplicate functionality is bad software-engineering.\n"
  "Please use their verbose equivalents.\n";

char *outdated_symbols_isbring_ismanual =
  "The following symbols got removed from the code-base:\n"
  "  * isbring\n"
  "  * ismanual\n"
  "because there is no need to deal with such stuff at the formula level.\n";

char *outdated_symbols_bankroll_rake_defcon =
  "The following symbols got removed from the code-base:\n"
  "  * bankroll\n"
  "  * rake\n"
  "  * defcon\n"
  "because \"defcon\" is a WinHoldem mis-concept,\n"
  "because a sufficient \"bankroll\" shouldn't influence your decision,\n"
  "because \"rake\" is better handled by a formula, if you need to consider it.\n";

char *outdated_symbol_friends =
  "The friend-symbols got completely removed from the code-base,\n"
  "because OpenHoldem does not support collusion, and never will do.\n"
  "\n"
  "For some time we kept these symbols for backward-compatibility,\n"
  "with hero being his one and only \"friend\",\n"
  "but now it is time to get rid of that WinHoldem-shit.\n";

char *outdated_symbol_NIT =
  "OpenHoldem 4.0.0 replaced \"NIT\" by\n"
  "\"f$prwin_number_of_iterations\".\n"
  "\n"
  "This requires one little change to your formula.\n";

char *outdated_symbol_handrank =
  "The symbol \"handrank\" got removed from the code-base,\n"
  "because we already have \"handrank169\", \"handrank1000\"\n"
  "\"handrank1326\", \"handrank2625\" and \"handrankp\"\n"
  "\n"
  "The user can clearly specify what he wants,\n" 
  "but a symbol \"handrank\" that takes one of the values above\n"
  "(depending on option settings) just calls for troubles.\n";

char *outdated_symbol_clocks =
  "The following symbols got removed from the code-base:\n"
  "  * clocks\n"
  "  * nclockspersecond\n"
  "  * ron$clocks\n"
  "  * run$clocks\n"
  "because nobody used them.\n";

char *outdated_symbols_ptt =
  "The \"ptt_\" symbols got removed from the code-base,\n"
  "to simplify OpenHoldem and its supporting libraries (OpenPPL)\n"
  "\n"
  "OpenHoldem does now only support \"pt_\" symbols\n"
  "and fetches cash-game or tournament-stats automatically.\n";

char *outdated_symbols_lists =
  "The following list-symbols got removed from the code-base\n"
  "because they were mis-conceptions and nobody used them:\n"
  "  * islistcall\n"
  "  * islistrais\n"
  "  * islistalli\n"
  "  * isemptylistcall\n"
  "  * isemptylistrais\n"
  "  * isemptylistalli\n"
  "  * nlistmax\n"
  "  * nlistmin\n";

char *outdated_symbols_islist_symbols =
  "The restriction to 1000 handlists got removed in OH 5.0\n"
  "and you can name your lists any way you want:\n"
  "\n"
  "##list0fTr4sh1W4ntToCa11##\n"
  "...\n"
  "WHEN list0fTr4sh1W4ntToCa11 AND ... CALL FORCE\n"
  "\n"
  "As a consequence the islistNNN-symbols got removed.\n"
  "We now just use the name of the list.";

char *outdated_symbols_tablemap =
  "The following tablemap-symbols got removed from the code-base:\n"
  "  * swagdelay\n"
  "  * allidelay\n"
  "  * swagtextmethod\n"
  "  * potmethod\n"
  "  * activemethod\n"
  "because there is no need to use them at the formula-level.\n"
  "OpenHoldem 4.0.0 cares about betsize-adaption automatically.\n";

char*outdated_symbols_handstrength =
   "The handstrength-symbols (\"mh_...\") got removed from the code-base\n"
   "and moved to an external library (\"mh_str_Handstrength_Library.ohf\")\n"
   "because technical symbols should be provided by OpenHoldem\n"
   "and poker-logical symbols should be provided by external libraries.\n";

char *outdated_symbols_runron = 
  "The run$/ron$-symbols got removed from the code-base,\n"
  "  * because they looked like a WinHoldem mis-conception\n"
  "  * because nobody used them\n"
  "  * because some of the results were wrong\n"
  "  * because they were unfixable (1000s of undocumented numbers)\n"
  "In case you really need them and can solve the problems above\n"
  "please get in contact with the development-team.\n";

char *outdated_symbols_randomround =
  "The symbols randomround1..randomround4\n"
  "got removed from the code-base.\n"
  "Please use randomround, randomhand,\n"
  "randomheartbeat or random instead.\n";

char *outdated_symbols_callshort_raisshort =
  "The symbols \"callshort\" and \"raisshort\" got removed from the code-base\n"
  "because they got designed for Fixed-Limit no-foldem Hold'em only.\n"
  "Better use a function to estimate future pot-sizes.\n";

char *outdated_various_symbols =
  "The following list-symbols got removed from the code-base\n"
  "because they were not needed at all:\n"
  " *isfiveofakind\n"
  " *bankroll\n"
  " *rake\n"
  " *defcon\n" 
  " *isaggmode\n" 
  " *isdefmode\n"
  " *nopponentsmax\n"
  " *elapsed1970\n";

char *outdated_symbol_ncommoncardspresent =
  "The symbol \"ncommoncardspresent\" got removed from the code base\n"
  "because it was never implemented correctly,\n"
  "but always had the same value as \"ncommoncardsknown\".\n" 
  "Furthermore its value would only differ at some casinos and at showdown,\n"
  "but this point of time is pretty meaningless for both OH-script and OpenPPL,\n"
  "whereas DLLers still have access to all info.\n";

char *outdated_symbol_originaldealposition =
  "The symbol \"originaldealposition\" got removed from the code base\n"
  "because there was no longer any need for it\n"
  "after making dealposition persistent.\n";

char *outdated_symbol_ac_aggressor =
  "The symbol \"originaldealposition\" got removed from the code base\n"
  "because it provided the same functionality as \"raischair\".\n";

char *outdated_symbols_pokertracker_tournament =
  "The PokerTracker tournament symbols \"ptt_\" got removed\n"
  "because we changed the \"pt_\"-symbols so that they\n"
  "automagically work for both ring-games and tournaments\n"
  "to simplify user-code and supporting libraries (OpenPPL).";

bool IsOutdatedSymbol(CString symbol) {
  // This function gets called for every symbol lookup.
  // So we optimize it a bit.
  // Fast switch, depending on first character
  // No check for length > 0 needed, as checking \0 won't harm.
  char first_character = symbol[0];
  switch (first_character) {
    case 'a':
      if (symbol == "ac_aggressor ") {
	      OH_MessageBox_Formula_Error(outdated_symbol_ac_aggressor, title_outdated_symbol);
	    return true;
      }
      if ((symbol == "allidelay") || (symbol == "activemethod")) {
	      OH_MessageBox_Formula_Error(outdated_symbols_tablemap, title_outdated_symbol);
	    return true;
      }
    case 'b':
      if (symbol == "br") {
	    OH_MessageBox_Formula_Error(outdated_symbols_br_ncps_nflopc_chair, title_outdated_symbol);
	    return true;
      }
      if (symbol == "bankroll") {
	      OH_MessageBox_Formula_Error(outdated_symbols_bankroll_rake_defcon, title_outdated_symbol);
	      return true;
      } 
    case 'c': 
      if (symbol == "chair") {
	      OH_MessageBox_Formula_Error(outdated_symbols_br_ncps_nflopc_chair, title_outdated_symbol);
	      return true;
      }
      if (symbol == "callshort") {
	      OH_MessageBox_Formula_Error(outdated_symbols_callshort_raisshort, title_outdated_symbol);
	      return true;
      }
      if (symbol == "clocks") {
	      OH_MessageBox_Formula_Error(outdated_symbol_clocks, title_outdated_symbol);
	      return true;
      }
    case 'd':
      if (symbol == "defcon") {
	      OH_MessageBox_Formula_Error(outdated_symbols_bankroll_rake_defcon, title_outdated_symbol);
	      return true;
      } 
    case 'e':
      if (symbol == "elapsed1970") {
	      OH_MessageBox_Formula_Error(outdated_various_symbols, title_outdated_symbol);
	      return true;
      }
    case 'f':
      if (symbol.Left(7) == "friends") {
	      OH_MessageBox_Formula_Error(outdated_symbol_friends, title_outdated_symbol);
	      return true;
      }
    case 'h':
      if (symbol == "handrank") {
	      OH_MessageBox_Formula_Error(outdated_symbol_handrank, title_outdated_symbol);
	      return true;
      }
    case 'i': 
      if ((symbol == "islistcall") || (symbol == "islistrais") 
	        || (symbol == "islistalli") || (symbol == "isemptylistcall") 
	        || (symbol == "isemptylistrais") || (symbol == "isemptylistalli")) {
	      OH_MessageBox_Formula_Error(outdated_symbols_lists, title_outdated_symbol);
	      return true;
      }
      if (symbol.Left(6) == "islist") {
        OH_MessageBox_Formula_Error(outdated_symbols_islist_symbols, title_outdated_symbol);
        return true;
      }
      if ((symbol == "isbring") || (symbol == "ismanual")) {
	      OH_MessageBox_Formula_Error(outdated_symbols_isbring_ismanual, title_outdated_symbol);
	      return true;
      }
      if ((symbol == "isfiveofakind") || (symbol == "isaggmode") 
	       || (symbol == "isdefmode")) {
	      OH_MessageBox_Formula_Error(outdated_various_symbols, title_outdated_symbol);
	      return true;
      }
    case 'm':
      if (symbol.Left(3) == "mh_") {
	      OH_MessageBox_Formula_Error(outdated_symbols_handstrength, title_outdated_symbol);
	      return true;
      }
    case 'n':
      if ((symbol == "nlistmax") || (symbol == "nlistmin")) {
	      OH_MessageBox_Formula_Error(outdated_symbols_lists, title_outdated_symbol);
	      return true;
      }
      if (symbol == "nopponentsmax") {
	      OH_MessageBox_Formula_Error(outdated_various_symbols, title_outdated_symbol);
	      return true;
      }
      if (symbol == "ncommoncardspresent") {
	      OH_MessageBox_Formula_Error(outdated_symbol_ncommoncardspresent, title_outdated_symbol);
	      return true;
      }
      if (symbol == "nclockspersecond") {
	      OH_MessageBox_Formula_Error(outdated_symbol_clocks, title_outdated_symbol);
	      return true;
      }
      if ((symbol == "ncps") || (symbol == "nflopc")) {
	      OH_MessageBox_Formula_Error(outdated_symbols_br_ncps_nflopc_chair, title_outdated_symbol);
	      return true;
      }
      if (symbol.Left(8) == "nfriends") {
	      OH_MessageBox_Formula_Error(outdated_symbol_friends, title_outdated_symbol);
	      return true;
      }
    case 'o':
      if (symbol == "originaldealposition") {
	      OH_MessageBox_Formula_Error(outdated_symbol_originaldealposition, title_outdated_symbol);
	      return true;
      }
      if (symbol == "oppdealt") {
	      OH_MessageBox_Formula_Error(outdated_symbols_br_ncps_nflopc_chair, title_outdated_symbol);
	      return true;
      }
    case 'p':
      if (symbol.Left(3) == "ptt") {
	      OH_MessageBox_Formula_Error(outdated_symbols_ptt, title_outdated_symbol);
	      return true;
      }
      if (symbol == "potdelay") {
	      OH_MessageBox_Formula_Error(outdated_symbols_tablemap, title_outdated_symbol);
	      return true;
      }
    case 'r':
      if (symbol == "raisshort") {
	      OH_MessageBox_Formula_Error(outdated_symbols_callshort_raisshort, title_outdated_symbol);
	      return true;
      }
      if (// Attention: "randomround" is valid
	      // Only randomround1..rnadomround4 are outdated
	      (symbol.GetLength() == 12) && (symbol.Left(11) == "randomround")) {
	      OH_MessageBox_Formula_Error(outdated_symbols_randomround, title_outdated_symbol);
	      return true;
      }
      if ((symbol.Left(4) == "run$") || (symbol.Left(4) == "ron$")) {
	      OH_MessageBox_Formula_Error(outdated_symbols_runron, title_outdated_symbol);
	      return true;
      }
      if (symbol == "rake") {
	      OH_MessageBox_Formula_Error(outdated_symbols_bankroll_rake_defcon, title_outdated_symbol);
	      return true;
      } 
    case 's': 
      if ((symbol == "swagdelay") || (symbol == "swagtextmethod")) {
	      OH_MessageBox_Formula_Error(outdated_symbols_tablemap, title_outdated_symbol);
	      return true;
      }
    case 'N':
      if (symbol == "NIT") {
	      OH_MessageBox_Formula_Error(outdated_symbol_NIT, title_outdated_symbol);
	      return true;
      }
    default: 
	  // Good symbol for sure
	  return false;
  }
}

void WarnAboutUnknownSymbol(CString symbol) {
  // Unknown symbol -- general warning
  CString error_message = CString("Unknown symbol in CGrammar::EvaluateSymbol(): \"")
  + symbol + CString("\"\nThis is most probably a typo in the symbols name.\n")
  + CString("Please check your formula and your DLL or Perl-script.");
  OH_MessageBox_Formula_Error(error_message, title_unknown_symbol);
}