#include "StdAfx.h"
#include <complex>

#include "CAutoconnector.h"
#include "CAutoplayer.h"
#include "CGameState.h"
#include "CGrammar.h"
#include "CHeartbeatThread.h"
#include "CIteratorThread.h"
#include "CPreferences.h"
#include "CRebuyManagement.h"
#include "CReplayFrame.h"
#include "CScraper.h"
#include "CStableFramesCounter.h"
#include "CSymbols.h"
#include "MainFrm.h"
#include "OpenHoldem.h"
#include "PokerChat.hpp"


CAutoplayer	*p_autoplayer = NULL;

CAutoplayer::CAutoplayer(BOOL bInitiallyOwn, LPCTSTR lpszName) : _mutex(bInitiallyOwn, lpszName)
{
	ASSERT(_mutex.m_hObject != NULL);

	// Why no seed???

	// Seed RNG
	//srand((unsigned)time( NULL ));

	set_autoplayer_engaged(false);
}

CAutoplayer::~CAutoplayer(void) 
{
}

#define ENT CSLock lock(m_critsec);
	
void CAutoplayer::set_autoplayer_engaged(const bool to_be_enabled_or_not) 
{ 
	ENT 
	_autoplayer_engaged = to_be_enabled_or_not; 
	// Set correct button state
	// We have to be careful, as during initialization the GUI does not yet exist.
	CMainFrame *pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);
	if (pMyMainWnd != NULL)
	{
		pMyMainWnd->m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_AUTOPLAYER, to_be_enabled_or_not);
	}
}

#undef ENT

void CAutoplayer::DoChat(void)
{
	double			f_chat = p_symbols->f$chat();
	HWND			hwnd_focus = GetFocus();
	POINT			cur_pos = {0};

	if ((f_chat == 0) && (_the_chat_message == NULL))
		return;

	if (!IsChatAllowed())
		return;

	// Converting the result of the $chat-function to a string.
	// Will be ignored, if we already have an unhandled chat message.
	write_log(3, "Calling RegisterChatMessage.\n");
	RegisterChatMessage(f_chat);

	::GetCursorPos(&cur_pos);

	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);
	RMapCI			r_iter = p_tablemap->r$()->find("chatbox");

	if (r_iter == p_tablemap->r$()->end())
		return;

	RECT r;
	r.left = r_iter->second.left;
	r.top = r_iter->second.top;
	r.right = r_iter->second.right;
	r.bottom = r_iter->second.bottom;

	CString s;
	s.Format("%s", _the_chat_message);

	// If we get a lock, do the action
	if (_mutex.Lock(500))
	{
		(theApp._dll_keyboard_sendstring) (p_autoconnector->attached_hwnd(), r, s, false, hwnd_focus, cur_pos);
		_mutex.Unlock();
	}
	// Clear old chat_message to allow new ones.
	_the_chat_message = NULL;
	ComputeFirstPossibleNextChatTime();
}

void CAutoplayer::DoRebuyIfNeccessary(void)
{
	if (p_symbols->f$rebuy() > 0)
	{
		if (_mutex.Lock(500))
		{
			p_rebuymanagement->TryToRebuy();
			_mutex.Unlock();
		}
	}
}

