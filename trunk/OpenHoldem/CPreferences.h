#ifndef OPENHOLDEM_CPREFERENCES_H_
#define OPENHOLDEM_CPREFERENCES_H_

#include "Registry.h"

class CPreferences
{
	//
	// Constructor and destructor
	//
public:
	CPreferences();
	~CPreferences();

	//
	// Public setter-methods
	//
public:
	// Analyzer
	void Set_max_opponents(unsigned int new_Value);
	// Autoplayer
	void Set_frame_delay(unsigned int new_Value);
	void Set_swag_delay_1(unsigned int new_Value);
	void Set_swag_delay_2(unsigned int new_Value);
	void Set_swag_delay_3(unsigned int new_Value);
	void Set_text_selection_method(unsigned int new_Value);
	void Set_text_deletion_method(unsigned int new_Value);
	void Set_bet_confirmation_method(unsigned int new_Value);
	void Set_button_click_method(unsigned int new_Value);
	void Set_ap_auto(bool new_Value);
	void Set_focus_detect(bool new_Value);
	// Dll extension
	void Set_dll_always_send_state(bool new_Value);
	void Set_dll_load_on_startup(bool new_Value);
	void Set_dll_name(CString new_Value);
	// Scraper
	void Set_scrape_delay(unsigned intnew_Value);
	// Symbols
	void Set_sym_av_time(double new_Value);
	void Set_sym_handrank_value(CString new_Value);
	void Set_sym_disable_caching(bool new_Value);
	// ICM
	void Set_icm_prize1(double new_Value);
	void Set_icm_prize2(double new_Value);
	void Set_icm_prize3(double new_Value);
	void Set_icm_prize4(double new_Value);
	// Replay Frames
	void Set_replay_record(bool new_Value);
	void Set_replay_record_every_change(bool new_Value);
	void Set_replay_max_frames(int new_Value);
	// Poker Tracker
	void Set_pt_ip_addr(CString new_Value);
	void Set_pt_port(CString new_Value);
	void Set_pt_dbname(CString new_Value);
	void Set_pt_user(CString new_Value);
	void Set_pt_pass(CString new_Value);
	void Set_pt_disable(bool new_Value);
	void Set_pt_update_delay(int new_Value);
	void Set_pt_cache_refresh(int new_Value);
	// Perl
	void Set_Perl_default_Formula(CString new_Value);
	void Set_Perl_Editor(CString new_Value);
	void Set_Perl_load_default_Formula(bool new_Value);
	void Set_Perl_load_Interpreter(bool new_Value);
	//  PokerChat
	void Set_Chat_enabled(bool new_Value);
	void Set_Chat_min_Delay(int new_Value);
	void Set_Chat_random_Delay(int new_Value);
    // Log Symbol
    void Set_LogSymbol_enabled(bool new_Value);
    void Set_LogSymbol_max_log(bool new_Value);
    // Tracing
	void Set_Trace_enabled(bool new_Value);
	void Set_Trace_functions(unsigned int FunctionNumber, bool Flag);
	// PPro
	void Set_ppro_hostname(CString New_Value);
	void Set_ppro_port(CString New_Value);
	void Set_ppro_username(CString New_Value);
	void Set_ppro_password(CString New_Value);
	void Set_ppro_x(int New_Value);
	void Set_ppro_y(int New_Value);
	void Set_ppro_dx(int New_Value);
	void Set_ppro_dy(int New_Value);
	void Set_ppro_handhistory(bool New_Value);
	void Set_ppro_chips(unsigned int New_Value);
	void Set_ppro_autoseat(bool New_Value);
	// Formula window location
	void Set_formula_x(int New_Value);
	void Set_formula_y(int New_Value);
	void Set_formula_dx(int New_Value);
	void Set_formula_dy(int New_Value);
	// Scraper window location
	void Set_scraper_x(int New_Value);
	void Set_scraper_y(int New_Value);
	void Set_scraper_dx(int New_Value);
	void Set_scraper_dy(int New_Value);
	// Main window location
	void Set_main_x(int New_Value);
	void Set_main_y(int New_Value);
	void Set_main_dx(int New_Value);
	void Set_main_dy(int New_Value);
	// Blind locking
	void Set_sblind(int New_Value);
    void Set_bblind(int New_Value);
    void Set_bbet(int New_Value);
    void Set_ante(int New_Value);
    void Set_gametype(int New_Value);
	// Formula editor
	void Set_expand_list(bool  New_Value);
	void Set_expand_std(bool New_Value);
	void Set_expand_udf(bool New_Value);
	void Set_udf_sort(bool New_Value);
	void Set_udf_group(bool New_Value);
	void Set_equal(int New_Value);
	void Set_precision(int New_Value);
	// Debug logging
	void Set_fdebuglog(bool New_Value);
	void Set_fdebuglog_myturn(bool New_Value);
	// Misc
	// (No method for versus_path,
	// as this attribute is nowhere set.) 
	void Set_scraper_zoom(int New_Value);


