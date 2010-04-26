#ifndef INC_CSCRAPER_H
#define INC_CSCRAPER_H

#include "../../CTransform/hash/stdint.h"
#include "../CTablemap/CTablemap.h"

struct SLimitInfo
{
	double	sblind;
	double	bblind;
	double	bbet;
	double	ante;
	int		limit;
	double	handnumber;
	double	sb_bb;
	double	bb_BB;
	bool	istournament;

	bool	found_sblind;
	bool	found_bblind;
	bool	found_bbet;
	bool	found_ante;
	bool	found_limit;
	bool	found_handnumber;
	bool	found_sb_bb;
	bool	found_bb_BB;
};

extern class CScraper 
{
public:
	// public functions
	CScraper(void);
	~CScraper(void);
public:
	void DoBasicScrapeButtons();
	void DoBasicScrapeAllPlayerCards();
	int  CompleteBasicScrapeToFullScrape();
public:
	void ClearScrapeAreas(void);
	void CreateBitmaps(void);
	void DeleteBitmaps(void);
	void SetLimitInfo(const SLimitInfo LI);
	const bool GetButtonState(const int button_index);
	const bool IsCommonAnimation(void);
	const bool IsStringAllin(const CString s);
	const bool IsStringRaise(const CString s);
	const bool IsStringCall(const CString s);
	const bool IsStringCheck(const CString s);
	const bool IsStringFold(const CString s);
	const bool IsStringAutopost(const CString s);
	const bool IsStringSitin(const CString s);
	const bool IsStringSitout(const CString s);
	const bool IsStringLeave(const CString s);
	const bool IsStringPrefold(const CString s);
	const bool IsStringSeated(const CString s);
	const bool IsStringActive(const CString s);
	const bool IsStringCardback(const CString s);
	const bool IsStringDealer(const CString s);

public:
	// public accessors
	const char *		title() { return _title; }
	const unsigned int	card_common(int n) { if (n>=0 && n<=4) return _card_common[n]; else return CARD_NOCARD; }
	const unsigned int	card_player(int s, int n) { if (s>=0 && s<=9 && n>=0 && n<=1) return _card_player[s][n]; else return CARD_NOCARD; }
	const unsigned int	card_player_for_display(int n) { if (n>=0 && n<=1) return _card_player_for_display[n]; else return CARD_NOCARD; }
	const CString		seated(int n) { if (n>=0 && n<=9) return _seated[n]; else return "false"; }
	const CString		active(int n) { if (n>=0 && n<=9) return _active[n]; else return "false"; }
	const bool			dealer(int n) { if (n>=0 && n<=9) return _dealer[n]; else return false; }
	const double		player_bet(int n) { if (n>=0 && n<=9) return _player_bet[n]; else return 0.; }
	const CString		player_name(int n) { if (n>=0 && n<=9) return _player_name[n]; else return ""; }
	const bool			name_good_scrape(int n) { if (n>=0 && n<=9) return _name_good_scrape[n]; else return false; }
	const double		player_balance(int n) { if (n>=0 && n<=9) return _player_balance[n]; else return 0.; }
	const double		balance_good_scrape(int n) { if (n>=0 && n<=9) return _balance_good_scrape[n]; else return 0.; }
	const bool			sitting_out(int n) { if (n>=0 && n<=9) return _sitting_out[n]; else return false; }
	const double		pot(int n) { if (n>=0 && n<=9) return _pot[n]; else return 0.; }
	const CString		button_state(int n) { if (n>=0 && n<=9) return _button_state[n]; else return ""; }
	const CString		i86X_button_state(int n) { if (n>=0 && n<=9) return _i86X_button_state[n]; else return ""; }
	const CString		i86_button_state() { return _i86_button_state; }
	const CString		button_label(int n) { if (n>=0 && n<=9) return _button_label[n]; else return ""; }
	const bool			handle_found_at_xy() { return _handle_found_at_xy; }
	const POINT			handle_xy() { return _handle_xy; }
	const SLimitInfo*	s_limit_info() { return &_s_limit_info; }
	const HBITMAP		entire_window_cur() { return _entire_window_cur; }
	const LARGE_INTEGER	clocks_hold() { return _clocks_hold; }

public:
#define ENT CSLock lock(m_critsec);
	// public mutators 
	// Used mainly by PokerPro and the scraper override dll to push their updates into the CScraper structures
	void	set_title(const char *s) { ENT strncpy_s(_title, MAX_WINDOW_TITLE, s, MAX_WINDOW_TITLE);}
	void	set_card_common(const int n, const unsigned int c) { ENT if (n>=0 && n<=4) _card_common[n] = c;}
	void	set_card_player(const int s, const int n, const unsigned int c) { ENT if (s>=0 && s<=9 && n>=0 && n<=1) _card_player[s][n] = c;}
	void	set_card_player_for_display(const int n, const unsigned int c) { ENT if (n>=0 && n<=1) _card_player_for_display[n] = c;}
	void	set_seated(const int n, CString s) { ENT if (n>=0 && n<=9) _seated[n] = s;}
	void	set_active(const int n, CString s) { ENT if (n>=0 && n<=9) _active[n] = s;}
	void	set_dealer(const int n, const bool b) { ENT if (n>=0 && n<=9) _dealer[n] = b;}
	void	set_player_bet(const int n, const double d) { ENT if (n>=0 && n<=9) _player_bet[n] = d;}
	void	set_player_name(const int n, const CString s) { ENT if (n>=0 && n<=9) _player_name[n] = s;}
	void	set_name_good_scrape(const int n, const bool b) { ENT if (n>=0 && n<=9) _name_good_scrape[n] = b;}
	void	set_player_balance(const int n, const double d) { ENT if (n>=0 && n<=9) _player_balance[n] = d;}
	void	set_balance_good_scrape(const int n, const bool b) { ENT if (n>=0 && n<=9) _balance_good_scrape[n] = b;}
	void	set_sitting_out(const int n, const bool b) { ENT if (n>=0 && n<=9) _sitting_out[n] = b;}
	void	set_pot(const int n, const double d) { ENT if (n>=0 && n<=9) _pot[n] = d;}
	void	set_button_state(const int n, const CString s) { ENT if (n>=0 && n<=9) _button_state[n] = s;}
	void	set_i86X_button_state(const int n, const CString s) { ENT if (n>=0 && n<=9) _i86X_button_state[n] = s;}
	void	set_i86_button_state(const CString s) { ENT _i86_button_state = s;}
	void	set_button_label(const int n, const CString s) { ENT if (n>=0 && n<=9) _button_label[n] = s;}
	void	set_sblind(const double d) { ENT _s_limit_info.sblind = d;}
	void	set_bblind(const double d) { ENT _s_limit_info.bblind = d;}
	void	set_bbet(const double d) { ENT _s_limit_info.bbet = d;}
	void	set_ante(const double d) { ENT _s_limit_info.ante = d;}
	void	set_limit(const int i) { ENT _s_limit_info.limit = i;}
	void	set_handnumber(const double d) { ENT _s_limit_info.handnumber = d;}
	void	set_istournament(const bool b) { ENT _s_limit_info.istournament = b;}
	void	set_sb_bb(const double d) { ENT _s_limit_info.sb_bb = d;}
	void	set_bb_BB(const double d) { ENT _s_limit_info.bb_BB = d;}
	void	set_found_sblind(const bool b) { ENT _s_limit_info.found_sblind = b;}
	void	set_found_bblind(const bool b) { ENT _s_limit_info.found_bblind = b;}
	void	set_found_bbet(const bool b) { ENT _s_limit_info.found_bbet = b;}
	void	set_found_ante(const bool b) { ENT _s_limit_info.found_ante = b;}
	void	set_found_limit(const bool b) { ENT _s_limit_info.found_limit = b;}
	void	set_found_handnumber(const bool b) { ENT _s_limit_info.found_handnumber = b;}
	void	set_found_sb_bb(const bool b) { ENT _s_limit_info.found_sb_bb = b;}
	void	set_found_bb_BB(const bool b) { ENT _s_limit_info.found_bb_BB = b;}
	void	set_clocks_hold(const LARGE_INTEGER li) { ENT _clocks_hold = li;}
	void	set_handle_found_at_xy(const bool b) { ENT _handle_found_at_xy = b;}
	void	set_handle_xy(const POINT p) { ENT _handle_xy.x = p.x; _handle_xy.y = p.y;}
	void	set_entire_window_cur(const HBITMAP h) { ENT _entire_window_cur = h;}
	void	delete_entire_window_cur() { ENT DeleteObject(_entire_window_cur);}
#undef ENT
	
private:
	// private variables - use public accessors and public mutators to address these
	char				_title[MAX_WINDOW_TITLE];
	unsigned int		_card_common[5], _card_player[10][2], _card_player_for_display[2];
	bool				_dealer[10], _sitting_out[10], _name_good_scrape[10], _balance_good_scrape[10];
	CString				_seated[10], _active[10], _player_name[10];
	double				_player_balance[10], _player_bet[10], _pot[10];
	CString				_button_state[10], _i86X_button_state[10], _i86_button_state, _button_label[10];
	bool				_handle_found_at_xy;
	POINT				_handle_xy;
	SLimitInfo			_s_limit_info; 
	LARGE_INTEGER		_clocks_hold;			// used for "clocks" symbol
	HBITMAP				_entire_window_cur;

private:
	// private functions and variables - not available via accessors or mutators
	void ScrapeCommonCards();
	void ScrapePlayerCards(const int chair);
	void ScrapeSeated(const int chair);
	void ScrapeActive(const int chair);
	void ScrapeDealer(const int chair);
	void ScrapeName(const int chair);
	void ScrapeBalance(const int chair);
	void ScrapeBet(const int chair);
	void ScrapePots();
	void ScrapeLimits();
	const double GetHandnumFromString(const CString t);
	bool ProcessRegion(RMapCI r_iter);
	const bool BitmapsSame(const HBITMAP HBitmapLeft, const HBITMAP HBitmapRight);
	const double DoChipScrape(RMapCI r_iter);
	const bool IsNumeric(const CString t);

	// for change detection
	bool			_ucf_last;
	unsigned int	_card_common_last[5], _card_player_last[10][2];
	CString			_seated_last[10], _active_last[10];
	bool			_dealer_last[10];
	CString			_name_last[10];
	double			_balance_last[10], _playerbet_last[10];
	CString			_button_state_last[10], _i86X_button_state_last[10], _button_label_last[10], _i86_button_state_last;
	double			_pot_last[10];
	bool			_istournament_last;
	int				_limit_last;
	double			_sblind_last, _bblind_last, _sb_bb_last, _bb_BB_last, _bbet_last, _ante_last;
	double			_handnumber_last;
	char			_title_last[MAX_WINDOW_TITLE];
	int				_scrape_something_changed;
	HBITMAP			_entire_window_last;

	CCritSec		m_critsec;

} *p_scraper;

#endif // INC_CSCRAPER_H