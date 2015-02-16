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
//  an interface SCraperAccess that provides higher-level accessors
//  like "UserHasCards()".
//  Better use that interface to access scraper-data whenever possible.
//
//******************************************************************************

#include "StdAfx.h"
#include "CScraper.h"

#include "Bitmaps.h" 
#include "CardFunctions.h"
#include "CAutoconnector.h"
#include "CPreferences.h"
#include "CScraperAccess.h"
#include "CScraperPreprocessor.h"
#include "CStringMatch.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineCasino.h"
#include "CSymbolEngineHistory.h"
#include "CSymbolEngineUserchair.h"
#include "CSymbolEngineTableLimits.h"
#include "CTableState.h"
#include "..\CTransform\CTransform.h"
#include "..\CTransform\hash\lookup3.h"
#include "debug.h"
#include "MainFrm.h"
#include "OpenHoldem.h"

CScraper *p_scraper = NULL;

#define __HDC_HEADER 		HBITMAP		old_bitmap = NULL; \
	HDC				hdc = GetDC(p_autoconnector->attached_hwnd()); \
	HDC				hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL); \
	HDC				hdcCompatible = CreateCompatibleDC(hdcScreen); \
  ++_leaking_GDI_objects;

#define __HDC_FOOTER_ATTENTION_HAS_TO_BE_CALLED_ON_EVERY_FUNCTION_EXIT_OTHERWISE_MEMORY_LEAK \
  DeleteDC(hdcCompatible); \
	DeleteDC(hdcScreen); \
	ReleaseDC(p_autoconnector->attached_hwnd(), hdc); \
  --_leaking_GDI_objects;

CScraper::CScraper(void) {
	ClearScrapeAreas();
  _leaking_GDI_objects = 0;
  total_region_counter = 0;
  identical_region_counter = 0;
}

CScraper::~CScraper(void) {
	ClearScrapeAreas();
  if (_leaking_GDI_objects != 0 ) {
    write_log(k_always_log_errors, "[CScraper] ERROR: leaking GDI objects: %i\n",
      _leaking_GDI_objects);
    write_log(k_always_log_errors, "[CScraper] Please get in contact with the development team\n");
  }
  assert(_leaking_GDI_objects == 0);
  write_log(true, "[CScraper] Total regions scraped %i\n",
    total_region_counter);
  write_log(true, "[CScraper] Identical regions scraped %i\n",
    identical_region_counter);
}

const CString CScraper::extractHandnumFromString(CString t) {
	CString resulting_handumber_digits_only;
  // Check for bad parameters
	if (!t || t == "") return "";
  for (int i=0; i<t.GetLength(); i++) {
		if (isdigit(t[i]))
		{
			resulting_handumber_digits_only += t[i];
		}
	}
	return resulting_handumber_digits_only;
}

bool CScraper::GetButtonState(CString button_state_as_string) {
	CString button_state_lower_case = button_state_as_string.MakeLower();
	return (button_state_lower_case.Left(4) == "true" 
		|| button_state_lower_case.Left(2) == "on" 
		|| button_state_lower_case.Left(3) == "yes" 
		|| button_state_lower_case.Left(7) == "checked" 
		|| button_state_lower_case.Left(3) == "lit");
}

bool CScraper::GetButtonState(int button_index) {
	CString l_button_state = "";
	if (button_index<=9) {
		if (p_symbol_engine_casino->ConnectedToManualMode() && button_index == 5)	{
			// Don't MakeLower our mm_network symbol
			l_button_state = _button_state[button_index];
		}	else {
			// Default
			l_button_state = _button_state[button_index].MakeLower();
		}
		return GetButtonState(l_button_state);
	}	else if (button_index==86) {
		l_button_state = _i86_button_state.MakeLower();
		return GetButtonState(l_button_state);
	}	else if (button_index>=860)	{
		l_button_state = _i86X_button_state[button_index-860];
		return GetButtonState(l_button_state);
	}
	return false;
}

bool CScraper::ProcessRegion(RMapCI r_iter) {
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

		__HDC_FOOTER_ATTENTION_HAS_TO_BE_CALLED_ON_EVERY_FUNCTION_EXIT_OTHERWISE_MEMORY_LEAK
		return true;
	}

	__HDC_FOOTER_ATTENTION_HAS_TO_BE_CALLED_ON_EVERY_FUNCTION_EXIT_OTHERWISE_MEMORY_LEAK
	return false;
}

