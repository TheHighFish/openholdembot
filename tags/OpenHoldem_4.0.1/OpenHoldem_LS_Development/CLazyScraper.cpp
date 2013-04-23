#include "stdafx.h"
#include "CLazyScraper.h"

#include "CAutoplayer.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CSymbols.h"
#include "CTablemapAccess.h"
#include "debug.h"

// As scraping is the most time-consuming part
// of the heartbeat-cycle, we do optionally
// execute only partial scrapes (aka "Lazy Scraping").
// Please see the manual for more info about that.

CLazyScraper *p_lazyscraper = NULL;

CLazyScraper::CLazyScraper()
{
}

CLazyScraper::~CLazyScraper()
{
}

void CLazyScraper::DoScrape()
{
	bool full_scrape_needed = false;
	bool opponents_cards_scraped = false;

	write_log(3, "CLazyScraper::DoScrape()\n");
	if (prefs.lazy_scraping_always_complete())
	{
		full_scrape_needed = true;
	}
	if (prefs.lazy_scraping_when_sitin_sitout_leave()
		&& p_autoplayer->TimeToHandleSitinSitoutLeave())
	{
		full_scrape_needed = true;
	}
	// Scrape action-buttons to decide, if it is our turn
	// If it is our turn we have to do of course a full scrape
	p_scraper->ScrapeActionButtons();
	if (IsMyTurn())
	{
		full_scrape_needed = true;
	}
	if (full_scrape_needed || prefs.lazy_scraping_when_we_hold_cards())
	{
		if (p_symbols->user_chair_confirmed())
		{
			p_scraper->ScrapeHeroesCards();
		}
		else
		{
			// scrape all cards, hoping to detect the userchair
			p_scraper->ScrapeAllCards();
			opponents_cards_scraped = true;
		}
	}
	if (prefs.lazy_scraping_when_we_hold_cards()
		&& HeroesCardsKnown())
	{
		full_scrape_needed = true;
	}
	if (full_scrape_needed)
	{
		printf("Full scrape needed.\n"); //!!!
		write_log(3, "CLazyScraper: Full Scrape needed.\n");
		if (!opponents_cards_scraped)
		{
			p_scraper->ScrapeOpponentsCards();
		}
		p_scraper->CompleteBasicScrapeToFullScrape();
	}
	else
	{
		write_log(3, "CLazyScraper: No full Scrape needed. Exiting early.\n");
	}
}

bool CLazyScraper::IsMyTurn()
{
	// Maybe better: at least two action-buttons visible,
	// though the fold-button should always be there.
	if (p_tablemap_access->GetButtonState(k_button_fold))
	{
		write_log(3, "CLazyScraper::IsMyTurn(): true\n");
		return true;
	}
	write_log(3, "CLazyScraper::IsMyTurn(): false\n");
	return false;
}

bool CLazyScraper::HeroesCardsKnown()
{
	if (!p_symbols->user_chair_confirmed())
	{
		write_log(3, "CLazyScraper::HeroesCardsKnown(): false, as userchair not confirmed\n");
		return false;
	}
	int userchair = p_symbols->sym()->userchair;
	int my_first_card  = p_scraper->card_player(userchair, 0);
	int my_second_card = p_scraper->card_player(userchair, 1);
	if (p_tablemap_access->IsValidCard(my_first_card)
		&& p_tablemap_access->IsValidCard(my_second_card))
	{
		write_log(3, "CLazyScraper::HaveCards(): false, no cards\n");
		return false;
	}
	write_log(3, "CLazyScraper::HaveCards(): true\n");
	return true;
}

