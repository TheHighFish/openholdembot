#include "StdAfx.h"
#include <io.h>
#include <fcntl.h>

#include "CGlobal.h"
#include "CPreferences.h"
#include "OpenHoldem.h"

#include "CScraper.h"
#include "CSymbols.h"
#include "CHeartbeatThread.h"
#include "..\CTablemap\CTablemap.h"
#include "CGrammar.h"

#include "resource.h"
#include "DialogFormulaScintilla.h"

CGlobal				*p_global = NULL;
CRITICAL_SECTION	CGlobal::cs_global;

//  Sanity check: enough disk-space for a replay frame?
//	We assume, 10 MB are enough
//	  - a large table of 1000x1000 pixels a 4 byte
//	  - some KB for the HTML file.
//	  - some space for other processes	
const unsigned int FREE_SPACE_NEEDED_FOR_REPLAYFRAME = 10000000;  


CGlobal::CGlobal(void)
{
	__SEH_SET_EXCEPTION_HANDLER

	int			i = 0;

	InitializeCriticalSectionAndSpinCount(&cs_global, 4000);
	
	// No longer any need to copy preferences from registry.
	autoplay_pressed = false;
	
	EnterCriticalSection(&cs_global);
		_attached_hwnd = NULL;

		for (i=0; i<10; i++)
			_flags[i] = false;
	LeaveCriticalSection(&cs_global);

	state_index = 0;

	mm_network = "";

	autoplay = false;
	autoplay_pressed = false;

	EnterCriticalSection(&cs_global);
		_m_wait_cursor = false;
		_replay_recorded_this_turn = false;
	LeaveCriticalSection(&cs_global);
}

CGlobal::~CGlobal(void)
{
	DeleteCriticalSection(&cs_global);
}

void CGlobal::CaptureState(const char *title)
{
	int					i = 0, j = 0;
	bool				playing = true;
	unsigned char		card = CARD_NOCARD;

	// figure out if I am playing
	int sym_chair = (int) p_symbols->sym()->chair;
	if (!p_symbols->user_chair_confirmed())
	{
		playing = false;
	}
	else if (p_scraper->card_player(sym_chair, 0) == CARD_BACK || 
			 p_scraper->card_player(sym_chair, 1) == CARD_BACK ||
			 p_scraper->card_player(sym_chair, 0) == CARD_NOCARD || 
			 p_scraper->card_player(sym_chair, 1) == CARD_NOCARD)
	{
		playing = false;
	}

	// When using MM, grab i5state for PT network
	bool sym_ismanual = (bool) p_symbols->sym()->ismanual;
	if (sym_ismanual)
	{
		mm_network = p_scraper->button_state(5);
	}

	// Poker window title
	strncpy_s(state[state_index&0xff].m_title, 64, title, _TRUNCATE);
	state[state_index&0xff].m_title[63] = '\0';

	// Pot information
	for (i=0; i<=9; i++)
		state[state_index&0xff].m_pot[i] = p_scraper->pot(i);

	// Common cards
	for (i=0; i<=4; i++)
	{
		if (p_scraper->card_common(i) == CARD_BACK)
		{
			card = WH_CARDBACK;
		}
		else if (p_scraper->card_common(i) == CARD_NOCARD)
		{
			card = WH_NOCARD;
		}
		else
		{
			card = ((StdDeck_RANK(p_scraper->card_common(i))+2)<<4) |
					(StdDeck_SUIT(p_scraper->card_common(i)) == StdDeck_Suit_CLUBS ? WH_SUIT_CLUBS :
					 StdDeck_SUIT(p_scraper->card_common(i)) == StdDeck_Suit_DIAMONDS ? WH_SUIT_DIAMONDS :
					 StdDeck_SUIT(p_scraper->card_common(i)) == StdDeck_Suit_HEARTS ? WH_SUIT_HEARTS :
					 StdDeck_SUIT(p_scraper->card_common(i)) == StdDeck_Suit_SPADES ? WH_SUIT_SPADES : 0) ;
		}

		state[state_index&0xff].m_cards[i] = card;
	}

	// playing, posting, dealerchair
	int sym_dealerchair = (int) p_symbols->sym()->dealerchair;
	bool sym_isautopost = (bool) p_symbols->sym()->isautopost;
	state[state_index&0xff].m_is_playing = playing;
	state[state_index&0xff].m_is_posting = sym_isautopost;
	state[state_index&0xff].m_fillerbits = 0;
	state[state_index&0xff].m_fillerbyte = 0;
	state[state_index&0xff].m_dealer_chair = sym_dealerchair;

	// loop through all 10 player chairs
	for (i=0; i<=9; i++)
	{

		// player name, balance, currentbet
		strncpy_s(state[state_index&0xff].m_player[i].m_name, 16, p_scraper->player_name(i).GetString(), _TRUNCATE);
		state[state_index&0xff].m_player[i].m_balance = p_symbols->sym()->balance[i];
		state[state_index&0xff].m_player[i].m_currentbet = p_symbols->sym()->currentbet[i];

		// player cards
		for (j=0; j<=1; j++)
		{
			if (p_scraper->card_player(i, j) == CARD_BACK)
			{
				card = WH_CARDBACK;
			}
			else if (p_scraper->card_player(i, j) == CARD_NOCARD)
			{
				card = WH_NOCARD;
			}
			else
			{
				card = ((StdDeck_RANK(p_scraper->card_player(i, j))+2)<<4) |
					   (StdDeck_SUIT(p_scraper->card_player(i, j)) == StdDeck_Suit_CLUBS ? WH_SUIT_CLUBS :
						StdDeck_SUIT(p_scraper->card_player(i, j)) == StdDeck_Suit_DIAMONDS ? WH_SUIT_DIAMONDS :
						StdDeck_SUIT(p_scraper->card_player(i, j)) == StdDeck_Suit_HEARTS ? WH_SUIT_HEARTS :
						StdDeck_SUIT(p_scraper->card_player(i, j)) == StdDeck_Suit_SPADES ? WH_SUIT_SPADES : 0) ;
			}

			state[state_index&0xff].m_player[i].m_cards[j] = card;
		}

		// player name known, balance known
		state[state_index&0xff].m_player[i].m_name_known = p_scraper->name_good_scrape(i) ? 1 : 0;
		state[state_index&0xff].m_player[i].m_balance_known = p_scraper->balance_good_scrape(i) ? 1 : 0;
		state[state_index&0xff].m_player[i].m_fillerbits = 0;
		state[state_index&0xff].m_player[i].m_fillerbyte = 0;
	}

	state_index++;
}

