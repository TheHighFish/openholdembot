//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

// DialogSAPrefs9.cpp : implementation file
//

#include "stdafx.h"
#include "DialogSAPrefs9.h"

#include "CPreferences.h"
#include "SAPrefsSubDlg.h"

// DialogSAPrefs9 dialog

IMPLEMENT_DYNAMIC(CDlgSAPrefs9, CDialog)

CDlgSAPrefs9::CDlgSAPrefs9(CWnd* pParent /*=NULL*/)
		: CSAPrefsSubDlg(CDlgSAPrefs9::IDD, pParent)
{
}

CDlgSAPrefs9::~CDlgSAPrefs9()
{
}

void CDlgSAPrefs9::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
}

BOOL CDlgSAPrefs9::OnInitDialog()
{
	CSAPrefsSubDlg::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs9::OnOK()
{
	CSAPrefsSubDlg::OnOK();
}

BEGIN_MESSAGE_MAP(CDlgSAPrefs9, CDialog)
END_MESSAGE_MAP()

// DialogSAPrefs9 message handlers
