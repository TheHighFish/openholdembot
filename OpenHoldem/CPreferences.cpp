#include "stdafx.h"

#include "debug.h"
#include "CPreferences.h"

//
// Constructor and destructor
//
CPreferences::CPreferences()
{
	Read_From_Registry();
}

CPreferences::~CPreferences()	
{
}


//
// CPreferences::setter-methods
//

// Analyzer
void CPreferences::Set_max_opponents(unsigned int New_Value)
{
	_max_opponents = New_Value;
	Reg.Write_OH_RegistryKey("max_opponents", int(_max_opponents));
}

// Autoplayer
void CPreferences::Set_frame_delay(unsigned int New_Value)
{
	_frame_delay = New_Value;
	Reg.Write_OH_RegistryKey("frame_delay", int(_frame_delay));
}

void CPreferences::Set_swag_delay_1(unsigned int New_Value)		
{
	_swag_delay_1 = New_Value;
    Reg.Write_OH_RegistryKey("swag_delay_1", int(_swag_delay_1));
}
 
void CPreferences::Set_swag_delay_2(unsigned int New_Value)	
{
	_swag_delay_2 = New_Value;
    Reg.Write_OH_RegistryKey("swag_delay_2", int(_swag_delay_2));    
}
 
void CPreferences::Set_swag_delay_3(unsigned int New_Value)	
{
	_swag_delay_3 = New_Value;
	Reg.Write_OH_RegistryKey("swag_delay_3", int(_swag_delay_3));    
}
 
void CPreferences::Set_text_selection_method(unsigned int New_Value)	
{
	_text_selection_method = New_Value;
	Reg.Write_OH_RegistryKey("swag_text_selection", int(_text_selection_method));    
}
 
void CPreferences::Set_text_deletion_method(unsigned int New_Value)	
{
	_text_deletion_method = New_Value;
	Reg.Write_OH_RegistryKey("swag_text_deletion", int(_text_deletion_method));    
}
 
void CPreferences::Set_bet_confirmation_method(unsigned int New_Value)	
{
	_bet_confirmation_method = New_Value;
	Reg.Write_OH_RegistryKey("swag_bet_confirmation", int(_bet_confirmation_method));    
}
 
void CPreferences::Set_button_click_method(unsigned int New_Value)	
{
	_button_click_method = New_Value;
	Reg.Write_OH_RegistryKey("button_click_method", int(_button_click_method));    
}
 
void CPreferences::Set_ap_auto(bool New_Value)	
{
	_ap_auto = New_Value;
	Reg.Write_OH_RegistryKey("auto", _ap_auto);    
}
 
void CPreferences::Set_focus_detect(bool New_Value)	
{
	_focus_detect = New_Value;
	Reg.Write_OH_RegistryKey("focus_detect", _focus_detect);
}
 
// Dll extension
void CPreferences::Set_dll_always_send_state(bool New_Value)	
{
	_dll_always_send_state = New_Value;
	Reg.Write_OH_RegistryKey("dll_always_send_state", _dll_always_send_state);    
}
 
void CPreferences::Set_dll_load_on_startup(bool New_Value)	
{
	_dll_load_on_startup = New_Value;
	Reg.Write_OH_RegistryKey("load_dll_on_startup", _dll_load_on_startup);    
}
 
void CPreferences::Set_dll_name(CString New_Value)	
{
	_dll_name = New_Value;
	Reg.Write_OH_RegistryKey("dll_name", _dll_name);
}

// Scraper
void CPreferences::Set_scrape_delay(unsigned int New_Value)	
{
	_scrape_delay = New_Value;
	Reg.Write_OH_RegistryKey("scrape_delay", int(_scrape_delay));
}
 
// Symbols
void CPreferences::Set_sym_av_time(double New_Value)	
{
	_sym_av_time = New_Value;
	Reg.Write_OH_RegistryKey("avtime", _sym_av_time);    
}
 
void CPreferences::Set_sym_handrank_value(CString New_Value)	
{
	_sym_handrank_value = New_Value;
	Reg.Write_OH_RegistryKey("handrank_value", _sym_handrank_value);
}
 
void CPreferences::Set_sym_disable_caching(bool New_Value)	
{
	_sym_disable_caching = New_Value;
	Reg.Write_OH_RegistryKey("disable_caching", _sym_disable_caching);
}
 