void CAutoplayer::DoAllin(void)
{
	RECT allin_button;
	RECT raise_button;
	POINT point_null = {-1, -1};
	POINT cur_pos    = {0, 0};
	bool allin_button_defined = false;
	bool raise_button_defined = false;
	int number_of_clicks = 1; // Default is: single click with the mouse

	write_log(3, "Starting DoAllin...\n");
	::GetCursorPos(&cur_pos);

	// If we get a lock, do the action
	if (!_mutex.Lock(500))
	{
		write_log(3, "...ending DoSwag early (could not get mutex lock).\n");
		return;
	}

	if (p_tablemap->buttonclickmethod() == BUTTON_DOUBLECLICK)
	{
		number_of_clicks = 2;
	}

	if (_alli_but!=p_tablemap->r$()->end())
	{
		allin_button.left = _alli_but->second.left;
		allin_button.top = _alli_but->second.top;
		allin_button.right = _alli_but->second.right;
		allin_button.bottom = _alli_but->second.bottom;
		allin_button_defined = true;
	}

	if (_rais_but!=p_tablemap->r$()->end())
	{
		// raise-button
		raise_button.left = _rais_but->second.left;
		raise_button.top = _rais_but->second.top;
		raise_button.right = _rais_but->second.right;
		raise_button.bottom = _rais_but->second.bottom;
		raise_button_defined = true;
	}
	if (p_tablemap->allinmethod() == 1)
	{
		// Clicking max (or allin) and then raise
		if (allin_button_defined && raise_button_defined)
		{
			write_log(3, "Text selection; calling mouse.dll to single click allin: %d,%d %d,%d\n", 
				allin_button.left, allin_button.top, allin_button.right, allin_button.bottom);
			// Click the allin button.
			// Don't restore the position after the first click (point_null).
			(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), allin_button, MouseLeft, number_of_clicks, NULL, point_null);
			// Same delay as 'Entry to confirm swag'? 
			Sleep(prefs.swag_delay_3());
			write_log(3, "Text selection; calling mouse.dll to single click raise: %d,%d %d,%d\n", 
				raise_button.left, raise_button.top, raise_button.right, raise_button.bottom);
			// Click the raise button;
			// Then restore the mouse position (cur_pos)
			(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), raise_button, MouseLeft, number_of_clicks, NULL, cur_pos);
		}
		else
		{
			write_log(1, "....Ending DoAllin early (no allin-button or no raise-button).\n");
		}
	}
	else  if (p_tablemap->allinmethod() == 2)
	{
		// Clicking only max (or allin)
		if (allin_button_defined)
		{
			write_log(3, "Text selection; calling mouse.dll to single click allin: %d,%d %d,%d\n", 
				allin_button.left, allin_button.top, allin_button.right, allin_button.bottom);
			// Click the raise button;
			// Then restore the mouse position (cur_pos)
			(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), allin_button, MouseLeft, number_of_clicks, NULL, cur_pos);
		}
		else
		{
			write_log(1, "....Ending DoAllin early (no allin-button).\n");
		}
	}
	// Third case (default): swagging the balance
	// (p_tablemap->allinmethod() == 0)
	// This will be handled by the swag-code.
	_mutex.Unlock();	
	p_stableframescounter->ResetOnAutoplayerAction();
}

void CAutoplayer::DoAutoplayer(void) 
{
	int	num_buttons_visible = 0;

	write_log(3, "Starting Autoplayer cadence...\n");

	// Check status of "Keyboard" menu item, and engage if necessary
	write_log(3, "Calling CheckBringKeyboard.\n");
	CheckBringKeyboard();

	// Get r$ indices of buttons that are visible
	num_buttons_visible = GetR$ButtonIndices();
	write_log(3, "Number of visible buttons: %d\n", num_buttons_visible);

	// Calculate f$play, f$prefold, f$rebuy, f$delay and f$chat for use below
	write_log(3, "Calling CalcSecondaryFormulas.\n");
	p_symbols->CalcSecondaryFormulas();

	// Handle f$play
	write_log(3, "Calling DoF$play.\n");
	DoF$play();

	// Handle i86buttons
	write_log(3, "Calling DoI86.\n");
	DoI86();

	// Handle f$prefold
	write_log(3, "Calling DoPrefold.\n");
	DoPrefold();

	// Rebuy
	write_log(3, "Calling DoRebuyIfNeccessary.\n");	
	DoRebuyIfNeccessary();

	//  Additional functionality: PokerChat
	//	(Handle f$chat)
	//
	//  Avoiding unnecessary calls to DoChat(),
	//	especially mouse movements to the chat box.
	if (IsChatAllowed() && ((p_symbols->f$chat() != 0) || (_the_chat_message != NULL)))
	{
		write_log(3, "Calling DoChat.\n");
		DoChat();
	}

	int NumberOfStableFrames = p_stableframescounter->GetNumberOfStableFrames();
	write_log(3, "Number of stable frames: % d\n", NumberOfStableFrames);

	bool isFinalAnswer = true;

	// check factors that affect isFinalAnswer status
	if (iter_vars.iterator_thread_running())
	{
		write_log(3, "Not Final Answer because iterator_thread_running\n");
		isFinalAnswer = false;
	}

	// Change from only requiring one visible button (OpenHoldem 2008-04-03)
	if (num_buttons_visible < 2)
	{
		write_log(3, "Not Final Answer because num_buttons_visible < 2\n");
		isFinalAnswer = false;
	}

	// if we are not playing (occluded?) 2008-03-25 Matrix
	if (!p_symbols->sym()->playing)
	{
		write_log(3, "Not Final Answer because !p_symbols->sym()->playing\n");
		isFinalAnswer = false;
	}

	// Scale f$delay to a number of scrapes
	// Avoid division by zero; we use milliseconds, so +1 doesn't change much.
	int additional_frames_to_wait = p_symbols->f$delay() / (prefs.scrape_delay() + 1);	

	// If we don't have enough stable frames, or have not waited f$delay milliseconds, then return.
	if (NumberOfStableFrames < (prefs.frame_delay() + additional_frames_to_wait))
	{
		write_log(3, "Not Final Answer because we don't have enough stable frames, or have not waited f$delay milliseconds\n");
		isFinalAnswer = false;
	}

	// If the game state processor didn't process this frame, then we should not act.
	if (!p_game_state->ProcessThisFrame ())
	{
		write_log(3, "Not Final Answer because game state processor didn't process this frame\n");
		isFinalAnswer = false;
	}

	// Now that we got through all of the above, we are ready to evaluate the primary formulas
	// and take the appropriate action
	write_log(3, "Calling CalcPrimaryFormulas with final answer.\n");
	p_symbols->CalcPrimaryFormulas(isFinalAnswer);

	if(!isFinalAnswer)
	{
		return;
	}

	if (p_symbols->f$alli())
	{
		DoAllin();
	}

	// do swag first since it is the odd one
	bool bDoSwag = false; // I'm just breaking this out to be a little clearer (spew)
	if ((p_tablemap->allinmethod() == 0) && p_symbols->f$alli() && p_scraper->GetButtonState(3)) //!!! //!!!
		bDoSwag = true;
	if (p_symbols->f$swag() && !p_symbols->f$alli() && p_scraper->GetButtonState(3)) //!!!
		bDoSwag = true;
	if (bDoSwag) 
	{
		write_log(3, "Calling DoSwag.\n");
		DoSwag();
	}
	else 
	{
		if (p_symbols->f$alli() && p_scraper->GetButtonState(3)) //!!!
		{
			write_log(3, "Calling DoSlider.\n");
			DoSlider();
		}
		else
		{
			write_log(3, "Calling DoARCCF.\n");
			DoARCCF();
		}
	}

	write_log(3, "...ending Autoplayer cadence.\n");
}

