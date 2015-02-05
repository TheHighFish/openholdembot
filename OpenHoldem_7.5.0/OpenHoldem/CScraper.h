//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Scraping the poker-table and providing access to the scraped data.
//  As the CScraper is low-level and quite large we created 
//  a interface SCraperAccess that provides higher-level accessors
//  like "UserHasCards()".
//  Better use that interface to access scraper-data whenever possible.
//
//******************************************************************************

#ifndef INC_CSCRAPER_H
#define INC_CSCRAPER_H

#include "../../CTransform/hash/stdint.h"
#include "../CTablemap/CTablemap.h"
#include "MagicNumbers.h"
#include "NumericalFunctions.h"

struct SLimitInfo
{
	double	sblind;
	double	bblind;
	double	bbet;
	double	ante;
	int		  limit;
	// Handnumber should be a string, as
	//   * it may contain characters
	//   * its lengths my exceed the precision of double
	CString	handnumber;
	double	sb_bb;
	double	bb_BB;
  double  buyin;
  bool    is_final_table;
};

class CScraper 
{
friend class CLazyScraper;

public:
	// public functions
	CScraper(void);
	~CScraper(void);
public:
	// public accessors
	const char*			title()                    { return _title; }
	const CString		seated(int n)              { RETURN_DEFAULT_IF_OUT_OF_RANGE(n, k_last_chair, "false") return _seated[n]; }
	const CString		active(int n)              { RETURN_DEFAULT_IF_OUT_OF_RANGE(n, k_last_chair, "false") return _active[n]; }
	const bool			dealer(int n)              { RETURN_DEFAULT_IF_OUT_OF_RANGE(n, k_last_chair, false)   return _dealer[n]; }
	const double		player_bet(int n)          { RETURN_DEFAULT_IF_OUT_OF_RANGE(n, k_last_chair, 0.0)     return _player_bet[n]; }
	const bool			sitting_out(int n)         { RETURN_DEFAULT_IF_OUT_OF_RANGE(n, k_last_chair, false)   return _sitting_out[n]; }
	const double		pot(int n)                 { RETURN_DEFAULT_IF_OUT_OF_RANGE(n, k_last_chair, 0.0)     return _pot[n]; }
	const CString		button_state(int n)        { RETURN_DEFAULT_IF_OUT_OF_RANGE(n, k_last_chair, "")      return _button_state[n]; }
public:	
	CString		i86_button_state()         { return _i86_button_state; }
	CString		i86X_button_state(int n)   { RETURN_DEFAULT_IF_OUT_OF_RANGE(n, (k_max_number_of_i86X_buttons-1), "") return _i86X_button_state[n]; }
	CString		betpot_button_state(int n) { RETURN_DEFAULT_IF_OUT_OF_RANGE(n, (k_max_betpot_buttons-1), "")         return _betpot_button_state[n]; }
public:
	const CString		button_label(int n) { if (n>=0 && n<=9) return _button_label[n]; else return ""; }
	const bool			handle_found_at_xy() { return _handle_found_at_xy; }
	const POINT			handle_xy() { return _handle_xy; }
	const SLimitInfo*	s_limit_info() { return &_s_limit_info; }
	const HBITMAP		entire_window_cur() { return _entire_window_cur; }
public: 
	void ClearScrapeAreas(void);
	void CreateBitmaps(void);
	void DeleteBitmaps(void);
	bool GetButtonState(const int button_index);
	bool GetButtonState(CString button_state_as_string);
	bool IsCommonAnimation();
	bool IsIdenticalScrape();
protected:
	void ScrapeDealer();
	void ScrapeActionButtons();
	void ScrapeActionButtonLabels();
	void ScrapeInterfaceButtons();
	void ScrapeBetpotButtons();
	void ClearAllPlayerNames();
	void ScrapeName(const int chair);
	void ScrapePlayerCards(int chair);
	void ScrapeSlider();
	void ScrapeCommonCards();
	void ScrapeSeatedActive();
	void ScrapeBetsBalances();
	void ScrapeAllPlayerCards();
private:
	void ScrapeSeated(int chair);
	void ScrapeActive(int chair);
private:
	int ScrapeCard(CString name);
  int ScrapeCardback(CString base_name);
  int ScrapeCardByRankAndSuit(CString base_name);
  int ScrapeCardface(CString base_name);
  int ScrapeNoCard(CString base_name);
private:
	int CardString2CardNumber(CString card);
private:
	// private functions and variables - not available via accessors or mutators
  double ScrapeUPBalance(int chair, char scrape_u_else_p);
	void ScrapeBalance(const int chair);
	void ScrapeBet(const int chair);
	void ScrapePots();
	void ScrapeLimits();
	const CString extractHandnumFromString(const CString t);
	const double DoChipScrape(RMapCI r_iter);
private:
	bool ProcessRegion(RMapCI r_iter);
	bool EvaluateRegion(CString name, CString *result);
  void EvaluateTrueFalseRegion(bool *result, const CString name);
  bool EvaluateNumericalRegion(double *result, const CString name);
	bool IsExtendedNumberic(CString text);
	CString ProcessBalanceNumbersOnly(CString balance_and_or_potential_text);
private:
	void SetButtonState(CString *button_state, CString text);
  void ResetLimitInfo();
private: 
#define ENT CSLock lock(m_critsec);
	// public mutators 
	// Used mainly by the scraper override dll to push their updates into the CScraper structures
	void	set_title(const char *s) { ENT strncpy_s(_title, MAX_WINDOW_TITLE, s, MAX_WINDOW_TITLE);}
	void	set_seated(const int n, CString s) { ENT if (n>=0 && n<=9) _seated[n] = s;}
	void	set_active(const int n, CString s) { ENT if (n>=0 && n<=9) _active[n] = s;}
	void	set_dealer(const int n, const bool b) { ENT if (n>=0 && n<=9) _dealer[n] = b;}
	void	set_player_bet(const int n, const double d) { ENT if (n>=0 && n<=9) _player_bet[n] = d;}
	void	set_sitting_out(const int n, const bool b) { ENT if (n>=0 && n<=9) _sitting_out[n] = b;}
	void	set_pot(const int n, const double d) { ENT if (n>=0 && n<=9) _pot[n] = d;}
	void	set_button_state(const int n, const CString s) { ENT if (n>=0 && n<=9) _button_state[n] = s;}
	void	set_i86X_button_state(const int n, const CString s) { ENT if (n>=0 && n<=9) _i86X_button_state[n] = s;}
	void	set_i86_button_state(const CString s) { ENT _i86_button_state = s;}
	void	set_button_label(const int n, const CString s) { ENT if (n>=0 && n<=9) _button_label[n] = s;}
	void	set_handnumber(const CString s) { ENT _s_limit_info.handnumber = s;}
	void	set_handle_found_at_xy(const bool b) { ENT _handle_found_at_xy = b;}
	void	set_handle_xy(const POINT p) { ENT _handle_xy.x = p.x; _handle_xy.y = p.y;}
	void	set_entire_window_cur(const HBITMAP h) { ENT _entire_window_cur = h;}
	void	delete_entire_window_cur() { ENT DeleteObject(_entire_window_cur);}
#undef ENT

private:
	// Private data -- buttons
	CString _i86_button_state;
	CString	_i86X_button_state[k_max_number_of_i86X_buttons];
	CString	_button_state[k_max_number_of_buttons];
	CString _button_label[k_max_number_of_buttons];
	CString	_betpot_button_state[k_max_betpot_buttons];
private:
	// private variables - use public accessors and public mutators to address these
	char				_title[MAX_WINDOW_TITLE];
	// dealer
	bool				_dealer[k_max_number_of_players];
	// players - sitting out
	bool				_sitting_out[k_max_number_of_players];
	// players - seated / active
	CString				_seated[k_max_number_of_players];
	CString				_active[k_max_number_of_players];
	// players - money
	double				_player_balance[k_max_number_of_players]; 
	double				_player_bet[k_max_number_of_players];
	// pot
	double				_pot[k_max_number_of_pots];
	// i3-slider-handle
	bool				_handle_found_at_xy;
	POINT				_handle_xy;
	// limit
	SLimitInfo			_s_limit_info;
	// misc
	HBITMAP				_entire_window_cur;
	
 private:
	// Old values of last heartbeat
	char			  _title_last[MAX_WINDOW_TITLE];
	HBITMAP			_entire_window_last;
 private:
	CCritSec		m_critsec;
  // Counter of GDI objects (potential memorz leak)
  // Should be 0 at end of program -- will be checked.
  int         _leaking_GDI_objects;
  // Temporary!!!
  int total_region_counter;
  int identical_region_counter;
};

extern CScraper *p_scraper;

#endif // INC_CSCRAPER_H