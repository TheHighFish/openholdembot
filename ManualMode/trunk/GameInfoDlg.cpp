// DialogLockBlinds.cpp : implementation file
//

#include "stdafx.h"
#include "GameInfoDlg.h"
#include "debug.h"
#include "ManualModeDlg.h"

// CGameInfoDlg dialog
CGameInfoDlg::CGameInfoDlg(CWnd* pParent /*=NULL*/)	: CDialog(CGameInfoDlg::IDD, pParent) {
#ifdef SEH_ENABLE
	// Set exception handler
	SetUnhandledExceptionFilter(MyUnHandledExceptionFilter);
#endif
	
#ifdef SEH_ENABLE
	try {
#endif

	pPlacement = NULL;

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CGameInfoDlg::Constructor :\n"); 
		throw;
	}
#endif
}

CGameInfoDlg::~CGameInfoDlg() {
	if(pPlacement != NULL)
		free((WINDOWPLACEMENT *) pPlacement);
}

void CGameInfoDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SMALL_BLIND, m_SmallBlind);
	DDX_Control(pDX, IDC_BIG_BLIND, m_BigBlind);
	DDX_Control(pDX, IDC_BIG_BET, m_BigBet);
	DDX_Control(pDX, IDC_ANTE, m_Ante);
	DDX_Control(pDX, IDC_GAME_TYPE, m_GameType);
	DDX_Control(pDX, IDC_TOURNAMENT, m_Tournament);
}


BEGIN_MESSAGE_MAP(CGameInfoDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CGameInfoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CGameInfoDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CGameInfoDlg message handlers

BOOL CGameInfoDlg::OnInitDialog() {
#ifdef SEH_ENABLE
	try {
#endif
		CDialog::OnInitDialog();

		CString str;

		m_SmallBlind.SetWindowText(m_sblind);
		m_BigBlind.SetWindowText(m_bblind);
		m_BigBet.SetWindowText(m_bigbet);
		m_Ante.SetWindowText(m_ante);
		m_GameType.AddString("Fixed Limit");
		m_GameType.AddString("Pot Limit");
		m_GameType.AddString("No Limit");
		m_GameType.SetWindowPos(NULL, 81, 121, 138, 75, SWP_SHOWWINDOW);

		if (m_gametype == LIMIT_NL)
			m_GameType.SelectString(0, "No Limit");
		else if (m_gametype == LIMIT_PL)
			m_GameType.SelectString(0, "Pot Limit");
		else
			m_GameType.SelectString(0, "Fixed Limit");

		if (m_tournament)
			m_Tournament.SetCheck(BST_CHECKED);
		else
			m_Tournament.SetCheck(BST_UNCHECKED);

		m_SmallBlind.SetFocus();
		m_SmallBlind.SetSel(MAKEWORD(0, -1));

		if (pPlacement != NULL) SetWindowPlacement(pPlacement);

		return false;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CGameInfoDlg::OnInitDialog :\n"); 
		throw;
	}
#endif
}

void CGameInfoDlg::OnBnClickedOk() {
#ifdef SEH_ENABLE
	try {
#endif
		CString str;

		m_SmallBlind.GetWindowText(m_sblind);
		m_BigBlind.GetWindowText(m_bblind);
		m_BigBet.GetWindowText(m_bigbet);
		m_Ante.GetWindowText(m_ante);
		if (m_GameType.GetCurSel()==0) {
			m_gametype = LIMIT_FL;
		}
		else if (m_GameType.GetCurSel()==1) {
			m_gametype = LIMIT_PL;
		}
		else if (m_GameType.GetCurSel()==2) {
			m_gametype = LIMIT_NL;
		}

		m_tournament = m_Tournament.GetCheck() & BST_CHECKED;

		if (pPlacement == NULL) 
			pPlacement = (WINDOWPLACEMENT*) malloc(sizeof(WINDOWPLACEMENT));
		GetWindowPlacement(pPlacement);

		OnOK();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CGameInfoDlg::OnBnClickedOk :\n"); 
		throw;
	}
#endif
}

void CGameInfoDlg::OnBnClickedCancel()
{
		if (pPlacement == NULL) 
			pPlacement = (WINDOWPLACEMENT*) malloc(sizeof(WINDOWPLACEMENT));
		GetWindowPlacement(pPlacement);

		OnCancel();
}
