#ifndef INC_POKERCHAT_HPP
#define INC_POKERCHAT_HPP

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
//	which would lead to problems for methods du to the
//	implicit this pointer.
//
//////////////////////////////////////////////////////////////////////////

//  Enumeration constants for simple chat messages.
//
enum
{
	no_simple_chat = 0,
	first_chat_message = 1,
 	simple_chat_gg = first_chat_message,
 	simple_chat_nh,
	simple_chat_vnh,
 	simple_chat_n1,
 	simple_chat_wow,
	simple_chat_lol,
 	simple_chat_rofl,
 	simple_chat_haha,
 	simple_chat_tu,
 	simple_chat_thx, 	
 	simple_chat_omg,
 	simple_chat_sick,
 	simple_chat_fu,
	simple_chat_fish,
 	simple_chat_hi,
 	simple_chat_hello,
 	simple_chat_bye,
	//  Due to a missing mapping function of characters 
	//	to keybord scan codes we do atm not support
	//	non-alpha-numeric characters.
 	//  simple_Chat_happy,  //  Message: ":)"
 	//  simple_Chat_sad,	//  Message: ":("
	last_chat_message = simple_chat_bye
};


//  Startup message for the DLL,
//	combined with a pointer to "send_ChatMessage"
//	similar to "pfgws".
//
static const char pointer_for_send_chat_message[] = "p_send_chat_message";

typedef void (*PointerTypeSendChatMessage)(char* message);


//  Interface for the DLL-guys.
//
//  IMPORTANT: 
//  ==========
//	The variable containing the message has to be global,
//	as we store a pointer to the message, until the autoplayer
//	acts again!
//
void SendChatMessage(char* message);

//  Chat enabled in preferences
//	and safety delay expired?
bool IsChatAllowed(void);

void ComputeFirstPossibleNextChatTime(void);

//  Simple messages for the formula guys.
//	Use f$chat as "the_MessageIndex".
void RegisterChatMessage(double message_index);

//  Pointer to the chatmessage to store it,
//	until it is processed and send to the keyboard
//
extern char *_the_chat_message;

#endif //INC_POKERCHAT_HPP