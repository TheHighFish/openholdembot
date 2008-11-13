#include "StdAfx.h"
#include <complex>

#include "CAutoplayer.h"
#include "CSymbols.h"
#include "CIteratorThread.h"
#include "CHeartbeatThread.h"
#include "CScraper.h"
#include "CGrammar.h"
#include "CPreferences.h"

#include "OpenHoldem.h"
#include "MainFrm.h"
#include "CGameState.h"

#include "PokerChat.hpp"

CAutoplayer	*p_autoplayer = NULL;

CAutoplayer::CAutoplayer(BOOL bInitiallyOwn, LPCTSTR lpszName) : _mutex(bInitiallyOwn, lpszName)
{
	ASSERT(_mutex.m_hObject != NULL);

	// Seed RNG
	//srand((unsigned)time( NULL ));

	set_autoplayer_engaged(false);
	ResetHand();
}

CAutoplayer::~CAutoplayer(void) 
{
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
	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);
	RMapCI			r_iter = p_tablemap->r$()->find("chatbox");

	if (r_iter == p_tablemap->r$()->end())
		return;

	GetCursorPos(&cur_pos);

	pt = RandomizeClickLocation(r_iter->second.left, r_iter->second.top, r_iter->second.right, r_iter->second.bottom);

	// Translate click point to screen/mouse coords
	ClientToScreen(pMyMainWnd->attached_hwnd(), &pt);
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
		SetFocus(pMyMainWnd->attached_hwnd());
		SetForegroundWindow(pMyMainWnd->attached_hwnd());
		SetActiveWindow(pMyMainWnd->attached_hwnd());
		SendInput(2, input, sizeof(INPUT));
		//
		//  Pre: f$chat > 0,
		//   Chatbox selected by a mouse click.
		//
		//  We can now "type in" the message, if there's one.
		//
		SendChatMessageToKeyboard();

		//
		//  Restore window state
		SetActiveWindow(hwnd_active);
		SetForegroundWindow(hwnd_foreground);
		SetFocus(hwnd_focus);
		SetCursorPos(cur_pos.x, cur_pos.y);

		_mutex.Unlock();
	}
}

void CAutoplayer::DoAutoplayer(void) 
{
	int				x = 0;
	int				num_buttons_visible = 0;
	int				delay = 0;

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
	RegisterChatMessage(p_symbols->f$chat());

	//  Avoiding unnecessary calls to DoChat(),
	//	especially mouse movements to the chat box.
	if ((p_symbols->f$chat() != 0) && IsChatAllowed())
		DoChat();

	// Get count of stable frames for use a little bit further down
	x = CountSameScrape();

	// If iterator thread is still iterating, then return
	if (iter_vars.iterator_thread_running())
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
	delay = p_symbols->f$delay() / prefs.scrape_delay();	// scale f$delay to a number of scrapes

	if (x < (int) prefs.frame_delay() + delay)
	{
		// Calc primary formulas, but not with final answer, so main window can display correctly
		p_symbols->CalcPrimaryFormulas(false);

		return;
	}

	// If the game state processor didn't process this frame, then we should not act.
	if (!p_game_state->ProcessThisFrame ())
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
}

void CAutoplayer::ResetHand(void) 
{
	for (int i=0; i<=4; i++)
	{
		set_didchec(i,0);
		set_didcall(i,0);
		set_didrais(i,0);
		set_didswag(i,0);
	}

	set_prevaction(-1);
}

void CAutoplayer::ResetRound(void) 
{
	set_didchec(4,0);
	set_didcall(4,0);
	set_didrais(4,0);
	set_didswag(4,0);
}

