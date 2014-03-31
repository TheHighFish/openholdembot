//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*****************************************************************************
//
// Purpose:
//
//*****************************************************************************

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

	DDX_Control(pDX, IDC_RADIO_AUTOCONNECTOR_3, _autoconnector_connect_never);
	DDX_Control(pDX, IDC_RADIO_AUTOCONNECTOR_4, _autoconnector_connect_once);
	DDX_Control(pDX, IDC_RADIO_AUTOCONNECTOR_5, _autoconnector_connect_permanent);
	DDX_Control(pDX, IDC_AUTOCONNECTOR_CLOSE_WHEN_TABLE_DISAPPEARS, _autoconnector_close_when_table_disappears);
}


BEGIN_MESSAGE_MAP(CDlgSAPrefs14, CDialog)
END_MESSAGE_MAP()


// DialogSAPrefs14 message handlers

BOOL CDlgSAPrefs14::OnInitDialog()
{
	CString		text = "";

	CSAPrefsSubDlg::OnInitDialog();

	if (preferences.autoconnector_when_to_connect() == k_AutoConnector_Connect_Never)
	{
		_autoconnector_connect_never.SetCheck(true);
	}
	else if (preferences.autoconnector_when_to_connect() == k_AutoConnector_Connect_Once)
	{
		_autoconnector_connect_once.SetCheck(true);
	}
	else // k_AutoConnector_Connect_Permanent
	{
		_autoconnector_connect_permanent.SetCheck(true);
	}

	_autoconnector_close_when_table_disappears.SetCheck(preferences.autoconnector_close_when_table_disappears());

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgSAPrefs14::OnOK()
{
	if (bool(_autoconnector_connect_never.GetCheck()) == true)
	{
		preferences.SetValue(k_prefs_autoconnector_when_to_connect, k_AutoConnector_Connect_Never);
	}
	else if (bool(_autoconnector_connect_once.GetCheck()) == true)
	{
		preferences.SetValue(k_prefs_autoconnector_when_to_connect, k_AutoConnector_Connect_Once);
	}
	else // _autoconnector_connect_permanent.GetCheck() == true
	{
		preferences.SetValue(k_prefs_autoconnector_when_to_connect, k_AutoConnector_Connect_Permanent);
	}

	preferences.SetValue(k_prefs_autoconnector_close_when_table_disappears, _autoconnector_close_when_table_disappears.GetCheck());
	CSAPrefsSubDlg::OnOK();
}
