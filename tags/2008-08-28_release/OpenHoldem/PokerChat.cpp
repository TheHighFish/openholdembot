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
//    which would lead to problems for methods.
//
//////////////////////////////////////////////////////////////////////////

#include <stdafx.h>
#include <String>
#include <time.h>
#include <windows.h>
#include <winuser.h>
#include "PokerChat.hpp"

#include "CGlobal.h"


using namespace std;


const unsigned short int max_Length_of_simple_Messages = 9;

//  All strings have to end with an additional '\0'.
//    We thought, that is default, but saw a message "wowtu"
//
char the_MessageTable[][max_Length_of_simple_Messages] =
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
    //    to keybord scan codes we do atm not support
    //    non-alpha-numeric characters.
    //":)\0",
    //":(\0"
};


//  Pointer to the chatmessage to store it,
//    until it is processed and send to the keyboard
//
static char *the_ChatMessage;


//  Interface for the DLL-guys.
//
//  IMPORTANT:
//  ==========
//    The variable containing the message has to be global,
//    as we store a pointer to the message, until the autoplayer
//    acts again!
//
void send_ChatMessage(char* the_Message)
{
    __SEH_HEADER

    if ((the_ChatMessage != NULL) || (the_ChatMessage == ""))
    {
        //  Old message has not been sent yet.
        //  Ignore newer one (especially, if
        //  new message == "")
        return;
    };
    //  Store it, until it is processed
    the_ChatMessage = the_Message;

    __SEH_LOGFATAL("PokerChat::send_ChatMessage  : \n");
}


//  End of enforced chat pause, a security measure against crazy bots.
//
time_t first_possible_next_ChatTime;


//  Chat enabled in preferences
//    and safety delay expired?
//
bool is_Chat_allowed(void)
{
    __SEH_HEADER

    if (!p_global->preferences.Chat_enabled)
    {
        return false;
    }
    //  Safety delay?
    time_t the_actual_Time;
    time(&the_actual_Time);
    if (first_possible_next_ChatTime > the_actual_Time)
    {
        return false;
    }
    //  Otherwise:
    return true;

    __SEH_LOGFATAL("PokerChat::is_Chat_allowed  : \n");
}


void compute_first_possible_next_ChatTime(void)
{
    __SEH_HEADER

    time_t the_actual_Time;
    time(&the_actual_Time);
    first_possible_next_ChatTime = the_actual_Time +
                                   p_global->preferences.Chat_min_Delay +
                                   //  We have to convert to a double here,
                                   //  as integer "division" would be result in zero.
                                   (double(rand()) / RAND_MAX) * p_global->preferences.Chat_random_Delay;

    __SEH_LOGFATAL("PokerChat::compute_first_possible_next_ChatTime  : \n");
}


//  Processing the chat-message (if there's one)
//    (To be called by the autoplayer, which selects
//    the chatbox, too.)
//
void send_ChatMessage_to_Keyboard()
{
    __SEH_HEADER

    if (!is_Chat_allowed())
    {
        return;
    }
    if (the_ChatMessage == NULL) {
        return;
    }
    //  Checks ok. Going to send the message.
    unsigned int i = 0;
    //  Send the_Message char by char,
    //    creating virtual keyboard input with "key_event"
    //    of the Windows API.
    while (the_ChatMessage[i] != '\0')
    {
        char the_next_Char = the_ChatMessage[i];
        //
        //  All messages have must be send in upper case,
        //    but will appear in lower case.
        //    This is caused by a discrepancy betwenn ASCII codes
        //    and keybord codes (and lack of a usable mapping function).
        //    Fortunately the ASCII code for upper case chars
        //    is identical with the keybord code for lower case chars.
        //    (To create upper case characters, you would need to set
        //      the additional flag for the shift key.)
        //
        the_next_Char = toupper(the_next_Char);
        keybd_event((BYTE) the_next_Char,
                    MapVirtualKey((BYTE) the_next_Char, 1),
                    0, 0);
        //  Some hardcoded delay between the keyboard messages.
        //  Is this really neccessary, as Windows messages don't
        //  have any time stamp?
        Sleep(80);
        i++;
    }
    //  Send <return> to the chatbox
    char the_Return_Char = '\r';
    keybd_event((BYTE) the_Return_Char,
                MapVirtualKey((BYTE) the_Return_Char, 1),
                0, 0);
    //  Deleting message and setting new safety pause
    the_ChatMessage = NULL;
    compute_first_possible_next_ChatTime();

    __SEH_LOGFATAL("PokerChat::send_ChatMessage_to_Keyboard()  : \n");
}


//  Getting a pointer to "send_ChatMessage" to send to the DLL
//    at startup (similar to "pfgws")
//
PointerType_send_ChatMessage get_Pointer_to__send_ChatMessage()
{
    __SEH_HEADER

    return &send_ChatMessage;

    __SEH_LOGFATAL("PokerChat::get_Pointer_to__send_ChatMessage  : \n");
}

//  Simple messages for the formula guys.
//    Use f$chat as "the_MessageIndex".
//
void register_ChatMessage(double the_MessageIndex)
{
    __SEH_HEADER

    unsigned int the_ChatIndex = the_MessageIndex;
    if (the_ChatIndex < first_ChatMessage)
    {
        return;
    }
    if (the_ChatIndex > last_ChatMessage)
    {
        return;
    }
    char* the_Message = the_MessageTable[the_ChatIndex];
    send_ChatMessage(the_Message);

    __SEH_LOGFATAL("PokerChat::register_ChatMessage  : \n");
}