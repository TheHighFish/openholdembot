//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#include "stdafx.h"
#include "COpenHoldemHopperCommunication.h"

#include "CAutoConnector.h"
#include "CFlagsToolbar.h"
#include "CFormulaParser.h"
#include "CFunctionCollection.h"
#include "COpenHoldemTitle.h"
#include "CPreferences.h"
#include "CTableMaploader.h"
#include "MainFrm.h"
#include "OpenHoldem.h"


COpenHoldemHopperCommunication *p_openholdem_hopper_communication = NULL;


//IMPLEMENT_DYNAMIC(COpenHoldemHopperCommunication, CWnd)

BEGIN_MESSAGE_MAP(COpenHoldemHopperCommunication, CWnd)  
END_MESSAGE_MAP()


LRESULT COpenHoldemHopperCommunication::OnSetWindowText(WPARAM, LPARAM title)
{
	write_log(preferences.debug_hopper_messages(), "[COpenHoldemHopperCommunication] Received 0x8001: OnSetWindowText\n"); 
	if (p_openholdem_title == NULL)	return false;
	if (title) 
	{
		CString *sTitle = (CString *)title;
		p_openholdem_title->SetUserDefinedOpenHoldemTitle(sTitle->GetString());
		delete sTitle;
	} 
	else 
	{
		p_openholdem_title->SetUserDefinedOpenHoldemTitle("");
	}
	return true;
}


LRESULT COpenHoldemHopperCommunication::OnConnectMessage(WPARAM, LPARAM hwnd)
{
	write_log(preferences.debug_hopper_messages(), "[COpenHoldemHopperCommunication] Received 0x8002: OnConnectMessage\n");
	return p_autoconnector->Connect((HWND)hwnd);
}


LRESULT COpenHoldemHopperCommunication::OnDisconnectMessage(WPARAM, LPARAM)
{
	write_log(preferences.debug_hopper_messages(), "[COpenHoldemHopperCommunication] Received 0x8003: OnDisconnectMessage\n");
	p_autoconnector->Disconnect("disconnected by hopper");
	return true;
}


LRESULT COpenHoldemHopperCommunication::OnConnectedHwndMessage(WPARAM, LPARAM)
{
	write_log(preferences.debug_hopper_messages(), "[COpenHoldemHopperCommunication] Received 0x8004: OnConnectedHwndMessage\n");
	return (LRESULT) p_autoconnector->attached_hwnd();
}


LRESULT COpenHoldemHopperCommunication::OnSetFlagMessage(WPARAM, LPARAM flag_to_set)
{
	write_log(preferences.debug_hopper_messages(), "[COpenHoldemHopperCommunication] Received 0x8005: OnSetFlagMessage\n");
	int _flag_to_set = int(flag_to_set);
	if ((_flag_to_set < 0) || (_flag_to_set >= kNumberOfFlags))
	{
		return false;
	}
	p_flags_toolbar->SetFlag(_flag_to_set, true);
	return true;
}


LRESULT COpenHoldemHopperCommunication::OnResetFlagMessage(WPARAM, LPARAM flag_to_reset)
{
	write_log(preferences.debug_hopper_messages(), "[COpenHoldemHopperCommunication] Received 0x8006: OnResetFlagMessage\n");
	int _flag_to_reset = int(flag_to_reset);
	if ((_flag_to_reset < 0) || (_flag_to_reset >= kNumberOfFlags))
	{
		return false;
	}
	p_flags_toolbar->SetFlag(_flag_to_reset, false);
	return true;
}

LRESULT COpenHoldemHopperCommunication::OnIsReadyMessage(WPARAM, LPARAM)
{
	write_log(preferences.debug_hopper_messages(), "[COpenHoldemHopperCommunication] Received 0x8007: OnIsReadyMessage\n");
	// 0 = Not ready, because of either
	//   * no formula
	//   * no tablemap
	if (p_function_collection->FormulaName() == "" 
		|| p_formula_parser->IsParsing()
		|| p_tablemap_loader->NumberOfTableMapsLoaded() < 1)
	{
		write_log(preferences.debug_hopper_messages(), 
			"[COpenHoldemHopperCommunication] OnIsReadyMessage() not ready\n");
		return 0;
	}
	// Otherwise:
	// 1 = ready and not connected
	// 2 = ready, but already connected
	else if (p_autoconnector->attached_hwnd() == NULL)
	{
		write_log(preferences.debug_hopper_messages(), 
			"[COpenHoldemHopperCommunication] OnIsReadyMessage() ready\n");
		return 1;
	}
	else
	{
		write_log(preferences.debug_hopper_messages(), 
			"[COpenHoldemHopperCommunication] OnIsReadyMessage() already connected\n");
		return 2;
	}
}


