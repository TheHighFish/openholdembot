// DialogSAPrefs2.cpp : implementation file
//

#include "stdafx.h"
#include "SAPrefsSubDlg.h"
#include "DialogSAPrefs2.h"
#include "Registry.h"
#include "global.h"


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
    DDX_Control(pDX, IDC_TEXTSELECTION, m_TextSelection);
    DDX_Control(pDX, IDC_TEXTDELETION, m_TextDeletion);
    DDX_Control(pDX, IDC_BETCONFIRMATION, m_BetConfirmation);
    DDX_Control(pDX, IDC_BUTTONCLICKING, m_ButtonClicking);
    DDX_Control(pDX, IDC_AUTO_AP, m_AutoAP);
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
    CString		text;

    text.Format("%d", global.preferences.frame_delay);
    m_FrameDelay.SetWindowText(text);
    m_FrameDelay_Spin.SetRange(0, 9);
    m_FrameDelay_Spin.SetPos(global.preferences.frame_delay);
    m_FrameDelay_Spin.SetBuddy(&m_FrameDelay);

    text.Format("%d", global.preferences.swag_delay_1);
    m_SwagDelay1.SetWindowText(text);
    m_SwagDelay1_Spin.SetRange(0, 5000);
    m_SwagDelay1_Spin.SetPos(global.preferences.swag_delay_1);
    m_SwagDelay1_Spin.SetBuddy(&m_SwagDelay1);

    text.Format("%d", global.preferences.swag_delay_2);
    m_SwagDelay2.SetWindowText(text);
    m_SwagDelay2_Spin.SetRange(0, 5000);
    m_SwagDelay2_Spin.SetPos(global.preferences.swag_delay_2);
    m_SwagDelay2_Spin.SetBuddy(&m_SwagDelay2);

    text.Format("%d", global.preferences.swag_delay_3);
    m_SwagDelay3.SetWindowText(text);
    m_SwagDelay3_Spin.SetRange(0, 5000);
    m_SwagDelay3_Spin.SetPos(global.preferences.swag_delay_3);
    m_SwagDelay3_Spin.SetBuddy(&m_SwagDelay3);

    m_TextSelection.AddString("Dbl Click");
    m_TextSelection.AddString("Click Drag");
    if (global.preferences.text_selection_method == TEXTSEL_DOUBLECLICK)
        m_TextSelection.SelectString(0, "Dbl Click");
    else if (global.preferences.text_selection_method == TEXTSEL_CLICKDRAG)
        m_TextSelection.SelectString(0, "Click Drag");

    m_TextDeletion.AddString("Delete");
    m_TextDeletion.AddString("Backspace");
    if (global.preferences.text_deletion_method == TEXTDEL_DELETE)
        m_TextDeletion.SelectString(0, "Delete");
    else if (global.preferences.text_deletion_method == TEXTDEL_BACKSPACE)
        m_TextDeletion.SelectString(0, "Backspace");

    m_BetConfirmation.AddString("Enter");
    m_BetConfirmation.AddString("Click Bet");
    if (global.preferences.bet_confirmation_method == BETCONF_ENTER)
        m_BetConfirmation.SelectString(0, "Enter");
    else if (global.preferences.bet_confirmation_method == BETCONF_CLICKBET)
        m_BetConfirmation.SelectString(0, "Click Bet");

    m_ButtonClicking.AddString("Single");
    m_ButtonClicking.AddString("Double");
    if (global.preferences.button_click_method == BUTTON_SINGLECLICK)
        m_ButtonClicking.SelectString(0, "Single");
    else if (global.preferences.button_click_method == BUTTON_DOUBLECLICK)
        m_ButtonClicking.SelectString(0, "Double");

    m_AutoAP.SetCheck(global.preferences.ap_auto ? BST_CHECKED : BST_UNCHECKED);


    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs2::OnOK()
{
    Registry		reg;
    CString			text;

    m_FrameDelay.GetWindowText(text);
    if (strtoul(text.GetString(), 0, 10)>MAX_FRAMEDELAY)
    {
        MessageBox("Invalid Frame Delay", "ERROR", MB_OK);
        return;
    }
    global.preferences.frame_delay = strtoul(text.GetString(), 0, 10);

    m_SwagDelay1.GetWindowText(text);
    if (strtoul(text.GetString(), 0, 10)>MAX_SWAGDELAY1)
    {
        MessageBox("Invalid Swag Delay (Select to Delete)", "ERROR", MB_OK);
        return;
    }
    global.preferences.swag_delay_1 = strtoul(text.GetString(), 0, 10);

    m_SwagDelay2.GetWindowText(text);
    if (strtoul(text.GetString(), 0, 10)>MAX_SWAGDELAY2)
    {
        MessageBox("Invalid Swag Delay (Delete to Entry)", "ERROR", MB_OK);
        return;
    }
    global.preferences.swag_delay_2 = strtoul(text.GetString(), 0, 10);

    m_SwagDelay3.GetWindowText(text);
    if (strtoul(text.GetString(), 0, 10)>MAX_SWAGDELAY3) {
        MessageBox("Invalid Swag Delay (Entry to Confirm)", "ERROR", MB_OK);
        return;
    }
    global.preferences.swag_delay_3 = strtoul(text.GetString(), 0, 10);

    if (m_TextSelection.GetCurSel()==0)
        global.preferences.text_selection_method = TEXTSEL_DOUBLECLICK;
    else if (m_TextSelection.GetCurSel()==1)
        global.preferences.text_selection_method = TEXTSEL_CLICKDRAG;

    if (m_TextDeletion.GetCurSel()==0)
        global.preferences.text_deletion_method = TEXTDEL_DELETE;
    else if (m_TextDeletion.GetCurSel()==1)
        global.preferences.text_deletion_method = TEXTDEL_BACKSPACE;

    if (m_BetConfirmation.GetCurSel()==0)
        global.preferences.bet_confirmation_method = BETCONF_ENTER;
    else if (m_BetConfirmation.GetCurSel()==1)
        global.preferences.bet_confirmation_method = BETCONF_CLICKBET;

    if (m_ButtonClicking.GetCurSel()==0)
        global.preferences.button_click_method = BUTTON_SINGLECLICK;
    else if (m_ButtonClicking.GetCurSel()==1)
        global.preferences.button_click_method = BUTTON_DOUBLECLICK;

    global.preferences.ap_auto = m_AutoAP.GetCheck()==BST_CHECKED ? true : false;

    reg.read_reg();
    reg.frame_delay = global.preferences.frame_delay;
    reg.swag_delay_1 = global.preferences.swag_delay_1;
    reg.swag_delay_2 = global.preferences.swag_delay_2;
    reg.swag_delay_3 = global.preferences.swag_delay_3;
    reg.text_selection_method = global.preferences.text_selection_method;
    reg.text_deletion_method = global.preferences.text_deletion_method;
    reg.bet_confirmation_method = global.preferences.bet_confirmation_method;
    reg.button_click_method = global.preferences.button_click_method;
    reg.ap_auto = global.preferences.ap_auto;
    reg.write_reg();

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
