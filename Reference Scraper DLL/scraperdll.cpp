//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: craperdll.cpp : Defines the entry point for the DLL application.
//
//******************************************************************************

#include <windows.h>
#include "scraperdll.h"
#include "poker_defs.h"

HWND	*target_hwnd;

SCRAPERDLL_API void OverrideScraper(SScraperState *state) 
{
	//state->card_common[0] = StdDeck_MAKE_CARD(StdDeck_Rank_ACE, StdDeck_Suit_HEARTS);
	//state->card_common[1] = StdDeck_MAKE_CARD(StdDeck_Rank_ACE, StdDeck_Suit_DIAMONDS);
	//state->card_common[2] = StdDeck_MAKE_CARD(StdDeck_Rank_ACE, StdDeck_Suit_SPADES);
	//state->card_common[3] = StdDeck_MAKE_CARD(StdDeck_Rank_ACE, StdDeck_Suit_CLUBS);
}

SCRAPERDLL_API void ProcessMessage(const char *message, const void *param)
{
	if (message==NULL)  return;

	if (strncmp(message, "connect", 7)==0) 
		target_hwnd = (HWND *) param;

	if (strncmp(message, "disconnect", 10)==0) 
		target_hwnd = (HWND *) NULL;
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return true;
}

