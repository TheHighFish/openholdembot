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
