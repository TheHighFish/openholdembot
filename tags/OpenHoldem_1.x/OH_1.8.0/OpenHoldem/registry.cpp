
#include "stdafx.h"

#include "registry.h"
#include "debug.h"
#include "global.h"

Registry::Registry(void) 
{
#ifdef SEH_ENABLE
	// Set exception handler
	SetUnhandledExceptionFilter(MyUnHandledExceptionFilter);
#endif
}

void Registry::read_reg(void) 
{
#ifdef SEH_ENABLE
	try {
#endif
		HKEY		hKey;
		DWORD		dwType, cbData;
		LONG		hkResult;
		char		str[256];

		// Defaults
		main_x = main_y = 0;
		main_dx = 800;
		main_dy = 400;

		formula_x = formula_y = 0;
		formula_dx = 700;
		formula_dy = 420;

		ppro_x = ppro_y = -1;
		ppro_dx = 555;
		ppro_dy = 577;

		scraper_x = scraper_y = 0;
		scraper_dx = 340;
		scraper_dy = 250;

		precision = 4;
		equal = 12;
		udf_sort = expand_std = expand_list = expand_udf = false;

		// symbols
		max_opponents = 9;
		handrank_value = "169";
		disable_caching = false;

		// autoplayer
		frame_delay = 2;
		swag_delay_1 = 400;
		swag_delay_2 = 400;
		swag_delay_3 = 700;
		scrape_delay = 750;
		ap_auto = false;

		// scraper
		name_scrapes = 3;
		balance_scrapes = 3;

		// swag
		text_selection_method = TEXTSEL_DOUBLECLICK;
		text_deletion_method = TEXTDEL_DELETE;
		bet_confirmation_method = BETCONF_CLICKBET;
		button_click_method = BUTTON_SINGLECLICK;

		// dll
		dll_always_send_state = true;
		load_dll_on_startup = false;
		dll_name = "whuser.dll";

		// ppro
		hostname = "127.0.0.1";
		port = "30000";
		username = "openholdem";
		password = "";
		handhistory = false;
		chips = 1000;
		autoseat = true;

		// blind locking
		sblind = 5;
		bblind = 10;
		bbet = 20;
		ante = 0;
		gametype = LIMIT_NL;

		// f$debug logging
		fdebuglog = false;
		fdebuglog_myturn = true;

		// game state engine
		avtime = 15;

		// scraper zoom level
		scraper_zoom = 2;  // 4x
		
		// Poker Tracker
		pt_disable = true;
		pt_ip_addr = "127.0.0.1";
		pt_port = "5432";
		pt_user = "pokertracker";
		pt_pass = "";
		pt_dbname = "";
		pt_updatedelay = 30;
		pt_cacherefresh = 120;

		// ICM
		icm_prize1 = 0.5;
		icm_prize2 = 0.3;
		icm_prize3 = 0.2;
		icm_prize4 = 0.0;

		// Replay frames
		replay_record = false;
		replay_record_every_change = false;
		replay_max_frames = 100;

		//  2008.02.27 by THF
		//  Perl
		Perl_default_Formula = "";
		//  Unfortunately "%SystemRoot%\\notepad.exe" does not work;
		//    It seems, this kind of filename substitution is only
		//    supported by the DOS shell, not by the Windows API.
		//    Therefore we use a hardcoded filename. 
		Perl_Editor = "C:\\Windows\\notepad.exe";
		Perl_load_default_Formula = false;
		Perl_load_Interpreter = false;

		//  2008.02.27 by THF
		//  PokerChat
		Chat_enabled = false;
		//  Just a security measure against crazy bot formulas...
		Chat_min_Delay = 600;      //  seconds
		Chat_random_Delay = 3000;  //  seconds;


		hkResult = RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\OpenHoldem\\OpenHoldem", 0, KEY_READ, &hKey);
		if (hkResult==ERROR_SUCCESS) {

			// Main window location and size
			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "main_x", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				main_x = atoi(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "main_y", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				main_y = atoi(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "main_dx", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				main_dx = atoi(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "main_dy", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				main_dy = atoi(str);


			// Formula window location and size
			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "formula_x", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				formula_x = atoi(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "formula_y", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				formula_y = atoi(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "formula_dx", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				formula_dx = atoi(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "formula_dy", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				formula_dy = atoi(str);


			// ppro window location and size
			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "ppro_x", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				ppro_x = atoi(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "ppro_y", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				ppro_y = atoi(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "ppro_dx", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				ppro_dx = atoi(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "ppro_dy", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				ppro_dy = atoi(str);


			// scraper window location and size
			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "scraper_x", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				scraper_x = atoi(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "scraper_y", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				scraper_y = atoi(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "scraper_dx", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				scraper_dx = atoi(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "scraper_dy", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				scraper_dy = atoi(str);


			// formula editor
			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "precision", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				precision = atoi(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "equal", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				equal = atoi(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "udf_sort", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				udf_sort = atoi(str)!=0;

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "expand_std", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				expand_std = atoi(str)!=0;

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "expand_list", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				expand_list = atoi(str)!=0;

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "expand_udf", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				expand_udf = atoi(str)!=0;


			// prefs - analyzer
			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "max_opponents", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				max_opponents = atoi(str);


			// prefs - autoplayer
			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "frame_delay", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				frame_delay = atoi(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "swag_delay_1", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				swag_delay_1 = atoi(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "swag_delay_2", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				swag_delay_2 = atoi(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "swag_delay_3", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				swag_delay_3 = atoi(str);

			else if( (hkResult = RegQueryValueEx(hKey, "swag_delay", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				swag_delay_3 = atoi(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "swag_text_selection", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				text_selection_method = atoi(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "swag_text_deletion", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				text_deletion_method = atoi(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "swag_bet_confirmation", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				bet_confirmation_method = atoi(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "button_click_method", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				button_click_method = atoi(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "ap_auto", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				ap_auto = atoi(str)!=0;

			// prefs - dll extension
			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "dll_always_send_state", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				dll_always_send_state = atoi(str)!=0;

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "load_dll_on_startup", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				load_dll_on_startup = atoi(str)!=0;

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "dll_name", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				dll_name = str;


			// prefs - scraper
			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "scrape_delay", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				scrape_delay = atoi(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "name_scrapes", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				name_scrapes = atoi(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "balance_scrapes", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				balance_scrapes = atoi(str);


			// prefs - symbols
			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "avtime", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				avtime = atof(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "handrank_value", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				handrank_value = str;

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "disable_caching", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				disable_caching = atoi(str)!=0;


			// Prefs - poker tracker
			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "pt_disable", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				pt_disable = atoi(str) != 0;

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "pt_ip_addr", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				pt_ip_addr = str;

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "pt_port", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				pt_port = str;

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "pt_user", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				pt_user = str;

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "pt_pass", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				pt_pass = str;

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "pt_dbname", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				pt_dbname = str;

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "pt_updatedelay", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				pt_updatedelay = atoi(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "pt_cacherefresh", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				pt_cacherefresh = atoi(str);


			// prefs - ICM
			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "icm_prize1", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				icm_prize1 = atof(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "icm_prize2", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				icm_prize2 = atof(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "icm_prize3", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				icm_prize3 = atof(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "icm_prize4", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				icm_prize4 = atof(str);


			// Prefs - Replay frames
			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "replay_record", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				replay_record = atoi(str) != 0;

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "replay_record_every_change", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				replay_record_every_change = atoi(str) != 0;

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "replay_max_frames", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				replay_max_frames = atoi(str);

		
			// poker Pro
			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "ppro_hostname", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				hostname = str;

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "ppro_port", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				port = str;

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "ppro_username", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				username = str;

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "ppro_password", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				password = str;

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "ppro_handhistory", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				handhistory = atoi(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "ppro_chips", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				chips = atoi(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "ppro_autoseat", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				autoseat = atoi(str);


			// blind locking
			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "sblind", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				sblind = atof(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "bblind", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				bblind = atof(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "bbet", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				bbet = atof(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "ante", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				ante = atof(str);

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "gametype", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				gametype = atoi(str);


			// f$debug logging
			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "fdebuglog", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				fdebuglog = atoi(str) != 0;

			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "fdebuglog_myturn", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				fdebuglog_myturn = atoi(str) != 0;


			// scraper zoom level
			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "scraper_zoom", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
				scraper_zoom = atoi(str);

			//  2008.02.27 by THF
			//  Perl
			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "Perl_default_Formula", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS) {
				Perl_default_Formula = str;
			}
			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "Perl_Editor", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS) {
				Perl_Editor = str;
			}
			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "Perl_load_default_Formula", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS) {
				Perl_load_default_Formula = atoi(str);
			}
			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "Perl_load_Interpreter", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS) {
				Perl_load_Interpreter = atoi(str);
			}

			//  2008.02.27 by THF
			//  PokerChat
			// 
			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "Chat_enabled", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS) {
				Chat_enabled = atoi(str);
			}
			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "Chat_min_Delay", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS) {
				Chat_min_Delay = atoi(str);
			}
			cbData = sizeof(str);
			if ( (hkResult = RegQueryValueEx(hKey, "Chat_random_Delay", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS) {
				Chat_random_Delay = atoi(str);
			}
		}

		RegCloseKey(hKey);
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("Registry::read_reg : \n"); 
		throw;
	}
#endif
}

void Registry::write_reg(void) 
{
#ifdef SEH_ENABLE
	try {
#endif
		HKEY		hKey;
		DWORD		dwDisp;
		char		str[256];

		// Settings
		RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\OpenHoldem\\OpenHoldem", 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDisp);

		// Main window location and size
		sprintf(str, "%d", main_x);
		RegSetValueEx(hKey, "main_x", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", main_y);
		RegSetValueEx(hKey, "main_y", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", main_dx);
		RegSetValueEx(hKey, "main_dx", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", main_dy);
		RegSetValueEx(hKey, "main_dy", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

		// Formula window location and size
		sprintf(str, "%d", formula_x);
		RegSetValueEx(hKey, "formula_x", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", formula_y);
		RegSetValueEx(hKey, "formula_y", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", formula_dx);
		RegSetValueEx(hKey, "formula_dx", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", formula_dy);
		RegSetValueEx(hKey, "formula_dy", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

		// ppro window location and size
		sprintf(str, "%d", ppro_x);
		RegSetValueEx(hKey, "ppro_x", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", ppro_y);
		RegSetValueEx(hKey, "ppro_y", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", ppro_dx);
		RegSetValueEx(hKey, "ppro_dx", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", ppro_dy);
		RegSetValueEx(hKey, "ppro_dy", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

		// scraper window location and size
		sprintf(str, "%d", scraper_x);
		RegSetValueEx(hKey, "scraper_x", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", scraper_y);
		RegSetValueEx(hKey, "scraper_y", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", scraper_dx);
		RegSetValueEx(hKey, "scraper_dx", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", scraper_dy);
		RegSetValueEx(hKey, "scraper_dy", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

		// formula editor
		sprintf(str, "%d", precision);
		RegSetValueEx(hKey, "precision", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", equal);
		RegSetValueEx(hKey, "equal", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", udf_sort);
		RegSetValueEx(hKey, "udf_sort", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", expand_std);
		RegSetValueEx(hKey, "expand_std", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", expand_list);
		RegSetValueEx(hKey, "expand_list", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", expand_udf);
		RegSetValueEx(hKey, "expand_udf", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

		// prefs - analyzer
		sprintf(str, "%d", max_opponents);
		RegSetValueEx(hKey, "max_opponents", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

		// prefs - autoplayer
		sprintf(str, "%d", frame_delay);
		RegSetValueEx(hKey, "frame_delay", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", swag_delay_1);
		RegSetValueEx(hKey, "swag_delay_1", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", swag_delay_2);
		RegSetValueEx(hKey, "swag_delay_2", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", swag_delay_3);
		RegSetValueEx(hKey, "swag_delay_3", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", text_selection_method);
		RegSetValueEx(hKey, "swag_text_selection", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", text_deletion_method);
		RegSetValueEx(hKey, "swag_text_deletion", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", bet_confirmation_method);
		RegSetValueEx(hKey, "swag_bet_confirmation", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", button_click_method);
		RegSetValueEx(hKey, "button_click_method", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", ap_auto);
		RegSetValueEx(hKey, "ap_auto", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

		// prefs - dll extension
		sprintf(str, "%d", dll_always_send_state);
		RegSetValueEx(hKey, "dll_always_send_state", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", load_dll_on_startup);
		RegSetValueEx(hKey, "load_dll_on_startup", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%s", dll_name.GetString());
		RegSetValueEx(hKey, "dll_name", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

		// prefs - scraper
		sprintf(str, "%d", scrape_delay);
		RegSetValueEx(hKey, "scrape_delay", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", name_scrapes);
		RegSetValueEx(hKey, "name_scrapes", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", balance_scrapes);
		RegSetValueEx(hKey, "balance_scrapes", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

		// prefs - symbols
		sprintf(str, "%.2f", avtime);
		RegSetValueEx(hKey, "avtime", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%s", handrank_value.GetString());
		RegSetValueEx(hKey, "handrank_value", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", disable_caching);
		RegSetValueEx(hKey, "disable_caching", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

		// Prefs - poker tracker
		sprintf(str, "%d", pt_disable);
		RegSetValueEx(hKey, "pt_disable", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%s", pt_ip_addr.GetString());
		RegSetValueEx(hKey, "pt_ip_addr", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%s", pt_port.GetString());
		RegSetValueEx(hKey, "pt_port", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%s", pt_user.GetString());
		RegSetValueEx(hKey, "pt_user", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%s", pt_pass.GetString());
		RegSetValueEx(hKey, "pt_pass", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%s", pt_dbname.GetString());
		RegSetValueEx(hKey, "pt_dbname", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", pt_updatedelay);
		RegSetValueEx(hKey, "pt_updatedelay", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", pt_cacherefresh);
		RegSetValueEx(hKey, "pt_cacherefresh", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

		// Prefs - ICM
		if (icm_prize1 == (int) icm_prize1)
			sprintf(str, "%.0f", icm_prize1);
		else
			sprintf(str, "%f", icm_prize1);
		RegSetValueEx(hKey, "icm_prize1", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

		if (icm_prize2 == (int) icm_prize2)
			sprintf(str, "%.0f", icm_prize2);
		else
			sprintf(str, "%f", icm_prize2);
		RegSetValueEx(hKey, "icm_prize2", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

		if (icm_prize3 == (int) icm_prize3)
			sprintf(str, "%.0f", icm_prize3);
		else
			sprintf(str, "%f", icm_prize3);
		RegSetValueEx(hKey, "icm_prize3", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

		if (icm_prize4 == (int) icm_prize4)
			sprintf(str, "%.0f", icm_prize4);
		else
			sprintf(str, "%f", icm_prize4);
		RegSetValueEx(hKey, "icm_prize4", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

		// Prefs - Replay frames
		sprintf(str, "%d", replay_record);
		RegSetValueEx(hKey, "replay_record", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", replay_record_every_change);
		RegSetValueEx(hKey, "replay_record_every_change", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", replay_max_frames);
		RegSetValueEx(hKey, "replay_max_frames", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

		// Poker Pro
		sprintf(str, "%s", hostname.GetString());
		RegSetValueEx(hKey, "ppro_hostname", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%s", port.GetString());
		RegSetValueEx(hKey, "ppro_port", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%s", username.GetString());
		RegSetValueEx(hKey, "ppro_username", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%s", password.GetString());
		RegSetValueEx(hKey, "ppro_password", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", handhistory);
		RegSetValueEx(hKey, "ppro_handhistory", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", chips);
		RegSetValueEx(hKey, "ppro_chips", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", autoseat);
		RegSetValueEx(hKey, "ppro_autoseat", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

		// blind locking
		sprintf(str, "%.2f", sblind);
		RegSetValueEx(hKey, "sblind", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%.2f", bblind);
		RegSetValueEx(hKey, "bblind", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%.2f", bbet);
		RegSetValueEx(hKey, "bbet", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%.2f", ante);
		RegSetValueEx(hKey, "ante", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", gametype);
		RegSetValueEx(hKey, "gametype", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

		// f$debug logging
		sprintf(str, "%d", fdebuglog);
		RegSetValueEx(hKey, "fdebuglog", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", fdebuglog_myturn);
		RegSetValueEx(hKey, "fdebuglog_myturn", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

		// scraper zoom level
		sprintf(str, "%d", scraper_zoom);
		RegSetValueEx(hKey, "scraper_zoom", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

		//  2008.02.27 by THF
		//  Perl
		sprintf(str, "%s", Perl_default_Formula.GetString());
		RegSetValueEx(hKey, "Perl_default_Formula", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%s", Perl_Editor.GetString());
		RegSetValueEx(hKey, "Perl_Editor", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", Perl_load_default_Formula);
		RegSetValueEx(hKey, "Perl_load_default_Formula", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", Perl_load_Interpreter);
		RegSetValueEx(hKey, "Perl_load_Interpreter", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

		//  2008.02.27 by THF
		//  PokerChat
		sprintf(str, "%d", Chat_enabled);
		RegSetValueEx(hKey, "Chat_enabled", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", Chat_min_Delay);
		RegSetValueEx(hKey, "Chat_min_Delay", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
		sprintf(str, "%d", Chat_random_Delay);
		RegSetValueEx(hKey, "Chat_random_Delay", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

		RegCloseKey(hKey);
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("Registry::write_reg : \n"); 
		throw;
	}
#endif
}

bool Registry::GetProfileFont(LPCTSTR lpszKey, LPCTSTR lpszVal, CFont& font, CDC* pDC) 
{
#ifdef SEH_ENABLE
	try {
#endif
		CWinApp *pApp = AfxGetApp();
		ASSERT_VALID(pApp);
		CString s = pApp->GetProfileString(lpszKey, lpszVal);
		if (s.IsEmpty())
			return FALSE;

		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));
		lf.lfCharSet = DEFAULT_CHARSET;
		int bItalic;
		int iPtSize;

		if (sscanf((LPCTSTR)s, "%[a-zA-Z ],%d,%d,%d",
			lf.lfFaceName, &iPtSize, &lf.lfWeight, &bItalic) != 4)
			return FALSE;
		lf.lfHeight = MulDiv(-iPtSize, 	// convert ptsize to logical units
			::GetDeviceCaps(pDC ? pDC->m_hDC : ::GetDC(NULL), LOGPIXELSY), 72);
		lf.lfItalic = bItalic;	// because lf.lfItalic is a BYTE
		font.DeleteObject();		// bye 
		return font.CreateFontIndirect(&lf)!=0;
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("Registry::GetProfileFont : \n"); 
		throw;
	}
#endif
}


bool Registry::WriteProfileFont(LPCTSTR lpszKey, LPCTSTR lpszVal, CFont& font, CDC* pDC) 
{
#ifdef SEH_ENABLE
	try {
#endif
		CWinApp *pApp = AfxGetApp();
		ASSERT_VALID(pApp);
		LOGFONT lf;
		font.GetLogFont(&lf);
		int iPtSize = MulDiv(-lf.lfHeight, 72,
			::GetDeviceCaps(pDC ? pDC->m_hDC : ::GetDC(NULL), LOGPIXELSY));
		CString s;
		s.Format("%s,%d,%d,%d", lf.lfFaceName, iPtSize, lf.lfWeight, lf.lfItalic);
		return pApp->WriteProfileString(lpszKey, lpszVal, s)!=0;
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("Registry::WriteProfileFont : \n"); 
		throw;
	}
#endif
}
