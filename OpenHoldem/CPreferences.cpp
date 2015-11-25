//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose:
//
//*******************************************************************************

#include "stdafx.h"
#include "CPreferences.h"

#include "debug.h"
#include "MagicNumbers.h"
#include "OH_MessageBox.h"

// CPreferences needs to be globally created, in order to provide saved settings to CMainFrame::PreCreateWindow method
CPreferences		preferences;

//
// Constructor and destructor
//
CPreferences::CPreferences() {
}

CPreferences::~CPreferences() {
}

// registry keys 
const char* k_registry_keys_for_numerical_values[k_prefs_last_numerical_value + 1] = {
	// bools
	"debug_preferences",	
	"debug_autoconnector",
	"debug_autoplayer",
	"debug_heartbeat",
	"debug_prwin",
	"debug_icm",
	"debug_occlusionchecker",
	"debug_pokertracker",
	"debug_rebuy",
	"debug_replayframes",
	"debug_scraper",
	"debug_sessioncounter",
	"debug_stableframescounter",
	"debug_symbolengine",
	"debug_symbolengine_open_ppl",
  "debug_memory_usage",
	"debug_parser",
	"debug_blindlocking",
	"debug_memorysymbols",
	"debug_tablemap_loader",
	"debug_filesystem_monitor",
	"debug_handhistory",
	"debug_hopper_messages",
	"debug_alltherest",
	"debug_table_positioner",
	"debug_istournament",
	"debug_gui",
	"debug_table_limits",
	"debug_lazy_scraper",
	"debug_betsize_adjustment",
  "debug_betsize_adjustment",
	"debug_handreset_detector",
	"debug_engine_container",
	"debug_dll_extension",
	"debug_scraper_preprocessor",
	"debug_openholdem",
	"debug_popup_blocker",
  "debug_formula",
  "debug_versus",
  "debug_auto_trace",
  "debug_ast_priority_ordering",
  "debug_filenames",
  "debug_hand_and_baord_expressions",
  "debug_tokenizer",
	"engage_autoplayer",
	"restore_position_and_focus",
	"use_auto_replay",
	"betsize_use_comma",
	"replay_record",
	"log_basic_info",
	"trace_enabled",
	"basic_logging_enabled",
	"error_logging_enabled",
	"dll_logging_enabled",
	"disable_msgbox",
	"validator_stop_on_error",
	"validator_use_heuristic_rules",
	"validator_shoot_replayframe_on_error",
  "validator_enabled",
	"autoconnector_close_when_table_disappears",
	"gui_start_minimized", 
	"rebuy_condition_no_cards",
	"rebuy_condition_change_in_handnumber",
	"rebuy_condition_heuristic_check_for_occlusion",
	"configurationcheck_input_settings",
	"configurationcheck_theme_settings",
	"configurationcheck_font_settings",
	"handhistory_generator_enable",
	"simple_window_title",
	"table_positioner_always_keep_position",
	// ints
	"replay_max_frames",
	"frame_delay",
	"click_delay",
	"betsize_delay_1",
	"betsize_delay_2",
	"betsize_delay_3",
	"scrape_delay",
	"chat_min_delay",
	"chat_random_delay",
	"log_max_logsize",
	"formula_x",	
	"formula_y",
	"formula_dx",
	"formula_dy",
	"scraper_x",
	"scraper_y",
	"scraper_dx",
	"scraper_dy",
	"main_x",
	"main_y",
	"autoconnector_when_to_connect",
	"rebuy_minimum_time_to_next_try",
	"lazy_scraping_when_to_scrape",
	"table_positioner_options",
	"scraper_zoom",
	"popup_blocker",
	// doubles
  // Only on double left, just to check for outdated ICM-config
	"icm_prize1",
	//
	// Finally an empty string
	// This correspondents to k_prefs_last_numerical_value (unused)
	// It will cause an assertion if used
	// and therefore help us to detect missing values.
	"",
};