void CAutoplayer::DoSwag(void) 
{
	HWND			hwnd_focus = GetFocus();
	POINT			cur_pos = {0};
	bool			lost_focus = false;
	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);
	double			f_swag = p_symbols->f$swag();
	RMapCI			r_edit = p_tablemap->r$()->find("i3edit");
	RMapCI			r_button = p_tablemap->r$()->find("i3button");
	POINT			point_null = {-1, -1};
	RECT			r_null = {-1, -1, -1, -1};

	write_log(3, "Starting DoSwag...\n");

	// swag regions are hard coded as #3 for now, due to legacy WH standard
	if (r_edit==p_tablemap->r$()->end())
	{
		write_log(3, "...ending DoSwag early (no edit field).\n");
		return;
	}
	::GetCursorPos(&cur_pos);

	// If we get a lock, do the action
	if (!_mutex.Lock(500))
	{
		write_log(3, "...ending DoSwag early (could not get mutex lock).\n");
		return;
	}

	RECT rect_edit;
	rect_edit.left = r_edit->second.left;
	rect_edit.top = r_edit->second.top;
	rect_edit.right = r_edit->second.right;
	rect_edit.bottom = r_edit->second.bottom;

	// TEXT SELECTION
	if (p_tablemap->swagselectionmethod() == TEXTSEL_DOUBLECLICK)
	{
		write_log(3, "Text selection; calling mouse.dll to double click: %d,%d %d,%d\n", rect_edit.left, rect_edit.top, 
			rect_edit.right, rect_edit.bottom);
		(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), rect_edit, MouseLeft, 2, NULL, point_null);
	}

	else if (p_tablemap->swagselectionmethod() == TEXTSEL_SINGLECLICK)
	{
		write_log(3, "Text selection; calling mouse.dll to single click: %d,%d %d,%d\n", rect_edit.left, rect_edit.top, 
			rect_edit.right, rect_edit.bottom);
		(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), rect_edit, MouseLeft, 1, NULL, point_null);
	}

	else if (p_tablemap->swagselectionmethod() == TEXTSEL_CLICKDRAG)
	{
		write_log(3, "Text selection; calling mouse.dll to click drag: %d,%d %d,%d\n", rect_edit.left, rect_edit.top, 
			rect_edit.right, rect_edit.bottom);
		(theApp._dll_mouse_click_drag) (p_autoconnector->attached_hwnd(), rect_edit, NULL, point_null);
	}

	else if (p_tablemap->swagselectionmethod() == TEXTSEL_NOTHING)
	{
	}

	else
	{
		write_log(3, "...ending DoSwag early (invalid swagselectionmethod).\n");
		_mutex.Unlock();
		return;
	}

	// Check for stolen focus, and thus misswag
	if (GetForegroundWindow() != p_autoconnector->attached_hwnd())
		lost_focus = true;

	write_log(3, "Sleeping %dms.\n", prefs.swag_delay_1());
	Sleep(prefs.swag_delay_1());



	// TEXT DELETION
	if (p_tablemap->swagdeletionmethod() == TEXTDEL_DELETE)
	{
		write_log(3, "Text deletion; calling keyboard.dll to press 'delete'\n");
		(theApp._dll_keyboard_sendkey) (p_autoconnector->attached_hwnd(), r_null, VK_DELETE, NULL, point_null);
	}

	else if (p_tablemap->swagdeletionmethod() == TEXTDEL_BACKSPACE)
	{
		write_log(3, "Text deletion; calling keyboard.dll to press 'backspace'\n");
		(theApp._dll_keyboard_sendkey) (p_autoconnector->attached_hwnd(), r_null, VK_BACK, NULL, point_null);
	}

	else if (p_tablemap->swagdeletionmethod() == TEXTDEL_NOTHING)
	{
	}

	else
	{
		write_log(3, "...ending DoSwag early (invalid swagdeletionmethod).\n");
		_mutex.Unlock();
		return;
	}

	// Check for stolen focus, and thus misswag
	if (GetForegroundWindow() != p_autoconnector->attached_hwnd())
		lost_focus = true;

	write_log(3, "Sleeping %dms.\n", prefs.swag_delay_2());
	Sleep(prefs.swag_delay_2());


	// if we are swagging allin then set the swag value to be our balance (spew)
	CString swag_amt;
	if ((p_tablemap->allinmethod() == 0) && p_symbols->f$alli())
		f_swag = p_symbols->sym()->balance[10];

	// SWAG AMOUNT ENTRY
	if (f_swag != (int) f_swag)
		swag_amt.Format("%.2f", f_swag);
	else
		swag_amt.Format("%.0f", f_swag);

	write_log(3, "Swag amount; calling keyboard.dll to swag: %s %d,%d %d,%d\n", swag_amt, rect_edit.left, rect_edit.top, 
			rect_edit.right, rect_edit.bottom);
	(theApp._dll_keyboard_sendstring) (p_autoconnector->attached_hwnd(), rect_edit, swag_amt, prefs.swag_use_comma(), NULL, point_null);

	// Check for stolen focus, and thus misswag
	if (GetForegroundWindow() != p_autoconnector->attached_hwnd())
		lost_focus = true;

	write_log(3, "Sleeping %dms.\n", prefs.swag_delay_3());
	Sleep(prefs.swag_delay_3());

	// BET CONFIRMATION ACTION
	if (!lost_focus || !prefs.focus_detect())
	{
		if (p_tablemap->swagconfirmationmethod() == BETCONF_ENTER)
		{
			write_log(3, "Confirmation; calling keyboard.dll to press 'Enter'\n");					
			(theApp._dll_keyboard_sendkey) (p_autoconnector->attached_hwnd(), r_null, VK_RETURN, hwnd_focus, cur_pos);
		}

		else if (p_tablemap->swagconfirmationmethod() == BETCONF_CLICKBET &&
				 (_rais_but!=p_tablemap->r$()->end() || r_button!=p_tablemap->r$()->end()) )
		{
			RECT rect_button;

			// use i3button region if it exists, otherwise use the bet/raise button region
			if (r_button!=p_tablemap->r$()->end())
			{
				rect_button.left = r_button->second.left;
				rect_button.top = r_button->second.top;
				rect_button.right = r_button->second.right;
				rect_button.bottom = r_button->second.bottom;
			}
			else
			{
				rect_button.left = _rais_but->second.left;
				rect_button.top = _rais_but->second.top;
				rect_button.right = _rais_but->second.right;
				rect_button.bottom = _rais_but->second.bottom;
			}

			if (p_tablemap->buttonclickmethod() == BUTTON_DOUBLECLICK)

			{
				write_log(3, "Confirmation; calling mouse.dll to double click bet button: %d,%d %d,%d\n", 
					rect_button.left, rect_button.top, rect_button.right, rect_button.bottom);					
				(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), rect_button, MouseLeft, 2, hwnd_focus, cur_pos);
			}
			else
			{
				write_log(3, "Confirmation; calling mouse.dll to single click bet button: %d,%d %d,%d\n", 
					rect_button.left, rect_button.top, rect_button.right, rect_button.bottom);					
				(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), rect_button, MouseLeft, 1, hwnd_focus, cur_pos);
			}
		}

		else if (p_tablemap->swagconfirmationmethod() == BETCONF_NOTHING)
		{
		}

		else
		{
			write_log(3, "...ending DoSwag early (invalid swagconfirmationmethod).\n");
			_mutex.Unlock();
			return;
		}
		
		p_symbols->RecordPrevAction(k_action_swag);
		write_logautoplay(1, "SWAG\n");

		p_heartbeat_thread->set_replay_recorded_this_turn(false);
	}

	_mutex.Unlock();
	p_stableframescounter->ResetOnAutoplayerAction();

	write_log(3, "...ending DoSwag, 'didswag' now: %d\n", p_symbols->sym()->didswag[4]);
}

