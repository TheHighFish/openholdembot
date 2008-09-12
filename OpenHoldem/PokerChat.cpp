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
//	  - an interface for whuser.dll
//	  - (limited) chat support for the OH scripting language.
//
//  PokerChat is non-OOP, as we pass a function pointer to DLLs,
//	which would lead to problems for methods.
//
//////////////////////////////////////////////////////////////////////////

#include <stdafx.h>
#include <String>
#include <time.h>
#include <windows.h>
#include <winuser.h>
#include "PokerChat.hpp"

#include "CPreferences.h"


using namespace std;


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


//  Pointer to the chatmessage to store it,
//	until it is processed and send to the keyboard
//
static char *_the_chat_message;

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
void SendChatMessage(char* message)
{
	if (message != NULL || message == "")
	{
		//  Old message has not been sent yet.
		//  Ignore newer one (especially, if
		//  new message == "")
		return;
	};
	//  Store it, until it is processed
	_the_chat_message = message;
}

//  Chat enabled in preferences
//	and safety delay expired?
bool IsChatAllowed(void)
{
	if (!prefs.chat_enabled())
		return false;

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
								   prefs.chat_min_delay() +
								   //  We have to convert to a double here,
								   //  as integer "division" would be result in zero.
								   (double(rand()) / RAND_MAX) * prefs.chat_random_delay();
}

//  Processing the chat-message (if there's one)
//	(To be called by the autoplayer, which selects
//	the chatbox, too.)
void SendChatMessageToKeyboard()
{
	if (!IsChatAllowed())
		return;

	if (_the_chat_message == NULL)
		return;

	//  Checks ok. Going to send the message.
	unsigned int i = 0;
	//  Send the_Message char by char,
	//	creating virtual keyboard input with "key_event"
	//	of the Windows API.
	while (_the_chat_message[i] != '\0')
	{
		char the_next_char = _the_chat_message[i];
		//
		//  All messages have must be send in upper case,
		//	but will appear in lower case.
		//	This is caused by a discrepancy betwenn ASCII codes
		//	and keybord codes (and lack of a usable mapping function).
		//	Fortunately the ASCII code for upper case chars
		//	is identical with the keybord code for lower case chars.
		//	(To create upper case characters, you would need to set
		//	  the additional flag for the shift key.)
		//
		the_next_char = toupper(the_next_char);
		keybd_event((BYTE) the_next_char,
					MapVirtualKey((BYTE) the_next_char, 1),
					0, 0);
		//  Some hardcoded delay between the keyboard messages.
		//  Is this really neccessary, as Windows messages don't
		//  have any time stamp?
		Sleep(80);
		i++;
	}
	//  Send <return> to the chatbox
	char the_return_char = '\r';
	keybd_event((BYTE) the_return_char,
				MapVirtualKey((BYTE) the_return_char, 1),
				0, 0);
	//  Deleting message and setting new safety pause
	_the_chat_message = NULL;
	ComputeFirstPossibleNextChatTime();
}


//  Getting a pointer to "send_ChatMessage" to send to the DLL
//	at startup (similar to "pfgws")
PointerTypeSendChatMessage GetPointerToSendChatMessage()
{
	return &SendChatMessage;
}

//  Simple messages for the formula guys.
//	Use f$chat as "the_MessageIndex".
void RegisterChatMessage(double message_index)
{
	unsigned int the_chat_index = message_index;

	if (the_chat_index < first_chat_message)
		return;

	if (the_chat_index > last_chat_message)
		return;

	char* the_message = _message_table[the_chat_index];
	SendChatMessage(the_message);
}