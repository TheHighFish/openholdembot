// SettingsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "OpenHoldem.h"
#include "DialogSettings.h"
#include "debug.h"
#include "global.h"

// CDlgSettings dialog

IMPLEMENT_DYNAMIC(CDlgSettings, CDialog)

CDlgSettings::CDlgSettings(CWnd* pParent /*=NULL*/)	: CDialog(CDlgSettings::IDD, pParent) {
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
		logfatal("CDlgSettings::Constructor : \n"); 
		throw;
	}
#endif
}

CDlgSettings::~CDlgSettings() {
#ifdef SEH_ENABLE
	try {
#endif
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgSettings::Destructor : \n"); 
		throw;
	}
#endif
}

void CDlgSettings::DoDataExchange(CDataExchange* pDX) {
#ifdef SEH_ENABLE
	try {
#endif
		CDialog::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_EDIT_BANKROLL, m_Bankroll);
		DDX_Control(pDX, IDC_EDIT_NIT, m_Nit);
		DDX_Control(pDX, IDC_EDIT_DEFCON, m_Defcon);
		DDX_Control(pDX, IDC_EDIT_RAKE, m_Rake);
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgSettings::DoDataExchange : \n"); 
		throw;
	}
#endif
}


BEGIN_MESSAGE_MAP(CDlgSettings, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgSettings::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgSettings::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgSettings message handlers

void CDlgSettings::OnBnClickedOk() {
#ifdef SEH_ENABLE
	try {
#endif
		CString s;

		m_Bankroll.GetWindowText(s);
		bankroll = atof(s);
		m_Nit.GetWindowText(s);
		nit = atof(s);
		m_Defcon.GetWindowText(s);
		defcon = atof(s);
		m_Rake.GetWindowText(s);
		rake = atof(s);

		OnOK();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgSettings::OnBnClickedOk : \n"); 
		throw;
	}
#endif
}

void CDlgSettings::OnBnClickedCancel() {
#ifdef SEH_ENABLE
	try {
#endif
		OnCancel();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgSettings::OnBnClickedCancel : \n"); 
		throw;
	}
#endif
}

BOOL CDlgSettings::OnInitDialog() {
#ifdef SEH_ENABLE
	try {
#endif
		CDialog::OnInitDialog();

		CString s;
	
		s.Format("%f", bankroll);
		m_Bankroll.SetWindowText(s);
		s.Format("%d", (int) nit);
		m_Nit.SetWindowText(s);
		s.Format("%f", defcon);
		m_Defcon.SetWindowText(s);
		s.Format("%f", rake);
		m_Rake.SetWindowText(s);
		rake = atof(s);

		return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgSettings::OnInitDialog : \n"); 
		throw;
	}
#endif
}
