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

// DialogSAPrefs12.cpp : implementation file
//

#include "stdafx.h"

#include "SAPrefsSubDlg.h"
#include "DialogSAPrefs12.h"
#include "CPreferences.h"


// CDlgSAPrefs12 dialog

IMPLEMENT_DYNAMIC(CDlgSAPrefs12, CDialog)

CDlgSAPrefs12::CDlgSAPrefs12(CWnd* pParent /*=NULL*/)
	: CSAPrefsSubDlg(CDlgSAPrefs12::IDD, pParent)
{

}

CDlgSAPrefs12::~CDlgSAPrefs12()
{
}

void CDlgSAPrefs12::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	// No need for any DDX.
	// We do it the good old Win95-way. :)
}


BEGIN_MESSAGE_MAP(CDlgSAPrefs12, CDialog)
END_MESSAGE_MAP()


// CDlgSAPrefs12 message handlers
BOOL CDlgSAPrefs12::OnInitDialog()
{	
	if (preferences.validator_enabled() == 0)
	{
		CheckRadioButton(IDC_RADIO_VALIDATOR_1, IDC_RADIO_VALIDATOR_3, IDC_RADIO_VALIDATOR_1);
	}
	else if (preferences.validator_enabled() == 1)
	{
		CheckRadioButton(IDC_RADIO_VALIDATOR_1, IDC_RADIO_VALIDATOR_3, IDC_RADIO_VALIDATOR_2);
	}
	else
	{
		CheckRadioButton(IDC_RADIO_VALIDATOR_1, IDC_RADIO_VALIDATOR_3, IDC_RADIO_VALIDATOR_3);
	}
	CheckDlgButton(IDC_CHECK_VALIDATOR_USE_HEURISTIC_RULES, preferences.validator_use_heuristic_rules() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_CHECK_VALIDATOR_STOP_ON_ERROR, preferences.validator_stop_on_error() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_CHECK_VALIDATOR_SHOOT_REPLAYFRAME_ON_ERROR, preferences.validator_shoot_replayframe_on_error() ? MF_CHECKED : MF_UNCHECKED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs12::OnOK()
{
	if (IsDlgButtonChecked(IDC_RADIO_VALIDATOR_1))
	{
		preferences.SetValue(k_prefs_validator_enabled, 0);
	}
	else if (IsDlgButtonChecked(IDC_RADIO_VALIDATOR_2))
	{
		preferences.SetValue(k_prefs_validator_enabled, 1);
	}
	else
	{
		preferences.SetValue(k_prefs_validator_enabled, 2);
	}
	preferences.SetValue(k_prefs_validator_use_heuristic_rules, IsDlgButtonChecked(IDC_CHECK_VALIDATOR_USE_HEURISTIC_RULES));
	preferences.SetValue(k_prefs_validator_stop_on_error, IsDlgButtonChecked(IDC_CHECK_VALIDATOR_STOP_ON_ERROR));
	preferences.SetValue(k_prefs_validator_shoot_replayframe_on_error, IsDlgButtonChecked(IDC_CHECK_VALIDATOR_SHOOT_REPLAYFRAME_ON_ERROR));

	CSAPrefsSubDlg::OnOK();
}


