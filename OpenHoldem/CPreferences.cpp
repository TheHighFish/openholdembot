#include "stdafx.h"
#include "CPreferences.h"

#include "debug.h"
#include "MagicNumbers.h"

// CPreferences needs to be globally created, in order to provide saved settings to CMainFrame::PreCreateWindow method
CPreferences		preferences;

//
// Constructor and destructor
//
CPreferences::CPreferences()
{
}

CPreferences::~CPreferences()	
{
}

void CPreferences::LoadPreferences()
{
	_preferences_heading = "Preferences";
	ReadPreferences();
}

//
// Private method to initialize the default values
//
void CPreferences::InitDefaults(void)
{
	// General initialization
	// that fits most values and purposes
	for (int i=0; i<k_prefs_last_bool_value; i++)
	{
		prefs_bool_values[i] = false;
	}
	for (int i=0; i<k_prefs_last_int_value; i++)
	{
		prefs_int_values[i] = 0;	
	}
	for (int i=0; i<k_prefs_last_CString_value; i++)
	{
		prefs_CString_values[i] = "";
	}
	for (int i=0; i<k_prefs_last_double_value; i++)
	{
		prefs_double_values[i] = 0.0;
	}

	// Special initialization of data that requires special values
	prefs_bool_values[k_prefs_engage_autoplayer] = true;
	prefs_bool_values[k_prefs_trace_enabled] = true;
	prefs_bool_values[k_prefs_basic_logging_enabled] = true;
	prefs_bool_values[k_prefs_error_logging_enabled] = true;
	prefs_bool_values[k_prefs_dll_logging_enabled] = true;
	prefs_bool_values[k_prefs_debug_preferences] = false; // To assure that we log at least the initialization of this value
	prefs_bool_values[k_prefs_rebuy_condition_no_cards] = true;
	prefs_bool_values[k_prefs_rebuy_condition_change_in_handnumber] = true;
	prefs_bool_values[k_prefs_rebuy_condition_heuristic_check_for_occlusion] = true;
	prefs_bool_values[k_prefs_configurationcheck_input_settings] = true;
	prefs_bool_values[k_prefs_configurationcheck_theme_settings] = true;
	prefs_bool_values[k_prefs_configurationcheck_font_settings] = true;

	prefs_int_values[k_prefs_main_dx] = 640;
	prefs_int_values[k_prefs_main_dy] = 400;
	prefs_int_values[k_prefs_formula_dx] = 640;
	prefs_int_values[k_prefs_formula_dy] = 400;
	prefs_int_values[k_prefs_scraper_dx] = 340;
	prefs_int_values[k_prefs_scraper_dy] = 250;
	prefs_int_values[k_prefs_frame_delay] = 2;
	prefs_int_values[k_prefs_click_delay] = 250;
	prefs_int_values[k_prefs_swag_delay_1] = 400;
	prefs_int_values[k_prefs_swag_delay_2] = 400;
	prefs_int_values[k_prefs_swag_delay_3] = 700;
	prefs_int_values[k_prefs_scrape_delay] = 750;
	prefs_int_values[k_prefs_scraper_zoom] = 2;			// 4x
	prefs_int_values[k_prefs_replay_max_frames] = 100;
	prefs_int_values[k_prefs_chat_min_delay] = 600;		//  seconds
	prefs_int_values[k_prefs_chat_random_delay] = 3000;	//  seconds;
	prefs_int_values[k_prefs_log_symbol_max_log] = 5;
	prefs_int_values[k_prefs_log_max_logsize] = 10;		// MB
	prefs_int_values[k_prefs_validator_enabled] = 1;		// 0 = disabled; 1 = when it's my turn; 2 = always
	prefs_int_values[k_prefs_autoconnector_when_to_connect] = k_AutoConnector_Connect_Permanent;
	prefs_int_values[k_prefs_rebuy_minimum_time_to_next_try] = 30;
	prefs_int_values[k_prefs_lazy_scraping_when_to_scrape] = k_lazy_scraping_always;
	prefs_int_values[k_prefs_table_positioner_options] = k_position_tables_tiled;
	
	prefs_CString_values[k_prefs_pt_ip_addr] = "127.0.0.1";
	prefs_CString_values[k_prefs_pt_port] = "5432";
	prefs_CString_values[k_prefs_pt_user] = "pokertracker";
	prefs_CString_values[k_prefs_rebuy_script] = "Rebuy.exe";
	prefs_CString_values[k_prefs_window_class_name] = "OpenHoldem";
	prefs_CString_values[k_prefs_mutex_name] = "OHAntiColl";
	
	//  Perl
	TCHAR windows_path[MAX_PATH];
	if (S_OK == SHGetFolderPath(NULL, CSIDL_WINDOWS, NULL, 0, windows_path)) 
	{
		prefs_CString_values[k_prefs_perl_editor] = windows_path;
		prefs_CString_values[k_prefs_perl_editor] += _T("\\notepad.exe");
	}
	else
	{
		prefs_CString_values[k_prefs_perl_editor] = _T("C:\\Windows\\notepad.exe");
	}
		
	prefs_double_values[k_prefs_icm_prize1] = 0.5;
	prefs_double_values[k_prefs_icm_prize2] = 0.3;
	prefs_double_values[k_prefs_icm_prize3] = 0.2;
}

