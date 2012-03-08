#include "stdafx.h"
#include "CCasinoInterface.h"

#include "..\CTableMap\CTableMapAccess.h"
#include "CAutoConnector.h"
#include "CAutoplayerFunctions.h"
#include "CHeartbeatThread.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CSymbols.h"
#include "MagicNumbers.h"
#include "MainFrm.h"
#include "OpenHoldem.h"
#include "SwagAdjustment.h"


CCasinoInterface *p_casino_interface = NULL;


CCasinoInterface::CCasinoInterface()
{
	// dummy point for mouse and keyboard DLL
	p_null.x = 0;
	p_null.y = 0;
}

CCasinoInterface::~CCasinoInterface()
{}

// !!! todo? debug-prfeferences?

bool CCasinoInterface::TableLostFocues()
{
	return (GetForegroundWindow() != p_autoconnector->attached_hwnd());
}


void CCasinoInterface::ClickRect(RECT rect)
{
	write_log(prefs.debug_autoplayer(), "[CasinoInterface] Calling mouse.dll to single click button: %d,%d %d,%d\n", 
		rect.left, rect.top, rect.right, rect.bottom);	
	(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), rect, MouseLeft, 
		1, GetFocus(), p_null);
	p_symbols->reset_elapsedautohold(); // ???
}

bool CCasinoInterface::ClickButton(int button_code)
{
	//write_log(prefs.debug_autoplayer(), "[CasinoInterface]  
	if (1) // buttonstate !!!
	{
		ClickRect(action_buttons[button_code]);
		write_log(prefs.debug_autoplayer(), "[CasinoInterface] Clicked button %s\n",
			k_autoplayer_functionname[button_code]);
		return true;
	}
	else
	{
		write_log(prefs.debug_autoplayer(), "[CasinoInterface] Could not click button %s\n",
			k_autoplayer_functionname[button_code]);
		return false;
	}
}

bool CCasinoInterface::ClickButtonSequence(int first_button_code, int second_button_code, 
	int delay_in_milli_seconds)
{
	if (ClickButton(first_button_code))
	{
		Sleep(delay_in_milli_seconds);
		return ClickButton(second_button_code);
	}
	return false;
}

