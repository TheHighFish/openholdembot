// DialogSAPrefs3.cpp : implementation file
//

#include "stdafx.h"

#include "SAPrefsSubDlg.h"
#include "DialogSAPrefs3.h"
#include "CPreferences.h"

// CDlgSAPrefs3 dialog

IMPLEMENT_DYNAMIC(CDlgSAPrefs3, CSAPrefsSubDlg)

CDlgSAPrefs3::CDlgSAPrefs3(CWnd* pParent /*=NULL*/)
		: CSAPrefsSubDlg(CDlgSAPrefs3::IDD, pParent)
{
}

CDlgSAPrefs3::~CDlgSAPrefs3()
{
}

void CDlgSAPrefs3::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DLLNAME, m_DllName);
}

BEGIN_MESSAGE_MAP(CDlgSAPrefs3, CSAPrefsSubDlg)
END_MESSAGE_MAP()

// CDlgSAPrefs3 message handlers
BOOL CDlgSAPrefs3::OnInitDialog()
{
	CSAPrefsSubDlg::OnInitDialog();
	m_DllName.SetWindowText(prefs.dll_name());

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs3::OnOK()
{
	CString			text = "";

	m_DllName.GetWindowText(text);
	prefs.set_dll_name(text);

	CSAPrefsSubDlg::OnOK();
}
