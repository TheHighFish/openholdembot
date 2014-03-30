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
	unsigned int		card_common[k_number_of_community_cards];
	unsigned int		card_player[k_max_number_of_players][k_number_of_cards_per_player];
	unsigned int		card_player_for_display[k_number_of_cards_per_player];
	bool				dealer[k_max_number_of_players];
	bool			    sitting_out[k_max_number_of_players];
	CString				seated[k_max_number_of_players];
	CString				active[k_max_number_of_players];
	CString				name[k_max_number_of_players];
	double				balance[k_max_number_of_players];
	bool				name_good_scrape[k_max_number_of_players];
	bool				balance_good_scrape[k_max_number_of_players];
	double				bet[k_max_number_of_players];
	double				pot[k_max_number_of_pots];
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
	double				handnumber;
	bool				found_handnumber;
	bool				istournament;
};

typedef void (*scraper_override_t)(SScraperState *state);
SCRAPERDLL_API void OverrideScraper(SScraperState *state);

typedef void (*scraper_process_message_t)(const char *message, const void *param);
SCRAPERDLL_API void ProcessMessage(const char *message, const void *param);

#endif //INC_SCRAPERDLL_H