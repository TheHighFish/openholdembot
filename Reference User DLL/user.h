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

#ifndef _user_h_
#define _user_h_

#ifdef USERDLL_EXPORTS
#define USERDLL_API __declspec(dllexport)
#else
#define USERDLL_API __declspec(dllimport)
#endif

struct holdem_player {
    char            m_name[16]          ;	//player name if known
    double          m_balance           ;	//player balance 
    double          m_currentbet        ;	//player current bet
    unsigned char   m_cards[2]          ;	//player cards

    unsigned char   m_name_known    : 1 ;	//0=no 1=yes
    unsigned char   m_balance_known : 1 ;	//0=no 1=yes
    unsigned char   m_fillerbits    : 6 ;	//filler bits
    unsigned char   m_fillerbyte        ;	//filler bytes
};

struct holdem_state {
    char            m_title[64]         ;	//table title
    double          m_pot[10]           ;	//total in each pot

    unsigned char   m_cards[5]          ;	//common cards

    unsigned char   m_is_playing    : 1 ;	//0=sitting-out, 1=sitting-in
    unsigned char   m_is_posting    : 1 ;	//0=autopost-off, 1=autopost-on
    unsigned char   m_fillerbits    : 6 ;	//filler bits

    unsigned char   m_fillerbyte        ;	//filler byte
    unsigned char   m_dealer_chair      ;	//0-9

    holdem_player   m_player[10]        ;	//player records
};

typedef double (*process_message_t)(const char* message, const void* param );
USERDLL_API double process_message( const char* message, const void* param );

double process_state(holdem_state* pstate);
double process_query(const char* pquery);


// ToDo by THF: move all this (and more) to an extra header,
// care about initialization

// Functions exported by OpenHoldem
extern "C" __declspec(dllimport) double __stdcall GetSymbolFromDll(const int chair, const char* name, bool& iserr);
extern "C" __declspec(dllimport) void   __stdcall SendChatMessageFomDll(const char *msg);
extern "C" __declspec(dllimport) void*  __stdcall GetPhl1kFromDll();
extern "C" __declspec(dllimport) void*  __stdcall GetPrw1326FromDll();
extern "C" __declspec(dllimport) void   __stdcall WriteLogFromDll(char* fmt, ...);
extern "C" __declspec(dllimport) char*  __stdcall GetHandnumberFromDll();


double getsym(const char* name)	
{ 
	bool error_flag;
	return GetSymbolFromDll(0, name, error_flag);
}

#endif
