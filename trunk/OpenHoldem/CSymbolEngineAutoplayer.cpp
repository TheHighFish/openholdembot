#include "stdafx.h"
#include "CSymbolEngineAutoplayer.h"

#include "CAutoconnector.h"
#include "CAutoplayerFunctions.h"
#include "CCasinoInterface.h"
#include "CGameState.h"
#include "CIteratorThread.h"
#include "CIteratorVars.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CStableFramesCounter.h"
#include "CStringMatch.h"
#include "MagicNumbers.h"
#include "NumericalFunctions.h"

CSymbolEngineAutoplayer *p_symbol_engine_autoplayer = NULL;

CSymbolEngineAutoplayer::CSymbolEngineAutoplayer()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	//
	// This engine does not use any other engines.
}

CSymbolEngineAutoplayer::~CSymbolEngineAutoplayer()
{}

void CSymbolEngineAutoplayer::InitOnStartup()
{
	_myturnbits      = 0;
	_issittingin     = false;
	_isautopost      = false;
	_isfinalanswer   = false;
}

void CSymbolEngineAutoplayer::ResetOnConnection()
{
	_myturnbits      = 0;
	_issittingin     = false;
	_isautopost      = false;
	_isfinalanswer   = false;
	_last_myturnbits = 0;
	DetectSpecialConnectionLikeBringAndManualMode();
}

void CSymbolEngineAutoplayer::ResetOnHandreset()
{
	_isfinaltable  = false;
}

void CSymbolEngineAutoplayer::ResetOnNewRound()
{}

void CSymbolEngineAutoplayer::ResetOnMyTurn()
{}

void CSymbolEngineAutoplayer::ResetOnHeartbeat()
{
	_last_myturnbits = _myturnbits;
	_myturnbits      = 0;
	_issittingin     = false;
	_isautopost      = false;
	_isfinalanswer   = false;
	CalculateMyTurnBits();
	CalculateSitInState();
	CalculateFinalAnswer();
}

void CSymbolEngineAutoplayer::CalculateMyTurnBits()
{
	write_log(preferences.debug_symbolengine(), "[CSymbolEngineAutoplayer] myturnbits reset: %i\n", _myturnbits);
	for (int i=0; i<k_max_number_of_buttons; i++)
	{
		if (p_scraper->GetButtonState(i))
		{
			CString button_label = p_scraper->button_label(i);

			if (p_string_match->IsStringFold(button_label))
			{
				_myturnbits |= (1<<0);
			}
			else if (p_string_match->IsStringCall(button_label))
			{
				_myturnbits |= (1<<1);
			}
			else if (p_string_match->IsStringRaise(button_label) || button_label.MakeLower() == "swag")
			{
				_myturnbits |= (1<<2);
			}
			else if (p_string_match->IsStringCheck(button_label))
			{
				_myturnbits |= (1<<4);
			}
			else if (p_string_match->IsStringAllin(button_label))
			{
				_myturnbits |= (1<<3);
			}
			else if (p_string_match->IsStringAutopost(button_label))
			{
				_isautopost = true;
			}
		}
	}
	write_log(preferences.debug_symbolengine(), "[CSymbolEngineAutoplayer] myturnbits now: %i\n", _myturnbits);
}

void CSymbolEngineAutoplayer::CalculateSitInState()
{
	for (int i=0; i<k_max_number_of_buttons; i++)
	{
		if (p_string_match->IsStringSitin(p_scraper->button_label(i)))
		{
			// Sitin-button found
			_issittingin = p_scraper->GetButtonState(i);
			return;
		}

		else if (p_string_match->IsStringSitout(p_scraper->button_label(i)))
		{
			// Sitout-button found
			_issittingin = !(p_scraper->GetButtonState(i));
			return;
		}
	}
	// we have neither a sitout or sitin button
	_issittingin = true;
}

void CSymbolEngineAutoplayer::DetectSpecialConnectionLikeBringAndManualMode()
{
	_isbring  = false;
	_ismanual = false;
	const int k_max_length_of_classname = 50;
	char classname[k_max_length_of_classname] = "";

	GetClassName(p_autoconnector->attached_hwnd(), classname, k_max_length_of_classname);
	if (strcmp(classname, "BRING")==0)
	{
		_isbring = true;																
	}
	else if (strcmp(classname, "OpenHoldemManualMode")==0)
	{
		_ismanual = true;
	}
}

