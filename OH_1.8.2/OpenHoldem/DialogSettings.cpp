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

    __SEH_SET_EXCEPTION_HANDLER(MyUnHandledExceptionFilter);


    __SEH_HEADER

    __SEH_LOGFATAL("CDlgSettings::Constructor : \n");

}

CDlgSettings::~CDlgSettings() {
    __SEH_HEADER

    __SEH_LOGFATAL("CDlgSettings::Destructor : \n");

}

void CDlgSettings::DoDataExchange(CDataExchange* pDX) {
    __SEH_HEADER
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_BANKROLL, m_Bankroll);
    DDX_Control(pDX, IDC_EDIT_NIT, m_Nit);
    DDX_Control(pDX, IDC_EDIT_DEFCON, m_Defcon);
    DDX_Control(pDX, IDC_EDIT_RAKE, m_Rake);

    __SEH_LOGFATAL("CDlgSettings::DoDataExchange : \n");

}


BEGIN_MESSAGE_MAP(CDlgSettings, CDialog)
    ON_BN_CLICKED(IDOK, &CDlgSettings::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CDlgSettings::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgSettings message handlers

void CDlgSettings::OnBnClickedOk() {
    __SEH_HEADER
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

    __SEH_LOGFATAL("CDlgSettings::OnBnClickedOk : \n");

}

void CDlgSettings::OnBnClickedCancel() {
    __SEH_HEADER
    OnCancel();

    __SEH_LOGFATAL("CDlgSettings::OnBnClickedCancel : \n");

}

BOOL CDlgSettings::OnInitDialog() {
    __SEH_HEADER
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

    __SEH_LOGFATAL("CDlgSettings::OnInitDialog : \n");

}
