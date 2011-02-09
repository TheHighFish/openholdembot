// CDlgSAPrefs15.cpp : implementation file
//

#include "stdafx.h"

#include "SAPrefsSubDlg.h"
#include "DialogSAPrefs15.h"
#include "CPreferences.h"
#include "OH_MessageBox.h"


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
	DDX_Control(pDX, IDC_GUI_START_MINIMIZED, _gui_start_minimized_Button);
	DDX_Control(pDX, IDC_GUI_Disable_Progress_Dialog, _gui_disable_progress_dialog_Button);

	DDX_Control(pDX, IDC_INFOBOXSIZE_EDIT, m_InfoboxSize_Edit);
	DDX_Control(pDX, IDC_INFOBOXSIZE_SPIN, m_InfoboxSize_Spin);
}


BEGIN_MESSAGE_MAP(CDlgSAPrefs15, CDialog)
END_MESSAGE_MAP()


// CDlgSAPrefs15 message handlers
BOOL CDlgSAPrefs15::OnInitDialog()
{
	CString		text = "";
	CSAPrefsSubDlg::OnInitDialog();

	_gui_start_minimized_Button.SetCheck(prefs.gui_start_minimized());
	_gui_disable_progress_dialog_Button.SetCheck(prefs.gui_disable_progress_dialog());

	text.Format("%d", prefs.infobox_size());
	m_InfoboxSize_Edit.SetWindowText(text);
	m_InfoboxSize_Spin.SetRange(5, 50);
	m_InfoboxSize_Spin.SetPos(prefs.infobox_size());
	m_InfoboxSize_Spin.SetBuddy(&m_InfoboxSize_Edit);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs15::OnOK()
{
	CString			text = "";

	prefs.set_gui_start_minimized(_gui_start_minimized_Button.GetCheck() == true);
	prefs.set_gui_disable_progress_dialog(_gui_disable_progress_dialog_Button.GetCheck() == true);

	m_InfoboxSize_Edit.GetWindowText(text);
	if (strtoul(text.GetString(), 0, 10) < 5 || strtoul(text.GetString(), 0, 10) > 50) {
		OH_MessageBox_Interactive("Invalid Info Box Size", "ERROR", MB_OK);
		return;
	}
	prefs.set_infobox_size(strtoul(text.GetString(), 0, 10));

	CSAPrefsSubDlg::OnOK();
}