bool CScraper::EvaluateRegion(CString name, CString *result) {
	__HDC_HEADER
	CTransform	trans;
	RMapCI		r_iter = p_tablemap->r$()->find(name.GetString());
	if (r_iter != p_tablemap->r$()->end()) 
	{
    // !!! Lots of potential for optimization here
    ++total_region_counter;
		if (ProcessRegion(r_iter)) {
      ++identical_region_counter;
      write_log(preferences.debug_alltherest(),
        "[CScraper] Region %s identical\n", name);
    } else {
      write_log(preferences.debug_alltherest(),
        "[CScraper] Region %s NOT identical\n", name);
    }
		old_bitmap = (HBITMAP) SelectObject(hdcCompatible, r_iter->second.cur_bmp);
		trans.DoTransform(r_iter, hdcCompatible, result);
		SelectObject(hdcCompatible, old_bitmap);
		write_log(preferences.debug_scraper(), "[CScraper] EvaluateRegion(), [%s] -> [%s]\n", 
			name, *result);
    __HDC_FOOTER_ATTENTION_HAS_TO_BE_CALLED_ON_EVERY_FUNCTION_EXIT_OTHERWISE_MEMORY_LEAK
		return true;
	}
	// Region does not exist
	__HDC_FOOTER_ATTENTION_HAS_TO_BE_CALLED_ON_EVERY_FUNCTION_EXIT_OTHERWISE_MEMORY_LEAK
	return false;
}

// Result will be changed to true, if "true" or something similar is found
// Result will be changed to falsee, if "false" or something similar is found
// Otherwise unchanged (keep default / allow multiple evaluations)
void CScraper::EvaluateTrueFalseRegion(bool *result, const CString name) {
  CString text_result;
	if (EvaluateRegion(name, &text_result))	{
    write_log(preferences.debug_scraper(), "[CScraper] %s result %s\n", 
      name, text_result.GetString());
    if (text_result == "true") {
      *result = true;
    } else if (text_result == "false") {
      *result = false;
    }
	}
}

bool CScraper::EvaluateNumericalRegion(double *result, const CString name) {
  CString text_result;
  if (EvaluateRegion(name, &text_result)) {
		CScraperPreprocessor::PreprocessMonetaryString(&text_result);
    write_log(preferences.debug_scraper(), "[CScraper] %s result %s\n", 
      name, text_result.GetString());
		if (text_result != "") {
      CScraperPreprocessor::PreprocessMonetaryString(&text_result);
      CTransform trans;
			*result = trans.StringToMoney(text_result);
      return true;
		}
	}
  return false;
}

void CScraper::SetButtonState(CString *button_state, CString text) {
	if (text != "")
	{
		*button_state = text;	
	}
	else
	{
		*button_state = "false";
	}
}

