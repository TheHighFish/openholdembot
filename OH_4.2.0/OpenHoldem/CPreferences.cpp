#include "stdafx.h"
#include "CPreferences.h"

#include "debug.h"
#include "MagicNumbers.h"

// CPreferences needs to be globally created, in order to provide saved settings to CMainFrame::PreCreateWindow method
CPreferences		prefs;

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
	// Main window location
	_main_x = _main_y = 0;
	_main_dx = 640;
	_main_dy = 400;

	// Formula window location
	_formula_x = _formula_y = 0;
	_formula_dx = 640;
	_formula_dy = 400;

	// Scraper window location
	_scraper_x = _scraper_y = 0;
	_scraper_dx = 340;
	_scraper_dy = 250;

	// autoplayer
	_frame_delay = 2;
	_click_delay  = 250;
	_swag_delay_1 = 400;
	_swag_delay_2 = 400;
	_swag_delay_3 = 700;
	_engage_autoplayer = true;
	_swag_use_comma = false;

	// scraper
	_scrape_delay = 750;

	// dll
	// Default is: empty string
	// Setting the string to "user.dll" whenever it is empty
	// caused problems for the people who want it undefined:
	// http://www.maxinmontreal.com/forums/viewtopic.php?f=112&t=11556
	_dll_name = "";

	// scraper zoom level
	_scraper_zoom = 2;  // 4x

	// Poker Tracker
	_pt_ip_addr = "127.0.0.1";
	_pt_port = "5432";
	_pt_user = "pokertracker";
	_pt_pass = "";
	_pt_dbname = "";

	// ICM
	_icm_prize1 = 0.5;
	_icm_prize2 = 0.3;
	_icm_prize3 = 0.2;
	_icm_prize4 = 0.0;
	_icm_prize5 = 0.0;

	// Replay frames
	_replay_record = false;
	_replay_record_every_change = false;
	_replay_max_frames = 100;
	_replay_record_every_change_playing = false;

	//  PokerChat
	//  Just a security measure against crazy bot formulas...
	_chat_min_delay = 600;	  //  seconds
	_chat_random_delay = 3000;  //  seconds;

	//  Perl
	_perl_default_formula = "";
	_perl_editor = "C:\\Windows\\notepad.exe";
	TCHAR windows_path[MAX_PATH];
	if (S_OK == SHGetFolderPath(NULL, CSIDL_WINDOWS, NULL, 0, windows_path)) 
	{
		_perl_editor = windows_path;
		_perl_editor += _T("\\notepad.exe");
	}
	else
	{
		_perl_editor = _T("C:\\Windows\\notepad.exe");
	}

	// log$ loggin
	_log_symbol_enabled = false;
	_log_symbol_max_log = 5;

	_trace_enabled = true;
	_basic_logging_enabled = true;
	_error_logging_enabled = true;
	_dll_logging_enabled = true;

	// Logging and debugging
	_disable_msgbox = false;
	_log_max_logsize = 10; // MB

	// Debugging
	_debug_autoconnector = false;
	_debug_autoplayer = false;
	_debug_heartbeat = false;
	_debug_prwin = false;
	_debug_icm = false;
	_debug_occlusionchecker = false;
	_debug_pokertracker = false;
	_debug_rebuy = false;
	_debug_replayframes = false;
	_debug_scraper = false;
	_debug_sessioncounter = false;
	_debug_stableframescounter = false;
	_debug_symbolengine = false;
	_debug_blindlocking = false;
	_debug_memorysymbols = false;
	_debug_tablemap_loader = false;
	_debug_filesystem_monitor = false;
	_debug_table_positioner = false;
	_debug_istournament = false;
	_debug_gui = false;
	_debug_table_limits = false;
	_debug_lazy_scraper = false;
	_debug_betsize_adjustment = false;
	_debug_handreset_detector = false;
	_debug_engine_container = false;
	_debug_handhistory = false;
	_debug_alltherest = false;

	// Validator
	//   0 = disabled
	//   1 = when it's my turn
	//   2 = always
	_validator_enabled = 1;
	_validator_use_heuristic_rules = false;
	_validator_stop_on_error = false;
	_validator_shoot_replayframe_on_error = false;

	// Auto-connector
	_autoconnector_when_to_connect = k_AutoConnector_Connect_Permanent;
	_autoconnector_close_when_table_disappears = false;

	// GUI
	_gui_start_minimized = false; 
	_gui_disable_progress_dialog = false;

	// Rebuy
	_rebuy_condition_no_cards = true;
	_rebuy_condition_change_in_handnumber = true;
	_rebuy_condition_heuristic_check_for_occlusion = true;
	_rebuy_minimum_time_to_next_try = 30;
	_rebuy_script = "Rebuy.exe";

	// Configuration check
	_configurationcheck_input_settings = true;
	_configurationcheck_theme_settings = true;
	_configurationcheck_font_settings = true;

	// Lazy scraping
	_lazy_scraping_when_to_scrape = k_lazy_scraping_always;

	// Handhistory generator
	_handhistory_generator_enable = false;

	// Table positioner
	_table_positioner_options = k_position_tables_tiled;

	// Obscure
	_window_class_name = "OpenHoldem";
	_mutex_name= "OHAntiColl";
	_simple_window_title = false;

	// CFileDialog saved paths
	_path_ohf = "";
	_path_tm = "";
	_path_perl = "";
	_path_dll = "";
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

void CPreferences::ReadReg(const LPCTSTR registry_key, unsigned int *registry_value)
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
//  This method is used to write unsigned ints and booleans, too.
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
	unsigned int PositionOfLastDelimiter = ExecutableFileName.ReverseFind('\\');
	// Truncate path. Keep last "\" to get versus_path.
	return(ExecutableFileName.Left(PositionOfLastDelimiter + 1));
}