bool CCasinoInterface::UseSliderForAllin()
{

	POINT			cur_pos = {0};


	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

	POINT			point_null = {-1, -1};
	RECT			r_null = {-1, -1, -1, -1};

	write_log(prefs.debug_autoplayer(), "[AutoPlayer] Starting DoSlider...\n");


	if (!(i3_slider_defined && i3_handle_defined))
	{
		write_log(prefs.debug_autoplayer(), "[AutoPlayer] ...ending DoSlider early (i3handle or i3slider are not defined in the tablemap)\n");
		return false;
	}

	if (!p_scraper->handle_found_at_xy())
	{
		write_log(prefs.debug_autoplayer(), "[AutoPlayer] ...ending DoSlider early (handle not found - i3handle must use a transform that resolves to either 'handle' or 'true')\n");
		return false;
	}

	// Click and drag handle
	RECT r;
	r.left      = p_scraper->handle_xy().x + ((i3_handle_region.right - i3_handle_region.left)/2);
	r.top       = p_scraper->handle_xy().y + ((i3_handle_region.bottom - i3_handle_region.top)/2);
	r.right     = p_scraper->handle_xy().x + (i3_slider_region.right - i3_slider_region.left);
	r.bottom    = r.top;		

	write_log(prefs.debug_autoplayer(), "[AutoPlayer] Slider : Calling mouse.dll to jam from %d,%d to %d,%d\n", r.left, r.top, r.right, r.bottom);
	(theApp._dll_mouse_click_drag) (p_autoconnector->attached_hwnd(), r, NULL, point_null);

	write_log(prefs.debug_autoplayer(), "[AutoPlayer] Sleeping %d ms\n.", prefs.swag_delay_3());
	Sleep(prefs.swag_delay_3());

	// Click confirmation button
	if (p_tablemap->swagconfirmationmethod() == BETCONF_ENTER)
	{
		write_log(prefs.debug_autoplayer(), "[AutoPlayer] Slider Confirmation : calling keyboard.dll to press 'Enter'\n");
		(theApp._dll_keyboard_sendkey) (p_autoconnector->attached_hwnd(), r_null, VK_RETURN, GetFocus(), cur_pos);
	}

	else if (p_tablemap->swagconfirmationmethod() == BETCONF_CLICKBET &&
			 (raise_button_available || i3_button_available))
	{
		RECT rect_button;

		// use allin button if it exists, otherwise use i3button region if it exists, 
		// otherwise use the bet/raise button region
		if (allin_button_available)
		{
			rect_button = allin_button;
		}
		else if (i3_button_available) 
		{
			write_log(prefs.debug_autoplayer(), "[AutoPlayer] Slider Confirmation : Using the i3button\n");
			rect_button = i3_button;
		}
		else
		{
			write_log(prefs.debug_autoplayer(), "[AutoPlayer] Slider Confirmation : Using the raise button\n");
			rect_button = raise_button;
		}

		if (p_tablemap->buttonclickmethod() == BUTTON_DOUBLECLICK)
		{
			write_log(prefs.debug_autoplayer(), "[AutoPlayer] Slider Confirmation : calling mouse.dll to double click bet button: %d,%d %d,%d\n", 
					rect_button.left, rect_button.top, rect_button.right, rect_button.bottom);
			(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), rect_button, MouseLeft, 2, GetFocus(), cur_pos);
		}
		else
		{
			write_log(prefs.debug_autoplayer(), "[AutoPlayer] Slider Confirmation : calling mouse.dll to single click bet button: %d,%d %d,%d\n", 
					rect_button.left, rect_button.top, rect_button.right, rect_button.bottom);
			(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), rect_button, MouseLeft, 1, GetFocus(), cur_pos);
		}
	}

	else
	{
		write_log(prefs.debug_autoplayer(), "[AutoPlayer] ...ending DoSlider early (invalid swagconfirmationmethod).\n");
		return false;
	}


	write_logautoplay(ActionConstantNames(k_action_jam));
	write_log(prefs.debug_autoplayer(), "[AutoPlayer] Jam complete: %d,%d,%d,%d\n", r.left, r.top, r.right, r.bottom);

	p_heartbeat_thread->set_replay_recorded_this_turn(false);

	// reset elapsedauto symbol
	
	

	write_log(prefs.debug_autoplayer(), "[AutoPlayer] ...ending DoSlider.\n");
	return true;
}

bool CCasinoInterface::CloseWindow()
{
	// Hard-coded click to the "X" at the top-right
	// of the title-bar
	// ToDo (maybe): make it work for different settings.

	RECT table_size, close_region;
	// http://msdn.microsoft.com/en-us/library/ms633503.aspx
	GetClientRect(p_autoconnector->attached_hwnd(), &table_size);

	close_region.top    = -3;
	close_region.bottom = -15;
	close_region.left   = table_size.right - 18;
	close_region.right  = table_size.right -  6;
	
	write_log(prefs.debug_autoplayer(), "[AutoPlayer] f$close is true.\n");
	write_log(prefs.debug_autoplayer(), "[AutoPlayer] preparing to execute f$close.\n");
	ClickRect(close_region);
	return true;
}

bool CCasinoInterface::EnterChatMessage(char *message)
{
	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);
	RECT			rect_chatbox;
	POINT			cur_pos = {0};

	if (!p_tablemap_access->GetTableMapRect("chatbox", &rect_chatbox))
	{
		write_log(prefs.debug_autoplayer(), "[AutoPlayer] Can't chat. No region defined.\n");
		return false;
	}
	write_log(prefs.debug_autoplayer(), "[AutoPlayer] Sending chat-message: %s\n", message);
	(theApp._dll_keyboard_sendstring) (p_autoconnector->attached_hwnd(), rect_chatbox, message, false, GetFocus(), cur_pos);

	// Clear old chat_message to allow new ones.
	//!!!_the_chat_message = NULL;
	//!!!ComputeFirstPossibleNextChatTime();
	return true;
}

bool CCasinoInterface::ClickI86ButtonIfAvailable(int button_number)
{
	assert(button_number >= 0);
	assert(button_number < k_max_number_of_i86X_buttons);

	if (i86X_button_available[button_number] && _i86X_state[button_number])
	{
		ClickRect(i86X_button[button_number]);
		write_log(prefs.debug_autoplayer(), "[AutoPlayer] Found valid i86 (%d) button and clicked it.\n", button_number);
		return true;
	}
	else
	{
		return false;
	}
}

