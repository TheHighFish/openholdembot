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

// WM_APP = 0x8000 = 32768
// Messages 0..WM_APP reserved for Windows
// Messages (WM_APP + 1)..0xBFFF user-defined
// http://msdn.microsoft.com/en-us/library/windows/desktop/ms644930(v=vs.85).aspx
const int WMA_SETWINDOWTEXT	= WM_APP + 1;
const int WMA_DOCONNECT		= WM_APP + 2;
const int WMA_DODISCONNECT	= WM_APP + 3;
const int WMA_CONNECTEDHWND	= WM_APP + 4;
const int WMA_SETFLAG       = WM_APP + 5;
const int WMA_RESETFLAG     = WM_APP + 6;
const int WMA_ISREADY       = WM_APP + 7;


extern class COpenHoldemHopperCommunication: public CWnd
{
public:
	COpenHoldemHopperCommunication();
	~COpenHoldemHopperCommunication();
public:
	LRESULT OnSetWindowText(WPARAM, LPARAM title);
	LRESULT OnConnectMessage(WPARAM, LPARAM hwnd);
	LRESULT OnDisconnectMessage(WPARAM, LPARAM);
	LRESULT OnConnectedHwndMessage(WPARAM, LPARAM);
	LRESULT OnSetFlagMessage(WPARAM, LPARAM flag_to_set);
	LRESULT OnResetFlagMessage(WPARAM, LPARAM flag_to_reset);
	LRESULT OnIsReadyMessage(WPARAM, LPARAM);
protected:
	DECLARE_MESSAGE_MAP()
} *p_openholdem_hopper_communication;