void CSymbolEngineAutoplayer::CalculateFinalAnswer()
{
	// [IMPERFECT CODE] Updates stable-frames-counter as a side-effect
	// and should therefore only get called once per heartbeat.

	_isfinalanswer = true;
	// check factors that affect isFinalAnswer status
	if (iter_vars.iterator_thread_running())
	{
		write_log(preferences.debug_autoplayer(), "[AutoPlayer] Not Final Answer because iterator_thread still running\n");
		_isfinalanswer = false;
	}

	// Change from only requiring one visible button (OpenHoldem 2008-04-03)
	if (p_casino_interface->NumberOfVisibleAutoplayerButtons() < k_min_buttons_needed_for_my_turn)
	{
		write_log(preferences.debug_autoplayer(), "[AutoPlayer] Not Final Answer because too few buttons visible\n");
		write_log(preferences.debug_autoplayer(), "[AutoPlayer] Buttons visible: %i\n", p_casino_interface->NumberOfVisibleAutoplayerButtons());
		_isfinalanswer = false;
	}

	// if we are not playing (occluded?) 2008-03-25 Matrix
	if (!p_scraper_access->UserHasCards())
	{
		write_log(preferences.debug_autoplayer(), "[AutoPlayer] Not Final Answer because the user is \"not playing\"\n");
		write_log(preferences.debug_autoplayer(), "[AutoPlayer] Possibly a tablemap-problem\n");
		_isfinalanswer = false;
	}

	//  Avoiding unnecessary calls to p_stableframescounter->UpdateNumberOfStableFrames(),
	if (_isfinalanswer)
	{
		p_stableframescounter->UpdateNumberOfStableFrames();
	}

	write_log(preferences.debug_autoplayer(), "[AutoPlayer] Number of stable frames: % d\n", p_stableframescounter->NumberOfStableFrames());
	// Scale f$delay to a number of scrapes and avoid division by 0 and negative values
	unsigned int additional_frames_to_wait = (preferences.scrape_delay() > 0 && p_autoplayer_functions->f$delay() > 0 ? (p_autoplayer_functions->f$delay()/preferences.scrape_delay()) : 0);

	// If we don't have enough stable frames, or have not waited f$delay milliseconds, then return.
	if (p_stableframescounter->NumberOfStableFrames() < preferences.frame_delay() + additional_frames_to_wait)
	{
		write_log(preferences.debug_autoplayer(), "[AutoPlayer] Not Final Answer because we don't have enough stable frames, or have not waited f$delay (=%d ms)\n", (int)p_autoplayer_functions->f$delay());
		_isfinalanswer = false;
	}

	// If the game state processor didn't process this frame, then we should not act.
	if (!p_game_state->ProcessThisFrame ())
	{
		write_log(preferences.debug_autoplayer(), "[AutoPlayer] Not Final Answer because game state processor didn't process this frame\n");
		write_log(preferences.debug_autoplayer(), "[AutoPlayer] Most common reason: missing balance-stability or card-stability.\n");
		_isfinalanswer = false;
	}
}

CString CSymbolEngineAutoplayer::GetFCKRAString()
{
	// Buttons visible (Fold, Call, Check, Raise, Allin)
	CString fckra_seen;
	fckra_seen.Format("%s%s%s%s%s",
		IsBitSet(_myturnbits, 0) ? "F" : ".",
		IsBitSet(_myturnbits, 1) ? "C" : ".",
		// Check button out of order to stay consistent
		// with button order in manual mode.
		IsBitSet(_myturnbits, 2) ? "K" : ".",
		IsBitSet(_myturnbits, 3) ? "R" : ".",
		IsBitSet(_myturnbits, 4) ? "A" : ".");
	return fckra_seen;
}

bool CSymbolEngineAutoplayer::IsFirstHeartbeatOfMyTurn()
{
	return(ismyturn()
		&& (_last_myturnbits == 0));
}
