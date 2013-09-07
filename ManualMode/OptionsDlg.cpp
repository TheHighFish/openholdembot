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


// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ManualMode.h"
#include "OptionsDlg.h"


// OptionsDlg dialog

IMPLEMENT_DYNAMIC(OptionsDlg, CDialog)

OptionsDlg::OptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(OptionsDlg::IDD, pParent)
	, m_unobstructivePopup(FALSE)
{

}

OptionsDlg::~OptionsDlg()
{
}

void OptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_unobstructivePopup);
}


BEGIN_MESSAGE_MAP(OptionsDlg, CDialog)

END_MESSAGE_MAP()


// OptionsDlg message handlers

