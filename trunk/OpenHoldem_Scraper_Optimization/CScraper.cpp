//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose: Scraping the poker-table and providing access to the scraped data.
//  As the CScraper is low-level and quite large we created 
//  a interface SCraperAccess that provides higher-level accessors
//  like "UserHasCards()".
//  Better use that interface to access scraper-data whenever possible.
//
//***************************************************************************** 

#include "StdAfx.h"
#include "CScraper.h"

#include "Bitmaps.h"
#include "CAutoconnector.h"
#include "CPreferences.h"
#include "CScraperPreprocessor.h"
#include "CStringMatch.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineUserchair.h"
#include "CSymbols.h"
#include "CSymbolEngineTableLimits.h"
#include "..\CTransform\CTransform.h"
#include "..\CTransform\hash\lookup3.h"
#include "debug.h"
#include "MainFrm.h"
#include "OpenHoldem.h"

CScraper *p_scraper = NULL;


#define __HDC_HEADER 		HBITMAP		old_bitmap = NULL; \
	HDC				hdc = GetDC(p_autoconnector->attached_hwnd()); \
	HDC				hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL); \
	HDC				hdcCompatible = CreateCompatibleDC(hdcScreen);

#define __HDC_FOOTER	DeleteDC(hdcCompatible); \
	DeleteDC(hdcScreen); \
	ReleaseDC(p_autoconnector->attached_hwnd(), hdc);


CScraper::CScraper(void)
{
	ClearScrapeAreas();
}

CScraper::~CScraper(void)
{
	ClearScrapeAreas();
}

const double CScraper::player_balance(int n) 
{ 
	RETURN_DEFAULT_IF_OUT_OF_RANGE(n, k_last_chair, 0.0);

	write_log(preferences.debug_scraper(), 
		"[CScraper] player_balance(%i) = %f\n", n, _player_balance[n]);
	return _player_balance[n]; 
}

const CString CScraper::extractHandnumFromString(CString t)
{
	CString resulting_handumber_digits_only;

	// Check for bad parameters
	if (!t || t == "")
		return "";

	for (int i=0; i<t.GetLength(); i++) 
	{
		if (isdigit(t[i]))
		{
			resulting_handumber_digits_only += t[i];
		}
	}
	return resulting_handumber_digits_only;
}

bool CScraper::GetButtonState(CString button_state_as_string)
{
	CString button_state_lower_case = button_state_as_string.MakeLower();
	return (button_state_lower_case.Left(4) == "true" 
		|| button_state_lower_case.Left(2) == "on" 
		|| button_state_lower_case.Left(3) == "yes" 
		|| button_state_lower_case.Left(7) == "checked" 
		|| button_state_lower_case.Left(3) == "lit");
}

bool CScraper::GetButtonState(int button_index)
{
	CString l_button_state = "";
	if (button_index<=9)
	{
		if (p_symbol_engine_autoplayer->ismanual() && button_index == 5)
		{
			// Don't MakeLower our mm_network symbol
			l_button_state = _button_state[button_index];
		}
		else
		{
			// Default
			l_button_state = _button_state[button_index].MakeLower();
		}
		return GetButtonState(l_button_state);
	}
	else if (button_index==86)
	{
		l_button_state = _i86_button_state.MakeLower();
		return GetButtonState(l_button_state);
	}
	else if (button_index>=860)
	{
		l_button_state = _i86X_button_state[button_index-860];
		return GetButtonState(l_button_state);
	}
	return false;
}

bool CScraper::ProcessRegion(RMapCI r_iter)
{
	__HDC_HEADER

	// Get "current" bitmap
	old_bitmap = (HBITMAP) SelectObject(hdcCompatible, r_iter->second.cur_bmp);
	BitBlt(hdcCompatible, 0, 0, r_iter->second.right - r_iter->second.left + 1, 
							    r_iter->second.bottom - r_iter->second.top + 1, 
								hdc, r_iter->second.left, r_iter->second.top, SRCCOPY);
	SelectObject(hdcCompatible, old_bitmap);

	// If the bitmaps are different, then continue on
	if (!BitmapsAreEqual(r_iter->second.last_bmp, r_iter->second.cur_bmp))
	{

		// Copy into "last" bitmap
		old_bitmap = (HBITMAP) SelectObject(hdcCompatible, r_iter->second.last_bmp);
		BitBlt(hdcCompatible, 0, 0, r_iter->second.right - r_iter->second.left + 1, 
									r_iter->second.bottom - r_iter->second.top + 1, 
									hdc, r_iter->second.left, r_iter->second.top, SRCCOPY);
		SelectObject(hdcCompatible, old_bitmap);

		__HDC_FOOTER
		return true;
	}

	__HDC_FOOTER
	return false;
}

bool CScraper::EvaluateRegion(CString name, CString *result)
{
	__HDC_HEADER
	CTransform	trans;
	RMapCI		r_iter = p_tablemap->r$()->find(name.GetString());
	if (r_iter != p_tablemap->r$()->end()) 
	{
		ProcessRegion(r_iter);
		old_bitmap = (HBITMAP) SelectObject(hdcCompatible, r_iter->second.cur_bmp);
		trans.DoTransform(r_iter, hdcCompatible, result);
		SelectObject(hdcCompatible, old_bitmap);
		__HDC_FOOTER
		write_log(preferences.debug_scraper(), "[CScraper] EvaluateRegion(), [%s] -> [%s]\n", 
			name, *result);
		return true;
	}
	// Region does not exist
	__HDC_FOOTER
	return false;
}

/* 
	Full scrape consists of
		ScrapeInterfaceButtons()
		ScrapeActionButtons()
		ScrapeActionButtonLabels()
		ScrapeBetpotButtons()		-- only needed when ActionButtons
		ScrapeSeated()
		ScrapeDealer()
		ScrapeActive()
		ScrapeSlider()				-- only needed for NL/PL, myturn(), certain allinmethods
*/

void CScraper::SetButtonState(CString *button_state, CString text)
{
	if (text != "")
	{
		*button_state = text;	
	}
	else
	{
		*button_state = "false";
	}
}

void CScraper::ScrapeInterfaceButtons()
{
	CString result;
	// Normal i86-button
	if (EvaluateRegion("i86state", &result))
	{
		SetButtonState(&_i86_button_state, result);	
	}
	// i86X-buttons
	CString button_name;
	for (int i=0; i<k_max_number_of_i86X_buttons; i++)
	{
		button_name.Format("i86%dstate", i);
		if (EvaluateRegion(button_name, &result))
		{
			SetButtonState(&_i86X_button_state[i], result);	
		}
	}
}

