// DialogSAPrefs1.cpp : implementation file
//

#include "stdafx.h"
#include "DialogSAPrefs21.h"

#include "SAPrefsSubDlg.h"
#include "CPreferences.h"

// CDlgSAPrefs21 dialog

IMPLEMENT_DYNAMIC(CDlgSAPrefs21, CSAPrefsSubDlg)

CDlgSAPrefs21::CDlgSAPrefs21(CWnd* pParent /*=NULL*/)
		: CSAPrefsSubDlg(CDlgSAPrefs21::IDD, pParent)
{
}

CDlgSAPrefs21::~CDlgSAPrefs21()
{
}

void CDlgSAPrefs21::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_POSITION_NEVER, _position_tables_never_button);
	DDX_Control(pDX, IDC_RADIO_POSITION_TILED, _position_tables_tiled_button);
	DDX_Control(pDX, IDC_RADIO_POSITION_CASCADED, _position_tables_cascaded_button);
}

BEGIN_MESSAGE_MAP(CDlgSAPrefs21, CSAPrefsSubDlg)
END_MESSAGE_MAP()

// CDlgSAPrefs1 message handlers
BOOL CDlgSAPrefs21::OnInitDialog()
{
	CSAPrefsSubDlg::OnInitDialog();
	if (preferences.table_positioner_options() == k_position_tables_tiled)
	{
		_position_tables_tiled_button.SetCheck(true);
	}
	else if (preferences.table_positioner_options() == k_position_tables_cascaded)
	{
		_position_tables_cascaded_button.SetCheck(true);
	}
	else
	{
		// Default: k_lazy_scraping_always
		_position_tables_never_button.SetCheck(true);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs21::OnOK()
{

	if (_position_tables_tiled_button.GetCheck())
	{
		preferences.set_table_positioner_options(k_position_tables_tiled);
	}
	else if (_position_tables_cascaded_button.GetCheck())
	{
		preferences.set_table_positioner_options(k_position_tables_cascaded);
	}
	else
	{
		// Default: k_lazy_scraping_never
		preferences.set_table_positioner_options(k_position_tables_never);
	}
	CSAPrefsSubDlg::OnOK();
}
