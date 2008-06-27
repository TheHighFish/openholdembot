#include "StdAfx.h"
#include <complex>

#include "Autoplayer.h"
#include "debug.h"
#include "symbols.h"
#include "global.h"
#include "threads.h"
#include "grammar.h"
#include "scraper.h"
//  2008.02.21 by THF
#include "PokerChat.hpp"

Autoplayer::Autoplayer(void) 
{

    __SEH_SET_EXCEPTION_HANDLER(MyUnHandledExceptionFilter);

    __SEH_HEADER

    // Seed RNG
    srand((unsigned)time( NULL ));

    __SEH_LOGFATAL("Autoplayer::Constructor : \n");

}

Autoplayer::~Autoplayer(void) 
{
    __SEH_HEADER

    __SEH_LOGFATAL("Autoplayer::Destructor : \n");

}


void Autoplayer::do_Chat(void)
{
    //  PokerChat
    //  2008.02.27 by THF
    //
    //  Activating the chat box by a mouse click;
    //    then sending the message to the keyboard.
    //
    //  We can't use "do_click = 5", as this would lead to problems
    //    with the default check button. Therefore we duplicated
    //    the clicking code. :(
    //
    __SEH_HEADER
    if (symbols.f$chat == 0)
    {
        return;
    }
    INPUT			input[100] = {0};
    POINT			pt;
    double			fScreenWidth = ::GetSystemMetrics( SM_CXSCREEN )-1;
    double			fScreenHeight = ::GetSystemMetrics( SM_CYSCREEN )-1;
    double			fx, fy;
    CMutex			Mutex(false, "OHAntiColl");
    HWND			hwnd_focus = GetFocus();
    HWND			hwnd_foreground = GetForegroundWindow();
    HWND			hwnd_active = GetActiveWindow();
    POINT			cur_pos;
    GetCursorPos(&cur_pos);
    pt = randomize_click_location(global.trans.map.r$[global.trans.map.r$chatbox].left,
                                  global.trans.map.r$[global.trans.map.r$chatbox].top,
                                  global.trans.map.r$[global.trans.map.r$chatbox].right,
                                  global.trans.map.r$[global.trans.map.r$chatbox].bottom);
    // Translate click point to screen/mouse coords
    ClientToScreen(global.attached_hwnd, &pt);
    fx = pt.x*(65535.0f/fScreenWidth);
    fy = pt.y*(65535.0f/fScreenHeight);
    // Set up the input structure
    ZeroMemory(&input[0],sizeof(INPUT));
    input[0].type = INPUT_MOUSE;
    input[0].mi.dx = fx;
    input[0].mi.dy = fy;
    input[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;
    ZeroMemory(&input[1],sizeof(INPUT));
    input[1].type = INPUT_MOUSE;
    input[1].mi.dx = fx;
    input[1].mi.dy = fy;
    input[1].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTUP;
    // If we get a lock, do the action
    if (Mutex.Lock(500))
    {
        SetFocus(global.attached_hwnd);
        SetForegroundWindow(global.attached_hwnd);
        SetActiveWindow(global.attached_hwnd);
        SendInput(2, input, sizeof(INPUT));
        //
        //  Pre: f$chat > 0,
        //   Chatbox selected by a mouse click.
        //
        //  We can now "type in" the message, if there's one.
        //
        send_ChatMessage_to_Keyboard();
        //
        //  Restore window state
        SetActiveWindow(hwnd_active);
        SetForegroundWindow(hwnd_foreground);
        SetFocus(hwnd_focus);
        SetCursorPos(cur_pos.x, cur_pos.y);
        Mutex.Unlock();
    }

    __SEH_LOGFATAL("Autoplayer::do_Chat : \n");

}


void Autoplayer::do_autoplayer(void) 
{

    __SEH_SET_EXCEPTION_HANDLER(MyUnHandledExceptionFilter);

    __SEH_HEADER

    bool			prwin_running, scrape_running;
    int				x, error;
    int				num_buttons_visible;
    int				delay;

    EnterCriticalSection(&cs_prwin);
    prwin_running = prwin_thread_alive;
    LeaveCriticalSection(&cs_prwin);
    EnterCriticalSection(&cs_updater);
    scrape_running = global.update_in_process;
    LeaveCriticalSection(&cs_updater);

    // Check status of "Keyboard" menu item, and engage if necessary
    check_bring_keyboard();

    // Get r$ indices of buttons that are visible
    num_buttons_visible = get_r$_button_indices();

    // Handle f$play
    error = SUCCESS;
	symbols.f$play = calc_f$symbol(&global.formula, "f$play", global.preferences.Trace_functions[nTracePlay], &error);
    do_f$play();

    // Handle i86buttons
    do_i86();

    //  2007.02.27 by THF
    //
    //  Additional functionality: PokerChat
    //    (Handle f$chat)
    //
    //  Selecting a chat message (or no one).
    //    This message will then be processed by the autoplayer,
    //    when it's time to click the buttons.
    //
    symbols.f$chat = calc_f$symbol(&global.formula, "f$chat", &error);
    register_ChatMessage(symbols.f$chat);
    //  Avoiding unnecessary calls to do_Chat(),
    //    especially mouse movements to the chat box.
    if ((symbols.f$chat != 0) && is_Chat_allowed())
    {
        do_Chat();
    }

	// Get count of stable frames for use a little bit further down
    x = count_same_scrapes();

    // If we are in a scrape/symbol calc cycle, then return
    if (scrape_running)
        return;

    // If prwin thread is still iterating, then return
    if (prwin_running)
        return;

    // Handle f$prefold
    error = SUCCESS;
    symbols.f$prefold = calc_f$symbol(&global.formula, "f$prefold", global.preferences.Trace_functions[nTracePrefold], &error);
    do_prefold();

    // if we have <2 visible buttons, then return
    // Change from only requiring one visible button (OpenHoldem 2008-04-03)
    if (num_buttons_visible < 2)
        return;

    // if we are not playing (occluded?) 2008-03-25 Matrix
    if (!symbols.sym.playing)
        return;

    // If we don't have enough stable frames, or have not waited f$delay milliseconds, then return (modified Spektre 2008-04-03)
    symbols.f$delay = calc_f$symbol(&global.formula, "f$delay", &error);
    delay = symbols.f$delay / global.preferences.scrape_delay;    // scale f$delay to a number of scrapes

	if (x < (int) global.preferences.frame_delay + delay)
        return;

    // calculate f$alli, f$swag, f$rais, and f$call for autoplayer's use
    symbols.sym.isfinalanswer = true;
    error = SUCCESS;
	symbols.f$alli = calc_f$symbol(&global.formula, "f$alli", global.preferences.Trace_functions[nTraceAlli], &error);
    error = SUCCESS;
    symbols.f$swag = calc_f$symbol(&global.formula, "f$swag", global.preferences.Trace_functions[nTraceSwag], &error);
    error = SUCCESS;
    symbols.f$rais = calc_f$symbol(&global.formula, "f$rais", global.preferences.Trace_functions[nTraceRais], &error);
    error = SUCCESS;
    symbols.f$call = calc_f$symbol(&global.formula, "f$call", global.preferences.Trace_functions[nTraceCall], &error);
    symbols.sym.isfinalanswer = false;

    // do swag first since it is the odd one
    if (symbols.f$swag && !symbols.f$alli && scraper.get_button_state(3)) 
	{
        do_swag();
    }
    else 
	{
		if (symbols.f$alli) do_slider();
		else do_arccf();
    }


    __SEH_LOGFATAL("Autoplayer::do_autoplayer :\n");

}

void Autoplayer::do_swag(void) {
    __SEH_HEADER

    int				input_count, r$index;
    POINT			pt;
    double			fScreenWidth = ::GetSystemMetrics( SM_CXSCREEN )-1;
    double			fScreenHeight = ::GetSystemMetrics( SM_CYSCREEN )-1;
    double			fx, fy;
    INPUT			input[100] = {0};
    CMutex			Mutex(false, "OHAntiColl");
    char			ch_str[100];
    int				i;
    int				vkey;
    HWND			hwnd_focus = GetFocus();
    HWND			hwnd_foreground = GetForegroundWindow();
    HWND			hwnd_active = GetActiveWindow();
    POINT			cur_pos;
    bool			lost_focus=false;

    ::GetCursorPos(&cur_pos);

    // swag buttons are hard coded as #3 now.  Can they be different?
    if (scraper.get_button_state(3) && global.trans.map.r$iXedit_index[3]!=-1)
    {

        // If we get a lock, do the action
        if (Mutex.Lock(500))
        {

            // TEXT SELECTION
            if (global.preferences.text_selection_method == TEXTSEL_DOUBLECLICK)
            {
                input_count = 0;

                // Double click in edit box
                r$index = global.trans.map.r$iXedit_index[3];
                pt = randomize_click_location(global.trans.map.r$[r$index].left, global.trans.map.r$[r$index].top,
                                              global.trans.map.r$[r$index].right, global.trans.map.r$[r$index].bottom);
                ClientToScreen(global.attached_hwnd, &pt);
                fx = pt.x*(65535.0f/fScreenWidth);
                fy = pt.y*(65535.0f/fScreenHeight);

                ZeroMemory(&input[input_count],sizeof(INPUT));
                input[input_count].type = INPUT_MOUSE;
                input[input_count].mi.dx = fx;
                input[input_count].mi.dy = fy;
                input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;
                input_count++;

                ZeroMemory(&input[input_count],sizeof(INPUT));
                input[input_count].type = INPUT_MOUSE;
                input[input_count].mi.dwFlags = MOUSEEVENTF_LEFTUP;
                input_count++;

                ZeroMemory(&input[input_count],sizeof(INPUT));
                input[input_count].type = INPUT_MOUSE;
                input[input_count].mi.dx = fx;
                input[input_count].mi.dy = fy;
                input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;
                input_count++;

                ZeroMemory(&input[input_count],sizeof(INPUT));
                input[input_count].type = INPUT_MOUSE;
                input[input_count].mi.dwFlags = MOUSEEVENTF_LEFTUP;
                input_count++;
            }

            else if (global.preferences.text_selection_method == TEXTSEL_CLICKDRAG)
            {
                input_count = 0;

                // left click, drag to left, un-left click
                r$index = global.trans.map.r$iXedit_index[3];
                pt.x = global.trans.map.r$[r$index].right;
                pt.y = global.trans.map.r$[r$index].top + (global.trans.map.r$[r$index].bottom-global.trans.map.r$[r$index].top)/2;
                ClientToScreen(global.attached_hwnd, &pt);
                fx = pt.x*(65535.0f/fScreenWidth);
                fy = pt.y*(65535.0f/fScreenHeight);

                ZeroMemory(&input[input_count],sizeof(INPUT));
                input[input_count].type = INPUT_MOUSE;
                input[input_count].mi.dx = fx;
                input[input_count].mi.dy = fy;
                input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;
                input_count++;

                pt.x = global.trans.map.r$[r$index].left;
                pt.y = global.trans.map.r$[r$index].top + (global.trans.map.r$[r$index].bottom-global.trans.map.r$[r$index].top)/2;
                ClientToScreen(global.attached_hwnd, &pt);
                fx = pt.x*(65535.0f/fScreenWidth);
                fy = pt.y*(65535.0f/fScreenHeight);

                ZeroMemory(&input[input_count],sizeof(INPUT));
                input[input_count].type = INPUT_MOUSE;
                input[input_count].mi.dx = fx;
                input[input_count].mi.dy = fy;
                input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
                input_count++;

                ZeroMemory(&input[input_count],sizeof(INPUT));
                input[input_count].type = INPUT_MOUSE;
                input[input_count].mi.dwFlags = MOUSEEVENTF_LEFTUP;
                input_count++;
            }

            else
            {
                return;
            }

            // Do text selection and sleep
            SetFocus(global.attached_hwnd);
            SetForegroundWindow(global.attached_hwnd);
            SetActiveWindow(global.attached_hwnd);

            SendInput(input_count, input, sizeof(INPUT));

            // Check for stolen focus, and thus misswag
            if (GetForegroundWindow() != global.attached_hwnd)
                lost_focus = true;

            ::SetCursorPos(cur_pos.x, cur_pos.y);

            Sleep(global.preferences.swag_delay_1);



            // TEXT DELETION
            if (global.preferences.text_deletion_method == TEXTDEL_DELETE)
            {
                input_count = 0;

                // Press delete
                // Delete down
                ZeroMemory(&input[input_count],sizeof(INPUT));
                input[input_count].type = INPUT_KEYBOARD;
                input[input_count].ki.wVk = VK_DELETE;
                input_count++;

                // Delete up
                ZeroMemory(&input[input_count],sizeof(INPUT));
                input[input_count].type = INPUT_KEYBOARD;
                input[input_count].ki.wVk = VK_DELETE;
                input[input_count].ki.dwFlags = KEYEVENTF_KEYUP;
                input_count++;
            }

            else if (global.preferences.text_deletion_method == TEXTDEL_BACKSPACE)
            {
                input_count = 0;

                // Press backspace
                // Backspace down
                ZeroMemory(&input[input_count],sizeof(INPUT));
                input[input_count].type = INPUT_KEYBOARD;
                input[input_count].ki.wVk = VK_BACK;
                input_count++;

                // Backspace up
                ZeroMemory(&input[input_count],sizeof(INPUT));
                input[input_count].type = INPUT_KEYBOARD;
                input[input_count].ki.wVk = VK_BACK;
                input[input_count].ki.dwFlags = KEYEVENTF_KEYUP;
                input_count++;
            }

            else
            {
                return;
            }

            // do it and sleep
            SetFocus(global.attached_hwnd);
            SetForegroundWindow(global.attached_hwnd);
            SetActiveWindow(global.attached_hwnd);

            SendInput(input_count, input, sizeof(INPUT));

            // Check for stolen focus, and thus misswag
            if (GetForegroundWindow() != global.attached_hwnd)
                lost_focus = true;

            ::SetCursorPos(cur_pos.x, cur_pos.y);

            Sleep(global.preferences.swag_delay_2);



            // SWAG AMOUNT ENTRY
            input_count = 0;

            // Click in edit box
            r$index = global.trans.map.r$iXedit_index[3];
            pt = randomize_click_location(global.trans.map.r$[r$index].left, global.trans.map.r$[r$index].top,
                                          global.trans.map.r$[r$index].right, global.trans.map.r$[r$index].bottom);
            ClientToScreen(global.attached_hwnd, &pt);
            fx = pt.x*(65535.0f/fScreenWidth);
            fy = pt.y*(65535.0f/fScreenHeight);

            ZeroMemory(&input[input_count],sizeof(INPUT));
            input[input_count].type = INPUT_MOUSE;
            input[input_count].mi.dx = fx;
            input[input_count].mi.dy = fy;
            input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;
            input_count++;

            ZeroMemory(&input[input_count],sizeof(INPUT));
            input[input_count].type = INPUT_MOUSE;
            input[input_count].mi.dwFlags = MOUSEEVENTF_LEFTUP;
            input_count++;


            if (symbols.f$swag != (int) symbols.f$swag)
            {
                sprintf(ch_str, "%.2f", symbols.f$swag);
            }
            else
            {
                sprintf(ch_str, "%.0f", symbols.f$swag);
            }

            for (i=0; i<(int) strlen(ch_str); i++)
            {
                if (ch_str[i]>='0' && ch_str[i]<='9')
                {
                    vkey = ch_str[i];
                }
                if (ch_str[i]=='.')
                {
                    vkey = VK_DECIMAL;
                }

                ZeroMemory(&input[input_count],sizeof(INPUT));
                input[input_count].type = INPUT_KEYBOARD;
                input[input_count].ki.wVk = vkey;
                input_count++;

                ZeroMemory(&input[input_count],sizeof(INPUT));
                input[input_count].type = INPUT_KEYBOARD;
                input[input_count].ki.wVk = vkey;
                input[input_count].ki.dwFlags = KEYEVENTF_KEYUP;
                input_count++;
            }

            // do it and sleep for global.preferences.swag_delay (ms)
            SetFocus(global.attached_hwnd);
            SetForegroundWindow(global.attached_hwnd);
            SetActiveWindow(global.attached_hwnd);

            SendInput(input_count, input, sizeof(INPUT));

            // Check for stolen focus, and thus misswag
            if (GetForegroundWindow() != global.attached_hwnd)
                lost_focus = true;

            ::SetCursorPos(cur_pos.x, cur_pos.y);

            Sleep(global.preferences.swag_delay_3);



            // BET CONFIRMATION ACTION
            if (global.preferences.bet_confirmation_method == BETCONF_ENTER)
            {
                input_count = 0;

                // Press enter
                ZeroMemory(&input[input_count],sizeof(INPUT));
                input[input_count].type = INPUT_KEYBOARD;
                input[input_count].ki.wVk = VK_RETURN;
                input_count++;

                ZeroMemory(&input[input_count],sizeof(INPUT));
                input[input_count].type = INPUT_KEYBOARD;
                input[input_count].ki.wVk = VK_RETURN;
                input[input_count].ki.dwFlags = KEYEVENTF_KEYUP;
                input_count++;
            }

            else if (global.preferences.bet_confirmation_method == BETCONF_CLICKBET &&
                     (rais_but!=-1 || global.trans.map.r$iXbutton_index[3]!=-1) )
            {
                input_count = 0;

                // Click on bet/raise button
                // use i3button region it it exists, otherwise use the bet/raise button region
                if (global.trans.map.r$iXbutton_index[3]!=-1)
                {
                    pt = randomize_click_location(global.trans.map.r$[global.trans.map.r$iXbutton_index[3]].left,
                                                  global.trans.map.r$[global.trans.map.r$iXbutton_index[3]].top,
                                                  global.trans.map.r$[global.trans.map.r$iXbutton_index[3]].right,
                                                  global.trans.map.r$[global.trans.map.r$iXbutton_index[3]].bottom);
                }

                else
                {
                    pt = randomize_click_location(global.trans.map.r$[rais_but].left, global.trans.map.r$[rais_but].top,
                                                  global.trans.map.r$[rais_but].right, global.trans.map.r$[rais_but].bottom);
                }

                // Click on button
                ClientToScreen(global.attached_hwnd, &pt);
                fx = pt.x*(65535.0f/fScreenWidth);
                fy = pt.y*(65535.0f/fScreenHeight);

                ZeroMemory(&input[input_count],sizeof(INPUT));
                input[input_count].type = INPUT_MOUSE;
                input[input_count].mi.dx = fx;
                input[input_count].mi.dy = fy;
                input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;
                input_count++;

                ZeroMemory(&input[input_count],sizeof(INPUT));
                input[input_count].type = INPUT_MOUSE;
                input[input_count].mi.dx = fx;
                input[input_count].mi.dy = fy;
                input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTUP;
                input_count++;

                // Do double click if set in preferences
                if (global.preferences.button_click_method == BUTTON_DOUBLECLICK)
                {
                    ZeroMemory(&input[input_count],sizeof(INPUT));
                    input[input_count].type = INPUT_MOUSE;
                    input[input_count].mi.dx = fx;
                    input[input_count].mi.dy = fy;
                    input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;
                    input_count++;

                    ZeroMemory(&input[input_count],sizeof(INPUT));
                    input[input_count].type = INPUT_MOUSE;
                    input[input_count].mi.dx = fx;
                    input[input_count].mi.dy = fy;
                    input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTUP;
                    input_count++;
                }

                // Restore cursor to current location
                ::GetCursorPos(&pt);
                fx = pt.x*(65535.0f/fScreenWidth);
                fy = pt.y*(65535.0f/fScreenHeight);

                ZeroMemory(&input[input_count],sizeof(INPUT));
                input[input_count].type = INPUT_MOUSE;
                input[input_count].mi.dx = fx;
                input[input_count].mi.dy = fy;
                input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
                input_count++;
            }

            else
            {
                return;
            }

            // do it
			if (!lost_focus || !global.preferences.focus_detect)
            {
                SetFocus(global.attached_hwnd);
                SetForegroundWindow(global.attached_hwnd);
                SetActiveWindow(global.attached_hwnd);

                SendInput(input_count, input, sizeof(INPUT));

                symbols.sym.didswag[4] += 1;
                symbols.sym.didswag[(int) symbols.sym.br-1] += 1;
                symbols.sym.prevaction = PREVACT_SWAG;
                global.replay_recorded_this_turn = false;

                // reset elapsedauto symbol
                time(&symbols.elapsedautohold);

                // log it
                write_log_autoplay("SWAG");
            }

            SetActiveWindow(hwnd_active);
            SetForegroundWindow(hwnd_foreground);
            SetFocus(hwnd_focus);

            ::SetCursorPos(cur_pos.x, cur_pos.y);

            Mutex.Unlock();
        }
    }

    __SEH_LOGFATAL("Autoplayer::do_swag :\n");

}

void Autoplayer::do_arccf(void) {
    __SEH_HEADER
    int				do_click, input_count;
    INPUT			input[100] = {0};
    POINT			pt;
    double			fScreenWidth = ::GetSystemMetrics( SM_CXSCREEN )-1;
    double			fScreenHeight = ::GetSystemMetrics( SM_CYSCREEN )-1;
    double			fx, fy;
    CMutex			Mutex(false, "OHAntiColl");
    HWND			hwnd_focus = GetFocus();
    HWND			hwnd_foreground = GetForegroundWindow();
    HWND			hwnd_active = GetActiveWindow();
    POINT			cur_pos;

    ::GetCursorPos(&cur_pos);


    do_click = -1;

    // ALLIN
    if (symbols.f$alli && (int) symbols.sym.myturnbits&0x8 && alli_but!=-1)
    {
        pt = randomize_click_location(global.trans.map.r$[alli_but].left, global.trans.map.r$[alli_but].top,
                                      global.trans.map.r$[alli_but].right, global.trans.map.r$[alli_but].bottom);
        do_click = 4;
    }

    // RAISE
    else if (symbols.f$rais && (int) symbols.sym.myturnbits&0x4 && rais_but!=-1)
    {
        pt = randomize_click_location(global.trans.map.r$[rais_but].left, global.trans.map.r$[rais_but].top,
                                      global.trans.map.r$[rais_but].right, global.trans.map.r$[rais_but].bottom);
        do_click = 3;
    }

    // CALL
    else if (symbols.f$call && (int) symbols.sym.myturnbits&0x1 && call_but!=-1)
    {
        pt = randomize_click_location(global.trans.map.r$[call_but].left, global.trans.map.r$[call_but].top,
                                      global.trans.map.r$[call_but].right, global.trans.map.r$[call_but].bottom);
        do_click = 2;
    }

    // CHECK
    // None of f$alli, f$swag, f$rais, f$call are > 0 or no buttons related to
    // these actions can be found. If there is a check button, then click it.
    else if (chec_but!=-1)
    {
        pt = randomize_click_location(global.trans.map.r$[chec_but].left, global.trans.map.r$[chec_but].top,
                                      global.trans.map.r$[chec_but].right, global.trans.map.r$[chec_but].bottom);
        do_click = 1;
    }

    // FOLD
    // None of f$alli, f$swag, f$rais, f$call are > 0 or no buttons related to
    // these actions can be found. If there is a fold button, then click it, otherwise we have a serious problem.
    else if (fold_but!=-1)
    {
        pt = randomize_click_location(global.trans.map.r$[fold_but].left, global.trans.map.r$[fold_but].top,
                                      global.trans.map.r$[fold_but].right, global.trans.map.r$[fold_but].bottom);
        do_click = 0;
    }

    if (do_click>=0)
    {
        input_count = 0;

        // Translate click point to screen/mouse coords
        ClientToScreen(global.attached_hwnd, &pt);
        fx = pt.x*(65535.0f/fScreenWidth);
        fy = pt.y*(65535.0f/fScreenHeight);

        // Click button
        ZeroMemory(&input[input_count],sizeof(INPUT));
        input[input_count].type = INPUT_MOUSE;
        input[input_count].mi.dx = fx;
        input[input_count].mi.dy = fy;
        input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;
        input_count++;

        ZeroMemory(&input[input_count],sizeof(INPUT));
        input[input_count].type = INPUT_MOUSE;
        input[input_count].mi.dx = fx;
        input[input_count].mi.dy = fy;
        input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTUP;
        input_count++;

        // Do double click if set in preferences
        if (global.preferences.button_click_method == BUTTON_DOUBLECLICK)
        {
            ZeroMemory(&input[input_count],sizeof(INPUT));
            input[input_count].type = INPUT_MOUSE;
            input[input_count].mi.dx = fx;
            input[input_count].mi.dy = fy;
            input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;
            input_count++;

            ZeroMemory(&input[input_count],sizeof(INPUT));
            input[input_count].type = INPUT_MOUSE;
            input[input_count].mi.dx = fx;
            input[input_count].mi.dy = fy;
            input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTUP;
            input_count++;
        }

        // Restore cursor to current location
        fx = cur_pos.x*(65535.0f/fScreenWidth);
        fy = cur_pos.y*(65535.0f/fScreenHeight);

        ZeroMemory(&input[input_count],sizeof(INPUT));
        input[input_count].type = INPUT_MOUSE;
        input[input_count].mi.dx = fx;
        input[input_count].mi.dy = fy;
        input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
        input_count++;

        // If we get a lock, do the action
        if (Mutex.Lock(500))
        {
            SetFocus(global.attached_hwnd);
            SetForegroundWindow(global.attached_hwnd);
            SetActiveWindow(global.attached_hwnd);

            SendInput(input_count, input, sizeof(INPUT));

            SetActiveWindow(hwnd_active);
            SetForegroundWindow(hwnd_foreground);
            SetFocus(hwnd_focus);

            ::SetCursorPos(cur_pos.x, cur_pos.y);

            Mutex.Unlock();

            switch (do_click)
            {
            case 4:  // allin
                symbols.sym.prevaction = PREVACT_ALLI;
                write_log_autoplay("ALLI");
                break;
            case 3:  // raise
                symbols.sym.didrais[4] += 1;
                symbols.sym.didrais[(int) symbols.sym.br-1] += 1;
                symbols.sym.prevaction = PREVACT_RAIS;
                write_log_autoplay("RAIS");
                break;
            case 2:  // call
                symbols.sym.didcall[4] += 1;
                symbols.sym.didcall[(int) symbols.sym.br-1] += 1;
                symbols.sym.prevaction = PREVACT_CALL;
                write_log_autoplay("CALL");
                break;
            case 1:  // check
                symbols.sym.didchec[4] += 1;
                symbols.sym.didchec[(int) symbols.sym.br-1] += 1;
                symbols.sym.prevaction = PREVACT_CHEC;
                write_log_autoplay("CHEC");
                break;
            case 0:  // fold
                symbols.sym.prevaction = PREVACT_FOLD;
                write_log_autoplay("FOLD");
                break;
            }

            global.replay_recorded_this_turn = false;

            // reset elapsedauto symbol
            time(&symbols.elapsedautohold);
        }
    }

    __SEH_LOGFATAL("Autoplayer::do_arccf :\n");

}

void Autoplayer::do_slider(void) {
    __SEH_HEADER
    int				do_drag, input_count,  x, y, x2;
    INPUT			input[100] = {0};
    POINT			pt, pt2;
    double			fScreenWidth = ::GetSystemMetrics( SM_CXSCREEN )-1;
    double			fScreenHeight = ::GetSystemMetrics( SM_CYSCREEN )-1;
    double			fx = 0;
	double			fy = 0;
	double			fx2 = 0;
	double			fy2 = 0;
    CMutex			Mutex(false, "OHAntiColl");
    HWND			hwnd_focus = GetFocus();
    HWND			hwnd_foreground = GetForegroundWindow();
    HWND			hwnd_active = GetActiveWindow();
    POINT			cur_pos;
	Stablemap_region handle, slider;

    ::GetCursorPos(&cur_pos);


    do_drag = -1;

    // ALLIN
    if (symbols.sym.ismyturn && global.trans.map.r$iXslider_index[3]!=-1 && global.trans.map.r$iXhandle_index[3]!=-1 &&
		scraper.handle_found_at_xy)
    {
		handle = global.trans.map.r$[global.trans.map.r$iXhandle_index[3]];
		slider = global.trans.map.r$[global.trans.map.r$iXslider_index[3]];
		x = scraper.handle_xy.x;
		y = scraper.handle_xy.y;
		x2 = slider.right-slider.left;
		pt = randomize_click_location(x, y, x + (handle.right-handle.left), handle.bottom);
		pt2.x = pt.x+x2;
		pt2.y = pt.y;
        do_drag = 1;
    }


    if (do_drag=1)
    {

        // Translate click point to screen/mouse coords
        ClientToScreen(global.attached_hwnd, &pt);
        fx = pt.x*(65535.0f/fScreenWidth);
        fy = pt.y*(65535.0f/fScreenHeight);
        ClientToScreen(global.attached_hwnd, &pt2);
        fx2 = pt2.x*(65535.0f/fScreenWidth);
        fy2 = pt2.y*(65535.0f/fScreenHeight);
		write_log("*** Jam from %d,%d to %d,%d \n", fx, fy, fx2, fy2);

        if (Mutex.Lock(500))
        {
            SetFocus(global.attached_hwnd);
            SetForegroundWindow(global.attached_hwnd);
            SetActiveWindow(global.attached_hwnd);
			// Move to handle & click & hold button
			input_count = 0;
			ZeroMemory(&input[input_count],sizeof(INPUT));
			input[input_count].type = INPUT_MOUSE;
			input[input_count].mi.dx = fx;
			input[input_count].mi.dy = fy;
			input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;
			input_count++;
            SendInput(input_count, input, sizeof(INPUT));
			Sleep(200);
			// Move the mouse
			input_count = 0;
			ZeroMemory(&input[input_count],sizeof(INPUT));
			input[input_count].type = INPUT_MOUSE;
			input[input_count].mi.dx = fx2;
			input[input_count].mi.dy = fy2;
			input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;
			input_count++;
            SendInput(input_count, input, sizeof(INPUT));
			Sleep(200);
			// Release the button
			input_count = 0;
			ZeroMemory(&input[input_count],sizeof(INPUT));
			input[input_count].type = INPUT_MOUSE;
			input[input_count].mi.dx = fx2;
			input[input_count].mi.dy = fy2;
			input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTUP;
			input_count++;
            SendInput(input_count, input, sizeof(INPUT));
			Sleep(100);
            SetActiveWindow(hwnd_active);
            SetForegroundWindow(hwnd_foreground);
            SetFocus(hwnd_focus);
            Mutex.Unlock();
		}
		if (symbols.f$alli && alli_but!=-1)
		{
			pt = randomize_click_location(global.trans.map.r$[alli_but].left, global.trans.map.r$[alli_but].top,
										  global.trans.map.r$[alli_but].right, global.trans.map.r$[alli_but].bottom);
			ClientToScreen(global.attached_hwnd, &pt);
			fx = pt.x*(65535.0f/fScreenWidth);
			fy = pt.y*(65535.0f/fScreenHeight);
			// Click button
			input_count = 0;
			ZeroMemory(&input[input_count],sizeof(INPUT));
			input[input_count].type = INPUT_MOUSE;
			input[input_count].mi.dx = fx;
			input[input_count].mi.dy = fy;
			input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
			input_count++;
			SendInput(input_count, input, sizeof(INPUT));
			Sleep(200);
		}
        // Restore cursor to current location
        fx = cur_pos.x*(65535.0f/fScreenWidth);
        fy = cur_pos.y*(65535.0f/fScreenHeight);

        input_count = 0;
        ZeroMemory(&input[input_count],sizeof(INPUT));
        input[input_count].type = INPUT_MOUSE;
        input[input_count].mi.dx = fx;
        input[input_count].mi.dy = fy;
        input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
        input_count++;

        // If we get a lock, do the action
        if (Mutex.Lock(500))
        {
            SetFocus(global.attached_hwnd);
            SetForegroundWindow(global.attached_hwnd);
            SetActiveWindow(global.attached_hwnd);

            SendInput(input_count, input, sizeof(INPUT));

            SetActiveWindow(hwnd_active);
            SetForegroundWindow(hwnd_foreground);
            SetFocus(hwnd_focus);

			::SetCursorPos(cur_pos.x, cur_pos.y);

            Mutex.Unlock();

            write_log_autoplay("JAM");

			write_log("*** Jam complete \n", fx, fy, fx2, fy2);

            // reset elapsedauto symbol
            time(&symbols.elapsedautohold);
			Sleep(1200);
        }
    }

    __SEH_LOGFATAL("Autoplayer::do_arccf :\n");

}

void Autoplayer::do_prefold(void) {
    __SEH_HEADER
    INPUT			input[100] = {0};
    POINT			pt;
    double			fScreenWidth = ::GetSystemMetrics( SM_CXSCREEN )-1;
    double			fScreenHeight = ::GetSystemMetrics( SM_CYSCREEN )-1;
    double			fx, fy;
    CMutex			Mutex(false, "OHAntiColl");
    HWND			hwnd_focus = GetFocus();
    HWND			hwnd_foreground = GetForegroundWindow();
    HWND			hwnd_active = GetActiveWindow();
    POINT			cur_pos;
    int				input_count = 0;

    ::GetCursorPos(&cur_pos);

    if (symbols.f$prefold == 0)  return;
    if (prefold_but == -1)  return;

    // Randomize click location
    pt = randomize_click_location(global.trans.map.r$[prefold_but].left, global.trans.map.r$[prefold_but].top,
                                  global.trans.map.r$[prefold_but].right, global.trans.map.r$[prefold_but].bottom);


    input_count = 0;

    // Translate click point to screen/mouse coords
    ClientToScreen(global.attached_hwnd, &pt);
    fx = pt.x*(65535.0f/fScreenWidth);
    fy = pt.y*(65535.0f/fScreenHeight);

    // Set up the input structure
    ZeroMemory(&input[input_count],sizeof(INPUT));
    input[input_count].type = INPUT_MOUSE;
    input[input_count].mi.dx = fx;
    input[input_count].mi.dy = fy;
    input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;
    input_count++;

    ZeroMemory(&input[input_count],sizeof(INPUT));
    input[input_count].type = INPUT_MOUSE;
    input[input_count].mi.dx = fx;
    input[input_count].mi.dy = fy;
    input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTUP;
    input_count++;

    // Restore cursor to current location
    fx = cur_pos.x*(65535.0f/fScreenWidth);
    fy = cur_pos.y*(65535.0f/fScreenHeight);

    ZeroMemory(&input[input_count],sizeof(INPUT));
    input[input_count].type = INPUT_MOUSE;
    input[input_count].mi.dx = fx;
    input[input_count].mi.dy = fy;
    input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
    input_count++;

    // If we get a lock, do the action
    if (Mutex.Lock(500))
    {
        SetFocus(global.attached_hwnd);
        SetForegroundWindow(global.attached_hwnd);
        SetActiveWindow(global.attached_hwnd);

        SendInput(input_count, input, sizeof(INPUT));

        SetActiveWindow(hwnd_active);
        SetForegroundWindow(hwnd_foreground);
        SetFocus(hwnd_focus);

        ::SetCursorPos(cur_pos.x, cur_pos.y);

        Mutex.Unlock();

        // reset elapsedauto symbol
        time(&symbols.elapsedautohold);

        symbols.sym.prevaction = PREVACT_FOLD;
        write_log_autoplay("FOLD");
    }

    __SEH_LOGFATAL("Autoplayer::do_prefold :\n");

}

int Autoplayer::count_same_scrapes(void) {
    __SEH_HEADER
    int						i;
    static unsigned int		card_common_last[5] = {0};
    static unsigned int		card_player_last[10][2] = {0};
    static bool				dealer_last[10] = {0};
    static double			playerbalance_last[10] = {0};
    static double			playerbet_last[10] = {0};
    static double			myturnbitslast = 0;
    bool					same_scrape;
    static int				num_same_scrapes=0;

    // These items need to be the same to count as a identical frame:
    // - up and down cards
    // - button position
    // - playerbets
    // - playerbalances
    // - button states
    same_scrape = true;
    for (i=0; i<5; i++)
    {
        if (scraper.card_common[i] != card_common_last[i])  same_scrape = false;
    }

    for (i=0; i<10; i++)
    {
        if (scraper.card_player[i][0] != card_player_last[i][0])  same_scrape = false;
        if (scraper.card_player[i][1] != card_player_last[i][1])  same_scrape = false;
        if (scraper.dealer[i] != dealer_last[i])  same_scrape = false;
        if (scraper.playerbalance[i] != playerbalance_last[i])  same_scrape = false;
        if (scraper.playerbet[i] != playerbet_last[i])  same_scrape = false;
    }

    if (symbols.sym.myturnbits != myturnbitslast)  same_scrape = false;

    if (same_scrape)
    {
        num_same_scrapes++;
    }
    else
    {
        for (i=0; i<5; i++)
        {
            card_common_last[i] = scraper.card_common[i];
        }
        for (i=0; i<10; i++)
        {
            card_player_last[i][0] = scraper.card_player[i][0];
            card_player_last[i][1] = scraper.card_player[i][1];
            dealer_last[i] = scraper.dealer[i];
            playerbalance_last[i] = scraper.playerbalance[i];
            playerbet_last[i] = scraper.playerbet[i];
        }
        myturnbitslast = symbols.sym.myturnbits;
        num_same_scrapes = 0;
    }

    return num_same_scrapes;

    __SEH_LOGFATAL("Autoplayer::count_same_scrapes :\n");

}


int Autoplayer::get_r$_button_indices(void)
{
    __SEH_HEADER
    int				i, r$index;
    int				button_index;
    CString			s;
    int				num_seen=0;

    //////////////////////////////////////////////////////////
    // find ALLIN button region from scraper
    button_index = -1;
    for (i=0; i<=9; i++)
    {
        if (scraper.get_button_state(i) && scraper.is_string_allin(scraper.buttonlabel[i]))
        {
            button_index = i;
            i = 10;
            num_seen++;
        }
    }
    // scraper can't find allin button
    if (button_index == -1)
    {
        alli_but = -1;
    }
    else
    {
        // find allin button region from profile
        alli_but = -1;
        s.Format("i%dbutton", button_index);
        for (i=0; i<=9; i++)
        {
            r$index = global.trans.map.r$iXbutton_index[i];
            if (r$index!=-1)
            {
                if (global.trans.map.r$[r$index].name == s)
                {
                    alli_but = r$index;
                    i = 10;
                }
            }
        }
    }

    //////////////////////////////////////////////////////////
    // find RAISE button region from scraper
    button_index = -1;
    for (i=0; i<=9; i++)
    {
        if (scraper.get_button_state(i) && scraper.is_string_raise(scraper.buttonlabel[i]))
        {
            button_index = i;
            i = 10;
            num_seen++;
        }
    }
    // scraper can't find raise button
    if (button_index == -1)
    {
        rais_but = -1;
    }
    else
    {
        // find raise button region from profile
        rais_but = -1;
        s.Format("i%dbutton", button_index);
        for (i=0; i<=9; i++)
        {
            r$index = global.trans.map.r$iXbutton_index[i];
            if (r$index!=-1)
            {
                if (global.trans.map.r$[r$index].name == s)
                {
                    rais_but = r$index;
                    i = 10;
                }
            }
        }
    }

    //////////////////////////////////////////////////////////
    // find CALL button region from scraper
    button_index = -1;
    for (i=0; i<=9; i++)
    {
        if (scraper.get_button_state(i) && scraper.is_string_call(scraper.buttonlabel[i]))
        {
            button_index = i;
            i = 10;
            num_seen++;
        }
    }
    // scraper can't find call button
    if (button_index == -1)
    {
        call_but = -1;
    }
    else
    {
        // find call button region from profile
        call_but = -1;
        s.Format("i%dbutton", button_index);
        for (i=0; i<=9; i++)
        {
            r$index = global.trans.map.r$iXbutton_index[i];
            if (r$index!=-1)
            {
                if (global.trans.map.r$[r$index].name == s)
                {
                    call_but = r$index;
                    i = 10;
                }
            }
        }
    }

    //////////////////////////////////////////////////////////
    // find CHECK button region from scraper
    button_index = -1;
    for (i=0; i<=9; i++)
    {
        if (scraper.get_button_state(i) && scraper.is_string_check(scraper.buttonlabel[i]))
        {
            button_index = i;
            i = 10;
            num_seen++;
        }
    }
    // scraper can't find check button
    if (button_index == -1)
    {
        chec_but = -1;
    }
    else
    {
        // find check button region from profile
        chec_but = -1;
        s.Format("i%dbutton", button_index);
        for (i=0; i<=9; i++)
        {
            r$index = global.trans.map.r$iXbutton_index[i];
            if (r$index!=-1)
            {
                if (global.trans.map.r$[r$index].name == s)
                {
                    chec_but = r$index;
                    i = 10;
                }
            }
        }
    }

    //////////////////////////////////////////////////////////
    // find FOLD button region from scraper
    button_index = -1;
    for (i=0; i<=9; i++)
    {
        if (scraper.get_button_state(i) && scraper.is_string_fold(scraper.buttonlabel[i]))
        {
            button_index = i;
            i = 10;
            num_seen++;
        }
    }
    // scraper can't find fold button
    if (button_index == -1)
    {
        fold_but = -1;
    }
    else
    {
        // find fold button region from profile
        fold_but = -1;
        s.Format("i%dbutton", button_index);
        for (i=0; i<=9; i++)
        {
            r$index = global.trans.map.r$iXbutton_index[i];
            if (r$index!=-1)
            {
                if (global.trans.map.r$[r$index].name == s)
                {
                    fold_but = r$index;
                    i = 10;
                }
            }
        }
    }

    //////////////////////////////////////////////////////////
    // find AUTOPOST button region from scraper
    button_index = -1;
    autopost_state = true;
    for (i=0; i<=9; i++)
    {
        if (scraper.is_string_autopost(scraper.buttonlabel[i]))
        {
            autopost_state = scraper.get_button_state(i);
            button_index = i;
            i = 10;
        }
    }
    // scraper can't find autopost button
    if (button_index == -1)
    {
        autopost_but = -1;
    }
    else
    {
        // find autopost button region from profile
        autopost_but = -1;
        s.Format("i%dbutton", button_index);
        for (i=0; i<=9; i++)
        {
            r$index = global.trans.map.r$iXbutton_index[i];
            if (r$index!=-1)
            {
                if (global.trans.map.r$[r$index].name == s)
                {
                    autopost_but = r$index;
                    i = 10;
                }
            }
        }
    }

    //////////////////////////////////////////////////////////
    // find SITIN button region from scraper
    button_index = -1;
    sitin_state = true;
    for (i=0; i<=9; i++)
    {
        if (scraper.is_string_sitin(scraper.buttonlabel[i]))
        {
            sitin_state = scraper.get_button_state(i);
            button_index = i;
            i = 10;
        }
    }
    // scraper can't find sitin button
    if (button_index == -1)
    {
        sitin_but = -1;
    }
    else
    {
        // find sitin button region from profile
        sitin_but = -1;
        s.Format("i%dbutton", button_index);
        for (i=0; i<=9; i++)
        {
            r$index = global.trans.map.r$iXbutton_index[i];
            if (r$index!=-1)
            {
                if (global.trans.map.r$[r$index].name == s)
                {
                    sitin_but = r$index;
                    i = 10;
                }
            }
        }
    }

    //////////////////////////////////////////////////////////
    // find SITOUT button region from scraper
    button_index = -1;
    sitout_state = false;
    for (i=0; i<=9; i++)
    {
        if (scraper.is_string_sitout(scraper.buttonlabel[i]))
        {
            sitout_state = scraper.get_button_state(i);
            button_index = i;
            i = 10;
        }
    }
    // scraper can't find sitout button
    if (button_index == -1)
    {
        sitout_but = -1;
    }
    else
    {
        // find sitout button region from profile
        sitout_but = -1;
        s.Format("i%dbutton", button_index);
        for (i=0; i<=9; i++)
        {
            r$index = global.trans.map.r$iXbutton_index[i];
            if (r$index!=-1)
            {
                if (global.trans.map.r$[r$index].name == s)
                {
                    sitout_but = r$index;
                    i = 10;
                }
            }
        }
    }

    //////////////////////////////////////////////////////////
    // find LEAVE button region from scraper
    button_index = -1;
    for (i=0; i<=9; i++)
    {
        if (scraper.get_button_state(i) && scraper.is_string_leave(scraper.buttonlabel[i]))
        {
            button_index = i;
            i = 10;
        }
    }
    // scraper can't find leave button
    if (button_index == -1)
    {
        leave_but = -1;
    }
    else
    {
        // find leave button region from profile
        leave_but = -1;
        s.Format("i%dbutton", button_index);
        for (i=0; i<=9; i++)
        {
            r$index = global.trans.map.r$iXbutton_index[i];
            if (r$index!=-1)
            {
                if (global.trans.map.r$[r$index].name == s)
                {
                    leave_but = r$index;
                    i = 10;
                }
            }
        }
    }

    //////////////////////////////////////////////////////////
    // find PREFOLD button region from scraper
    button_index = -1;
    for (i=0; i<=9; i++)
    {
        if (scraper.get_button_state(i) && scraper.is_string_prefold(scraper.buttonlabel[i]))
        {
            button_index = i;
            i = 10;
        }
    }
    // scraper can't find prefold button
    if (button_index == -1)
    {
        prefold_but = -1;
    }
    else
    {
        // find leave button region from profile
        prefold_but = -1;
        s.Format("i%dbutton", button_index);
        for (i=0; i<=9; i++)
        {
            r$index = global.trans.map.r$iXbutton_index[i];
            if (r$index!=-1)
            {
                if (global.trans.map.r$[r$index].name == s)
                {
                    prefold_but = r$index;
                    i = 10;
                }
            }
        }
    }

    //////////////////////////////////////////////////////////
    // find i86 button region from scraper
    i86_but = -1;
    i86_state = false;
    if (scraper.get_button_state(86))
    {
        i86_but = global.trans.map.r$i86button_index;
        i86_state = true;
    }

    //////////////////////////////////////////////////////////
    // find i86 button region from scraper
    for (i=0; i<=9; i++)
    {
        i86X_but[i] = -1;
        i86X_state[i] = false;
        if (scraper.get_button_state(860+i))
        {
            i86X_but[i] = global.trans.map.r$i86Xbutton_index[i];
            i86X_state[i] = true;
        }
    }

    return num_seen;


    __SEH_LOGFATAL("Autoplayer::get_r$_button_indices :\n");

}

void Autoplayer::check_bring_keyboard(void) {
    __SEH_HEADER
    HMENU			bringsysmenu;
    MENUITEMINFO	mii;
    int				input_count, i;
    INPUT			input[100] = {0};
    CMutex			Mutex(false, "OHAntiColl");
    HWND			hwnd_focus = GetFocus();
    HWND			hwnd_foreground = GetForegroundWindow();
    HWND			hwnd_active = GetActiveWindow();
    POINT			cur_pos;
    char			temp[256];
    CString			c_text;
    int				keybd_item_pos;

    GetCursorPos(&cur_pos);

    // Find position of "Keyboard" item on system menu
    bringsysmenu = GetSystemMenu(global.attached_hwnd, false);

    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_STRING;
    mii.fType = MFT_STRING;
    mii.dwTypeData = temp;
    keybd_item_pos = -1;
    for (i=GetMenuItemCount(bringsysmenu)-1; i>=0; i--) {
	    mii.cch = 256;
    
		// Get the text of this menu item
        GetMenuItemInfo(bringsysmenu, i, true, &mii);
        c_text = temp;

        // See if this is the "keyboard" menu item
        if (c_text.MakeLower().Find("keyboard") != -1) {
            keybd_item_pos = i;
            continue;
        }
    }

    // Get state of keyboard menu item
    if (keybd_item_pos == -1) {
        return;
    }
    else {
        mii.cbSize = sizeof(MENUITEMINFO);
        mii.fMask = MIIM_STATE;
        GetMenuItemInfo(bringsysmenu, keybd_item_pos, true, &mii);
    }

    if (!(mii.fState&MFS_CHECKED) && symbols.sym.isbring) {

        input_count = 0;
        // Alt key down
        ZeroMemory(&input[input_count],sizeof(INPUT));
        input[input_count].type = INPUT_KEYBOARD;
        input[input_count].ki.wVk = VK_MENU;
        input_count++;

        // Space bar down
        ZeroMemory(&input[input_count],sizeof(INPUT));
        input[input_count].type = INPUT_KEYBOARD;
        input[input_count].ki.wVk = VK_SPACE;
        input_count++;

        // Space bar up
        ZeroMemory(&input[input_count],sizeof(INPUT));
        input[input_count].type = INPUT_KEYBOARD;
        input[input_count].ki.wVk = VK_SPACE;
        input[input_count].ki.dwFlags = KEYEVENTF_KEYUP;
        input_count++;

        // Alt key up
        ZeroMemory(&input[input_count],sizeof(INPUT));
        input[input_count].type = INPUT_KEYBOARD;
        input[input_count].ki.wVk = VK_MENU;
        input[input_count].ki.dwFlags = KEYEVENTF_KEYUP;
        input_count++;
        if (Mutex.Lock(500)) {
            SetFocus(global.attached_hwnd);
            SetForegroundWindow(global.attached_hwnd);
            SetActiveWindow(global.attached_hwnd);
            SendInput(input_count, input, sizeof(INPUT));
            Mutex.Unlock();
        }
        Sleep(200);

        input_count = 0;
        // K down
        ZeroMemory(&input[input_count],sizeof(INPUT));
        input[input_count].type = INPUT_KEYBOARD;
        input[input_count].ki.wVk = 'K';
        input_count++;

        // K up
        ZeroMemory(&input[input_count],sizeof(INPUT));
        input[input_count].type = INPUT_KEYBOARD;
        input[input_count].ki.wVk = 'K';
        input[input_count].ki.dwFlags = KEYEVENTF_KEYUP;
        input_count++;
        if (Mutex.Lock(500)) {
            SetFocus(global.attached_hwnd);
            SetForegroundWindow(global.attached_hwnd);
            SetActiveWindow(global.attached_hwnd);
            SendInput(input_count, input, sizeof(INPUT));
            SetActiveWindow(hwnd_active);
            SetForegroundWindow(hwnd_foreground);
            SetFocus(hwnd_focus);
            SetCursorPos(cur_pos.x, cur_pos.y);
            Mutex.Unlock();
        }
    }

    __SEH_LOGFATAL("Autoplayer::check_bring_keyboard :\n");

}

void Autoplayer::do_f$play(void) {
    __SEH_HEADER
    INPUT			input[100] = {0};
    POINT			pt;
    double			fScreenWidth = ::GetSystemMetrics( SM_CXSCREEN )-1;
    double			fScreenHeight = ::GetSystemMetrics( SM_CYSCREEN )-1;
    double			fx, fy;
    CMutex			Mutex(false, "OHAntiColl");
    bool			do_click;
    int				input_count = 0;
    HWND			hwnd_focus = GetFocus();
    HWND			hwnd_foreground = GetForegroundWindow();
    HWND			hwnd_active = GetActiveWindow();
    POINT			cur_pos;

    ::GetCursorPos(&cur_pos);

    do_click = false;

    // leave table
    if (symbols.f$play==-2 && leave_but!=-1)
    {
        pt = randomize_click_location(global.trans.map.r$[leave_but].left, global.trans.map.r$[leave_but].top,
                                      global.trans.map.r$[leave_but].right, global.trans.map.r$[leave_but].bottom);

        input_count = 0;

        // Translate click point to screen/mouse coords
        ClientToScreen(global.attached_hwnd, &pt);
        fx = pt.x*(65535.0f/fScreenWidth);
        fy = pt.y*(65535.0f/fScreenHeight);

        // Set up the input structure
        ZeroMemory(&input[input_count],sizeof(INPUT));
        input[input_count].type = INPUT_MOUSE;
        input[input_count].mi.dx = fx;
        input[input_count].mi.dy = fy;
        input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;
        input_count++;

        ZeroMemory(&input[input_count],sizeof(INPUT));
        input[input_count].type = INPUT_MOUSE;
        input[input_count].mi.dx = fx;
        input[input_count].mi.dy = fy;
        input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTUP;
        input_count++;

        // Restore cursor to current location
        fx = cur_pos.x*(65535.0f/fScreenWidth);
        fy = cur_pos.y*(65535.0f/fScreenHeight);

        ZeroMemory(&input[input_count],sizeof(INPUT));
        input[input_count].type = INPUT_MOUSE;
        input[input_count].mi.dx = fx;
        input[input_count].mi.dy = fy;
        input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
        input_count++;

        do_click = true;
    }

    // no action
    else if (symbols.f$play==-1)
    {
    }

    // sit out
    else if (symbols.f$play==0 && ((sitout_but!=-1 && sitout_state==false) || (sitin_but!=-1 && sitin_state==true)))
    {

        if (sitout_but!=-1 && sitout_state==false)
        {
            pt = randomize_click_location(global.trans.map.r$[sitout_but].left, global.trans.map.r$[sitout_but].top,
                                          global.trans.map.r$[sitout_but].right, global.trans.map.r$[sitout_but].bottom);
        }
        else if (sitin_but!=-1 && sitin_state==true)
        {
            pt = randomize_click_location(global.trans.map.r$[sitin_but].left, global.trans.map.r$[sitin_but].top,
                                          global.trans.map.r$[sitin_but].right, global.trans.map.r$[sitin_but].bottom);
        }

        input_count = 0;

        // Translate click point to screen/mouse coords
        ClientToScreen(global.attached_hwnd, &pt);
        fx = pt.x*(65535.0f/fScreenWidth);
        fy = pt.y*(65535.0f/fScreenHeight);

        // Set up the input structure
        ZeroMemory(&input[input_count],sizeof(INPUT));
        input[input_count].type = INPUT_MOUSE;
        input[input_count].mi.dx = fx;
        input[input_count].mi.dy = fy;
        input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;
        input_count++;

        ZeroMemory(&input[input_count],sizeof(INPUT));
        input[input_count].type = INPUT_MOUSE;
        input[input_count].mi.dx = fx;
        input[input_count].mi.dy = fy;
        input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTUP;
        input_count++;

        // Restore cursor to current location
        fx = cur_pos.x*(65535.0f/fScreenWidth);
        fy = cur_pos.y*(65535.0f/fScreenHeight);

        ZeroMemory(&input[input_count],sizeof(INPUT));
        input[input_count].type = INPUT_MOUSE;
        input[input_count].mi.dx = fx;
        input[input_count].mi.dy = fy;
        input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
        input_count++;

        do_click = true;
    }

    // sit in
    else if (symbols.f$play==1 && ((sitin_but!=-1 && sitin_state==false) || (sitout_but!=-1 && sitout_state==true)))
    {
        if (sitin_but!=-1 && sitin_state==false)
        {
            pt = randomize_click_location(global.trans.map.r$[sitin_but].left, global.trans.map.r$[sitin_but].top,
                                          global.trans.map.r$[sitin_but].right, global.trans.map.r$[sitin_but].bottom);
        }
        else if (sitout_but!=-1 && sitout_state==true)
        {
            pt = randomize_click_location(global.trans.map.r$[sitout_but].left, global.trans.map.r$[sitout_but].top,
                                          global.trans.map.r$[sitout_but].right, global.trans.map.r$[sitout_but].bottom);
        }

        input_count = 0;

        // Translate click point to screen/mouse coords
        ClientToScreen(global.attached_hwnd, &pt);
        fx = pt.x*(65535.0f/fScreenWidth);
        fy = pt.y*(65535.0f/fScreenHeight);

        // Set up the input structure
        ZeroMemory(&input[input_count],sizeof(INPUT));
        input[input_count].type = INPUT_MOUSE;
        input[input_count].mi.dx = fx;
        input[input_count].mi.dy = fy;
        input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;
        input_count++;

        ZeroMemory(&input[input_count],sizeof(INPUT));
        input[input_count].type = INPUT_MOUSE;
        input[input_count].mi.dx = fx;
        input[input_count].mi.dy = fy;
        input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTUP;
        input_count++;

        // Restore cursor to current location
        fx = cur_pos.x*(65535.0f/fScreenWidth);
        fy = cur_pos.y*(65535.0f/fScreenHeight);

        ZeroMemory(&input[input_count],sizeof(INPUT));
        input[input_count].type = INPUT_MOUSE;
        input[input_count].mi.dx = fx;
        input[input_count].mi.dy = fy;
        input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
        input_count++;

        do_click = true;
    }

    // Autopost
    if (symbols.f$play==1 && autopost_but!=-1 && autopost_state==false)
    {
        pt = randomize_click_location(global.trans.map.r$[autopost_but].left, global.trans.map.r$[autopost_but].top,
                                      global.trans.map.r$[autopost_but].right, global.trans.map.r$[autopost_but].bottom);

        // Translate click point to screen/mouse coords
        ClientToScreen(global.attached_hwnd, &pt);
        fx = pt.x*(65535.0f/fScreenWidth);
        fy = pt.y*(65535.0f/fScreenHeight);

        // Set up the input structure
        ZeroMemory(&input[input_count],sizeof(INPUT));
        input[input_count].type = INPUT_MOUSE;
        input[input_count].mi.dx = fx;
        input[input_count].mi.dy = fy;
        input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;
        input_count++;

        ZeroMemory(&input[input_count],sizeof(INPUT));
        input[input_count].type = INPUT_MOUSE;
        input[input_count].mi.dx = fx;
        input[input_count].mi.dy = fy;
        input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTUP;
        input_count++;

        // Restore cursor to current location
        fx = cur_pos.x*(65535.0f/fScreenWidth);
        fy = cur_pos.y*(65535.0f/fScreenHeight);

        ZeroMemory(&input[input_count],sizeof(INPUT));
        input[input_count].type = INPUT_MOUSE;
        input[input_count].mi.dx = fx;
        input[input_count].mi.dy = fy;
        input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
        input_count++;

        do_click = true;
    }

    if (do_click)
    {
        // If we get a lock, do the action
        if (Mutex.Lock(500)) {
            SetFocus(global.attached_hwnd);
            SetForegroundWindow(global.attached_hwnd);
            SetActiveWindow(global.attached_hwnd);

            SendInput(input_count, input, sizeof(INPUT));

            SetActiveWindow(hwnd_active);
            SetForegroundWindow(hwnd_foreground);
            SetFocus(hwnd_focus);

            ::SetCursorPos(cur_pos.x, cur_pos.y);

            Mutex.Unlock();

            // reset elapsedauto symbol
            time(&symbols.elapsedautohold);

        }
    }

    __SEH_LOGFATAL("Autoplayer::do_f$play :\n");

}

void Autoplayer::do_i86(void) 
{
    __SEH_HEADER

    INPUT			input[100] = {0};
    POINT			pt;
    double			fScreenWidth = ::GetSystemMetrics( SM_CXSCREEN )-1;
    double			fScreenHeight = ::GetSystemMetrics( SM_CYSCREEN )-1;
    double			fx, fy;
    CMutex			Mutex(false, "OHAntiColl");
    bool			do_click;
    int				input_count = 0;
    int				i;
    HWND			hwnd_focus = GetFocus();
    HWND			hwnd_foreground = GetForegroundWindow();
    HWND			hwnd_active = GetActiveWindow();
    POINT			cur_pos;

    ::GetCursorPos(&cur_pos);

    do_click = false;
    if (i86_but!=-1 && i86_state)
    {
        pt = randomize_click_location(global.trans.map.r$[i86_but].left, global.trans.map.r$[i86_but].top,
                                      global.trans.map.r$[i86_but].right, global.trans.map.r$[i86_but].bottom);

        input_count = 0;

        // Translate click point to screen/mouse coords
        ClientToScreen(global.attached_hwnd, &pt);
        fx = pt.x*(65535.0f/fScreenWidth);
        fy = pt.y*(65535.0f/fScreenHeight);

        // Set up the input structure
        ZeroMemory(&input[input_count],sizeof(INPUT));
        input[input_count].type = INPUT_MOUSE;
        input[input_count].mi.dx = fx;
        input[input_count].mi.dy = fy;
        input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;
        input_count++;

        ZeroMemory(&input[input_count],sizeof(INPUT));
        input[input_count].type = INPUT_MOUSE;
        input[input_count].mi.dx = fx;
        input[input_count].mi.dy = fy;
        input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTUP;
        input_count++;

        // Restore cursor to current location
        fx = cur_pos.x*(65535.0f/fScreenWidth);
        fy = cur_pos.y*(65535.0f/fScreenHeight);

        ZeroMemory(&input[input_count],sizeof(INPUT));
        input[input_count].type = INPUT_MOUSE;
        input[input_count].mi.dx = fx;
        input[input_count].mi.dy = fy;
        input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
        input_count++;

        do_click = true;
    }

    else
    {
        for (i=0; i<=9; i++)
        {
            if (i86X_but[i]!=-1 && i86X_state[i])
            {
                pt = randomize_click_location(global.trans.map.r$[i86X_but[i]].left, global.trans.map.r$[i86X_but[i]].top,
                                              global.trans.map.r$[i86X_but[i]].right, global.trans.map.r$[i86X_but[i]].bottom);

                input_count = 0;

                // Translate click point to screen/mouse coords
                ClientToScreen(global.attached_hwnd, &pt);
                fx = pt.x*(65535.0f/fScreenWidth);
                fy = pt.y*(65535.0f/fScreenHeight);

                // Set up the input structure
                ZeroMemory(&input[input_count],sizeof(INPUT));
                input[input_count].type = INPUT_MOUSE;
                input[input_count].mi.dx = fx;
                input[input_count].mi.dy = fy;
                input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;
                input_count++;
                ZeroMemory(&input[input_count],sizeof(INPUT));
                input[input_count].type = INPUT_MOUSE;
                input[input_count].mi.dx = fx;
                input[input_count].mi.dy = fy;
                input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTUP;
                input_count++;

                // Restore cursor to current location
                fx = cur_pos.x*(65535.0f/fScreenWidth);
                fy = cur_pos.y*(65535.0f/fScreenHeight);

                ZeroMemory(&input[input_count],sizeof(INPUT));
                input[input_count].type = INPUT_MOUSE;
                input[input_count].mi.dx = fx;
                input[input_count].mi.dy = fy;
                input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
                input_count++;

                do_click = true;
                i = 10;
            }
        }
    }

    if (do_click)
    {
        // If we get a lock, do the action
        if (Mutex.Lock(500))
        {
            SetFocus(global.attached_hwnd);
            SetForegroundWindow(global.attached_hwnd);
            SetActiveWindow(global.attached_hwnd);

            SendInput(input_count, input, sizeof(INPUT));

            SetActiveWindow(hwnd_active);
            SetForegroundWindow(hwnd_foreground);
            SetFocus(hwnd_focus);

            ::SetCursorPos(cur_pos.x, cur_pos.y);

            Mutex.Unlock();

            // reset elapsedauto symbol
            time(&symbols.elapsedautohold);
        }
    }

    __SEH_LOGFATAL("Autoplayer::do_i86 :\n");

}

POINT Autoplayer::randomize_click_location(int left, int top, int right, int bottom) 
{
    __SEH_HEADER

    POINT p;

    // uniform random distribution, yuck!
    //p.x = ((double) rand() / (double) RAND_MAX) * (right-left) + left;
    //p.y = ((double) rand() / (double) RAND_MAX) * (bottom-top) + top;

    // normal random distribution - much better!
    get_click_point(left + (right-left)/2, top + (bottom-top)/2, (right-left)/2, (bottom-top)/2, &p);

    return p;

    __SEH_LOGFATAL("Autoplayer::random_click_location :\n");

}

void Autoplayer::get_click_point(int x, int y, int rx, int ry, POINT *p) 
{
    __SEH_HEADER

    p->x = x + (int) (randomNormalScaled(2*rx, 0, 1) + 0.5) - (rx);
    p->y = y + (int) (randomNormalScaled(2*ry, 0, 1) + 0.5) - (ry);

    __SEH_LOGFATAL("Autoplayer::get_click_point :\n");

}

// random number - 0 -> scale, with normal distribution
// ignore results outside 3.5 stds from the mean
double Autoplayer::randomNormalScaled(double scale, double m, double s) 
{
    __SEH_HEADER

    double res = -99;
    while (res < -3.5 || res > 3.5) res = randomNormal(m, s);
    return (res / 3.5*s + 1) * (scale / 2.0);

    __SEH_LOGFATAL("Autoplayer::randomNormalScaled :\n");

}

double Autoplayer::randomNormal(double m, double s) 
{
    __SEH_HEADER

    /* mean m, standard deviation s */
    double x1, x2, w, y1, y2;
    do {
        x1 = 2.0 * ((double) rand()/(double) RAND_MAX) - 1.0;
        x2 = 2.0 * ((double) rand()/(double) RAND_MAX) - 1.0;
        w = x1 * x1 + x2 * x2;
    } while ( w >= 1.0 );

    w = sqrt( (-2.0 * log( w ) ) / w );
    y1 = x1 * w;
    y2 = x2 * w;

    return( m + y1 * s );

    __SEH_LOGFATAL("Autoplayer::randomNormal :\n");

}