void CScraper::ScrapeActionButtons()
{
	CString button_name;
	CString result;
	for (int i=0; i<k_max_number_of_buttons; i++)
	{
		button_name.Format("i%dstate", i);
		if (EvaluateRegion(button_name, &result))
		{
			SetButtonState(_button_state, result);	
		}
	}
	// Ugly WinHoldem convention
	// When using ManualMode, grab i5state for PT network 
	if (p_symbol_engine_autoplayer->ismanual())
	{
		p_tablemap->set_network(p_scraper->button_state(5));
	}
}

void CScraper::ScrapeActionButtonLabels()
{
	// WinHoldem defaults
	set_button_label(0, "fold");
	set_button_label(1, "call");
	set_button_label(2, "raise");
	set_button_label(3, "allin");

	CString label;
	CString result;
	for (int i=4; i<k_max_number_of_buttons; i++)
	{
		set_button_label(i, "");
		label.Format("i%dlabel", i);
		if (EvaluateRegion(label, &result))
		{
			set_button_label(i, result);
		}
	}
}

/* ??? Do we need this?
		for (k=0; k<=9 && text == ""; k++)
		{
			s.Format("i%dlabel%d", j, k);
			r_iter = p_tablemap->r$()->find(s.GetString());
			if (r_iter != p_tablemap->r$()->end())
			{
				ProcessRegion(r_iter);
				old_bitmap = (HBITMAP) SelectObject(hdcCompatible, r_iter->second.cur_bmp);
				trans.DoTransform(r_iter, hdcCompatible, &text);
				SelectObject(hdcCompatible, old_bitmap);

				if (text!="")
					set_button_label(j, text);

				write_log(preferences.debug_scraper(), "[CScraper] i%dlabel%d, result %s\n", j, k, text.GetString());
			}
		}
		*/


void CScraper::ScrapeBetpotButtons()
{
	CString button_name;
	CString result;
	for (int i=0; i<k_max_betpot_buttons; i++)
	{
		if (EvaluateRegion(button_name, &result))
		{
			SetButtonState(&_betpot_button_state[i], result);	
		}
	}
}

void CScraper::ScrapeSeated(int chair)
{
	CString seated;
	CString result;

	set_seated(chair, "false");
	seated.Format("p%dseated", chair);
	if (EvaluateRegion(seated, &result))
	{
		if (result != "")
		{
			set_seated(chair, result);
		}
	}
	//!!! only if not seated
	// try u region next uXseated,
	// but only if we didn't get a positive result from the p region
	seated.Format("u%dseated", chair);
	if (EvaluateRegion(seated, &result))
	{
		if (result!="")
		{
			set_seated(chair, result);
		}
	}
}

void CScraper::ScrapeDealer(int chair)
{
	CString dealer;
	CString result;

	set_dealer(chair, false); //!!! Attention; might leave dealer-bit at a later chair! Better clear everything at one place!
	dealer.Format("p%ddealer", chair);
	if (EvaluateRegion(dealer, &result))
	{
		if (p_string_match->IsStringDealer(result))
		{
			set_dealer(chair, true);
			return;
		}
	}
	// Now search for uXdealer
	dealer.Format("u%ddealer", chair);
	if (EvaluateRegion(dealer, &result))
	{
		if (p_string_match->IsStringDealer(result))
		{
			set_dealer(chair, true);
			return;
		}
	}
}

void CScraper::ScrapeActive(int chair)
{
	CString active;
	CString result;

	
	set_active(chair, "false");

	// try p region first pXactive
	active.Format("p%dactive", chair);
	if (EvaluateRegion(active, &result))
	{
		set_active(chair, result);
	}
	// try u region next, but only if we didn't get a key result from the p region
	
	// !!! to be improved
	if (((!p_string_match->IsStringActive(_active[chair]) && p_tablemap->activemethod() != 2) ||
	 (p_string_match->IsStringActive(_active[chair]) && p_tablemap->activemethod() == 2) ) )
	{
		active.Format("u%dactive", chair);
		if (EvaluateRegion(active, &result))
		{
			set_active(chair, result);
		}
	}
}

// !!! Needs serious clean-up
void CScraper::ScrapeSlider()
{
	__HDC_HEADER

	RMapCI handleCI, handleI, slider;
	CString text;
	int j, k;
	POINT handle_xy;

	// find handle
	handleCI = p_tablemap->r$()->find("i3handle");
	slider = p_tablemap->r$()->find("i3slider");

	if (handleCI!=p_tablemap->r$()->end() && slider!=p_tablemap->r$()->end() && _button_state[3]!="false")
	{
		j = slider->second.right - handleCI->second.left;
		text="";
		
		_handle_found_at_xy = false;
		
		for (k=0; k<=j; k++)
		{
			/*!!!
			handleI = p_tablemap->set_r$()->find("i3handle");
			handleI->second.left += k;
			handleI->second.right += k;

			EvaluateRegion("i3handle", &text);

			handleI->second.left -= k;
			handleI->second.right -= k;
			*/
			if (text == "handle" || text == "true") 
				break;
		}

		if (text!="" && k <= j)
		{
			handleCI = p_tablemap->r$()->find("i3handle");
			handle_xy.x = handleCI->second.left + k;
			handle_xy.y = handleCI->second.top;

			set_handle_found_at_xy(true);
			set_handle_xy(handle_xy);
		}

		write_log(preferences.debug_scraper(), "[CScraper] i3handle, result %d,%d\n", handle_xy.x, handle_xy.y);
	}

	__HDC_FOOTER
}

int CScraper::CardString2CardNumber(CString card)
{
	int result;
	StdDeck_stringToCard((char*) card.GetString(), &result);
	return result;
}


