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
#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CSymbolEngineHistory.h"
#include "CSymbolEngineIsTournament.h"
#include "CSymbolEngineTableLimits.h"
#include "CSymbolEngineUserchair.h"
#include "CSymbols.h"
#include "debug.h"

CLazyScraper *p_lazyscraper = NULL;

CLazyScraper::CLazyScraper()
{
}

CLazyScraper::~CLazyScraper()
{
}

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

void CLazyScraper::DoScrape()
{
	if (p_symbol_engine_userchair->userchair_confirmed())
	{
		//ScrapeDataToDetectUserchair();
	}
	//ScrapeNamesUpToMyFirstAction();
	//ScrapeHandresetData();
	//ScrapeButtonsToDetectMyTurn();
	if (p_scraper_access->IsMyTurn() /*|| time for secondary formulas*/)
	{
		// "everything" except names
		if (p_symbol_engine_istournament->istournament());
		if (p_symbol_engine_tablelimits->isnl() || p_symbol_engine_tablelimits->ispl())
		{
			//ScrapeBetPotButtons();
			//ScrapeSlider();
			//ScrapeSwagBox();
		}
	}
	
	
	

	write_log(preferences.debug_lazy_scraper(), "[CLazyScraper] DoScrape()\n");
	// As scraping is the most time-consuming part
	// of the heartbeat-cycle, we do optionally
	// execute only partial scrapes (aka "Lazy Scraping").
	// Please see the manual for more info about that.
	p_scraper->DoBasicScrapeButtons();
	if (1 /* CardScrapeNeeded()!!*/)
	{
		write_log(preferences.debug_lazy_scraper(), "[CLazyScraper] CardScrape needed\n");
		p_scraper->DoBasicScrapeAllPlayerCards();
		if (1 /*CompleteScrapeNeeded()!!*/)
		{
			write_log(preferences.debug_lazy_scraper(), "[CLazyScraper] CompleteScrape needed\n");
			p_scraper->CompleteBasicScrapeToFullScrape();
		}
	}
}

void ScrapeNamesUpToMyFirstAction()
{
	if (p_symbol_engine_history->DidActThisHand());
}

void ScrapeHandresetData()
{
	if (p_tablemap->HandResetMethodDealer());
	if (p_tablemap->HandResetMethodCards());
	if (p_tablemap->HandResetMethodHandNumber());
}

void ScrapeDataToDetectUserchair()
{}




bool CLazyScraper::IsMyTurn()
{
	if (p_scraper_access->visible_buttons[k_autoplayer_function_fold])
	{
		write_log(preferences.debug_lazy_scraper(), "[CLazyScraper] IsMyTurn(): true\n");
		return true;
	}
	write_log(preferences.debug_lazy_scraper(), "[CLazyScraper] IsMyTurn(): false\n");
	return false;
}

bool CLazyScraper::HaveCards()
{
	if (!p_symbol_engine_userchair->userchair_confirmed())
	{
		write_log(preferences.debug_lazy_scraper(), "[CLazyScraper] HaveCards(): false, as userchair not confirmed\n");
		return false;
	}
	int userchair = p_symbol_engine_userchair->userchair();
	int my_first_card  = p_scraper_access->GetPlayerCards(userchair, 0);
	int my_second_card = p_scraper_access->GetPlayerCards(userchair, 1);
	if (p_scraper_access->IsValidCard(my_first_card)
		&& p_scraper_access->IsValidCard(my_second_card))
	{
		write_log(preferences.debug_lazy_scraper(), "[CLazyScraper] HaveCards(): false, no cards\n");
		return false;
	}
	write_log(preferences.debug_lazy_scraper(), "[CLazyScraper] HaveCards(): true\n");
	return true;
}

bool CLazyScraper::CardScrapeNeeded()
{
	return (IsMyTurn() 
		|| (preferences.lazy_scraping_when_to_scrape() == k_lazy_scraping_cards)
		|| (preferences.lazy_scraping_when_to_scrape() == k_lazy_scraping_always));
}

bool CLazyScraper::CompleteScrapeNeeded()
{
	return (IsMyTurn() 
		|| (HaveCards() && (preferences.lazy_scraping_when_to_scrape() == k_lazy_scraping_cards))
		|| (preferences.lazy_scraping_when_to_scrape() == k_lazy_scraping_always));
}

/*
bool CLazyScraper::ScrapeDataForHandresetDetection()
{
	if (p_tablemap->HandResetMothodDealer())
	{

	}
	if (p_tablemap->HandResetMothodCards())
	{

	}
	if (p_tablemap->HandResetMothodHandNumber())
	{

	}
}
/*

ScrapeDataForUserchairDetection()



ScrapeDataForOurTurnDetection()

ScrapeBetpotButtonsAndSliderForNoLimit()
*/