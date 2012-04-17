/* TODO: 
* get mutex
* get focus
* get mouse
* act
* restore mouse
* prevaction
* p_stableframescounter->ResetOnAutoplayerAction();
* p_symbols->reset_elapsedautohold();	
*/

#include "StdAfx.h"
#include "CAutoplayer.h"

#include <complex>
#include "CAutoconnector.h"
#include "CAutoplayerFunctions.h"
#include "CCasinoInterface.h"
#include "CGameState.h"
#include "CGrammar.h"
#include "CHeartbeatThread.h"
#include "CIteratorThread.h"
#include "CPreferences.h"
#include "CRebuyManagement.h"
#include "CReplayFrame.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CStableFramesCounter.h"
#include "CSymbols.h"
#include "MainFrm.h"
#include "OpenHoldem.h"
#include "PokerChat.hpp"


CAutoplayer	*p_autoplayer = NULL;

CAutoplayer::CAutoplayer(BOOL bInitiallyOwn, LPCTSTR lpszName) : _mutex(bInitiallyOwn, lpszName)
{
	ASSERT(_mutex.m_hObject != NULL); 

	set_autoplayer_engaged(false);
	// Set correct button state
	// We have to be careful, as during initialization the GUI does not yet exist.
	bool to_be_enabled_or_not = _autoplayer_engaged; 
	CMainFrame *pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);
	if (pMyMainWnd != NULL)
	{
		pMyMainWnd->m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_AUTOPLAYER, to_be_enabled_or_not);
	}
}

CAutoplayer::~CAutoplayer(void) 
{}

bool CAutoplayer::GetMutex()
{
	return _mutex.Lock(500);
}



bool CAutoplayer::TimeToHandleSecondaryFormulas()
{
	// Disabled (N-1) out of N heartbeats (3 out of 4 seconds)
	// to avoid multiple fast clicking on the sitin / sitout-button.
	// Contrary to the old f$play-function we use a heartbeat-counter 
	// for that logic, as with a small scrape-delay it was
	// still possible to act multiple times within the same second.

	// Scrape_delay() should always be > 0, there's a check in the GUI.
	assert(prefs.scrape_delay() > 0);
	int hearbeats_to_pause = 4 / prefs.scrape_delay();
	if  (hearbeats_to_pause < 1)
	{
		hearbeats_to_pause = 1;
	}
	return ((p_heartbeat_thread->heartbeat_counter() % hearbeats_to_pause) == 0);
}


bool CAutoplayer::DoBetPot(void)
{
	// Start with 2 * potsize, continue with lower betsizes, finally 1/4 pot
	for (int i=k_autoplayer_function_betpot_2_1; i<=k_autoplayer_function_betpot_1_4; i++)
	{
		if (p_autoplayer_functions->GetAautoplayerFunctionValue(i))
		{
			write_log(prefs.debug_autoplayer(), "[AutoPlayer] %s true.\n", 
				k_autoplayer_functionname[i]);
			write_log(prefs.debug_autoplayer(), "[AutoPlayer] Trying to click button.\n");
			if (/*BetPotMethod()*/1 == 1) //!!!
			{
				return p_casino_interface->ClickButton(i);
			}
			else 
			{
				return p_casino_interface->ClickButtonSequence(i, k_autoplayer_function_raise, /*betpot_delay* !!! */ 1);
			}
		}
	}
}

bool CAutoplayer::AnyPrimaryFormulaTrue()
{
	for (int i=k_autoplayer_function_allin; i<=k_autoplayer_function_call; i++)
	{
		if (p_autoplayer_functions->GetAautoplayerFunctionValue(i))
		{
			return true;
		}
	}
	return false;
}

bool CAutoplayer::AnySecondaryFormulaTrue()
{
	for (int i=k_autoplayer_function_prefold; i<=k_autoplayer_function_chat; i++)
	{
		if (p_autoplayer_functions->GetAautoplayerFunctionValue(i))
		{
			return true;
		}
	}
	return false;
}

void CAutoplayer::ExecutePrimaryFormulas()
{
	if (!AnyPrimaryFormulaTrue())
	{
		// Attention: check/fold !!!	
	}
	else
	{
		if (p_autoplayer_functions->f$alli())
		{
			//DoAllin; !!!
			return;
		}
		if (DoBetPot())
		{
			return;
		}
		//seag
		
	}
}