// ICM
void CPreferences::Set_icm_prize1(double New_Value)	
{
	_icm_prize1 = New_Value;
	Reg.Write_OH_RegistryKey("icm_prize1", _icm_prize1);
}
 
void CPreferences::Set_icm_prize2(double New_Value)	
{
	_icm_prize2 = New_Value;
	Reg.Write_OH_RegistryKey("icm_prize2", _icm_prize2);    
}
 
void CPreferences::Set_icm_prize3(double New_Value)	
{
	_icm_prize3 = New_Value;
	Reg.Write_OH_RegistryKey("icm_prize3", _icm_prize3);
}
 
void CPreferences::Set_icm_prize4(double New_Value)	
{
	_icm_prize4 = New_Value;
	Reg.Write_OH_RegistryKey("icm_prize4", _icm_prize4);
}
 
// Replay Frames
void CPreferences::Set_replay_record(bool New_Value)	
{
	_replay_record = New_Value;
	Reg.Write_OH_RegistryKey("replay_record", _replay_record);    
}
 
void CPreferences::Set_replay_record_every_change(bool New_Value)	
{
	_replay_record_every_change = New_Value;
	Reg.Write_OH_RegistryKey("replay_record_every_change", _replay_record_every_change);	
}
 
void CPreferences::Set_replay_max_frames(int New_Value)	
{
	_replay_max_frames = New_Value;
	Reg.Write_OH_RegistryKey("replay_max_frames", _replay_max_frames);
}
 
// Poker Tracker
void CPreferences::Set_pt_ip_addr(CString New_Value)	
{
	_pt_ip_addr = New_Value;
	Reg.Write_OH_RegistryKey("pt_ip_addr", _pt_ip_addr);
}
 
void CPreferences::Set_pt_port(CString New_Value)
{
	_pt_port = New_Value;
	Reg.Write_OH_RegistryKey("pt_port", _pt_port);
}
 
void CPreferences::Set_pt_dbname(CString New_Value)
{
	_pt_dbname = New_Value;
	Reg.Write_OH_RegistryKey("pt_dbname", _pt_dbname);
}
 
void CPreferences::Set_pt_user(CString New_Value)
{
	_pt_user = New_Value;
	Reg.Write_OH_RegistryKey("pt_user", _pt_user);
}
 
void CPreferences::Set_pt_pass(CString New_Value)
{
	_pt_pass = New_Value;
	Reg.Write_OH_RegistryKey("pt_pass", _pt_pass);
}
 
void CPreferences::Set_pt_disable(bool New_Value)
{
	_pt_disable = New_Value;
	Reg.Write_OH_RegistryKey("pt_disable", _pt_disable);
}
 
void CPreferences::Set_pt_update_delay(int New_Value)
{
	_pt_update_delay = New_Value;
	Reg.Write_OH_RegistryKey("pt_updatedelay", _pt_update_delay);
}
 
void CPreferences::Set_pt_cache_refresh(int New_Value)
{
	_pt_cache_refresh = New_Value;
	Reg.Write_OH_RegistryKey("pt_cacherefresh", _pt_cache_refresh);
}
 
// Perl
void CPreferences::Set_Perl_default_Formula(CString New_Value)
{
	_Perl_default_Formula = New_Value;	 
	Reg.Write_OH_RegistryKey("Perl_default_Formula", _Perl_default_Formula);    
}
 
void CPreferences::Set_Perl_Editor(CString New_Value)
{
	_Perl_Editor = New_Value;
	Reg.Write_OH_RegistryKey("Perl_Editor", _Perl_Editor);    
}
 
void CPreferences::Set_Perl_load_default_Formula(bool New_Value)
{
	_Perl_load_default_Formula = New_Value;
	Reg.Write_OH_RegistryKey("Perl_load_default_Formula", _Perl_load_default_Formula);
}
 
void CPreferences::Set_Perl_load_Interpreter(bool New_Value)
{
	_Perl_load_Interpreter = New_Value;
	Reg.Write_OH_RegistryKey("Perl_load_Interpreter", _Perl_load_Interpreter);
}
 
//  PokerChat
void CPreferences::Set_Chat_enabled(bool New_Value)
{
	_Chat_enabled = New_Value;
	Reg.Read_OH_RegistryKey("Perl_default_Formula", &_Perl_default_Formula);
}
 
