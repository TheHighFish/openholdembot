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
#include "MainFrm.h"
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
	DDX_Control(pDX, IDC_LOG_BASIC_INFO, m_EnableBasicInfo);
	DDX_Control(pDX, IDC_ENABLE_TRACE, m_EnableTrace);
	DDX_Control(pDX, IDC_ENABLE_ERROR_LOGGiNG, m_EnableErrorLogging);
	DDX_Control(pDX, IDC_ENABLE_DLL_LOGGiNG, m_EnableDLLLogging);

	DDX_Control(pDX, IDC_MAXIMUM_LOGSIZE, m_MaximumLogSize);
	DDX_Control(pDX, IDC_MAXIMUM_LOGSIZE_SPIN, m_MaximumLogSize_Spin);
}

BOOL CDlgSAPrefs11::OnInitDialog() {
	CSAPrefsSubDlg::OnInitDialog();
	CString		text = "";

	m_EnableTrace.SetCheck(MAIN->p_preferences()->trace_enabled() ? BST_CHECKED : BST_UNCHECKED);
	m_EnableDLLLogging.SetCheck(MAIN->p_preferences()->dll_logging_enabled() ? BST_CHECKED : BST_UNCHECKED);

  // Always mark basic logging and error logging as enabled
	// m_EnableBasicInfo.SetCheck(MAIN->p_preferences()->basic_logging_enabled() ? BST_CHECKED : BST_UNCHECKED);
  // m_EnableErrorLogging.SetCheck(MAIN->p_preferences()->error_logging_enabled() ? BST_CHECKED : BST_UNCHECKED);
  m_EnableBasicInfo.SetCheck(BST_CHECKED);
  m_EnableErrorLogging.SetCheck(BST_CHECKED);

	text.Format("%d", MAIN->p_preferences()->log_max_logsize());
	text.Format("%d", MAIN->p_preferences()->log_max_logsize());
	m_MaximumLogSize.SetWindowText(text);
	m_MaximumLogSize_Spin.SetRange(0, 999);
	m_MaximumLogSize_Spin.SetPos(MAIN->p_preferences()->log_max_logsize());
	m_MaximumLogSize_Spin.SetBuddy(&m_MaximumLogSize);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// DialogSAPrefs11 message handlers
void CDlgSAPrefs11::OnOK() {
	CString			text = "";

	MAIN->p_preferences()->SetValue(k_prefs_trace_enabled, m_EnableTrace.GetCheck()==BST_CHECKED ? true : false);
	MAIN->p_preferences()->SetValue(k_prefs_dll_logging_enabled, m_EnableDLLLogging.GetCheck()==BST_CHECKED ? true : false);

  // Always mark basic logging and error logging as enabled
  // MAIN->p_preferences()->SetValue(k_prefs_basic_logging_enabled, m_EnableBasicInfo.GetCheck()==BST_CHECKED ? true : false);
	// MAIN->p_preferences()->SetValue(k_prefs_error_logging_enabled, m_EnableErrorLogging.GetCheck()==BST_CHECKED ? true : false);
  MAIN->p_preferences()->SetValue(k_prefs_basic_logging_enabled, true);
	MAIN->p_preferences()->SetValue(k_prefs_error_logging_enabled, true);

	m_MaximumLogSize.GetWindowText(text);
	MAIN->p_preferences()->SetValue(k_prefs_log_max_logsize, strtoul(text.GetString(), NULL, 10));
	
	CSAPrefsSubDlg::OnOK();
}

