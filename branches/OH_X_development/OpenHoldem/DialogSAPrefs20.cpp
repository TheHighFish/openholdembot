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

// DialogSAPrefs20.cpp : implementation file
//

#include "stdafx.h"
#include "DialogSAPrefs20.h"

#include "CPreferences.h"
#include "MainFrm.h"
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
	CheckDlgButton(IDC_DEBUG_AUTOCONNECTOR , MAIN->p_preferences()->debug_autoconnector() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_AUTOPLAYER, MAIN->p_preferences()->debug_autoplayer() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_HEARTBEAT, MAIN->p_preferences()->debug_heartbeat() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_PRWIN, MAIN->p_preferences()->debug_prwin() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_ICM, MAIN->p_preferences()->debug_icm() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_OCCLUSSION, MAIN->p_preferences()->debug_occlusionchecker() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_POKERTRACKER, MAIN->p_preferences()->debug_pokertracker() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_REBUY, MAIN->p_preferences()->debug_rebuy() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_REPLAYFRAMES, MAIN->p_preferences()->debug_replayframes() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_SCRAPER, MAIN->p_preferences()->debug_scraper() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_SESSIONCOUNTER, MAIN->p_preferences()->debug_sessioncounter() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_STABLEFRAMESCOUNTER, MAIN->p_preferences()->debug_stableframescounter() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_SYMBOLENGINE, MAIN->p_preferences()->debug_symbolengine() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_SYMBOLENGINE_OPENPPL, MAIN->p_preferences()->debug_symbolengine_open_ppl() ? MF_CHECKED : MF_UNCHECKED);
  CheckDlgButton(IDC_DEBUG_MEMORY_USAGE, MAIN->p_preferences()->debug_memory_usage() ? MF_CHECKED : MF_UNCHECKED);
  CheckDlgButton(IDC_DEBUG_PARSER, MAIN->p_preferences()->debug_parser() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_BLINDLOCKING, MAIN->p_preferences()->debug_blindlocking() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_MEMORY_SYMBOLS, MAIN->p_preferences()->debug_memorysymbols() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_TABLEMAP_LOADER, MAIN->p_preferences()->debug_tablemap_loader() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_FILESYSTEM_MONITOR, MAIN->p_preferences()->debug_filesystem_monitor() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_TABLE_POSITIONER, MAIN->p_preferences()->debug_table_positioner() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_ISTOURNAMENT, MAIN->p_preferences()->debug_istournament() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_GUI, MAIN->p_preferences()->debug_gui() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_TABLE_LIMITS, MAIN->p_preferences()->debug_table_limits() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_LAZY_SCRAPER, MAIN->p_preferences()->debug_lazy_scraper() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_BETSIZE_ADJUSTMENT, MAIN->p_preferences()->debug_betsize_adjustment() ? MF_CHECKED : MF_UNCHECKED);
  CheckDlgButton(IDC_DEBUG_ALLIN_ADJUSTMENT, MAIN->p_preferences()->debug_allin_adjustment() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_HANDRESET_DETECTOR, MAIN->p_preferences()->debug_handreset_detector() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_ENGINE_CONTAINER, MAIN->p_preferences()->debug_engine_container() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_HANDHISTORY, MAIN->p_preferences()->debug_handhistory() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_PREFERENCES, MAIN->p_preferences()->debug_preferences() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_DLL_EXTENSION, MAIN->p_preferences()->debug_dll_extension() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_SCRAPER_PREPROCESSOR, MAIN->p_preferences()->debug_scraper_preprocessor() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_HOPPER_MESSAGES, MAIN->p_preferences()->debug_hopper_messages() ? MF_CHECKED : MF_UNCHECKED);
	CheckDlgButton(IDC_DEBUG_OPENHOLDEM, MAIN->p_preferences()->debug_openholdem() ? MF_CHECKED : MF_UNCHECKED);
  CheckDlgButton(IDC_DEBUG_POPUP_BLOCKER, MAIN->p_preferences()->debug_popup_blocker() ? MF_CHECKED : MF_UNCHECKED);
  CheckDlgButton(IDC_DEBUG_FORMULA, MAIN->p_preferences()->debug_formula() ? MF_CHECKED : MF_UNCHECKED);
  CheckDlgButton(IDC_DEBUG_ALLTHEREST, MAIN->p_preferences()->debug_alltherest() ? MF_CHECKED : MF_UNCHECKED);
  CheckDlgButton(IDC_DEBUG_VERSUS, MAIN->p_preferences()->debug_versus() ? MF_CHECKED : MF_UNCHECKED);
  CheckDlgButton(IDC_DEBUG_AUTO_TRACE, MAIN->p_preferences()->debug_auto_trace() ? MF_CHECKED : MF_UNCHECKED);
  CheckDlgButton(IDC_DEBUG_FILENAMES, MAIN->p_preferences()->debug_filenames() ? MF_CHECKED : MF_UNCHECKED);
  CheckDlgButton(IDC_DEBUG_AST_PRIORITY_ORDERING, MAIN->p_preferences()->debug_ast_priority_ordering() ? MF_CHECKED : MF_UNCHECKED);
  CheckDlgButton(IDC_DEBUG_BOARD_EXPRESSIONS, MAIN->p_preferences()->debug_hand_and_baord_expressions() ? MF_CHECKED : MF_UNCHECKED);
  CheckDlgButton(IDC_DEBUG_TOKENIZER, MAIN->p_preferences()->debug_tokenizer() ? MF_CHECKED : MF_UNCHECKED);
  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs20::OnOK()
{
	MAIN->p_preferences()->SetValue(k_prefs_debug_autoconnector, IsDlgButtonChecked(IDC_DEBUG_AUTOCONNECTOR));
	MAIN->p_preferences()->SetValue(k_prefs_debug_autoplayer, IsDlgButtonChecked(IDC_DEBUG_AUTOPLAYER));
	MAIN->p_preferences()->SetValue(k_prefs_debug_heartbeat, IsDlgButtonChecked(IDC_DEBUG_HEARTBEAT));
	MAIN->p_preferences()->SetValue(k_prefs_debug_prwin, IsDlgButtonChecked(IDC_DEBUG_PRWIN));
	MAIN->p_preferences()->SetValue(k_prefs_debug_icm, IsDlgButtonChecked(IDC_DEBUG_ICM));
	MAIN->p_preferences()->SetValue(k_prefs_debug_occlusionchecker, IsDlgButtonChecked(IDC_DEBUG_OCCLUSSION));
	MAIN->p_preferences()->SetValue(k_prefs_debug_pokertracker, IsDlgButtonChecked(IDC_DEBUG_POKERTRACKER));
	MAIN->p_preferences()->SetValue(k_prefs_debug_rebuy, IsDlgButtonChecked(IDC_DEBUG_REBUY));
	MAIN->p_preferences()->SetValue(k_prefs_debug_replayframes, IsDlgButtonChecked(IDC_DEBUG_REPLAYFRAMES));
	MAIN->p_preferences()->SetValue(k_prefs_debug_scraper, IsDlgButtonChecked(IDC_DEBUG_SCRAPER));
	MAIN->p_preferences()->SetValue(k_prefs_debug_sessioncounter, IsDlgButtonChecked(IDC_DEBUG_SESSIONCOUNTER));
	MAIN->p_preferences()->SetValue(k_prefs_debug_stableframescounter, IsDlgButtonChecked(IDC_DEBUG_STABLEFRAMESCOUNTER));
	MAIN->p_preferences()->SetValue(k_prefs_debug_symbolengine, IsDlgButtonChecked(IDC_DEBUG_SYMBOLENGINE));
	MAIN->p_preferences()->SetValue(k_prefs_debug_symbolengine_open_ppl, IsDlgButtonChecked(IDC_DEBUG_SYMBOLENGINE_OPENPPL));
  MAIN->p_preferences()->SetValue(k_prefs_debug_memory_usage, IsDlgButtonChecked(IDC_DEBUG_MEMORY_USAGE));
  MAIN->p_preferences()->SetValue(k_prefs_debug_parser, IsDlgButtonChecked(IDC_DEBUG_PARSER));
	MAIN->p_preferences()->SetValue(k_prefs_debug_blindlocking, IsDlgButtonChecked(IDC_DEBUG_BLINDLOCKING));
	MAIN->p_preferences()->SetValue(k_prefs_debug_memorysymbols, IsDlgButtonChecked(IDC_DEBUG_MEMORY_SYMBOLS));
	MAIN->p_preferences()->SetValue(k_prefs_debug_tablemap_loader, IsDlgButtonChecked(IDC_DEBUG_TABLEMAP_LOADER));
	MAIN->p_preferences()->SetValue(k_prefs_debug_filesystem_monitor, IsDlgButtonChecked(IDC_DEBUG_FILESYSTEM_MONITOR));
	MAIN->p_preferences()->SetValue(k_prefs_debug_table_positioner, IsDlgButtonChecked(IDC_DEBUG_TABLE_POSITIONER));
	MAIN->p_preferences()->SetValue(k_prefs_debug_istournament, IsDlgButtonChecked(IDC_DEBUG_ISTOURNAMENT));
	MAIN->p_preferences()->SetValue(k_prefs_debug_gui, IsDlgButtonChecked(IDC_DEBUG_GUI));
	MAIN->p_preferences()->SetValue(k_prefs_debug_table_limits, IsDlgButtonChecked(IDC_DEBUG_TABLE_LIMITS));
	MAIN->p_preferences()->SetValue(k_prefs_debug_lazy_scraper, IsDlgButtonChecked(IDC_DEBUG_LAZY_SCRAPER));
	MAIN->p_preferences()->SetValue(k_prefs_debug_betsize_adjustment, IsDlgButtonChecked(IDC_DEBUG_BETSIZE_ADJUSTMENT));
  MAIN->p_preferences()->SetValue(k_prefs_debug_allin_adjustment, IsDlgButtonChecked(IDC_DEBUG_ALLIN_ADJUSTMENT));
	MAIN->p_preferences()->SetValue(k_prefs_debug_handreset_detector, IsDlgButtonChecked(IDC_DEBUG_HANDRESET_DETECTOR));
	MAIN->p_preferences()->SetValue(k_prefs_debug_engine_container, IsDlgButtonChecked(IDC_DEBUG_ENGINE_CONTAINER));
	MAIN->p_preferences()->SetValue(k_prefs_debug_handhistory, IsDlgButtonChecked(IDC_DEBUG_HANDHISTORY));
	MAIN->p_preferences()->SetValue(k_prefs_debug_preferences, IsDlgButtonChecked(IDC_DEBUG_PREFERENCES));
	MAIN->p_preferences()->SetValue(k_prefs_debug_dll_extension, IsDlgButtonChecked(IDC_DEBUG_DLL_EXTENSION));
	MAIN->p_preferences()->SetValue(k_prefs_debug_scraper_preprocessor, IsDlgButtonChecked(IDC_DEBUG_SCRAPER_PREPROCESSOR));
	MAIN->p_preferences()->SetValue(k_prefs_debug_hopper_messages, IsDlgButtonChecked(IDC_DEBUG_HOPPER_MESSAGES));
	MAIN->p_preferences()->SetValue(k_prefs_debug_openholdem, IsDlgButtonChecked(IDC_DEBUG_OPENHOLDEM));
	MAIN->p_preferences()->SetValue(k_prefs_debug_popup_blocker, IsDlgButtonChecked(IDC_DEBUG_POPUP_BLOCKER));
  MAIN->p_preferences()->SetValue(k_prefs_debug_formula, IsDlgButtonChecked(IDC_DEBUG_FORMULA));
	MAIN->p_preferences()->SetValue(k_prefs_debug_alltherest, IsDlgButtonChecked(IDC_DEBUG_ALLTHEREST));
  MAIN->p_preferences()->SetValue(k_prefs_debug_versus, IsDlgButtonChecked(IDC_DEBUG_VERSUS));
  MAIN->p_preferences()->SetValue(k_prefs_debug_auto_trace, IsDlgButtonChecked(IDC_DEBUG_AUTO_TRACE));
  MAIN->p_preferences()->SetValue(k_prefs_debug_filenames, IsDlgButtonChecked(IDC_DEBUG_FILENAMES));
  MAIN->p_preferences()->SetValue(k_prefs_debug_ast_priority_ordering, IsDlgButtonChecked(IDC_DEBUG_AST_PRIORITY_ORDERING));
  MAIN->p_preferences()->SetValue(k_prefs_debug_hand_and_baord_expressions, IsDlgButtonChecked(IDC_DEBUG_BOARD_EXPRESSIONS));
  MAIN->p_preferences()->SetValue(k_prefs_debug_tokenizer, IsDlgButtonChecked(IDC_DEBUG_TOKENIZER));
	CSAPrefsSubDlg::OnOK();
}


