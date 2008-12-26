/*
	Filename	:	whuser.cpp
	Author(s)	:	winholdem development
	Date		:	2004-JUN-25
	Copyright	:	(c) 2004 HixoxiH Software
	History		:
*/

#define WHUSER_EXPORTS

#include "whuser.h"
#include <windows.h>
#include <atlstr.h>
#include <conio.h>

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

pfgws_t m_pget_winholdem_symbol;

double gws( int chair, const char* name, bool& iserr ) {
   return (*m_pget_winholdem_symbol)(chair,name,iserr);
}

double gws( const char* name ) {
   bool iserr;
   int mychair = (int) gws(0,"userchair",iserr);
   return gws(mychair,name,iserr);
}

double process_query( const char* pquery ) {
	if (pquery==NULL)
		return 0;

	if (strncmp(pquery,"dll$test",8)==0)
		return gws("random");

	if (strncmp(pquery,"dll$spend",9)==0)
		return gws("f$spend");

	if (strncmp(pquery,"dll$recurse",11)==0)
		return gws("dll$mynumber");

	if (strncmp(pquery,"dll$mynumber",12)==0)
		return 12345.67;

	if (strncmp(pquery,"dll$complex",11)==0)
		return gws("f$spend/25 * 1.1");

	return 0;
}

double process_state( holdem_state* pstate ) {
	if (pstate!=NULL) {	m_holdem_state[ (++m_ndx)&0xff ] = *pstate; }

	_cprintf("state: %d\n", m_ndx&0xff);

	return 0;
}

/////////////////////////////////////////////////////
//WINHOLDEM RUNTIME ENTRY POINT
/////////////////////////////////////////////////////
WHUSER_API double process_message (const char* pmessage, const void* param) {
	if (pmessage==NULL) { return 0; }
	if (param==NULL) { return 0; }

	if (strcmp(pmessage,"state")==0) { 
		holdem_state *state = (holdem_state*) param;
//		CString s; 
//		s.Format("<%s>\nPOTS: %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\nCOMMON: %d %d %d %d %d\n%d %d %d\n<%s> %d %.2f %d", 
//			state->m_title,
//			state->m_pot[0], state->m_pot[1], state->m_pot[2], state->m_pot[3], state->m_pot[4], 
//			state->m_pot[5], state->m_pot[6], state->m_pot[7], state->m_pot[8], state->m_pot[9],
//			state->m_cards[0], state->m_cards[1], state->m_cards[2], state->m_cards[3], state->m_cards[4],
//			state->m_is_playing, state->m_is_posting, state->m_dealer_chair,
//			state->m_player[4].m_name, state->m_player[4].m_name_known, state->m_player[4].m_balance, state->m_player[4].m_balance_known);
//		MessageBox(NULL, s, "state", MB_OK);

		return process_state( (holdem_state*)param ); 
	}

	if (strcmp(pmessage,"query")==0) { 
//		MessageBox(NULL, (LPCSTR) param, "query", MB_OK);
		return process_query( (const char*)param ); 
	}

	if (strcmp(pmessage,"pfgws")==0) {	
//		MessageBox(NULL, "pfgws", "MESSAGE", MB_OK);
		m_pget_winholdem_symbol = (pfgws_t)param;
		return 0;
	}

	if (strcmp(pmessage,"event")==0 && strcmp((const char *) param, "load")==0) { 
//		MessageBox(NULL, "event-load", "MESSAGE", MB_OK);
	}

	if (strcmp(pmessage,"event")==0 && strcmp((const char *) param, "unload")==0) { 
//		MessageBox(NULL, "event-unload", "MESSAGE", MB_OK);
	}

	return 0;
}
/////////////////////////////////////////////////////

/////////////////////////////////
//DLLMAIN
/////////////////////////////////
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
/////////////////////////////////
