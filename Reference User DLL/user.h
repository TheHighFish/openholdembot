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
// DO NOT CHANGE ANYTHING IN THIS FILE!
//   
// This Header defines an interface
// Functions and data-types must exactly match.
//
//******************************************************************************

#ifndef _INC_USER_H
#define _INC_USER_H

// Import and export directives
// for use by this DLL and by OpenHoldem
#ifdef USER_DLL
#define DLL_IMPLEMENTS extern "C" __declspec(dllexport)
#define EXE_IMPLEMENTS extern "C" __declspec(dllimport)
#else
#define DLL_IMPLEMENTS extern "C" __declspec(dllimport)
#define EXE_IMPLEMENTS extern "C" __declspec(dllexport)
#endif

// Number of saved table-states
// This number must not be changed, as we do a "& 0xFF"
// at various places to normalize the index.
const int kNumberOfHoldemStatesForDLL = 256;

// SHoldemePlayer
// used for sequence of 256 consequive table-states
struct holdem_player {
    char            m_name[16]          ;       //player name if known
    double          m_balance           ;       //player balance 
    double          m_currentbet        ;       //player current bet
    unsigned char   m_cards[2]          ;       //player cards
    unsigned char   m_name_known    : 1 ;       //0=no 1=yes
    unsigned char   m_balance_known : 1 ;       //0=no 1=yes
    unsigned char   m_fillerbits    : 6 ;       //filler bits
    unsigned char   m_fillerbyte        ;       //filler bytes
};

struct holdem_state {
    char            m_title[64]         ;       //table title
    double          m_pot[10]           ;       //total in each pot
    unsigned char   m_cards[5]          ;       //common cards
    unsigned char   m_is_playing    : 1 ;       //0=sitting-out, 1=sitting-in
    unsigned char   m_is_posting    : 1 ;       //0=autopost-off, 1=autopost-on
    unsigned char   m_fillerbits    : 6 ;       //filler bits
    unsigned char   m_fillerbyte        ;       //filler byte
    unsigned char   m_dealer_chair      ;       //0-9
    holdem_player   m_player[10]        ;       //player records
};

// Functions implemented and exported by the DLL,
// imported by OpenHoldem
DLL_IMPLEMENTS double __stdcall ProcessQuery(const char* pquery);
DLL_IMPLEMENTS void __stdcall DLLOnLoad();
DLL_IMPLEMENTS void __stdcall DLLOnUnLoad();

// Functions implemented and exported by OpenHoldem,
// imported by the DLL
EXE_IMPLEMENTS double __stdcall GetSymbol(const char* name_of_single_symbol__not_expression);
EXE_IMPLEMENTS void*  __stdcall GetPrw1326();
EXE_IMPLEMENTS char*  __stdcall GetHandnumber();
EXE_IMPLEMENTS void   __stdcall ParseHandList(const char* name_of_list, const char* list_body);
EXE_IMPLEMENTS void   __stdcall SendChatMessage(const char *message);
EXE_IMPLEMENTS void   __stdcall WriteLog(char* format, ...);

// Variables exported by OpenHoldem
// avoiding the message-mess of WinHoldem,
// no longer sending any state-messages
// http://www.maxinmontreal.com/forums/viewtopic.php?f=174&t=18642
EXE_IMPLEMENTS extern holdem_state  state[kNumberOfHoldemStatesForDLL];
EXE_IMPLEMENTS extern int state_index;

#endif // _INC_USER_H
