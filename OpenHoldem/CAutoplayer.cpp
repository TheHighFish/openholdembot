/* TODO: 
* get mutex
* get focus
* get mouse
* act
* restore mouse
* prevaction
* p_stableframescounter->ResetOnAutoplayerAction();
* p_symbols->reset_elapsedautohold();
* delay
* check for loss of swag-focus: first sleep, then check, then act,
  NOT the other way: http://www.maxinmontreal.com/forums/viewtopic.php?f=120&t=14791
*/

#include "StdAfx.h"
#include "CAutoplayer.h"

#include <complex>
#include "BringKeyboard.h"
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
			if (p_tablemap->betpotmethod() == BETPOT_RAISE)
			{
				return p_casino_interface->ClickButtonSequence(i, k_autoplayer_function_raise, /*betpot_delay* !!! */ 1);
			}
			else 
			{
				// Default: click only betpot
				return p_casino_interface->ClickButton(i);				
			}
		}
	}
	// We didn't click any betpot-button
	return false;
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

bool CAutoplayer::ExecutePrimaryFormulas() 
{
	write_log(prefs.debug_autoplayer(), "[AutoPlayer] ExecutePrimaryFormulas()\n");
	// Precondition: my turn and isfinalanswer
	// So we have to take an action and are able to do so.
	if (p_autoplayer_functions->f$alli())
	{
		return DoAllin();
	}
	if (DoBetPot())
	{
		return true;
	}
	return ExecuteRaiseCallCheckFold();
}

bool CAutoplayer::ExecuteRaiseCallCheckFold()
{
	write_log(prefs.debug_autoplayer(), "[AutoPlayer] ExecuteRaiseCallCheckFold()\n");
	if (p_autoplayer_functions->f$rais()
		&& p_casino_interface->ClickButton(k_button_raise))
	{
		return true;
	}
	else if (p_autoplayer_functions->f$call()
		&& p_casino_interface->ClickButton(k_autoplayer_function_call))
	{
		return true;
	}
	// Try to check
	else if (p_casino_interface->ClickButton(k_autoplayer_function_check))
	{
		return true;
	}
	else
	{
		// Otherwise: fold
		return p_casino_interface->ClickButton(k_autoplayer_function_fold);
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
	return p_casino_interface->EnterChatMessage(CString(_the_chat_message));
}

bool CAutoplayer::DoAllin(void)
{
	bool success = false;
	write_log(prefs.debug_autoplayer(), "[AutoPlayer] Starting DoAllin...\n");

	int number_of_clicks = 1; // Default is: single click with the mouse
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
		success = p_casino_interface->ClickButtonSequence(k_autoplayer_function_allin,
			k_autoplayer_function_raise, prefs.swag_delay_3());

		write_logautoplay(ActionConstantNames(k_action_allin));
	    p_heartbeat_thread->set_replay_recorded_this_turn(false);
		return success;
	}
	else  if (p_tablemap->allinmethod() == 2)
	{
		success = p_casino_interface->ClickButton(k_autoplayer_function_allin);

		write_logautoplay(ActionConstantNames(k_action_allin));
	    p_heartbeat_thread->set_replay_recorded_this_turn(false);
		return success;
	}
	else if (p_tablemap->allinmethod() == 3)
	{
		success = p_casino_interface->UseSliderForAllin();
		write_logautoplay(ActionConstantNames(k_action_allin));
	    p_heartbeat_thread->set_replay_recorded_this_turn(false);
		return success;
	}
	else
	{
		// Fourth case (default = 0): swagging the balance
		double betsize_for_allin = p_symbols->sym()->currentbet[10 /*!!!*/]
			+ p_symbols->sym()->balance[10]; //!!!
		p_casino_interface->EnterBetsize(betsize_for_allin);
	}
}

void CAutoplayer::DoAutoplayer(void) 
{
	write_log(prefs.debug_autoplayer(), "[AutoPlayer] Starting Autoplayer cadence...\n");

	CheckBringKeyboard();

	p_scraper_access->GetNeccessaryTablemapObjects();
	int	num_buttons_visible = p_casino_interface->NumberOfVisibleAutoplayerButtons();
	/* TODO: better log-file format !!! 	
	write_log(prefs.debug_autoplayer(), "[AutoPlayer] Number of visible buttons: %d (%c%c%c%c%c)\n", 
		num_buttons_visible, 
		allin_option_available ? 'A' : '.',
		raise_button_available ? 'R' : '.',
		call_button_available  ? 'C' : '.',
		check_button_available ? 'K' : '.',
		fold_button_available  ? 'F' : '.');*/

	// Calculate f$play, f$prefold, f$rebuy, f$delay and f$chat for use below
	write_log(prefs.debug_autoplayer(), "[AutoPlayer] Calling CalcSecondaryFormulas.\n");
	p_autoplayer_functions->CalcSecondaryFormulas();

	// Handle f$sitin, f$sitout, f$leave (formerly f$play)
	// Also handling f$cloase here
	write_log(prefs.debug_autoplayer(), "[AutoPlayer] Calling DoF$Sitin_Sitout_Leave.\n");
	//!!!DoF$Sitin_Sitout_Leave();

  
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
	ExecutePrimaryFormulas();


/* !!!!
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
*/

	write_log(prefs.debug_autoplayer(), "[AutoPlayer] ...ending Autoplayer cadence.\n");
}

void CAutoplayer::DoSwag(void) 
{
	if (p_autoplayer_functions->f$betsize() > 0)
	{
		p_casino_interface->EnterBetsize(p_autoplayer_functions->f$betsize());
	}
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
