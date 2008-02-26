// EditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EditDlg.h"


// CEditDlg dialog

IMPLEMENT_DYNAMIC(CEditDlg, CDialog)

CEditDlg::CEditDlg(CWnd* pParent /*=NULL*/)	: CDialog(CEditDlg::IDD, pParent) {

}

CEditDlg::~CEditDlg() {
}

void CEditDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITENTRY, m_EditEntry);
}


BEGIN_MESSAGE_MAP(CEditDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CEditDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CEditDlg message handlers

BOOL CEditDlg::OnInitDialog() {
	CDialog::OnInitDialog();

	SetWindowText(m_titletext);
	m_EditEntry.SetWindowText(m_result);
	m_EditEntry.SetSel(MAKEWORD(0,-1));
	m_EditEntry.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEditDlg::OnBnClickedOk() {
	 m_EditEntry.GetWindowText(m_result);

	OnOK();
}
