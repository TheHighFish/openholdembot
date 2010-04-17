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
	DDX_Control(pDX, IDC_CONFIGURATION_KEYBOARD_SETTINGS, m_ConfigurationKeyboardSettings_Button);
	DDX_Control(pDX, IDC_CONFIGURATION_THEME_SETTINGS, m_ConfigurationThemeSettings_Button);
	DDX_Control(pDX, IDC_CONFIGURATION_FONT_SETTINGS, m_ConfigurationFontSettings_Button);
}

BEGIN_MESSAGE_MAP(CDlgSAPrefs17, CSAPrefsSubDlg)
  ON_BN_CLICKED(IDC_CHECK1, &CDlgSAPrefs17::OnBnClickedCheck1)
END_MESSAGE_MAP()

// CDlgSAPrefs1 message handlers
BOOL CDlgSAPrefs17::OnInitDialog()
{
	CSAPrefsSubDlg::OnInitDialog();
	m_ConfigurationKeyboardSettings_Button.SetCheck(prefs.configurationcheck_keyboard_settings() != 0);
	m_ConfigurationThemeSettings_Button.SetCheck(prefs.configurationcheck_theme_settings() != 0);
	m_ConfigurationFontSettings_Button.SetCheck(prefs.configurationcheck_font_settings() != 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs17::OnOK()
{
	prefs.set_configurationcheck_keyboard_settings(m_ConfigurationKeyboardSettings_Button.GetCheck());
	prefs.set_configurationcheck_theme_settings(m_ConfigurationThemeSettings_Button.GetCheck());
	prefs.set_configurationcheck_font_settings(m_ConfigurationFontSettings_Button.GetCheck());
	CSAPrefsSubDlg::OnOK();
}

void CDlgSAPrefs17::OnBnClickedCheck1()
{
  // TODO: Add your control notification handler code here
}
