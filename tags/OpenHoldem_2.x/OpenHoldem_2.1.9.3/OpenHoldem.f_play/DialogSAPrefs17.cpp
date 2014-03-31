// DialogSAPrefs1.cpp : implementation file
//

#include "stdafx.h"

#include "SAPrefsSubDlg.h"
#include "DialogSAPrefs17.h"
#include "CPreferences.h"

// CDlgSAPrefs17 dialog

IMPLEMENT_DYNAMIC(CDlgSAPrefs17, CSAPrefsSubDlg)

CDlgSAPrefs17::CDlgSAPrefs17(CWnd* pParent /*=NULL*/)
		: CSAPrefsSubDlg(CDlgSAPrefs17::IDD, pParent)
{
}

CDlgSAPrefs17::~CDlgSAPrefs17()
{
}

void CDlgSAPrefs17::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONFIGURATION_DISABLE_LESS_CRITICAL_CHECKS, m_ConfigurationDisableLessCriticalChecks_Button);
}

BEGIN_MESSAGE_MAP(CDlgSAPrefs17, CSAPrefsSubDlg)
END_MESSAGE_MAP()

// CDlgSAPrefs1 message handlers
BOOL CDlgSAPrefs17::OnInitDialog()
{
	CSAPrefsSubDlg::OnInitDialog();
	m_ConfigurationDisableLessCriticalChecks_Button.SetCheck(prefs.configurationcheck_disable_less_critical_checks() != 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs17::OnOK()
{
	prefs.set_configurationcheck_disable_less_critical_checks(m_ConfigurationDisableLessCriticalChecks_Button.GetCheck());
	CSAPrefsSubDlg::OnOK();
}
