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

// DialogSAPrefs10.cpp : implementation file
//

#include "stdafx.h"
#include "DialogSAPrefs11.h"

#include <limits.h>
#include "SAPrefsSubDlg.h"
#include "CPreferences.h"
#include "OH_MessageBox.h"

#define MAX_MAX_LOG 1000000

// DialogSAPrefs11 dialog


IMPLEMENT_DYNAMIC(CDlgSAPrefs11, CDialog)

CDlgSAPrefs11::CDlgSAPrefs11(CWnd* pParent /*=NULL*/)
		: CSAPrefsSubDlg(CDlgSAPrefs11::IDD, pParent) {
}

CDlgSAPrefs11::~CDlgSAPrefs11() {
}

void CDlgSAPrefs11::DoDataExchange(CDataExchange* pDX) {
	CSAPrefsSubDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOG_PRWIN_FUNCTIONS, m_LogPrwinFunctions);
	DDX_Control(pDX, IDC_LOG_HOPPER_FUNCTIONS, m_LogHopperFunctions);
	DDX_Control(pDX, IDC_LOG_INI_FUNCTIONS, m_LogIniFunctions);
	DDX_Control(pDX, IDC_LOG_ICM_FUNCTIONS, m_LogICMFunctions);
  DDX_Control(pDX, IDC_LOG_DELAY_FUNCTION, m_LogDelayFunction);

	DDX_Control(pDX, IDC_MAXIMUM_LOGSIZE, m_MaximumLogSize);
	DDX_Control(pDX, IDC_MAXIMUM_LOGSIZE_SPIN, m_MaximumLogSize_Spin);
}

BOOL CDlgSAPrefs11::OnInitDialog() {
	CSAPrefsSubDlg::OnInitDialog();
	CString		text = "";

	m_LogHopperFunctions.SetCheck(preferences.log_hopper_functions() ? BST_CHECKED : BST_UNCHECKED);
	m_LogICMFunctions.SetCheck(preferences.log_icm_functions() ? BST_CHECKED : BST_UNCHECKED);
	m_LogPrwinFunctions.SetCheck(preferences.log_prwin_functions() ? BST_CHECKED : BST_UNCHECKED);
  m_LogIniFunctions.SetCheck(preferences.log_ini_functions() ? BST_CHECKED : BST_UNCHECKED);
  m_LogDelayFunction.SetCheck(preferences.log_delay_function() ? BST_CHECKED : BST_UNCHECKED);

	text.Format("%d", preferences.log_max_logsize());
	text.Format("%d", preferences.log_max_logsize());
	m_MaximumLogSize.SetWindowText(text);
	m_MaximumLogSize_Spin.SetRange(0, 999);
	m_MaximumLogSize_Spin.SetPos(preferences.log_max_logsize());
	m_MaximumLogSize_Spin.SetBuddy(&m_MaximumLogSize);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// DialogSAPrefs11 message handlers
void CDlgSAPrefs11::OnOK() {
	CString			text = "";

	preferences.SetValue(k_prefs_log_hopper_functions, m_LogHopperFunctions.GetCheck()==BST_CHECKED ? true : false);
	preferences.SetValue(k_prefs_log_icm_functions, m_LogICMFunctions.GetCheck()==BST_CHECKED ? true : false);
  preferences.SetValue(k_prefs_log_prwin_functions, m_LogPrwinFunctions.GetCheck()==BST_CHECKED ? true : false);
	preferences.SetValue(k_prefs_log_ini_functions, m_LogIniFunctions.GetCheck()==BST_CHECKED ? true : false);
  preferences.SetValue(k_prefs_log_delay_function, m_LogDelayFunction.GetCheck() == BST_CHECKED ? true : false);

	m_MaximumLogSize.GetWindowText(text);
	preferences.SetValue(k_prefs_log_max_logsize, strtoul(text.GetString(), NULL, 10));
	
	CSAPrefsSubDlg::OnOK();
}

