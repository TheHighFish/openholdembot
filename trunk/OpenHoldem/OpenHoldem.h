#ifndef INC_OPENHOLDEM_H
#define INC_OPENHOLDEM_H

// OpenHoldem.h : main header file for the OpenHoldem application
//

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"	   // main symbols

#include "..\..\Reference Mouse DLL\mousedll.h"
#include "..\..\Reference Keyboard DLL\keyboarddll.h"
#include "..\..\Reference Scraper DLL\scraperdll.h"

// COpenHoldemApp:
// See OpenHoldem.cpp for the implementation of this class
//

extern	char	_startup_path[MAX_PATH];

class COpenHoldemApp : public CWinApp 
{
public:
	COpenHoldemApp();
	~COpenHoldemApp();
	void MyLoadStdProfileSettings(UINT nMaxMRU);
	virtual BOOL InitInstance();
	afx_msg void OnAppAbout();
	afx_msg void OnForceCrash();
	DECLARE_MESSAGE_MAP()

public:
	virtual int ExitInstance();

	mouse_click_t				_dll_mouse_click;
	keyboard_sendstring_t		_dll_keyboard_sendstring;
	scraper_override_t			_dll_scraper_override;
	scraper_process_message_t	_dll_scraper_process_message;

	unsigned long int	_session_id;
	HMODULE				_mouse_dll;
	HMODULE				_keyboard_dll;
	HMODULE				_scraper_dll;
};

extern COpenHoldemApp theApp;

#define		WMA_SETWINDOWTEXT				WM_APP+1
#define		WMA_DOCONNECT					WM_APP+2

#endif //INC_OPENHOLDEM_H