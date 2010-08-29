// OpenScrape.h : main header file for the OpenScrape application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "DialogTableMap.h"

// COpenScrapeApp:
// See OpenScrape.cpp for the implementation of this class
//

extern	char	_startup_path[MAX_PATH];


//!  The Main class of openscrape that creates the other dialogues 
class COpenScrapeApp : public CWinApp
{
public:
	COpenScrapeApp();
	virtual BOOL InitInstance();
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
	int				sessionnum;
	CDlgTableMap	*m_TableMapDlg;

	virtual int ExitInstance();
};

extern COpenScrapeApp theApp;
