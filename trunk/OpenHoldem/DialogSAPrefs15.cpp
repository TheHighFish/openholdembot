// CDlgSAPrefs15.cpp : implementation file
//

#include "stdafx.h"

#include "SAPrefsSubDlg.h"
#include "DialogSAPrefs15.h"
#include "CPreferences.h"


// CDlgSAPrefs15 dialog

IMPLEMENT_DYNAMIC(CDlgSAPrefs15, CDialog)

CDlgSAPrefs15::CDlgSAPrefs15(CWnd* pParent /*=NULL*/)
	: CSAPrefsSubDlg(CDlgSAPrefs15::IDD, pParent)
{

}

CDlgSAPrefs15::~CDlgSAPrefs15()
{
}

void CDlgSAPrefs15::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSAPrefs15, CDialog)
END_MESSAGE_MAP()


// CDlgSAPrefs15 message handlers
BOOL CDlgSAPrefs15::OnInitDialog()
{
	return true;
}

void CDlgSAPrefs15::OnOK()
{}