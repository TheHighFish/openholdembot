#include "stdafx.h"

#include "debug.h"
#include "CPreferences.h"

// CPreferences needs to be globally created, in order to provide saved settings to CMainFrame::PreCreateWindow method
CPreferences		prefs;

//
// Constructor and destructor
//
CPreferences::CPreferences()
{
	// Create a registry key for OH,
	// if it does not exist
	
	DWORD		dwDisp;

	LONG res = RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\OpenHoldem\\OpenHoldem", 0, NULL,
				   REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &_hkey, &dwDisp);

	ASSERT(res==ERROR_SUCCESS);

	ReadFromRegistry();
}

CPreferences::~CPreferences()	
{
	RegCloseKey(_hkey);
}

//
// Private method to initialize the default values
//
void CPreferences::InitDefaults(void)
{
	// Main window location
	_main_x = _main_y = 0;
	_main_dx = 800;
	_main_dy = 400;

	// Formula window location
	_formula_x = _formula_y = 0;
	_formula_dx = 700;
	_formula_dy = 420;

	// PPro window location
	_ppro_x = _ppro_y = -1;
	_ppro_dx = 555;
	_ppro_dy = 577;

	// Scraper window location
	_scraper_x = _scraper_y = 0;
	_scraper_dx = 340;
	_scraper_dy = 250;

	// Formula editor
	_precision = 4;
	_equal = 12;
	_udf_sort = _udf_group = _expand_std = _expand_list = _expand_udf = false;

	// analyzer
	_max_opponents = 9;

	// symbols
	_sym_handrank_value = "169";
	_sym_disable_caching = false;

	// autoplayer
	_frame_delay = 2;
	_swag_delay_1 = 400;
	_swag_delay_2 = 400;
	_swag_delay_3 = 700;
	_ap_auto = false;
	_focus_detect = false;

	// autoplayer - swag
	_text_selection_method = TEXTSEL_DOUBLECLICK;
	_text_deletion_method = TEXTDEL_DELETE;
	_bet_confirmation_method = BETCONF_CLICKBET;
	_button_click_method = BUTTON_SINGLECLICK;

	// scraper
	_scrape_delay = 750;

	// dll
	_dll_always_send_state = true;
	_dll_load_on_startup = false;
	_dll_name = "whuser.dll";

	// Ppro
	_ppro_hostname = "127.0.0.1";
	_ppro_port = "30000";
	_ppro_username = "openholdem";
	_ppro_password = "";
	_ppro_handhistory = false;
	_ppro_chips = 1000;
	_ppro_autoseat = true;

	// Blind locking
	_sblind = 5;
	_bblind = 10;
	_bbet = 20;
	_ante = 0;
	_gametype = LIMIT_NL;

	// f$debug logging
	_fdebuglog = false;
	_fdebuglog_myturn = true;

	// game state engine
	_sym_av_time = 15; 

	// scraper zoom level
	_scraper_zoom = 2;  // 4x

	// Poker Tracker
	_pt_disable = true;
	_pt_ip_addr = "127.0.0.1";
	_pt_port = "5432";
	_pt_user = "pokertracker";
	_pt_pass = "";
	_pt_dbname = "";
	_pt_update_delay = 30;
	_pt_cache_refresh = 120;

	// ICM
	_icm_prize1 = 0.5;
	_icm_prize2 = 0.3;
	_icm_prize3 = 0.2;
	_icm_prize4 = 0.0;

	// Replay frames
	_replay_record = false;
	_replay_record_every_change = false;
	_replay_max_frames = 100;

	//  Perl
	_perl_default_formula = "";
	//  Unfortunately "%SystemRoot%\\notepad.exe" does not work;
	//	It seems, this kind of filename substitution is only
	//	supported by the DOS shell, not by the Windows API.
	//	Therefore we use a hardcoded filename.
	_perl_editor = "C:\\Windows\\notepad.exe";
	_perl_load_default_formula = false;
	_perl_load_interpreter = false;

	//  PokerChat
	_chat_enabled = false;
	//  Just a security measure against crazy bot formulas...
	_chat_min_delay = 600;	  //  seconds
	_chat_random_delay = 3000;  //  seconds;

	// log$ loggin
	_log_symbol_enabled = false;
	_log_symbol_max_log = 5;

	_trace_enabled = false;
	for (int i=0;i<nTraceFunctions;i++)
		_trace_functions[i] = false;

	_versus_path = "";
}

