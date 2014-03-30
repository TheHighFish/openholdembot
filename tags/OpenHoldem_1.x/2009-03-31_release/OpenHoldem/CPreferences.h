#ifndef INC_CPREFERENCES_H_
#define INC_CPREFERENCES_H_

#include "..\CCritSec\CCritSec.h"

extern class CPreferences
{
public:
	// public functions
	CPreferences();
	~CPreferences();

public:
	// public accessors
	// Analyzer
	const unsigned int max_opponents() { return _max_opponents; }
	// Autoplayer
	const unsigned int frame_delay() { return _frame_delay; }
	const unsigned int swag_delay_1() { return _swag_delay_1; }
	const unsigned int swag_delay_2() { return _swag_delay_2; }
	const unsigned int swag_delay_3() { return _swag_delay_3; }
	const unsigned int text_selection_method() { return _text_selection_method; }
	const unsigned int text_deletion_method() { return _text_deletion_method; }
	const unsigned int bet_confirmation_method() { return _bet_confirmation_method; }
	const unsigned int button_click_method() { return _button_click_method; }
	const bool ap_auto() { return _ap_auto; }
	const bool focus_detect() { return _focus_detect; }
	// Dll extension
	const bool dll_always_send_state() { return _dll_always_send_state; }
	const bool dll_load_on_startup() { return _dll_load_on_startup; }
	const CString dll_name() { return _dll_name; }
	// Scraper
	const unsigned int scrape_delay() { return _scrape_delay; }
	// Symbols
	const double sym_av_time() { return _sym_av_time; }
	const CString sym_handrank_value() { return _sym_handrank_value; }
	const bool sym_disable_caching() { return _sym_disable_caching; }
	// ICM
	const double icm_prize1() { return _icm_prize1; }
	const double icm_prize2() { return _icm_prize2; }
	const double icm_prize3() { return _icm_prize3; }
	const double icm_prize4() { return _icm_prize4; }
	// Replay Frames
	const bool replay_record() { return _replay_record; }
	const bool replay_record_every_change() { return _replay_record_every_change; }
	const int replay_max_frames() { return _replay_max_frames; }
	// Poker Tracker
	const CString pt_ip_addr() { return _pt_ip_addr; }
	const CString pt_port() { return _pt_port; }
	const CString pt_dbname() { return _pt_dbname; }
	const CString pt_user() { return _pt_user; }
	const CString pt_pass() { return _pt_pass; }
	const bool pt_disable() { return _pt_disable; }
	const int pt_update_delay() { return _pt_update_delay; }
	const int pt_cache_refresh() { return _pt_cache_refresh; }
	// Perl
	const CString perl_default_formula() { return _perl_default_formula; }
	const CString perl_editor() { return _perl_editor; }
	const bool perl_load_default_formula() { return _perl_load_default_formula; }
	const bool perl_load_interpreter() { return _perl_load_interpreter; }
	//  PokerChat
	const bool chat_enabled() { return _chat_enabled; }
	const int chat_min_delay() { return _chat_min_delay; }
	const int chat_random_delay() { return _chat_random_delay; }
	// Log Symbol
	const bool log_symbol_enabled() { return _log_symbol_enabled; }
	const int log_symbol_max_log() { return _log_symbol_max_log; }
	// Tracing
	const bool trace_enabled() { return _trace_enabled; }
	const bool trace_functions(unsigned int i) { return _trace_functions[i]; }
	// PPro
	const CString ppro_hostname() { return _ppro_hostname; }
	const CString ppro_port() { return _ppro_port; }
	const CString ppro_username() { return _ppro_username; }
	const CString ppro_password() { return _ppro_password; }
	const int ppro_x() { return _ppro_x; }
	const int ppro_y() { return _ppro_y; }
	const int ppro_dx() { return _ppro_dx; }
	const int ppro_dy() { return _ppro_dy; }
	const bool ppro_handhistory() { return _ppro_handhistory; }
	const unsigned int ppro_chips() { return _ppro_chips; }
	const bool ppro_autoseat() { return _ppro_autoseat; }
	// Main window locations
	const int main_x() { return _main_x; }
	const int main_y() { return _main_y; }
	const int main_dx() { return _main_dx; }
	const int main_dy() { return _main_dy; }
	// Scraper window locations
	const int scraper_x() { return _scraper_x; }
	const int scraper_y() { return _scraper_y; }
	const int scraper_dx() { return _scraper_dx; }
	const int scraper_dy() { return _scraper_dy; }
	// Formula window locations
	const int formula_x() { return _formula_x; }
	const int formula_y() { return _formula_y; }
	const int formula_dx() { return _formula_dx; }
	const int formula_dy() { return _formula_dy; }	
	// Blind locking
	const double sblind() { return _sblind; }
	const double bblind() { return _bblind; }
	const double bbet() { return _bbet; }
	const double ante() { return _ante; }
	const int gametype() { return _gametype; }
	// Formula editor
	const bool expand_list() { return _expand_list; }
	const bool expand_std() { return _expand_std; }
	const bool expand_udf() { return _expand_udf; }
	const bool udf_sort() { return _udf_sort; }
	const bool udf_group() { return _udf_group; }
	const int equal() { return _equal; }
	const int precision() { return _precision; }
	// Debug logging
	const bool fdebuglog() { return _fdebuglog; }
	const bool fdebuglog_myturn() { return _fdebuglog_myturn; }
	// Misc
	const int scraper_zoom() { return _scraper_zoom; }
	const CString versus_path() { return _versus_path; }	
	
public:
#define ENT CSLock lock(m_critsec);
	// public mutators

	// Analyzer
	void set_max_opponents(const unsigned int i) { ENT _max_opponents = i; WriteReg("max_opponents", (int) i); }
	// Autoplayer
	void set_frame_delay(const unsigned int i) { ENT _frame_delay = i; WriteReg("frame_delay", (int) i); }
	void set_swag_delay_1(const unsigned int i) { ENT _swag_delay_1 = i; WriteReg("swag_delay_1", (int) i); }
	void set_swag_delay_2(const unsigned int i) { ENT _swag_delay_2 = i; WriteReg("swag_delay_2", (int) i); }
	void set_swag_delay_3(const unsigned int i) { ENT _swag_delay_3 = i; WriteReg("swag_delay_3", (int) i); }
	void set_text_selection_method(const unsigned int i) { ENT _text_selection_method = i; WriteReg("swag_text_selection", (int) i); }
	void set_text_deletion_method(const unsigned int i) { ENT _text_deletion_method = i; WriteReg("swag_text_deletion", (int) i); }
	void set_bet_confirmation_method(const unsigned int i) { ENT _bet_confirmation_method = i; WriteReg("swag_bet_confirmation", (int) i); }
	void set_button_click_method(const unsigned int i) { ENT _button_click_method = i; WriteReg("button_click_method", (int) i); }
	void set_ap_auto(const bool b) { ENT _ap_auto = b; WriteReg("auto", (int) b!=false); }
	void set_focus_detect(const bool b) { ENT _focus_detect = b; WriteReg("focus_detect", (int) b!=false); }
	// Dll extension
	void set_dll_always_send_state(const bool b) { ENT _dll_always_send_state = b; WriteReg("dll_always_send_state", b); }
	void set_dll_load_on_startup(const bool b) { ENT _dll_load_on_startup = b; WriteReg("load_dll_on_startup", b); }
	void set_dll_name(const CString s) { ENT _dll_name = s; WriteReg("dll_name", s); }
	// Scraper
	void set_scrape_delay(const unsigned int i) { ENT _scrape_delay = i; WriteReg("scrape_delay", (int) i); }
	// Symbols
	void set_sym_av_time(const double d) { ENT _sym_av_time = d; WriteReg("avtime", d); }
	void set_sym_handrank_value(const CString s) { ENT _sym_handrank_value = s; WriteReg("handrank_value", s); }
	void set_sym_disable_caching(const bool b) { ENT _sym_disable_caching = b; WriteReg("disable_caching", b); }
	// ICM
	void set_icm_prize1(const double d) { ENT _icm_prize1 = d; WriteReg("icm_prize1", d); }
	void set_icm_prize2(const double d) { ENT _icm_prize2 = d; WriteReg("icm_prize2", d); }
	void set_icm_prize3(const double d) { ENT _icm_prize3 = d; WriteReg("icm_prize3", d); }
	void set_icm_prize4(const double d) { ENT _icm_prize4 = d; WriteReg("icm_prize4", d); }
	// Replay Frames
	void set_replay_record(const bool b) { ENT _replay_record = b; WriteReg("replay_record", b); }
	void set_replay_record_every_change(const bool b) { ENT _replay_record_every_change = b; WriteReg("replay_record_every_change", b); }
	void set_replay_max_frames(const int i) { ENT _replay_max_frames = i; WriteReg("replay_max_frames", i); }
	// Poker Tracker
	void set_pt_ip_addr(const CString s) { ENT _pt_ip_addr = s; WriteReg("pt_ip_addr", s); }
	void set_pt_port(const CString s) { ENT _pt_port = s; WriteReg("pt_port", s); }
	void set_pt_dbname(const CString s) { ENT _pt_dbname = s; WriteReg("pt_dbname", s); }
	void set_pt_user(const CString s) { ENT _pt_user = s; WriteReg("pt_user", s); }
	void set_pt_pass(const CString s) { ENT _pt_pass = s; WriteReg("pt_pass", s); }
	void set_pt_disable(const bool b) { ENT _pt_disable = b; WriteReg("pt_disable", b); }
	void set_pt_update_delay(const int i) { ENT _pt_update_delay = i; WriteReg("pt_updatedelay", i); }
	void set_pt_cache_refresh(const int i) { ENT _pt_cache_refresh = i; WriteReg("pt_cacherefresh", i); }
	// Perl
	void set_perl_default_formula(const CString s) { ENT _perl_default_formula = s; WriteReg("perl_default_formula", s); }
	void set_perl_editor(const CString s) { ENT _perl_editor = s; WriteReg("perl_editor", s); }
	void set_perl_load_default_formula(const bool b) { ENT _perl_load_default_formula = b; WriteReg("perl_load_default_formula", b); }
	void set_perl_load_interpreter(const bool b) { ENT _perl_load_interpreter = b; WriteReg("perl_load_interpreter", b); }
	//  PokerChat
	void set_chat_enabled(const bool b) { ENT _chat_enabled = b; WriteReg("chat_enabled", b); }
	void set_chat_min_delay(const int i) { ENT _chat_min_delay = i; WriteReg("chat_min_delay", i); }
	void set_chat_random_delay(const int i) { ENT _chat_random_delay = i; WriteReg("chat_random_delay", i); }
	// Log Symbol
	void set_log_symbol_enabled(const bool b) { ENT _log_symbol_enabled = b; WriteReg("log_symbol_enabled", b); }
	void set_log_symbol_max_log(const int b) { ENT _log_symbol_max_log = b; WriteReg("log_symbol_max_log", b); }
	// Tracing
	void set_trace_enabled(const bool b) { ENT _trace_enabled = b; WriteReg("trace_enabled", b); }
	void set_trace_functions(const unsigned int i, const bool b) 
	{ ENT _trace_functions[i] = b; CString regValue; regValue.Format("trace_functions%d", i+1);	WriteReg(regValue, b); }
	// PPro
	void set_ppro_hostname(const CString s) { ENT _ppro_hostname = s; WriteReg("ppro_hostname", s); }
	void set_ppro_port(const CString s) { ENT _ppro_port = s; WriteReg("ppro_port", s); }
	void set_ppro_username(const CString s) { ENT _ppro_username = s; WriteReg("ppro_username", s); }
	void set_ppro_password(const CString s) { ENT _ppro_password = s; WriteReg("ppro_password", s); }
	void set_ppro_x(const int i) { ENT _ppro_x = i; WriteReg("ppro_x", i); }
	void set_ppro_y(const int i) { ENT _ppro_y = i; WriteReg("ppro_y", i); }
	void set_ppro_dx(const int i) { ENT _ppro_dx = i; WriteReg("ppro_dx", i); }
	void set_ppro_dy(const int i) { ENT _ppro_dy = i; WriteReg("ppro_dy", i); }
	void set_ppro_handhistory(const bool b) { ENT _ppro_handhistory = b; WriteReg("ppro_handhistory", b); }
	void set_ppro_chips(const unsigned int i) { ENT _ppro_chips = i; WriteReg("ppro_chips", (int) i); }
	void set_ppro_autoseat(const bool b) { ENT _ppro_autoseat = b; WriteReg("ppro_autoseat", b); }
	// Formula window location
	void set_formula_x(const int i) { ENT _formula_x = i; WriteReg("formula_x", i); }
	void set_formula_y(const int i) { ENT _formula_y = i; WriteReg("formula_y", i); }
	void set_formula_dx(const int i) { ENT _formula_dx = i; WriteReg("formula_dx", i); }
	void set_formula_dy(const int i) { ENT _formula_dy = i; WriteReg("formula_dy", i); }
	// Scraper window location
	void set_scraper_x(const int i) { ENT _scraper_x = i; WriteReg("scraper_x", i); }
	void set_scraper_y(const int i) { ENT _scraper_y = i; WriteReg("scraper_y", i); }
	void set_scraper_dx(const int i) { ENT _scraper_dx = i; WriteReg("scraper_dx", i); }
	void set_scraper_dy(const int i) { ENT _scraper_dy = i; WriteReg("scraper_dy", i); }
	// Main window location
	void set_main_x(const int i) { ENT _main_x = i; WriteReg("main_x", i); }
	void set_main_y(const int i) { ENT _main_y = i; WriteReg("main_y", i); }
	void set_main_dx(const int i) { ENT _main_dx = i; WriteReg("main_dx", i); }
	void set_main_dy(const int i) { ENT _main_dy = i; WriteReg("main_dy", i); }
	// Blind locking
	void set_sblind(const double d) { ENT _sblind = d; WriteReg("sblind", d); }
	void set_bblind(const double d) { ENT _bblind = d; WriteReg("bblind", d); }
	void set_bbet(const double d) { ENT _bbet = d; WriteReg("bbet", d); }
	void set_ante(const double d) { ENT _ante = d; WriteReg("ante", d); }
	void set_gametype(const int i) { ENT _gametype = i; WriteReg("gametype", i); }
	// Formula editor
	void set_expand_list(const bool  b) { ENT _expand_list = b; WriteReg("expand_list", b); }
	void set_expand_std(const bool b) { ENT _expand_std = b; WriteReg("expand_std", b); }
	void set_expand_udf(const bool b) { ENT _expand_udf = b; WriteReg("expand_udf", b); }
	void set_udf_sort(const bool b) { ENT _udf_sort = b; WriteReg("udf_sort", b); }
	void set_udf_group(const bool b) { ENT _udf_group = b; WriteReg("udf_group", b); }
	void set_equal(const int i) { ENT _equal = i; WriteReg("equal", i); }
	void set_precision(const int i) { ENT _precision = i; WriteReg("precision", i); }
	// Debug logging
	void set_fdebuglog(const bool b) { ENT _fdebuglog = b; WriteReg("fdebuglog", b); }
	void set_fdebuglog_myturn(const bool b) { ENT _fdebuglog_myturn = b; WriteReg("fdebuglog_myturn", b); }
	// Misc
	// (No method for versus_path,
	// as this attribute is nowhere set.) 
	void set_scraper_zoom(const int i) { ENT _scraper_zoom = i; WriteReg("scraper_zoom", i); }
#undef ENT

private:
	// private variables - use public accessors and public mutators to address these

	// Analyzer
	unsigned int	_max_opponents;
	// Autoplayer
	unsigned int	_frame_delay;
	unsigned int	_swag_delay_1;
	unsigned int	_swag_delay_2;
	unsigned int	_swag_delay_3;
	unsigned int	_text_selection_method;
	unsigned int	_text_deletion_method;
	unsigned int	_bet_confirmation_method;
	unsigned int	_button_click_method;
	bool			_ap_auto;
	bool			_focus_detect;

	// Dll extension
	bool			_dll_always_send_state;
	bool			_dll_load_on_startup;
	CString			_dll_name;
	// Scraper
	unsigned int	_scrape_delay;
	// Symbols
	double			_sym_av_time;
	CString			_sym_handrank_value;
	bool			_sym_disable_caching;
	// ICM
	double			_icm_prize1;
	double			_icm_prize2;
	double			_icm_prize3;
	double			_icm_prize4;
	// Replay Frames
	bool			_replay_record;
	bool			_replay_record_every_change;
	int				_replay_max_frames;
	// Poker Tracker
	CString			_pt_ip_addr;
	CString			_pt_port;
	CString			_pt_dbname;
	CString			_pt_user;
	CString			_pt_pass;
	bool			_pt_disable;
	int				_pt_update_delay;
	int				_pt_cache_refresh;
	// Perl
	CString			_perl_default_formula;
	CString			_perl_editor;
	bool			_perl_load_default_formula;
	bool			_perl_load_interpreter;
	//  PokerChat
	bool			_chat_enabled;
	int				_chat_min_delay;
	int				_chat_random_delay;
	// Log Symbol
	bool			_log_symbol_enabled;
	int				_log_symbol_max_log;
	// Tracing
	bool			_trace_enabled;
	bool			_trace_functions[nTraceFunctions];
	// PPro
	CString			_ppro_hostname;
	bool			_ppro_autoseat;
	CString			_ppro_port;
	CString			_ppro_username;
	CString			_ppro_password;
	bool			_ppro_handhistory;
	int				_ppro_chips;
	// Formula window location
	int				_formula_x;	
	int				_formula_y;
	int				_formula_dx;
	int				_formula_dy;
	// Scraper window location
	int				_scraper_x;
	int				_scraper_y;
	int				_scraper_dx;
	int				_scraper_dy;
	// Main window location
	int				_main_x;
	int				_main_y;
	int				_main_dx;
	int				_main_dy;
	// PPro window location
	int				_ppro_x;
	int				_ppro_y;
	int				_ppro_dx;
	int				_ppro_dy;
	// Blind locking
	double			_sblind;
	double			_bblind;
	double			_bbet;
	double			_ante;
	int				_gametype;
	// Formula editor
	bool			_expand_list;
	bool			_expand_std;
	bool			_expand_udf;
	bool			_udf_sort;
	bool			_udf_group;
	int				_equal;
	int				_precision;
	// Debug logging
	bool			_fdebuglog;
	bool			_fdebuglog_myturn;
	// Misc
	int				_scraper_zoom;
	CString			_versus_path;

private:
	// private functions and variables - not available via accessors or mutators
	void InitDefaults(void);
	void ReadFromRegistry(void);
	void ReadReg(const LPCTSTR registry_key, int *registry_value);
	void ReadReg(const LPCTSTR registry_key, bool *registry_value);
	void ReadReg(const LPCTSTR registry_key, unsigned int *registry_value);
	void ReadReg(const LPCTSTR registry_key, CString *registry_value);
	void ReadReg(const LPCTSTR registry_key, double *registry_value);
	void WriteReg(const LPCTSTR registry_key, const int registry_value);
	void WriteReg(const LPCTSTR registry_key, const CString registry_value);
	void WriteReg(const LPCTSTR registry_key, const double registry_value);

	CCritSec		m_critsec;
	HKEY			_hkey;

} prefs;

#endif // INC_CPREFERENCES_H_