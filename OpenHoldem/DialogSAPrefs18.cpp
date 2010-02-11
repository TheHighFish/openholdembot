// DialogSAPrefs1.cpp : implementation file
//

#include "stdafx.h"
#include "DialogSAPrefs18.h"

#include "SAPrefsSubDlg.h"
#include "CPreferences.h"

// CDlgSAPrefs18 dialog

IMPLEMENT_DYNAMIC(CDlgSAPrefs18, CSAPrefsSubDlg)

CDlgSAPrefs18::CDlgSAPrefs18(CWnd* pParent /*=NULL*/)
		: CSAPrefsSubDlg(CDlgSAPrefs18::IDD, pParent)
{
}

CDlgSAPrefs18::~CDlgSAPrefs18()
{
}

void CDlgSAPrefs18::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	/*
	DDX_Control(pDX, IDC_CONFIGURATION_KEYBOARD_SETTINGS, m_ConfigurationKeyboardSettings_Button);
	DDX_Control(pDX, IDC_CONFIGURATION_PERL_DEPENDENCIES, m_ConfigurationPerlDependencies_Button);
	*/
}

BEGIN_MESSAGE_MAP(CDlgSAPrefs18, CSAPrefsSubDlg)
END_MESSAGE_MAP()

// CDlgSAPrefs1 message handlers
BOOL CDlgSAPrefs18::OnInitDialog()
{
	CSAPrefsSubDlg::OnInitDialog();
	/*
	m_ConfigurationKeyboardSettings_Button.SetCheck(prefs.configurationcheck_keyboard_settings() != 0);
	m_ConfigurationPerlDependencies_Button.SetCheck(prefs.configurationcheck_perl_dependencies() != 0);
	*/
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs18::OnOK()
{
	/*
	prefs.set_configurationcheck_keyboard_settings(m_ConfigurationKeyboardSettings_Button.GetCheck());
	prefs.set_configurationcheck_perl_dependencies(m_ConfigurationPerlDependencies_Button.GetCheck());
	*/
	CSAPrefsSubDlg::OnOK();
}