// Cares about "everything"
//   * cardfaces
//   * ranks and suits
//   * cardbacks
int CScraper::ScrapeCard(CString name)
{
	// Scrape "normal" cards first
	// This includes "no card" and covers the most common cases
	CString card_str;
	if (EvaluateRegion(name, &card_str))
	{
		if (card_str != "")
		{
			return CardString2CardNumber(card_str);
		}
	}
	// Next: in case of player cards try to scrape card-backs
	if (name[0] == 'p')
	{
		CString cardback = name.Left(2) + "cardback";
		CString cardback_result;
		if (EvaluateRegion(cardback, &cardback_result))
		{
			return CARD_BACK;
		}
	}

	// Otherwise: try to scrape suits and ranks
	CString rank = name + "rank";
	CString suit = name + "suit";
	CString rank_result, suit_result;
	if (EvaluateRegion(rank, &rank_result)
		&& EvaluateRegion(suit, &suit_result))
	{
		if (rank_result == "10") rank_result = "T";

		if (rank_result != "" && suit_result !="")
		{
			card_str = rank_result + suit_result;
			return CardString2CardNumber(card_str);
		}
	}
	// Otherwise: in case of playercards try to scrape uXcardfaceY
	CString uname = name;
	if (name[0] == 'p')
	{
		uname.SetAt(0, 'u');
	}
	if (EvaluateRegion(uname, &card_str))
	{
		if (card_str!="")
		{
			return CardString2CardNumber(card_str);
		}
	}
	// Nothing found
	return CARD_NOCARD;
}

void CScraper::ScrapePlayerCards(int chair)
{
	CString card_name;

	// if (!seated) || !active()) continue
	for (int j=0; j<k_number_of_cards_per_player; j++)
	{
		card_name.Format("p%dcardface%d", chair, j);
		int card = ScrapeCard(card_name);
		set_card_player(chair, j, card);
	}
}

void CScraper::ScrapeCommonCards()
{
	CString card_name;
	for (int i=0; i<k_number_of_community_cards; i++)
	{
		card_name.Format("c0cardface%d", i);
		int card = ScrapeCard(card_name);
		set_card_common(i, card);
	}
}
	
// returns true if common cards are in the middle of an animation
bool CScraper::IsCommonAnimation(void)
{
	int	flop_card_count = 0;

	// Count all the flop cards
	for (int i=0; i<k_number_of_flop_cards; i++)
	{
		if (_card_common[i] != CARD_NOCARD)
		{
			flop_card_count++;
		}
	}

	// If there are some flop cards present,
	// but not all 3 then there is an animation going on
	if (flop_card_count > 0 && flop_card_count < k_number_of_flop_cards)
	{
		return true;
	}
	// If the turn card is present,
	// but not all 3 flop cards are present then there is an animation going on
	else if (_card_common[3] != CARD_NOCARD && flop_card_count != k_number_of_flop_cards)
	{
		return true;
	}
	// If the river card is present,
	// but the turn card isn't
	// OR not all 3 flop cards are present then there is an animation going on
	else if (_card_common[4] != CARD_NOCARD && (_card_common[3] == CARD_NOCARD || flop_card_count != k_number_of_flop_cards))
	{
		return true;
	}
	return false;
}

void CScraper::ScrapeName(int chair) 
{
	RETURN_IF_OUT_OF_RANGE (chair, p_tablemap->LastChair())

	bool				got_new_scrape = false;
	CString				text = "";
	CString				result;
	bool				is_seated = p_string_match->IsStringSeated(_seated[chair]);
	CTransform			trans;
	CString				s = "";

	// Player name uXname
	s.Format("u%dname", chair);
	EvaluateRegion(s, &result);	
	write_log(preferences.debug_scraper(), "[CScraper] u%dname, result %s\n", chair, text.GetString());
	if (result != "")
	{
		set_name_good_scrape(chair, true);
		set_player_name(chair, result);
		return;
	}
	// Player name pXname
	s.Format("p%dname", chair);
	EvaluateRegion(s, &result);
	write_log(preferences.debug_scraper(), "[CScraper] p%dname, result %s\n", chair, text.GetString());
	if (result != "")
	{
		set_name_good_scrape(chair, true);
		set_player_name(chair, result);
		return;
	}
	set_name_good_scrape(chair, false);
}

void CScraper::ScrapeBalance(int chair)
{
	RETURN_IF_OUT_OF_RANGE (chair, p_tablemap->LastChair())

	bool				got_new_scrape = false;
	CString				text = "";
	CTransform			trans;
	int					ret = 0;
	bool				is_seated = p_string_match->IsStringSeated(_seated[chair]);
	CString				s = "";

	int	sym_chair = p_symbol_engine_userchair->userchair();

	got_new_scrape = false;

	set_sitting_out(chair, false);

	// Player name uXbalance
	s.Format("u%dbalance", chair);
	if (EvaluateRegion(s, &text))
	{
		if (p_string_match->IsStringAllin(text))
		{
			got_new_scrape = true;
			text = "0";

			write_log(preferences.debug_scraper(), "[CScraper] u%dbalance, result ALLIN", chair);
		}
		else if (text.MakeLower().Find("out")!=-1)
		{
			set_sitting_out(chair, true);
			set_active(chair, "false");

			write_log(preferences.debug_scraper(), "[CScraper] u%dbalance, result OUT\n", chair);
		}
		else
		{
			CScraperPreprocessor::ProcessBalanceNumbersOnly(&text);
			if (text!="" && p_string_match->IsNumeric(text))
				got_new_scrape = true;
			write_log(preferences.debug_scraper(), "[CScraper] u%dbalance, result %s\n", chair, text.GetString());
		}
	}

	// Player balance pXbalance
	s.Format("p%dbalance", chair);
	if (EvaluateRegion(s, &text))
	{
		if (p_string_match->IsStringAllin(text))
		{
			got_new_scrape = true;
			text = "0";

			write_log(preferences.debug_scraper(), "[CScraper] u%dbalance, result ALLIN\n", chair);
		}
		else if (text.MakeLower().Find("out")!=-1)
		{
			set_sitting_out(chair, true);
			set_active(chair, "false");

			write_log(preferences.debug_scraper(), "[CScraper] u%dbalance, result OUT\n", chair);
		}
		else
		{
			CScraperPreprocessor::ProcessBalanceNumbersOnly(&text);
			if (text!="" && p_string_match->IsNumeric(text))
				got_new_scrape = true;
			write_log(preferences.debug_scraper(), "[CScraper] u%dbalance, result %s\n", chair, text.GetString());
		}
	}

	if (got_new_scrape)
	{
		set_balance_good_scrape(chair, true);
		set_player_balance(chair, trans.StringToMoney(text));
	}
	else
	{
		set_balance_good_scrape(chair, false);
	}
}

