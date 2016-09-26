//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Very simple user-DLL as a starting-point
//
// Required OpenHoldem version: 7.7.6
//
//******************************************************************************

// Needs to be defined here, before #include "user.h"
// to generate proper export- and inport-definitions
#define USER_DLL

// #define OPT_DEMO_OUTPUT if you are a beginner 
// who wants to see some message-boxes with output of game-states, etc.
// It is disabled upon request, 
//   * as it is not really needed
//   * as some DLL-users don't have MFC (atlstr.h) installed
// http://www.maxinmontreal.com/forums/viewtopic.php?f=156&t=16232
#undef OPT_DEMO_OUTPUT

#include "user.h"
#include <conio.h>
#include <windows.h>

#ifdef OPT_DEMO_OUTPUT
#include <atlstr.h>
#endif OPT_DEMO_OUTPUT

// Supporting macros
#define HIGH_NIBBLE(c)	(((c)>>4)&0x0F)
#define LOW_NIBBLE(c)	  ((c)&0x0F)

// Card macro
#define RANK(c)         ( ISKNOWN(c) ? HIGH_NIBBLE(c) : 0 )
#define SUIT(c)         ( ISKNOWN(c) ? LOW_NIBBLE(c) : 0 )
#define ISCARDBACK(c)   ((c) == CARD_BACK)
#define ISUNKNOWN(c)    ((c) == CARD_UNDEFINED)
#define ISNOCARD(c)     ((c) == CARD_NOCARD)
#define ISKNOWN(c)      (!ISCARDBACK(c) && !ISUNKNOWN(c) && !ISNOCARD(c))

// ProcessQuery()
//   Handling the lookup of dll$symbols
DLL_IMPLEMENTS double __stdcall ProcessQuery(const char* pquery) {
	if (pquery==NULL)
		return 0;
	if (strncmp(pquery,"dll$test",9)==0) {
    //ParseHandList("list_dlltest", "AA AKs AKo");
		return GetSymbol("random");
  }
	if (strncmp(pquery,"dll$spend",10)==0) {
		return GetSymbol("f$spend");
  }
	if (strncmp(pquery,"dll$recurse",12)==0) {
		return GetSymbol("dll$mynumber");
  }
	if (strncmp(pquery,"dll$mynumber",13)==0) {
		return 12345.67;
  }
  if (strncmp(pquery, "dll$scrape", 11) == 0) {
    char* scraped_result;
    int result_lenght;
    scraped_result = ScrapeTableMapRegion("p0balance", result_lenght);
    if (scraped_result != nullptr) {
      // The TEXT() macro supports both ASCII and Unicode.
      // For the people who use Unicode but don't understand the "error".
      // http://www.maxinmontreal.com/forums/viewtopic.php?f=174&t=19999
      // http://stackoverflow.com/questions/15498070/what-does-t-stands-for-in-a-cstring
      MessageBox(0, scraped_result, TEXT("Scraped custom region"), 0);
      LocalFree(scraped_result);
    }
  }
	return 0;
}

// OnLoad and OnUnload()
//   called once and at the beginning of a session
//   when the DLL gets loaded / unloaded
//   Do initilization / finalization here.

DLL_IMPLEMENTS void __stdcall DLLOnLoad() {
#ifdef OPT_DEMO_OUTPUT
		MessageBox(NULL, "event-load", "MESSAGE", MB_OK);
#endif OPT_DEMO_OUTPUT
}

DLL_IMPLEMENTS void __stdcall DLLOnUnLoad() {
#ifdef OPT_DEMO_OUTPUT
		MessageBox(NULL, "event-unload", "MESSAGE", MB_OK);
#endif OPT_DEMO_OUTPUT
}

// DLL entry point
//   Technically required, but don't do anything here.
//   Initializations belong into the OnLoad() function,
//   where they get executed at run-time.
//   Doing things here at load-time is a bad idea,
//   as some functionalitz might not be properly initialized   
//   (including error/handling).
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
  switch (ul_reason_for_call)	{
		case DLL_PROCESS_ATTACH:
#ifdef OPT_DEMO_OUTPUT
			AllocConsole();
#endif OPT_DEMO_OUTPUT
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
#ifdef OPT_DEMO_OUTPUT
			FreeConsole();
#endif OPT_DEMO_OUTPUT
			break;
    }
  return TRUE;
}