int CCasinoInterface::NumberOfVisibleButtons()
{
	// Buttons for playing actions, e.g. fold or allin.
	// There have to be at least 2 to make it our turn.
	int number_of_visible_buttons = 0
		+ (allin_option_available ? 1 : 0)
		+ (raise_button_available ? 1 : 0)
		+ (call_button_available  ? 1 : 0)
		+ (check_button_available ? 1 : 0)
		+ (fold_button_available  ? 1 : 0);
	return number_of_visible_buttons;
}

void CCasinoInterface::GetNeccessaryTablemapObjects()
{
	p_scraper_access->SetScraperAccessData();
		
	// iX
	allin_button_defined    = p_tablemap_access->GetButtonRect(p_scraper_access->get_allin_button_name(), &allin_button);
	allin_button_available  = allin_button_defined && p_scraper_access->get_allin_button_visible();

	raise_button_defined    = p_tablemap_access->GetButtonRect(p_scraper_access->get_raise_button_name(), &raise_button);
	raise_button_available  = raise_button_defined && p_scraper_access->get_raise_button_visible();

	call_button_defined     = p_tablemap_access->GetButtonRect(p_scraper_access->get_call_button_name(), &call_button);
	call_button_available   = call_button_defined && p_scraper_access->get_call_button_visible();

	check_button_defined    = p_tablemap_access->GetButtonRect(p_scraper_access->get_check_button_name(), &check_button);
	check_button_available  = check_button_defined && p_scraper_access->get_check_button_visible();

	fold_button_defined     = p_tablemap_access->GetButtonRect(p_scraper_access->get_fold_button_name(), &fold_button);
	fold_button_available   = fold_button_defined && p_scraper_access->get_fold_button_visible();

	sitin_button_defined    = p_tablemap_access->GetButtonRect(p_scraper_access->get_sitin_button_name(), &sitin_button);
	sitin_button_available  = sitin_button_defined && p_scraper_access->get_sitin_button_visible();

	sitout_button_defined   = p_tablemap_access->GetButtonRect(p_scraper_access->get_sitout_button_name(), &sitout_button);
	sitout_button_available = sitout_button_defined && p_scraper_access->get_sitout_button_visible();

	leave_button_defined    = p_tablemap_access->GetButtonRect(p_scraper_access->get_leave_button_name(), &leave_button);
	leave_button_available  = leave_button_defined && p_scraper_access->get_leave_button_visible();

	// i86
	i86_button_defined      = p_tablemap_access->GetButtonRect("i86button", &i86_button);
	i86_button_available    = i86_button_defined && p_scraper_access->get_i86_button_visible();

	for (int i = 0; i < k_max_number_of_i86X_buttons; i++)
	{
		i86X_button_defined[i]     = p_tablemap_access->GetButtonRect(p_scraper_access->get_i86X_button_name(i), &i86X_button[i]);
		i86X_button_available[i]   = i86X_button_defined[i] && p_scraper_access->get_i86X_button_visible(i);
	}

	CString betpot_X_Y_name;
	/*!!!for (int x = 0; x < k_max_number_of_betpot_X_y_buttons; x++)
	{
		for (int y = 0; y < k_max_number_of_betpot_x_Y_buttons; y++)
		{
			betpot_X_Y_name.Format("betpot_%d_%d_button", x+1, y+1);
			betpot_X_Y_button_defined[x][y]     = p_tablemap_access->GetButtonRect(betpot_X_Y_name, &betpot_X_Y_button[x][y]);
			betpot_X_Y_button_available[x][y]   = betpot_X_Y_button_defined[x][y] && p_scraper_access->get_betpot_button_visible(x, y);
		}
	}*/

	// i3
	i3_button_defined   = p_tablemap_access->GetButtonRect("i3button", &i3_button);
	i3_button_available = i3_button_defined && p_scraper_access->get_i3_button_visible();

	i3_edit_defined     = p_tablemap_access->GetTableMapRect("i3edit", &i3_edit_region);
	i3_slider_defined   = p_tablemap_access->GetTableMapRect("i3slider", &i3_slider_region);
	i3_handle_defined   = p_tablemap_access->GetTableMapRect("i3button", &i3_handle_region);

	allin_option_available = false;
	
	if (i3_button_available)
		allin_option_available = true;
	if (p_scraper_access->get_i3_button_visible() && allin_button_available)
		allin_option_available = true;
	if (p_scraper_access->get_i3_button_visible() && i3_edit_defined)
		allin_option_available = true;
	if (p_scraper_access->get_i3_button_visible() && i3_slider_defined && i3_handle_defined)
		allin_option_available = true;
}