void CPreferences::Set_Chat_min_Delay(int New_Value)
{
	_Chat_min_Delay = New_Value;
	Reg.Write_OH_RegistryKey("Chat_min_Delay", _Chat_min_Delay);    
}
 
void CPreferences::Set_Chat_random_Delay(int New_Value)
{
	_Chat_random_Delay = New_Value;
	Reg.Write_OH_RegistryKey("Chat_random_Delay", _Chat_random_Delay);
}
 
// Log Symbol
void CPreferences::Set_LogSymbol_enabled(bool New_Value)
{
	_LogSymbol_enabled = New_Value;
	Reg.Write_OH_RegistryKey("LogSymbol_enabled", _LogSymbol_enabled);    
}
 
void CPreferences::Set_LogSymbol_max_log(bool New_Value)
{
	_LogSymbol_max_log = New_Value;
	Reg.Write_OH_RegistryKey("LogSymbol_max_log", _LogSymbol_max_log);    
}
 
// Tracing
void CPreferences::Set_Trace_enabled(bool New_Value)
{
	_Trace_enabled = New_Value;
	Reg.Write_OH_RegistryKey("Trace_enabled", _Trace_enabled);
}
 
void CPreferences::Set_Trace_functions(unsigned int FunctionNumber, bool Flag)
{
	_Trace_functions[FunctionNumber] = Flag;
	CString regValue;
	// Registry keys are named 1..N
	// Array indices are 0..(N-1)
	// TODO: Maybe change?
	regValue.Format("Trace_functions%d", FunctionNumber+1);
	Reg.Write_OH_RegistryKey(regValue, _Trace_functions[FunctionNumber]);
}

// PPro
void CPreferences::Set_ppro_hostname(CString New_Value)
{
	_ppro_hostname = New_Value;
	Reg.Write_OH_RegistryKey("ppro_hostname", _ppro_hostname);
}
 
void CPreferences::Set_ppro_port(CString New_Value)
{
	_ppro_port = New_Value;
	Reg.Write_OH_RegistryKey("ppro_port", _ppro_port);
}
 
void CPreferences::Set_ppro_username(CString New_Value)
{
	_ppro_username = New_Value;
	Reg.Write_OH_RegistryKey("ppro_username", _ppro_username);
}
 
void CPreferences::Set_ppro_password(CString New_Value)
{
	_ppro_password = New_Value;
	Reg.Write_OH_RegistryKey("ppro_password", _ppro_password);
}
 
void CPreferences::Set_ppro_x(int New_Value)
{
	_ppro_x = New_Value;
	Reg.Write_OH_RegistryKey("ppro_x", _ppro_x);
}
 
void CPreferences::Set_ppro_y(int New_Value)
{
	_ppro_y = New_Value;
	Reg.Write_OH_RegistryKey("ppro_y", _ppro_y);
}
 
void CPreferences::Set_ppro_dx(int New_Value)
{
	_ppro_dx = New_Value;
	Reg.Write_OH_RegistryKey("ppro_dx", _ppro_dx);
}
 
void CPreferences::Set_ppro_dy(int New_Value)
{
	_ppro_dy = New_Value;
	Reg.Write_OH_RegistryKey("ppro_dy", _ppro_dy);
}
 
void CPreferences::Set_ppro_handhistory(bool New_Value)
{
	_ppro_handhistory = New_Value;
	Reg.Write_OH_RegistryKey("ppro_handhistory", _ppro_handhistory);
}
 
void CPreferences::Set_ppro_chips(unsigned int New_Value)
{
	_ppro_chips = New_Value;
	Reg.Write_OH_RegistryKey("ppro_chips", _ppro_chips);
}
 
void CPreferences::Set_ppro_autoseat(bool New_Value)
{
	_ppro_autoseat = New_Value;
    Reg.Write_OH_RegistryKey("ppro_autoseat", _ppro_autoseat);
}
 
// Formula window location
void CPreferences::Set_formula_x(int New_Value)
{
	_formula_x = New_Value;
	Reg.Write_OH_RegistryKey("formula_x", _formula_x);
}
 
void CPreferences::Set_formula_y(int New_Value)
{
	_formula_y = New_Value;
	Reg.Write_OH_RegistryKey("formula_y", _formula_y);
}
 
void CPreferences::Set_formula_dx(int New_Value)
{
	_formula_dx = New_Value;
	Reg.Write_OH_RegistryKey("formula_dx", _formula_dx);
}
 
