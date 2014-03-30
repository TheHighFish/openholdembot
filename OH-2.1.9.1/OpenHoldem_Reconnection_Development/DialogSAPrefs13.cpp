// DialogSAPrefs12.cpp : implementation file
//

#include "stdafx.h"

#include "SAPrefsSubDlg.h"
#include "DialogSAPrefs13.h"
#include "CPreferences.h"


// CDlgSAPrefs13 dialog

IMPLEMENT_DYNAMIC(CDlgSAPrefs13, CDialog)

CDlgSAPrefs13::CDlgSAPrefs13(CWnd* pParent /*=NULL*/)
	: CSAPrefsSubDlg(CDlgSAPrefs13::IDD, pParent)
{

}

CDlgSAPrefs13::~CDlgSAPrefs13()
{
}

void CDlgSAPrefs13::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OMF_CLASS_NAME, _class_name_edit);
	DDX_Control(pDX, IDC_OMF_MUTEX_NAME, _class_mutex_edit);
	DDX_Control(pDX, IDC_SIMPLE_TITLE, _simple_window_title);
}


BEGIN_MESSAGE_MAP(CDlgSAPrefs13, CDialog)
END_MESSAGE_MAP()


// CDlgSAPrefs13 message handlers
BOOL CDlgSAPrefs13::OnInitDialog()
{
	CSAPrefsSubDlg::OnInitDialog();

	_class_name_edit.SetWindowText(prefs.window_class_name());
	_class_mutex_edit.SetWindowText(prefs.mutex_name());
	_simple_window_title.SetCheck(prefs.simple_window_title());

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs13::OnOK()
{
	CString temp;

	_class_name_edit.GetWindowText(temp);
	prefs.set_window_class_name(temp);
	_class_mutex_edit.GetWindowText(temp);
	prefs.set_mutex_name(temp);

	prefs.set_simple_window_title(_simple_window_title.GetCheck() == 1);

	CSAPrefsSubDlg::OnOK();
}