void CGlobal::CreateReplayFrame(void)
{
	CString			path = "", filename = "", text = "", fcra_seen = "";
	FILE			*fp = NULL;
	int				i = 0;
	time_t			ltime = 0;
	tm				now_time = {0};
	char			now_time_str[100] = {0};
	ULARGE_INTEGER	free_bytes_for_user_on_disk = {0}, 
					total_bytes_on_disk = {0}, 
					free_bytes_total_on_disk = {0};
	int				e = SUCCESS;

	//  Sanity check: Enough disk-space for replay frame?	
	GetDiskFreeSpaceEx(
		_startup_path,				  //  Directory on disk of interest
		&free_bytes_for_user_on_disk,  
		&total_bytes_on_disk,	
		&free_bytes_total_on_disk);
	if (free_bytes_for_user_on_disk.QuadPart < FREE_SPACE_NEEDED_FOR_REPLAYFRAME) 
	{
		MessageBox(NULL, "Not enough disk space to create replay-frame.", 
				"ERROR", 0);	
		return;
	}

	// Get current time
	time(&ltime);
	localtime_s(&now_time, &ltime);
	strftime(now_time_str, 100, "%Y-%m-%d %H:%M:%S", &now_time);

	// Create replay/session dir if it does not exist
	path.Format("%s\\replay\\session_%lu\\", _startup_path, _session_id);
	if (GetFileAttributes(path.GetString()) == INVALID_FILE_ATTRIBUTES)
		SHCreateDirectoryEx(NULL, path.GetString(), NULL);

	// Get exclusive access to CScraper and CSymbols variables
	// (Wait for scrape/symbol cycle to finish before saving frame)
	EnterCriticalSection(&p_heartbeat_thread->cs_update_in_progress);

		// Create bitmap file
		path.Format("%s\\replay\\session_%lu\\frame%03d.bmp", _startup_path, _session_id, _next_replay_frame);
		CreateBMPFile(path.GetString(), p_scraper->entire_window_cur());

		// Create HTML file
		path.Format("%s\\replay\\session_%lu\\frame%03d.htm", _startup_path, _session_id, _next_replay_frame);
		if (fopen_s(&fp, path.GetString(), "w")==0)
		{
			fprintf(fp, p_scraper->title());
			fprintf(fp, "\n");
			fprintf(fp, "<html>\n");
			fprintf(fp, "<style>\n");
			fprintf(fp, "td {text-align:right;}\n");
			fprintf(fp, "</style>\n");
			fprintf(fp, "<body>\n");
			fprintf(fp, "<font face=courier>\n");
			fprintf(fp, "<img src=\"frame%03d.bmp\">\n", _next_replay_frame);
			fprintf(fp, "<br>\n");
			fprintf(fp, "<a href=\"frame%03d.htm\">PREV</a>\n",
					_next_replay_frame-1 >= 0 ? _next_replay_frame-1 : prefs.replay_max_frames());
			fprintf(fp, "<a href=\"frame%03d.htm\">NEXT</a>\n",
					_next_replay_frame+1 < prefs.replay_max_frames() ? _next_replay_frame+1 : 0);
			fprintf(fp, " [%lu.%03d] [%s]<br>\n", _session_id, _next_replay_frame, now_time_str);
			fprintf(fp, "<br>\n");
			fprintf(fp, "<table>\n");
			fprintf(fp, "<tr>\n");
			fprintf(fp, "<td>\n");

			// Table for: SFABD, hand, bet, balance, name
			fprintf(fp, "<table border=4 cellpadding=1 cellspacing=1>\n");
			fprintf(fp, "<tr>\n");
			fprintf(fp, "<th>#</th>\n");
			fprintf(fp, "<th>SFABDP</th>\n");  //seated, friend, active, button, dealt, playing
			fprintf(fp, "<th>hand</th>\n");
			fprintf(fp, "<th>bet</th>\n");
			fprintf(fp, "<th>balance</th>\n");
			fprintf(fp, "<th>name</th>\n");
			fprintf(fp, "</tr>\n");
			for (i=0; i<p_tablemap->s$items()->num_chairs; i++)
			{

				fprintf(fp, "<tr>\n");
				fprintf(fp, "<td>%d</td>", i);  // #
				text.Format("%s%s%s%s%s%s",
					(int) (p_symbols->sym()->playersseatedbits) & (1<<i) ? "s" : "-",
					p_symbols->sym()->userchair == i ? "f" : "-",
					(int) (p_symbols->sym()->playersactivebits) & (1<<i) ? "a" : "-",
					p_symbols->sym()->dealerchair== i ? "b" : "-",
					(int) (p_symbols->sym()->playersdealtbits) & (1<<i) ? "d" : "-",
					(int) (p_symbols->sym()->playersplayingbits) & (1<<i) ? "p" : "-");
				fprintf(fp, "<td>%s</td>", text.GetString());  // SFABDP
				fprintf(fp, "<td>%s%s</td>",
						GetCardHtml(p_scraper->card_player(i, 0)),
						GetCardHtml(p_scraper->card_player(i, 1)) );  // hand
				fprintf(fp, "<td>%11.2f</td>", p_scraper->player_bet(i));  // bet
				fprintf(fp, "<td>%11.2f</td>", p_scraper->player_balance(i));  // balance
				fprintf(fp, "<td>%-15s</td>\n", p_scraper->player_name(i).GetString());  // name
				fprintf(fp, "</tr>\n");
			}
			fprintf(fp, "</table>\n");
			fprintf(fp, "</td>\n");

			// Table for: FCRA
			fprintf(fp, "<td>\n");
			fprintf(fp, "<table align=center border=4 cellpadding=1 cellspacing=1>\n");
			fprintf(fp, "<tr><th>FCRA</th></tr>\n");
			fprintf(fp, "<tr>\n");
			int sym_myturnbits = (int) p_symbols->sym()->myturnbits;
			fcra_seen.Format("%s%s%s%s",
				sym_myturnbits&0x1 ? "F" : ".",
				sym_myturnbits&0x2 ? "C" : ".",
				sym_myturnbits&0x4 ? "R" : ".",
				sym_myturnbits&0x8 ? "A" : ".");

			fprintf(fp, "<td>%s</td>\n", fcra_seen.GetString());
			fprintf(fp, "</tr>\n");
			fprintf(fp, "</table>\n");

			// Table for: sb, bb, BB
			fprintf(fp, "<table align=center border=4 cellpadding=1 cellspacing=1>\n");
			fprintf(fp, "<tr><th>sb</th><td>%11.2f</td></tr>\n", p_symbols->sym()->sblind);
			fprintf(fp, "<tr><th>bb</th><td>%11.2f</td></tr>\n", p_symbols->sym()->bblind);
			fprintf(fp, "<tr><th>BB</th><td>%11.2f</td></tr>\n", p_symbols->bigbet());
			fprintf(fp, "</table>\n");

			// Table for: common cards
			fprintf(fp, "<table align=center border=4 cellpadding=1 cellspacing=1>\n");
			fprintf(fp, "<tr><th>commoncard</th></tr>\n");
			fprintf(fp, "<tr>\n");
			fprintf(fp, "<td>%s%s%s%s%s</td>\n",
					GetCardHtml(p_scraper->card_common(0)),
					GetCardHtml(p_scraper->card_common(1)),
					GetCardHtml(p_scraper->card_common(2)),
					GetCardHtml(p_scraper->card_common(3)),
					GetCardHtml(p_scraper->card_common(4)) );
			fprintf(fp, "</tr>\n");
			fprintf(fp, "</table>\n");

			// Table for: pots
			fprintf(fp, "<table align=center border=4 cellpadding=1 cellspacing=1>\n");
			fprintf(fp, "<tr><th>#</th><th>pot</th></tr>\n");
			fprintf(fp, "<tr>\n");
			fprintf(fp, "<td>0</td><td>%11.2f</td>\n", p_scraper->pot(0));
			fprintf(fp, "</tr>\n");
			for (i=1; i<=9; i++)
			{
				if (p_scraper->pot(i))
				{
					fprintf(fp, "<tr>\n");
					fprintf(fp, "<td>%d</td><td>%11.2f</td>\n", i, p_scraper->pot(i));
					fprintf(fp, "</tr>\n");
				}
				else
				{
					i = 11;
				}
			}
			fprintf(fp, "</table>\n");

			fprintf(fp, "</td>\n");
			fprintf(fp, "</tr>\n");
			fprintf(fp, "</table>\n");
			fprintf(fp, "</body></html>\n");

			fclose(fp);
		}	

	LeaveCriticalSection(&p_heartbeat_thread->cs_update_in_progress);

	// Increment counter
	EnterCriticalSection(&cs_global);
		_next_replay_frame++;
		if (_next_replay_frame >= prefs.replay_max_frames())
			_next_replay_frame = 0;
	LeaveCriticalSection(&cs_global);
}

