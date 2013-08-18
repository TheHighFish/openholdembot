// DialogSAPrefs20.cpp : implementation file
//

#include "stdafx.h"
#include "DialogSAPrefs20.h"

#include "CPreferences.h"
#include "SAPrefsSubDlg.h"

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
	CheckDlgButton(IDC_DEBUG_AUTOCONNECTOR , preferences.debug_autoconnector() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_AUTOPLAYER, preferences.debug_autoplayer() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_HEARTBEAT, preferences.debug_heartbeat() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_PRWIN, preferences.debug_prwin() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_ICM, preferences.debug_icm() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_OCCLUSSION, preferences.debug_occlusionchecker() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_POKERTRACKER, preferences.debug_pokertracker() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_REBUY, preferences.debug_rebuy() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_REPLAYFRAMES, preferences.debug_replayframes() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_SCRAPER, preferences.debug_scraper() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_SESSIONCOUNTER, preferences.debug_sessioncounter() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_STABLEFRAMESCOUNTER, preferences.debug_stableframescounter() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_SYMBOLENGINE, preferences.debug_symbolengine() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_BLINDLOCKING, preferences.debug_blindlocking() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_MEMORY_SYMBOLS, preferences.debug_memorysymbols() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_TABLEMAP_LOADER, preferences.debug_tablemap_loader() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_FILESYSTEM_MONITOR, preferences.debug_filesystem_monitor() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_TABLE_POSITIONER, preferences.debug_table_positioner() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_ISTOURNAMENT, preferences.debug_istournament() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_GUI, preferences.debug_gui() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_TABLE_LIMITS, preferences.debug_table_limits() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_LAZY_SCRAPER, preferences.debug_lazy_scraper() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_BETSIZE_ADJUSTMENT, preferences.debug_betsize_adjustment() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_HANDRESET_DETECTOR, preferences.debug_handreset_detector() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_ENGINE_CONTAINER, preferences.debug_engine_container() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_HANDHISTORY, preferences.debug_handhistory() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_ALLTHEREST, preferences.debug_alltherest() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_PREFERENCES, preferences.debug_preferences() ? MF_CHECKED : MF_UNCHECKED);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs20::OnOK()
{
	preferences.set_debug_autoconnector(IsDlgButtonChecked(IDC_DEBUG_AUTOCONNECTOR));
	preferences.set_debug_autoplayer(IsDlgButtonChecked(IDC_DEBUG_AUTOPLAYER));
	preferences.set_debug_heartbeat(IsDlgButtonChecked(IDC_DEBUG_HEARTBEAT));
	preferences.set_debug_prwin(IsDlgButtonChecked(IDC_DEBUG_PRWIN));
	preferences.set_debug_icm(IsDlgButtonChecked(IDC_DEBUG_ICM));
	preferences.set_debug_occlusionchecker(IsDlgButtonChecked(IDC_DEBUG_OCCLUSSION));
	preferences.set_debug_pokertracker(IsDlgButtonChecked(IDC_DEBUG_POKERTRACKER));
	preferences.set_debug_rebuy(IsDlgButtonChecked(IDC_DEBUG_REBUY));
	preferences.set_debug_replayframes(IsDlgButtonChecked(IDC_DEBUG_REPLAYFRAMES));
	preferences.set_debug_scraper(IsDlgButtonChecked(IDC_DEBUG_SCRAPER));
	preferences.set_debug_sessioncounter(IsDlgButtonChecked(IDC_DEBUG_SESSIONCOUNTER));
	preferences.set_debug_stableframescounter(IsDlgButtonChecked(IDC_DEBUG_STABLEFRAMESCOUNTER));
	preferences.set_debug_symbolengine(IsDlgButtonChecked(IDC_DEBUG_SYMBOLENGINE));
	preferences.set_debug_blindlocking(IsDlgButtonChecked(IDC_DEBUG_BLINDLOCKING));
	preferences.set_debug_memorysymbols(IsDlgButtonChecked(IDC_DEBUG_MEMORY_SYMBOLS));
	preferences.set_debug_tablemap_loader(IsDlgButtonChecked(IDC_DEBUG_TABLEMAP_LOADER));
	preferences.set_debug_filesystem_monitor(IsDlgButtonChecked(IDC_DEBUG_FILESYSTEM_MONITOR));
	preferences.set_debug_table_positioner(IsDlgButtonChecked(IDC_DEBUG_TABLE_POSITIONER));
	preferences.set_debug_istournament(IsDlgButtonChecked(IDC_DEBUG_ISTOURNAMENT));
	preferences.set_debug_gui(IsDlgButtonChecked(IDC_DEBUG_GUI));
	preferences.set_debug_table_limits(IsDlgButtonChecked(IDC_DEBUG_TABLE_LIMITS));
	preferences.set_debug_lazy_scraper(IsDlgButtonChecked(IDC_DEBUG_LAZY_SCRAPER));
	preferences.set_debug_betsize_adjustment(IsDlgButtonChecked(IDC_DEBUG_BETSIZE_ADJUSTMENT));
	preferences.set_debug_handreset_detector(IsDlgButtonChecked(IDC_DEBUG_HANDRESET_DETECTOR));
	preferences.set_debug_engine_container(IsDlgButtonChecked(IDC_DEBUG_ENGINE_CONTAINER));
	preferences.set_debug_handhistory(IsDlgButtonChecked(IDC_DEBUG_HANDHISTORY));
	preferences.set_debug_alltherest(IsDlgButtonChecked(IDC_DEBUG_ALLTHEREST));
	preferences.set_debug_preferences(IsDlgButtonChecked(IDC_DEBUG_PREFERENCES));
	CSAPrefsSubDlg::OnOK();
}