void CScraper::ScrapeInterfaceButtons() {
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

void CScraper::ScrapeActionButtons() {
	CString button_name;
	CString result;
	for (int i=0; i<k_max_number_of_buttons; i++)
	{
		button_name.Format("i%dstate", i);
		if (EvaluateRegion(button_name, &result))
		{
			SetButtonState(&_button_state[i], result);	
		}
	}
	// Ugly WinHoldem convention
	// When using ManualMode, grab i5state for PT network 
	if (p_symbol_engine_casino->ConnectedToManualMode())
	{
		p_tablemap->set_network(p_scraper->button_state(5));
	}
}

void CScraper::ScrapeActionButtonLabels() {
	CString label;
	CString result;
  // Every button needs a label
  // No longer using any WinHoldem defaults
	for (int i=0; i<k_max_number_of_buttons; i++)	{
		set_button_label(i, "");
		label.Format("i%dlabel", i);
		if (EvaluateRegion(label, &result))
		{
			set_button_label(i, result);
		}
	}
}

void CScraper::ScrapeBetpotButtons() {
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

void CScraper::ScrapeSeatedActive() {
	for (int i=0; i<k_max_number_of_players; i++)	{
		p_table_state->_players[i]._seated = false;
		p_table_state->_players[i]._active = false;
	}
	for (int i=0; i<p_tablemap->nchairs(); i++)	{
		ScrapeSeated(i);
		if (p_scraper_access->IsPlayerSeated(i))		{
			ScrapeActive(i);
		}
	}
}

void CScraper::ScrapeBetsAndBalances() {
	for (int i=0; i<p_tablemap->nchairs(); i++)
	{
		// We have to scrape "every" player,
    //   * as people might bet-fold-standup.
    //   * as people might be missing in tournament, but we use ICM
		// Improvement: 
		//   * scrape everybody up to my first action (then we know who was dealt)
		//   * after that we scrape only dealt players
		//   * and also players who have cards (fresh sitdown and hand-reset, former playersdealt is wrong)
		if ((!p_symbol_engine_history->DidActThisHand())
			|| IsBitSet(p_symbol_engine_active_dealt_playing->playersdealtbits(), i)
      || p_table_state->_players[i].HasAnyCards())
		{
			ScrapeBet(i);
			ScrapeBalance(i);
		}
	}
}

void CScraper::ScrapeSeated(int chair) {
	CString seated;
	CString result;

	p_table_state->_players[chair]._seated = false;
	seated.Format("p%dseated", chair);
	if (EvaluateRegion(seated, &result)) {
		if (result != "")	{
			p_table_state->_players[chair]._seated = result;
		}
	}
	if (p_scraper_access->IsPlayerSeated(chair)) {
		return;
	}
	// try u region next uXseated,
	// but only if we didn't get a positive result from the p region
	seated.Format("u%dseated", chair);
	if (EvaluateRegion(seated, &result)) {
		if (result!="")	{
			p_table_state->_players[chair]._seated = result;
		}
	}
}

void CScraper::ScrapeDealer() {
	// The dealer might sit in any chair, even empty ones in some cases
	// That's why we scrape all chairs
	CString dealer;
	CString result;

	for (int i=0; i<p_tablemap->nchairs(); i++)	{
		p_table_state->_players[i]._dealer = false;
	}

	for (int i=0; i<p_tablemap->nchairs(); i++)
	{
		dealer.Format("p%ddealer", i);
		if (EvaluateRegion(dealer, &result))
		{
			if (p_string_match->IsStringDealer(result))	{
				p_table_state->_players[i]._dealer = true;
				return;
			}
		}
		// Now search for uXdealer
		dealer.Format("u%ddealer", i);
		if (EvaluateRegion(dealer, &result))
		{
			if (p_string_match->IsStringDealer(result))
			{
				p_table_state->_players[i]._dealer = true;
				return;
			}
		}
	}
}

void CScraper::ScrapeActive(int chair) {
	CString active;
	CString result;
	p_table_state->_players[chair]._active = false;
  // try p region first pXactive
	active.Format("p%dactive", chair);
	if (EvaluateRegion(active, &result)) {
		p_table_state->_players[chair]._active = result;
	}
	if (p_scraper_access->IsPlayerActive(chair)) {
		return;
	}
	active.Format("u%dactive", chair);
	if (EvaluateRegion(active, &result)) {
		p_table_state->_players[chair]._active = result;
	}
}

void CScraper::ScrapeColourCodes() {
  CString result;
  CString region;
  for (int i=0; i<p_tablemap->nchairs(); i++) {
    region.Format("p%icolourcode", i);
    if (EvaluateRegion(region, &result)) {
      p_table_state->_players[i]._colourcode = atoi(result);
    } else {
      p_table_state->_players[i]._colourcode = k_undefined_zero;
    }
  }
}

void CScraper::ScrapeSlider() {
	__HDC_HEADER

	RMapCI handleCI, slider;
	RMapI handleI;
	CString text;
	POINT handle_xy;

	// find handle
	handleCI = p_tablemap->r$()->find("i3handle");
	slider = p_tablemap->r$()->find("i3slider");
  if (handleCI!=p_tablemap->r$()->end() && slider!=p_tablemap->r$()->end() && _button_state[3]!="false")	
	{
		int j = slider->second.right - handleCI->second.left;
		text = "";
		set_handle_found_at_xy(false);
		for (int k=0; k<=j; k++) 
		{
			handleI = p_tablemap->set_r$()->find("i3handle");
			handleI->second.left  += k;
			handleI->second.right += k;

			EvaluateRegion("i3handle", &text);
			handleI->second.left  -= k;
			handleI->second.right -= k;
			if (text == "handle" || text == "true") 
			{
				handleCI = p_tablemap->r$()->find("i3handle");
				handle_xy.x = handleCI->second.left + k;
				handle_xy.y = handleCI->second.top;
				set_handle_found_at_xy(true);
				set_handle_xy(handle_xy);
				write_log(preferences.debug_scraper(), "[CScraper] i3handle, result %d,%d\n", handle_xy.x, handle_xy.y);
				break;
			}
		}
		if (!handle_found_at_xy())
		{
			write_log(preferences.debug_scraper(), "[CScraper] i3handle, cannot find handle in the slider region...\n");
		}
	}
  __HDC_FOOTER_ATTENTION_HAS_TO_BE_CALLED_ON_EVERY_FUNCTION_EXIT_OTHERWISE_MEMORY_LEAK
}

int CScraper::CardString2CardNumber(CString card) {
	int result;
	if (StdDeck_stringToCard((char*) card.GetString(), &result)) {
    AssertRange(result, 0, 255);
	  return result;
  } else {
    return CARD_UNDEFINED;
  }
}

int CScraper::ScrapeCardface(CString base_name) {
  CString card_str;
  // Here name = base_name
	if (EvaluateRegion(base_name, &card_str)) {
		if (card_str != "") {
			return CardString2CardNumber(card_str);
		}
	}
  return CARD_UNDEFINED;
}

int CScraper::ScrapeCardByRankAndSuit(CString base_name) {
  CString rank = base_name + "rank";
	CString suit = base_name + "suit";
	CString rank_result, suit_result;
	// Scrape suit first (usually very fast colour-transform)
	if (EvaluateRegion(suit, &suit_result))	{
		// If a suit could not be recognized we don't need to scrape the rank at all
		// which is often an expensive fuzzy font in this case.
		if (IsSuitString(suit_result)) {
			EvaluateRegion(rank, &rank_result);
			if (IsRankString(rank_result))
			{
				if (rank_result == "10") rank_result = "T";
				CString card_str = rank_result + suit_result;
				return CardString2CardNumber(card_str);
			}
		}
	}
  return CARD_UNDEFINED;
}

int CScraper::ScrapeCardback(CString base_name) {
  if (base_name[0] == 'p')	{
	  CString cardback = base_name.Left(2) + "cardback";
	  CString cardback_result;
	  if (EvaluateRegion(cardback, &cardback_result)) {
	    if ((cardback_result == "cardback")
	        || (cardback_result == "true")) {
		    return CARD_BACK;
	    }
	  }
  }
  return CARD_UNDEFINED;
}

int CScraper::ScrapeNoCard(CString base_name){
  CString card_no_card = base_name + "nocard";
  CString no_card_result;
  if (EvaluateRegion(card_no_card, &no_card_result) 
		  && (no_card_result == "true"))	{
    write_log(preferences.debug_scraper(), "[CScraper] ScrapeNoCard(%s) -> true\n",
      card_no_card);
    return CARD_NOCARD;
  }
  write_log(preferences.debug_scraper(), "[CScraper] ScrapeNoCard(%s) -> false\n",
      card_no_card);
  return CARD_UNDEFINED;
}

// Cares about "everything"
//   * cardfaces
//   * ranks and suits
//   * cardbacks
int CScraper::ScrapeCard(CString name) {
  // First: in case of player cards try to scrape card-backs
  // This hasd to be the very first one,
  // because some casinos use different locations for cardbacks and cards
  // which would cause problems for the nocard-regioms
  // http://www.maxinmontreal.com/forums/viewtopic.php?f=117&t=17960
  int result = ScrapeCardback(name);
  if (result == CARD_BACK) return CARD_BACK;
  // Then try to scrape "no card"
  result = ScrapeNoCard(name);
  if (result == CARD_NOCARD) return CARD_NOCARD;
	// Then scrape "normal" cards (cardfaces) according to the specification
	result = ScrapeCardface(name);
  if (result != CARD_UNDEFINED) return result;
	// Otherwise: try to scrape suits and ranks individually
  result = ScrapeCardByRankAndSuit(name);
  if (result != CARD_UNDEFINED) return result;
  // Otherwise: in case of playercards try to scrape uXcardfaceY
	CString uname = name;
	if (name[0] == 'p')	{
		uname.SetAt(0, 'u');
	}
  result = ScrapeCardface(uname);
  if (result != CARD_UNDEFINED) return result;
	// Nothing found
  /*
  write_log(k_always_log_errors, 
    "[CScraper] WARNING ScrapeCard(%s) found nothing\n", name);
  write_log(k_always_log_errors, 
    "[CScraper] Not nocard, no cards and no cardbacks.\n");
  write_log(k_always_log_errors,
    "[CScraper] Defaulting to cardbacks (players) / nocard (board)\n");
  */
  if (name[0] == 'p') {
    // Currently we have a problem with cardbacks,
    // but whatever we try to scrape is not NOCARD and not a card.
    // So we assume card_backs.
	  return CARD_BACK;
  } else {
    // Treat undefined community card as no card
    assert(name[0] == 'c');
    return CARD_NOCARD;
  }
}

void CScraper::ScrapePlayerCards(int chair) {
	CString card_name;
	int card = CARD_UNDEFINED;
	for (int i=0; i<k_number_of_cards_per_player; i++) {
		card_name.Format("p%dcardface%d", chair, i);
		if ((i > 0) 
      && ((card == CARD_UNDEFINED) || (card == CARD_BACK) || (card == CARD_NOCARD))) {
			// Stop scraping if we find missing cards or cardbacks
		} else {
			card = ScrapeCard(card_name);
		}
    p_table_state->_players[chair]._hole_cards[i].SetValue(card);
	}
  p_table_state->_players[chair].CheckPlayerCardsForConsistency();
}

void CScraper::ScrapeCommonCards() {
	CString card_name;
	for (int i=0; i<k_number_of_community_cards; i++)
	{
		card_name.Format("c0cardface%d", i);
		int card = ScrapeCard(card_name);
    p_table_state->_common_cards[i].SetValue(card);
	}
}
	
// returns true if common cards are in the middle of an animation
bool CScraper::IsCommonAnimation(void) {
	int	flop_card_count = 0;

	// Count all the flop cards
	for (int i=0; i<k_number_of_flop_cards; i++) {
    if (p_table_state->_common_cards[i].IsKnownCard()) {
			flop_card_count++;
		}
	}

	// If there are some flop cards present,
	// but not all 3 then there is an animation going on
	if (flop_card_count > 0 && flop_card_count < k_number_of_flop_cards) {
		return true;
	}
	// If the turn card is present,
	// but not all 3 flop cards are present then there is an animation going on
	else if (p_table_state->_common_cards[3].IsKnownCard()
      && flop_card_count != k_number_of_flop_cards) {
		return true;
	}
	// If the river card is present,
	// but the turn card isn't
	// OR not all 3 flop cards are present then there is an animation going on
	else if (p_table_state->_common_cards[4].IsKnownCard() 
      && (!p_table_state->_common_cards[3].IsKnownCard() || flop_card_count != k_number_of_flop_cards)) {
		return true;
	}
	return false;
}

void CScraper::ClearAllPlayerNames() {
	for (int i=0; i<k_max_number_of_players; i++) {
    p_table_state->_players[i]._name = "";
	}
}

void CScraper::ScrapeName(int chair) {
	RETURN_IF_OUT_OF_RANGE (chair, p_tablemap->LastChair())

	CString				result;
	CString				s = "";

	// Player name uXname
	s.Format("u%dname", chair);
	EvaluateRegion(s, &result);	
	write_log(preferences.debug_scraper(), "[CScraper] u%dname, result %s\n", chair, result.GetString());
	if (result != "")	{
    p_table_state->_players[chair]._name = result;
		return;
	}
	// Player name pXname
	s.Format("p%dname", chair);
	EvaluateRegion(s, &result);
	write_log(preferences.debug_scraper(), "[CScraper] p%dname, result %s\n", chair, result.GetString());
	if (result != "") {
		p_table_state->_players[chair]._name = result;
    return;
	}
}

double CScraper::ScrapeUPBalance(int chair, char scrape_u_else_p) {
  CString	name;
  CString text;
  assert((scrape_u_else_p == 'u') || (scrape_u_else_p == 'p'));
  set_sitting_out(chair, false);
  name.Format("%c%dbalance", scrape_u_else_p, chair);
  if (EvaluateRegion(name, &text)) {
		if (p_string_match->IsStringAllin(text)) {
      return 0.0;
			write_log(preferences.debug_scraper(), "[CScraper] %s, result ALLIN", name);
		}
		else if (text.MakeLower().Find("out")!=-1) {
			set_sitting_out(chair, true);
			p_table_state->_players[chair]._active = false;
			write_log(preferences.debug_scraper(), "[CScraper] %s, result OUT\n", name);
      return k_undefined;
		}	else {
			CScraperPreprocessor::ProcessBalanceNumbersOnly(&text);
			if (text!="" && p_string_match->IsNumeric(text)) {
        CTransform trans;
        double result = trans.StringToMoney(text);
			  write_log(preferences.debug_scraper(), "[CScraper] u%dbalance, result %s\n", chair, text.GetString());
        return result;
      }
		}
	}
  return k_undefined;
}

void CScraper::ScrapeBalance(int chair) {
	RETURN_IF_OUT_OF_RANGE (chair, p_tablemap->LastChair())
  // Scrape uXbalance and pXbalance
  double result = ScrapeUPBalance(chair, 'p');
  if (result >= 0) {
    p_table_state->_players[chair]._balance = result;
    return;
  }
  result = ScrapeUPBalance(chair, 'u');
  if (result >= 0) {
    p_table_state->_players[chair]._balance = result;
    return;
  }
}

void CScraper::ScrapeBet(int chair) {
  RETURN_IF_OUT_OF_RANGE (chair, p_tablemap->LastChair())

	__HDC_HEADER;
	CString				text = "";
	CString				s = "", t="";

	p_table_state->_players[chair]._bet = 0.0;
  	// Player bet pXbet
  s.Format("p%dbet", chair);
  double result = 0;
	if (EvaluateNumericalRegion(&result, s)) {
	  p_table_state->_players[chair]._bet = result;
		__HDC_FOOTER_ATTENTION_HAS_TO_BE_CALLED_ON_EVERY_FUNCTION_EXIT_OTHERWISE_MEMORY_LEAK
		return;
	}

	// uXbet
	s.Format("u%dbet", chair);
  result = 0;
	if (EvaluateNumericalRegion(&result, s)) {
		p_table_state->_players[chair]._bet = result;
		__HDC_FOOTER_ATTENTION_HAS_TO_BE_CALLED_ON_EVERY_FUNCTION_EXIT_OTHERWISE_MEMORY_LEAK
		return;
	}		
		
	// pXchip00
	s.Format("p%dchip00", chair);
	RMapCI r_iter = p_tablemap->r$()->find(s.GetString());
	if (r_iter != p_tablemap->r$()->end() && p_table_state->_players[chair]._bet == 0) 	{
		old_bitmap = (HBITMAP) SelectObject(hdcCompatible, _entire_window_cur);
		double chipscrape_res = DoChipScrape(r_iter);
		SelectObject(hdcCompatible, old_bitmap);

		t.Format("%.2f", chipscrape_res);
		CScraperPreprocessor::PreprocessMonetaryString(&t);
		p_table_state->_players[chair]._bet = strtod(t.GetString(), 0);

		write_log(preferences.debug_scraper(), "[CScraper] p%dchipXY, result %f\n", 
      chair, p_table_state->_players[chair]._bet);
	}
	__HDC_FOOTER_ATTENTION_HAS_TO_BE_CALLED_ON_EVERY_FUNCTION_EXIT_OTHERWISE_MEMORY_LEAK
}

void CScraper::ResetLimitInfo() {
  _s_limit_info.handnumber = "";
	_s_limit_info.sblind = k_undefined;
	_s_limit_info.bblind = k_undefined;
	_s_limit_info.bbet   = k_undefined;
	_s_limit_info.ante   = k_undefined;
	_s_limit_info.sb_bb  = k_undefined;
	_s_limit_info.bb_BB  = k_undefined;
	_s_limit_info.limit  = k_undefined_zero;
  _s_limit_info.buyin  = k_undefined;
  _s_limit_info.is_final_table = false;
}

//
// !! All code below not yet refactored
//
void CScraper::ClearScrapeAreas(void) {
	for (int i=0; i<k_number_of_community_cards; i++) {
    p_table_state->_common_cards[i].ClearValue();
  }
	for (int i=0; i<k_max_number_of_players; i++) {
    p_table_state->_players[i].Reset();
		p_table_state->_players[i]._active = false;
		p_table_state->_players[i]._active = false;
		p_table_state->_players[i]._dealer = false;
		p_table_state->_players[i]._bet = 0.0;
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

	ResetLimitInfo();
	strcpy_s(_title_last, MAX_WINDOW_TITLE, "");
}

void CScraper::ScrapeAllPlayerCards() {
	for (int i=0; i<k_max_number_of_players; i++){
		for (int j=0; j<k_number_of_cards_per_player; j++) {
			p_table_state->_players[i]._hole_cards[j].ClearValue();
		}
	}
	write_log(preferences.debug_scraper(), "[CScraper] ScrapeAllPlayerCards()\n");
	for (int i=0; i<p_tablemap->nchairs(); i++) {
		write_log(preferences.debug_scraper(), "[CScraper] Calling ScrapePlayerCards, chair %d.\n", i);
		ScrapePlayerCards(i);
	}
}

void CScraper::ScrapePots() {
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
    double result = 0;
		if (EvaluateNumericalRegion(&result, s)) {
			set_pot(j, result);
			continue;
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

	__HDC_FOOTER_ATTENTION_HAS_TO_BE_CALLED_ON_EVERY_FUNCTION_EXIT_OTHERWISE_MEMORY_LEAK
}

void CScraper::ScrapeLimits() {
	__HDC_HEADER
	CString				handnumber = "";
	CString				text = "";
	CString				titletext = "";
	char				c_titletext[MAX_WINDOW_TITLE] = {0};
	bool				got_new_scrape = false, log_blind_change = false;
	CTransform			trans;
	CString				s = "";
	RMapCI				r_iter = p_tablemap->r$()->end();
	SMapCI				s_iter = p_tablemap->s$()->end();
	// These persist after scraping the handnumber region
	// to seed l_handnumber when we scrape info from
	// the titlebar.  That way if we do not find handnumber
	// information in the titlebar we won't overwrite the values we scraped
	// from the handnumber region.
	handnumber = "";

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
	double l_sblind = k_undefined;
  double l_bblind = k_undefined; 
  double l_bbet   = k_undefined; 
  double l_ante   = k_undefined;
  double l_sb_bb  = k_undefined;
  double l_bb_BB  = k_undefined;
  double l_buyin  = k_undefined;
	int    l_limit  = k_undefined;
	// These are scraped from specific regions earlier in this
	// function.  Use the values we scraped (if any) to seed
	// the l_ locals so that we don't blindly overwrite the
	// information we scraped from those specific regions with
	// default values if we can't find them in the titlebar.
	CString l_handnumber = handnumber; 

	// s$ttlimits - Scrape blinds/stakes/limit info from title text
	s_iter = p_tablemap->s$()->find("ttlimits");
	if (s_iter != p_tablemap->s$()->end())
	{
		GetWindowText(p_autoconnector->attached_hwnd(), c_titletext, MAX_WINDOW_TITLE-1);
		titletext = c_titletext;
	 	
		CScraperPreprocessor::PreprocessTitleString(&titletext);
    trans.ParseStringBSL(
			titletext, s_iter->second.text, NULL,
			&l_handnumber, &l_sblind, &l_bblind, &l_bbet, &l_ante, 
      &l_limit, &l_sb_bb, &l_bb_BB, &l_buyin);

		write_log(preferences.debug_scraper(), "[CScraper] ttlimits, result sblind/bblind/bbet/ante/gametype: %.2f / %.2f / %.2f / %.2f / %i\n", 
      l_sblind, l_bblind, l_bbet, l_ante, l_limit);

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
					&l_handnumber, &l_sblind, &l_bblind, &l_bbet, &l_ante, 
          &l_limit, &l_sb_bb, &l_bb_BB, &l_buyin);

				write_log(preferences.debug_scraper(), "[CScraper] ttlimits, result sblind/bblind/bbet/ante/gametype: %.2f / %.2f / %.2f / %.2f / %i\n", 
          l_sblind, l_bblind, l_bbet, l_ante, l_limit);
			}
		}

		// c0limits needs both
    // * a region r$c0limits to define where to scrape the limis 
    // * a symbol s$c0limits to define how to interpret the scrape text,
    //   similar to s$ttlimits
    // Since OH 7.3.3 we do no longer support c0limitsX
    // because it never got correctly implemented 
    // and therefore seems to be not needed. 
		if (EvaluateRegion("c0limits", &text))	{
      write_log(preferences.debug_scraper(), 
        "[CScraper] r$c0limit evalutes to %s\n", text);
			if (text != "")	{
			  s_iter = p_tablemap->s$()->find("c0limits");
        CString how_to_interpret_c0limit = s_iter->second.text;
        write_log(preferences.debug_scraper(), 
          "[CScraper] s$c0limit is %s\n", how_to_interpret_c0limit);
				trans.ParseStringBSL(
					text, how_to_interpret_c0limit, NULL,
					&l_handnumber, &l_sblind, &l_bblind, &l_bbet, &l_ante, 
          &l_limit, &l_sb_bb, &l_bb_BB, &l_buyin);
        write_log(preferences.debug_scraper(), "[CScraper] ttlimits, result sblind/bblind/bbet/ante/gametype: %.2f / %.2f / %.2f / %.2f / %i\n", 
      l_sblind, l_bblind, l_bbet, l_ante, l_limit);
			}
		}
    // save what we just scanned through
    if (handnumber != "") {
			set_handnumber(l_handnumber);
    }
    if (l_sblind != k_undefined) {
			_s_limit_info.sblind = l_sblind;
    }
    if (l_bblind != k_undefined) {
			_s_limit_info.bblind = l_bblind;
    }
    if (l_bbet != k_undefined) {
			_s_limit_info.bbet = l_bbet;
    }
    if (l_ante != k_undefined) {
			_s_limit_info.ante = l_ante;
    }
    if (l_limit != k_undefined) {
			_s_limit_info.limit = l_limit;
    }
    if (l_sb_bb != k_undefined) {
			_s_limit_info.sb_bb = l_sb_bb;
    }
    if (l_bb_BB != k_undefined) {
			_s_limit_info.bb_BB = l_bb_BB;
    }
    if (l_buyin != k_undefined) {
			_s_limit_info.buyin = l_buyin;
    }
    // r$c0smallblind
    EvaluateNumericalRegion(&_s_limit_info.sblind, "c0smallblind");
		// r$c0bigblind
    EvaluateNumericalRegion(&_s_limit_info.bblind, "c0bigblind");
		// r$c0bigbet
    EvaluateNumericalRegion(&_s_limit_info.bbet, "c0bigbet");
		// r$c0ante
    EvaluateNumericalRegion(&_s_limit_info.ante, "c0ante");
		// r$c0isfinaltable
    EvaluateTrueFalseRegion(&_s_limit_info.is_final_table, "c0isfinaltable");
	}
  __HDC_FOOTER_ATTENTION_HAS_TO_BE_CALLED_ON_EVERY_FUNCTION_EXIT_OTHERWISE_MEMORY_LEAK
}

void CScraper::CreateBitmaps(void) {
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

void CScraper::DeleteBitmaps(void) {
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

// This is the chip scrape routine
const double CScraper::DoChipScrape(RMapCI r_iter) {
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

bool CScraper::IsExtendedNumberic(CString text) {
  bool currently_unused = false;
  assert(currently_unused);
  return false;
}

bool CScraper::IsIdenticalScrape() {
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
	// !! How often does this happen?
	// !! How costly is the comparison?
	if (BitmapsAreEqual(_entire_window_last, _entire_window_cur) &&
		strcmp(_title, _title_last)==0)
	{
		DeleteDC(hdcCompatible);
		DeleteDC(hdcScreen);
		ReleaseDC(p_autoconnector->attached_hwnd(), hdc);
		write_log(preferences.debug_scraper(), "[CScraper] IsIdenticalScrape() true\n");
    __HDC_FOOTER_ATTENTION_HAS_TO_BE_CALLED_ON_EVERY_FUNCTION_EXIT_OTHERWISE_MEMORY_LEAK
		return true;
	}

	// Copy into "last" title
	strcpy_s(_title_last, MAX_WINDOW_TITLE, _title);

	// Copy into "last" bitmap
	old_bitmap = (HBITMAP) SelectObject(hdcCompatible, _entire_window_last);
	BitBlt(hdcCompatible, 0, 0, cr.right-cr.left+1, cr.bottom-cr.top+1, hdc, cr.left, cr.top, SRCCOPY);
	SelectObject(hdc, old_bitmap);

	__HDC_FOOTER_ATTENTION_HAS_TO_BE_CALLED_ON_EVERY_FUNCTION_EXIT_OTHERWISE_MEMORY_LEAK
	write_log(preferences.debug_scraper(), "[CScraper] IsIdenticalScrape() false\n");
	return false;
}

#undef __HDC_HEADER 
#undef __HDC_FOOTER_ATTENTION_HAS_TO_BE_CALLED_ON_EVERY_FUNCTION_EXIT_OTHERWISE_MEMORY_LEAK