void CScraper::ScrapeBet(int chair)
{
	RETURN_IF_OUT_OF_RANGE (chair, p_tablemap->LastChair())

	__HDC_HEADER
	CString				text = "";
	CString				s = "", t="";

	set_player_bet(chair, 0);

	// Player bet pXbet
	s.Format("p%dbet", chair);
	if (EvaluateRegion(s, &text))
	{
		if (text!="")
		{
			CTransform trans;
			set_player_bet(chair, trans.StringToMoney(text));
		}

		write_log(preferences.debug_scraper(), "[CScraper] p%dbet, result %s\n", chair, text.GetString());
	}

	// uXbet
	s.Format("u%dbet", chair);
	if (EvaluateRegion(s, &text))
	{
		if (text!="")
		{
			CTransform trans;
			set_player_bet(chair, trans.StringToMoney(text));
		}

		write_log(preferences.debug_scraper(), "[CScraper] u%dbet, result %s\n", chair, text.GetString());
	}		
		
	// pXchip00
	s.Format("p%dchip00", chair);
	RMapCI r_iter = p_tablemap->r$()->find(s.GetString());
	if (r_iter != p_tablemap->r$()->end() && _player_bet[chair] == 0)
	{
		old_bitmap = (HBITMAP) SelectObject(hdcCompatible, _entire_window_cur);
		double chipscrape_res = DoChipScrape(r_iter);
		SelectObject(hdcCompatible, old_bitmap);

		//???
		t.Format("%.2f", chipscrape_res);
		CScraperPreprocessor::PreprocessMonetaryString(&t);
		set_player_bet(chair, strtod(t.GetString(), 0));

		write_log(preferences.debug_scraper(), "[CScraper] p%dchipXY, result %f\n", chair, _player_bet[chair]);
	}
	__HDC_FOOTER
}

//
// !!! All code below not yet refactored
//
void CScraper::ClearScrapeAreas(void)
{
	for (int i=0; i<k_number_of_community_cards; i++)
		set_card_common(i, CARD_NOCARD);

	for (int i=0; i<k_max_number_of_players; i++)
	{
		set_card_player(i, 0, CARD_NOCARD);
		set_card_player(i, 1, CARD_NOCARD);
		set_seated(i, "false");
		set_active(i, "false");
		set_dealer(i, false);
		set_player_bet(i, 0.0);
		set_player_name(i, "");
		set_name_good_scrape(i, false);
		set_player_balance(i, 0.0);
		set_balance_good_scrape(i, false);
		set_i86X_button_state(i, "false");
		set_button_state(i, "false");
		set_button_label(i, "");
		set_pot(i, 0.);
	}
	set_i86_button_state("false");

	set_button_label(0, "fold");
	set_button_label(1, "call");
	set_button_label(2, "raise");
	set_button_label(3, "allin");

	set_istournament(false); // !! duplicated code?
	set_handnumber(0);
	set_sblind(0);
	set_bblind(0);
	set_bbet(0);
	set_ante(0);
	set_sb_bb(0);
	set_bb_BB(0);
	set_limit(-1);
	set_found_handnumber(false);
	set_found_sblind(false);
	set_found_bblind(false);
	set_found_bbet(false);
	set_found_ante(false);
	set_found_limit(false);
	set_found_sb_bb(false);
	set_found_bb_BB(false);

	// change detection
	for (int i=0; i<k_number_of_community_cards; i++)
	{
		_card_common_last[i] = CARD_NOCARD;
	}
	for (int i=0; i<k_max_number_of_players; i++)
	{
		_card_player_last[i][0] = CARD_NOCARD;
		_card_player_last[i][1] = CARD_NOCARD;
		_seated_last[i] = "false";
		_active_last[i] = "false";
		_dealer_last[i] = false;
		_name_last[i] = "";
		_balance_last[i] = 0;
		_playerbet_last[i] = 0;
		_button_state_last[i] = "";
		_i86X_button_state_last[i] = "";
		_button_label_last[i] = "";
		_pot_last[i] = 0;
	}

	_i86_button_state_last = "";
	_istournament_last = false;
	_limit_last=0;
	_sblind_last=0;
	_bblind_last=0;
	_sb_bb_last=0;
	_bb_BB_last=0;
	_bbet_last=0;
	_ante_last=0;
	_handnumber_last= "";
	strcpy_s(_title_last, MAX_WINDOW_TITLE, "");
}



// returns true if window has changed and we processed the changes, false otherwise
void CScraper::CompleteBasicScrapeToFullScrape()
{
	write_log(preferences.debug_scraper(), "[CScraper] Starting Scraper cadence...\n");
	
	__HDC_HEADER

	// Get bitmap of whole window
	RECT		cr = {0};
	GetClientRect(p_autoconnector->attached_hwnd(), &cr);

	old_bitmap = (HBITMAP) SelectObject(hdcCompatible, _entire_window_cur);
	BitBlt(hdcCompatible, 0, 0, cr.right, cr.bottom, hdc, cr.left, cr.top, SRCCOPY);
	SelectObject(hdcCompatible, old_bitmap);

	// get window title
	_title[0] = '\0';
	GetWindowText(p_autoconnector->attached_hwnd(), _title, MAX_WINDOW_TITLE-1);

	// If the bitmaps are the same, then return now
	// !!! How often does this happen?
	// !!! How costly is the comparison?
	if (BitmapsAreEqual(_entire_window_last, _entire_window_cur) &&
		strcmp(_title, _title_last)==0)
	{
		DeleteDC(hdcCompatible);
		DeleteDC(hdcScreen);
		ReleaseDC(p_autoconnector->attached_hwnd(), hdc);

		write_log(preferences.debug_scraper(), "[CScraper] ...ending Scraper cadence early (no change).\n");
	}

	// Copy into "last" title
	if (strcmp(_title, _title_last)!=0)
	{
		strcpy_s(_title_last, MAX_WINDOW_TITLE, _title);

		set_found_handnumber(false);
		set_found_sblind(false);
		set_found_bblind(false);
		set_found_bbet(false);
		set_found_ante(false);
		set_found_sb_bb(false);
		set_found_bb_BB(false);
		set_found_limit(false);
	}

	// Copy into "last" bitmap
	old_bitmap = (HBITMAP) SelectObject(hdcCompatible, _entire_window_last);
	BitBlt(hdcCompatible, 0, 0, cr.right-cr.left+1, cr.bottom-cr.top+1, hdc, cr.left, cr.top, SRCCOPY);
	SelectObject(hdc, old_bitmap);

	__HDC_FOOTER

	// Common cards
	write_log(preferences.debug_scraper(), "[CScraper] Calling ScrapeCommonCards.\n");
	ScrapeCommonCards();

	// Player information
	for (int i=0; i<=9; i++)
	{
		// ??? Looks strange; should we really scrape cards before seated?
		write_log(preferences.debug_scraper(), "[CScraper] Calling ScrapePlayerCards, chair %d.\n", i);
		ScrapePlayerCards(i);
	
		write_log(preferences.debug_scraper(), "[CScraper] Calling ScrapeSeated, chair %d.\n", i);
		ScrapeSeated(i);
	
		write_log(preferences.debug_scraper(), "[CScraper] Calling ScrapeActive, chair %d.\n", i);
		ScrapeActive(i);
	
		write_log(preferences.debug_scraper(), "[CScraper] Calling ScrapeDealer, chair %d.\n", i);
		ScrapeDealer(i);
	
		write_log(preferences.debug_scraper(), "[CScraper] Calling ScrapeName, chair %d.\n", i);
		ScrapeName(i);
	
		write_log(preferences.debug_scraper(), "[CScraper] Calling ScrapeBet, chair %d.\n", i);
		ScrapeBet(i);
	
		write_log(preferences.debug_scraper(), "[CScraper] Calling ScrapeBalance, chair %d.\n", i);
		ScrapeBalance(i);  // Must come after ScrapeBet, as is dependent on ScrapeBet's results

		if (!p_string_match->IsStringSeated(_seated[i]) && !p_string_match->IsStringActive(_active[i]))
		{
			set_player_name(i, "");
			set_name_good_scrape(i, false);
			set_player_balance(i, 0);
			set_balance_good_scrape(i, false);
		}
	}

	write_log(preferences.debug_scraper(), "[CScraper] Calling ScrapeButtons.\n");
	DoBasicScrapeButtons();		// Buttons

	write_log(preferences.debug_scraper(), "[CScraper] Calling ScrapePots.\n");
	ScrapePots();		// Pots

	write_log(preferences.debug_scraper(), "[CScraper] Calling ScrapeLimits.\n");
	ScrapeLimits();		// limits

	write_log(preferences.debug_scraper(), "[CScraper] ...ending Scraper cadence\n");
}


