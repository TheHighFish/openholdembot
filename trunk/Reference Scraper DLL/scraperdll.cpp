// scraperdll.cpp : Defines the entry point for the DLL application.
//

#include <windows.h>
#include "scraperdll.h"
#include "poker_defs.h"


SCRAPERDLL_API void OverrideScraper(SScraperState *state) 
{
	state->card_common[0] = StdDeck_MAKE_CARD(StdDeck_Rank_ACE, StdDeck_Suit_HEARTS);
	state->card_common[1] = StdDeck_MAKE_CARD(StdDeck_Rank_ACE, StdDeck_Suit_DIAMONDS);
	state->card_common[2] = StdDeck_MAKE_CARD(StdDeck_Rank_ACE, StdDeck_Suit_SPADES);
	state->card_common[3] = StdDeck_MAKE_CARD(StdDeck_Rank_ACE, StdDeck_Suit_CLUBS);
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