const char* k_registry_keys_for_CStrings[k_prefs_last_CString_value + 1] =
{
	"dll_name",
	"pt_ip_addr",
	"pt_port",
	"pt_dbname",
	"pt_user",
	"pt_pass",
	"rebuy_script",
	"window_class_name",
	"mutex_name",
	"path_ohf",
	"path_tm",
	"path_dll",
	// Finally an empty string
	// This correspondents to k_prefs_last_numerical_value (unused)
	// It will cause an assertion if used
	// and therefore help us to detect missing values.
	"",
};

void CPreferences::LoadPreferences()
{
	_preferences_heading = "Preferences";
	InitDefaults();
	ReadPreferences();
  CheckForOutdatedICMConfig();
}


void CPreferences::CheckForOutdatedICMConfig() {
  if (prefs_numerical_values[k_prefs_icm_prize1] != kUndefinedZero) {
    OH_MessageBox_Error_Warning(
      "Outdated ICM-configuration\n"
      "\n"
      "OpenHoldem does no longer use the ini-file for that\n"
      "but f$icm_prize1 .. f$icm_prize9 functions\n"
      "\n"
      "Now you can adapt the payout-structure at runtime\n"
      "depending on casino, game-type, number of players...");
  }
}

//
// Private method to initialize the default values
//
void CPreferences::InitDefaults(void)
{
	// General initialization
	// that fits most values and purposes
	for (int i=0; i<k_prefs_last_numerical_value; i++)
	{
		prefs_numerical_values[i] = 0.0;	// 0 for ints and false for bools
	}
	for (int i=0; i<k_prefs_last_CString_value; i++)
	{
		prefs_CString_values[i] = "";
	}

	// Initialization of data that requires special values
	// Bools
	prefs_numerical_values[k_prefs_engage_autoplayer] = true;
	prefs_numerical_values[k_prefs_restore_position_and_focus] = false;
	prefs_numerical_values[k_prefs_use_auto_replay] = true;
	prefs_numerical_values[k_prefs_trace_enabled] = true;
	prefs_numerical_values[k_prefs_basic_logging_enabled] = true;
	prefs_numerical_values[k_prefs_error_logging_enabled] = true;
	prefs_numerical_values[k_prefs_dll_logging_enabled] = true;
	prefs_numerical_values[k_prefs_debug_preferences] = true; // To assure that we log at least the initialization of this value
	prefs_numerical_values[k_prefs_rebuy_condition_no_cards] = true;
	prefs_numerical_values[k_prefs_rebuy_condition_change_in_handnumber] = true;
	prefs_numerical_values[k_prefs_rebuy_condition_heuristic_check_for_occlusion] = true;
	prefs_numerical_values[k_prefs_configurationcheck_input_settings] = true;
	prefs_numerical_values[k_prefs_configurationcheck_theme_settings] = true;
	prefs_numerical_values[k_prefs_configurationcheck_font_settings] = true;

	// Ints
	prefs_numerical_values[k_prefs_formula_dx] = 640;
	prefs_numerical_values[k_prefs_formula_dy] = 400;
	prefs_numerical_values[k_prefs_scraper_dx] = 340;
	prefs_numerical_values[k_prefs_scraper_dy] = 250;
	prefs_numerical_values[k_prefs_frame_delay] = 2;
	prefs_numerical_values[k_prefs_click_delay] = 250;
	prefs_numerical_values[k_prefs_swag_delay_1] = 400;
	prefs_numerical_values[k_prefs_swag_delay_2] = 400;
	prefs_numerical_values[k_prefs_swag_delay_3] = 700;
	prefs_numerical_values[k_prefs_scrape_delay] = 750;
	prefs_numerical_values[k_prefs_scraper_zoom] = 2;			// 4x
	prefs_numerical_values[k_prefs_replay_max_frames] = 100;
	prefs_numerical_values[k_prefs_chat_min_delay] = 600;		//  seconds
	prefs_numerical_values[k_prefs_chat_random_delay] = 3000;	//  seconds;
	prefs_numerical_values[k_prefs_log_max_logsize] = 10;		// MB		
	prefs_numerical_values[k_prefs_autoconnector_when_to_connect] = k_AutoConnector_Connect_Permanent;
	prefs_numerical_values[k_prefs_rebuy_minimum_time_to_next_try] = 30;
	prefs_numerical_values[k_prefs_lazy_scraping_when_to_scrape] = k_lazy_scraping_always;
	prefs_numerical_values[k_prefs_table_positioner_options] = k_position_tables_tiled;

	// Doubles
	prefs_numerical_values[k_prefs_icm_prize1] = kUndefinedZero;
	
	// CString
	prefs_CString_values[k_prefs_pt_ip_addr] = "127.0.0.1";
	prefs_CString_values[k_prefs_pt_port] = "5432";
	prefs_CString_values[k_prefs_pt_user] = "pokertracker";
	prefs_CString_values[k_prefs_rebuy_script] = "Rebuy.exe";
	prefs_CString_values[k_prefs_window_class_name] = "OpenHoldem";
	prefs_CString_values[k_prefs_mutex_name] = "OHAntiColl";
}

