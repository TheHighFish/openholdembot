// DialogSAPrefs2.cpp : implementation file
//

#include "stdafx.h"

#include "SAPrefsSubDlg.h"
#include "DialogSAPrefs2.h"
#include "CPreferences.h"


// CDlgSAPrefs2 dialog

IMPLEMENT_DYNAMIC(CDlgSAPrefs2, CSAPrefsSubDlg)

CDlgSAPrefs2::CDlgSAPrefs2(CWnd* pParent /*=NULL*/)
		: CSAPrefsSubDlg(CDlgSAPrefs2::IDD, pParent)
{
}

CDlgSAPrefs2::~CDlgSAPrefs2()
{
}

void CDlgSAPrefs2::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FRAMEDELAY, m_FrameDelay);
	DDX_Control(pDX, IDC_FRAMEDELAY_SPIN, m_FrameDelay_Spin);
	DDX_Control(pDX, IDC_SWAGDELAY1, m_SwagDelay1);
	DDX_Control(pDX, IDC_SWAGDELAY1_SPIN, m_SwagDelay1_Spin);
	DDX_Control(pDX, IDC_SWAGDELAY2, m_SwagDelay2);
	DDX_Control(pDX, IDC_SWAGDELAY2_SPIN, m_SwagDelay2_Spin);
	DDX_Control(pDX, IDC_SWAGDELAY3, m_SwagDelay3);
	DDX_Control(pDX, IDC_SWAGDELAY3_SPIN, m_SwagDelay3_Spin);
	DDX_Control(pDX, IDC_AUTO_AP, m_AutoAP);
	DDX_Control(pDX, IDC_FOCUSDETECT, m_FocusDetect);
	DDX_Control(pDX, IDC_SWAGUSECOMMA, m_SwagUseComma);
	DDX_Control(pDX, IDC_ONLY_CALC_ON_TURN, m_CalcOnMyTurn);
	DDX_Control(pDX, IDC_NEED_BALANCE_STABILITY, m_NeedBalanceStability);
}

BEGIN_MESSAGE_MAP(CDlgSAPrefs2, CSAPrefsSubDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SWAGDELAY1_SPIN, &CDlgSAPrefs2::OnDeltaposSwagdelay1Spin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SWAGDELAY2_SPIN, &CDlgSAPrefs2::OnDeltaposSwagdelay2Spin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SWAGDELAY3_SPIN, &CDlgSAPrefs2::OnDeltaposSwagdelay3Spin)
END_MESSAGE_MAP()

// CDlgSAPrefs2 message handlers
BOOL CDlgSAPrefs2::OnInitDialog()
{
	CSAPrefsSubDlg::OnInitDialog();
	CString		text = "";

	text.Format("%d", prefs.frame_delay());
	m_FrameDelay.SetWindowText(text);
	m_FrameDelay_Spin.SetRange(0, 9);
	m_FrameDelay_Spin.SetPos(prefs.frame_delay());
	m_FrameDelay_Spin.SetBuddy(&m_FrameDelay);

	text.Format("%d", prefs.swag_delay_1());
	m_SwagDelay1.SetWindowText(text);
	m_SwagDelay1_Spin.SetRange(0, 5000);
	m_SwagDelay1_Spin.SetPos(prefs.swag_delay_1());
	m_SwagDelay1_Spin.SetBuddy(&m_SwagDelay1);

	text.Format("%d", prefs.swag_delay_2());
	m_SwagDelay2.SetWindowText(text);
	m_SwagDelay2_Spin.SetRange(0, 5000);
	m_SwagDelay2_Spin.SetPos(prefs.swag_delay_2());
	m_SwagDelay2_Spin.SetBuddy(&m_SwagDelay2);

	text.Format("%d", prefs.swag_delay_3());
	m_SwagDelay3.SetWindowText(text);
	m_SwagDelay3_Spin.SetRange(0, 5000);
	m_SwagDelay3_Spin.SetPos(prefs.swag_delay_3());
	m_SwagDelay3_Spin.SetBuddy(&m_SwagDelay3);

	m_AutoAP.SetCheck(prefs.ap_auto() ? BST_CHECKED : BST_UNCHECKED);

	m_FocusDetect.SetCheck(prefs.focus_detect() ? BST_CHECKED : BST_UNCHECKED);

	m_SwagUseComma.SetCheck(prefs.swag_use_comma() ? BST_CHECKED : BST_UNCHECKED);

	m_CalcOnMyTurn.SetCheck(prefs.calc_only_my_turn() ? BST_CHECKED : BST_UNCHECKED);

	m_NeedBalanceStability.SetCheck(prefs.need_balance_stability() ? BST_CHECKED : BST_UNCHECKED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs2::OnOK()
{
	CString			text = "";

	m_FrameDelay.GetWindowText(text);
	if (strtoul(text.GetString(), 0, 10)>MAX_FRAMEDELAY)
	{
		MessageBox("Invalid Frame Delay", "ERROR", MB_OK);
		return;
	}
	prefs.set_frame_delay(strtoul(text.GetString(), 0, 10));

	m_SwagDelay1.GetWindowText(text);
	if (strtoul(text.GetString(), 0, 10)>MAX_SWAGDELAY1)
	{
		MessageBox("Invalid Swag Delay (Select to Delete)", "ERROR", MB_OK);
		return;
	}
	prefs.set_swag_delay_1(strtoul(text.GetString(), 0, 10));

	m_SwagDelay2.GetWindowText(text);
	if (strtoul(text.GetString(), 0, 10)>MAX_SWAGDELAY2)
	{
		MessageBox("Invalid Swag Delay (Delete to Entry)", "ERROR", MB_OK);
		return;
	}
	prefs.set_swag_delay_2(strtoul(text.GetString(), 0, 10));

	m_SwagDelay3.GetWindowText(text);
	if (strtoul(text.GetString(), 0, 10)>MAX_SWAGDELAY3) 
	{
		MessageBox("Invalid Swag Delay (Entry to Confirm)", "ERROR", MB_OK);
		return;
	}
	prefs.set_swag_delay_3(strtoul(text.GetString(), 0, 10));

	prefs.set_ap_auto(m_AutoAP.GetCheck()==BST_CHECKED ? true : false);
	prefs.set_focus_detect(m_FocusDetect.GetCheck()==BST_CHECKED ? true : false);
	prefs.set_swag_use_comma(m_SwagUseComma.GetCheck()==BST_CHECKED ? true : false);
	prefs.set_calc_only_my_turn(m_CalcOnMyTurn.GetCheck()==BST_CHECKED ? true : false);
	prefs.set_need_balance_stability(m_NeedBalanceStability.GetCheck()==BST_CHECKED ? true : false);

	CSAPrefsSubDlg::OnOK();
}

void CDlgSAPrefs2::OnDeltaposSwagdelay1Spin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	pNMUpDown->iDelta*=100;
	*pResult = 0;
}

void CDlgSAPrefs2::OnDeltaposSwagdelay2Spin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	pNMUpDown->iDelta*=100;
	*pResult = 0;
}

void CDlgSAPrefs2::OnDeltaposSwagdelay3Spin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	pNMUpDown->iDelta*=100;
	*pResult = 0;
}
