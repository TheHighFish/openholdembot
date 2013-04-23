#ifndef INC_CLAZYSCRAPER_H
#define INC_CLAZYSCRAPER_H

extern class CLazyScraper
{
public:
	CLazyScraper();
	~CLazyScraper();
public:
	void DoScrape();
private:
	bool IsMyTurn();
	bool HeroesCardsKnown();
private:
	void ScrapeHeroesCards();
	void ScrapeOpponentsCards();
	void ScrapeAllCards();
} *p_lazyscraper;

#endif // INC_CLAZYSCRAPER_H