void CAutoplayer::DoARCCF(void) 
{
	ActionConstant	do_click = k_action_undefined;
	HWND			hwnd_focus = GetFocus();
	POINT			cur_pos = {0};
	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

	double			alli = p_symbols->f$alli();
	double			rais = p_symbols->f$rais();
	double			call = p_symbols->f$call();
	int				sym_myturnbits = (int) p_symbols->sym()->myturnbits;

	write_log(3, "Starting DoARCCF...\n");

	::GetCursorPos(&cur_pos);

	RECT			r;

	do_click = k_action_undefined;

	// ALLIN
	if (alli && sym_myturnbits&0x8 && _alli_but!=p_tablemap->r$()->end())
	{
		r.left = _alli_but->second.left;
		r.top = _alli_but->second.top;
		r.right = _alli_but->second.right;
		r.bottom =_alli_but->second.bottom;
		do_click = k_action_allin;
		write_log(3, "Found valid f$alli formula/allin button combination.\n");
	}

	// RAISE
	else if (rais && sym_myturnbits&0x4 && _rais_but!=p_tablemap->r$()->end())
	{
		r.left = _rais_but->second.left;
		r.top = _rais_but->second.top;
		r.right = _rais_but->second.right;
		r.bottom =_rais_but->second.bottom;
		do_click = k_action_raise;
		write_log(3, "Found valid f$rais formula/raise button combination.\n");
	}

	// CALL
	else if (call && sym_myturnbits&0x1 && _call_but!=p_tablemap->r$()->end())
	{
		r.left = _call_but->second.left;
		r.top = _call_but->second.top;
		r.right = _call_but->second.right;
		r.bottom =_call_but->second.bottom;
		do_click = k_action_call;
		write_log(3, "Found valid f$call formula/call button combination.\n");
	}

	// CHECK
	// None of f$alli, f$swag, f$rais, f$call are > 0 or no buttons related to
	// these actions can be found. If there is a check button, then click it.
	else if (_chec_but!=p_tablemap->r$()->end())
	{
		r.left = _chec_but->second.left;
		r.top = _chec_but->second.top;
		r.right = _chec_but->second.right;
		r.bottom =_chec_but->second.bottom;
		do_click = k_action_check;
		write_log(3, "Found valid check button (all primary formulas = 0).\n");
	}

	// FOLD
	// None of f$alli, f$swag, f$rais, f$call are > 0 or no buttons related to
	// these actions can be found. If there is a fold button, then click it, otherwise we have a serious problem.
	else if (_fold_but!=p_tablemap->r$()->end())
	{
		r.left = _fold_but->second.left;
		r.top = _fold_but->second.top;
		r.right = _fold_but->second.right;
		r.bottom =_fold_but->second.bottom;
		do_click = k_action_fold;
		write_log(3, "Found valid fold button (all primary formulas = 0).\n");
	}

	if (do_click == k_action_undefined)
	{
		write_log(3, "...ending DoARCCF early (no relevant primary formula/available button combination).\n");
		return;
	}

	else
	{
		// If we get a lock, do the action
		if (!_mutex.Lock(500))
		{
			write_log(3, "...ending DoARCCF early (could not get mutex lock).\n");
			return;
		}

		if (p_tablemap->buttonclickmethod() == BUTTON_DOUBLECLICK)
		{
			write_log(3, "Calling mouse.dll to double click: %d,%d %d,%d\n", r.left, r.top, r.right, r.bottom);
			(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), r, MouseLeft, 2, hwnd_focus, cur_pos);
		}
		else
		{
			write_log(3, "Calling mouse.dll to single click: %d,%d %d,%d\n", r.left, r.top, r.right, r.bottom);
			(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), r, MouseLeft, 1, hwnd_focus, cur_pos);
		}
		
		_mutex.Unlock();
		p_stableframescounter->ResetOnAutoplayerAction();

		// record did*/prevaction
		int sym_br = (int) p_symbols->sym()->br;

		// Writing 4-digit-name of action, e.g "ALLI" or "RAIS" to the log.
		write_logautoplay(1, ActionConstantNames(do_click));
		p_symbols->RecordPrevAction(do_click);
		
		p_heartbeat_thread->set_replay_recorded_this_turn(false);
	}

	write_log(3, "...ending DoARCCF, 'didrais'/'didcall'/'didchec' now: %d %d %d\n", 
	p_symbols->sym()->didrais[4], p_symbols->sym()->didcall[4], p_symbols->sym()->didchec[4]);
}

