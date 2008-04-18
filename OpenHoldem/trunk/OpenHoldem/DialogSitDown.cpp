// DialogSitDown.cpp : implementation file
//

#include "stdafx.h"
#include "OpenHoldem.h"
#include "DialogSitDown.h"
#include "PokerPro.h"
#include "Debug.h"
#include "global.h"

// CDlgSitDown dialog

IMPLEMENT_DYNAMIC(CDlgSitDown, CDialog)

CDlgSitDown::CDlgSitDown(CWnd* pParent /*=NULL*/) : CDialog(CDlgSitDown::IDD, pParent) {

    __SEH_SET_EXCEPTION_HANDLER(MyUnHandledExceptionFilter);


    __SEH_HEADER

    __SEH_LOGFATAL("CDlgSitDown::Constructor : \n");

}

CDlgSitDown::~CDlgSitDown(){
    __SEH_HEADER

    __SEH_LOGFATAL("CDlgSitDown::Destructor : \n");

}

void CDlgSitDown::DoDataExchange(CDataExchange* pDX) {
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_AVAILABLECHAIRS, m_AvailableChairs);
}


BEGIN_MESSAGE_MAP(CDlgSitDown, CDialog)
    ON_LBN_DBLCLK(IDC_AVAILABLECHAIRS, &CDlgSitDown::OnLbnDblclkAvailablechairs)
    ON_BN_CLICKED(IDOK, &CDlgSitDown::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgSitDown message handlers


BOOL CDlgSitDown::OnInitDialog() {
    __SEH_HEADER
    CDialog::OnInitDialog();
    int i;
    CString s;

    nvacant = 0;
    for (i=0; i<10; i++) {
        if (ppro.data.m_pinf[i].m_isSeated&0x1) {
            continue;
        }
        vacant[nvacant++] = i;
        s.Format("Chair %d", i);
        m_AvailableChairs.AddString(s.GetString());
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE

    __SEH_LOGFATAL("CDlgSitDown::OnInitDialog : \n");

}

void CDlgSitDown::OnLbnDblclkAvailablechairs() {
    __SEH_HEADER
    m_selected_chair = vacant[m_AvailableChairs.GetCurSel()];
    OnOK();

    __SEH_LOGFATAL("CDlgSitDown::OnLbnDblclkAvailablechairs : \n");

}

void CDlgSitDown::OnBnClickedOk() {
    __SEH_HEADER
    OnOK();

    m_selected_chair = vacant[m_AvailableChairs.GetCurSel()];

    __SEH_LOGFATAL("CDlgSitDown::OnBnClickedOk : \n");

}