void CScraper::DoBasicScrapeAllPlayerCards()
{
	write_log(preferences.debug_scraper(), "[CScraper] DoBasicScrapeAllPlayerCards()\n");
	for (int i=0; i<=9; i++)
	{
		write_log(preferences.debug_scraper(), "[CScraper] Calling ScrapePlayerCards, chair %d.\n", i);
		ScrapePlayerCards(i);
	}
}

void CScraper::DoBasicScrapeButtons(void) //!!!
{
	ScrapeInterfaceButtons();
	ScrapeActionButtons();
	ScrapeActionButtonLabels();
	ScrapeBetpotButtons();
}

void CScraper::ScrapePots()
{
	__HDC_HEADER
	int					j = 0;
	CString				text = "";
	CTransform			trans;
	CString				s = "", t="";
	RMapCI				r_iter = p_tablemap->r$()->end();

	for (int j=0; j<k_max_number_of_pots; j++)
		set_pot(j, 0);

	for (int j=0; j<k_max_number_of_pots; j++)
	{
		// r$c0potX
		s.Format("c0pot%d", j);
		if (EvaluateRegion(s, &text))
		{
			CScraperPreprocessor::PreprocessMonetaryString(&text);
			if (text!="")
			{
				set_pot(j, trans.StringToMoney(text));

			}
			write_log(preferences.debug_scraper(), "[CScraper] c0pot%d, result %s\n", j, text.GetString());
		}

		// r$c0potXchip00_index
		s.Format("c0pot%dchip00", j);
		r_iter = p_tablemap->r$()->find(s.GetString());
		if (r_iter != p_tablemap->r$()->end() && _pot[j] == 0)
		{
			ProcessRegion(r_iter);
			//old_bitmap = (HBITMAP) SelectObject(hdcCompatible, r_iter->second.cur_bmp);
			//trans.DoTransform(r_iter, hdcCompatible, &text);
			//SelectObject(hdcCompatible, old_bitmap);

			old_bitmap = (HBITMAP) SelectObject(hdcCompatible, _entire_window_cur);
			double chipscrape_res = DoChipScrape(r_iter);
			SelectObject(hdcCompatible, old_bitmap);

			t.Format("%.2f", chipscrape_res);
			CScraperPreprocessor::PreprocessMonetaryString(&t);
			set_pot(j, strtod(t.GetString(), 0));

			write_log(preferences.debug_scraper(), "[CScraper] c0pot%dchipXY, result %f\n", j, _pot[j]);

			// update the bitmap for second chip position in the first stack
			s.Format("c0pot%dchip01", j);
			r_iter = p_tablemap->r$()->find(s.GetString());
			if (r_iter != p_tablemap->r$()->end())
				ProcessRegion(r_iter);

			// update the bitmap for first chip position in the second stack
			s.Format("c0pot%dchip10", j);
			r_iter = p_tablemap->r$()->find(s.GetString());
			if (r_iter != p_tablemap->r$()->end())
				ProcessRegion(r_iter);
		}
	}

	__HDC_FOOTER
}

