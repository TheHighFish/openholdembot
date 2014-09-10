//==============================================*****************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//==============================================*****************
//
// Purpose:
//
//==============================================*****************

#ifndef INC_CLAZYSCRAPER_H
#define INC_CLAZYSCRAPER_H

class CLazyScraper
{
public:
	CLazyScraper();
	~CLazyScraper();
public:
	void DoScrape();
private:
	bool NeedDealerChair();
	bool NeedHandNumber();
	bool NeedUsersCards();
	bool NeedAllPlayersCards();
	bool NeedFoldButton();
	bool NeedActionbuttons();
	bool NeedInterfaceButtons();
	bool NeedBetpotButtons();
	bool NeedSlider();
	bool NeedBetsAndBalances();
	bool NeedAllPlayerNames();
	bool NeedUnknownPlayerNames();
	bool NeedCommunityCards();
private:
	void ScrapeUnknownPlayerNames();

private:
	bool CardScrapeNeeded();
	bool CompleteScrapeNeeded();
};

extern CLazyScraper *p_lazyscraper;

#endif // INC_CLAZYSCRAPER_H
