// DialogSAPrefs14.cpp : implementation file
//

#include "stdafx.h"

#include "SAPrefsSubDlg.h"
#include "DialogSAPrefs14.h"
#include "CPreferences.h"


// DialogSAPrefs14 dialog

IMPLEMENT_DYNAMIC(CDlgSAPrefs14, CDialog)

CDlgSAPrefs14::CDlgSAPrefs14(CWnd* pParent /*=NULL*/)
	: CSAPrefsSubDlg(CDlgSAPrefs14::IDD, pParent)
{

}

CDlgSAPrefs14::~CDlgSAPrefs14()
{
}

void CDlgSAPrefs14::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_RADIO_AUTOCONNECTOR_1, _autoconnector_connection_manually);
	DDX_Control(pDX, IDC_RADIO_AUTOCONNECTOR_2, _autoconnector_connection_automatically);
	DDX_Control(pDX, IDC_AUTOCONNECTOR_CONNECT_ON_START, _autoconnector_connect_on_start);
	DDX_Control(pDX, IDC_AUTOCONNECTOR_CLOSE_WHEN_TABLE_DISAPPEARS, _autoconnector_close_when_table_disappears);
}


BEGIN_MESSAGE_MAP(CDlgSAPrefs14, CDialog)
END_MESSAGE_MAP()


// DialogSAPrefs14 message handlers

BOOL CDlgSAPrefs14::OnInitDialog()
{
	CSAPrefsSubDlg::OnInitDialog();

	if (prefs.autoconnector_connection_method() == 0)
	{
		_autoconnector_connection_manually.SetCheck(true);
	}
	else
	{
		_autoconnector_connection_automatically.SetCheck(true);
	}
	_autoconnector_connect_on_start.SetCheck(prefs.autoconnector_connect_on_start());
	_autoconnector_close_when_table_disappears.SetCheck(prefs.autoconnector_close_when_table_disappears());

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs14::OnOK()
{
	if (_autoconnector_connection_manually.GetCheck() == true)
	{
		prefs.set_autoconnector_connection_method(0);
	}
	else
	{
		prefs.set_autoconnector_connection_method(1);
	}
	prefs.set_autoconnector_connect_on_start(_autoconnector_connect_on_start.GetCheck());
	prefs.set_autoconnector_close_when_table_disappears(_autoconnector_close_when_table_disappears.GetCheck());
	CSAPrefsSubDlg::OnOK();
}