void CScraper::ScrapeLimits()
{
	__HDC_HEADER
	CString				handnumber = "";
	bool				istournament = false;
	CString				text = "";
	CString				titletext = "";
	char				c_titletext[MAX_WINDOW_TITLE] = {0};
	bool				got_new_scrape = false, log_blind_change = false;
	CTransform			trans;
	CString				s = "";
	RMapCI				r_iter = p_tablemap->r$()->end();
	SMapCI				s_iter = p_tablemap->s$()->end();

	set_found_sblind(false);
	set_found_bblind(false);
	set_found_bbet(false);
	set_found_ante(false);
	set_found_limit(false);
	set_found_sb_bb(false);
	set_found_bb_BB(false);

	// These persist after scraping the istournament/handnumber regions
	// to seed l_istournament and l_handnumber when we scrape info from
	// the titlebar.  That way if we do not find tournament or handnumber
	// information in the titlebar we won't overwrite the values we scraped
	// from the istournament/handnumber regions.
	istournament = false;
	handnumber = "";

	// c0istournament
	r_iter = p_tablemap->r$()->find("c0istournament");
	if (r_iter != p_tablemap->r$()->end())
	{
		ProcessRegion(r_iter);
		old_bitmap = (HBITMAP) SelectObject(hdcCompatible, r_iter->second.cur_bmp);
		trans.DoTransform(r_iter, hdcCompatible, &text);
		SelectObject(hdcCompatible, old_bitmap);

		if (text == "" || text == "false")
			istournament = false;
		else 
			istournament = true;

		set_istournament(istournament);

		write_log(preferences.debug_scraper(), "[CScraper] c0istournament, result %s\n", text.GetString());
	}

	// r$c0handnumber
	if (EvaluateRegion("c0handnumber", &text))
	{
		if (text!="")
		{
			handnumber = extractHandnumFromString(text);
			set_handnumber(handnumber);
			got_new_scrape = true;
		}

		write_log(preferences.debug_scraper(), "[CScraper] c0handnumber, result %s\n", text.GetString());
	}

	for (int j=0; j<=9; j++)
	{
		// r$c0handnumberX
		s.Format("c0handnumber%d", j);
		if (EvaluateRegion(s, &text))
		{
			if (text!="")
			{
				handnumber = extractHandnumFromString (text);
				set_handnumber(handnumber);
				got_new_scrape = true;
			}

			write_log(preferences.debug_scraper(), "[CScraper] c0handnumber%d, result %s\n", j, text.GetString());
		}
	}
	double l_sblind=0., l_bblind=0., l_bbet=0., l_ante=0., l_sb_bb=0., l_bb_BB=0.;
	int l_limit=0;
	bool l_found_handnumber=false, l_found_sblind=false, l_found_bblind=false;
	bool l_found_bbet=false, l_found_ante=false, l_found_limit=false, l_found_sb_bb=false;
	bool l_found_bb_BB=false;

	// These are scraped from specific regions earlier in this
	// function.  Use the values we scraped (if any) to seed
	// the l_ locals so that we don't blindly overwrite the
	// information we scraped from those specific regions with
	// default values if we can't find them in the titlebar.
	CString l_handnumber = handnumber;
	bool l_istournament = istournament;

	// s$ttlimits - Scrape blinds/stakes/limit info from title text
	s_iter = p_tablemap->s$()->find("ttlimits");
	if (s_iter != p_tablemap->s$()->end())
	{
		GetWindowText(p_autoconnector->attached_hwnd(), c_titletext, MAX_WINDOW_TITLE-1);
		titletext = c_titletext;
	 	
		CScraperPreprocessor::PreprocessTitleString(&titletext);
		trans.ParseStringBSL(
			titletext, s_iter->second.text, NULL,
			&l_handnumber, &l_sblind, &l_bblind, &l_bbet, &l_ante, &l_limit, &l_sb_bb, &l_bb_BB, &l_istournament, 
			&l_found_handnumber, &l_found_sblind, &l_found_bblind, &l_found_bbet, 
			&l_found_ante, &l_found_limit, &l_found_sb_bb, &l_found_bb_BB);

		write_log(preferences.debug_scraper(), "[CScraper] ttlimits, result sblind/bblind/bbet/ante/gametype: %f.2 / %f.2 / %f.2 / %f.2 / %s\n", 
			p_symbol_engine_tablelimits->sblind(), p_symbol_engine_tablelimits->bblind(), p_symbol_engine_tablelimits->bigbet(), 
			p_symbol_engine_tablelimits->ante(), p_symbol_engine_tablelimits->GetGametypeAsString());

		// s$ttlimitsX - Scrape blinds/stakes/limit info from title text
		for (int j=0; j<=9; j++)
		{
			s.Format("ttlimits%d", j);
			s_iter = p_tablemap->s$()->find(s.GetString());
			if (s_iter != p_tablemap->s$()->end())
			{
				GetWindowText(p_autoconnector->attached_hwnd(), c_titletext, MAX_WINDOW_TITLE-1);
				titletext = c_titletext;
	
				CScraperPreprocessor::PreprocessTitleString(&titletext);
				trans.ParseStringBSL(
					titletext, s_iter->second.text, NULL,
					&l_handnumber, &l_sblind, &l_bblind, &l_bbet, &l_ante, &l_limit, &l_sb_bb, &l_bb_BB, &l_istournament, 
					&l_found_handnumber, &l_found_sblind, &l_found_bblind, &l_found_bbet, 
					&l_found_ante, &l_found_limit, &l_found_sb_bb, &l_found_bb_BB);

				write_log(preferences.debug_scraper(), "[CScraper] ttlimits%d, result sblind/bblind/bbet/ante/gametype: %f/%f/%f/%f/%d\n", j,
					p_symbol_engine_tablelimits->sblind(), p_symbol_engine_tablelimits->bblind(), p_symbol_engine_tablelimits->bigbet(), 
					p_symbol_engine_tablelimits->ante(), p_symbol_engine_tablelimits->gametype()); 
			}
		}

		// r$c0limits, s$c0limits
		s.Format("c0limits");
		s_iter = p_tablemap->s$()->find(s.GetString());
		if (EvaluateRegion(s, &text))
		{
			CScraperPreprocessor::PreprocessMonetaryString(&text);
			if (text!="")
			{
				trans.ParseStringBSL(
					text, s_iter->second.text, NULL,
					&l_handnumber, &l_sblind, &l_bblind, &l_bbet, &l_ante, &l_limit, &l_sb_bb, &l_bb_BB, &l_istournament, 
					&l_found_handnumber, &l_found_sblind, &l_found_bblind, &l_found_bbet, 
					&l_found_ante, &l_found_limit, &l_found_sb_bb, &l_found_bb_BB);

				write_log(preferences.debug_scraper(), "[CScraper] c0limits, result sblind/bblind/bbet/ante/gametype: %f/%f/%f/%f/%d\n", 
					p_symbol_engine_tablelimits->sblind(), p_symbol_engine_tablelimits->bblind(), p_symbol_engine_tablelimits->bigbet(), 
					p_symbol_engine_tablelimits->ante(), p_symbol_engine_tablelimits->gametype());
			}
		}

		int j=0;
		for (j=0; j<=9; j++)
		{
			// r$c0limitsX, s$c0limitsX
			s.Format("c0limits%d", j);
			s_iter = p_tablemap->s$()->find(s.GetString());

			if (EvaluateRegion(s, &text))
			{
				CScraperPreprocessor::PreprocessMonetaryString(&text);
				if (text!="")
				{
					trans.ParseStringBSL(text, s_iter->second.text, NULL,
						&l_handnumber, &l_sblind, &l_bblind, &l_bbet, &l_ante, &l_limit, &l_sb_bb, &l_bb_BB, &l_istournament, 
						&l_found_handnumber, &l_found_sblind, &l_found_bblind, &l_found_bbet, 
						&l_found_ante, &l_found_limit, &l_found_sb_bb, &l_found_bb_BB);
				}

				write_log(preferences.debug_scraper(), "[CScraper] c0limits%d, result sblind/bblind/bbet/ante/gametype: %f/%f/%f/%f/%d\n", j,
					p_symbol_engine_tablelimits->sblind(), p_symbol_engine_tablelimits->bblind(), p_symbol_engine_tablelimits->bigbet(), 
					p_symbol_engine_tablelimits->ante(), p_symbol_engine_tablelimits->gametype());
			}

			// save what we just scanned through
			set_handnumber(l_handnumber);
			set_sblind(l_sblind);
			set_bblind(l_bblind);
			set_bbet(l_bbet);
			set_ante(l_ante);
			set_limit(l_limit);
			set_sb_bb(l_sb_bb);
			set_bb_BB(l_bb_BB);
			set_istournament(l_istournament);
			set_found_handnumber(l_found_handnumber);
			set_found_sblind(l_found_sblind);
			set_found_bblind(l_found_bblind);
			set_found_bbet(l_found_bbet);
			set_found_ante(l_found_ante);
			set_found_limit(l_found_limit);
			set_found_sb_bb(l_found_sb_bb);
			set_found_bb_BB(l_found_bb_BB);
		}


		// r$c0smallblind
		s.Format("c0smallblind");
		if (EvaluateRegion(s, &text))
		{
			CScraperPreprocessor::PreprocessMonetaryString(&text);
			if (text!="")
			{
				set_sblind(trans.StringToMoney(text));
				set_found_sblind(true);
			}

			write_log(preferences.debug_scraper(), "[CScraper] c0smallblind, result %s\n", j, text.GetString());
		}

		// r$c0bigblind
		s.Format("c0bigblind");
		if (EvaluateRegion(s, &text))
		{
			CScraperPreprocessor::PreprocessMonetaryString(&text);
			if (text!="")
			{
				set_bblind(trans.StringToMoney(text));
				set_found_bblind(true);
			}

			write_log(preferences.debug_scraper(), "[CScraper] c0bigblind, result %s", j, text.GetString());
		}

		// r$c0bigbet
		s.Format("c0bigbet");
		if (EvaluateRegion(s, &text))
		{
			CScraperPreprocessor::PreprocessMonetaryString(&text);
			if (text!="")
			{
				set_bbet(trans.StringToMoney(text));
				set_found_bbet(true);
			}

			write_log(preferences.debug_scraper(), "[CScraper] c0bigbet, result %s\n", j, text.GetString());
		}

		// r$c0ante
		s.Format("c0ante");
		if (EvaluateRegion(s, &text))
		{
			CScraperPreprocessor::PreprocessMonetaryString(&text);
			if (text!="")
			{
				set_ante(trans.StringToMoney(text));
				set_found_ante(true);
			}

			write_log(preferences.debug_scraper(), "[CScraper] c0ante, result %s\n", j, text.GetString());
		}
	}

	__HDC_FOOTER
}