void CAutoplayer::DoSlider(void) 
{
	HWND			hwnd_focus = GetFocus();
	POINT			cur_pos = {0};

	bool			sym_ismyturn = (bool) p_symbols->sym()->ismyturn;
	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);
	double			alli = p_symbols->f$alli();
	RMapCI			slider = p_tablemap->r$()->find("i3slider");
	RMapCI			handle = p_tablemap->r$()->find("i3handle");
	RMapCI			r_button = p_tablemap->r$()->find("i3button");
	POINT			point_null = {-1, -1};
	RECT			r_null = {-1, -1, -1, -1};

	write_log(3, "Starting DoSlider...\n");

	::GetCursorPos(&cur_pos);

	if (!sym_ismyturn)
		return;

	if (slider==p_tablemap->r$()->end() | handle==p_tablemap->r$()->end())
		return;

	if (!p_scraper->handle_found_at_xy())
		return;


	// Get mutex lock and jam
	if (_mutex.Lock(500))
	{
		// Click and drag handle
		RECT	r;
		r.left = p_scraper->handle_xy().x + ((handle->second.right - handle->second.left)/2);
		r.top = p_scraper->handle_xy().y + ((handle->second.bottom - handle->second.top)/2);
		r.right = p_scraper->handle_xy().x + (slider->second.right - slider->second.left);
		r.bottom = r.top;		
		
		write_log(1, "Calling mouse.dll to jam from %d,%d to %d,%d\n", r.left, r.top, r.right, r.bottom);
		(theApp._dll_mouse_click_drag) (p_autoconnector->attached_hwnd(), r, NULL, point_null);

		write_log(3, "Sleeping %d ms\n.", prefs.swag_delay_3());
		Sleep(prefs.swag_delay_3());

		// Click confirmation button
		if (p_tablemap->swagconfirmationmethod() == BETCONF_ENTER)
		{
			write_log(3, "Confirmation; calling keyboard.dll to press 'Enter'\n");
			(theApp._dll_keyboard_sendkey) (p_autoconnector->attached_hwnd(), r_null, VK_RETURN, hwnd_focus, cur_pos);
		}

		else if (p_tablemap->swagconfirmationmethod() == BETCONF_CLICKBET &&
				 (_rais_but!=p_tablemap->r$()->end() || r_button!=p_tablemap->r$()->end()) )
		{
			RECT rect_button;

			// use allin button if it exists, otherwise use i3button region if it exists, 
			// otherwise use the bet/raise button region
			if (_alli_but!=p_tablemap->r$()->end())
			{
				rect_button.left = _alli_but->second.left;
				rect_button.top = _alli_but->second.top;
				rect_button.right = _alli_but->second.right;
				rect_button.bottom = _alli_but->second.bottom;
			}
			
			else if (r_button!=p_tablemap->r$()->end())
			{
				rect_button.left = r_button->second.left;
				rect_button.top = r_button->second.top;
				rect_button.right = r_button->second.right;
				rect_button.bottom = r_button->second.bottom;
			}
			
			else
			{
				rect_button.left = _rais_but->second.left;
				rect_button.top = _rais_but->second.top;
				rect_button.right = _rais_but->second.right;
				rect_button.bottom = _rais_but->second.bottom;
			}

			if (p_tablemap->buttonclickmethod() == BUTTON_DOUBLECLICK)
			{
				write_log(3, "Confirmation; calling mouse.dll to double click bet button: %d,%d %d,%d\n", 
						rect_button.left, rect_button.top, rect_button.right, rect_button.bottom);	
				(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), rect_button, MouseLeft, 2, hwnd_focus, cur_pos);
			}
			else
			{
				write_log(3, "Confirmation; calling mouse.dll to single click bet button: %d,%d %d,%d\n", 
						rect_button.left, rect_button.top, rect_button.right, rect_button.bottom);	
				(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), rect_button, MouseLeft, 1, hwnd_focus, cur_pos);
			}
		}

		else
		{
			write_log(3, "...ending DoSlider early (invalid swagconfirmationmethod).\n");
			_mutex.Unlock();
			return;
		}


		write_logautoplay(1, "JAM");
		write_log(1, "Jam complete: %d,%d,%d,%d\n", r.left, r.top, r.right, r.bottom);

		// reset elapsedauto symbol
		time_t my_time_t;
		time(&my_time_t);
		p_symbols->set_elapsedautohold(my_time_t);
	}

	_mutex.Unlock();
	p_stableframescounter->ResetOnAutoplayerAction();

	write_log(3, "...ending DoSlider.\n");
}