void CCasinoInterface::SelectSwagText()
{
	if (p_tablemap->swagselectionmethod() == TEXTSEL_DOUBLECLICK)
	{
		write_log(prefs.debug_autoplayer(), "[AutoPlayer] Text selection; calling mouse.dll to double click: %d,%d %d,%d\n", 
			i3_edit_region.left, i3_edit_region.top, i3_edit_region.right, i3_edit_region.bottom);
		(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), i3_edit_region, MouseLeft, 2, NULL, p_null);
	}

	else if (p_tablemap->swagselectionmethod() == TEXTSEL_SINGLECLICK)
	{
		write_log(prefs.debug_autoplayer(), "[AutoPlayer] Text selection; calling mouse.dll to single click: %d,%d %d,%d\n", 
			i3_edit_region.left, i3_edit_region.top, i3_edit_region.right, i3_edit_region.bottom);
		(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), i3_edit_region, MouseLeft, 1, NULL, p_null);
	}

	else if (p_tablemap->swagselectionmethod() == TEXTSEL_CLICKDRAG)
	{
		write_log(prefs.debug_autoplayer(), "[AutoPlayer] Text selection; calling mouse.dll to click drag: %d,%d %d,%d\n", 
			i3_edit_region.left, i3_edit_region.top, i3_edit_region.right, i3_edit_region.bottom);
		(theApp._dll_mouse_click_drag) (p_autoconnector->attached_hwnd(), i3_edit_region, NULL, p_null);
	}

	else if (p_tablemap->swagselectionmethod() == TEXTSEL_NOTHING)
	{
	}
	else
	{
		write_log(prefs.debug_autoplayer(), "[AutoPlayer] ...ending DoSwag early (invalid swagselectionmethod).\n");
		return;
	}
}

void CCasinoInterface::DeleteSwagText()
{
	if (p_tablemap->swagdeletionmethod() == TEXTDEL_DELETE)
	{
		write_log(prefs.debug_autoplayer(), "[AutoPlayer] Text deletion; calling keyboard.dll to press 'delete'\n");
		//!!!(theApp._dll_keyboard_sendkey) (p_autoconnector->attached_hwnd(), r_null, VK_DELETE, NULL, p_null);
	}
	else if (p_tablemap->swagdeletionmethod() == TEXTDEL_BACKSPACE)
	{
		write_log(prefs.debug_autoplayer(), "[AutoPlayer] Text deletion; calling keyboard.dll to press 'backspace'\n");
		//!!!(theApp._dll_keyboard_sendkey) (p_autoconnector->attached_hwnd(), r_null, VK_BACK, NULL, p_null);
	}
	else if (p_tablemap->swagdeletionmethod() == TEXTDEL_NOTHING)
	{
	}
}


