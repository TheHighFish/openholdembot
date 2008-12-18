#ifndef INC_SCRPAERDLL_H
#define INC_SCRAPERDLL_H

#include <atlstr.h>

#ifdef SCRAPERDLL_EXPORTS
#define SCRAPERDLL_API __declspec(dllexport)
#else
#define SCRAPERDLL_API __declspec(dllimport)
#endif

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
	double			bet[10];
	double			pot[10];
	CString			button_state[10];
	CString			i86X_button_state[10];
	CString			i86_button_state;
	CString			button_label[10];
};

typedef void (*scraper_override_t)(SScraperState *state);
SCRAPERDLL_API void OverrideScraper(SScraperState *state);

#endif //INC_SCRAPERDLL_H