void CAutoplayer::DoSwag(void) 
{
	int				input_count = 0;
	POINT			pt = {0};
	double			fScreenWidth = ::GetSystemMetrics( SM_CXSCREEN )-1;
	double			fScreenHeight = ::GetSystemMetrics( SM_CYSCREEN )-1;
	double			fx = 0., fy = 0.;
	INPUT			input[100] = {0};
	char			ch_str[100] = {0};
	int				i = 0;
	int				vkey = 0;
	HWND			hwnd_focus = GetFocus();
	HWND			hwnd_foreground = GetForegroundWindow();
	HWND			hwnd_active = GetActiveWindow();
	POINT			cur_pos = {0};
	bool			lost_focus = false;
	int				e = SUCCESS;
	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);
	double			f_swag = p_symbols->f$swag();
	RMapCI			r_edit = p_tablemap->r$()->find("i3edit");
	RMapCI			r_button = p_tablemap->r$()->find("i3button");

	
	::GetCursorPos(&cur_pos);

	// swag buttons are hard coded as #3 now.  Can they be different?
	if (p_scraper->GetButtonState(3) && r_edit!=p_tablemap->r$()->end())
	{

		// If we get a lock, do the action
		if (_mutex.Lock(500))
		{

			// TEXT SELECTION
			if (p_tablemap->swagselectionmethod() == TEXTSEL_DOUBLECLICK)
			{
				input_count = 0;

				// Double click in edit box
				pt = RandomizeClickLocation(r_edit->second.left, r_edit->second.top, 
											r_edit->second.right, r_edit->second.bottom);
				ClientToScreen(pMyMainWnd->attached_hwnd(), &pt);
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

			if (p_tablemap->swagselectionmethod() == TEXTSEL_SINGLECLICK)
			{
				input_count = 0;

				// Single click in edit box
				pt = RandomizeClickLocation(r_edit->second.left, r_edit->second.top, 
											r_edit->second.right, r_edit->second.bottom);
				ClientToScreen(pMyMainWnd->attached_hwnd(), &pt);
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
			}

			else if (p_tablemap->swagselectionmethod() == TEXTSEL_CLICKDRAG)
			{
				input_count = 0;

				// left click, drag to left, un-left click
				pt.x = r_edit->second.right;
				pt.y = r_edit->second.top + (r_edit->second.bottom - r_edit->second.top)/2;
				ClientToScreen(pMyMainWnd->attached_hwnd(), &pt);
				fx = pt.x*(65535.0f/fScreenWidth);
				fy = pt.y*(65535.0f/fScreenHeight);

				ZeroMemory(&input[input_count],sizeof(INPUT));
				input[input_count].type = INPUT_MOUSE;
				input[input_count].mi.dx = fx;
				input[input_count].mi.dy = fy;
				input[input_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;
				input_count++;

				pt.x = r_edit->second.left;
				pt.y = r_edit->second.top + (r_edit->second.bottom - r_edit->second.top)/2;
				ClientToScreen(pMyMainWnd->attached_hwnd(), &pt);
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
			SetFocus(pMyMainWnd->attached_hwnd());
			SetForegroundWindow(pMyMainWnd->attached_hwnd());
			SetActiveWindow(pMyMainWnd->attached_hwnd());

			SendInput(input_count, input, sizeof(INPUT));

			// Check for stolen focus, and thus misswag
			if (GetForegroundWindow() != pMyMainWnd->attached_hwnd())
				lost_focus = true;

			::SetCursorPos(cur_pos.x, cur_pos.y);

			Sleep(prefs.swag_delay_1());



			// TEXT DELETION
			if (p_tablemap->swagdeletionmethod() == TEXTDEL_DELETE)
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

			else if (p_tablemap->swagdeletionmethod() == TEXTDEL_BACKSPACE)
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
			SetFocus(pMyMainWnd->attached_hwnd());
			SetForegroundWindow(pMyMainWnd->attached_hwnd());
			SetActiveWindow(pMyMainWnd->attached_hwnd());

			SendInput(input_count, input, sizeof(INPUT));

			// Check for stolen focus, and thus misswag
			if (GetForegroundWindow() != pMyMainWnd->attached_hwnd())
				lost_focus = true;

			::SetCursorPos(cur_pos.x, cur_pos.y);

			Sleep(prefs.swag_delay_2());



			// SWAG AMOUNT ENTRY
			input_count = 0;

			// Click in edit box
			pt = RandomizeClickLocation(r_edit->second.left, r_edit->second.top, 
										r_edit->second.right, r_edit->second.bottom);

			ClientToScreen(pMyMainWnd->attached_hwnd(), &pt);
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
				sprintf_s(ch_str, 100, "%.2f", f_swag);
			}
			else
			{
				sprintf_s(ch_str, 100, "%.0f", f_swag);
			}

			for (i=0; i<(int) strlen(ch_str); i++)
			{
				if (ch_str[i]>='0' && ch_str[i]<='9')
				{
					vkey = ch_str[i];
				}
				if (ch_str[i]=='.')
				{
					if (prefs.swag_use_comma())
					{
						vkey = VK_OEM_COMMA;
					}
					else
					{
						vkey = VK_DECIMAL;
					}
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

			// do it and sleep for prefs.swag_delay (ms)
			SetFocus(pMyMainWnd->attached_hwnd());
			SetForegroundWindow(pMyMainWnd->attached_hwnd());
			SetActiveWindow(pMyMainWnd->attached_hwnd());

			SendInput(input_count, input, sizeof(INPUT));

			// Check for stolen focus, and thus misswag
			if (GetForegroundWindow() != pMyMainWnd->attached_hwnd())
				lost_focus = true;

			::SetCursorPos(cur_pos.x, cur_pos.y);

			Sleep(prefs.swag_delay_3());



			// BET CONFIRMATION ACTION
			if (p_tablemap->swagconfirmationmethod() == BETCONF_ENTER)
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

			else if (p_tablemap->swagconfirmationmethod() == BETCONF_CLICKBET &&
					 (_rais_but!=p_tablemap->r$()->end() || r_button!=p_tablemap->r$()->end()) )
			{
				input_count = 0;

				// Click on bet/raise button
				// use i3button region it it exists, otherwise use the bet/raise button region
				if (r_button!=p_tablemap->r$()->end())
					pt = RandomizeClickLocation(r_button->second.left, r_button->second.top, 
												r_button->second.right, r_button->second.bottom);

				else
					pt = RandomizeClickLocation(_rais_but->second.left, _rais_but->second.top, 
												_rais_but->second.right, _rais_but->second.bottom);

				// Click on button
				ClientToScreen(pMyMainWnd->attached_hwnd(), &pt);
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
				if (p_tablemap->buttonclickmethod() == BUTTON_DOUBLECLICK)
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
			if (!lost_focus || !prefs.focus_detect())
			{
				SetFocus(pMyMainWnd->attached_hwnd());
				SetForegroundWindow(pMyMainWnd->attached_hwnd());
				SetActiveWindow(pMyMainWnd->attached_hwnd());

				SendInput(input_count, input, sizeof(INPUT));

				// record didswag/prevaction
				int sym_br = (int) p_symbols->sym()->br;

				set_didswag(4, p_symbols->sym()->didswag[4] + 1);
				set_didswag(sym_br-1, p_symbols->sym()->didswag[sym_br-1] + 1);
				set_prevaction(PREVACT_SWAG);

				p_symbols->UpdateAutoplayerInfo();

				// reset elapsedauto symbol
				time_t my_time_t;
				time(&my_time_t);
				p_symbols->set_elapsedautohold(my_time_t);

				p_heartbeat_thread->set_replay_recorded_this_turn(false);

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
}

void CAutoplayer::DoARCCF(void) 
{
	int				do_click = 0, input_count = 0;
	INPUT			input[100] = {0};
	POINT			pt = {0};
	double			fScreenWidth = ::GetSystemMetrics( SM_CXSCREEN )-1;
	double			fScreenHeight = ::GetSystemMetrics( SM_CYSCREEN )-1;
	double			fx = 0., fy = 0.;
	HWND			hwnd_focus = GetFocus();
	HWND			hwnd_foreground = GetForegroundWindow();
	HWND			hwnd_active = GetActiveWindow();
	POINT			cur_pos = {0};
	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

	int				e = SUCCESS;
	double			alli = p_symbols->f$alli();
	double			rais = p_symbols->f$rais();
	double			call = p_symbols->f$call();
	int				sym_myturnbits = (int) p_symbols->sym()->myturnbits;

	::GetCursorPos(&cur_pos);

	do_click = -1;

	// ALLIN
	if (alli && sym_myturnbits&0x8 && _alli_but!=p_tablemap->r$()->end())
	{
		pt = RandomizeClickLocation(_alli_but->second.left, _alli_but->second.top, 
									_alli_but->second.right,  _alli_but->second.bottom);
		do_click = 4;
	}

	// RAISE
	else if (rais && sym_myturnbits&0x4 && _rais_but!=p_tablemap->r$()->end())
	{
		pt = RandomizeClickLocation(_rais_but->second.left, _rais_but->second.top, 
									_rais_but->second.right,  _rais_but->second.bottom);
		do_click = 3;
	}

	// CALL
	else if (call && sym_myturnbits&0x1 && _call_but!=p_tablemap->r$()->end())
	{
		pt = RandomizeClickLocation(_call_but->second.left, _call_but->second.top, 
									_call_but->second.right, _call_but->second.bottom);
		do_click = 2;
	}

	// CHECK
	// None of f$alli, f$swag, f$rais, f$call are > 0 or no buttons related to
	// these actions can be found. If there is a check button, then click it.
	else if (_chec_but!=p_tablemap->r$()->end())
	{
		pt = RandomizeClickLocation(_chec_but->second.left, _chec_but->second.top, 
									_chec_but->second.right, _chec_but->second.bottom);
		do_click = 1;
	}

	// FOLD
	// None of f$alli, f$swag, f$rais, f$call are > 0 or no buttons related to
	// these actions can be found. If there is a fold button, then click it, otherwise we have a serious problem.
	else if (_fold_but!=p_tablemap->r$()->end())
	{
		pt = RandomizeClickLocation(_fold_but->second.left, _fold_but->second.top, 
									_fold_but->second.right, _fold_but->second.bottom);
		do_click = 0;
	}

	if (do_click>=0)
	{
		input_count = 0;

		// Translate click point to screen/mouse coords
		ClientToScreen(pMyMainWnd->attached_hwnd(), &pt);
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
		if (p_tablemap->buttonclickmethod() == BUTTON_DOUBLECLICK)
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
			SetFocus(pMyMainWnd->attached_hwnd());
			SetForegroundWindow(pMyMainWnd->attached_hwnd());
			SetActiveWindow(pMyMainWnd->attached_hwnd());

			SendInput(input_count, input, sizeof(INPUT));

			SetActiveWindow(hwnd_active);
			SetForegroundWindow(hwnd_foreground);
			SetFocus(hwnd_focus);

			::SetCursorPos(cur_pos.x, cur_pos.y);

			_mutex.Unlock();


			// record did*/prevaction
			int sym_br = (int) p_symbols->sym()->br;

			switch (do_click)
			{
				case 4:  // allin
					set_prevaction(PREVACT_ALLI);
					write_logautoplay("ALLI");
					break;

				case 3:  // raise
					set_didrais(4, p_symbols->sym()->didrais[4] + 1);
					set_didrais(sym_br-1, p_symbols->sym()->didrais[sym_br-1] + 1);
					set_prevaction(PREVACT_RAIS);
					write_logautoplay("RAIS");
					break;

				case 2:  // call
					set_didcall(4, p_symbols->sym()->didcall[4] + 1);
					set_didcall(sym_br-1, p_symbols->sym()->didcall[sym_br-1] + 1);
					set_prevaction(PREVACT_CALL);
					write_logautoplay("CALL");
					break;

				case 1:  // check
					set_didchec(4, p_symbols->sym()->didchec[4] + 1);
					set_didchec(sym_br-1, p_symbols->sym()->didchec[sym_br-1] + 1);
					set_prevaction(PREVACT_CHEC);
					write_logautoplay("CHEC");
					break;

				case 0:  // fold
					set_prevaction(PREVACT_FOLD);
					write_logautoplay("FOLD");
					break;
			}

			p_symbols->UpdateAutoplayerInfo();

			// reset elapsedauto symbol
			time_t my_time_t;
			time(&my_time_t);
			p_symbols->set_elapsedautohold(my_time_t);

			p_heartbeat_thread->set_replay_recorded_this_turn(false);
		}
	}
}

void CAutoplayer::DoSlider(void) 
{
	int				input_count = 0,  x = 0, y = 0, x2 = 0;
	INPUT			input[100] = {0};
	POINT			pt = {0}, pt2 = {0};
	double			fScreenWidth = ::GetSystemMetrics( SM_CXSCREEN )-1;
	double			fScreenHeight = ::GetSystemMetrics( SM_CYSCREEN )-1;
	double			fx = 0.;
	double			fy = 0.;
	double			fx2 = 0.;
	double			fy2 = 0.;
	HWND			hwnd_focus = GetFocus();
	HWND			hwnd_foreground = GetForegroundWindow();
	HWND			hwnd_active = GetActiveWindow();
	POINT			cur_pos = {0};
	int				e = SUCCESS;
	bool			sym_ismyturn = (bool) p_symbols->sym()->ismyturn;
	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);
	double			alli = p_symbols->f$alli();
	RMapCI			slider = p_tablemap->r$()->find("i3slider");
	RMapCI			handle = p_tablemap->r$()->find("i3handle");

	::GetCursorPos(&cur_pos);

	if (!sym_ismyturn)
		return;

	if (slider==p_tablemap->r$()->end() | handle==p_tablemap->r$()->end())
		return;

	if (!p_scraper->handle_found_at_xy())
		return;

	// Vars
	x = p_scraper->handle_xy().x;
	y = p_scraper->handle_xy().y;
	x2 = slider->second.right - slider->second.left;
	pt = RandomizeClickLocation(x, y, x + (handle->second.right - handle->second.left), handle->second.bottom);
	pt2.x = pt.x+x2;
	pt2.y = pt.y;

	// Translate click point to screen/mouse coords
	ClientToScreen(pMyMainWnd->attached_hwnd(), &pt);
	fx = pt.x*(65535.0f/fScreenWidth);
	fy = pt.y*(65535.0f/fScreenHeight);
	ClientToScreen(pMyMainWnd->attached_hwnd(), &pt2);
	fx2 = pt2.x*(65535.0f/fScreenWidth);
	fy2 = pt2.y*(65535.0f/fScreenHeight);
	write_log("*** Jam from %d,%d to %d,%d \n", fx, fy, fx2, fy2);

	if (_mutex.Lock(500))
	{
		SetFocus(pMyMainWnd->attached_hwnd());
		SetForegroundWindow(pMyMainWnd->attached_hwnd());
		SetActiveWindow(pMyMainWnd->attached_hwnd());

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

	if (alli && _alli_but!=p_tablemap->r$()->end())
	{
		pt = RandomizeClickLocation(_alli_but->second.left, _alli_but->second.top, 
									_alli_but->second.right, _alli_but->second.bottom);

		ClientToScreen(pMyMainWnd->attached_hwnd(), &pt);
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
		SetFocus(pMyMainWnd->attached_hwnd());
		SetForegroundWindow(pMyMainWnd->attached_hwnd());
		SetActiveWindow(pMyMainWnd->attached_hwnd());

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

void CAutoplayer::DoPrefold(void) 
{
	INPUT			input[100] = {0};
	POINT			pt = {0};
	double			fScreenWidth = ::GetSystemMetrics( SM_CXSCREEN )-1;
	double			fScreenHeight = ::GetSystemMetrics( SM_CYSCREEN )-1;
	double			fx = 0., fy = 0.;
	HWND			hwnd_focus = GetFocus();
	HWND			hwnd_foreground = GetForegroundWindow();
	HWND			hwnd_active = GetActiveWindow();
	POINT			cur_pos = {0};
	int				input_count = 0;
	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

	double			prefold = p_symbols->f$prefold();

	::GetCursorPos(&cur_pos);

	if (prefold == 0)  
		return;

	if (_pre_fold_but == p_tablemap->r$()->end())  
		return;

	// Randomize click location
	pt = RandomizeClickLocation(_pre_fold_but->second.left, _pre_fold_but->second.top, 
								_pre_fold_but->second.right,  _pre_fold_but->second.bottom);

	input_count = 0;

	// Translate click point to screen/mouse coords
	ClientToScreen(pMyMainWnd->attached_hwnd(), &pt);
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
		SetFocus(pMyMainWnd->attached_hwnd());
		SetForegroundWindow(pMyMainWnd->attached_hwnd());
		SetActiveWindow(pMyMainWnd->attached_hwnd());

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
		set_prevaction(PREVACT_FOLD);

		p_symbols->UpdateAutoplayerInfo();

		write_logautoplay("FOLD");
	}
}

const int CAutoplayer::CountSameScrape(void) 
{
	int						i = 0;
	static unsigned int		card_common_last[5] = {0};
	static unsigned int		card_player_last[10][2] = {0};
	static bool				dealer_last[10] = {0};
	static double			playerbalance_last[10] = {0};
	static double			playerbet_last[10] = {0};
	static double			myturnbitslast = 0;
	bool					same_scrape = false;
	static int				num_same_scrapes = 0;

	// These items need to be the same to count as a identical frame:
	// - up and down cards
	// - button position
	// - playerbets
	// - playerbalances
	// - button states
	same_scrape = true;
	for (i=0; i<=4; i++)
		if (p_scraper->card_common(i) != card_common_last[i])  
			same_scrape = false;

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
}


int CAutoplayer::GetR$ButtonIndices(void)
{
	int				i = 0;
	int				button_index = 0;
	CString			s = "";
	int				num_seen = 0;

	//////////////////////////////////////////////////////////
	// find ALLIN button region from scraper
	button_index = -1;
	for (i=0; i<=9 && button_index==-1; i++)
	{
		if (p_scraper->GetButtonState(i) && p_scraper->IsStringAllin(p_scraper->button_label(i)))
		{
			button_index = i;
			num_seen++;
		}
	}

	// find allin button region from table map
	s.Format("i%dbutton", button_index);
	_alli_but = p_tablemap->r$()->find(s.GetString());

	//////////////////////////////////////////////////////////
	// find RAISE button region from scraper
	button_index = -1;
	for (i=0; i<=9 && button_index==-1; i++)
	{
		if (p_scraper->GetButtonState(i) && p_scraper->IsStringRaise(p_scraper->button_label(i)))
		{
			button_index = i;
			num_seen++;
		}
	}

	// find rais button region from table map
	s.Format("i%dbutton", button_index);
	_rais_but = p_tablemap->r$()->find(s.GetString());

	//////////////////////////////////////////////////////////
	// find CALL button region from scraper
	button_index = -1;
	for (i=0; i<=9 && button_index==-1; i++)
	{
		if (p_scraper->GetButtonState(i) && p_scraper->IsStringCall(p_scraper->button_label(i)))
		{
			button_index = i;
			num_seen++;
		}
	}

	// find call button region from table map
	s.Format("i%dbutton", button_index);
	_call_but = p_tablemap->r$()->find(s.GetString());

	//////////////////////////////////////////////////////////
	// find CHECK button region from scraper
	button_index = -1;
	for (i=0; i<=9 && button_index==-1; i++)
	{
		if (p_scraper->GetButtonState(i) && p_scraper->IsStringCheck(p_scraper->button_label(i)))
		{
			button_index = i;
			num_seen++;
		}
	}

	// find chec button region from table map
	s.Format("i%dbutton", button_index);
	_chec_but = p_tablemap->r$()->find(s.GetString());

	//////////////////////////////////////////////////////////
	// find FOLD button region from scraper
	button_index = -1;
	for (i=0; i<=9 && button_index==-1; i++)
	{
		if (p_scraper->GetButtonState(i) && p_scraper->IsStringFold(p_scraper->button_label(i)))
		{
			button_index = i;
			num_seen++;
		}
	}

	// find fold button region from table map
	s.Format("i%dbutton", button_index);
	_fold_but = p_tablemap->r$()->find(s.GetString());

	//////////////////////////////////////////////////////////
	// find AUTOPOST button region from scraper
	button_index = -1;
	_autopost_state = true;
	for (i=0; i<=9 && button_index==-1; i++)
	{
		if (p_scraper->IsStringAutopost(p_scraper->button_label(i)))
		{
			_autopost_state = p_scraper->GetButtonState(i);
			button_index = i;
		}
	}

	// find autopost button region from table map
	s.Format("i%dbutton", button_index);
	_autopost_but = p_tablemap->r$()->find(s.GetString());

	//////////////////////////////////////////////////////////
	// find SITIN button region from scraper
	button_index = -1;
	_sitin_state = true;
	for (i=0; i<=9 && button_index==-1; i++)
	{
		if (p_scraper->IsStringSitin(p_scraper->button_label(i)))
		{
			_sitin_state = p_scraper->GetButtonState(i);
			button_index = i;
		}
	}

	// find sitin button region from table map
	s.Format("i%dbutton", button_index);
	_sitin_but = p_tablemap->r$()->find(s.GetString());

	//////////////////////////////////////////////////////////
	// find SITOUT button region from scraper
	button_index = -1;
	_sitout_state = false;
	for (i=0; i<=9 && button_index==-1; i++)
	{
		if (p_scraper->IsStringSitout(p_scraper->button_label(i)))
		{
			_sitout_state = p_scraper->GetButtonState(i);
			button_index = i;
		}
	}

	// find sitout button region from table map
	s.Format("i%dbutton", button_index);
	_sitout_but = p_tablemap->r$()->find(s.GetString());

	//////////////////////////////////////////////////////////
	// find LEAVE button region from scraper
	button_index = -1;
	for (i=0; i<=9 && button_index==-1; i++)
	{
		if (p_scraper->GetButtonState(i) && p_scraper->IsStringLeave(p_scraper->button_label(i)))
		{
			button_index = i;
		}
	}

	// find leave button region from table map
	s.Format("i%dbutton", button_index);
	_leave_but = p_tablemap->r$()->find(s.GetString());

	//////////////////////////////////////////////////////////
	// find PREFOLD button region from scraper
	button_index = -1;
	for (i=0; i<=9 && button_index==-1; i++)
	{
		if (p_scraper->GetButtonState(i) && p_scraper->IsStringPrefold(p_scraper->button_label(i)))
		{
			button_index = i;
		}
	}

	// find prefold button region from table map
	s.Format("i%dbutton", button_index);
	_pre_fold_but = p_tablemap->r$()->find(s.GetString());

	//////////////////////////////////////////////////////////
	// find i86 button region from scraper
	_i86_state = false;
	_i86_but = p_tablemap->r$()->end();
	if (p_scraper->GetButtonState(86))
	{
		_i86_but = p_tablemap->r$()->find("i86button");
		_i86_state = true;
	}

	//////////////////////////////////////////////////////////
	// find i86X button region from scraper
	for (i=0; i<=9; i++)
	{
		_i86X_state[i] = false;
		_i86X_but[i] = p_tablemap->r$()->end();
		if (p_scraper->GetButtonState(860+i))
		{
			s.Format("i86%dbutton", i);
			_i86X_but[i] = p_tablemap->r$()->find(s.GetString());
			_i86X_state[i] = true;
		}
	}

	return num_seen;
}

void CAutoplayer::CheckBringKeyboard(void) 
{
	HMENU			bringsysmenu = NULL;
	MENUITEMINFO	mii;
	int				input_count = 0, i = 0;
	INPUT			input[100] = {0};
	HWND			hwnd_focus = GetFocus();
	HWND			hwnd_foreground = GetForegroundWindow();
	HWND			hwnd_active = GetActiveWindow();
	POINT			cur_pos = {0};
	char			temp[256] = {0};
	CString			c_text = "";
	int				keybd_item_pos = 0;
	int				e = SUCCESS;
	bool			sym_isbring = (bool) p_symbols->sym()->isbring;
	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

	// Init locals
	memset(&mii, 0, sizeof(MENUITEMINFO));

	GetCursorPos(&cur_pos);

	// Find position of "Keyboard" item on system menu
	bringsysmenu = GetSystemMenu(pMyMainWnd->attached_hwnd(), false);

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
			SetFocus(pMyMainWnd->attached_hwnd());
			SetForegroundWindow(pMyMainWnd->attached_hwnd());
			SetActiveWindow(pMyMainWnd->attached_hwnd());
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
			SetFocus(pMyMainWnd->attached_hwnd());
			SetForegroundWindow(pMyMainWnd->attached_hwnd());
			SetActiveWindow(pMyMainWnd->attached_hwnd());
			SendInput(input_count, input, sizeof(INPUT));
			SetActiveWindow(hwnd_active);
			SetForegroundWindow(hwnd_foreground);
			SetFocus(hwnd_focus);
			SetCursorPos(cur_pos.x, cur_pos.y);
			_mutex.Unlock();
		}
	}
}

void CAutoplayer::DoF$play(void) 
{
	INPUT			input[100] = {0};
	POINT			pt = {0};
	double			fScreenWidth = ::GetSystemMetrics( SM_CXSCREEN )-1;
	double			fScreenHeight = ::GetSystemMetrics( SM_CYSCREEN )-1;
	double			fx = 0., fy = 0.;
	bool			do_click = false;
	int				input_count = 0;
	HWND			hwnd_focus = GetFocus();
	HWND			hwnd_foreground = GetForegroundWindow();
	HWND			hwnd_active = GetActiveWindow();
	POINT			cur_pos = {0};
	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

	double			f_play = p_symbols->f$play();

	::GetCursorPos(&cur_pos);

	do_click = false;

	// leave table
	if (f_play==-2 && _leave_but!=p_tablemap->r$()->end())
	{
		pt = RandomizeClickLocation(_leave_but->second.left, _leave_but->second.top, 
									_leave_but->second.right,  _leave_but->second.bottom);

		input_count = 0;

		// Translate click point to screen/mouse coords
		ClientToScreen(pMyMainWnd->attached_hwnd(), &pt);
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
	else if (f_play==0 && 
			 ( (_sitout_but!=p_tablemap->r$()->end() && _sitout_state==false) || 
			   (_sitin_but!=p_tablemap->r$()->end() && _sitin_state==true) ) )
	{

		if (_sitout_but!=p_tablemap->r$()->end() && _sitout_state==false)
			pt = RandomizeClickLocation(_sitout_but->second.left, _sitout_but->second.top, 
										_sitout_but->second.right,  _sitout_but->second.bottom);

		else if (_sitin_but!=p_tablemap->r$()->end() && _sitin_state==true)
			pt = RandomizeClickLocation(_sitin_but->second.left, _sitin_but->second.top, 
										_sitin_but->second.right, _sitin_but->second.bottom);

		input_count = 0;

		// Translate click point to screen/mouse coords
		ClientToScreen(pMyMainWnd->attached_hwnd(), &pt);
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
	else if (f_play==1 && 
			 ( (_sitin_but!=p_tablemap->r$()->end() && _sitin_state==false) || 
			   (_sitout_but!=p_tablemap->r$()->end() && _sitout_state==true) ) )
	{
		if (_sitin_but!=p_tablemap->r$()->end() && _sitin_state==false)
			pt = RandomizeClickLocation(_sitin_but->second.left, _sitin_but->second.top, 
										_sitin_but->second.right, _sitin_but->second.bottom);

		else if (_sitout_but!=p_tablemap->r$()->end() && _sitout_state==true)
			pt = RandomizeClickLocation(_sitout_but->second.left, _sitout_but->second.top, 
										_sitout_but->second.right, _sitout_but->second.bottom);

		input_count = 0;

		// Translate click point to screen/mouse coords
		ClientToScreen(pMyMainWnd->attached_hwnd(), &pt);
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
	if (f_play==1 && _autopost_but!=p_tablemap->r$()->end() && _autopost_state==false)
	{
		pt = RandomizeClickLocation(_autopost_but->second.left, _autopost_but->second.top, 
									_autopost_but->second.right, _autopost_but->second.bottom);

		// Translate click point to screen/mouse coords
		ClientToScreen(pMyMainWnd->attached_hwnd(), &pt);
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
			SetFocus(pMyMainWnd->attached_hwnd());
			SetForegroundWindow(pMyMainWnd->attached_hwnd());
			SetActiveWindow(pMyMainWnd->attached_hwnd());

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
}

void CAutoplayer::DoI86(void) 
{
	INPUT			input[100] = {0};
	POINT			pt = {0};
	double			fScreenWidth = ::GetSystemMetrics( SM_CXSCREEN )-1;
	double			fScreenHeight = ::GetSystemMetrics( SM_CYSCREEN )-1;
	double			fx = 0., fy = 0.;
	bool			do_click = false;
	int				input_count = 0;
	int				i = 0;
	HWND			hwnd_focus = GetFocus();
	HWND			hwnd_foreground = GetForegroundWindow();
	HWND			hwnd_active = GetActiveWindow();
	POINT			cur_pos = {0};
	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

	::GetCursorPos(&cur_pos);

	do_click = false;
	if (_i86_but!=p_tablemap->r$()->end() && _i86_state)
	{
		pt = RandomizeClickLocation(_i86_but->second.left, _i86_but->second.top,
									_i86_but->second.right, _i86_but->second.bottom);

		input_count = 0;

		// Translate click point to screen/mouse coords
		ClientToScreen(pMyMainWnd->attached_hwnd(), &pt);
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
			if (_i86X_but[i]!=p_tablemap->r$()->end() && _i86X_state[i])
			{
				pt = RandomizeClickLocation(_i86X_but[i]->second.left, _i86X_but[i]->second.top,
											_i86X_but[i]->second.right, _i86X_but[i]->second.bottom);

				input_count = 0;

				// Translate click point to screen/mouse coords
				ClientToScreen(pMyMainWnd->attached_hwnd(), &pt);
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
			SetFocus(pMyMainWnd->attached_hwnd());
			SetForegroundWindow(pMyMainWnd->attached_hwnd());
			SetActiveWindow(pMyMainWnd->attached_hwnd());

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
}

const POINT CAutoplayer::RandomizeClickLocation(const int left, const int top, const int right, const int bottom) 
{
	POINT p = {0};

	// uniform random distribution, yuck!
	//p.x = ((double) rand() / (double) RAND_MAX) * (right-left) + left;
	//p.y = ((double) rand() / (double) RAND_MAX) * (bottom-top) + top;

	// normal random distribution - much better!
	GetClickPoint(left + (right-left)/2, top + (bottom-top)/2, (right-left)/2, (bottom-top)/2, &p);

	return p;
}

const void CAutoplayer::GetClickPoint(const int x, const int y, const int rx, const int ry, POINT *p) 
{
	p->x = x + (int) (RandomNormalScaled(2*rx, 0, 1) + 0.5) - (rx);
	p->y = y + (int) (RandomNormalScaled(2*ry, 0, 1) + 0.5) - (ry);
}

// random number - 0 -> scale, with normal distribution
// ignore results outside 3.5 stds from the mean
const double CAutoplayer::RandomNormalScaled(const double scale, const double m, const double s) 
{
	double res = -99;
	while (res < -3.5 || res > 3.5) res = RandomNormal(m, s);
	return (res / 3.5*s + 1) * (scale / 2.0);
}

const double CAutoplayer::RandomNormal(const double m, const double s) 
{
	/* mean m, standard deviation s */
	double x1 = 0., x2 = 0., w = 0., y1 = 0., y2 = 0.;

	do {
		x1 = 2.0 * ((double) rand()/(double) RAND_MAX) - 1.0;
		x2 = 2.0 * ((double) rand()/(double) RAND_MAX) - 1.0;
		w = x1 * x1 + x2 * x2;
	} while ( w >= 1.0 );

	w = sqrt( (-2.0 * log( w ) ) / w );
	y1 = x1 * w;
	y2 = x2 * w;

	return( m + y1 * s );
}
