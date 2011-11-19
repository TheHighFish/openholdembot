#include "StdAfx.h"
#include "CAutoplayer.h"

#include <complex>
#include "CAutoconnector.h"
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
#include "CTableMapAccess.h"
#include "MainFrm.h"
#include "OpenHoldem.h"
#include "PokerChat.hpp"
#include "SwagAdjustment.h"


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

void CAutoplayer::GetNeccessaryTablemapObjects()
{
	CString button_name;
	allin_button_defined  = p_tablemap_access->GetButtonRect(k_button_allin,  &allin_button);
	raise_button_defined  = p_tablemap_access->GetButtonRect(k_button_raise,  &raise_button);
	call_button_defined   = p_tablemap_access->GetButtonRect(k_button_call,   &call_button);
	check_button_defined  = p_tablemap_access->GetButtonRect(k_button_check,  &check_button);
	fold_button_defined   = p_tablemap_access->GetButtonRect(k_button_fold,   &fold_button);
	i3_button_defined     = p_tablemap_access->GetButtonRect(k_button_i3,     &i3_button);
	sitin_button_defined  = p_tablemap_access->GetButtonRect(k_button_sitin,  &sitin_button);
	sitout_button_defined = p_tablemap_access->GetButtonRect(k_button_sitout, &sitout_button);
	leave_button_defined  = p_tablemap_access->GetButtonRect(k_button_leave,  &leave_button);
	i86_button_defined    = p_tablemap_access->GetButtonRect(k_button_i86,    &i86_button);

	// swagging
	i3_edit_defined   = p_tablemap_access->GetTableMapRect("i3edit", &i3_edit_region);
	i3_slider_defined = p_tablemap_access->GetTableMapRect("i3slider", &i3_slider_region);
	i3_handle_defined = p_tablemap_access->GetTableMapRect("i3button", &i3_handle_region);
	for (int i=0; i<k_number_of_i86X_buttons; i++)
	{
		button_name.Format("i86%dbutton", i);
		i86X_buttons_defined[i] = p_tablemap_access->GetTableMapRect(button_name, &i86X_buttons[i]);
	}
	CSLock lock(m_critsec); // Needed???
	// Set correct button state
	// We have to be careful, as during initialization the GUI does not yet exist.
	bool to_be_enabled_or_not = _autoplayer_engaged; 
	CMainFrame *pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);
	if (pMyMainWnd != NULL)
	{
		pMyMainWnd->m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_AUTOPLAYER, to_be_enabled_or_not);
	}
}

