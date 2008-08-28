// DialogEditHashPoint.cpp : implementation file
//

#include "stdafx.h"
#include "DialogEditHashPoint.h"
#include "debug.h"


// CDlgEditHashPoint dialog

IMPLEMENT_DYNAMIC(CDlgEditHashPoint, CDialog)

CDlgEditHashPoint::CDlgEditHashPoint(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEditHashPoint::IDD, pParent)
{
	__SEH_SET_EXCEPTION_HANDLER
}

CDlgEditHashPoint::~CDlgEditHashPoint()
{
}

void CDlgEditHashPoint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TYPE, m_Type);
	DDX_Control(pDX, IDC_X, m_X);
	DDX_Control(pDX, IDC_Y, m_Y);
}


BEGIN_MESSAGE_MAP(CDlgEditHashPoint, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgEditHashPoint::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgEditHashPoint message handlers

BOOL CDlgEditHashPoint::OnInitDialog()
{
	__SEH_HEADER

	CDialog::OnInitDialog();

	CString		text;

	CDialog::OnInitDialog();

	SetWindowText(titletext.GetString());

	m_Type.AddString("Type 0");
	m_Type.AddString("Type 1");
	m_Type.AddString("Type 2");
	m_Type.AddString("Type 3");
	m_Type.SelectString(-1, type.GetString());
	m_Type.SetWindowPos(NULL, 0, 0, 75, 300, SWP_NOMOVE | SWP_NOZORDER);

	text.Format("%d", x);
	m_X.SetWindowText(text.GetString());

	text.Format("%d", y);
	m_Y.SetWindowText(text.GetString());

	m_Type.SetFocus();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE

	__SEH_LOGFATAL("CDlgEditHashPoint::OnInitDialog : \n");
}

void CDlgEditHashPoint::OnBnClickedOk()
{
	__SEH_HEADER

	CString		text;

	m_Type.GetWindowText(type);

	m_X.GetWindowText(text);
	x = atoi(text.GetString());

	m_Y.GetWindowText(text);
	y = atoi(text.GetString());

	OnOK();

	__SEH_LOGFATAL("CDlgEditHashPoint::OnBnClickedOk : \n");
}