//
// Private methods to access the registry
//

void CPreferences::ReadFromRegistry()
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

		// ppro window location and size
		ReadReg("ppro_x", &_ppro_x);
		ReadReg("ppro_y", &_ppro_y);
		ReadReg("ppro_dx", &_ppro_dx);
		ReadReg("ppro_dy", &_ppro_dy);

		// scraper window location and size
		ReadReg("scraper_x", &_scraper_x);
		ReadReg("scraper_y", &_scraper_y);
		ReadReg("scraper_dx", &_scraper_dx);
		ReadReg("scraper_dy", &_scraper_dy);

		// formula editor
		ReadReg("precision", &_precision);
		ReadReg("equal", &_equal);
		ReadReg("udf_sort", &_udf_sort);
		ReadReg("udf_group", &_udf_group);		
		ReadReg("expand_std", &_expand_std);
		ReadReg("expand_list", &_expand_list);
		ReadReg("expand_udf", &_expand_udf);

		// prefs - analyzer
		ReadReg("max_opponents", &_max_opponents);

		// prefs - autoplayer
		ReadReg("frame_delay", &_frame_delay);
		ReadReg("swag_delay_1", &_swag_delay_1);
		ReadReg("swag_delay_2", &_swag_delay_2);
		//  If a key "swag_delay" exists, use this as "swag_delay_3",
		//	if not defined in another way (checked below).
		//	(WH backward compatibility.)
		ReadReg("swag_delay", &_swag_delay_3);
		ReadReg("swag_delay_3", &_swag_delay_3);
		ReadReg("swag_text_selection", &_text_selection_method);
		ReadReg("swag_text_deletion", &_text_deletion_method);
		ReadReg("swag_bet_confirmation", &_bet_confirmation_method);
		ReadReg("button_click_method", &_button_click_method);
		ReadReg("auto", &_ap_auto);
		ReadReg("focus_detect", &_focus_detect);

		// prefs - dll extension
		ReadReg("dll_always_send_state", &_dll_always_send_state);
		ReadReg("load_dll_on_startup", &_dll_load_on_startup);
		ReadReg("dll_name", &_dll_name);

		// prefs - scraper
		ReadReg("scrape_delay", &_scrape_delay);

		// prefs - symbols
		ReadReg("avtime", &_sym_av_time);
		ReadReg("handrank_value", &_sym_handrank_value);
		ReadReg("disable_caching", &_sym_disable_caching);

		// Prefs - poker tracker
		ReadReg("pt_disable", &_pt_disable);
		ReadReg("pt_ip_addr", &_pt_ip_addr);
		ReadReg("pt_port", &_pt_port);
		ReadReg("pt_user", &_pt_user);
		ReadReg("pt_pass", &_pt_pass);
		ReadReg("pt_dbname", &_pt_dbname);
		ReadReg("pt_updatedelay", &_pt_update_delay);
		ReadReg("pt_cacherefresh", &_pt_cache_refresh);

		// prefs - ICM
		ReadReg("icm_prize1", &_icm_prize1);
		ReadReg("icm_prize2", &_icm_prize2);
		ReadReg("icm_prize3", &_icm_prize3);
		ReadReg("icm_prize4", &_icm_prize4);

		// Prefs - Replay frames
		ReadReg("replay_record", &_replay_record);
		ReadReg("replay_record_every_change", &_replay_record_every_change);
		ReadReg("replay_max_frames", &_replay_max_frames);

		// poker Pro
		ReadReg("ppro_hostname", &_ppro_hostname);
		ReadReg("ppro_port", &_ppro_port);
		ReadReg("ppro_username", &_ppro_username);
		ReadReg("ppro_password", &_ppro_password);
		ReadReg("ppro_handhistory", &_ppro_handhistory);
		ReadReg("ppro_chips", &_ppro_chips);
		ReadReg("ppro_autoseat", &_ppro_autoseat);

		// blind locking
		ReadReg("sblind", &_sblind);
		ReadReg("bblind", &_bblind);
		ReadReg("bbet", &_bbet);
		ReadReg("ante", &_ante);
		ReadReg("gametype", &_gametype);

		// f$debug logging
		ReadReg("fdebuglog", &_fdebuglog);
		ReadReg("fdebuglog_myturn", &_fdebuglog_myturn);
		// scraper zoom level
		ReadReg("scraper_zoom", &_scraper_zoom);

		// Perl
		ReadReg("perl_editor", &_perl_editor);
		ReadReg("perl_load_default_formula", &_perl_load_default_formula);
		ReadReg("perl_load_interpreter", &_perl_load_interpreter);
		ReadReg("perl_default_formula", &_perl_default_formula);

		// PokerChat
		ReadReg("chat_enabled", &_chat_enabled);
		ReadReg("chat_min_delay", &_chat_min_delay);
		ReadReg("chat_random_delay", &_chat_random_delay);

		// log$ logging
		ReadReg("log_symbol_enabled", &_log_symbol_enabled);
		ReadReg("log_symbol_max_log", &_log_symbol_max_log);

		ReadReg("trace_enabled", &_trace_enabled);

		CString regValue;
		for (int i=0;i<nTraceFunctions;i++) 
		{
			regValue.Format("trace_functions%d", i+1);
			ReadReg(regValue, &_trace_functions[i]);
		}
		
		// versus path
		ReadReg("versus", &_versus_path);
	}
}

