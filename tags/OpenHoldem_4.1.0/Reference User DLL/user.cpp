//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*****************************************************************************
//
// Purpose: Very simple user-DLL as a starting-point
//
//*****************************************************************************


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

/////////////////////////////////////
//card macros
#define RANK(c)			((c>>4)&0x0f)
#define SUIT(c)			((c>>0)&0x0f)
#define ISCARDBACK(c)	(c==0xff)
#define ISUNKNOWN(c)	(c==0)
/////////////////////////////////////

////////////////////////////////////
//consecutive states
holdem_state	m_holdem_state[256];
unsigned char	m_ndx;
////////////////////////////////////


double process_query( const char* pquery ) 
{
	if (pquery==NULL)
		return 0;

	if (strncmp(pquery,"dll$test",8)==0)
		return getsym("random");

	if (strncmp(pquery,"dll$spend",9)==0)
		return getsym("f$spend");

	if (strncmp(pquery,"dll$recurse",11)==0)
		return getsym("dll$mynumber");

	if (strncmp(pquery,"dll$mynumber",12)==0)
		return 12345.67;

	if (strncmp(pquery,"dll$complex",11)==0)
		return getsym("f$spend/25 * 1.1");

	return 0;
}

double process_state( holdem_state* pstate ) 
{
	if (pstate!=NULL) {	m_holdem_state[ (++m_ndx)&0xff ] = *pstate; }

	_cprintf("state: %d\n", m_ndx&0xff);

	return 0;
}

/////////////////////////////////////////////////////
// user.dll entry point
/////////////////////////////////////////////////////
USERDLL_API double process_message (const char* pmessage, const void* param) 
{
	if (pmessage==NULL) { return 0; }
	if (param==NULL) { return 0; }

	if (strcmp(pmessage,"state")==0) 
	{ 
		holdem_state *state = (holdem_state*) param;

#ifdef OPT_DEMO_OUTPUT
		CString s; 
		s.Format("<%s>\nPOTS: %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\nCOMMON: %d %d %d %d %d\n%d %d %d\n<%s> %d %.2f %d", 
			state->m_title,
			state->m_pot[0], state->m_pot[1], state->m_pot[2], state->m_pot[3], state->m_pot[4], 
			state->m_pot[5], state->m_pot[6], state->m_pot[7], state->m_pot[8], state->m_pot[9],
			state->m_cards[0], state->m_cards[1], state->m_cards[2], state->m_cards[3], state->m_cards[4],
			state->m_is_playing, state->m_is_posting, state->m_dealer_chair,
			state->m_player[4].m_name, state->m_player[4].m_name_known, state->m_player[4].m_balance, state->m_player[4].m_balance_known);
		MessageBox(NULL, s, "state", MB_OK);
#endif OPT_DEMO_OUTPUT

		return process_state( (holdem_state*)param ); 
	}

	if (strcmp(pmessage,"query")==0) 
	{ 
#ifdef OPT_DEMO_OUTPUT
		MessageBox(NULL, (LPCSTR) param, "query", MB_OK);
#endif OPT_DEMO_OUTPUT

		return process_query( (const char*)param ); 
	}

	if (strcmp(pmessage,"event")==0 && strcmp((const char *) param, "load")==0) 
	{ 
#ifdef OPT_DEMO_OUTPUT
		MessageBox(NULL, "event-load", "MESSAGE", MB_OK);
#endif OPT_DEMO_OUTPUT
	}

	if (strcmp(pmessage,"event")==0 && strcmp((const char *) param, "unload")==0) 
	{ 
#ifdef OPT_DEMO_OUTPUT
		MessageBox(NULL, "event-unload", "MESSAGE", MB_OK);
#endif OPT_DEMO_OUTPUT
	}

	return 0;
}
/////////////////////////////////////////////////////

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) 
{
    switch (ul_reason_for_call)	
	{
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