//
// Private methods to access the registry
//

void CPreferences::ReadPreferences()
{
	InitDefaults();
	{
		// Main window location and size
		ReadReg("main_x", &_main_x);
		ReadReg("main_y", &_main_y);
		ReadReg("main_dx", &_main_dx);
		ReadReg("main_dy", &_main_dy);

		// Formula window location and size
		ReadReg("formula_x", &_formula_x);
		ReadReg("formula_y", &_formula_y);
		ReadReg("formula_dx", &_formula_dx);
		ReadReg("formula_dy", &_formula_dy);

		// scraper window location and size
		ReadReg("scraper_x", &_scraper_x);
		ReadReg("scraper_y", &_scraper_y);
		ReadReg("scraper_dx", &_scraper_dx);
		ReadReg("scraper_dy", &_scraper_dy);

		// prefs - autoplayer
		ReadReg("frame_delay", &_frame_delay);
		ReadReg("click_delay", &_click_delay);
		ReadReg("swag_delay_1", &_swag_delay_1);
		ReadReg("swag_delay_2", &_swag_delay_2);
		//  If a key "swag_delay" exists, use this as "swag_delay_3",
		//	if not defined in another way (checked below).
		//	(WH backward compatibility.)
		ReadReg("swag_delay", &_swag_delay_3);
		ReadReg("swag_delay_3", &_swag_delay_3);
		ReadReg("auto", &_engage_autoplayer);
		ReadReg("swag_use_comma", &_swag_use_comma);

		// prefs - dll extension
		ReadReg("dll_name", &_dll_name);

		// prefs - scraper
		ReadReg("scrape_delay", &_scrape_delay);

		// Prefs - poker tracker
		ReadReg("pt_ip_addr", &_pt_ip_addr);
		ReadReg("pt_port", &_pt_port);
		ReadReg("pt_user", &_pt_user);
		ReadReg("pt_pass", &_pt_pass);
		ReadReg("pt_dbname", &_pt_dbname);

		// prefs - ICM
		ReadReg("icm_prize1", &_icm_prize1);
		ReadReg("icm_prize2", &_icm_prize2);
		ReadReg("icm_prize3", &_icm_prize3);
		ReadReg("icm_prize4", &_icm_prize4);
		ReadReg("icm_prize5", &_icm_prize5);

		// Prefs - Replay frames
		ReadReg("replay_record", &_replay_record);
		ReadReg("replay_record_every_change", &_replay_record_every_change);
		ReadReg("replay_max_frames", &_replay_max_frames);
		ReadReg("replay_record_every_change_playing", &_replay_record_every_change_playing);

		// scraper zoom level
		ReadReg("scraper_zoom", &_scraper_zoom);

		// Perl
		ReadReg("perl_editor", &_perl_editor);
		ReadReg("perl_default_formula", &_perl_default_formula);

		// PokerChat
		ReadReg("chat_min_delay", &_chat_min_delay);
		ReadReg("chat_random_delay", &_chat_random_delay);

		// log$ logging
		ReadReg("log_symbol_enabled", &_log_symbol_enabled);
		ReadReg("log_symbol_max_log", &_log_symbol_max_log);

		ReadReg("trace_enabled", &_trace_enabled);
		ReadReg("basic_logging_enabled", &_basic_logging_enabled);
		ReadReg("error_logging_enabled", &_error_logging_enabled);
		ReadReg("dll_logging_enabled", &_dll_logging_enabled);

		// Logging and debugging
		ReadReg("disable_msgbox", &_disable_msgbox);
		ReadReg("log_max_logsize", &_log_max_logsize);

		// Debugging
		ReadReg("debug_autoconnector", &_debug_autoconnector);
		ReadReg("debug_autoplayer", &_debug_autoplayer);
		ReadReg("debug_heartbeat", &_debug_heartbeat);
		ReadReg("debug_prwin", &_debug_prwin);
		ReadReg("debug_icm", &_debug_icm);
		ReadReg("debug_occlusionchecker", &_debug_occlusionchecker);
		ReadReg("debug_pokertracker", &_debug_pokertracker);
		ReadReg("debug_rebuy", &_debug_rebuy);
		ReadReg("debug_replayframes", &_debug_replayframes);
		ReadReg("debug_scraper", &_debug_scraper);
		ReadReg("debug_sessioncounter", &_debug_sessioncounter);
		ReadReg("debug_stableframescounter", &_debug_stableframescounter);
		ReadReg("debug_symbolengine", &_debug_symbolengine);
		ReadReg("debug_blindlocking", &_debug_blindlocking);
		ReadReg("debug_memorysymbols", &_debug_memorysymbols);
		ReadReg("debug_tablemap_loader", &_debug_tablemap_loader);
		ReadReg("debug_filesystem_monitor", &_debug_filesystem_monitor);
		ReadReg("debug_table_positioner", &_debug_table_positioner);
		ReadReg("debug_istournament", &_debug_istournament);
		ReadReg("debug_gui", &_debug_gui);
		ReadReg("debug_table_limits", &_debug_table_limits);
		ReadReg("debug_lazy_scraper", &_debug_lazy_scraper);
		ReadReg("debug_betsize_adjustment", &_debug_betsize_adjustment);
		ReadReg("debug_handreset_detector", &_debug_handreset_detector);
		ReadReg("debug_engine_container", &_debug_engine_container);
		ReadReg("debug_handhistory", &_debug_handhistory);
		ReadReg("debug_alltherest", &_debug_alltherest);
		ReadReg("debug_preferences", &_debug_preferences);

		// Validator
		ReadReg("validator_enabled", &_validator_enabled);
		ReadReg("validator_use_heuristic_rules", &_validator_use_heuristic_rules);
		ReadReg("validator_stop_on_error", &_validator_stop_on_error);
		ReadReg("validator_shoot_replayframe_on_error", &_validator_shoot_replayframe_on_error);

		// Auto-connector
		ReadReg("autoconnector_when_to_connect", & _autoconnector_when_to_connect);
		ReadReg("autoconnector_close_when_table_disappears", &_autoconnector_close_when_table_disappears);

		// GUI
		ReadReg("gui_start_minimized", &_gui_start_minimized);
		ReadReg("gui_disable_progress_dialog", &_gui_disable_progress_dialog);

		// Rebuy
		ReadReg("rebuy_condition_no_cards", &_rebuy_condition_no_cards);
		ReadReg("rebuy_condition_change_in_handnumber", &_rebuy_condition_change_in_handnumber);
		ReadReg("rebuy_condition_heuristic_check_for_occlusion", &_rebuy_condition_heuristic_check_for_occlusion);
		ReadReg("rebuy_minimum_time_to_next_try", &_rebuy_minimum_time_to_next_try);
		ReadReg("rebuy_script", &_rebuy_script);

		// Configuration check
		ReadReg("configurationcheck_input_settings", &_configurationcheck_input_settings);	
		ReadReg("configurationcheck_theme_settings", &_configurationcheck_theme_settings);
		ReadReg("configurationcheck_font_settings", &_configurationcheck_font_settings);

		// Lazy scraping
		ReadReg("lazy_scraping_when_to_scrape", &_lazy_scraping_when_to_scrape);

		// Handhistory_generator
		ReadReg("handhistory_generator_enable", &_handhistory_generator_enable);

		// Table positioner
		ReadReg("table_positioner_options", &_table_positioner_options);

		// obscure
		ReadReg("window_class_name", &_window_class_name);
		ReadReg("mutex_name", &_mutex_name);
		ReadReg("simple_window_title", &_simple_window_title);

		// CFileDialog saved paths
		ReadReg("last_path_ohf", &_path_ohf);
		ReadReg("last_path_tm", &_path_tm);
		ReadReg("last_path_perl", &_path_perl);
		ReadReg("last_path_dll", &_path_dll);
	}
}

