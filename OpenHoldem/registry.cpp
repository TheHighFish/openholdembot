
#include "stdafx.h"

#include "registry.h"
#include "debug.h"
#include "global.h"

#define OH_SUBKEY  "Software\\OpenHoldem\\OpenHoldem"

Registry::Registry(void)
{
    __SEH_SET_EXCEPTION_HANDLER(MyUnHandledExceptionFilter);
}


void Registry::init_Defaults(void)
{
    __SEH_HEADER

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
    udf_sort = udf_group = expand_std = expand_list = expand_udf = false;

    // symbols
    max_opponents = 9;
    handrank_value = "169";
    disable_caching = false;

    // autoplayer
    frame_delay = 2;
    swag_delay_1 = 400;
    swag_delay_2 = 400;
    swag_delay_3 = 700;
    ap_auto = false;
	focus_detect = false;

    // scraper
    scrape_delay = 750;

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

    // log$ loggin
    LogSymbol_enabled = false;
    LogSymbol_max_log = 5;

	Trace_enabled = false;
	memset(Trace_functions, false, sizeof(bool)*nTraceFunctions);

    versus_path = "";

    __SEH_LOGFATAL("Registry::init_Defaults : \n");

}


void Registry::read_OH_RegistryKey(HKEY hKey, LPCTSTR RegistryKey, int *RegistryValue)
{
    __SEH_HEADER

    DWORD		dwType, cbData;
    char		str[256];
    LONG		hkResult;

    cbData = sizeof(str);
    if ( (hkResult = RegQueryValueEx(hKey, RegistryKey, NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
        *RegistryValue = atoi(str);
    //  Otherwise: Keep default value.

    __SEH_LOGFATAL("Registry::read_OH_RegistryKey : \n");

}


void Registry::read_OH_RegistryKey(HKEY hKey, LPCTSTR RegistryKey, bool *RegistryValue)
{
    __SEH_HEADER

    DWORD		dwType, cbData;
    char		str[256];
    LONG		hkResult;

    cbData = sizeof(str);
    if ( (hkResult = RegQueryValueEx(hKey, RegistryKey, NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
        *RegistryValue = atoi(str);
    //  Otherwise: Keep default value.

    __SEH_LOGFATAL("Registry::read_OH_RegistryKey : \n");
}


void Registry::read_OH_RegistryKey(HKEY hKey, LPCTSTR RegistryKey, unsigned int *RegistryValue)
{
    __SEH_HEADER

    DWORD		dwType, cbData;
    char		str[256];
    LONG		hkResult;

    cbData = sizeof(str);
    if ( (hkResult = RegQueryValueEx(hKey, RegistryKey, NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
        *RegistryValue = atoi(str);
    //  Otherwise: Keep default value.

    __SEH_LOGFATAL("Registry::read_OH_RegistryKey : \n");
}


void Registry::read_OH_RegistryKey(HKEY hKey, LPCTSTR RegistryKey,  CString *RegistryValue)
{
    __SEH_HEADER

    DWORD		dwType, cbData;
    char		str[256];
    LONG		hkResult;

    cbData = sizeof(str);
    if ( (hkResult = RegQueryValueEx(hKey, RegistryKey, NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
        *RegistryValue = str;
    //  Otherwise: Keep default value.

    __SEH_LOGFATAL("Registry::read_OH_RegistryKey : \n");
}


void Registry::read_OH_RegistryKey(HKEY hKey, LPCTSTR RegistryKey, double *RegistryValue)
{
    __SEH_HEADER

    DWORD		dwType, cbData;
    char		str[256];
    LONG		hkResult;

    cbData = sizeof(str);
    if ( (hkResult = RegQueryValueEx(hKey, RegistryKey, NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS)
        *RegistryValue = atof(str);
    //  Otherwise: Keep default value.

    __SEH_LOGFATAL("Registry::read_OH_RegistryKey : \n");
}


void Registry::write_OH_RegistryKey(HKEY hKey, LPCTSTR RegistryKey, int RegistryValue)
//  This method is used to write unsigned ints and booleans, too.
{
    __SEH_HEADER

    char		str[256];

    sprintf(str, "%d", RegistryValue);
    RegSetValueEx(hKey, RegistryKey, 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

    __SEH_LOGFATAL("Registry::write_OH_RegistryKey : \n");

}


void Registry::write_OH_RegistryKey(HKEY hKey, LPCTSTR RegistryKey, CString RegistryValue)
{
    __SEH_HEADER

    char		str[256];

    sprintf(str, "%s", RegistryValue);
    RegSetValueEx(hKey, RegistryKey, 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);


    __SEH_LOGFATAL("Registry::write_OH_RegistryKey : \n");
}


void Registry::write_OH_RegistryKey(HKEY hKey, LPCTSTR RegistryKey, double RegistryValue)
{
    __SEH_HEADER

    char		str[256];

    sprintf(str, "%f", RegistryValue);
    RegSetValueEx(hKey, RegistryKey, 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

    __SEH_LOGFATAL("Registry::write_OH_RegistryKey : \n");

}


void Registry::read_reg(void)
{
    __SEH_HEADER

    HKEY		hKey;
    LONG		hkResult;

    init_Defaults();
    hkResult = RegOpenKeyEx(HKEY_CURRENT_USER, OH_SUBKEY, 0, KEY_READ, &hKey);
    if (hkResult==ERROR_SUCCESS) {

        // Main window location and size
        read_OH_RegistryKey(hKey, "main_x", &main_x);
        read_OH_RegistryKey(hKey, "main_y", &main_y);
        read_OH_RegistryKey(hKey, "main_dx", &main_dx);
        read_OH_RegistryKey(hKey, "main_dy", &main_dy);

        // Formula window location and size
        read_OH_RegistryKey(hKey, "formula_x", &formula_x);
        read_OH_RegistryKey(hKey, "formula_y", &formula_y);
        read_OH_RegistryKey(hKey, "formula_dx", &formula_dx);
        read_OH_RegistryKey(hKey, "formula_dy", &formula_dy);

        // ppro window location and size
        read_OH_RegistryKey(hKey, "ppro_x", &ppro_x);
        read_OH_RegistryKey(hKey, "ppro_y", &ppro_y);
        read_OH_RegistryKey(hKey, "ppro_dx", &ppro_dx);
        read_OH_RegistryKey(hKey, "ppro_dy", &ppro_dy);

        // scraper window location and size
        read_OH_RegistryKey(hKey, "scraper_x", &scraper_x);
        read_OH_RegistryKey(hKey, "scraper_y", &scraper_y);
        read_OH_RegistryKey(hKey, "scraper_dx", &scraper_dx);
        read_OH_RegistryKey(hKey, "scraper_dy", &scraper_dy);

        // formula editor
        read_OH_RegistryKey(hKey, "precision", &precision);
        read_OH_RegistryKey(hKey, "equal", &equal);
        read_OH_RegistryKey(hKey, "udf_sort", &udf_sort);
        read_OH_RegistryKey(hKey, "udf_group", &udf_group);
        read_OH_RegistryKey(hKey, "udf_sort", &udf_sort);
        read_OH_RegistryKey(hKey, "expand_std", &expand_std);
        read_OH_RegistryKey(hKey, "expand_list", &expand_list);
        read_OH_RegistryKey(hKey, "expand_udf", &expand_udf);

        // prefs - analyzer
        read_OH_RegistryKey(hKey, "max_opponents", &max_opponents);

        // prefs - autoplayer
        read_OH_RegistryKey(hKey, "frame_delay", &frame_delay);
        read_OH_RegistryKey(hKey, "swag_delay_1", &swag_delay_1);
        read_OH_RegistryKey(hKey, "swag_delay_2", &swag_delay_2);
        //  If a key "swag_delay" exists, use this as "swag_delay_3",
        //    if not defined in another way (checked below).
        //    (WH backward compatibility.)
        read_OH_RegistryKey(hKey, "swag_delay", &swag_delay_3);
        read_OH_RegistryKey(hKey, "swag_delay_3", &swag_delay_3);
        read_OH_RegistryKey(hKey, "swag_text_selection", &text_selection_method);
        read_OH_RegistryKey(hKey, "swag_text_deletion", &text_deletion_method);
        read_OH_RegistryKey(hKey, "swag_bet_confirmation", &bet_confirmation_method);
        read_OH_RegistryKey(hKey, "button_click_method", &button_click_method);
        read_OH_RegistryKey(hKey, "ap_auto", &ap_auto);
        read_OH_RegistryKey(hKey, "focus_detect", &focus_detect);

        // prefs - dll extension
        read_OH_RegistryKey(hKey, "dll_always_send_state", &dll_always_send_state);
        read_OH_RegistryKey(hKey, "load_dll_on_startup", &load_dll_on_startup);
        read_OH_RegistryKey(hKey, "dll_name", &dll_name);

        // prefs - scraper
        read_OH_RegistryKey(hKey, "scrape_delay", &scrape_delay);

        // prefs - symbols
        read_OH_RegistryKey(hKey, "avtime", &avtime);
        read_OH_RegistryKey(hKey, "handrank_value", &handrank_value);
        read_OH_RegistryKey(hKey, "disable_caching", &disable_caching);

        // Prefs - poker tracker
        read_OH_RegistryKey(hKey, "pt_disable", &pt_disable);
        read_OH_RegistryKey(hKey, "pt_ip_addr", &pt_ip_addr);
        read_OH_RegistryKey(hKey, "pt_port", &pt_port);
        read_OH_RegistryKey(hKey, "pt_user", &pt_user);
        read_OH_RegistryKey(hKey, "pt_pass", &pt_pass);
        read_OH_RegistryKey(hKey, "pt_dbname", &pt_dbname);
        read_OH_RegistryKey(hKey, "pt_updatedelay", &pt_updatedelay);
        read_OH_RegistryKey(hKey, "pt_cacherefresh", &pt_cacherefresh);

        // prefs - ICM
        read_OH_RegistryKey(hKey, "icm_prize1", &icm_prize1);
        read_OH_RegistryKey(hKey, "icm_prize2", &icm_prize2);
        read_OH_RegistryKey(hKey, "icm_prize3", &icm_prize3);
        read_OH_RegistryKey(hKey, "icm_prize4", &icm_prize4);

        // Prefs - Replay frames
        read_OH_RegistryKey(hKey, "replay_record", &replay_record);
        read_OH_RegistryKey(hKey, "replay_record_every_change", &replay_record_every_change);
        read_OH_RegistryKey(hKey, "replay_max_frames", &replay_max_frames);

        // poker Pro
        read_OH_RegistryKey(hKey, "ppro_hostname", &hostname);
        read_OH_RegistryKey(hKey, "ppro_port", &port);
        read_OH_RegistryKey(hKey, "ppro_username", &username);
        read_OH_RegistryKey(hKey, "ppro_password", &password);
        read_OH_RegistryKey(hKey, "ppro_handhistory", &handhistory);
        read_OH_RegistryKey(hKey, "ppro_chips", &chips);
        read_OH_RegistryKey(hKey, "ppro_autoseat", &autoseat);

        // blind locking
        read_OH_RegistryKey(hKey, "sblind", &sblind);
        read_OH_RegistryKey(hKey, "bblind", &bblind);
        read_OH_RegistryKey(hKey, "bbet", &bbet);
        read_OH_RegistryKey(hKey, "ante", &ante);
        read_OH_RegistryKey(hKey, "gametype", &gametype);

        // f$debug logging
        read_OH_RegistryKey(hKey, "fdebuglog", &fdebuglog);
        read_OH_RegistryKey(hKey, "fdebuglog_myturn", &fdebuglog_myturn);

        // scraper zoom level
        read_OH_RegistryKey(hKey, "scraper_zoom", &scraper_zoom);

        //  2008.02.27 by THF
        //  Perl
        read_OH_RegistryKey(hKey, "Perl_default_Formula", &Perl_default_Formula);
        read_OH_RegistryKey(hKey, "Perl_Editor", &Perl_Editor);
        read_OH_RegistryKey(hKey, "Perl_load_default_Formula", &Perl_load_default_Formula);
        read_OH_RegistryKey(hKey, "Perl_load_Interpreter", &Perl_load_Interpreter);

        //  2008.02.27 by THF
        //  PokerChat
        //
        read_OH_RegistryKey(hKey, "Chat_enabled", &Chat_enabled);
        read_OH_RegistryKey(hKey, "Chat_min_Delay", &Chat_min_Delay);
        read_OH_RegistryKey(hKey, "Chat_random_Delay", &Chat_random_Delay);

        // log$ logging
        read_OH_RegistryKey(hKey, "LogSymbol_enabled", &LogSymbol_enabled);
        read_OH_RegistryKey(hKey, "LogSymbol_max_log", &LogSymbol_max_log);

        read_OH_RegistryKey(hKey, "Trace_enabled", &Trace_enabled);
		CString regValue;
		for (int i=0;i<nTraceFunctions;i++) {
			regValue.Format("Trace_Functions%d", i);
			read_OH_RegistryKey(hKey, regValue, &Trace_functions[i]);
		}

        read_OH_RegistryKey(hKey, "versus", &versus_path);
    }

    RegCloseKey(hKey);

    __SEH_LOGFATAL("Registry::read_reg : \n");
}

void Registry::write_reg(void)
{
    __SEH_HEADER

    HKEY		hKey;
    DWORD		dwDisp;

    // Settings
    RegCreateKeyEx(HKEY_CURRENT_USER, OH_SUBKEY, 0, NULL,
                   REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDisp);

    // Main window location and size
    write_OH_RegistryKey(hKey, "main_x", main_x);
    write_OH_RegistryKey(hKey, "main_y", main_y);
    write_OH_RegistryKey(hKey, "main_dx", main_dx);
    write_OH_RegistryKey(hKey, "main_dy", main_dy);

    // Formula window location and size
    write_OH_RegistryKey(hKey, "formula_x", formula_x);
    write_OH_RegistryKey(hKey, "formula_y", formula_y);
    write_OH_RegistryKey(hKey, "formula_dx", formula_dx);
    write_OH_RegistryKey(hKey, "formula_dy", formula_dy);

    // ppro window location and size
    write_OH_RegistryKey(hKey, "ppro_x", ppro_x);
    write_OH_RegistryKey(hKey, "ppro_y", ppro_y);
    write_OH_RegistryKey(hKey, "ppro_dx", ppro_dx);
    write_OH_RegistryKey(hKey, "ppro_dy", ppro_dy);

    // scraper window location and size
    write_OH_RegistryKey(hKey, "scraper_x", scraper_x);
    write_OH_RegistryKey(hKey, "scraper_y", scraper_y);
    write_OH_RegistryKey(hKey, "scraper_dx", scraper_dx);
    write_OH_RegistryKey(hKey, "scraper_dy", scraper_dy);

    // formula editor
    write_OH_RegistryKey(hKey, "precision", precision);
    write_OH_RegistryKey(hKey, "equal", equal);
    write_OH_RegistryKey(hKey, "udf_sort", udf_sort);
    write_OH_RegistryKey(hKey, "udf_group", udf_group);
    write_OH_RegistryKey(hKey, "expand_std", expand_std);
    write_OH_RegistryKey(hKey, "expand_list", expand_list);
    write_OH_RegistryKey(hKey, "expand_udf", expand_udf);

    // prefs - analyzer
    write_OH_RegistryKey(hKey, "max_opponents", int(max_opponents));

    // prefs - autoplayer
    write_OH_RegistryKey(hKey, "frame_delay", int(frame_delay));
    write_OH_RegistryKey(hKey, "swag_delay_1", int(swag_delay_1));
    write_OH_RegistryKey(hKey, "swag_delay_2", int(swag_delay_2));
    write_OH_RegistryKey(hKey, "swag_delay_3", int(swag_delay_3));
    write_OH_RegistryKey(hKey, "swag_text_selection", int(text_selection_method));
    write_OH_RegistryKey(hKey, "swag_text_deletion", int(text_deletion_method));
    write_OH_RegistryKey(hKey, "swag_bet_confirmation", int(bet_confirmation_method));
    write_OH_RegistryKey(hKey, "button_click_method", int(button_click_method));
    write_OH_RegistryKey(hKey, "ap_auto", ap_auto);
    write_OH_RegistryKey(hKey, "focus_detect", focus_detect);

    // prefs - dll extension
    write_OH_RegistryKey(hKey, "dll_always_send_state", dll_always_send_state);
    write_OH_RegistryKey(hKey, "load_dll_on_startup", load_dll_on_startup);
    write_OH_RegistryKey(hKey, "dll_name", dll_name);

    // prefs - scraper
    write_OH_RegistryKey(hKey, "scrape_delay", int(scrape_delay));

    // prefs - symbols
    write_OH_RegistryKey(hKey, "avtime", avtime);
    write_OH_RegistryKey(hKey, "handrank_value", handrank_value);
    write_OH_RegistryKey(hKey, "disable_caching", disable_caching);

    // Prefs - poker tracker
    write_OH_RegistryKey(hKey, "pt_disable", pt_disable);
    write_OH_RegistryKey(hKey, "pt_ip_addr", pt_ip_addr);
    write_OH_RegistryKey(hKey, "pt_port", pt_port);
    write_OH_RegistryKey(hKey, "pt_user", pt_user);
    write_OH_RegistryKey(hKey, "pt_pass", pt_pass);
    write_OH_RegistryKey(hKey, "pt_dbname", pt_dbname);
    write_OH_RegistryKey(hKey, "pt_updatedelay", pt_updatedelay);
    write_OH_RegistryKey(hKey, "pt_cacherefresh", pt_cacherefresh);

    // Prefs - ICM
    write_OH_RegistryKey(hKey, "icm_prize1", icm_prize1);
    write_OH_RegistryKey(hKey, "icm_prize2", icm_prize2);
    write_OH_RegistryKey(hKey, "icm_prize3", icm_prize3);
    write_OH_RegistryKey(hKey, "icm_prize4", icm_prize4);

    // Prefs - Replay frames
    write_OH_RegistryKey(hKey, "replay_record", replay_record);
    write_OH_RegistryKey(hKey, "replay_record_every_change", replay_record_every_change);

    // Poker Pro
    write_OH_RegistryKey(hKey, "ppro_hostname", hostname);
    write_OH_RegistryKey(hKey, "ppro_port", port);
    write_OH_RegistryKey(hKey, "ppro_username", username);
    write_OH_RegistryKey(hKey, "ppro_password", password);
    write_OH_RegistryKey(hKey, "ppro_handhistory", handhistory);
    write_OH_RegistryKey(hKey, "ppro_chips", chips);
    write_OH_RegistryKey(hKey, "ppro_autoseat", autoseat);

    // blind locking
    write_OH_RegistryKey(hKey, "sblind", sblind);
    write_OH_RegistryKey(hKey, "bblind", bblind);
    write_OH_RegistryKey(hKey, "bbet", bbet);
    write_OH_RegistryKey(hKey, "ante", ante);
    write_OH_RegistryKey(hKey, "gametype", gametype);

    // f$debug logging
    write_OH_RegistryKey(hKey, "fdebuglog", fdebuglog);
    write_OH_RegistryKey(hKey, "fdebuglog_myturn", fdebuglog_myturn);

    // scraper zoom level
    write_OH_RegistryKey(hKey, "scraper_zoom", scraper_zoom);

    //  2008.02.27 by THF
    //  Perl
    write_OH_RegistryKey(hKey, "Perl_default_Formula", Perl_default_Formula);
    write_OH_RegistryKey(hKey, "Perl_Editor", Perl_Editor);
    write_OH_RegistryKey(hKey, "Perl_load_default_Formula", Perl_load_default_Formula);
    write_OH_RegistryKey(hKey, "Perl_load_Interpreter", Perl_load_Interpreter);

    //  2008.02.27 by THF
    //  PokerChat
    write_OH_RegistryKey(hKey, "Chat_enabled", Chat_enabled);
    write_OH_RegistryKey(hKey, "Chat_min_Delay", Chat_min_Delay);
    write_OH_RegistryKey(hKey, "Chat_random_Delay", Chat_random_Delay);

    // log$ loggin
    write_OH_RegistryKey(hKey, "LogSymbol_enabled", LogSymbol_enabled);
    write_OH_RegistryKey(hKey, "LogSymbol_max_log", LogSymbol_max_log);

    write_OH_RegistryKey(hKey, "Trace_enabled", Trace_enabled);
    write_OH_RegistryKey(hKey, "Trace_functions1", Trace_functions[0]);
    write_OH_RegistryKey(hKey, "Trace_functions2", Trace_functions[1]);
    write_OH_RegistryKey(hKey, "Trace_functions3", Trace_functions[2]);
    write_OH_RegistryKey(hKey, "Trace_functions4", Trace_functions[3]);
    write_OH_RegistryKey(hKey, "Trace_functions5", Trace_functions[4]);

    RegCloseKey(hKey);

    __SEH_LOGFATAL("Registry::write_reg : \n");
}

bool Registry::GetProfileFont(LPCTSTR lpszKey, LPCTSTR lpszVal, CFont& font, CDC* pDC)
{
    __SEH_HEADER

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

    __SEH_LOGFATAL("Registry::GetProfileFont : \n");
}


bool Registry::WriteProfileFont(LPCTSTR lpszKey, LPCTSTR lpszVal, CFont& font, CDC* pDC)
{
    __SEH_HEADER

    CWinApp *pApp = AfxGetApp();
    ASSERT_VALID(pApp);
    LOGFONT lf;
    font.GetLogFont(&lf);
    int iPtSize = MulDiv(-lf.lfHeight, 72,
                         ::GetDeviceCaps(pDC ? pDC->m_hDC : ::GetDC(NULL), LOGPIXELSY));
    CString s;
    s.Format("%s,%d,%d,%d", lf.lfFaceName, iPtSize, lf.lfWeight, lf.lfItalic);
    return pApp->WriteProfileString(lpszKey, lpszVal, s)!=0;

    __SEH_LOGFATAL("Registry::WriteProfileFont : \n");
}

void Registry::readRegString(CString RegistryKey, char* RegistryValue)
{
    __SEH_HEADER

    HKEY				hKey;
    LONG				result;
    DWORD				strSize = MAX_PATH;

    result = RegOpenKeyEx(HKEY_CURRENT_USER, OH_SUBKEY, 0, KEY_READ, &hKey);
    if (result==ERROR_SUCCESS){
        RegQueryValueEx(hKey,RegistryKey,NULL,NULL,(LPBYTE)RegistryValue,&strSize);
    }
    RegCloseKey(hKey);

    __SEH_LOGFATAL("Registry::readRegString :\n");
}

void Registry::writeRegString(CString RegistryKey, CString RegistryValue)
{
    __SEH_HEADER

    HKEY				hKey;
    LONG				result;
    char				str[MAX_PATH];

    result = RegCreateKeyEx(HKEY_CURRENT_USER, OH_SUBKEY, 0, NULL,
                            REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
    if (result==ERROR_SUCCESS){
        sprintf(str, "%s", RegistryValue);
        RegSetValueEx(hKey, RegistryKey, 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
    }
    RegCloseKey(hKey);

    __SEH_LOGFATAL("Registry::writeRegString :\n");
}