bool CCasinoInterface::EnterBetsize(double total_betsize_in_dollars)
{
	POINT			cur_pos = {0};
	bool			lost_focus = false;
	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);
	POINT			point_null = {-1, -1};
	RECT			r_null = {-1, -1, -1, -1};
	CString			swag_amt;

	write_log(prefs.debug_autoplayer(), "[AutoPlayer] Starting DoSwag...\n");

	// swag regions are hard coded as #3 for now, due to legacy WH standard
	if (!i3_edit_defined)
	{
		write_log(prefs.debug_autoplayer(), "[AutoPlayer] ...ending DoSwag early (no edit field).\n");
		return false;
	}
	SelectSwagText();
	

	// First sleep(), THEN check for stolen foxus
	Sleep(prefs.swag_delay_1()); 
	// Check for stolen , and thus misswag
	if (GetForegroundWindow() != p_autoconnector->attached_hwnd())
		lost_focus = true;

	write_log(prefs.debug_autoplayer(), "[AutoPlayer] Sleeping %dms.\n", prefs.swag_delay_1());
	
	DeleteSwagText();

	

	// Check for stolen focus, and thus misswag
	if (GetForegroundWindow() != p_autoconnector->attached_hwnd())
		lost_focus = true;

	write_log(prefs.debug_autoplayer(), "[AutoPlayer] Sleeping %dms.\n", prefs.swag_delay_2());
	Sleep(prefs.swag_delay_2());

	// if we are swagging allin then set the swag value to be our balance (spew)
	// !!! To be handled in allin

	// SWAG AMOUNT ENTRY
	double swag_adjusted = SwagAmountAdjusted(p_autoplayer_functions->f$betsize());
	if (swag_adjusted != (int) swag_adjusted)
		swag_amt.Format("%.2f", swag_adjusted);
	else
		swag_amt.Format("%.0f", swag_adjusted);

	write_log(prefs.debug_autoplayer(), "[AutoPlayer] Swag amount (not adjusted): %.2f\n", p_autoplayer_functions->f$betsize());
	write_log(prefs.debug_autoplayer(), "[AutoPlayer] Swag amount; calling keyboard.dll to swag (adjusted): %s %d,%d %d,%d\n", 
		swag_amt, i3_edit_region.left, i3_edit_region.top, i3_edit_region.right, i3_edit_region.bottom);
	(theApp._dll_keyboard_sendstring) (p_autoconnector->attached_hwnd(), i3_edit_region, swag_amt, prefs.swag_use_comma(), NULL, point_null);

	// Check for stolen focus, and thus misswag
	if (GetForegroundWindow() != p_autoconnector->attached_hwnd())
		lost_focus = true;

	write_log(prefs.debug_autoplayer(), "[AutoPlayer] Sleeping %dms.\n", prefs.swag_delay_3());
	Sleep(prefs.swag_delay_3());

	// BET CONFIRMATION ACTION
	if (!lost_focus || !prefs.focus_detect())
	{
		if (p_tablemap->swagconfirmationmethod() == BETCONF_ENTER)
		{
			write_log(prefs.debug_autoplayer(), "[AutoPlayer] Confirmation; calling keyboard.dll to press 'Enter'\n");					
			(theApp._dll_keyboard_sendkey) (p_autoconnector->attached_hwnd(), r_null, VK_RETURN, GetFocus(), cur_pos);
		}
		else if (p_tablemap->swagconfirmationmethod() == BETCONF_CLICKBET &&
				 (raise_button_available || i3_button_available))
		{
			RECT rect_button;

			// use i3button region if it exists, otherwise use the bet/raise button region
			if (i3_button_available)
			{
				rect_button = i3_button;
				write_log(prefs.debug_autoplayer(), "[AutoPlayer] Bet Confirmation: Using i3button\n");
			}
			else
			{
				rect_button = raise_button;
				write_log(prefs.debug_autoplayer(), "[AutoPlayer] Bet Confirmation: Using raise button\n");
			}

			if (p_tablemap->buttonclickmethod() == BUTTON_DOUBLECLICK)
			{
				write_log(prefs.debug_autoplayer(), "[AutoPlayer] Confirmation; calling mouse.dll to double click bet button: %d,%d %d,%d\n", 
					rect_button.left, rect_button.top, rect_button.right, rect_button.bottom);					
				(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), rect_button, MouseLeft, 2, GetFocus(), cur_pos);
			}
			else
			{
				write_log(prefs.debug_autoplayer(), "[AutoPlayer] Confirmation; calling mouse.dll to single click bet button: %d,%d %d,%d\n", 
					rect_button.left, rect_button.top, rect_button.right, rect_button.bottom);					
				(theApp._dll_mouse_click) (p_autoconnector->attached_hwnd(), rect_button, MouseLeft, 1, GetFocus(), cur_pos);
			}
		}

		else if (p_tablemap->swagconfirmationmethod() == BETCONF_NOTHING)
		{
		}

		else
		{
			write_log(prefs.debug_autoplayer(), "[AutoPlayer] ...ending DoSwag early (invalid swagconfirmationmethod).\n");
			return false;
		}
		
		p_symbols->RecordPrevAction(k_action_swag);
		write_logautoplay(ActionConstantNames(k_action_swag));

		p_heartbeat_thread->set_replay_recorded_this_turn(false);
	}

	write_log(prefs.debug_autoplayer(), "[AutoPlayer] ...ending DoSwag, 'didswag' now: %d\n", p_symbols->sym()->didswag[4]);
}