void CScraper::CreateBitmaps(void)
{
	HDC				hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);

	// Whole window
	RECT			cr = {0};
	GetClientRect(p_autoconnector->attached_hwnd(), &cr);
	_entire_window_last = CreateCompatibleBitmap(hdcScreen, cr.right, cr.bottom);
	set_entire_window_cur(CreateCompatibleBitmap(hdcScreen, cr.right, cr.bottom));

	// r$regions
	for (RMapI r_iter=p_tablemap->set_r$()->begin(); r_iter!=p_tablemap->set_r$()->end(); r_iter++)
	{
		int w = r_iter->second.right - r_iter->second.left + 1;
		int h = r_iter->second.bottom - r_iter->second.top + 1;
		r_iter->second.last_bmp = CreateCompatibleBitmap(hdcScreen, w, h);
		r_iter->second.cur_bmp = CreateCompatibleBitmap(hdcScreen, w, h);
	}

	DeleteDC(hdcScreen);
}

void CScraper::DeleteBitmaps(void)
{
	// Whole window
	DeleteObject(_entire_window_last);
	delete_entire_window_cur();

	// Common cards
	for (RMapI r_iter=p_tablemap->set_r$()->begin(); r_iter!=p_tablemap->set_r$()->end(); r_iter++)
	{
		DeleteObject(r_iter->second.last_bmp); r_iter->second.last_bmp=NULL;
		DeleteObject(r_iter->second.cur_bmp); r_iter->second.cur_bmp=NULL;
	}
}

void CScraper::SetLimitInfo(const SLimitInfo LI)
{
	set_handnumber(LI.handnumber);
	set_sblind(LI.sblind);
	set_bblind(LI.bblind);
	set_bbet(LI.bbet);
	set_ante(LI.ante);
	set_limit(LI.limit);
	set_istournament(LI.istournament);
	set_found_sblind(LI.found_sblind);
	set_found_bblind(LI.found_bblind);
	set_found_bbet(LI.found_bbet);
	set_found_ante(LI.found_ante);
	set_found_limit(LI.found_limit);
}

