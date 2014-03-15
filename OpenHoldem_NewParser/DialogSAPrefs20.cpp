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
	CheckDlgButton(IDC_DEBUG_SYMBOLENGINE_OPENPPL, preferences.debug_symbolengine_open_ppl() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_PARSER, preferences.debug_parser() ? MF_CHECKED : MF_UNCHECKED);
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
	CheckDlgButton(IDC_DEBUG_PREFERENCES, preferences.debug_preferences() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_DLL_EXTENSION, preferences.debug_dll_extension() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_SCRAPER_PREPROCESSOR, preferences.debug_scraper_preprocessor() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_HOPPER_MESSAGES, preferences.debug_hopper_messages() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_OPENHOLDEM, preferences.debug_openholdem() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_POPUP_BLOCKER, preferences.debug_popup_blocker() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_ALLTHEREST, preferences.debug_alltherest() ? MF_CHECKED : MF_UNCHECKED);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs20::OnOK()
{
	preferences.SetValue(k_prefs_debug_autoconnector, IsDlgButtonChecked(IDC_DEBUG_AUTOCONNECTOR));
	preferences.SetValue(k_prefs_debug_autoplayer, IsDlgButtonChecked(IDC_DEBUG_AUTOPLAYER));
	preferences.SetValue(k_prefs_debug_heartbeat, IsDlgButtonChecked(IDC_DEBUG_HEARTBEAT));
	preferences.SetValue(k_prefs_debug_prwin, IsDlgButtonChecked(IDC_DEBUG_PRWIN));
	preferences.SetValue(k_prefs_debug_icm, IsDlgButtonChecked(IDC_DEBUG_ICM));
	preferences.SetValue(k_prefs_debug_occlusionchecker, IsDlgButtonChecked(IDC_DEBUG_OCCLUSSION));
	preferences.SetValue(k_prefs_debug_pokertracker, IsDlgButtonChecked(IDC_DEBUG_POKERTRACKER));
	preferences.SetValue(k_prefs_debug_rebuy, IsDlgButtonChecked(IDC_DEBUG_REBUY));
	preferences.SetValue(k_prefs_debug_replayframes, IsDlgButtonChecked(IDC_DEBUG_REPLAYFRAMES));
	preferences.SetValue(k_prefs_debug_scraper, IsDlgButtonChecked(IDC_DEBUG_SCRAPER));
	preferences.SetValue(k_prefs_debug_sessioncounter, IsDlgButtonChecked(IDC_DEBUG_SESSIONCOUNTER));
	preferences.SetValue(k_prefs_debug_stableframescounter, IsDlgButtonChecked(IDC_DEBUG_STABLEFRAMESCOUNTER));
	preferences.SetValue(k_prefs_debug_symbolengine, IsDlgButtonChecked(IDC_DEBUG_SYMBOLENGINE));
	preferences.SetValue(k_prefs_debug_symbolengine_open_ppl, IsDlgButtonChecked(IDC_DEBUG_SYMBOLENGINE_OPENPPL));
	preferences.SetValue(k_prefs_debug_parser, IsDlgButtonChecked(IDC_DEBUG_PARSER));
	preferences.SetValue(k_prefs_debug_blindlocking, IsDlgButtonChecked(IDC_DEBUG_BLINDLOCKING));
	preferences.SetValue(k_prefs_debug_memorysymbols, IsDlgButtonChecked(IDC_DEBUG_MEMORY_SYMBOLS));
	preferences.SetValue(k_prefs_debug_tablemap_loader, IsDlgButtonChecked(IDC_DEBUG_TABLEMAP_LOADER));
	preferences.SetValue(k_prefs_debug_filesystem_monitor, IsDlgButtonChecked(IDC_DEBUG_FILESYSTEM_MONITOR));
	preferences.SetValue(k_prefs_debug_table_positioner, IsDlgButtonChecked(IDC_DEBUG_TABLE_POSITIONER));
	preferences.SetValue(k_prefs_debug_istournament, IsDlgButtonChecked(IDC_DEBUG_ISTOURNAMENT));
	preferences.SetValue(k_prefs_debug_gui, IsDlgButtonChecked(IDC_DEBUG_GUI));
	preferences.SetValue(k_prefs_debug_table_limits, IsDlgButtonChecked(IDC_DEBUG_TABLE_LIMITS));
	preferences.SetValue(k_prefs_debug_lazy_scraper, IsDlgButtonChecked(IDC_DEBUG_LAZY_SCRAPER));
	preferences.SetValue(k_prefs_debug_betsize_adjustment, IsDlgButtonChecked(IDC_DEBUG_BETSIZE_ADJUSTMENT));
	preferences.SetValue(k_prefs_debug_handreset_detector, IsDlgButtonChecked(IDC_DEBUG_HANDRESET_DETECTOR));
	preferences.SetValue(k_prefs_debug_engine_container, IsDlgButtonChecked(IDC_DEBUG_ENGINE_CONTAINER));
	preferences.SetValue(k_prefs_debug_handhistory, IsDlgButtonChecked(IDC_DEBUG_HANDHISTORY));
	preferences.SetValue(k_prefs_debug_preferences, IsDlgButtonChecked(IDC_DEBUG_PREFERENCES));
	preferences.SetValue(k_prefs_debug_dll_extension, IsDlgButtonChecked(IDC_DEBUG_DLL_EXTENSION));
	preferences.SetValue(k_prefs_debug_scraper_preprocessor, IsDlgButtonChecked(IDC_DEBUG_SCRAPER_PREPROCESSOR));
	preferences.SetValue(k_prefs_debug_hopper_messages, IsDlgButtonChecked(IDC_DEBUG_HOPPER_MESSAGES));
	preferences.SetValue(k_prefs_debug_openholdem, IsDlgButtonChecked(IDC_DEBUG_OPENHOLDEM));
	preferences.SetValue(k_prefs_debug_popup_blocker, IsDlgButtonChecked(IDC_DEBUG_POPUP_BLOCKER));
	preferences.SetValue(k_prefs_debug_alltherest, IsDlgButtonChecked(IDC_DEBUG_ALLTHEREST));
	CSAPrefsSubDlg::OnOK();
}