//
// Private methods to access the registry
//

void CPreferences::ReadPreferences()
{
	// Then read the registry values and overwrite the defaults if defined
	for (int i=0; i<k_prefs_last_numerical_value; i++)
	{
		assert(k_registry_keys_for_numerical_values[i] != "");
		ReadReg(k_registry_keys_for_numerical_values[i], &prefs_numerical_values[i]); 
	}
	for (int i=0; i<k_prefs_last_CString_value; i++)
	{
		assert(k_registry_keys_for_CStrings[i] != "");
		ReadReg(k_registry_keys_for_CStrings[i], &prefs_CString_values[i]);
	}
}

void CPreferences::ReadReg(const LPCTSTR registry_key, CString *registry_value)
{
	CString value;
	value = AfxGetApp()->GetProfileString(_preferences_heading, registry_key);
	if (!value.IsEmpty())
		*registry_value = value;
	write_log(debug_preferences(), "[CPreferences] %s = %s\n",
		registry_key, registry_value);
}

void CPreferences::ReadReg(const LPCTSTR registry_key, double *registry_value)
{
	CString value;
	value = AfxGetApp()->GetProfileString(_preferences_heading, registry_key);
	if (!value.IsEmpty())
		*registry_value = atof(value);
	write_log(debug_preferences(), "[CPreferences] %s = %s\n",
		registry_key, Number2CString(*registry_value));
}

void CPreferences::WriteReg(const LPCTSTR registry_key, const CString &registry_value)
{
	AfxGetApp()->WriteProfileString(_preferences_heading, registry_key, registry_value);
}

void CPreferences::WriteReg(const LPCTSTR registry_key, const double registry_value)
{
	CString str;
	str.Format("%.2f", registry_value);
	AfxGetApp()->WriteProfileString(_preferences_heading, registry_key, str);
}

#define ENT CSLock lock(m_critsec);

void CPreferences::SetValue(int index_of_variable, CString value)
{
	ENT 
	AssertRange(index_of_variable, 0, k_prefs_last_CString_value);
	prefs_CString_values[index_of_variable] = value;
	WriteReg(k_registry_keys_for_CStrings[index_of_variable], value);
	write_log(debug_preferences(), "[CPreferences] %s = %s\n",
		k_registry_keys_for_CStrings[index_of_variable], value);
}

void CPreferences::SetValue(int index_of_variable, double value)
{
	ENT 
	AssertRange(index_of_variable, 0, k_prefs_last_numerical_value);
	prefs_numerical_values[index_of_variable] = value;
	if (IsInteger(value))
	{
		WriteReg(k_registry_keys_for_numerical_values[index_of_variable], int(value));
	}
	else
	{
		WriteReg(k_registry_keys_for_numerical_values[index_of_variable], value);
	}
	write_log(debug_preferences(), "[CPreferences] %s = %s\n",
		k_registry_keys_for_numerical_values[index_of_variable], Number2CString(value));
}