void CAutoplayer::DoPrefold(void) 
{
	HWND			hwnd_focus = GetFocus();
	POINT			cur_pos = {0};
	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);
	double			prefold = p_symbols->f$prefold();

	write_log(3, "Starting DoPrefold...\n");

	::GetCursorPos(&cur_pos);

	if (prefold == 0)  
		return;

	if (_pre_fold_but == p_tablemap->r$()->end())  
		return;

	// Click location
	RECT	r;
	r.left = _pre_fold_but->second.left;
	r.top = _pre_fold_but->second.top;
	r.right = _pre_fold_but->second.right;
	r.bottom = _pre_fold_but->second.bottom;
	
	// If we get a lock, do the action
	if (_mutex.Lock(500))
	{
		write_log(3, "Confirmation; calling mouse.dll to single click prefold button: %d,%d %d,%d\n", r.left, r.top, r.right, r.bottom);	

		(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), r, MouseLeft, 1, hwnd_focus, cur_pos);

		_mutex.Unlock();
		
		p_symbols->RecordPrevAction(k_action_fold);
		write_logautoplay(1, "FOLD");
	}
	p_symbols->CalcAutoTrace();
	write_log(3, "...ending DoPrefold.\n");
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
	bringsysmenu = GetSystemMenu(p_autoconnector->attached_hwnd(), false);

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
			SetFocus(p_autoconnector->attached_hwnd());
			SetForegroundWindow(p_autoconnector->attached_hwnd());
			SetActiveWindow(p_autoconnector->attached_hwnd());
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
			SetFocus(p_autoconnector->attached_hwnd());
			SetForegroundWindow(p_autoconnector->attached_hwnd());
			SetActiveWindow(p_autoconnector->attached_hwnd());

			SendInput(input_count, input, sizeof(INPUT));

			SetActiveWindow(hwnd_focus);
			SetForegroundWindow(hwnd_focus);
			SetFocus(hwnd_focus);

			SetCursorPos(cur_pos.x, cur_pos.y);

			_mutex.Unlock();
		}
	}
}

