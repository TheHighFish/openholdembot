//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

// CDlgSAPrefs15.cpp : implementation file
//

#include "stdafx.h"

#include "SAPrefsSubDlg.h"
#include "DialogSAPrefs15.h"
#include "CPreferences.h"
#include "MainFrm.h"
#include "OH_MessageBox.h"

// CDlgSAPrefs15 dialog

IMPLEMENT_DYNAMIC(CDlgSAPrefs15, CDialog)

CDlgSAPrefs15::CDlgSAPrefs15(CWnd* pParent /*=NULL*/)
	: CSAPrefsSubDlg(CDlgSAPrefs15::IDD, pParent)
{

}

CDlgSAPrefs15::~CDlgSAPrefs15()
{
}

void CDlgSAPrefs15::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GUI_START_MINIMIZED, _gui_start_minimized_Button);
	DDX_Control(pDX, IDC_DISABLE_MSGBOX, m_disable_msgbox);
}


BEGIN_MESSAGE_MAP(CDlgSAPrefs15, CDialog)
	ON_BN_CLICKED(IDC_DISABLE_MSGBOX, &CDlgSAPrefs15::OnBnClickedDisableMsgbox)
END_MESSAGE_MAP()


// CDlgSAPrefs15 message handlers
BOOL CDlgSAPrefs15::OnInitDialog()
{
	CSAPrefsSubDlg::OnInitDialog();

	_gui_start_minimized_Button.SetCheck(theApp.p_preferences()->gui_start_minimized());
	m_disable_msgbox.SetCheck(theApp.p_preferences()->disable_msgbox() ? BST_CHECKED : BST_UNCHECKED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs15::OnOK()
{
	theApp.p_preferences()->SetValue(k_prefs_gui_start_minimized, _gui_start_minimized_Button.GetCheck() == true);
	theApp.p_preferences()->SetValue(k_prefs_disable_msgbox, m_disable_msgbox.GetCheck()==BST_CHECKED ? true : false);

	CSAPrefsSubDlg::OnOK();
}

void CDlgSAPrefs15::OnBnClickedDisableMsgbox()
{
	if (m_disable_msgbox.GetCheck()==BST_CHECKED)
		OH_MessageBox_Interactive("Warning: Selecting this option instructs OpenHoldem to refrain from\n"
				   "displaying ANY runtime informational or error message boxes.  Examples\n"
				   "include parse errors, DLL load errors, etc.  It is strongly advised that\n"
				   "this option only be used in a production environment that has been\n"
				   "completely and thoroughly tested for proper behavior.\n\n"
				   "Note that interactive messages are not disabled, such as when using the\n"
				   "formula editor.", "WARNING", MB_OK);
}