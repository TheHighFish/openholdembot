#include "stdafx.h"
#include "CAutoconnector.h"
#include "CLazyScraper.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CScreenshotEngine.h"
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

void CLazyScraper::DoScrape()
{
	write_log(3, "CLazyScraper::DoScrape()\n");
	// As scraping is the most time-consuming part
	// of the heartbeat-cycle, we do optionally
	// execute only partial scrapes (aka "Lazy Scraping").
	// Please see the manual for more info about that.
	p_screenshot_engine->TakeScreenshotOfWindow(p_autoconnector->attached_hwnd());
	p_scraper->DoBasicScrapeButtons();
	if (1 /* CardScrapeNeeded()!!*/)
	{
		write_log(3, "CLazyScraper: CardScrape needed\n");
		p_scraper->DoBasicScrapeAllPlayerCards();
		if (1 /*CompleteScrapeNeeded()!!*/)
		{
			write_log(3, "CLazyScraper: CompleteScrape needed\n");
			p_scraper->CompleteBasicScrapeToFullScrape();
		}
	}
	p_screenshot_engine->ReleaseBitmap();
}

bool CLazyScraper::IsMyTurn()
{
	if (p_scraper_access->visible_buttons[k_autoplayer_function_fold])
	{
		write_log(3, "CLazyScraper::IsMyTurn(): true\n");
		return true;
	}
	write_log(3, "CLazyScraper::IsMyTurn(): false\n");
	return false;
}

bool CLazyScraper::HaveCards()
{
	if (!p_symbol_engine_userchair->userchair_confirmed())
	{
		write_log(3, "CLazyScraper::HaveCards(): false, as userchair not confirmed\n");
		return false;
	}
	int userchair = p_symbol_engine_userchair->userchair();
	int my_first_card  = p_scraper_access->GetPlayerCards(userchair, 0);
	int my_second_card = p_scraper_access->GetPlayerCards(userchair, 1);
	if (p_scraper_access->IsValidCard(my_first_card)
		&& p_scraper_access->IsValidCard(my_second_card))
	{
		write_log(3, "CLazyScraper::HaveCards(): false, no cards\n");
		return false;
	}
	write_log(3, "CLazyScraper::HaveCards(): true\n");
	return true;
}

bool CLazyScraper::CardScrapeNeeded()
{
	return (IsMyTurn() 
		|| (prefs.lazy_scraping_when_to_scrape() == k_lazy_scraping_cards)
		|| (prefs.lazy_scraping_when_to_scrape() == k_lazy_scraping_always));
}

bool CLazyScraper::CompleteScrapeNeeded()
{
	return (IsMyTurn() 
		|| (HaveCards() && (prefs.lazy_scraping_when_to_scrape() == k_lazy_scraping_cards))
		|| (prefs.lazy_scraping_when_to_scrape() == k_lazy_scraping_always));
}