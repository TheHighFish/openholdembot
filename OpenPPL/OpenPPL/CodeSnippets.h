//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*****************************************************************************
//
// Purpose:
//
//*****************************************************************************


#ifndef INC_CODESNIPPETS_H
#define INC_CODESNIPPETS_H

static const char *k_code_snippet_license =
	"//************************************************************************\n"
	"//                                                                        \n"
	"// Converted with OpenPPL 4.4.0                                           \n"
	"//                                                                        \n"
	"// OpenPPL is part of the OpenHoldem project                              \n"
	"//   Download page:         http://code.google.com/p/openholdembot/       \n"
	"//   Forums:                http://www.maxinmontreal.com/forums/index.php \n"
	"//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html          \n"
	"//                                                                        \n" 
	"// The original PPL (Poker Programming Language) got invented by          \n"
	"//   Shanky Bonusbots:      http://www.bonusbots.com/supportforum/YaBB.pl \n"
	"//                                                                        \n"
	"// Required OpenHoldem version: 4.4.0 or later                            \n"
	"//                                                                        \n"
	"// Contributors:                                                          \n"
	"//   Bob3456                                                              \n"
	"//   CoolBotMe123                                                         \n"
	"//   CrumbleSunday                                                        \n"
	"//   Dr.Kaktuz                                                            \n"
	"//   Efu                                                                  \n"
	"//   Fn101                                                                \n"
	"//   Gecko                                                                \n"
	"//   JConner                                                              \n"
	"//   Jokedoke                                                             \n"
	"//   KaeKruse                                                             \n"
	"//   Kyo                                                                  \n"
	"//   Lavezzi                                                              \n"
	"//   MerzkiyTipon                                                         \n"
	"//   Morello                                                              \n"
	"//   MrRuder                                                              \n"
	"//   Nik0                                                                 \n"
	"//   NPNG                                                                 \n"
	"//   Poker_Dog                                                            \n"
	"//   Postenjak                                                            \n"
	"//   Rambam                                                               \n"
	"//   Relhby                                                               \n"
	"//   Satanikut                                                            \n"
	"//   Stangelnator                                                         \n"
	"//   SteveG1963                                                           \n"
	"//   Strikles                                                             \n"
	"//   Superblak                                                            \n"
	"//   TheHighFish                                                          \n"
	"//   ThemAndMe                                                            \n"
	"//   Want2Learn                                                           \n"
	"//   Zolp                                                                 \n"
	"//                                                                        \n"
	"//************************************************************************\n"
	"\n"
	"\n";

static const char *k_code_snippet_options =
	"//***********************************************************\n"
	"//                                                           \n"
	"//  Option settings                                          \n"
	"//  ===============                                          \n"
	"//                                                           \n"
	"//  To be ignored,                                           \n"
	"//  as nearly all of them are for the built-in default bot,  \n"
	"//  which is not supported.                                  \n"
	"//                                                           \n"
	"//***********************************************************\n"
	"\n"
	"\n";

static const char *k_code_snippet_list_section =
	"//***********************************************************\n"
	"//                                                           \n"
	"//  Hand lists                                               \n"
	"//  ==========                                               \n"
	"//                                                           \n"
	"//  Translated code for user-defined hand-lists.             \n"
	"//  Note: hand-lists are not part of standard PPL.           \n"
	"//  Please refer to the manual for details.                  \n"
	"//                                                           \n"
	"//  OpenPPL lists look like that                             \n"
	"//                                                           \n"
	"//  CUSTOM                                                   \n"
	"//                                                           \n"
	"//  LISTS                                                    \n"
	"//                                                           \n"
	"//  NEW LIST 007                                             \n"
	"//      // Standard open-raising range for CutOff            \n"
	"//      AA KK QQ                                             \n"
	"//      AKs AQs                                              \n"
	"//      AKo 72o                                              \n"
	"//  END LIST                                                 \n"
	"//                                                           \n"
	"//***********************************************************\n"
	"\n"
	"\n";

static const char *k_code_snippet_user_defined_functions =
	"//***********************************************************\n"
	"//                                                           \n"
	"//  User defined functions                                   \n"
	"//  ======================                                   \n"
	"//                                                           \n"
	"//  Translated code for user-defined functions.              \n"
	"//  Note: UDFs are not part of standard PPL.                 \n"
	"//  Please refer to the manual for details.                  \n"
	"//                                                           \n"
	"//  CUSTOM                                                   \n"
	"//                                                           \n"
	"//  SYMBOLS                                                  \n"
	"//                                                           \n"
	"//  NEW SYMBOL ThisIsAnExample                               \n"
	"//      WHEN (OpponentsLeft > 3) RETURN 5 FORCE              \n"
	"//      WHEN (Potsize >= 20) RETURN (Pi * R * R) FORCE       \n"
	"//      WHEN Others RETURN 42 FORCE                          \n"
	"//  END SYMBOL                                               \n"
	"//                                                           \n"
	"//***********************************************************\n"
	"\n"
	"\n";

static const char *k_code_snippet_start_of_code_sections =
	"//***********************************************************\n"
	"//                                                           \n"
	"//  Main Code Sections                                       \n"
	"//  ==================                                       \n"
	"//                                                           \n"
	"//  Preflop, Flop, Turn and River                            \n"
	"//  are mandatory for every OpenPPL-profile.                 \n"
	"//                                                           \n"
	"//***********************************************************\n"
	"\n"
	"\n";

static const char *k_code_snippet_reset_user_variables =
	"//************************************************************************\n"
	"//\n"
	"// Resetting all User-Defined Variables.\n"
	"//\n"
	"// To be called at the very beginning of a hand.\n"
	"//\n"
	"//************************************************************************\n"
	"\n"
	"\n";

static const char *k_code_snippet_prefold_function =
	"//************************************************************************\n"
	"//\n"
	"// Prefold-function.\n"
	"//\n"
	"// Only generated when a prefold-section is defined.\n"
	"// Translates a Fold/Do_Not_PreFold-decision to a true/false-value\n"
	"//\n"
	"//************************************************************************\n"
	"\n"
	"\n"
	"##f$prefold##\n"
	"[   (betround == f$OpenPPL_ConstBetRoundPreflop)\n"
	" && (f$OpenPPL_BotsActionsOnThisRoundIncludingChecks == 0)\n"
	" && (f$OpenPPL_prefold == f$OpenPPL_Fold)]\n"
	"\n"
	"\n";


#endif INC_CODESNIPPETS_H