// DialogSitDown.cpp : implementation file
//

#include "stdafx.h"
#include "OpenHoldem.h"
#include "DialogSitDown.h"
#include "CPokerPro.h"

// CDlgSitDown dialog

IMPLEMENT_DYNAMIC(CDlgSitDown, CDialog)

CDlgSitDown::CDlgSitDown(CWnd* pParent /*=NULL*/) : CDialog(CDlgSitDown::IDD, pParent) 
{
	__SEH_SET_EXCEPTION_HANDLER
}

CDlgSitDown::~CDlgSitDown()
{
}

void CDlgSitDown::DoDataExchange(CDataExchange* pDX) 
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AVAILABLECHAIRS, m_AvailableChairs);
}

BEGIN_MESSAGE_MAP(CDlgSitDown, CDialog)
	ON_LBN_DBLCLK(IDC_AVAILABLECHAIRS, &CDlgSitDown::OnLbnDblclkAvailablechairs)
	ON_BN_CLICKED(IDOK, &CDlgSitDown::OnBnClickedOk)
END_MESSAGE_MAP()

// CDlgSitDown message handlers
BOOL CDlgSitDown::OnInitDialog() 
{
	int		i = 0;
	CString s = "";

	CDialog::OnInitDialog();

	nvacant = 0;
	for (i=0; i<10; i++) 
	{
		if (p_pokerpro->ppdata()->m_pinf[i].m_isSeated&0x1)
			continue;

		vacant[nvacant++] = i;
		s.Format("Chair %d", i);
		m_AvailableChairs.AddString(s.GetString());
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSitDown::OnLbnDblclkAvailablechairs() 
{
	m_selected_chair = vacant[m_AvailableChairs.GetCurSel()];
	OnOK();
}

void CDlgSitDown::OnBnClickedOk() 
{
	OnOK();
	m_selected_chair = vacant[m_AvailableChairs.GetCurSel()];
}