CString CGlobal::GetCardHtml(unsigned int card)
{
	CString suit = "", color = "", rank = "", final = "";

	suit =	card == CARD_BACK ? "*" :
		   card == CARD_NOCARD ? "&nbsp" :
		   StdDeck_SUIT(card) == Suit_CLUBS ? "&clubs;" :
		   StdDeck_SUIT(card) == Suit_DIAMONDS ? "&diams;" :
		   StdDeck_SUIT(card) == Suit_HEARTS ? "&hearts;" :
		   StdDeck_SUIT(card) == Suit_SPADES ? "&spades;" :
		   "&nbsp";

	color = card == CARD_BACK ? "black" :
			card == CARD_NOCARD ? "black" :
			StdDeck_SUIT(card) == Suit_CLUBS ? "green" :
			StdDeck_SUIT(card) == Suit_DIAMONDS ? "blue" :
			StdDeck_SUIT(card) == Suit_HEARTS ? "red" :
			StdDeck_SUIT(card) == Suit_SPADES ? "black" :
			"black";

	rank =	card == CARD_BACK ? "*" :
		   card == CARD_NOCARD ? " " :
		   StdDeck_RANK(card) == Rank_ACE ? "A" :
		   StdDeck_RANK(card) == Rank_KING ? "K" :
		   StdDeck_RANK(card) == Rank_QUEEN ? "Q" :
		   StdDeck_RANK(card) == Rank_JACK ? "J" :
		   StdDeck_RANK(card) == Rank_TEN ? "T" :
		   StdDeck_RANK(card) == Rank_9 ? "9" :
		   StdDeck_RANK(card) == Rank_8 ? "8" :
		   StdDeck_RANK(card) == Rank_7 ? "7" :
		   StdDeck_RANK(card) == Rank_6 ? "6" :
		   StdDeck_RANK(card) == Rank_5 ? "5" :
		   StdDeck_RANK(card) == Rank_4 ? "4" :
		   StdDeck_RANK(card) == Rank_3 ? "3" :
		   StdDeck_RANK(card) == Rank_2 ? "2" :
		   "&nbsp";

	final.Format("<font color=%s>%s%s</font>", color, rank, suit);

	return final;
}
