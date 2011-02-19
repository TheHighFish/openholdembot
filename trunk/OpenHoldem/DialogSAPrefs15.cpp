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

	DDX_Control(pDX, IDC_INFOBOX_CHECK, m_DisplayInfobox_Check);
	DDX_Control(pDX, IDC_INFOBOX_HAND_CHECK, m_DisplayHand_Check);
	DDX_Control(pDX, IDC_INFOBOX_LIMIT_CHECK, m_DisplayLimit_Check);
	DDX_Control(pDX, IDC_INFOBOX_POT_CHECK, m_DisplayPot_Check);
	DDX_Control(pDX, IDC_INFOBOX_LOGSYMS_CHECK, m_DisplayLogsyms_Check);	
}


BEGIN_MESSAGE_MAP(CDlgSAPrefs15, CDialog)
	ON_BN_CLICKED(IDC_INFOBOX_CHECK, &CDlgSAPrefs15::OnBnClickedDisplayInfobox)
END_MESSAGE_MAP()


// CDlgSAPrefs15 message handlers
BOOL CDlgSAPrefs15::OnInitDialog()
{
	CSAPrefsSubDlg::OnInitDialog();

	_gui_start_minimized_Button.SetCheck(prefs.gui_start_minimized());
	_gui_disable_progress_dialog_Button.SetCheck(prefs.gui_disable_progress_dialog());

	if (prefs.infobox_display())
	{
		m_DisplayInfobox_Check.SetCheck(BST_CHECKED);
		m_DisplayHand_Check.SetCheck(prefs.infobox_hand());
		m_DisplayLimit_Check.SetCheck(prefs.infobox_limit());
		m_DisplayPot_Check.SetCheck(prefs.infobox_pot());
		m_DisplayLogsyms_Check.SetCheck(prefs.infobox_logsyms());
	}
	else
	{
		m_DisplayInfobox_Check.SetCheck(BST_UNCHECKED);
		m_DisplayHand_Check.EnableWindow(0);
		m_DisplayLimit_Check.EnableWindow(0);
		m_DisplayPot_Check.EnableWindow(0);
		m_DisplayLogsyms_Check.EnableWindow(0);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs15::OnOK()
{
	prefs.set_gui_start_minimized(_gui_start_minimized_Button.GetCheck() == BST_CHECKED);
	prefs.set_gui_disable_progress_dialog(_gui_disable_progress_dialog_Button.GetCheck() == BST_CHECKED);

	if (m_DisplayInfobox_Check.GetCheck() == BST_CHECKED)
	{
		int selected = 0;

		prefs.set_infobox_display(true);
		prefs.set_infobox_hand(m_DisplayHand_Check.GetCheck() == BST_CHECKED);
		prefs.set_infobox_limit(m_DisplayLimit_Check.GetCheck() == BST_CHECKED);
		prefs.set_infobox_pot(m_DisplayPot_Check.GetCheck() == BST_CHECKED);
		prefs.set_infobox_logsyms(m_DisplayLogsyms_Check.GetCheck() == BST_CHECKED);

		if (m_DisplayLogsyms_Check.GetCheck() == BST_CHECKED && !prefs.log_symbol_enabled())
			prefs.set_log_symbol_enabled(true);

		if (m_DisplayHand_Check.GetCheck() == BST_CHECKED)
			selected += 1;
		if (m_DisplayLimit_Check.GetCheck() == BST_CHECKED)
			selected += 1;
		if (m_DisplayPot_Check.GetCheck() == BST_CHECKED)
			selected += 1;

		if (selected == 0)
		{
			if (m_DisplayLogsyms_Check.GetCheck() == BST_UNCHECKED)
			{
				prefs.set_infobox_hand(true);
				m_DisplayHand_Check.SetCheck(BST_CHECKED);
				prefs.set_infobox_limit(true);
				m_DisplayLimit_Check.SetCheck(BST_CHECKED);
				prefs.set_infobox_pot(true);
				m_DisplayPot_Check.SetCheck(BST_CHECKED);

				prefs.set_infobox_size(48);
				OH_MessageBox_Interactive("Warning: Info Box is set to be displayed but no option was selected\n"
				"Enabling defaults\n", "WARNING", MB_OK);
			}
			else
				prefs.set_infobox_size(0);
		}
		else
			prefs.set_infobox_size(16*selected);
	}
	else
		prefs.set_infobox_display(false);


	CSAPrefsSubDlg::OnOK();
}

void CDlgSAPrefs15::OnBnClickedDisplayInfobox()
{
	if (m_DisplayInfobox_Check.GetCheck() == BST_UNCHECKED)
	{
		m_DisplayHand_Check.EnableWindow(0);
		m_DisplayLimit_Check.EnableWindow(0);
		m_DisplayPot_Check.EnableWindow(0);
		m_DisplayLogsyms_Check.EnableWindow(0);
	}
	else
	{
		m_DisplayHand_Check.EnableWindow(1);
		m_DisplayLimit_Check.EnableWindow(1);
		m_DisplayPot_Check.EnableWindow(1);
		m_DisplayLogsyms_Check.EnableWindow(1);
		m_DisplayHand_Check.SetCheck(prefs.infobox_hand());
		m_DisplayLimit_Check.SetCheck(prefs.infobox_limit());
		m_DisplayPot_Check.SetCheck(prefs.infobox_pot());
		m_DisplayLogsyms_Check.SetCheck(prefs.infobox_logsyms());
	}
}
