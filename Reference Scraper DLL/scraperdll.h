//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#ifndef INC_SCRAPERDLL_H
#define INC_SCRAPERDLL_H

#include <atlstr.h>
#include "../OpenHoldem/MagicNumbers.h"

#ifdef SCRAPERDLL_EXPORTS
#define SCRAPERDLL_API __declspec(dllexport)
#else
#define SCRAPERDLL_API __declspec(dllimport)
#endif

enum LimitType { NoLimit, PotLimit, FixedLimit };


struct SScraperState
{
	char				title[512];
	unsigned int		card_common[kNumberOfCommunityCards];
	unsigned int		card_player[kMaxNumberOfPlayers][kNumberOfCardsPerPlayer];
	bool				dealer[kMaxNumberOfPlayers];
	bool			    sitting_out[kMaxNumberOfPlayers];
	CString				seated[kMaxNumberOfPlayers];
	CString				active[kMaxNumberOfPlayers];
	CString				name[kMaxNumberOfPlayers];
	double				balance[kMaxNumberOfPlayers];
	bool				name_good_scrape[kMaxNumberOfPlayers];
	bool				balance_good_scrape[kMaxNumberOfPlayers];
	double				bet[kMaxNumberOfPlayers];
	double				pot[kMaxNumberOfPots];
	CString				button_state[k_max_number_of_buttons];
	CString				i86X_button_state[k_max_number_of_buttons];
	CString				i86_button_state;
	CString				button_label[k_max_number_of_buttons];
	double				sblind;
	bool				found_sblind;
	double				bblind;
	bool				found_bblind;
	double				bbet;
	bool				found_bbet;
	double				ante;
	bool				found_ante;
	LimitType			limit;
	bool				found_limit;
	CString				handnumber;
	bool				found_handnumber;
	bool				istournament;
};

typedef void (*scraper_override_t)(SScraperState *state);
SCRAPERDLL_API void OverrideScraper(SScraperState *state);

typedef void (*scraper_process_message_t)(const char *message, const void *param);
SCRAPERDLL_API void ProcessMessage(const char *message, const void *param);

#endif //INC_SCRAPERDLL_H