void CPreferences::Set_formula_dy(int New_Value)
{
	_formula_dy = New_Value;
	Reg.Write_OH_RegistryKey("formula_dy", _formula_dy);
}
 
// Scraper window location
void CPreferences::Set_scraper_x(int New_Value)
{
	_scraper_x = New_Value;
	Reg.Write_OH_RegistryKey("scraper_x", _scraper_x);    
}
 
void CPreferences::Set_scraper_y(int New_Value)
{
	_scraper_y = New_Value;
	Reg.Write_OH_RegistryKey("scraper_y", _scraper_y);    
}
 
void CPreferences::Set_scraper_dx(int New_Value)
{
	_scraper_dx = New_Value;
	Reg.Write_OH_RegistryKey("scraper_dx", _scraper_dx);    
}
 
void CPreferences::Set_scraper_dy(int New_Value)
{
	_scraper_dy = New_Value;
	Reg.Write_OH_RegistryKey("scraper_dy", _scraper_dy);
}
 
// Main window location
void CPreferences::Set_main_x(int New_Value)
{
	_main_x = New_Value;
	Reg.Write_OH_RegistryKey("main_x", _main_x);
}
 
void CPreferences::Set_main_y(int New_Value)
{
	_main_y = New_Value;
	Reg.Write_OH_RegistryKey("main_y", _main_y);    
}
 
void CPreferences::Set_main_dx(int New_Value)
{
	_main_dx = New_Value;
	Reg.Write_OH_RegistryKey("main_dx", _main_dx);
}
 
void CPreferences::Set_main_dy(int New_Value)
{
	_main_dy = New_Value;
	Reg.Write_OH_RegistryKey("main_dy", _main_dy);
}
 
// Blind locking
void CPreferences::Set_sblind(int New_Value)
{
	_sblind = New_Value;
	Reg.Write_OH_RegistryKey("sblind", _sblind);    
}
 
void CPreferences::Set_bblind(int New_Value)
{
	_bblind = New_Value;
	Reg.Write_OH_RegistryKey("bblind", _bblind);    
}
 
void CPreferences::Set_bbet(int New_Value)
{
	_bbet = New_Value;
	Reg.Write_OH_RegistryKey("bbet", _bbet);    
}
 
void CPreferences::Set_ante(int New_Value)
{
	_ante = New_Value;
	Reg.Write_OH_RegistryKey("ante", _ante);    
}
 
void CPreferences::Set_gametype(int New_Value)
{
	_gametype = New_Value;
	Reg.Write_OH_RegistryKey("gametype", _gametype);
}
 
// Formula editor
void CPreferences::Set_expand_list(bool  New_Value)
{
	_expand_list = New_Value;
	Reg.Write_OH_RegistryKey("expand_list", _expand_list);    
}
 
void CPreferences::Set_expand_std(bool New_Value)
{
	_expand_std = New_Value;
	Reg.Write_OH_RegistryKey("expand_std", _expand_std);    
}
 
void CPreferences::Set_expand_udf(bool New_Value)
{
	_expand_udf = New_Value;
	Reg.Write_OH_RegistryKey("expand_udf", _expand_udf);
}
 
void CPreferences::Set_udf_sort(bool New_Value)
{
	_udf_sort = New_Value;
	Reg.Write_OH_RegistryKey("udf_sort", _udf_sort);    
}
 
void CPreferences::Set_udf_group(bool New_Value)
{
	_udf_group = New_Value;
	Reg.Write_OH_RegistryKey("udf_group", _udf_group);    
}
 
void CPreferences::Set_equal(int New_Value)
{
	_equal = New_Value;
	Reg.Write_OH_RegistryKey("equal", _equal);    
}
 
void CPreferences::Set_precision(int New_Value)
{
	_precision = New_Value;
	Reg.Write_OH_RegistryKey("precision", _precision);    
}
 
// Debug logging
void CPreferences::Set_fdebuglog(bool New_Value)
{
	_fdebuglog = New_Value;
	Reg.Write_OH_RegistryKey("fdebuglog", _fdebuglog);    
}
 
void CPreferences::Set_fdebuglog_myturn(bool New_Value)
{
	_fdebuglog_myturn = New_Value;
	Reg.Write_OH_RegistryKey("fdebuglog_myturn", _fdebuglog_myturn);
}
 
