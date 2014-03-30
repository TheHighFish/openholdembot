// DialogLockBlinds.cpp : implementation file
//

#include "stdafx.h"
#include "OpenHoldem.h"
#include "DialogLockBlinds.h"
#include "debug.h"
#include "structs_defines.h"

// CDlgLockBlinds dialog
CDlgLockBlinds::CDlgLockBlinds(CWnd* pParent /*=NULL*/)	: CDialog(CDlgLockBlinds::IDD, pParent) {
#ifdef SEH_ENABLE
	// Set exception handler
	SetUnhandledExceptionFilter(MyUnHandledExceptionFilter);
#endif
	
#ifdef SEH_ENABLE
	try {
#endif

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgLockBlinds::Constructor :\n"); 
		throw;
	}
#endif
}


void CDlgLockBlinds::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOCK_BLINDS_SMALL_BLIND, m_SmallBlind);
	DDX_Control(pDX, IDC_LOCK_BLINDS_BIG_BLIND2, m_BigBlind);
	DDX_Control(pDX, IDC_LOCK_BLINDS_BIG_BET, m_BigBet);
	DDX_Control(pDX, IDC_LOCK_BLINDS_ANTE, m_Ante);
	DDX_Control(pDX, IDC_LOCK_BLINDS_GAME_TYPE, m_GameType);
}


BEGIN_MESSAGE_MAP(CDlgLockBlinds, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgLockBlinds::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgLockBlinds message handlers

BOOL CDlgLockBlinds::OnInitDialog() {
#ifdef SEH_ENABLE
	try {
#endif
		CDialog::OnInitDialog();

		CString str;

		str.Format("%.2f", sblind); m_SmallBlind.SetWindowText(str);
		str.Format("%.2f", bblind); m_BigBlind.SetWindowText(str);
		str.Format("%.2f", bbet); m_BigBet.SetWindowText(str);
		str.Format("%.2f", ante); m_Ante.SetWindowText(str);
		m_GameType.AddString("Fixed Limit");
		m_GameType.AddString("Pot Limit");
		m_GameType.AddString("No Limit");

		if (gametype == LIMIT_NL) {
			m_GameType.SelectString(0, "No Limit");
		}
		else if (gametype == LIMIT_PL) {
			m_GameType.SelectString(0, "Pot Limit");
		}
		else {
			m_GameType.SelectString(0, "Fixed Limit");
		}

		m_GameType.SetWindowPos(NULL, 0, 0, 138, 200, SWP_NOMOVE | SWP_NOZORDER);
 

		return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgLockBlinds::OnInitDialog :\n"); 
		throw;
	}
#endif
}

void CDlgLockBlinds::OnBnClickedOk() {
#ifdef SEH_ENABLE
	try {
#endif
		CString str;

		m_SmallBlind.GetWindowText(str);
		sblind = atof(str.GetString());

		m_BigBlind.GetWindowText(str);
		bblind = atof(str.GetString());

		m_BigBet.GetWindowText(str);
		bbet = atof(str.GetString());

		m_Ante.GetWindowText(str);
		ante = atof(str.GetString());

		if (m_GameType.GetCurSel()==0) {
			gametype = LIMIT_FL;
		}
		else if (m_GameType.GetCurSel()==1) {
			gametype = LIMIT_PL;
		}
		else if (m_GameType.GetCurSel()==2) {
			gametype = LIMIT_NL;
		}

		OnOK();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgLockBlinds::OnBnClickedOk :\n"); 
		throw;
	}
#endif
}
