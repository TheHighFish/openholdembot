#ifndef INC_SCRAPERDLL_H
#define INC_SCRAPERDLL_H

#include <atlstr.h>

#ifdef SCRAPERDLL_EXPORTS
#define SCRAPERDLL_API __declspec(dllexport)
#else
#define SCRAPERDLL_API __declspec(dllimport)
#endif

enum LimitType { NoLimit, PotLimit, FixedLimit };


struct SScraperState
{
	char			title[512];
	unsigned int	card_common[5];
	unsigned int	card_player[10][2];
	unsigned int	card_player_for_display[2];
	bool			dealer[10];
	bool			sitting_out[10];
	CString			seated[10];
	CString			active[10];
	CString			name[10];
	double			balance[10];
	bool			name_good_scrape[10];
	bool			balance_good_scrape[10];
	double			bet[10];
	double			pot[10];
	CString			button_state[10];
	CString			i86X_button_state[10];
	CString			i86_button_state;
	CString			button_label[10];
	double			sblind;
	double			bblind;
	double			bbet;
	double			ante;
	LimitType		limit;
	double			handnumber;
	bool			istournament;
};

typedef void (*scraper_override_t)(SScraperState *state);
SCRAPERDLL_API void OverrideScraper(SScraperState *state);

typedef void (*scraper_process_message_t)(const char *message, const void *param);
SCRAPERDLL_API void ProcessMessage(const char *message, const void *param);

#endif //INC_SCRAPERDLL_H