// Misc
// (No method for versus_path,
// as this attribute is nowhere set.) 
void CPreferences::Set_scraper_zoom(int New_Value)
{
	_scraper_zoom = New_Value;
	Reg.Write_OH_RegistryKey("scraper_zoom", _scraper_zoom);
}
 

//
// CPreferences::getter-methods
//

// Analyzer
unsigned int CPreferences::max_opponents()
{
	return _max_opponents;
}
 
// Autoplayer
unsigned int CPreferences::frame_delay()
{
	return _frame_delay;
}
 
unsigned int CPreferences::swag_delay_1()
{
	return _swag_delay_1;
}
 
unsigned int CPreferences::swag_delay_2()
{
	return _swag_delay_2;
}
 
unsigned int CPreferences::swag_delay_3()
{
	return _swag_delay_3;
}
 
unsigned int CPreferences::text_selection_method()
{
	return _text_selection_method;
}
 
unsigned int CPreferences::text_deletion_method()
{
	return _text_deletion_method;
}
 
unsigned int CPreferences::bet_confirmation_method()
{
	return _bet_confirmation_method;
}
 
unsigned int CPreferences::button_click_method()
{
	return _button_click_method;
}
 
bool		CPreferences::ap_auto()
{
	return _ap_auto;
}
 
bool		CPreferences::focus_detect()
{
	return _focus_detect;
}
 
// Dll extension
bool		CPreferences::dll_always_send_state()
{
	return _dll_always_send_state;
}

bool		CPreferences::dll_load_on_startup()
{
	return _dll_load_on_startup;
}
 
CString		CPreferences::dll_name()
{
	return _dll_name;
}
 
// Scraper
unsigned int CPreferences::scrape_delay()
{
	return _scrape_delay;
}
 
// Symbols
double		CPreferences::sym_av_time()
{
	return _sym_av_time;
}
 
CString		CPreferences::sym_handrank_value()
{
	return _sym_handrank_value;
}
 
bool		CPreferences::sym_disable_caching()
{
	return _sym_disable_caching;
}
 
// ICM
double		CPreferences::icm_prize1()
{
	return _icm_prize1;
}
 
double		CPreferences::icm_prize2()
{
	return _icm_prize2;
}
 
double		CPreferences::icm_prize3()
{
	return _icm_prize3;
}
 
double		CPreferences::icm_prize4()
{
	return _icm_prize4;
}
 
// Replay Frames
bool		CPreferences::replay_record()
{
	return _replay_record;
}
 
bool		CPreferences::replay_record_every_change()
{
	return _replay_record_every_change;
}
 
int			CPreferences::replay_max_frames()
{
	return _replay_max_frames;
}
 
// Poker Tracker
CString		CPreferences::pt_ip_addr()
{
	return _pt_ip_addr;
}
 
CString		CPreferences::pt_port()
{
	return _pt_port;
}
 
CString		CPreferences::pt_dbname()
{
	return _pt_dbname;
}
 
CString		CPreferences::pt_user()
{
	return _pt_user;
}
 
CString		CPreferences::pt_pass()
{
	return _pt_pass;
}
 
bool		CPreferences::pt_disable()
{
	return _pt_disable;
}
 
int			CPreferences::pt_update_delay()
{
	return _pt_update_delay;
}
 
int			CPreferences::pt_cache_refresh()
{
	return _pt_cache_refresh;
}
 
// Perl
CString		CPreferences::Perl_default_Formula()
{
	return _Perl_default_Formula;
}
 
CString		CPreferences::Perl_Editor()
{
	return _Perl_Editor;
}
 
bool		CPreferences::Perl_load_default_Formula()
{
	return _Perl_load_default_Formula;
}
 
bool		CPreferences::Perl_load_Interpreter()
{
	return _Perl_load_Interpreter;
}
 
//  PokerChat
bool		CPreferences::Chat_enabled()
{
	return _Chat_enabled;
}
 
int			CPreferences::Chat_min_Delay()
{
	return _Chat_min_Delay;
}
 
int			CPreferences::Chat_random_Delay()
{
	return _Chat_random_Delay;
}
 
// Log Symbol
bool        CPreferences::LogSymbol_enabled()
{
	return _LogSymbol_enabled;
}
 
