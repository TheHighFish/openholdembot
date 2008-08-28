// DialogSAPrefs7.cpp : implementation file
//

#include "stdafx.h"

#include "SAPrefsSubDlg.h"
#include "DialogSAPrefs7.h"

#include "CGlobal.h"

#include "Registry.h"


// CDlgSAPrefs7 dialog

IMPLEMENT_DYNAMIC(CDlgSAPrefs7, CDialog)

CDlgSAPrefs7::CDlgSAPrefs7(CWnd* pParent /*=NULL*/)
        : CSAPrefsSubDlg(CDlgSAPrefs7::IDD, pParent)
{

}

CDlgSAPrefs7::~CDlgSAPrefs7()
{
}

void CDlgSAPrefs7::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_ICM_1, m_ICM1);
    DDX_Control(pDX, IDC_ICM_2, m_ICM2);
    DDX_Control(pDX, IDC_ICM_3, m_ICM3);
    DDX_Control(pDX, IDC_ICM_4, m_ICM4);
}


BEGIN_MESSAGE_MAP(CDlgSAPrefs7, CDialog)
END_MESSAGE_MAP()


// CDlgSAPrefs7 message handlers

BOOL CDlgSAPrefs7::OnInitDialog()
{
    CString text;

    CSAPrefsSubDlg::OnInitDialog();

    if (p_global->preferences.icm_prize1 == (int) p_global->preferences.icm_prize1)
        text.Format("%.0f", p_global->preferences.icm_prize1);
    else
        text.Format("%f", p_global->preferences.icm_prize1);
    m_ICM1.SetWindowText(text);

    if (p_global->preferences.icm_prize2 == (int) p_global->preferences.icm_prize2)
        text.Format("%.0f", p_global->preferences.icm_prize2);
    else
        text.Format("%f", p_global->preferences.icm_prize2);
    m_ICM2.SetWindowText(text);

    if (p_global->preferences.icm_prize3 == (int) p_global->preferences.icm_prize3)
        text.Format("%.0f", p_global->preferences.icm_prize3);
    else
        text.Format("%f", p_global->preferences.icm_prize3);
    m_ICM3.SetWindowText(text);

    if (p_global->preferences.icm_prize4 == (int) p_global->preferences.icm_prize4)
        text.Format("%.0f", p_global->preferences.icm_prize4);
    else
        text.Format("%f", p_global->preferences.icm_prize4);
    m_ICM4.SetWindowText(text);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs7::OnOK()
{
    Registry		reg;
    CString			text;

    m_ICM1.GetWindowText(text);
    p_global->preferences.icm_prize1 = atof(text.GetString());
    m_ICM2.GetWindowText(text);
    p_global->preferences.icm_prize2 = atof(text.GetString());
    m_ICM3.GetWindowText(text);
    p_global->preferences.icm_prize3 = atof(text.GetString());
    m_ICM4.GetWindowText(text);
    p_global->preferences.icm_prize4 = atof(text.GetString());

    reg.read_reg();
    reg.icm_prize1 = p_global->preferences.icm_prize1;
    reg.icm_prize2 = p_global->preferences.icm_prize2;
    reg.icm_prize3 = p_global->preferences.icm_prize3;
    reg.icm_prize4 = p_global->preferences.icm_prize4;
    reg.write_reg();

    CSAPrefsSubDlg::OnOK();
}
