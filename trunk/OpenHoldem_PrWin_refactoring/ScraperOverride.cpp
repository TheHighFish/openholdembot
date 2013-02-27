#include "stdafx.h"
#include "ScraperOverride.h"

#include "CPreferences.h"
#include "CScraper.h"
#include "OpenHoldem.h"
#include "..\..\Reference Scraper DLL\scraperdll.h"


SScraperState  ss;

void PopulateScraperStateWithScrapedValues();
void CallScraperOverrideDLL();
void PropagateChangedValuesBackToScraper();

void OverriderScraperStateByScraperPreprocessorDLLIfNecessary()
{
	if (theApp._scraper_dll)
	{
		PopulateScraperStateWithScrapedValues();
		CallScraperOverrideDLL();
		PropagateChangedValuesBackToScraper();
	}
}

void PopulateScraperStateWithScrapedValues()
{
	// populate structure that gets passed to dll
	strncpy_s(ss.title, MAX_WINDOW_TITLE, p_scraper->title(), MAX_WINDOW_TITLE);

	for (int i=0; i<k_number_of_community_cards; i++)
	{
		ss.card_common[i] = p_scraper->card_common(i);
	}

	for (int i=0; i<k_max_number_of_players; i++)
	{
		ss.card_player[i][0] = p_scraper->card_player(i, 0);
		ss.card_player[i][1] = p_scraper->card_player(i, 1);
		ss.dealer[i] = p_scraper->dealer(i);
		ss.sitting_out[i] = p_scraper->sitting_out(i);
		ss.seated[i] = p_scraper->seated(i);
		ss.active[i] = p_scraper->active(i);
		ss.name[i] = p_scraper->player_name(i);
		ss.balance[i] = p_scraper->player_balance(i);
		ss.name_good_scrape[i] = p_scraper->name_good_scrape(i);
		ss.balance_good_scrape[i] = p_scraper->balance_good_scrape(i);
		ss.bet[i] = p_scraper->player_bet(i);
	}

	for (int i=0; i<k_max_number_of_pots; i++)
	{
		ss.pot[i] = p_scraper->pot(i);
	}

	for (int i=0; i<k_max_number_of_buttons; i++)
	{
		ss.button_state[i] = p_scraper->button_state(i);
		ss.i86X_button_state[i] = p_scraper->i86X_button_state(i);
		ss.button_label[i] = p_scraper->button_label(i);
	}

	ss.i86_button_state = p_scraper->i86_button_state();

	ss.sblind = p_scraper->s_limit_info()->sblind;
	ss.found_sblind = p_scraper->s_limit_info()->found_sblind;
	ss.bblind = p_scraper->s_limit_info()->bblind;
	ss.found_bblind = p_scraper->s_limit_info()->found_bblind;
	ss.bbet = p_scraper->s_limit_info()->bbet;
	ss.found_bbet = p_scraper->s_limit_info()->found_bbet;
	ss.ante = p_scraper->s_limit_info()->ante;
	ss.found_ante = p_scraper->s_limit_info()->found_ante;
	ss.limit = (LimitType) p_scraper->s_limit_info()->limit;
	ss.found_limit = p_scraper->s_limit_info()->found_limit;
	ss.handnumber = p_scraper->s_limit_info()->handnumber;
	ss.found_handnumber = p_scraper->s_limit_info()->found_handnumber;
	ss.istournament = p_scraper->s_limit_info()->istournament;
}

void CallScraperOverrideDLL()
{
	// Call the scraper override !! extra option for logging?
	write_log(prefs.debug_heartbeat(), "[HeartBeatThread] Sending scraper data to override DLL.\n");
	(theApp._dll_scraper_override) (&ss);
}

void PropagateChangedValuesBackToScraper()
{
	// Replace values in p_scraper with those provided by scraper dll
	p_scraper->set_title(ss.title);

	for (int i=0; i<k_number_of_community_cards; i++)
	{
		p_scraper->set_card_common(i, ss.card_common[i]);
	}

	for (int i=0; i<k_max_number_of_players; i++)
	{
		p_scraper->set_card_player(i, 0, ss.card_player[i][0]);
		p_scraper->set_card_player(i, 1, ss.card_player[i][1]);
		p_scraper->set_dealer(i, ss.dealer[i]);
		p_scraper->set_sitting_out(i, ss.sitting_out[i]);
		p_scraper->set_seated(i, ss.seated[i]);
		p_scraper->set_active(i, ss.active[i]);
		p_scraper->set_player_name(i, ss.name[i]);
		p_scraper->set_player_balance(i, ss.balance[i]);
		p_scraper->set_name_good_scrape(i, ss.name_good_scrape[i]);
		p_scraper->set_balance_good_scrape(i, ss.balance_good_scrape[i]);
		p_scraper->set_player_bet(i, ss.bet[i]);
	}

	for (int i=0; i<k_max_number_of_pots; i++)
	{
		p_scraper->set_pot(i, ss.pot[i]);
	}

	for (int i=0; i<k_max_number_of_buttons; i++)
	{
		p_scraper->set_button_state(i, ss.button_state[i]);
		p_scraper->set_i86X_button_state(i, ss.i86X_button_state[i]);
		p_scraper->set_button_label(i, ss.button_label[i]);
	}

	p_scraper->set_i86_button_state(ss.i86_button_state);

	p_scraper->set_sblind(ss.sblind);
	p_scraper->set_found_sblind(ss.found_sblind);
	p_scraper->set_bblind(ss.bblind);
	p_scraper->set_found_bblind(ss.found_bblind);
	p_scraper->set_bbet(ss.bbet);
	p_scraper->set_found_bbet(ss.found_bbet);
	p_scraper->set_ante(ss.ante);
	p_scraper->set_found_ante(ss.found_ante);
	p_scraper->set_limit(ss.limit);
	p_scraper->set_found_limit(ss.found_limit);
	p_scraper->set_handnumber(ss.handnumber);
	p_scraper->set_found_handnumber(ss.found_handnumber);
	p_scraper->set_istournament(ss.istournament);
}