// This is the chip scrape routine
const double CScraper::DoChipScrape(RMapCI r_iter)
{
	HDC				hdc = GetDC(p_autoconnector->attached_hwnd());

	int				j = 0, stackindex = 0, chipindex = 0;
	int				hash_type = 0, pixcount = 0, chipwidth = 0, chipheight = 0;
	int				top = 0, bottom = 0, left = 0, right = 0;
	bool			stop_loop = false;
	uint32_t		*uresult = NULL, hash = 0, pix[MAX_HASH_WIDTH*MAX_HASH_HEIGHT] = {0};
	double			result = 0;
	CString			resstring = "";

	CString			type = "";
	int				vertcount = 0, horizcount = 0;
	RMapCI			r_start = p_tablemap->r$()->end();
	RMapCI			r_vert[10];
	RMapCI			r_horiz[10];
	CString			s = "";

	// Initialize arrays
	for (int j=0; j<10; j++)
	{
		r_vert[j] = p_tablemap->r$()->end();
		r_horiz[j] = p_tablemap->r$()->end();
	}

	// Check for bad parameters
	if (r_iter == p_tablemap->r$()->end())
	{
		ReleaseDC(p_autoconnector->attached_hwnd(), hdc);
		return 0.;
	}

	// figure out if we are dealing with a pot or playerbet here
	if (r_iter->second.name.Mid(0,5)=="c0pot" && r_iter->second.name.Mid(6,4)=="chip")
		type = r_iter->second.name.Mid(0,10);

	else if (r_iter->second.name.Mid(0,1)=="p" && r_iter->second.name.Mid(2,4)=="chip")
		type = r_iter->second.name.Mid(0,6);

	else
	{
		ReleaseDC(p_autoconnector->attached_hwnd(), hdc);
		return 0.;
	}

	// find start, vert stride, and horiz stride regions
	s.Format("%s00", type.GetString());
	r_start = p_tablemap->r$()->find(s.GetString());
	if (r_start == p_tablemap->r$()->end())
	{
		ReleaseDC(p_autoconnector->attached_hwnd(), hdc);
		return 0.;
	}

	for (int j = 1; j<=9; j++)
	{
		s.Format("%s0%d", type.GetString(), j);
		r_vert[j] = p_tablemap->r$()->find(s.GetString());
		if (r_vert[j] != p_tablemap->r$()->end())
			vertcount++;

		s.Format("%s%d0", type.GetString(), j);
		r_horiz[j] = p_tablemap->r$()->find(s.GetString());
		if (r_horiz[j] != p_tablemap->r$()->end())
			horizcount++;
	}

	hash_type = r_start->second.transform[1] - '0';

	// Bitblt the attached windows bitmap into a HDC
	HDC hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);
	HDC hdcCompat = CreateCompatibleDC(hdcScreen);
	RECT rect;
	GetClientRect(p_autoconnector->attached_hwnd(), &rect);
	HBITMAP attached_bitmap = CreateCompatibleBitmap(hdcScreen, rect.right, rect.bottom);
	HBITMAP	old_bitmap = (HBITMAP) SelectObject(hdcCompat, attached_bitmap);
	BitBlt(hdcCompat, 0, 0, rect.right, rect.bottom, hdc, 0, 0, SRCCOPY);
	
	// Get chipscrapemethod option from tablemap, if specified
	CString res = p_tablemap->chipscrapemethod();
	CString cs_method = res.MakeLower();
	int cs_method_x = 0, cs_method_y = 0;
	if (cs_method!="" && cs_method!="all" && cs_method.Find("x")!=-1)
	{
		cs_method_x = strtoul(cs_method.Left(cs_method.Find("x")), NULL, 10);
		cs_method_y = strtoul(cs_method.Mid(cs_method.Find("x")+1), NULL, 10);
	}

	stop_loop = false;
	// loop through horizontal stacks
	for (stackindex=0; stackindex<MAX_CHIP_STACKS && !stop_loop; stackindex++)
	{
		// loop through vertical chips
		for (chipindex=0; chipindex<MAX_CHIPS_PER_STACK && !stop_loop; chipindex++)
		{

			// figure out left, right, top, bottom of next chip to be scraped
			if (vertcount==1)
			{
				top = r_start->second.top + chipindex*(r_vert[1]->second.top - r_start->second.top);
				bottom = r_start->second.bottom + chipindex*(r_vert[1]->second.bottom - r_start->second.bottom);
			}
			else
			{
				if (r_vert[chipindex+1] == p_tablemap->r$()->end())
				{
					stop_loop = true;
				}
				else
				{
					top = r_vert[chipindex+1]->second.top;
					bottom = r_vert[chipindex+1]->second.bottom;
				}
			}

			if (horizcount==1)
			{
				left = r_start->second.left + stackindex*(r_horiz[1]->second.left - r_start->second.left);
				right = r_start->second.right + stackindex*(r_horiz[1]->second.right - r_start->second.right);
			}
			else
			{
				if (r_horiz[stackindex+1] == p_tablemap->r$()->end())
				{
					stop_loop = true;
				}
				else
				{
					left = r_horiz[stackindex+1]->second.left;
					right = r_horiz[stackindex+1]->second.right;
				}
			}

			if (!stop_loop)
			{
				chipwidth = right - left + 1;
				chipheight = bottom - top + 1;

				// calculate hash
				if (hash_type>=1 && hash_type<=3)
				{
					pixcount = 0;
					for (PMapCI p_iter=p_tablemap->p$(hash_type)->begin(); p_iter!=p_tablemap->p$(hash_type)->end(); p_iter++)
					{
							int x = p_iter->second.x;
							int y = p_iter->second.y;

							if (x<chipwidth && y<chipheight)
								pix[pixcount++] = GetPixel(hdcCompat, left + x, top + y);
					}

					if (hash_type==1) hash = hashword(&pix[0], pixcount, HASH_SEED_1);
					else if (hash_type==2) hash = hashword(&pix[0], pixcount, HASH_SEED_2);
					else if (hash_type==3) hash = hashword(&pix[0], pixcount, HASH_SEED_3);
				}

				// lookup hash in h$ records
				HMapCI h_iter = p_tablemap->h$(hash_type)->find(hash);

				// no hash match
				if (h_iter == p_tablemap->h$(hash_type)->end())
				{
					// See if we should stop horiz or vert loops on a non-match
					if (cs_method == "")
					{
						// Stop horizontal scrape loop if chipindex==0 AND a non-match
						if (chipindex==0)
							stackindex = MAX_CHIP_STACKS+1;

						// stop vertical scrape loop on a non-match
						chipindex = MAX_CHIPS_PER_STACK+1;
					}
				}

				// hash match found
				else
				{
					resstring = h_iter->second.name;
					resstring.Remove(',');
					resstring.Remove('$');
					result += atof(resstring.GetString());
				}
			}

			// See if we should stop chip loop due to chipscrapemethod
			if (cs_method!="" && cs_method!="all" && chipindex>=cs_method_y)
				chipindex = MAX_CHIPS_PER_STACK+1;
		}

		// See if we should stop stack loop due to chipscrapemethod
		if (cs_method!="" && cs_method!="all" && stackindex>=cs_method_x)
			stackindex = MAX_CHIP_STACKS+1;

	}

	SelectObject(hdcCompat, old_bitmap);
	DeleteObject(attached_bitmap);
	DeleteDC(hdcCompat);
	DeleteDC(hdcScreen);

	ReleaseDC(p_autoconnector->attached_hwnd(), hdc);
	return result;
}

bool CScraper::IsExtendedNumberic(CString text)
{
	return false;
}

#undef __HDC_HEADER 
#undef __HDC_FOOTER