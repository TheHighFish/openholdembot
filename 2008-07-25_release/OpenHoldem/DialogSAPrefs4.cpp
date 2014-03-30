// DialogSAPrefs4.cpp : implementation file
//

#include "stdafx.h"
#include "SAPrefsSubDlg.h"
#include "DialogSAPrefs4.h"
#include "global.h"
#include "Registry.h"


// CDlgSAPrefs4 dialog

IMPLEMENT_DYNAMIC(CDlgSAPrefs4, CSAPrefsSubDlg)

CDlgSAPrefs4::CDlgSAPrefs4(CWnd* pParent /*=NULL*/)
        : CSAPrefsSubDlg(CDlgSAPrefs4::IDD, pParent)
{

}

CDlgSAPrefs4::~CDlgSAPrefs4()
{
}

void CDlgSAPrefs4::DoDataExchange(CDataExchange* pDX)
{
    CSAPrefsSubDlg::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SCRAPEDELAY, m_ScrapeDelay);
    DDX_Control(pDX, IDC_SCRAPEDELAY_SPIN, m_ScrapeDelay_Spin);
}


BEGIN_MESSAGE_MAP(CDlgSAPrefs4, CSAPrefsSubDlg)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SCRAPEDELAY_SPIN, &CDlgSAPrefs4::OnDeltaposScrapedelaySpin)
END_MESSAGE_MAP()


// CDlgSAPrefs4 message handlers

BOOL CDlgSAPrefs4::OnInitDialog()
{
    CSAPrefsSubDlg::OnInitDialog();
    CString		text;

    text.Format("%d", global.preferences.scrape_delay);
    m_ScrapeDelay.SetWindowText(text);
    m_ScrapeDelay_Spin.SetRange(100, 5000);
    m_ScrapeDelay_Spin.SetPos(global.preferences.scrape_delay);
    m_ScrapeDelay_Spin.SetBuddy(&m_ScrapeDelay);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs4::OnOK()
{
    Registry		reg;
    CString			text;

    m_ScrapeDelay.GetWindowText(text);
    if (strtoul(text.GetString(), 0, 10)<MIN_SCRAPEDELAY || strtoul(text.GetString(), 0, 10)>MAX_SCRAPEDELAY) {
        MessageBox("Invalid Scrape Delay", "ERROR", MB_OK);
        return;
    }
    global.preferences.scrape_delay = strtoul(text.GetString(), 0, 10);

    reg.read_reg();
    reg.scrape_delay = global.preferences.scrape_delay;
    reg.write_reg();

    CSAPrefsSubDlg::OnOK();
}

void CDlgSAPrefs4::OnDeltaposScrapedelaySpin(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    pNMUpDown->iDelta*=100;
    *pResult = 0;
}
