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
#ifdef SEH_ENABLE
	// Set exception handler
	SetUnhandledExceptionFilter(MyUnHandledExceptionFilter);
#endif

#ifdef SEH_ENABLE
	try {
#endif
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgSitDown::Constructor : \n"); 
		throw;
	}
#endif
}

CDlgSitDown::~CDlgSitDown(){
#ifdef SEH_ENABLE
	try {
#endif
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgSitDown::Destructor : \n"); 
		throw;
	}
#endif
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
#ifdef SEH_ENABLE
	try {
#endif
		CDialog::OnInitDialog();
		int i;
		CString s;

		nvacant = 0;
		for (i=0; i<10; i++) {
			if (ppro.data.m_pinf[i].m_isSeated&0x1) { continue; }
			vacant[nvacant++] = i;
			s.Format("Chair %d", i);
			m_AvailableChairs.AddString(s.GetString());
		}

		return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgSitDown::OnInitDialog : \n"); 
		throw;
	}
#endif
}

void CDlgSitDown::OnLbnDblclkAvailablechairs() {
#ifdef SEH_ENABLE
	try {
#endif
		m_selected_chair = vacant[m_AvailableChairs.GetCurSel()];
		OnOK();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgSitDown::OnLbnDblclkAvailablechairs : \n"); 
		throw;
	}
#endif
}

void CDlgSitDown::OnBnClickedOk() {
#ifdef SEH_ENABLE
	try {
#endif
		OnOK();
		
		m_selected_chair = vacant[m_AvailableChairs.GetCurSel()];
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgSitDown::OnBnClickedOk : \n"); 
		throw;
	}
#endif
}