int         CPreferences::LogSymbol_max_log()
{
	return _LogSymbol_max_log;
}
 
    // Tracing
bool		CPreferences::Trace_enabled()
{
	return _Trace_enabled;
}
 
bool		CPreferences::Trace_functions(unsigned int FunctionNumber)
{
	return _Trace_functions[FunctionNumber];
}

// PPro
CString			CPreferences::ppro_hostname()
{
	return _ppro_hostname;
}
 
CString			CPreferences::ppro_port()
{
	return _ppro_port;
}
 
CString			CPreferences::ppro_username()
{
	return _ppro_username;
}
 
CString			CPreferences::ppro_password()
{
	return _ppro_password;
}
 
int				CPreferences::ppro_x()
{
	return _ppro_x;
}
 
int				CPreferences::ppro_y()
{
	return _ppro_y;
}
 
int				CPreferences::ppro_dx()
{
	return _ppro_dx;
}
 
int				CPreferences::ppro_dy()
{
	return _ppro_dy;
}
 
bool			CPreferences::ppro_handhistory()
{
	return _ppro_handhistory;
}
 
unsigned int	CPreferences::ppro_chips()
{
	return _ppro_chips;
}
 
bool			CPreferences::ppro_autoseat()
{
	return _ppro_autoseat;
}
 
// Main window locations
int				CPreferences::main_x()
{
	return _main_x;
}
 
int				CPreferences::main_y()
{
	return _main_y;
}
 
int				CPreferences::main_dx()
{
	return _main_dx;
}
 
int				CPreferences::main_dy()
{
	return _main_dy;
}
 
// Scraper window locations
int				CPreferences::scraper_x()
{
	return _scraper_x;
}
 
int				CPreferences::scraper_y()
{
	return _scraper_y;
}
 
int				CPreferences::scraper_dx()
{
	return _scraper_dx;
}
 
int				CPreferences::scraper_dy()
{
	return _scraper_dy;
}
 
// Formula window locations
int				CPreferences::formula_x()
{
	return _formula_x;
}
 
int				CPreferences::formula_y()
{
	return _formula_y;
}
 
int				CPreferences::formula_dx()
{
	return _formula_dx;
}
 
int				CPreferences::formula_dy()
{
	return _formula_dy;
}
 
// Blind locking
int				CPreferences::sblind()
{
	return _sblind;
}
 
int				CPreferences::bblind()
{
	return _bblind;
}
 
int				CPreferences::bbet()
{
	return _bbet;
}
 
int				CPreferences::ante()
{
	return _ante;
}
 
int				CPreferences::gametype()
{
	return _gametype;
}
 
// Formula editor
bool			CPreferences::expand_list()
{
	return _expand_list;
}
 
bool			CPreferences::expand_std()
{
	return _expand_std;
}
 
bool			CPreferences::expand_udf()
{
	return _expand_udf;
}
 
bool			CPreferences::udf_sort()
{
	return _udf_sort;
}
 
bool			CPreferences::udf_group()
{
	return _udf_group;
}
 
int				CPreferences::equal()
{
	return _equal;
}
 
int				CPreferences::precision()
{
	return _precision;
}
 
// Debug logging
bool			CPreferences::fdebuglog()
{
	return _fdebuglog;
}
 
bool			CPreferences::fdebuglog_myturn()
{
	return _fdebuglog_myturn;
}
 
// Misc
int				CPreferences::scraper_zoom()
{
	return _scraper_zoom;
}
 
CString			CPreferences::versus_path()
{
	return _versus_path;
}
 

//
// Private method to initialize the default values
//
void CPreferences::Init_Defaults(void)
{
    __SEH_HEADER

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
    _Perl_default_Formula = "";
    //  Unfortunately "%SystemRoot%\\notepad.exe" does not work;
    //    It seems, this kind of filename substitution is only
    //    supported by the DOS shell, not by the Windows API.
    //    Therefore we use a hardcoded filename.
    _Perl_Editor = "C:\\Windows\\notepad.exe";
    _Perl_load_default_Formula = false;
    _Perl_load_Interpreter = false;

    //  PokerChat
    _Chat_enabled = false;
    //  Just a security measure against crazy bot formulas...
    _Chat_min_Delay = 600;      //  seconds
    _Chat_random_Delay = 3000;  //  seconds;

    // log$ loggin
    _LogSymbol_enabled = false;
    _LogSymbol_max_log = 5;

	_Trace_enabled = false;
	for (int i=0;i<nTraceFunctions;i++)
		_Trace_functions[i] = false;

    _versus_path = "";

    __SEH_LOGFATAL("Registry::init_Defaults : \n");
}