void CPreferences::ReadReg(const LPCTSTR registry_key, int *registry_value)
{
	CString value;
	value = AfxGetApp()->GetProfileString(_preferences_heading, registry_key);
	if (!value.IsEmpty())
		*registry_value = atoi(value);
}

void CPreferences::ReadReg(const LPCTSTR registry_key, bool *registry_value)
{
	CString value;
	value = AfxGetApp()->GetProfileString(_preferences_heading, registry_key);
	if (!value.IsEmpty())
		*registry_value = atoi(value);
}

void CPreferences::ReadReg(const LPCTSTR registry_key,  CString *registry_value)
{
	CString value;
	value = AfxGetApp()->GetProfileString(_preferences_heading, registry_key);
	if (!value.IsEmpty())
		*registry_value = value;
}

void CPreferences::ReadReg(const LPCTSTR registry_key, double *registry_value)
{
	CString value;
	value = AfxGetApp()->GetProfileString(_preferences_heading, registry_key);
	if (!value.IsEmpty())
		*registry_value = atof(value);
}

void CPreferences::WriteReg(const LPCTSTR registry_key, const int registry_value)
//  This method is used to write ints and booleans, too.
{
	CString str;
	str.Format("%d", registry_value);
	AfxGetApp()->WriteProfileString(_preferences_heading, registry_key, str);
}

void CPreferences::WriteReg(const LPCTSTR registry_key, const CString &registry_value)
{
	AfxGetApp()->WriteProfileString(_preferences_heading, registry_key, registry_value);
}

void CPreferences::WriteReg(const LPCTSTR registry_key, const double registry_value)
{
	CString str;
	str.Format("%f", registry_value);
	AfxGetApp()->WriteProfileString(_preferences_heading, registry_key, str);
}

const CString CPreferences::versus_path()
{
	// Take the OpenHoldem-installation-directory as versus_path.
	TCHAR Buffer[MAX_PATH];
	GetModuleFileName(NULL, Buffer, MAX_PATH);
	CString ExecutableFileName = Buffer;
	int PositionOfLastDelimiter = ExecutableFileName.ReverseFind('\\');
	// Truncate path. Keep last "\" to get versus_path.
	return(ExecutableFileName.Left(PositionOfLastDelimiter + 1));
}