	//
	// Public getter-methods
	//
public:
	// Analyzer
	unsigned int	max_opponents();
	// Autoplayer
	unsigned int	frame_delay();
	unsigned int	swag_delay_1();
	unsigned int	swag_delay_2();
	unsigned int	swag_delay_3();
	unsigned int	text_selection_method();
	unsigned int	text_deletion_method();
	unsigned int	bet_confirmation_method();
	unsigned int	button_click_method();
	bool			ap_auto();
	bool			focus_detect();
	// Dll extension
	bool			dll_always_send_state();
	bool			dll_load_on_startup();
	CString			dll_name();
	// Scraper
	unsigned int	scrape_delay();
	// Symbols
	double			sym_av_time();
	CString			sym_handrank_value();
	bool			sym_disable_caching();
	// ICM
	double			icm_prize1();
	double			icm_prize2();
	double			icm_prize3();
	double			icm_prize4();
	// Replay Frames
	bool			replay_record();
	bool			replay_record_every_change();
	int				replay_max_frames();
	// Poker Tracker
	CString			pt_ip_addr();
	CString			pt_port();
	CString			pt_dbname();
	CString			pt_user();
	CString			pt_pass();
	bool			pt_disable();
	int				pt_update_delay();
	int				pt_cache_refresh();
	// Perl
	CString			Perl_default_Formula();
	CString			Perl_Editor();
	bool			Perl_load_default_Formula();
	bool			Perl_load_Interpreter();
	//  PokerChat
	bool			Chat_enabled();
	int				Chat_min_Delay();
	int				Chat_random_Delay();
    // Log Symbol
    bool            LogSymbol_enabled();
    int             LogSymbol_max_log();
    // Tracing
	bool			Trace_enabled();
	bool			Trace_functions(unsigned int FunctionNumber);
	// PPro
	CString			ppro_hostname();
	CString			ppro_port();
	CString			ppro_username();
	CString			ppro_password();
	int				ppro_x();
	int				ppro_y();
	int				ppro_dx();
	int				ppro_dy();
	bool			ppro_handhistory();
	unsigned int	ppro_chips();
	bool			ppro_autoseat();
	// Main window locations
	int				main_x();
	int				main_y();
	int				main_dx();
	int				main_dy();
	// Scraper window locations
	int				scraper_x();
	int				scraper_y();
	int				scraper_dx();
	int				scraper_dy();
	// Formula window locations
	int				formula_x();
	int				formula_y();
	int				formula_dx();
	int				formula_dy();	
	// Blind locking
	int				sblind();
    int				bblind();
    int				bbet();
    int				ante();
    int				gametype();
	// Formula editor
	bool			expand_list();
	bool			expand_std();
	bool			expand_udf();
	bool			udf_sort();
	bool			udf_group();
	int				equal();
	int				precision();
	// Debug logging
	bool			fdebuglog();
	bool			fdebuglog_myturn();
	// Misc
	int				scraper_zoom();
	CString			versus_path();	


	//
	// Private methods
	//
private:
	// Initialization
	void Init_Defaults(void);
	// Registry
	void Read_From_Registry(void);

	//
	// Private attributes
	//
private:
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
	CString			_Perl_default_Formula;
	CString			_Perl_Editor;
	bool			_Perl_load_default_Formula;
	bool			_Perl_load_Interpreter;
	//  PokerChat
	bool			_Chat_enabled;
	int				_Chat_min_Delay;
	int				_Chat_random_Delay;
    // Log Symbol
    bool            _LogSymbol_enabled;
    int             _LogSymbol_max_log;
    // Tracing
	bool			_Trace_enabled;
	bool			_Trace_functions[nTraceFunctions];
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
	int				_sblind;
	int				_bblind;
	int				_bbet;
	int				_ante;
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

	//
	// Private registry object for I/O
	//
private:
	Registry		Reg;
};

extern CPreferences *p_Preferences;

#endif // OPENHOLDEM_CPREFERENCES_H_