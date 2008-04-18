// DialogSAPrefs1.cpp : implementation file
//

#include "stdafx.h"
#include "SAPrefsSubDlg.h"
#include "DialogSAPrefs1.h"
#include "Registry.h"
#include "global.h"

// CDlgSAPrefs1 dialog

IMPLEMENT_DYNAMIC(CDlgSAPrefs1, CSAPrefsSubDlg)

CDlgSAPrefs1::CDlgSAPrefs1(CWnd* pParent /*=NULL*/)
        : CSAPrefsSubDlg(CDlgSAPrefs1::IDD, pParent)
{

}

CDlgSAPrefs1::~CDlgSAPrefs1()
{
}

void CDlgSAPrefs1::DoDataExchange(CDataExchange* pDX)
{
    CSAPrefsSubDlg::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MAXOPPONENTS, m_MaxOpponents);
    DDX_Control(pDX, IDC_MAXOPPONENTS_SPIN, m_MaxOpponents_Spin);
}


BEGIN_MESSAGE_MAP(CDlgSAPrefs1, CSAPrefsSubDlg)
END_MESSAGE_MAP()


// CDlgSAPrefs1 message handlers

BOOL CDlgSAPrefs1::OnInitDialog()
{
    CSAPrefsSubDlg::OnInitDialog();
    CString		text;

    text.Format("%d", global.preferences.max_opponents);
    m_MaxOpponents.SetWindowText(text);
    m_MaxOpponents_Spin.SetRange(1, 22);
    m_MaxOpponents_Spin.SetPos(global.preferences.max_opponents);
    m_MaxOpponents_Spin.SetBuddy(&m_MaxOpponents);


    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs1::OnOK()
{
    Registry		reg;
    CString			text;

    m_MaxOpponents.GetWindowText(text);
    if (strtoul(text.GetString(), 0, 10)<1 || strtoul(text.GetString(), 0, 10)>MAX_OPPONENTS) {
        MessageBox("Invalid Max Opponents", "ERROR", MB_OK);
        return;
    }
    global.preferences.max_opponents = strtoul(text.GetString(), 0, 10);

    reg.read_reg();
    reg.max_opponents = global.preferences.max_opponents;
    reg.write_reg();


    CSAPrefsSubDlg::OnOK();
}
