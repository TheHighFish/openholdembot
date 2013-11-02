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

#include <stdafx.h>
#include "CProblemSolver.h"

#include "CAutoConnector.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEnginePokerval.h"
#include "CSymbolEngineTime.h"
#include "CSymbolEngineUserchair.h"
#include "CTableMapLoader.h"
#include "OH_MessageBox.h"

CProblemSolver::CProblemSolver()
{}

CProblemSolver::~CProblemSolver()
{}

const char *k_title_string = "Problem Solver";

bool CProblemSolver::NoTableMapsInScraperFolder()
{
	return (p_tablemap_loader->NumberOfTableMapsLoaded() == 0);
}

bool CProblemSolver::NotConnected()
{
	return (p_autoconnector->IsConnected() == false);
}

bool CProblemSolver::UserChairUnknown()
{
	return (p_symbol_engine_userchair->userchair_confirmed() == false);
}

bool CProblemSolver::NoOpponents()
{
	return (p_symbol_engine_active_dealt_playing->nopponentsplaying() == 0);
}

bool CProblemSolver::AutoPlayerDidActAtLeastOnce()
{
	// We compare the time since connection to the time since last action.
	return (p_symbol_engine_time->elapsed() != p_symbol_engine_time->elapsedauto());
}

bool CProblemSolver::NoCardsVisible()
{
	return (p_symbol_engine_pokerval->rankhiplayer() < 2);
}

bool CProblemSolver::NotEnoughButtonsVisible()
{
	// We need at least 2 visible buttons to play
	return (p_symbol_engine_pokerval->npcbits() < 2);
}


void CProblemSolver::TryToDetectBeginnersProblems()
{
	if (NoTableMapsInScraperFolder())
	{
		OH_MessageBox_Interactive(
			"BAD NEWS:\n"
			"  * No tablemaps in scraper folder.\n"
			"    Without tablemaps you can't connect to a table.",
			k_title_string, 0);

	}
	else if (NotConnected())
	{
		OH_MessageBox_Interactive(
			"GOOD NEWS:\n"
			"  * At least one tablemap in scraper folder.\n"
			"\n"
			"BAD NEWS:\n"
			"  * You are not connected to a table.\n"
			"    To connect to a table three conditions must be met:\n"
			"      * You need a tablemap for that casino and game-type.\n"
			"      * The tablesize must be right.\n"
			"      * The titlestring must match.\n"
			"    If it does not connect, then you have to fix your tablemap.",
			k_title_string, 0);
	}
	else if (UserChairUnknown())
	{
		OH_MessageBox_Interactive(
			"GOOD NEWS:\n"
			"  * At least one tablemap in scraper folder.\n"
			"  * You are connected to a table.\n"
			"\n"
			"BAD NEWS:\n"
			"  * you are not seated or userchair not recognized.\n"
			"    To detect the userchair two conditions must be met:\n"
			"      * Cards visible.\n"
			"      * Buttons visible at the same time.\n"
			"    Please revisit your tablemap.",
			k_title_string, 0);
	}
	else if (NoOpponents())
	{
		OH_MessageBox_Interactive(
			"GOOD NEWS:\n"
			"  * At least one tablemap in scraper folder.\n."
			"  * You are connected to a table.\n"
			"  * Your chair got recognized.\n"
			"\n"
			"BAD NEWS:\n"
			"  * There seem to be no opponents.\n"
			"    Please revisit your tablemap, especially:\n"
			"      * seated regions\n"
			"      * active regions\n"
			"      * cardbacks\n",
			k_title_string, 0);
	}
	else if (AutoPlayerDidActAtLeastOnce())
	{
		// This is the GOOD case!
		OH_MessageBox_Interactive(
			"GOOD NEWS:\n"
			"  * At least one tablemap in scraper folder.\n"
			"  * You are connected to a table.\n"
			"  * Your chair got recognized.\n"
			"  * The autoplayer did act at least once.\n"
			"    This means that your map and your settings\n"
			"    are at least not completely wrong.\n" 
			"    The rest is fine-tuning,\n"
			"    we won't deal with that here.\n"
			"\n"
			"Good luck, dear friend.\n",
			k_title_string, 0);
	}
	else if (NoCardsVisible())
	{
		OH_MessageBox_Interactive(
			"GOOD NEWS:\n"
			"  * At least one tablemap in scraper folder\n."
			"  * You are connected to a table.\n"
			"  * Your chair got recognized.\n"
			"  * You seem to have at least 1 opponent.\n"
			"\n"
			"BAD NEWS:\n"
			"  * You don't have any cards\n"
			"    and without cards you can't play poker.\n"
			"    If there should be cards then please revisit your tablemap.",
			k_title_string, 0);
	}
	else if (NotEnoughButtonsVisible())
	{
		OH_MessageBox_Interactive(
			"GOOD NEWS:\n"
			"  * At least one tablemap in scraper folder\n."
			"  * You are connected to a table.\n"
			"  * Your chair got recognized.\n"
			"  * You seem to have at least 1 opponent.\n"
			"  * Your cards got recognized.\n"
			"\n"
			"BAD NEWS:\n"
			"  * Not enough buttons detected.\n"
			"    Without at least two buttons the autoplayer can't act.\n"
			"    If it is your turn then please revisit your tablemap.",
			k_title_string, 0);
	}
	else
	{
		// This should not happen.
		// No error detected, but autoplayer did not yet act.
		OH_MessageBox_Interactive(
			"GOOD NEWS:\n"
			"  * At least one tablemap in scraper folder\n."
			"  * You are connected to a table\n"
			"  * Your chair got recognized\n"
			"  * You seem to have at least 1 opponent\n"
			"  * Your cards got recognized\n"
			"  * Buttons got detected\n"
			"\n"
			"BAD NEWS:\n"
			"  * The autoplayer did not (yet) act,\n"
			"    but this should happen within some seconds.\n"
			"\n"
			"Good luck, dear friend.\n",
			k_title_string, 0);
	}
}