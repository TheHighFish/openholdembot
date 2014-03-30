// DialogSAPrefs20.cpp : implementation file
//

#include "stdafx.h"

#include "SAPrefsSubDlg.h"
#include "DialogSAPrefs20.h"
#include "CPreferences.h"


// CDlgSAPrefs20 dialog

IMPLEMENT_DYNAMIC(CDlgSAPrefs20, CDialog)

CDlgSAPrefs20::CDlgSAPrefs20(CWnd* pParent /*=NULL*/)
	: CSAPrefsSubDlg(CDlgSAPrefs20::IDD, pParent)
{

}

CDlgSAPrefs20::~CDlgSAPrefs20()
{
}

void CDlgSAPrefs20::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	// No need for any DDX.
	// We do it the good old Win95-way. :)
}


BEGIN_MESSAGE_MAP(CDlgSAPrefs20, CDialog)
END_MESSAGE_MAP()


// CDlgSAPrefs20 message handlers
BOOL CDlgSAPrefs20::OnInitDialog()
{	
	CheckDlgButton(IDC_DEBUG_AUTOCONNECTOR , prefs.debug_autoconnector() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_AUTOPLAYER, prefs.debug_autoplayer() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_HEARTBEAT, prefs.debug_heartbeat() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_PRWIN, prefs.debug_prwin() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_ICM, prefs.debug_icm() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_OCCLUSSION, prefs.debug_occlusionchecker() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_POKERTRACKER, prefs.debug_pokertracker() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_REBUY, prefs.debug_rebuy() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_REPLAYFRAMES, prefs.debug_replayframes() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_SCRAPER, prefs.debug_scraper() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_SESSIONCOUNTER, prefs.debug_sessioncounter() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_STABLEFRAMESCOUNTER, prefs.debug_stableframescounter() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_SYMBOLENGINE, prefs.debug_symbolengine() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_BLINDLOCKING, prefs.debug_blindlocking() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_MEMORY_SYMBOLS, prefs.debug_memorysymbols() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_ALLTHEREST, prefs.debug_alltherest() ? MF_CHECKED : MF_UNCHECKED);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs20::OnOK()
{
	prefs.set_debug_autoconnector(IsDlgButtonChecked(IDC_DEBUG_AUTOCONNECTOR));
	prefs.set_debug_autoplayer(IsDlgButtonChecked(IDC_DEBUG_AUTOPLAYER));
	prefs.set_debug_heartbeat(IsDlgButtonChecked(IDC_DEBUG_HEARTBEAT));
	prefs.set_debug_prwin(IsDlgButtonChecked(IDC_DEBUG_PRWIN));
	prefs.set_debug_icm(IsDlgButtonChecked(IDC_DEBUG_ICM));
	prefs.set_debug_occlusionchecker(IsDlgButtonChecked(IDC_DEBUG_OCCLUSSION));
	prefs.set_debug_pokertracker(IsDlgButtonChecked(IDC_DEBUG_POKERTRACKER));
	prefs.set_debug_rebuy(IsDlgButtonChecked(IDC_DEBUG_REBUY));
	prefs.set_debug_replayframes(IsDlgButtonChecked(IDC_DEBUG_REPLAYFRAMES));
	prefs.set_debug_scraper(IsDlgButtonChecked(IDC_DEBUG_SCRAPER));
	prefs.set_debug_sessioncounter(IsDlgButtonChecked(IDC_DEBUG_SESSIONCOUNTER));
	prefs.set_debug_stableframescounter(IsDlgButtonChecked(IDC_DEBUG_STABLEFRAMESCOUNTER));
	prefs.set_debug_symbolengine(IsDlgButtonChecked(IDC_DEBUG_SYMBOLENGINE));
	prefs.set_debug_blindlocking(IsDlgButtonChecked(IDC_DEBUG_BLINDLOCKING));
	prefs.set_debug_memorysymbols(IsDlgButtonChecked(IDC_DEBUG_MEMORY_SYMBOLS));
	prefs.set_debug_alltherest(IsDlgButtonChecked(IDC_DEBUG_ALLTHEREST));
	CSAPrefsSubDlg::OnOK();
}