//
// Private methods to access the registry
//

void CPreferences::Read_From_Registry()
{
	__SEH_HEADER

	Init_Defaults();
	{
        // Main window location and size
        Reg.Read_OH_RegistryKey("main_x", &_main_x);
        Reg.Read_OH_RegistryKey("main_y", &_main_y);
        Reg.Read_OH_RegistryKey("main_dx", &_main_dx);
        Reg.Read_OH_RegistryKey("main_dy", &_main_dy);

        // Formula window location and size
        Reg.Read_OH_RegistryKey("formula_x", &_formula_x);
        Reg.Read_OH_RegistryKey("formula_y", &_formula_y);
        Reg.Read_OH_RegistryKey("formula_dx", &_formula_dx);
        Reg.Read_OH_RegistryKey("formula_dy", &_formula_dy);

        // ppro window location and size
        Reg.Read_OH_RegistryKey("ppro_x", &_ppro_x);
        Reg.Read_OH_RegistryKey("ppro_y", &_ppro_y);
        Reg.Read_OH_RegistryKey("ppro_dx", &_ppro_dx);
        Reg.Read_OH_RegistryKey("ppro_dy", &_ppro_dy);

        // scraper window location and size
        Reg.Read_OH_RegistryKey("scraper_x", &_scraper_x);
        Reg.Read_OH_RegistryKey("scraper_y", &_scraper_y);
        Reg.Read_OH_RegistryKey("scraper_dx", &_scraper_dx);
        Reg.Read_OH_RegistryKey("scraper_dy", &_scraper_dy);

        // formula editor
        Reg.Read_OH_RegistryKey("precision", &_precision);
        Reg.Read_OH_RegistryKey("equal", &_equal);
        Reg.Read_OH_RegistryKey("udf_sort", &_udf_sort);
        Reg.Read_OH_RegistryKey("udf_group", &_udf_group);        
        Reg.Read_OH_RegistryKey("expand_std", &_expand_std);
        Reg.Read_OH_RegistryKey("expand_list", &_expand_list);
        Reg.Read_OH_RegistryKey("expand_udf", &_expand_udf);

        // prefs - analyzer
        Reg.Read_OH_RegistryKey("max_opponents", &_max_opponents);

        // prefs - autoplayer
        Reg.Read_OH_RegistryKey("frame_delay", &_frame_delay);
        Reg.Read_OH_RegistryKey("swag_delay_1", &_swag_delay_1);
        Reg.Read_OH_RegistryKey("swag_delay_2", &_swag_delay_2);
        //  If a key "swag_delay" exists, use this as "swag_delay_3",
        //    if not defined in another way (checked below).
        //    (WH backward compatibility.)
        Reg.Read_OH_RegistryKey("swag_delay", &_swag_delay_3);
        Reg.Read_OH_RegistryKey("swag_delay_3", &_swag_delay_3);
        Reg.Read_OH_RegistryKey("swag_text_selection", &_text_selection_method);
        Reg.Read_OH_RegistryKey("swag_text_deletion", &_text_deletion_method);
        Reg.Read_OH_RegistryKey("swag_bet_confirmation", &_bet_confirmation_method);
        Reg.Read_OH_RegistryKey("button_click_method", &_button_click_method);
        Reg.Read_OH_RegistryKey("auto", &_ap_auto);
        /*Reg.Read_OH_RegistryKey("focus_detect", &_focus_detect);*/

        // prefs - dll extension
        Reg.Read_OH_RegistryKey("dll_always_send_state", &_dll_always_send_state);
        Reg.Read_OH_RegistryKey("load_dll_on_startup", &_dll_load_on_startup);
        Reg.Read_OH_RegistryKey("dll_name", &_dll_name);

        // prefs - scraper
        Reg.Read_OH_RegistryKey("scrape_delay", &_scrape_delay);

        // prefs - symbols
        Reg.Read_OH_RegistryKey("avtime", &_sym_av_time);
        Reg.Read_OH_RegistryKey("handrank_value", &_sym_handrank_value);
        Reg.Read_OH_RegistryKey("disable_caching", &_sym_disable_caching);

        // Prefs - poker tracker
        Reg.Read_OH_RegistryKey("pt_disable", &_pt_disable);
        Reg.Read_OH_RegistryKey("pt_ip_addr", &_pt_ip_addr);
        Reg.Read_OH_RegistryKey("pt_port", &_pt_port);
        Reg.Read_OH_RegistryKey("pt_user", &_pt_user);
        Reg.Read_OH_RegistryKey("pt_pass", &_pt_pass);
        Reg.Read_OH_RegistryKey("pt_dbname", &_pt_dbname);
        Reg.Read_OH_RegistryKey("pt_updatedelay", &_pt_update_delay);
        Reg.Read_OH_RegistryKey("pt_cacherefresh", &_pt_cache_refresh);

        // prefs - ICM
        Reg.Read_OH_RegistryKey("icm_prize1", &_icm_prize1);
        Reg.Read_OH_RegistryKey("icm_prize2", &_icm_prize2);
        Reg.Read_OH_RegistryKey("icm_prize3", &_icm_prize3);
        Reg.Read_OH_RegistryKey("icm_prize4", &_icm_prize4);

        // Prefs - Replay frames
        Reg.Read_OH_RegistryKey("replay_record", &_replay_record);
        Reg.Read_OH_RegistryKey("replay_record_every_change", &_replay_record_every_change);
        Reg.Read_OH_RegistryKey("replay_max_frames", &_replay_max_frames);

        // poker Pro
        Reg.Read_OH_RegistryKey("ppro_hostname", &_ppro_hostname);
        Reg.Read_OH_RegistryKey("ppro_port", &_ppro_port);
        Reg.Read_OH_RegistryKey("ppro_username", &_ppro_username);
        Reg.Read_OH_RegistryKey("ppro_password", &_ppro_password);
        Reg.Read_OH_RegistryKey("ppro_handhistory", &_ppro_handhistory);
        Reg.Read_OH_RegistryKey("ppro_chips", &_ppro_chips);
        Reg.Read_OH_RegistryKey("ppro_autoseat", &_ppro_autoseat);

        // blind locking
        Reg.Read_OH_RegistryKey("sblind", &_sblind);
        Reg.Read_OH_RegistryKey("bblind", &_bblind);
        Reg.Read_OH_RegistryKey("bbet", &_bbet);
        Reg.Read_OH_RegistryKey("ante", &_ante);
        Reg.Read_OH_RegistryKey("gametype", &_gametype);

        // f$debug logging
        Reg.Read_OH_RegistryKey("fdebuglog", &_fdebuglog);
        Reg.Read_OH_RegistryKey("fdebuglog_myturn", &_fdebuglog_myturn);
        // scraper zoom level
        Reg.Read_OH_RegistryKey("scraper_zoom", &_scraper_zoom);

        // Perl
		Reg.Read_OH_RegistryKey("Perl_Editor", &_Perl_Editor);
		Reg.Read_OH_RegistryKey("Perl_load_default_Formula", &_Perl_load_default_Formula);
		Reg.Read_OH_RegistryKey("Perl_load_Interpreter", &_Perl_load_Interpreter);
		Reg.Write_OH_RegistryKey("Chat_enabled", _Chat_enabled);

        // PokerChat
        Reg.Read_OH_RegistryKey("Chat_enabled", &_Chat_enabled);
        Reg.Read_OH_RegistryKey("Chat_min_Delay", &_Chat_min_Delay);
        Reg.Read_OH_RegistryKey("Chat_random_Delay", &_Chat_random_Delay);

        // log$ logging
        Reg.Read_OH_RegistryKey("LogSymbol_enabled", &_LogSymbol_enabled);
        Reg.Read_OH_RegistryKey("LogSymbol_max_log", &_LogSymbol_max_log);

        Reg.Read_OH_RegistryKey("Trace_enabled", &_Trace_enabled);
		CString regValue;
		for (int i=0;i<nTraceFunctions;i++) {
			regValue.Format("Trace_functions%d", i+1);
			Reg.Read_OH_RegistryKey(regValue, &_Trace_functions[i]);
		}
		
		// versus path
        Reg.Read_OH_RegistryKey("versus", &_versus_path);
    }

    __SEH_LOGFATAL("Registry::read_reg : \n");
}


CPreferences *p_Preferences;