bool CAutoplayer::ExecuteSecondaryFormulas()
{
	if (!AnySecondaryFormulaTrue())
	{
		write_log(prefs.debug_autoplayer(), "[AutoPlayer] All secondary formulas false.\n");
		write_log(prefs.debug_autoplayer(), "[AutoPlayer] Nothing to do.\n");
		return false;
	}
	for (int i=k_autoplayer_function_prefold; i<=k_autoplayer_function_leave; i++)
	{
		if (p_autoplayer_functions->GetAautoplayerFunctionValue(i))
		{
			return p_casino_interface->ClickButton(i);
		}
		// Close rebuy and chat work require different treatment,
		// more than just clicking a simple region...
		if (p_autoplayer_functions->GetAautoplayerFunctionValue(k_autoplayer_function_close))
		{
			p_casino_interface->CloseWindow();
		}
		if (p_autoplayer_functions->GetAautoplayerFunctionValue(k_autoplayer_function_rebuy))
		{
			// ??? mutex twice!
			p_rebuymanagement->TryToRebuy();			
		}
		if (p_autoplayer_functions->GetAautoplayerFunctionValue(k_autoplayer_function_chat))
		{
			DoChat();
		}
	}
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


bool CAutoplayer::DoChat(void)
{
	if ((p_autoplayer_functions->f$chat() == 0) || (_the_chat_message == NULL))
		return false;
	if (!IsChatAllowed())
		return false;

	// Converting the result of the $chat-function to a string.
	// Will be ignored, if we already have an unhandled chat message.
	RegisterChatMessage(p_autoplayer_functions->f$chat()); 
	return true; //!!!p_casino_interface->EnterChatMessage(_the_chat_message);
}

void CAutoplayer::DoAllin(void)
{

	int number_of_clicks = 1; // Default is: single click with the mouse

	write_log(prefs.debug_autoplayer(), "[AutoPlayer] Starting DoAllin...\n");

	if (p_tablemap->buttonclickmethod() == BUTTON_DOUBLECLICK)
	{
		number_of_clicks = 2;
	}

	/*
		TM symbol allinmethod.
		0: swag the balance (default)
		1: click max (or allin), then raise
		2: click only max (or allin) [Spew and THF]
		3: use the slider
	*/

	if (p_tablemap->allinmethod() == 1)
	{
		// Clicking max (or allin) and then raise
		//!!! return values
		p_casino_interface->ClickButtonSequence(k_autoplayer_function_allin,
			k_autoplayer_function_raise, prefs.swag_delay_3());

		write_logautoplay(ActionConstantNames(k_action_allin));
	    p_heartbeat_thread->set_replay_recorded_this_turn(false);
	}
	else  if (p_tablemap->allinmethod() == 2)
	{
		p_casino_interface->ClickButton(k_autoplayer_function_allin);

		write_logautoplay(ActionConstantNames(k_action_allin));
	    p_heartbeat_thread->set_replay_recorded_this_turn(false);

	}
	else if (p_tablemap->allinmethod() == 3)
	{
		p_casino_interface->UseSliderForAllin();
	}
	// Third case (default): swagging the balance
	// (p_tablemap->allinmethod() == 0)
	// This will be handled by the swag-code.
	p_stableframescounter->ResetOnAutoplayerAction();
}

void CAutoplayer::DoAutoplayer(void) 
{
	write_log(prefs.debug_autoplayer(), "[AutoPlayer] Starting Autoplayer cadence...\n");

	// Check status of "Keyboard" menu item, and engage if necessary
	write_log(prefs.debug_autoplayer(), "[AutoPlayer] Calling CheckBringKeyboard.\n");
	CheckBringKeyboard();

	// Access TM objects !! Better name, better comment
	//!!!GetNeccessaryTablemapObjects();

	int	num_buttons_visible = 0; //!!!p_casino_interface->NumberOfVisibleButtons();
	/* TODO: better log-file format !!!
	write_log(prefs.debug_autoplayer(), "[AutoPlayer] Number of visible buttons: %d (%c%c%c%c%c)\n", 
		num_buttons_visible, 
		allin_option_available ? 'A' : '.',
		raise_button_available ? 'R' : '.',
		call_button_available  ? 'C' : '.',
		check_button_available ? 'K' : '.',
		fold_button_available  ? 'F' : '.');
	*/

	// Calculate f$play, f$prefold, f$rebuy, f$delay and f$chat for use below
	write_log(prefs.debug_autoplayer(), "[AutoPlayer] Calling CalcSecondaryFormulas.\n");
	p_autoplayer_functions->CalcSecondaryFormulas();

	// Handle f$sitin, f$sitout, f$leave (formerly f$play)
	// Also handling f$cloase here
	write_log(prefs.debug_autoplayer(), "[AutoPlayer] Calling DoF$Sitin_Sitout_Leave.\n");
	//!!!DoF$Sitin_Sitout_Leave();

	// Handle i86buttons
	write_log(prefs.debug_autoplayer(), "[AutoPlayer] Calling DoI86.\n");
	//!!!DoI86();

   
	bool isFinalAnswer = true;

	// check factors that affect isFinalAnswer status
	if (iter_vars.iterator_thread_running())
	{
		write_log(prefs.debug_autoplayer(), "[AutoPlayer] Not Final Answer because iterator_thread_running\n");
		isFinalAnswer = false;
	}

	// Change from only requiring one visible button (OpenHoldem 2008-04-03)
	if (num_buttons_visible < 2)
	{
		write_log(prefs.debug_autoplayer(), "[AutoPlayer] Not Final Answer because num_buttons_visible < 2\n");
		isFinalAnswer = false;
	}

	// if we are not playing (occluded?) 2008-03-25 Matrix
	if (!p_symbols->sym()->playing)
	{
		write_log(prefs.debug_autoplayer(), "[AutoPlayer] Not Final Answer because !p_symbols->sym()->playing\n");
		isFinalAnswer = false;
	}

	//  Avoiding unnecessary calls to p_stableframescounter->UpdateNumberOfStableFrames(),
	if (isFinalAnswer)
		p_stableframescounter->UpdateNumberOfStableFrames();

	write_log(prefs.debug_autoplayer(), "[AutoPlayer] Number of stable frames: % d\n", p_stableframescounter->NumberOfStableFrames());

	// Scale f$delay to a number of scrapes and avoid division by 0 and negative values
	unsigned int additional_frames_to_wait = (prefs.scrape_delay() > 0 && p_autoplayer_functions->f$delay() > 0 ? (p_autoplayer_functions->f$delay()/prefs.scrape_delay()) : 0);

	// If we don't have enough stable frames, or have not waited f$delay milliseconds, then return.
	if (p_stableframescounter->NumberOfStableFrames() < prefs.frame_delay() + additional_frames_to_wait)
	{
		write_log(prefs.debug_autoplayer(), "[AutoPlayer] Not Final Answer because we don't have enough stable frames, or have not waited f$delay (=%d ms)\n", (int)p_autoplayer_functions->f$delay());
		isFinalAnswer = false;
	}

	// If the game state processor didn't process this frame, then we should not act.
	if (!p_game_state->ProcessThisFrame ())
	{
		write_log(prefs.debug_autoplayer(), "[AutoPlayer] Not Final Answer because game state processor didn't process this frame\n");
		isFinalAnswer = false;
	}

	// Now that we got through all of the above, we are ready to evaluate the primary formulas
	// and take the appropriate action
	write_log(prefs.debug_autoplayer(), "[AutoPlayer] Calling CalcPrimaryFormulas with final answer.\n");
	p_symbols->set_sym_isfinalanswer(isFinalAnswer);
	p_autoplayer_functions->CalcPrimaryFormulas(isFinalAnswer);

	if(!isFinalAnswer)
	{
		return;
	}

	if (p_autoplayer_functions->f$alli())
	{
		DoAllin();
	}

	DoBetPot(); // !!!

	// do swag first since it is the odd one
	bool bDoSwag = false; // I'm just breaking this out to be a little clearer (spew)

	if ((p_tablemap->allinmethod() == 0) && p_autoplayer_functions->f$alli() && p_scraper->GetButtonState(k_button_i3)) //!!! //!!!
		bDoSwag = true;

	if (p_autoplayer_functions->f$betsize() && !p_autoplayer_functions->f$alli() && p_scraper->GetButtonState(k_button_i3)) //!!!
		bDoSwag = true;

	if (bDoSwag) 
	{
		write_log(prefs.debug_autoplayer(), "[AutoPlayer] Calling DoSwag.\n");
		DoSwag();
	}
	else 
	{
		write_log(prefs.debug_autoplayer(), "[AutoPlayer] Calling DoARCCF.\n");
		DoARCCF();
	}

	write_log(prefs.debug_autoplayer(), "[AutoPlayer] ...ending Autoplayer cadence.\n");
}

void CAutoplayer::DoSwag(void) 
{
	if (p_autoplayer_functions->f$betsize() > 0)
	{
		p_casino_interface->EnterBetsize(p_autoplayer_functions->f$betsize());
	}
}

void CAutoplayer::DoARCCF(void) 
{
	ActionConstant	do_click = k_action_undefined;
	HWND			hwnd_focus = GetFocus();
	POINT			cur_pos = {0};
	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

	int				sym_myturnbits = (int) p_symbols->sym()->myturnbits;

	write_log(prefs.debug_autoplayer(), "[AutoPlayer] Starting DoARCCF...\n");

	RECT			r;

	do_click = k_action_undefined;

	// ALLIN
//!!!	if (p_autoplayer_functions->f$alli() && sym_myturnbits&0x8 && allin_button_available)
	{
//!!!		r = allin_button;
		do_click = k_action_allin;
		
	}

	// RAISE
//!!!	else if (p_autoplayer_functions->f$rais() && sym_myturnbits&0x4 && raise_button_available)
	{
//!!!		r = raise_button;
		do_click = k_action_raise;
	
	}

	// CALL
//!!!	else if (p_autoplayer_functions->f$call() && sym_myturnbits&0x1 && call_button_available)
	{
//!!!		r = call_button;
		do_click = k_action_call;
	
	}

	// CHECK
	// None of f$alli, f$betsize, f$rais, f$call are > 0 or no buttons related to
	// these actions can be found. If there is a check button, then click it.
//!!!	else if (check_button_available)
	{
//!!!		r = check_button;
		do_click = k_action_check;
		
	}

	// FOLD
	// None of f$alli, f$betsize, f$rais, f$call are > 0 or no buttons related to
	// these actions can be found. If there is a fold button, then click it, otherwise we have a serious problem.
//!!!	else if (fold_button_available)
	{
//!!!		r = fold_button;
		do_click = k_action_fold;
		
	}

	if (do_click == k_action_undefined)
	{
		
		return;
	}

	else
	{


		//Mutex locked -> Click_delay
		Sleep(prefs.click_delay());
		
		if (p_tablemap->buttonclickmethod() == BUTTON_DOUBLECLICK)
		{
			
			(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), r, MouseLeft, 2, hwnd_focus, cur_pos);
		}
		else
		{
			(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), r, MouseLeft, 1, hwnd_focus, cur_pos);
		}
		

		p_stableframescounter->ResetOnAutoplayerAction();

		// record did*/prevaction
		int sym_br = (int) p_symbols->sym()->br;

		// Writing 4-digit-name of action, e.g "ALLI" or "RAIS" to the log.
		write_logautoplay(ActionConstantNames(do_click));
		p_symbols->RecordPrevAction(do_click);
		
		//???Missing in rev. 1944
		//p_heartbeat_thread->set_replay_recorded_this_turn(false);
	}


	
	// !!! Remove hard-coded constants
	//!!!p_symbols->sym()->didrais[4], p_symbols->sym()->didcall[4], p_symbols->sym()->didchec[4]);
}

void CAutoplayer::DoSlider(void) 
{
	
}

void CAutoplayer::DoPrefold(void) 
{
	HWND			hwnd_focus = GetFocus();
	POINT			cur_pos = {0};
	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

	write_log(prefs.debug_autoplayer(), "[AutoPlayer] Starting DoPrefold...\n");



	if (p_autoplayer_functions->f$prefold() == 0)  
		return;

		p_casino_interface->ClickButton(k_autoplayer_function_prefold);

		p_symbols->RecordPrevAction(k_action_fold);
		write_logautoplay(ActionConstantNames(k_action_fold));
	p_autoplayer_functions->CalcAutoTrace();
	write_log(prefs.debug_autoplayer(), "[AutoPlayer] ...ending DoPrefold.\n");
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
		}
	}
}

bool CAutoplayer::HandleInterfacebuttonsI86(void) 
{
	for (int i=0; i<k_max_number_of_i86X_buttons; i++)
	{
		if (p_casino_interface->ClickI86ButtonIfAvailable(i))
		{
			return true;
		}
	}
	return false;
}
