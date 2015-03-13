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

// DlgSAPrefs16.cpp : implementation file
//

#include "stdafx.h"
#include "CPreferences.h"
#include "DialogSAPrefs16.h"
#include "OH_MessageBox.h"
#include "SAPrefsSubDlg.h"

// CDlgSAPrefs16 dialog

#define MAX_MINIMUM_DELAY_TO_NEXT_REBUY 3600 // seconds


IMPLEMENT_DYNAMIC(CDlgSAPrefs16, CDialog)

CDlgSAPrefs16::CDlgSAPrefs16(CWnd* pParent /*=NULL*/)
	: CSAPrefsSubDlg(CDlgSAPrefs16::IDD, pParent)
{

}

CDlgSAPrefs16::~CDlgSAPrefs16()
{
}

void CDlgSAPrefs16::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REBUY_NO_CARDS, _rebuy_condition_no_cards_Button);
	DDX_Control(pDX, IDC_REBUY_CHANGE_IN_HANDNUMBER, _rebuy_condition_change_in_handnumber_Button);
	DDX_Control(pDX, IDC_REBUY_HEURISTIC_OCCLUSION_CHECK, _rebuy_condition_heuristic_check_for_occlusion_Button);
	DDX_Control(pDX, IDC_REBUY_EDIT_MINIMUM_DELAY, _rebuy_minimum_time_to_next_try_Edit);
	DDX_Control(pDX, IDC_REBUY_EDIT_MINIMUM_DELAY_SPIN, _rebuy_minimum_time_to_next_try_Spin);
	DDX_Control(pDX, IDC_REBUY_EDIT_SCRIPT, _rebuy_script_Edit);
}


BEGIN_MESSAGE_MAP(CDlgSAPrefs16, CDialog)
END_MESSAGE_MAP()


// CDlgSAPrefs16 message handlers
BOOL CDlgSAPrefs16::OnInitDialog()
{
	CString		text = "";

	CSAPrefsSubDlg::OnInitDialog();

	_rebuy_condition_no_cards_Button.SetCheck(MAIN->p_preferences()->rebuy_condition_no_cards());
	_rebuy_condition_change_in_handnumber_Button.SetCheck(MAIN->p_preferences()->rebuy_condition_change_in_handnumber());
	_rebuy_condition_heuristic_check_for_occlusion_Button.SetCheck(MAIN->p_preferences()->rebuy_condition_heuristic_check_for_occlusion());

	text.Format("%d", MAIN->p_preferences()->rebuy_minimum_time_to_next_try());
	_rebuy_minimum_time_to_next_try_Edit.SetWindowText(text);
	_rebuy_minimum_time_to_next_try_Spin.SetRange(0, (short)MAX_MINIMUM_DELAY_TO_NEXT_REBUY);
	_rebuy_minimum_time_to_next_try_Spin.SetPos(MAIN->p_preferences()->rebuy_minimum_time_to_next_try());
	_rebuy_minimum_time_to_next_try_Spin.SetBuddy(&_rebuy_minimum_time_to_next_try_Edit);

	_rebuy_script_Edit.SetWindowText(MAIN->p_preferences()->rebuy_script());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs16::OnOK()
{
	CString text = "";
	
	MAIN->p_preferences()->SetValue(k_prefs_rebuy_condition_no_cards, _rebuy_condition_no_cards_Button.GetCheck() == true);
	MAIN->p_preferences()->SetValue(k_prefs_rebuy_condition_change_in_handnumber, _rebuy_condition_change_in_handnumber_Button.GetCheck() == true);
	MAIN->p_preferences()->SetValue(k_prefs_rebuy_condition_heuristic_check_for_occlusion, _rebuy_condition_heuristic_check_for_occlusion_Button.GetCheck() == true);

	_rebuy_minimum_time_to_next_try_Edit.GetWindowText(text);
	if (strtoul(text.GetString(), 0, 10)<0 || strtoul(text.GetString(), 0, 10)>MAX_MINIMUM_DELAY_TO_NEXT_REBUY) 
	{
		OH_MessageBox_Interactive("Invalid minimum time to next rebuy", "ERROR", MB_OK);
		return;
	}
	MAIN->p_preferences()->SetValue(k_prefs_rebuy_minimum_time_to_next_try, strtoul(text.GetString(), 0, 10));

	_rebuy_script_Edit.GetWindowText(text);
	MAIN->p_preferences()->SetValue(k_prefs_rebuy_script, text);
	
	CSAPrefsSubDlg::OnOK();
}