int CAutoplayer::NumberOfVisibleButtons()
{
	// Buttons for playing actions, e.g. fold or allin.
	// There have to be at least 2 to make it our turn.
	int number_of_visible_buttons = 0
		+ (allin_button_defined ? 1 : 0)
		+ (raise_button_defined ? 1 : 0)
		+ (call_button_defined  ? 1 : 0)
		+ (check_button_defined ? 1 : 0)
		+ (fold_button_defined  ? 1 : 0);
	return number_of_visible_buttons;
}

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
	RECT			rect_chatbox;
	if (!p_tablemap_access->GetTableMapRect("chatbox", &rect_chatbox))
	{
		write_log(3, "No chat region defined\n");
		return;
	}
	CString s;
	s.Format("%s", _the_chat_message);

	// If we get a lock, do the action
	if (_mutex.Lock(500))
	{
		(theApp._dll_keyboard_sendstring) (p_autoconnector->attached_hwnd(), rect_chatbox, s, false, hwnd_focus, cur_pos);
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
	POINT point_null = {-1, -1};
	POINT cur_pos    = {0, 0};
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

	// Access TM objects !! Better name, better comment
	GetNeccessaryTablemapObjects();
	num_buttons_visible = NumberOfVisibleButtons();
	write_log(3, "Number of visible buttons: %d\n", num_buttons_visible);

	// Calculate f$play, f$prefold, f$rebuy, f$delay and f$chat for use below
	write_log(3, "Calling CalcSecondaryFormulas.\n");
	p_symbols->CalcSecondaryFormulas();

	// Handle f$sitin, f$sitout, f$leave (formerly f$play)
	write_log(3, "Calling DoF$Sitin_Sitout_Leave.\n");
	DoF$Sitin_Sitout_Leave();

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
	if (p_symbols->f$betsize() && !p_symbols->f$alli() && p_scraper->GetButtonState(3)) //!!!
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
	double			f_swag = p_symbols->f$betsize();
	POINT			point_null = {-1, -1};
	RECT			r_null = {-1, -1, -1, -1};

	write_log(3, "Starting DoSwag...\n");

	// swag regions are hard coded as #3 for now, due to legacy WH standard
	if (!i3_edit_defined)
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

	// TEXT SELECTION
	if (p_tablemap->swagselectionmethod() == TEXTSEL_DOUBLECLICK)
	{
		write_log(3, "Text selection; calling mouse.dll to double click: %d,%d %d,%d\n", 
			i3_edit_region.left, i3_edit_region.top, i3_edit_region.right, i3_edit_region.bottom);
		(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), i3_edit_region, MouseLeft, 2, NULL, point_null);
	}

	else if (p_tablemap->swagselectionmethod() == TEXTSEL_SINGLECLICK)
	{
		write_log(3, "Text selection; calling mouse.dll to single click: %d,%d %d,%d\n", 
			i3_edit_region.left, i3_edit_region.top, i3_edit_region.right, i3_edit_region.bottom);
		(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), i3_edit_region, MouseLeft, 1, NULL, point_null);
	}

	else if (p_tablemap->swagselectionmethod() == TEXTSEL_CLICKDRAG)
	{
		write_log(3, "Text selection; calling mouse.dll to click drag: %d,%d %d,%d\n", 
			i3_edit_region.left, i3_edit_region.top, i3_edit_region.right, i3_edit_region.bottom);
		(theApp._dll_mouse_click_drag) (p_autoconnector->attached_hwnd(), i3_edit_region, NULL, point_null);
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
	double swag_adjusted = SwagAmountAdjusted(f_swag);
	if (swag_adjusted != (int) swag_adjusted)
		swag_amt.Format("%.2f", swag_adjusted);
	else
		swag_amt.Format("%.0f", swag_adjusted);

	write_log(3, "Swag amount (not adjusted): %.2f\n", f_swag);
	write_log(3, "Swag amount; calling keyboard.dll to swag (adjusted): %s %d,%d %d,%d\n", 
		swag_amt, i3_edit_region.left, i3_edit_region.top, i3_edit_region.right, i3_edit_region.bottom);
	(theApp._dll_keyboard_sendstring) (p_autoconnector->attached_hwnd(), i3_edit_region, swag_amt, prefs.swag_use_comma(), NULL, point_null);

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
				 (raise_button_defined || i3_button_defined))
		{
			RECT rect_button;

			// use i3button region if it exists, otherwise use the bet/raise button region
			if (i3_button_defined)
			{
				rect_button = i3_button;
			}
			else
			{
				rect_button = raise_button;
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
	if (alli && sym_myturnbits&0x8 && allin_button_defined)
	{
		r = allin_button;
		do_click = k_action_allin;
		write_log(3, "Found valid f$alli formula/allin button combination.\n");
	}

	// RAISE
	else if (rais && sym_myturnbits&0x4 && raise_button_defined)
	{
		r = raise_button;
		do_click = k_action_raise;
		write_log(3, "Found valid f$rais formula/raise button combination.\n");
	}

	// CALL
	else if (call && sym_myturnbits&0x1 && call_button_defined)
	{
		r = call_button;
		do_click = k_action_call;
		write_log(3, "Found valid f$call formula/call button combination.\n");
	}

	// CHECK
	// None of f$alli, f$betsize, f$rais, f$call are > 0 or no buttons related to
	// these actions can be found. If there is a check button, then click it.
	else if (check_button_defined)
	{
		r = check_button;
		do_click = k_action_check;
		write_log(3, "Found valid check button (all primary formulas = 0).\n");
	}

	// FOLD
	// None of f$alli, f$betsize, f$rais, f$call are > 0 or no buttons related to
	// these actions can be found. If there is a fold button, then click it, otherwise we have a serious problem.
	else if (fold_button_defined)
	{
		r = fold_button;
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
	POINT			point_null = {-1, -1};
	RECT			r_null = {-1, -1, -1, -1};

	write_log(3, "Starting DoSlider...\n");

	::GetCursorPos(&cur_pos);

	if (!sym_ismyturn)
		return;

	if (!(i3_slider_defined && i3_handle_defined))
		return;

	if (!p_scraper->handle_found_at_xy())
		return;


	// Get mutex lock and jam
	if (_mutex.Lock(500))
	{
		// Click and drag handle
		//!!!
		
		RECT	r;
		/*
		r.left = p_scraper->handle_xy().x + ((handle->second.right - handle->second.left)/2);
		r.top = p_scraper->handle_xy().y + ((handle->second.bottom - handle->second.top)/2);
		r.right = p_scraper->handle_xy().x + (slider->second.right - slider->second.left);
		r.bottom = r.top;		
		*/
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
				 (raise_button_defined || i3_button_defined))
		{
			RECT rect_button;

			// use allin button if it exists, otherwise use i3button region if it exists, 
			// otherwise use the bet/raise button region
			if (allin_button_defined)
			{
				rect_button = allin_button;
			}
			else if (i3_button_defined) 
			{
				rect_button = i3_button;
			}
			else
			{
				rect_button = raise_button;
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

	if (!prefold_button_defined)  
		return;

	// If we get a lock, do the action
	if (_mutex.Lock(500))
	{
		write_log(3, "Confirmation; calling mouse.dll to single click prefold button: %d,%d %d,%d\n", 
			prefold_button.left, prefold_button.top, prefold_button.right, prefold_button.bottom);	

		(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), prefold_button, MouseLeft, 1, hwnd_focus, cur_pos);

		_mutex.Unlock();
		
		p_symbols->RecordPrevAction(k_action_fold);
		write_logautoplay(1, "FOLD");
	}
	p_symbols->CalcAutoTrace();
	write_log(3, "...ending DoPrefold.\n");
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

bool CAutoplayer::TimeToHandleSitinSitoutLeave()
{
	// Disabled (N-1) out of N heartbeats (3 out of 4 seconds)
	// to avoid multiple fast clicking on the sitin / sitout-button.
	// Contrary to the old f$play-function we use a heartbeat-counter 
	// for that logic, as with a small scrape-delay it was
	// still possible to act multiple times within the same second.
	int hearbeats_to_pause = 4 / prefs.scrape_delay();
	if  (hearbeats_to_pause < 1)
	{
		hearbeats_to_pause = 1;
	}
	return ((p_heartbeat_thread->heartbeat_counter() % hearbeats_to_pause) == 0);
}

void CAutoplayer::DoF$Sitin_Sitout_Leave(void) 
{
	HWND			hwnd_focus = GetFocus();
	POINT			cur_pos = {0};
	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);
	double			f_sitin  = p_symbols->f$sitin();
	double			f_sitout = p_symbols->f$sitout();
	double			f_leave  = p_symbols->f$leave();
	RECT			r = {0};

	write_log(3, "Starting DoF$Sitin_Sitout_Leave...\n");
	if (!TimeToHandleSitinSitoutLeave())
	{
		write_log(3, "DoF$Sitin_Sitout_Leave disabled for the current heartbeat.\n");
		write_log(3, "Leaving DoF$Sitin_Sitout_Leave early.\n");
		return;
	}

	::GetCursorPos(&cur_pos);

	bool do_click = false;

	// leave table
	if (f_leave==true && leave_button_defined)
	{
		r = leave_button;
		do_click = true;
		write_log(3, "f$leave is true (leave) and leave button exists.\n");
	}

	// sit out
	else if (f_sitout==true && 
		((sitout_button_defined && _sitout_state==false) || (sitin_button_defined && _sitin_state==true)))
	{
		if (sitout_button_defined && (_sitout_state==false))
		{
			r = sitout_button;
		}
		else if (sitin_button_defined && (_sitin_state==true))
		{
			r = sitin_button;
		}

		do_click = true;
		write_log(3, "f$sitout is true and sitout button exists.\n");
	}

	// sit in
	else if (f_sitin==true && 
		((sitin_button_defined && (_sitin_state==false)) || (sitout_button_defined && (_sitout_state==true))))
	{
		if (sitin_button_defined && _sitin_state==false)
		{
			r = sitin_button;
		}

		else if (sitout_button_defined && _sitout_state==true)
		{
			r = sitout_button;
		}

		do_click = true;
		write_log(3, "f$sitin is true and sitin button exists.\n");
	}

	// Autopost
	if (f_sitin && (_autopost_state==false) && autopost_button_defined)
	{
		write_log(3, "Found valid f$sitin / autopost button combination.\n");
		// If we get a lock, do the action
		if (_mutex.Lock(500)) 
		{
			write_log(3, "Calling mouse.dll to single click button: %d,%d %d,%d\n", r.left, r.top, r.right, r.bottom);	
			(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), autopost_button, MouseLeft, 1, hwnd_focus, cur_pos);

			_mutex.Unlock();

			// reset elapsedauto symbol
			time_t my_time_t;
			time(&my_time_t);
			p_symbols->set_elapsedautohold(my_time_t);
		}
	}

	write_log(3, "...ending DoF$Sitin_Sitout_Leave.\n");
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

	if (i86_button_defined && _i86_state)
	{
		r = i86_button;
		do_click = true;
		write_log(3, "Found valid i86 button.\n");
	}

	else
	{
		for (int i=0; i<k_number_of_i86X_buttons; i++)
		{
			if (i86X_buttons_defined[i] && _i86X_state[i])
			{
				r = i86X_buttons[i];
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
