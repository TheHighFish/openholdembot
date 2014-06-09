//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

//////////////////////////////////////////////////////////////////////////
//
//  Author: TheHighFish
//
//  Created: 2007.11.21
//
//  Last change: 2008.02.25
//
//  Description: A very simple chat system for OpenHoldem,
//	providing:
//	  - an interface for user.dll
//	  - (limited) chat support for the OH scripting language.
//
//  PokerChat is non-OOP, as we pass a function pointer to DLLs,
//	which would lead to problems for methods.
//
//////////////////////////////////////////////////////////////////////////

#include <stdafx.h>
#include <math.h>
#include <String>
#include <time.h>
#include <windows.h>
#include <winuser.h>
#include "PokerChat.hpp"

#include "CPreferences.h"

using namespace std;


char *_the_chat_message = NULL;

const unsigned short int max_length_of_simple_messages = 9;

//  All strings have to end with an additional '\0'.
//	We thought, that is default, but saw a message "wowtu"
//
char _message_table[][max_length_of_simple_messages] =
{
	"NO CHAT\0",  //  Index = 0 means: no chat. This "message" will not be sent.
	"gg\0",
	"nh\0",
	"vnh\0",
	"n1\0",
	"wow\0"
	"lol\0",
	"rofl\0",
	"haha\0",
	"tu\0",
	"thx\0",
	"omg\0",
	"sick\0",
	"fu\0",
	"fish\0",
	"hi\0",
	"hello\0",
	"bye\0"
	//  Due to a missing mapping function of characters
	//	to keybord scan codes we do atm not support
	//	non-alpha-numeric characters.
	//":)\0",
	//":(\0"
};


//  End of enforced chat pause, a security measure against crazy bots.
time_t _first_possible_next_chat_time;



//  Interface for the DLL-guys.
//
//  IMPORTANT:
//  ==========
//	The variable containing the message has to be global,
//	as we store a pointer to the message, until the autoplayer
//	acts again!
//
void SendChatMessage(char* new_message)
{
	if (_the_chat_message != NULL || new_message == "")
	{
		//  Old message has not been sent yet.
		//  Ignore newer one (especially, if
		//  new message == "")
		return;
	};
	//  Store it, until it is processed
	_the_chat_message = new_message;
}

//  Chat enabled in preferences
//	and safety delay expired?
bool IsChatAllowed(void)
{
	//  Safety delay?
	time_t the_actual_time;
	time(&the_actual_time);

	if (_first_possible_next_chat_time > the_actual_time)
		return false;

	//  Otherwise:
	return true;
}

void ComputeFirstPossibleNextChatTime(void)
{
	time_t the_actual_time;
	time(&the_actual_time);
	_first_possible_next_chat_time = the_actual_time +
								   preferences.chat_min_delay() +
								   //  We have to convert to a double here,
								   //  as integer "division" would be result in zero.
								   (double(rand()) / RAND_MAX) * preferences.chat_random_delay();
}

//  Simple messages for the formula guys.
//	Use f$chat as "the_MessageIndex".
void RegisterChatMessage(double message_index)
{
	int the_chat_index = floor(message_index + 0.1);

	if ((the_chat_index < first_chat_message) || (the_chat_index > last_chat_message))
	{
		return;
	}

	char* the_message = _message_table[the_chat_index];
	SendChatMessage(the_message);
}