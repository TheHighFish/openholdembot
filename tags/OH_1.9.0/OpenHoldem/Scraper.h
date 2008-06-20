#pragma once

#include "../../CTransform/hash/stdint.h"
#include "structs_defines.h"

struct SLimitInfo
{
	double	handnumber;
	double	sblind;
	double	bblind;
	double	bbet;
	double	ante;
	int		limit;
	double	sb_bb;
	double	bb_BB;
	bool	istournament;
	bool	found_handnumber;
	bool	found_sblind;
	bool	found_bblind;
	bool	found_bbet;
	bool	found_ante;
	bool	found_limit;
	bool	found_sb_bb;
	bool	found_bb_BB;
};

// Global construction of scraper class
extern class CScraper {
public:
	CScraper(void);
	int DoScrape(void);
	void clear_scrape_areas(void);
	void create_bitmaps(void);
	void delete_bitmaps(void);
	bool get_button_state(int button_index);
	bool is_common_animation(void);
	bool is_string_allin(CString s);
	bool is_string_raise(CString s);
	bool is_string_call(CString s);
	bool is_string_check(CString s);
	bool is_string_fold(CString s);
	bool is_string_autopost(CString s);
	bool is_string_sitin(CString s);
	bool is_string_sitout(CString s);
	bool is_string_leave(CString s);
	bool is_string_prefold(CString s);
	bool is_string_seated(CString s);
	bool is_string_active(CString s);
	bool is_string_cardback(CString s);
	bool is_string_dealer(CString s);

	char				title[512];
	unsigned int		card_common[5];
	unsigned int		card_player[10][2];
	CString				seated[10], active[10];
	bool				dealer[10];
	bool				sittingout[10];
	CString				playername[10];
	bool				name_good_scrape[10];
	double				playerbalance[10];
	double				balance_good_scrape[10];
	double				playerbet[10];
	double				pot[10];
	CString				buttonstate[10];
	CString				i86X_buttonstate[10];
	CString				i86_buttonstate;
	CString				buttonlabel[10];
	int					scrape_something_changed;
	unsigned int		card_player_for_display[2];
	bool                handle_found_at_xy;
	POINT				handle_xy;

	struct SLimitInfo	s_limit_info;

	LARGE_INTEGER		clockshold;			// used for "clocks" symbol
	HBITMAP				entire_window_Last;
	HBITMAP				entire_window_Cur;

	// locked blinds
	bool				blinds_are_locked;
	double				locked_sblind, locked_bblind, locked_bbet, locked_ante;
	int					locked_gametype;

private:
	void scrape_common_cards(HDC hdcCompatible, HDC hdc);
	void scrape_player_cards(int chair, HDC hdcCompatible, HDC hdc);
	void scrape_seated(int chair, HDC hdcCompatible, HDC hdc);
	void scrape_active(int chair, HDC hdcCompatible, HDC hdc);
	void scrape_dealer(int chair, HDC hdcCompatible, HDC hdc);
	void scrape_name(int chair, HDC hdcCompatible, HDC hdc);
	void scrape_balance(int chair, HDC hdcCompatible, HDC hdc);
	void scrape_bet(int chair, HDC hdcCompatible, HDC hdc);
	void scrape_buttons(HDC hdcCompatible, HDC hdc);
	void scrape_pots(HDC hdcCompatible, HDC hdc);
	void scrape_limits(HDC hdcCompatible, HDC hdc);
	double get_handnum_from_string(CString t);
	bool process_region(HDC hdcCompatible, HDC hdc, int r$index);
	bool bitmaps_same(HBITMAP HBitmapLeft, HBITMAP HBitmapRight);
	double do_chip_scrape(HDC hdc, int i);
	bool is_numeric(CString t);

	// for change detection
	bool			ucf_last;
	unsigned int	card_common_last[5], card_player_last[10][2];
	CString			seated_last[10], active_last[10];
	bool			dealer_last[10];
	CString			name_last[10];
	double			balance_last[10], playerbet_last[10];
	CString			buttonstate_last[10], i86X_buttonstate_last[10], buttonlabel_last[10], i86_buttonstate_last;
	double			pot_last[10];
	bool			istournament_last;
	int				limit_last;
	double			sblind_last, bblind_last, sb_bb_last, bb_BB_last, bbet_last, ante_last;
	double			handnumber_last;
	char			title_last[512];

} scraper;
