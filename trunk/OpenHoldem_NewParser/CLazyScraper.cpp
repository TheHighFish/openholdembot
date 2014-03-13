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

#include "stdafx.h"
#include "CLazyScraper.h"

#include "CAutoplayer.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CSymbolEngineHistory.h"
#include "CSymbolEngineIsTournament.h"
#include "CSymbolEngineTableLimits.h"
#include "CSymbolEngineUserchair.h"
#include "debug.h"

CLazyScraper *p_lazyscraper = NULL;

CLazyScraper::CLazyScraper()
{
}

CLazyScraper::~CLazyScraper()
{
}

// Some ideas:
// ===========
//
// Things we need to scrape every heartbeat,
// depending on handreset-method:
//   * handnumber
//   * action-buttons (standard, no betpot)
//   * cards of the userchair
//
// Things we need to scrape if the userchair is not yet known:
//   * action-buttons (standard, no betpot)
//   * all players cards
//
// Things we need to scrape on our turn
//   * betpot-buttons
//   * slider
// (only if the game-type is NL or PL)
//
// Once per everyheartbeat (cash-game):
//   * scrape "seated" for all chairs
//   * scrape "active" for all seated chairs
//   * scrape cards for all active chairs
//   * scrape bets and balances for all active chairs
// (but we might skip this if we have folded 
// and the hand-history-generator is disabled)
//
// Tournaments:
//   * scrape bets and balances for all chairs when our turn because of ICM
//
// Name-scraping
// Preflop up to our first action only, because:
//   * once it is our turn we have stable frames
//   * later name-changes (new players) don't affect the game
//   * potential occlusion later can no longer affect PT
//   * improved CPU-usage as name-scraping needs about 40%
//
// DLLs and/or HH-gen might need all data all the time.
// If in doubt be conservative.

void CLazyScraper::DoScrape()
{
	if (p_scraper->IsIdenticalScrape())
	{
		return;
	}
	p_scraper->ScrapeLimits();
	if (NeedDealerChair())
	{
		p_scraper->ScrapeDealer();
	}
	if (NeedUsersCards())
	{
		assert(p_symbol_engine_userchair->userchair_confirmed());
		p_scraper->ScrapePlayerCards(p_symbol_engine_userchair->userchair());
	}
	p_scraper->ScrapeSeatedActive();
	if (NeedAllPlayersCards())
	{
		p_scraper->ScrapeAllPlayerCards(); 
	}
	if (NeedCommunityCards())
	{
		p_scraper->ScrapeCommonCards();
	}
	if (NeedFoldButton())
	{
		// For fast detection of my turn
		// !! Currently included in NeedActionbuttons()
	}
	if (NeedActionbuttons())
	{
		p_scraper->ScrapeActionButtons();
		p_scraper->ScrapeActionButtonLabels();
	}
	if (NeedInterfaceButtons())
	{
		p_scraper->ScrapeInterfaceButtons();
	}
	if (NeedBetpotButtons())
	{
		p_scraper->ScrapeBetpotButtons();
	}
	if (NeedSlider())
	{
		p_scraper->ScrapeSlider();
	}
	// Swagbox AKA i3edit does not need to be scraped
	// The CasinoInterface checks the existence and uses this region automatically
	if (NeedBetsAndBalances())
	{
		p_scraper->ScrapeBetsBalances();
		p_scraper->ScrapePots();
	}
	if (NeedAllPlayerNames())
	{
		p_scraper->ClearAllPlayerNames();
		ScrapeUnknownPlayerNames();
	}
	if (NeedUnknownPlayerNames())
	{
		ScrapeUnknownPlayerNames();
	}
}

bool CLazyScraper::NeedDealerChair()
{
	//if (p_tablemap->HandResetMethodDealer())
	return true;
}

bool CLazyScraper::NeedHandNumber()
{
	//if (p_tablemap->HandResetMethodHandNumber())
	return true;
}

bool CLazyScraper::NeedUsersCards()
{
	return (p_tablemap->HandResetMethodCards() && p_symbol_engine_userchair->userchair_confirmed());
}

bool CLazyScraper::NeedAllPlayersCards()
{
	// User playing
	// HH-gen
	// DLL?
	return true;
}

bool CLazyScraper::NeedFoldButton()
{
	// To detect my turn
	return true;
}

bool CLazyScraper::NeedActionbuttons()
{
	// If MyTurn
	return true;
}

bool CLazyScraper::NeedInterfaceButtons()
{
	return (p_autoplayer->TimeToHandleSecondaryFormulas());
}

bool CLazyScraper::NeedBetpotButtons()
{
	return (p_scraper_access->IsMyTurn()
		&& (p_symbol_engine_tablelimits->isnl() || p_symbol_engine_tablelimits->ispl()));
}

bool CLazyScraper::NeedSlider()
{
	return NeedBetpotButtons(); 
}

bool CLazyScraper::NeedBetsAndBalances()
{
	return true;
}

bool CLazyScraper::NeedAllPlayerNames()
{
	// Scraping names is the most costly part and mostly needed for PokerTracker.
	// It is enough if we do this until our turn, because
	// * at our turn we have stable frames
	// * new players after our turn can't affect the current hand
	return (!p_symbol_engine_history->DidActThisHand());
}

bool CLazyScraper::NeedUnknownPlayerNames()
{
	// Always true
	// If a new player sits down or we missed an existing player
	// due to occlusion we should always scrape that name.
	return true;
}

bool CLazyScraper::NeedCommunityCards()
{
	return true;
}

void CLazyScraper::ScrapeUnknownPlayerNames()
{
	for (int i=0; i<p_tablemap->nchairs(); i++)
	{
		if (p_scraper_access->IsPlayerSeated(i) 
			&& (p_scraper->player_name(i) == ""))
		{
			p_scraper->ScrapeName(i);
		}
	}
}