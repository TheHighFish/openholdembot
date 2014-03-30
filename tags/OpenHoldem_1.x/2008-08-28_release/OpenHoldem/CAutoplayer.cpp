#include "StdAfx.h"
#include <complex>

#include "CAutoplayer.h"
#include "CSymbols.h"
#include "CIteratorThread.h"
#include "CScraper.h"
#include "CGlobal.h"

#include "grammar.h"
#include "PokerChat.hpp"

CAutoplayer	*p_autoplayer = NULL;

CRITICAL_SECTION	CAutoplayer::cs_autoplayer;

CAutoplayer::CAutoplayer(BOOL bInitiallyOwn, LPCTSTR lpszName) : _mutex(bInitiallyOwn, lpszName)
{
	__SEH_SET_EXCEPTION_HANDLER

	__SEH_HEADER

    InitializeCriticalSectionAndSpinCount(&cs_autoplayer, 4000);

	ASSERT(_mutex.m_hObject != NULL);

	// Seed RNG
	srand((unsigned)time( NULL ));

	__SEH_LOGFATAL("CAutoplayer::Constructor : \n");
}

CAutoplayer::~CAutoplayer(void) 
{
	__SEH_HEADER

    DeleteCriticalSection(&cs_autoplayer);

	__SEH_LOGFATAL("CAutoplayer::Destructor : \n");
}


void CAutoplayer::DoChat(void)
{
	//  PokerChat
	//  2008.02.27 by THF
	//
	//  Activating the chat box by a mouse click;
	//	then sending the message to the keyboard.
	//
	//  We can't use "do_click = 5", as this would lead to problems
	//	with the default check button. Therefore we duplicated
	//	the clicking code. :(
	//
	__SEH_HEADER

	double f_chat = p_symbols->f$chat();

	if (f_chat == 0)
		return;

	INPUT			input[100] = {0};
	POINT			pt;
	double			fScreenWidth = ::GetSystemMetrics( SM_CXSCREEN )-1;
	double			fScreenHeight = ::GetSystemMetrics( SM_CYSCREEN )-1;
	double			fx, fy;
	HWND			hwnd_focus = GetFocus();
	HWND			hwnd_foreground = GetForegroundWindow();
	HWND			hwnd_active = GetActiveWindow();
	POINT			cur_pos;

	GetCursorPos(&cur_pos);

	pt = RandomizeClickLocation(p_global->trans.map.r$[p_global->trans.map.r$chatbox].left,
								  p_global->trans.map.r$[p_global->trans.map.r$chatbox].top,
								  p_global->trans.map.r$[p_global->trans.map.r$chatbox].right,
								  p_global->trans.map.r$[p_global->trans.map.r$chatbox].bottom);

	// Translate click point to screen/mouse coords
	ClientToScreen(p_global->attached_hwnd(), &pt);
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
	if (_mutex.Lock(500))
	{
		SetFocus(p_global->attached_hwnd());
		SetForegroundWindow(p_global->attached_hwnd());
		SetActiveWindow(p_global->attached_hwnd());
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

		_mutex.Unlock();
	}

	__SEH_LOGFATAL("CAutoplayer::DoChat : \n");
}

void CAutoplayer::DoAutoplayer(void) 
{
	__SEH_HEADER

	int				x;
	int				num_buttons_visible;
	int				delay;

	// Check status of "Keyboard" menu item, and engage if necessary
	CheckBringKeyboard();

	// Get r$ indices of buttons that are visible
	num_buttons_visible = GetR$ButtonIndices();

	// Calculate f$play, f$prefold, f$delay and f$chat for use below
	p_symbols->CalcSecondaryFormulas();

	// Handle f$play
	DoF$play();

	// Handle i86buttons
	DoI86();

	// Handle f$prefold
	DoPrefold();

	//  2007.02.27 by THF
	//
	//  Additional functionality: PokerChat
	//	(Handle f$chat)
	//
	//  Selecting a chat message (or no one).
	//	This message will then be processed by the autoplayer,
	//	when it's time to click the buttons.
	//
	register_ChatMessage(p_symbols->f$chat());

	//  Avoiding unnecessary calls to DoChat(),
	//	especially mouse movements to the chat box.
	if ((p_symbols->f$chat() != 0) && is_Chat_allowed())
		DoChat();

	// Get count of stable frames for use a little bit further down
	x = CountSameScrape();

	// If iterator thread is still iterating, then return
	if (_iter_vars.iterator_thread_running)
	{
		// Calc primary formulas, but not with final answer, so main window can display correctly
		p_symbols->CalcPrimaryFormulas(false);

		return;
	}

	// if we have <2 visible buttons, then return
	// Change from only requiring one visible button (OpenHoldem 2008-04-03)
	if (num_buttons_visible < 2)
	{
		// Calc primary formulas, but not with final answer, so main window can display correctly
		p_symbols->CalcPrimaryFormulas(false);

		return;
	}

	// if we are not playing (occluded?) 2008-03-25 Matrix
	if (!p_symbols->sym()->playing)
	{
		// Calc primary formulas, but not with final answer, so main window can display correctly
		p_symbols->CalcPrimaryFormulas(false);
	
		return;
	}

	// If we don't have enough stable frames, or have not waited f$delay milliseconds, then return (modified Spektre 2008-04-03)
	delay = p_symbols->f$delay() / p_global->preferences.scrape_delay;	// scale f$delay to a number of scrapes

	if (x < (int) p_global->preferences.frame_delay + delay)
	{
		// Calc primary formulas, but not with final answer, so main window can display correctly
		p_symbols->CalcPrimaryFormulas(false);

		return;
	}

	// Now that we got through all of the above, we are ready to evaluate the primary formulas
	// and take the appropriate action
	p_symbols->CalcPrimaryFormulas(true);

	// do swag first since it is the odd one
	if (p_symbols->f$swag() && !p_symbols->f$alli() && p_scraper->GetButtonState(3)) 
	{
		DoSwag();
	}
	else 
	{
		if (p_symbols->f$alli() && p_scraper->GetButtonState(3))
			DoSlider();
		else 
			DoARCCF();
	}

	__SEH_LOGFATAL("CAutoplayer::DoAutoplayer :\n");

}

