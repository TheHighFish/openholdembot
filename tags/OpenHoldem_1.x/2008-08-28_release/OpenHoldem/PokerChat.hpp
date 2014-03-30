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
//    providing:
//      - an interface for whuser.dll
//      - (limited) chat support for the OH scripting language.
//
//  PokerChat is non-OOP, as we pass a function pointer to DLLs,
//    which would lead to problems for methods du to the
//    implicit this pointer.
//
//////////////////////////////////////////////////////////////////////////

//  Enumeration constants for simple chat messages.
//
enum
{
    no_simple_Chat = 0,
	first_ChatMessage = 1,
 	simple_Chat_gg = first_ChatMessage,
 	simple_Chat_nh,
	simple_Chat_vnh,
 	simple_Chat_n1,
 	simple_Chat_wow,
	simple_Chat_lol,
 	simple_chat_rofl,
 	simple_Chat_haha,
 	simple_Chat_tu,
 	simple_Chat_thx, 	
 	simple_Chat_omg,
 	simple_Chat_sick,
 	simple_Chat_fu,
	simple_Chat_fish,
 	simple_Chat_hi,
 	simple_Chat_hello,
 	simple_Chat_bye,
	//  Due to a missing mapping function of characters 
	//    to keybord scan codes we do atm not support
	//    non-alpha-numeric characters.
 	//  simple_Chat_happy,  //  Message: ":)"
 	//  simple_Chat_sad,    //  Message: ":("
	last_ChatMessage = simple_Chat_bye
};


//  Startup message for the DLL,
//    combined with a pointer to "send_ChatMessage"
//    similar to "pfgws".
//
static const char Pointer_for__send_ChatMessage[] = "p_send_chatMessage";


typedef void (*PointerType_send_ChatMessage)(char* the_Message);


//  Interface for the DLL-guys.
//
//  IMPORTANT: 
//  ==========
//    The variable containing the message has to be global,
//    as we store a pointer to the message, until the autoplayer
//    acts again!
//
void send_ChatMessage(char* the_Message);


//  Processing the chat-message (if there's one)
//    (To be called by the autoplayer, which selects
//    the chatbox, too.) 
//    
void send_ChatMessage_to_Keyboard();


//  Getting a pointer to "send_ChatMessage" to send to the DLL
//    at startup (similar to "pfgws")
//
PointerType_send_ChatMessage get_Pointer_to__send_ChatMessage();


//  Chat enabled in preferences
//    and safety delay expired?
//
bool is_Chat_allowed(void);


//  Simple messages for the formula guys.
//    Use f$chat as "the_MessageIndex".
//
void register_ChatMessage(double the_MessageIndex);


#endif //INC_POKERCHAT_HPP