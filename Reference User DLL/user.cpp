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

// Card macro
#define RANK(c)       (((c)>>4)&0x0f)
#define SUIT(c)       (((c)>>0)&0x0f)
#define ISCARDBACK(c) ((c)==0xff)
#define ISUNKNOWN(c)  ((c)==0)

// process_query()
//   Handling the lookup of dll$symbols
double process_query( const char* pquery ) {
	if (pquery==NULL)
		return 0;
	if (strncmp(pquery,"dll$test",8)==0) {
		return GetSymbol("random");
  }
	if (strncmp(pquery,"dll$spend",9)==0) {
		return GetSymbol("f$spend");
  }
	if (strncmp(pquery,"dll$recurse",11)==0) {
		return GetSymbol("dll$mynumber");
  }
	if (strncmp(pquery,"dll$mynumber",12)==0) {
		return 12345.67;
  }
	return 0;
}

// OnLoad and OnUnload()
//   called once and at the beginning of a session
//   when the DLL gets loaded / unloaded
//   Do initilization / finalization here.

void OnLoad() {
#ifdef OPT_DEMO_OUTPUT
		MessageBox(NULL, "event-load", "MESSAGE", MB_OK);
#endif OPT_DEMO_OUTPUT
}

void OnUnload() {
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
			AllocConsole();
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			FreeConsole();
			break;
    }
  return TRUE;
}