void CAutoplayer::DoF$play(void) 
{
	bool			do_click = false;
	HWND			hwnd_focus = GetFocus();
	POINT			cur_pos = {0};
	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);
	double			f_play = p_symbols->f$play();
	RECT			r = {0};

	write_log(3, "Starting DoF$play...\n");

	::GetCursorPos(&cur_pos);

	do_click = false;

	// leave table
	if (f_play==-2 && _leave_but!=p_tablemap->r$()->end())
	{
		r.left = _leave_but->second.left;
		r.top = _leave_but->second.top;
		r.right = _leave_but->second.right;
		r.bottom = _leave_but->second.bottom;

		do_click = true;
		write_log(3, "Found valid f$play (leave) / leave button combination.\n");
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
		{
			r.left = _sitout_but->second.left;
			r.top = _sitout_but->second.top;
			r.right = _sitout_but->second.right;
			r.bottom = _sitout_but->second.bottom;
		}

		else if (_sitin_but!=p_tablemap->r$()->end() && _sitin_state==true)
		{
			r.left = _sitin_but->second.left;
			r.top = _sitin_but->second.top;
			r.right = _sitin_but->second.right;
			r.bottom = _sitin_but->second.bottom;
		}

		do_click = true;
		write_log(3, "Found valid f$play (sitout) / sitout button combination.\n");
	}

	// sit in
	else if (f_play==1 && 
			 ( (_sitin_but!=p_tablemap->r$()->end() && _sitin_state==false) || 
			   (_sitout_but!=p_tablemap->r$()->end() && _sitout_state==true) ) )
	{
		if (_sitin_but!=p_tablemap->r$()->end() && _sitin_state==false)
		{
			r.left = _sitin_but->second.left;
			r.top = _sitin_but->second.top;
			r.right = _sitin_but->second.right;
			r.bottom = _sitin_but->second.bottom;
		}

		else if (_sitout_but!=p_tablemap->r$()->end() && _sitout_state==true)
		{
			r.left = _sitout_but->second.left;
			r.top = _sitout_but->second.top;
			r.right = _sitout_but->second.right;
			r.bottom = _sitout_but->second.bottom;
		}

		do_click = true;
		write_log(3, "Found valid f$play (sitin) / sitin button combination.\n");
	}

	// Autopost
	else if (f_play==1 && _autopost_but!=p_tablemap->r$()->end() && _autopost_state==false)
	{
		r.left = _autopost_but->second.left;
		r.top = _autopost_but->second.top;
		r.right = _autopost_but->second.right;
		r.bottom = _autopost_but->second.bottom;

		do_click = true;
		write_log(3, "Found valid f$play (sitin) / autopost button combination.\n");
	}

	if (do_click)
	{
		// If we get a lock, do the action
		if (_mutex.Lock(500)) 
		{
			write_log(3, "Calling mouse.dll to single click button: %d,%d %d,%d\n", r.left, r.top, r.right, r.bottom);	
			(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), r, MouseLeft, 1, hwnd_focus, cur_pos);

			_mutex.Unlock();

			// reset elapsedauto symbol
			time_t my_time_t;
			time(&my_time_t);
			p_symbols->set_elapsedautohold(my_time_t);
		}
	}

	write_log(3, "...ending DoF$play.\n");
}

