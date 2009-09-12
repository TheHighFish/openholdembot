// DlgSAPrefs16.cpp : implementation file
//

#include "stdafx.h"
#include "CPreferences.h"
#include "DialogSAPrefs16.h"
#include "SAPrefsSubDlg.h"

// CDlgSAPrefs16 dialog

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
	DDX_Control(pDX, IDC_REBUY_NO_CARDS, _rebuy_condition_no_cards);
	DDX_Control(pDX, IDC_REBUY_CHANGE_IN_HANDNUMBER, _rebuy_condition_change_in_handnumber);
	DDX_Control(pDX, IDC_REBUY_HEURISTIC_OCCLUSION_CHECK, _rebuy_condition_heuristic_check_for_occlusion);
	DDX_Control(pDX, IDC_REBUY_EDIT_MINIMUM_DELAY, _rebuy_minimum_time_to_next_try);
}


BEGIN_MESSAGE_MAP(CDlgSAPrefs16, CDialog)
END_MESSAGE_MAP()


// CDlgSAPrefs16 message handlers
BOOL CDlgSAPrefs16::OnInitDialog()
{
	CSAPrefsSubDlg::OnInitDialog();

	_rebuy_condition_no_cards.SetCheck(prefs.rebuy_condition_no_cards());
	_rebuy_condition_change_in_handnumber.SetCheck(prefs.rebuy_condition_change_in_handnumber());
	_rebuy_condition_heuristic_check_for_occlusion.SetCheck(prefs.rebuy_condition_heuristic_check_for_occlusion());
	//!!!_rebuy_minimum_time_to_next_try.set
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs16::OnOK()
{
	prefs.set_rebuy_condition_no_cards(_rebuy_condition_no_cards.GetCheck() == true);
	prefs.set_rebuy_condition_change_in_handnumber(_rebuy_condition_change_in_handnumber.GetCheck() == true);
	prefs.set_rebuy_condition_heuristic_check_for_occlusion(_rebuy_condition_heuristic_check_for_occlusion.GetCheck() == true);
	//!!!prefs.set_rebuy_minimum_time_to_next_try();

	CSAPrefsSubDlg::OnOK();
}