void CAutoplayer::DoSwag(void) 
{
	__SEH_HEADER

	int				input_count, r$index;
	POINT			pt;
	double			fScreenWidth = ::GetSystemMetrics( SM_CXSCREEN )-1;
	double			fScreenHeight = ::GetSystemMetrics( SM_CYSCREEN )-1;
	double			fx, fy;
	INPUT			input[100] = {0};
	char			ch_str[100];
	int				i;
	int				vkey;
	HWND			hwnd_focus = GetFocus();
	HWND			hwnd_foreground = GetForegroundWindow();
	HWND			hwnd_active = GetActiveWindow();
	POINT			cur_pos;
	bool			lost_focus=false;
	int				e = SUCCESS;

	double	f_swag = p_symbols->f$swag();
	
	::GetCursorPos(&cur_pos);

	// swag buttons are hard coded as #3 now.  Can they be different?
	if (p_scraper->GetButtonState(3) && p_global->trans.map.r$iXedit_index[3]!=-1)
	{

		// If we get a lock, do the action
		if (_mutex.Lock(500))
		{

			// TEXT SELECTION
			if (p_global->preferences.text_selection_method == TEXTSEL_DOUBLECLICK)
			{
				input_count = 0;

				// Double click in edit box
				r$index = p_global->trans.map.r$iXedit_index[3];
				pt = RandomizeClickLocation(p_global->trans.map.r$[r$index].left, p_global->trans.map.r$[r$index].top,
											  p_global->trans.map.r$[r$index].right, p_global->trans.map.r$[r$index].bottom);
				ClientToScreen(p_global->attached_hwnd(), &pt);
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

			else if (p_global->preferences.text_selection_method == TEXTSEL_CLICKDRAG)
			{
				input_count = 0;

				// left click, drag to left, un-left click
				r$index = p_global->trans.map.r$iXedit_index[3];
				pt.x = p_global->trans.map.r$[r$index].right;
				pt.y = p_global->trans.map.r$[r$index].top + (p_global->trans.map.r$[r$index].bottom-p_global->trans.map.r$[r$index].top)/2;
				ClientToScreen(p_global->attached_hwnd(), &pt);
				fx = pt.x*(65535.0f/fScreenWidth);
				fy = pt.y*(65535.0f/fScreenHeight);

				ZeroMemory(&input[input_count],sizeof(INPUT));
				input[input_count].type = INPUT_MOUSE;
				input[input_count].mi.dx = fx;
				input[input_count].mi.dy = fy;
				input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;
				input_count++;

				pt.x = p_global->trans.map.r$[r$index].left;
				pt.y = p_global->trans.map.r$[r$index].top + (p_global->trans.map.r$[r$index].bottom-p_global->trans.map.r$[r$index].top)/2;
				ClientToScreen(p_global->attached_hwnd(), &pt);
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
				_mutex.Unlock();
				return;
			}

			// Do text selection and sleep
			SetFocus(p_global->attached_hwnd());
			SetForegroundWindow(p_global->attached_hwnd());
			SetActiveWindow(p_global->attached_hwnd());

			SendInput(input_count, input, sizeof(INPUT));

			// Check for stolen focus, and thus misswag
			if (GetForegroundWindow() != p_global->attached_hwnd())
				lost_focus = true;

			::SetCursorPos(cur_pos.x, cur_pos.y);

			Sleep(p_global->preferences.swag_delay_1);



			// TEXT DELETION
			if (p_global->preferences.text_deletion_method == TEXTDEL_DELETE)
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

			else if (p_global->preferences.text_deletion_method == TEXTDEL_BACKSPACE)
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
				_mutex.Unlock();
				return;
			}

			// do it and sleep
			SetFocus(p_global->attached_hwnd());
			SetForegroundWindow(p_global->attached_hwnd());
			SetActiveWindow(p_global->attached_hwnd());

			SendInput(input_count, input, sizeof(INPUT));

			// Check for stolen focus, and thus misswag
			if (GetForegroundWindow() != p_global->attached_hwnd())
				lost_focus = true;

			::SetCursorPos(cur_pos.x, cur_pos.y);

			Sleep(p_global->preferences.swag_delay_2);



			// SWAG AMOUNT ENTRY
			input_count = 0;

			// Click in edit box
			r$index = p_global->trans.map.r$iXedit_index[3];
			pt = RandomizeClickLocation(p_global->trans.map.r$[r$index].left, p_global->trans.map.r$[r$index].top,
										  p_global->trans.map.r$[r$index].right, p_global->trans.map.r$[r$index].bottom);
			ClientToScreen(p_global->attached_hwnd(), &pt);
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


			if (f_swag != (int) f_swag)
			{
				sprintf(ch_str, "%.2f", f_swag);
			}
			else
			{
				sprintf(ch_str, "%.0f", f_swag);
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

			// do it and sleep for p_global->preferences.swag_delay (ms)
			SetFocus(p_global->attached_hwnd());
			SetForegroundWindow(p_global->attached_hwnd());
			SetActiveWindow(p_global->attached_hwnd());

			SendInput(input_count, input, sizeof(INPUT));

			// Check for stolen focus, and thus misswag
			if (GetForegroundWindow() != p_global->attached_hwnd())
				lost_focus = true;

			::SetCursorPos(cur_pos.x, cur_pos.y);

			Sleep(p_global->preferences.swag_delay_3);



			// BET CONFIRMATION ACTION
			if (p_global->preferences.bet_confirmation_method == BETCONF_ENTER)
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

			else if (p_global->preferences.bet_confirmation_method == BETCONF_CLICKBET &&
					 (_rais_but!=-1 || p_global->trans.map.r$iXbutton_index[3]!=-1) )
			{
				input_count = 0;

				// Click on bet/raise button
				// use i3button region it it exists, otherwise use the bet/raise button region
				if (p_global->trans.map.r$iXbutton_index[3]!=-1)
				{
					pt = RandomizeClickLocation(p_global->trans.map.r$[p_global->trans.map.r$iXbutton_index[3]].left,
												  p_global->trans.map.r$[p_global->trans.map.r$iXbutton_index[3]].top,
												  p_global->trans.map.r$[p_global->trans.map.r$iXbutton_index[3]].right,
												  p_global->trans.map.r$[p_global->trans.map.r$iXbutton_index[3]].bottom);
				}

				else
				{
					pt = RandomizeClickLocation(p_global->trans.map.r$[_rais_but].left, p_global->trans.map.r$[_rais_but].top,
												  p_global->trans.map.r$[_rais_but].right, p_global->trans.map.r$[_rais_but].bottom);
				}

				// Click on button
				ClientToScreen(p_global->attached_hwnd(), &pt);
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
				if (p_global->preferences.button_click_method == BUTTON_DOUBLECLICK)
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
				_mutex.Unlock();
				return;
			}

			// do it
			if (!lost_focus || !p_global->preferences.focus_detect)
			{
				SetFocus(p_global->attached_hwnd());
				SetForegroundWindow(p_global->attached_hwnd());
				SetActiveWindow(p_global->attached_hwnd());

				SendInput(input_count, input, sizeof(INPUT));

				// record didswag/prevaction
				int sym_br = (int) p_symbols->sym()->br;

				EnterCriticalSection(&cs_autoplayer);
					_didswag[4] = p_symbols->sym()->didswag[4] + 1;
					_didswag[sym_br-1] = p_symbols->sym()->didswag[sym_br-1] + 1;
					_prevaction = PREVACT_SWAG;
				LeaveCriticalSection(&cs_autoplayer);

				p_symbols->UpdateAutoplayerInfo();

				// reset elapsedauto symbol
				time_t my_time_t;
				time(&my_time_t);
				p_symbols->set_elapsedautohold(my_time_t);

				p_global->set_replay_recorded_this_turn(false);

				// log it
				write_logautoplay("SWAG");
			}

			SetActiveWindow(hwnd_active);
			SetForegroundWindow(hwnd_foreground);
			SetFocus(hwnd_focus);

			::SetCursorPos(cur_pos.x, cur_pos.y);

			_mutex.Unlock();
		}
	}

	__SEH_LOGFATAL("CAutoplayer::DoSwag :\n");
}

void CAutoplayer::DoARCCF(void) 
{
	__SEH_HEADER

	int				do_click, input_count;
	INPUT			input[100] = {0};
	POINT			pt;
	double			fScreenWidth = ::GetSystemMetrics( SM_CXSCREEN )-1;
	double			fScreenHeight = ::GetSystemMetrics( SM_CYSCREEN )-1;
	double			fx, fy;
	HWND			hwnd_focus = GetFocus();
	HWND			hwnd_foreground = GetForegroundWindow();
	HWND			hwnd_active = GetActiveWindow();
	POINT			cur_pos;

	int				e = SUCCESS;
	double			alli = p_symbols->f$alli();
	double			rais = p_symbols->f$rais();
	double			call = p_symbols->f$call();
	int				sym_myturnbits = (int) p_symbols->sym()->myturnbits;

	::GetCursorPos(&cur_pos);

	do_click = -1;

	// ALLIN
	if (alli && sym_myturnbits&0x8 && _alli_but!=-1)
	{
		pt = RandomizeClickLocation(p_global->trans.map.r$[_alli_but].left, p_global->trans.map.r$[_alli_but].top,
									  p_global->trans.map.r$[_alli_but].right, p_global->trans.map.r$[_alli_but].bottom);
		do_click = 4;
	}

	// RAISE
	else if (rais && sym_myturnbits&0x4 && _rais_but!=-1)
	{
		pt = RandomizeClickLocation(p_global->trans.map.r$[_rais_but].left, p_global->trans.map.r$[_rais_but].top,
									  p_global->trans.map.r$[_rais_but].right, p_global->trans.map.r$[_rais_but].bottom);
		do_click = 3;
	}

	// CALL
	else if (call && sym_myturnbits&0x1 && _call_but!=-1)
	{
		pt = RandomizeClickLocation(p_global->trans.map.r$[_call_but].left, p_global->trans.map.r$[_call_but].top,
									  p_global->trans.map.r$[_call_but].right, p_global->trans.map.r$[_call_but].bottom);
		do_click = 2;
	}

	// CHECK
	// None of f$alli, f$swag, f$rais, f$call are > 0 or no buttons related to
	// these actions can be found. If there is a check button, then click it.
	else if (_chec_but!=-1)
	{
		pt = RandomizeClickLocation(p_global->trans.map.r$[_chec_but].left, p_global->trans.map.r$[_chec_but].top,
									  p_global->trans.map.r$[_chec_but].right, p_global->trans.map.r$[_chec_but].bottom);
		do_click = 1;
	}

	// FOLD
	// None of f$alli, f$swag, f$rais, f$call are > 0 or no buttons related to
	// these actions can be found. If there is a fold button, then click it, otherwise we have a serious problem.
	else if (_fold_but!=-1)
	{
		pt = RandomizeClickLocation(p_global->trans.map.r$[_fold_but].left, p_global->trans.map.r$[_fold_but].top,
									  p_global->trans.map.r$[_fold_but].right, p_global->trans.map.r$[_fold_but].bottom);
		do_click = 0;
	}

	if (do_click>=0)
	{
		input_count = 0;

		// Translate click point to screen/mouse coords
		ClientToScreen(p_global->attached_hwnd(), &pt);
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
		if (p_global->preferences.button_click_method == BUTTON_DOUBLECLICK)
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
		if (_mutex.Lock(500))
		{
			SetFocus(p_global->attached_hwnd());
			SetForegroundWindow(p_global->attached_hwnd());
			SetActiveWindow(p_global->attached_hwnd());

			SendInput(input_count, input, sizeof(INPUT));

			SetActiveWindow(hwnd_active);
			SetForegroundWindow(hwnd_foreground);
			SetFocus(hwnd_focus);

			::SetCursorPos(cur_pos.x, cur_pos.y);

			_mutex.Unlock();


			// record did*/prevaction
			int sym_br = (int) p_symbols->sym()->br;

			EnterCriticalSection(&cs_autoplayer);
				switch (do_click)
				{
					case 4:  // allin
						_prevaction = PREVACT_ALLI;
						write_logautoplay("ALLI");
						break;

					case 3:  // raise
						_didrais[4] = p_symbols->sym()->didrais[4] + 1;
						_didrais[sym_br-1] = p_symbols->sym()->didrais[sym_br-1] + 1;
						_prevaction = PREVACT_RAIS;
						write_logautoplay("RAIS");
						break;

					case 2:  // call
						_didcall[4] = p_symbols->sym()->didcall[4] + 1;
						_didcall[sym_br-1] = p_symbols->sym()->didcall[sym_br-1] + 1;
						_prevaction = PREVACT_CALL;
						write_logautoplay("CALL");
						break;

					case 1:  // check
						_didchec[4] = p_symbols->sym()->didchec[4] + 1;
						_didchec[sym_br-1] = p_symbols->sym()->didchec[sym_br-1] + 1;
						_prevaction = PREVACT_CHEC;
						write_logautoplay("CHEC");
						break;

					case 0:  // fold
						_prevaction = PREVACT_FOLD;
						write_logautoplay("FOLD");
						break;
				}
			LeaveCriticalSection(&cs_autoplayer);

			p_symbols->UpdateAutoplayerInfo();

			// reset elapsedauto symbol
			time_t my_time_t;
			time(&my_time_t);
			p_symbols->set_elapsedautohold(my_time_t);

			p_global->set_replay_recorded_this_turn(false);
		}
	}

	__SEH_LOGFATAL("CAutoplayer::DoARCCF :\n");

}

void CAutoplayer::DoSlider(void) 
{
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
	HWND			hwnd_focus = GetFocus();
	HWND			hwnd_foreground = GetForegroundWindow();
	HWND			hwnd_active = GetActiveWindow();
	POINT			cur_pos;
	Stablemap_region handle, slider;

	int				e = SUCCESS;
	bool			sym_ismyturn = (bool) p_symbols->sym()->ismyturn;
	double			alli = p_symbols->f$alli();

	::GetCursorPos(&cur_pos);

	do_drag = -1;

	// ALLIN
	if (sym_ismyturn && p_global->trans.map.r$iXslider_index[3]!=-1 && p_global->trans.map.r$iXhandle_index[3]!=-1 &&
		p_scraper->handle_found_at_xy())
	{
		handle = p_global->trans.map.r$[p_global->trans.map.r$iXhandle_index[3]];
		slider = p_global->trans.map.r$[p_global->trans.map.r$iXslider_index[3]];
		x = p_scraper->handle_xy().x;
		y = p_scraper->handle_xy().y;
		x2 = slider.right-slider.left;
		pt = RandomizeClickLocation(x, y, x + (handle.right-handle.left), handle.bottom);
		pt2.x = pt.x+x2;
		pt2.y = pt.y;
		do_drag = 1;
	}


	if (do_drag=1)
	{

		// Translate click point to screen/mouse coords
		ClientToScreen(p_global->attached_hwnd(), &pt);
		fx = pt.x*(65535.0f/fScreenWidth);
		fy = pt.y*(65535.0f/fScreenHeight);
		ClientToScreen(p_global->attached_hwnd(), &pt2);
		fx2 = pt2.x*(65535.0f/fScreenWidth);
		fy2 = pt2.y*(65535.0f/fScreenHeight);
		write_log("*** Jam from %d,%d to %d,%d \n", fx, fy, fx2, fy2);

		if (_mutex.Lock(500))
		{
			SetFocus(p_global->attached_hwnd());
			SetForegroundWindow(p_global->attached_hwnd());
			SetActiveWindow(p_global->attached_hwnd());

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

			_mutex.Unlock();
		}

		if (alli && _alli_but!=-1)
		{
			pt = RandomizeClickLocation(p_global->trans.map.r$[_alli_but].left, p_global->trans.map.r$[_alli_but].top,
										  p_global->trans.map.r$[_alli_but].right, p_global->trans.map.r$[_alli_but].bottom);
			ClientToScreen(p_global->attached_hwnd(), &pt);
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
		if (_mutex.Lock(500))
		{
			SetFocus(p_global->attached_hwnd());
			SetForegroundWindow(p_global->attached_hwnd());
			SetActiveWindow(p_global->attached_hwnd());

			SendInput(input_count, input, sizeof(INPUT));

			SetActiveWindow(hwnd_active);
			SetForegroundWindow(hwnd_foreground);
			SetFocus(hwnd_focus);

			::SetCursorPos(cur_pos.x, cur_pos.y);

			_mutex.Unlock();

			write_logautoplay("JAM");

			write_log("*** Jam complete \n", fx, fy, fx2, fy2);

			// reset elapsedauto symbol
			time_t my_time_t;
			time(&my_time_t);
			p_symbols->set_elapsedautohold(my_time_t);

			Sleep(1200);
		}
	}

	__SEH_LOGFATAL("CAutoplayer::DoARCCF :\n");

}

void CAutoplayer::DoPrefold(void) 
{
	__SEH_HEADER
	INPUT			input[100] = {0};
	POINT			pt;
	double			fScreenWidth = ::GetSystemMetrics( SM_CXSCREEN )-1;
	double			fScreenHeight = ::GetSystemMetrics( SM_CYSCREEN )-1;
	double			fx, fy;
	HWND			hwnd_focus = GetFocus();
	HWND			hwnd_foreground = GetForegroundWindow();
	HWND			hwnd_active = GetActiveWindow();
	POINT			cur_pos;
	int				input_count = 0;

	double			prefold = p_symbols->f$prefold();

	::GetCursorPos(&cur_pos);

	if (prefold == 0)  
		return;

	if (_pre_fold_but == -1)  
		return;

	// Randomize click location
	pt = RandomizeClickLocation(p_global->trans.map.r$[_pre_fold_but].left, p_global->trans.map.r$[_pre_fold_but].top,
								  p_global->trans.map.r$[_pre_fold_but].right, p_global->trans.map.r$[_pre_fold_but].bottom);


	input_count = 0;

	// Translate click point to screen/mouse coords
	ClientToScreen(p_global->attached_hwnd(), &pt);
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
	if (_mutex.Lock(500))
	{
		SetFocus(p_global->attached_hwnd());
		SetForegroundWindow(p_global->attached_hwnd());
		SetActiveWindow(p_global->attached_hwnd());

		SendInput(input_count, input, sizeof(INPUT));

		SetActiveWindow(hwnd_active);
		SetForegroundWindow(hwnd_foreground);
		SetFocus(hwnd_focus);

		::SetCursorPos(cur_pos.x, cur_pos.y);

		_mutex.Unlock();

		// reset elapsedauto symbol
		time_t my_time_t;
		time(&my_time_t);
		p_symbols->set_elapsedautohold(my_time_t);

		// set prevaction
		EnterCriticalSection(&cs_autoplayer);
			_prevaction = PREVACT_FOLD;
		LeaveCriticalSection(&cs_autoplayer);

		p_symbols->UpdateAutoplayerInfo();

		write_logautoplay("FOLD");
	}

	__SEH_LOGFATAL("CAutoplayer::DoPrefold :\n");

}

const int CAutoplayer::CountSameScrape(void) 
{
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
	for (i=0; i<=4; i++)
	{
		if (p_scraper->card_common(i) != card_common_last[i])  same_scrape = false;
	}

	for (i=0; i<=9; i++)
	{
		if (p_scraper->card_player(i, 0) != card_player_last[i][0])  same_scrape = false;
		if (p_scraper->card_player(i, 1) != card_player_last[i][1])  same_scrape = false;
		if (p_scraper->dealer(i) != dealer_last[i])  same_scrape = false;
		if (p_scraper->player_balance(i) != playerbalance_last[i])  same_scrape = false;
		if (p_scraper->player_bet(i) != playerbet_last[i])  same_scrape = false;
	}

	int e = SUCCESS;
	int sym_myturnbits = (int) p_symbols->sym()->myturnbits;

	if (sym_myturnbits != myturnbitslast)  same_scrape = false;

	if (same_scrape)
	{
		num_same_scrapes++;
	}
	else
	{
		for (i=0; i<5; i++)
		{
			card_common_last[i] = p_scraper->card_common(i);
		}
		for (i=0; i<10; i++)
		{
			card_player_last[i][0] = p_scraper->card_player(i, 0);
			card_player_last[i][1] = p_scraper->card_player(i, 1);
			dealer_last[i] = p_scraper->dealer(i);
			playerbalance_last[i] = p_scraper->player_balance(i);
			playerbet_last[i] = p_scraper->player_bet(i);
		}
		myturnbitslast = sym_myturnbits;
		num_same_scrapes = 0;
	}

	return num_same_scrapes;

	__SEH_LOGFATAL("CAutoplayer::CountSameScrape :\n");
}


int CAutoplayer::GetR$ButtonIndices(void)
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
		if (p_scraper->GetButtonState(i) && p_scraper->IsStringAllin(p_scraper->button_label(i)))
		{
			button_index = i;
			i = 10;
			num_seen++;
		}
	}
	// scraper can't find allin button
	if (button_index == -1)
	{
		_alli_but = -1;
	}
	else
	{
		// find allin button region from profile
		_alli_but = -1;
		s.Format("i%dbutton", button_index);
		for (i=0; i<=9; i++)
		{
			r$index = p_global->trans.map.r$iXbutton_index[i];
			if (r$index!=-1)
			{
				if (p_global->trans.map.r$[r$index].name == s)
				{
					_alli_but = r$index;
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
		if (p_scraper->GetButtonState(i) && p_scraper->IsStringRaise(p_scraper->button_label(i)))
		{
			button_index = i;
			i = 10;
			num_seen++;
		}
	}
	// scraper can't find raise button
	if (button_index == -1)
	{
		_rais_but = -1;
	}
	else
	{
		// find raise button region from profile
		_rais_but = -1;
		s.Format("i%dbutton", button_index);
		for (i=0; i<=9; i++)
		{
			r$index = p_global->trans.map.r$iXbutton_index[i];
			if (r$index!=-1)
			{
				if (p_global->trans.map.r$[r$index].name == s)
				{
					_rais_but = r$index;
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
		if (p_scraper->GetButtonState(i) && p_scraper->IsStringCall(p_scraper->button_label(i)))
		{
			button_index = i;
			i = 10;
			num_seen++;
		}
	}
	// scraper can't find call button
	if (button_index == -1)
	{
		_call_but = -1;
	}
	else
	{
		// find call button region from profile
		_call_but = -1;
		s.Format("i%dbutton", button_index);
		for (i=0; i<=9; i++)
		{
			r$index = p_global->trans.map.r$iXbutton_index[i];
			if (r$index!=-1)
			{
				if (p_global->trans.map.r$[r$index].name == s)
				{
					_call_but = r$index;
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
		if (p_scraper->GetButtonState(i) && p_scraper->IsStringCheck(p_scraper->button_label(i)))
		{
			button_index = i;
			i = 10;
			num_seen++;
		}
	}
	// scraper can't find check button
	if (button_index == -1)
	{
		_chec_but = -1;
	}
	else
	{
		// find check button region from profile
		_chec_but = -1;
		s.Format("i%dbutton", button_index);
		for (i=0; i<=9; i++)
		{
			r$index = p_global->trans.map.r$iXbutton_index[i];
			if (r$index!=-1)
			{
				if (p_global->trans.map.r$[r$index].name == s)
				{
					_chec_but = r$index;
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
		if (p_scraper->GetButtonState(i) && p_scraper->IsStringFold(p_scraper->button_label(i)))
		{
			button_index = i;
			i = 10;
			num_seen++;
		}
	}
	// scraper can't find fold button
	if (button_index == -1)
	{
		_fold_but = -1;
	}
	else
	{
		// find fold button region from profile
		_fold_but = -1;
		s.Format("i%dbutton", button_index);
		for (i=0; i<=9; i++)
		{
			r$index = p_global->trans.map.r$iXbutton_index[i];
			if (r$index!=-1)
			{
				if (p_global->trans.map.r$[r$index].name == s)
				{
					_fold_but = r$index;
					i = 10;
				}
			}
		}
	}

	//////////////////////////////////////////////////////////
	// find AUTOPOST button region from scraper
	button_index = -1;
	_autopost_state = true;
	for (i=0; i<=9; i++)
	{
		if (p_scraper->IsStringAutopost(p_scraper->button_label(i)))
		{
			_autopost_state = p_scraper->GetButtonState(i);
			button_index = i;
			i = 10;
		}
	}
	// scraper can't find autopost button
	if (button_index == -1)
	{
		_autopost_but = -1;
	}
	else
	{
		// find autopost button region from profile
		_autopost_but = -1;
		s.Format("i%dbutton", button_index);
		for (i=0; i<=9; i++)
		{
			r$index = p_global->trans.map.r$iXbutton_index[i];
			if (r$index!=-1)
			{
				if (p_global->trans.map.r$[r$index].name == s)
				{
					_autopost_but = r$index;
					i = 10;
				}
			}
		}
	}

	//////////////////////////////////////////////////////////
	// find SITIN button region from scraper
	button_index = -1;
	_sitin_state = true;
	for (i=0; i<=9; i++)
	{
		if (p_scraper->IsStringSitin(p_scraper->button_label(i)))
		{
			_sitin_state = p_scraper->GetButtonState(i);
			button_index = i;
			i = 10;
		}
	}
	// scraper can't find sitin button
	if (button_index == -1)
	{
		_sitin_but = -1;
	}
	else
	{
		// find sitin button region from profile
		_sitin_but = -1;
		s.Format("i%dbutton", button_index);
		for (i=0; i<=9; i++)
		{
			r$index = p_global->trans.map.r$iXbutton_index[i];
			if (r$index!=-1)
			{
				if (p_global->trans.map.r$[r$index].name == s)
				{
					_sitin_but = r$index;
					i = 10;
				}
			}
		}
	}

	//////////////////////////////////////////////////////////
	// find SITOUT button region from scraper
	button_index = -1;
	_sitout_state = false;
	for (i=0; i<=9; i++)
	{
		if (p_scraper->IsStringSitout(p_scraper->button_label(i)))
		{
			_sitout_state = p_scraper->GetButtonState(i);
			button_index = i;
			i = 10;
		}
	}
	// scraper can't find sitout button
	if (button_index == -1)
	{
		_sitout_but = -1;
	}
	else
	{
		// find sitout button region from profile
		_sitout_but = -1;
		s.Format("i%dbutton", button_index);
		for (i=0; i<=9; i++)
		{
			r$index = p_global->trans.map.r$iXbutton_index[i];
			if (r$index!=-1)
			{
				if (p_global->trans.map.r$[r$index].name == s)
				{
					_sitout_but = r$index;
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
		if (p_scraper->GetButtonState(i) && p_scraper->IsStringLeave(p_scraper->button_label(i)))
		{
			button_index = i;
			i = 10;
		}
	}
	// scraper can't find leave button
	if (button_index == -1)
	{
		_leave_but = -1;
	}
	else
	{
		// find leave button region from profile
		_leave_but = -1;
		s.Format("i%dbutton", button_index);
		for (i=0; i<=9; i++)
		{
			r$index = p_global->trans.map.r$iXbutton_index[i];
			if (r$index!=-1)
			{
				if (p_global->trans.map.r$[r$index].name == s)
				{
					_leave_but = r$index;
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
		if (p_scraper->GetButtonState(i) && p_scraper->IsStringPrefold(p_scraper->button_label(i)))
		{
			button_index = i;
			i = 10;
		}
	}
	// scraper can't find prefold button
	if (button_index == -1)
	{
		_pre_fold_but = -1;
	}
	else
	{
		// find leave button region from profile
		_pre_fold_but = -1;
		s.Format("i%dbutton", button_index);
		for (i=0; i<=9; i++)
		{
			r$index = p_global->trans.map.r$iXbutton_index[i];
			if (r$index!=-1)
			{
				if (p_global->trans.map.r$[r$index].name == s)
				{
					_pre_fold_but = r$index;
					i = 10;
				}
			}
		}
	}

	//////////////////////////////////////////////////////////
	// find i86 button region from scraper
	_i86_but = -1;
	_i86_state = false;
	if (p_scraper->GetButtonState(86))
	{
		_i86_but = p_global->trans.map.r$i86button_index;
		_i86_state = true;
	}

	//////////////////////////////////////////////////////////
	// find i86 button region from scraper
	for (i=0; i<=9; i++)
	{
		_i86X_but[i] = -1;
		_i86X_state[i] = false;
		if (p_scraper->GetButtonState(860+i))
		{
			_i86X_but[i] = p_global->trans.map.r$i86Xbutton_index[i];
			_i86X_state[i] = true;
		}
	}

	return num_seen;

	__SEH_LOGFATAL("CAutoplayer::GetR$ButtonIndices :\n");
}

void CAutoplayer::CheckBringKeyboard(void) 
{
	__SEH_HEADER
	
	HMENU			bringsysmenu;
	MENUITEMINFO	mii;
	int				input_count, i;
	INPUT			input[100] = {0};
	HWND			hwnd_focus = GetFocus();
	HWND			hwnd_foreground = GetForegroundWindow();
	HWND			hwnd_active = GetActiveWindow();
	POINT			cur_pos;
	char			temp[256];
	CString			c_text;
	int				keybd_item_pos;

	int				e = SUCCESS;
	bool			sym_isbring = (bool) p_symbols->sym()->isbring;

	GetCursorPos(&cur_pos);

	// Find position of "Keyboard" item on system menu
	bringsysmenu = GetSystemMenu(p_global->attached_hwnd(), false);

	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STRING;
	mii.fType = MFT_STRING;
	mii.dwTypeData = temp;
	keybd_item_pos = -1;
	for (i=GetMenuItemCount(bringsysmenu)-1; i>=0; i--) 
	{
		mii.cch = 256;
	
		// Get the text of this menu item
		GetMenuItemInfo(bringsysmenu, i, true, &mii);
		c_text = temp;

		// See if this is the "keyboard" menu item
		if (c_text.MakeLower().Find("keyboard") != -1) 
		{
			keybd_item_pos = i;
			continue;
		}
	}

	// Get state of keyboard menu item
	if (keybd_item_pos == -1) 
	{
		return;
	}
	else 
	{
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fMask = MIIM_STATE;
		GetMenuItemInfo(bringsysmenu, keybd_item_pos, true, &mii);
	}

	if (!(mii.fState&MFS_CHECKED) && sym_isbring) 
	{

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

		if (_mutex.Lock(500)) 
		{
			SetFocus(p_global->attached_hwnd());
			SetForegroundWindow(p_global->attached_hwnd());
			SetActiveWindow(p_global->attached_hwnd());
			SendInput(input_count, input, sizeof(INPUT));
			_mutex.Unlock();
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

		if (_mutex.Lock(500)) 
		{
			SetFocus(p_global->attached_hwnd());
			SetForegroundWindow(p_global->attached_hwnd());
			SetActiveWindow(p_global->attached_hwnd());
			SendInput(input_count, input, sizeof(INPUT));
			SetActiveWindow(hwnd_active);
			SetForegroundWindow(hwnd_foreground);
			SetFocus(hwnd_focus);
			SetCursorPos(cur_pos.x, cur_pos.y);
			_mutex.Unlock();
		}
	}

	__SEH_LOGFATAL("CAutoplayer::CheckBringKeyboard :\n");

}

void CAutoplayer::DoF$play(void) 
{
	__SEH_HEADER

	INPUT			input[100] = {0};
	POINT			pt;
	double			fScreenWidth = ::GetSystemMetrics( SM_CXSCREEN )-1;
	double			fScreenHeight = ::GetSystemMetrics( SM_CYSCREEN )-1;
	double			fx, fy;
	bool			do_click;
	int				input_count = 0;
	HWND			hwnd_focus = GetFocus();
	HWND			hwnd_foreground = GetForegroundWindow();
	HWND			hwnd_active = GetActiveWindow();
	POINT			cur_pos;

	double			f_play = p_symbols->f$play();

	::GetCursorPos(&cur_pos);

	do_click = false;

	// leave table
	if (f_play==-2 && _leave_but!=-1)
	{
		pt = RandomizeClickLocation(p_global->trans.map.r$[_leave_but].left, p_global->trans.map.r$[_leave_but].top,
									  p_global->trans.map.r$[_leave_but].right, p_global->trans.map.r$[_leave_but].bottom);

		input_count = 0;

		// Translate click point to screen/mouse coords
		ClientToScreen(p_global->attached_hwnd(), &pt);
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
	else if (f_play==-1)
	{
	}

	// sit out
	else if (f_play==0 && ((_sitout_but!=-1 && _sitout_state==false) || (_sitin_but!=-1 && _sitin_state==true)))
	{

		if (_sitout_but!=-1 && _sitout_state==false)
		{
			pt = RandomizeClickLocation(p_global->trans.map.r$[_sitout_but].left, p_global->trans.map.r$[_sitout_but].top,
										  p_global->trans.map.r$[_sitout_but].right, p_global->trans.map.r$[_sitout_but].bottom);
		}
		else if (_sitin_but!=-1 && _sitin_state==true)
		{
			pt = RandomizeClickLocation(p_global->trans.map.r$[_sitin_but].left, p_global->trans.map.r$[_sitin_but].top,
										  p_global->trans.map.r$[_sitin_but].right, p_global->trans.map.r$[_sitin_but].bottom);
		}

		input_count = 0;

		// Translate click point to screen/mouse coords
		ClientToScreen(p_global->attached_hwnd(), &pt);
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
	else if (f_play==1 && ((_sitin_but!=-1 && _sitin_state==false) || (_sitout_but!=-1 && _sitout_state==true)))
	{
		if (_sitin_but!=-1 && _sitin_state==false)
		{
			pt = RandomizeClickLocation(p_global->trans.map.r$[_sitin_but].left, p_global->trans.map.r$[_sitin_but].top,
										  p_global->trans.map.r$[_sitin_but].right, p_global->trans.map.r$[_sitin_but].bottom);
		}
		else if (_sitout_but!=-1 && _sitout_state==true)
		{
			pt = RandomizeClickLocation(p_global->trans.map.r$[_sitout_but].left, p_global->trans.map.r$[_sitout_but].top,
										  p_global->trans.map.r$[_sitout_but].right, p_global->trans.map.r$[_sitout_but].bottom);
		}

		input_count = 0;

		// Translate click point to screen/mouse coords
		ClientToScreen(p_global->attached_hwnd(), &pt);
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
	if (f_play==1 && _autopost_but!=-1 && _autopost_state==false)
	{
		pt = RandomizeClickLocation(p_global->trans.map.r$[_autopost_but].left, p_global->trans.map.r$[_autopost_but].top,
									  p_global->trans.map.r$[_autopost_but].right, p_global->trans.map.r$[_autopost_but].bottom);

		// Translate click point to screen/mouse coords
		ClientToScreen(p_global->attached_hwnd(), &pt);
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
		if (_mutex.Lock(500)) 
		{
			SetFocus(p_global->attached_hwnd());
			SetForegroundWindow(p_global->attached_hwnd());
			SetActiveWindow(p_global->attached_hwnd());

			SendInput(input_count, input, sizeof(INPUT));

			SetActiveWindow(hwnd_active);
			SetForegroundWindow(hwnd_foreground);
			SetFocus(hwnd_focus);

			::SetCursorPos(cur_pos.x, cur_pos.y);

			_mutex.Unlock();

			// reset elapsedauto symbol
			time_t my_time_t;
			time(&my_time_t);
			p_symbols->set_elapsedautohold(my_time_t);
		}
	}

	__SEH_LOGFATAL("CAutoplayer::DoF$play :\n");
}

void CAutoplayer::DoI86(void) 
{
	__SEH_HEADER

	INPUT			input[100] = {0};
	POINT			pt;
	double			fScreenWidth = ::GetSystemMetrics( SM_CXSCREEN )-1;
	double			fScreenHeight = ::GetSystemMetrics( SM_CYSCREEN )-1;
	double			fx, fy;
	bool			do_click;
	int				input_count = 0;
	int				i;
	HWND			hwnd_focus = GetFocus();
	HWND			hwnd_foreground = GetForegroundWindow();
	HWND			hwnd_active = GetActiveWindow();
	POINT			cur_pos;

	::GetCursorPos(&cur_pos);

	do_click = false;
	if (_i86_but!=-1 && _i86_state)
	{
		pt = RandomizeClickLocation(p_global->trans.map.r$[_i86_but].left, p_global->trans.map.r$[_i86_but].top,
									  p_global->trans.map.r$[_i86_but].right, p_global->trans.map.r$[_i86_but].bottom);

		input_count = 0;

		// Translate click point to screen/mouse coords
		ClientToScreen(p_global->attached_hwnd(), &pt);
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
			if (_i86X_but[i]!=-1 && _i86X_state[i])
			{
				pt = RandomizeClickLocation(p_global->trans.map.r$[_i86X_but[i]].left, p_global->trans.map.r$[_i86X_but[i]].top,
											  p_global->trans.map.r$[_i86X_but[i]].right, p_global->trans.map.r$[_i86X_but[i]].bottom);

				input_count = 0;

				// Translate click point to screen/mouse coords
				ClientToScreen(p_global->attached_hwnd(), &pt);
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
		if (_mutex.Lock(500))
		{
			SetFocus(p_global->attached_hwnd());
			SetForegroundWindow(p_global->attached_hwnd());
			SetActiveWindow(p_global->attached_hwnd());

			SendInput(input_count, input, sizeof(INPUT));

			SetActiveWindow(hwnd_active);
			SetForegroundWindow(hwnd_foreground);
			SetFocus(hwnd_focus);

			::SetCursorPos(cur_pos.x, cur_pos.y);

			_mutex.Unlock();

			// reset elapsedauto symbol
			time_t my_time_t;
			time(&my_time_t);
			p_symbols->set_elapsedautohold(my_time_t);
		}
	}

	__SEH_LOGFATAL("CAutoplayer::DoI86 :\n");
}

const POINT CAutoplayer::RandomizeClickLocation(const int left, const int top, const int right, const int bottom) 
{
	__SEH_HEADER

	POINT p;

	// uniform random distribution, yuck!
	//p.x = ((double) rand() / (double) RAND_MAX) * (right-left) + left;
	//p.y = ((double) rand() / (double) RAND_MAX) * (bottom-top) + top;

	// normal random distribution - much better!
	GetClickPoint(left + (right-left)/2, top + (bottom-top)/2, (right-left)/2, (bottom-top)/2, &p);

	return p;

	__SEH_LOGFATAL("CAutoplayer::RandomizeClickLocation :\n");
}

const void CAutoplayer::GetClickPoint(const int x, const int y, const int rx, const int ry, POINT *p) 
{
	__SEH_HEADER

	p->x = x + (int) (RandomNormalScaled(2*rx, 0, 1) + 0.5) - (rx);
	p->y = y + (int) (RandomNormalScaled(2*ry, 0, 1) + 0.5) - (ry);

	__SEH_LOGFATAL("CAutoplayer::GetClickPoint :\n");
}

// random number - 0 -> scale, with normal distribution
// ignore results outside 3.5 stds from the mean
const double CAutoplayer::RandomNormalScaled(const double scale, const double m, const double s) 
{
	__SEH_HEADER

	double res = -99;
	while (res < -3.5 || res > 3.5) res = RandomNormal(m, s);
	return (res / 3.5*s + 1) * (scale / 2.0);

	__SEH_LOGFATAL("CAutoplayer::RandomNormalScaled :\n");

}

const double CAutoplayer::RandomNormal(const double m, const double s) 
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

	__SEH_LOGFATAL("CAutoplayer::RandomNormal :\n");

}
