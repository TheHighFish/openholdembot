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
#include "CFormula.h"
#include "CTableMaploader.h"
#include "MainFrm.h"
#include "OpenHoldem.h"


COpenHoldemHopperCommunication *p_openholdem_hopper_communication = NULL;


//IMPLEMENT_DYNAMIC(COpenHoldemHopperCommunication, CWnd)

BEGIN_MESSAGE_MAP(COpenHoldemHopperCommunication, CWnd)
	ON_WM_CREATE()
	ON_MESSAGE(WMA_SETWINDOWTEXT, &COpenHoldemHopperCommunication::OnSetWindowText)
	ON_MESSAGE(WMA_DOCONNECT,     &COpenHoldemHopperCommunication::OnConnectMessage)
	ON_MESSAGE(WMA_DODISCONNECT,  &COpenHoldemHopperCommunication::OnDisconnectMessage)
	ON_MESSAGE(WMA_CONNECTEDHWND, &COpenHoldemHopperCommunication::OnConnectedHwndMessage)
	ON_MESSAGE(WMA_SETFLAG,       &COpenHoldemHopperCommunication::OnSetFlagMessage)
	ON_MESSAGE(WMA_RESETFLAG,     &COpenHoldemHopperCommunication::OnResetFlagMessage)
	ON_MESSAGE(WMA_ISREADY,       &COpenHoldemHopperCommunication::OnIsReadyMessage)  
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

LRESULT COpenHoldemHopperCommunication::OnIsReadyMessage(WPARAM, LPARAM)
{
	// 0 = Not ready, because of either
	//   * no formula
	//   * no tablemap
	if (p_formula->formula_name() == "" 
		|| p_formula->IsParsing()
		|| p_tablemap_loader->NumberOfTableMapsLoaded() < 1)
	{
		return (LRESULT)0;
	}
	// Otherwise:
	// 1 = ready and not connected
	// 2 = ready, but already connected
	else if (p_autoconnector->attached_hwnd() == NULL)
	{
		return (LRESULT)1;
	}
	else
	{
		return (LRESULT)2;
	}
}
