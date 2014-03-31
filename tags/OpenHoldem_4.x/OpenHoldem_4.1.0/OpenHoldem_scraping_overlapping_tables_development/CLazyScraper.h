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
	bool HaveCards();
private:
	bool CardScrapeNeeded();
	bool CompleteScrapeNeeded();
} *p_lazyscraper;

#endif // INC_CLAZYSCRAPER_H
