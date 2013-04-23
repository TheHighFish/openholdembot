#include "stdafx.h"
#include "UnknownSymbols.h"

#include "OH_MessageBox.h"

char *title_outdated_symbol = "ERROR: outdated symbol";

char *title_unknown_symbol = "ERROR: unknown symbol";


char *outdated_symbols_br_ncps_nflopc =
	"The following symbols got removed from the code-base:\n"
	"  * br (betround)\n"
	"  * ncps (nclockspersecond)\n"
	"  * nflopc (ncommoncardsknown)\n"
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
	"OpenHoldem 2.2.0 replaced \"NIT\" by\n"
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
	"(depending on option settings) just calls for troubles.";

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
	"OpenHoldem does now only support \"pt\" symbols\n"
	"and fetches cash-game or tournament-stats automatically,\n"
	"provided your c0istournament-symbol returns the correct value.";

char *outdated_symbols_lists =
	"The \"list\" symbols got removed from the code-base, e.g.\n"
	"  * islistalli, ...\n"
	"  * isemptylistcall, ...\n"
	"  * nlistmin, nlistmax, ...\n"
	"Please refer to the release-notes for more info.";

char *outdated_symbols_tablemap =
	"The following tablemap-symbols got removed from the code-base:\n"
	"  * swagdelay\n"
	"  * allidelay\n"
	"  * swagtextmethod\n"
	"  * potmethod\n"
	"  * activemethod\n"
	"because there is no need to use them at the formula-level.";

char*outdated_symbols_handstrength =
   "The handstrength-symbols (\"mh_...\") got removed from the code-base\n"
   "and moved to an external library (\"mh_str_Handstrength_Library.ohf\")\n"
   "because technical symbols should be provided by OpenHoldem\n"
   "and poker-logical symbols should be provided by external libraries.\n";
	
void WarnAboutUnknownOrOutdatedSymbol(CString symbol)
{
	if ((symbol == "br") || (symbol == "ncps") || (symbol == "nflopc"))
	{
		OH_MessageBox(outdated_symbols_br_ncps_nflopc, title_outdated_symbol, 0);
	}
	else if ((symbol == "isbring") || (symbol == "ismanual"))
	{
		OH_MessageBox(outdated_symbols_isbring_ismanual, title_outdated_symbol, 0);
	}
	else if ((symbol == "bankroll") || (symbol == "rake") || (symbol == "defcon"))
	{
		OH_MessageBox(outdated_symbols_bankroll_rake_defcon, title_outdated_symbol, 0);
	}
	else if ((symbol.Left(7) == "friends") || (symbol.Left(8) == "nfriends"))
	{
		OH_MessageBox(outdated_symbol_friends, title_outdated_symbol, 0);
	}
	else if (symbol == "NIT")
	{
		OH_MessageBox(outdated_symbol_NIT, title_outdated_symbol, 0);
	}
	else if (symbol == "handrank")
	{
		OH_MessageBox(outdated_symbol_handrank, title_outdated_symbol, 0);
	}
	else if ((symbol == "clocks") || (symbol == "nclockspersecond")
		|| (symbol == "ron$clocks") || (symbol == "run$clocks"))
	{
		OH_MessageBox(outdated_symbol_clocks, title_outdated_symbol, 0);
	}
	else if (symbol.Left(4) == "ptt_")
	{
		OH_MessageBox(outdated_symbols_ptt, title_outdated_symbol, 0);
	}
	else if ((symbol.Left(6) == "islist") || (symbol.Left(6) == "isemptylist")
		|| (symbol.Left(5) == "nlist"))
	{
		OH_MessageBox(outdated_symbols_lists, title_outdated_symbol, 0);
	}
	else if ((symbol.Left(9) == "swagdelay") || (symbol.Left(9) == "allidelay")
		|| (symbol.Left(8) == "potdelay") || (symbol.Left(14) == "swagtextmethod")
		|| (symbol.Left(12) == "activemethod"))
	{
		OH_MessageBox(outdated_symbols_tablemap, title_outdated_symbol, 0);
	}
	else if (symbol.Left(3) == "mh_")
	{
		OH_MessageBox(outdated_symbols_handstrength, title_outdated_symbol, 0);
	}
	else
	{
		// Unknown symbol -- general warning
		CString error_message = CString("Unknown symbol in CGrammar::EvaluateSymbol(): \"")
			+ symbol + CString("\"\nThat is most probably a typo in the symbols name.\n")
			+ CString("Please check your formula and your DLL or Perl-script.");
		OH_MessageBox(error_message, title_unknown_symbol, 0);
	}

}