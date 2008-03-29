
class Registry {
public:
	Registry(void);

	void read_reg(void);
	void write_reg(void);
	bool WriteProfileFont(LPCTSTR lpszKey, LPCTSTR lpszVal, CFont& font, CDC* pDC=NULL);
	bool GetProfileFont(LPCTSTR lpszKey, LPCTSTR lpszVal, CFont& font, CDC* pDC=NULL);

	// Window locations and sizes
	int main_x, main_y, main_dx, main_dy;
	int formula_x, formula_y, formula_dx, formula_dy;
	int ppro_x, ppro_y, ppro_dx, ppro_dy;
	int scraper_x, scraper_y, scraper_dx, scraper_dy;

	// formula tab
	int precision, equal;
	bool udf_sort, expand_std, expand_list, expand_udf;

	// prefs - analyzer
	unsigned int	max_opponents;

	// prefs - autoplayer
	unsigned int	frame_delay, swag_delay_1, swag_delay_2, swag_delay_3;
	unsigned int	text_selection_method, text_deletion_method, bet_confirmation_method, button_click_method;
	bool			ap_auto;

	// prefs - dll extension
	bool			dll_always_send_state;
	bool			load_dll_on_startup;
	CString			dll_name;

	// prefs - scraper
	unsigned int	scrape_delay;
	unsigned int	name_scrapes, balance_scrapes;

	// prefs - symbols
	double			avtime;		// table percentages are averaged over this period of time
	CString			handrank_value;
	bool			disable_caching;

	// Prefs - poker tracker
	CString			pt_ip_addr, pt_port, pt_user, pt_pass, pt_dbname;
	int				pt_updatedelay, pt_cacherefresh;
	bool			pt_disable;

	// prefs - ICM
	double			icm_prize1, icm_prize2, icm_prize3, icm_prize4;

	// prefs - replay
	bool			replay_record, replay_record_every_change;
	int				replay_max_frames;

	// PokerPro
	CString			hostname, port, username, password;
	bool			handhistory;
	int				chips;
	bool			autoseat;

	// blind locking
	double			sblind, bblind, bbet, ante;
	int				gametype;

	// f$debug logging
	bool			fdebuglog;
	bool			fdebuglog_myturn;

	// Scraper output dialog - zoom level
	int				scraper_zoom;

	//  2008.02.27 by THF
	//  Preferences for Perl interpreter
	CString			Perl_default_Formula;
	CString			Perl_Editor;
	bool			Perl_load_default_Formula;
	bool			Perl_load_Interpreter;

	//  2008.02.27 by THF
	//  Preferences for PokerChat	
	bool			Chat_enabled;
	int				Chat_min_Delay;
	int				Chat_random_Delay;
};