void CAutoplayer::DoI86(void) 
{
	bool			do_click = false;
	int				i = 0;
	HWND			hwnd_focus = GetFocus();
	POINT			cur_pos = {0};
	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);
	RECT			r;

	write_log(3, "Starting DoI86...\n");

	::GetCursorPos(&cur_pos);

	do_click = false;

	if (_i86_but!=p_tablemap->r$()->end() && _i86_state)
	{
		r.left = _i86_but->second.left;
		r.top = _i86_but->second.top;
		r.right = _i86_but->second.right;
		r.bottom = _i86_but->second.bottom;

		do_click = true;
		write_log(3, "Found valid i86 button.\n");
	}

	else
	{
		for (i=0; i<=9; i++)
		{
			if (_i86X_but[i]!=p_tablemap->r$()->end() && _i86X_state[i])
			{
				r.left = _i86X_but[i]->second.left;
				r.top = _i86X_but[i]->second.top;
				r.right = _i86X_but[i]->second.right;
				r.bottom = _i86X_but[i]->second.bottom;

				do_click = true;
				write_log(3, "Found valid i86 (%d) button.\n", i);
				break;
			}
		}
	}

	if (do_click)
	{
		// If we get a lock, do the action
		if (_mutex.Lock(500))
		{
			write_log(3, "Calling mouse.dll to single click button: %d,%d %d,%d\n", r.left, r.top, r.right, r.bottom);	
			(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), r, MouseLeft, 1, hwnd_focus, cur_pos);

			_mutex.Unlock();

			// reset elapsedauto symbol
			time_t my_time_t;
			time(&my_time_t);
			p_symbols->set_elapsedautohold(my_time_t);
		}
	}

	write_log(3, "...ending DoI86.\n");
}
