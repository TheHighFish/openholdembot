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
	DDX_Control(pDX, IDC_RADIO_AUTOCONNECTOR_3, _autoconnector_connect_never);
	DDX_Control(pDX, IDC_RADIO_AUTOCONNECTOR_4, _autoconnector_connect_once);
	DDX_Control(pDX, IDC_RADIO_AUTOCONNECTOR_5, _autoconnector_connect_permanent);
	DDX_Control(pDX, IDC_RECONNECT_TIME_EDIT, _autoconnector_time_to_next_try_edit);
	DDX_Control(pDX, IDC_RECONNECT_TIME_SPIN, _autoconnector_time_to_next_try_spin);
	DDX_Control(pDX, IDC_AUTOCONNECTOR_CLOSE_WHEN_TABLE_DISAPPEARS, _autoconnector_close_when_table_disappears);
}


BEGIN_MESSAGE_MAP(CDlgSAPrefs14, CDialog)
END_MESSAGE_MAP()


// DialogSAPrefs14 message handlers

BOOL CDlgSAPrefs14::OnInitDialog()
{
	CString		text = "";

	CSAPrefsSubDlg::OnInitDialog();

	if (prefs.autoconnector_connection_method() == k_AutoConnector_Connect_Manually)
	{
		_autoconnector_connection_manually.SetCheck(true);
	}
	else // k_AutoConnector_Connect_Automatically
	{
		_autoconnector_connection_automatically.SetCheck(true);
	}

	if (prefs.autoconnector_when_to_connect() == k_AutoConnector_Connect_Never)
	{
		_autoconnector_connect_never.SetCheck(true);
	}
	else if (prefs.autoconnector_when_to_connect() == k_AutoConnector_Connect_Once)
	{
		_autoconnector_connect_once.SetCheck(true);
	}
	else // k_AutoConnector_Connect_Permanent
	{
		_autoconnector_connect_permanent.SetCheck(true);
	}

	text.Format("%d", prefs.autoconnector_time_to_next_try());
	_autoconnector_time_to_next_try_edit.SetWindowText(text);
	_autoconnector_time_to_next_try_spin.SetRange(0, 100);
	_autoconnector_time_to_next_try_spin.SetPos(prefs.autoconnector_time_to_next_try());
	_autoconnector_time_to_next_try_spin.SetBuddy(&_autoconnector_time_to_next_try_edit);

	_autoconnector_close_when_table_disappears.SetCheck(prefs.autoconnector_close_when_table_disappears());

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgSAPrefs14::OnOK()
{
	CString text;

	if (_autoconnector_connection_manually.GetCheck() == true)
	{
		prefs.set_autoconnector_connection_method(k_AutoConnector_Connect_Manually);
	}
	else // _autoconnector_connection_automatically.GetCheck() == true
	{
		prefs.set_autoconnector_connection_method(k_AutoConnector_Connect_Automatically);
	}

	if (_autoconnector_connect_never.GetCheck() == true)
	{
		prefs.set_autoconnector_when_to_connect(k_AutoConnector_Connect_Never);
	}
	else if (_autoconnector_connect_once.GetCheck() == true)
	{
		prefs.set_autoconnector_when_to_connect(k_AutoConnector_Connect_Once);
	}
	else // _autoconnector_connect_permanent.GetCheck() == true
	{
		prefs.set_autoconnector_when_to_connect(k_AutoConnector_Connect_Permanent);
	}

	_autoconnector_time_to_next_try_edit.GetWindowText(text);
	if (strtoul(text.GetString(), 0, 10)<0 || strtoul(text.GetString(), 0, 10)>100) 
	{
		MessageBox("Invalid time to next connection attempt", "ERROR", MB_OK);
		return;
	}
	prefs.set_autoconnector_time_to_next_try(strtoul(text.GetString(), 0, 10));

	prefs.set_autoconnector_close_when_table_disappears(_autoconnector_close_when_table_disappears.GetCheck());
	CSAPrefsSubDlg::OnOK();
}