void CPreferences::ReadReg(const LPCTSTR registry_key, int *registry_value)
{
	DWORD		dwType = 0, cbData = 0;
	char		str[256] = {0};
	LONG		hkResult = 0;

	cbData = sizeof(str);
	if ( (hkResult = RegQueryValueEx(_hkey, registry_key, NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
		*registry_value = atoi(str);
	//  Otherwise: Keep default value.
}

void CPreferences::ReadReg(const LPCTSTR registry_key, bool *registry_value)
{
	DWORD		dwType = 0, cbData = 0;
	char		str[256] = {0};
	LONG		hkResult = 0;

	cbData = sizeof(str);
	if ( (hkResult = RegQueryValueEx(_hkey, registry_key, NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
		*registry_value = atoi(str);
	//  Otherwise: Keep default value.
}

void CPreferences::ReadReg(const LPCTSTR registry_key, unsigned int *registry_value)
{
	DWORD		dwType = 0, cbData = 0;
	char		str[256] = {0};
	LONG		hkResult = 0;

	cbData = sizeof(str);
	if ( (hkResult = RegQueryValueEx(_hkey, registry_key, NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
		*registry_value = atoi(str);
	//  Otherwise: Keep default value.
}

void CPreferences::ReadReg(const LPCTSTR registry_key,  CString *registry_value)
{
	DWORD		dwType = 0, cbData = 0;
	char		str[256] = {0};
	LONG		hkResult = 0;

	cbData = sizeof(str);
	if ( (hkResult = RegQueryValueEx(_hkey, registry_key, NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
		*registry_value = str;
	//  Otherwise: Keep default value.
}

void CPreferences::ReadReg(const LPCTSTR registry_key, double *registry_value)
{
	DWORD		dwType = 0, cbData = 0;
	char		str[256] = {0};
	LONG		hkResult = 0;

	cbData = sizeof(str);
	if ( (hkResult = RegQueryValueEx(_hkey, registry_key, NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
		*registry_value = atof(str);
	//  Otherwise: Keep default value.
}

void CPreferences::WriteReg(const LPCTSTR registry_key, const int registry_value)
//  This method is used to write unsigned ints and booleans, too.
{
	char		str[256] = {0};

	sprintf_s(str, 256, "%d", registry_value);
	RegSetValueEx(_hkey, registry_key, 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
}

void CPreferences::WriteReg(const LPCTSTR registry_key, const CString registry_value)
{
	char		str[256] = {0};

	sprintf_s(str, 256, "%s", registry_value);
	RegSetValueEx(_hkey, registry_key, 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
}

void CPreferences::WriteReg(const LPCTSTR registry_key, const double registry_value)
{
	char		str[256] = {0};

	sprintf_s(str, 256, "%f", registry_value);
	RegSetValueEx(_hkey, registry_key, 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
}
