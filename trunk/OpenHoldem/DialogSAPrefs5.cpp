// DialogSAPrefs5.cpp : implementation file
//

#include "stdafx.h"

#include "SAPrefsSubDlg.h"
#include "DialogSAPrefs5.h"

#include "CPreferences.h"

// CDlgSAPrefs5 dialog

IMPLEMENT_DYNAMIC(CDlgSAPrefs5, CSAPrefsSubDlg)

CDlgSAPrefs5::CDlgSAPrefs5(CWnd* pParent /*=NULL*/)
		: CSAPrefsSubDlg(CDlgSAPrefs5::IDD, pParent)
{
}

CDlgSAPrefs5::~CDlgSAPrefs5()
{
}

void CDlgSAPrefs5::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AVTIME, m_AvTime);
}

BEGIN_MESSAGE_MAP(CDlgSAPrefs5, CSAPrefsSubDlg)
END_MESSAGE_MAP()

// CDlgSAPrefs5 message handlers
BOOL CDlgSAPrefs5::OnInitDialog()
{
	CString		text = "";

	CSAPrefsSubDlg::OnInitDialog();

	text.Format("%.2f", prefs.sym_av_time());
	m_AvTime.SetWindowText(text);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs5::OnOK()
{
	CString			text = "";

	m_AvTime.GetWindowText(text);
	prefs.set_sym_av_time(atof(text.GetString()));

	CSAPrefsSubDlg::OnOK();
}
