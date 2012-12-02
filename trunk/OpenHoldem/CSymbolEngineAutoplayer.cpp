#include "stdafx.h"
#include "CSymbolEngineAutoplayer.h"

#include "CAutoconnector.h"
#include "CScraper.h"
#include "CStringMatch.h"
#include "MagicNumbers.h"

CSymbolEngineAutoplayer::CSymbolEngineAutoplayer()
{}

CSymbolEngineAutoplayer::~CSymbolEngineAutoplayer()
{}

void CSymbolEngineAutoplayer::InitOnStartup()
{}

void CSymbolEngineAutoplayer::ResetOnConnection()
{
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
	_myturnbits    = 0;
	_issittingin   = false;
	_isautopost    = false;
	_isfinalanswer = false;
	CalculateMyTurnBits();
	CalculateSitInState();
}

void CSymbolEngineAutoplayer::CalculateMyTurnBits()
{
	for (int i=0; i<k_max_number_of_buttontons; i++)
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
}

void CSymbolEngineAutoplayer::CalculateSitInState()
{
	for (int i=0; i<k_max_number_of_buttontons; i++)
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