//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*****************************************************************************
//
// Purpose:
//
//*****************************************************************************

#include "stdafx.h"
#include "COpenHoldemHopperCommunication.h"

#include "CAutoConnector.h"
#include "CFlagsToolbar.h"
#include "MainFrm.h"
#include "OpenHoldem.h"


COpenHoldemHopperCommunication *p_openholdem_hopper_communication = NULL;


//IMPLEMENT_DYNAMIC(COpenHoldemHopperCommunication, CWnd)

BEGIN_MESSAGE_MAP(COpenHoldemHopperCommunication, CWnd)

END_MESSAGE_MAP()


LRESULT COpenHoldemHopperCommunication::OnSetWindowText(WPARAM, LPARAM title)
{
	if (title) 
	{
		CString *sTitle = (CString *)title;
		PMainframe()->SetMainWindowTitle(sTitle->GetString());
		delete sTitle;
	} 
	else 
	{
		PMainframe()->SetMainWindowTitle("");
	}
	return true;
}


LRESULT COpenHoldemHopperCommunication::OnConnectMessage(WPARAM, LPARAM hwnd)
{
	return p_autoconnector->Connect((HWND)hwnd);
}


LRESULT COpenHoldemHopperCommunication::OnDisconnectMessage(WPARAM, LPARAM)
{
	p_autoconnector->Disconnect();
	return true;
}


LRESULT COpenHoldemHopperCommunication::OnConnectedHwndMessage(WPARAM, LPARAM)
{
	return (LRESULT) p_autoconnector->attached_hwnd();
}


LRESULT COpenHoldemHopperCommunication::OnSetFlagMessage(WPARAM, LPARAM flag_to_set)
{
	int _flag_to_set = int(flag_to_set);
	if ((_flag_to_set < 0) || (_flag_to_set >= k_number_of_flags))
	{
		return false;
	}
	p_flags_toolbar->SetFlag(_flag_to_set, true);
	return true;
}


LRESULT COpenHoldemHopperCommunication::OnResetFlagMessage(WPARAM, LPARAM flag_to_reset)
{
	int _flag_to_reset = int(flag_to_reset);
	if ((_flag_to_reset < 0) || (_flag_to_reset >= k_number_of_flags))
	{
		return false;
	}
	p_flags_toolbar->SetFlag(_flag_to_reset, true);
	return true;
}

