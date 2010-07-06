// DialogChairNum.cpp : implementation file
//

#include "stdafx.h"
#include "DialogChairNum.h"


// CDlgChairNum dialog

IMPLEMENT_DYNAMIC(CDlgChairNum, CDialog)

CDlgChairNum::CDlgChairNum(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgChairNum::IDD, pParent)
{

}

CDlgChairNum::~CDlgChairNum()
{
}

void CDlgChairNum::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHAIRNUM, m_ChairNum);
}


BEGIN_MESSAGE_MAP(CDlgChairNum, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgChairNum::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgChairNum message handlers

BOOL CDlgChairNum::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ChairNum.SetFocus();

	return false;  // return TRUE unless you set the focus to a control
}

void CDlgChairNum::OnBnClickedOk()
{
	CString text;

	m_ChairNum.GetWindowText(text);
	chair = strtoul(text.GetString(), NULL, 10);

	OnOK();
}
