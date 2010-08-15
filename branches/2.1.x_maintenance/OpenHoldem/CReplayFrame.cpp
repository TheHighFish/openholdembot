#include "StdAfx.h"

#include "CHeartbeatThread.h"
#include "CPreferences.h"
#include "CReplayFrame.h"
#include "CScraper.h"
#include "CSessionCounter.h"
#include "CSymbols.h"
#include "CTableLimits.h"
#include "..\CTablemap\CTablemap.h"
#include "OpenHoldem.h"


//  Sanity check: enough disk-space for a replay frame?
//	We assume, 10 MB are enough
//	  - a large table of 1000x1000 pixels a 4 byte
//	  - some KB for the HTML file.
//	  - some space for other processes	
const unsigned int FREE_SPACE_NEEDED_FOR_REPLAYFRAME = 10000000;  

CReplayFrame::CReplayFrame(void)
{
    CTime		time = 0, latest_time = 0;
    int			last_frame_num = -1, frame_num = 0;
    CString		path, filename, current_path;
    CFileFind	hFile;
    BOOL		bFound = false;

	CSLock lock(m_critsec);

	// Find next replay frame number
    _next_replay_frame = -1;

    path.Format("%s\\replay\\session_%lu\\*.bmp", _startup_path, p_sessioncounter->session_id());
    bFound = hFile.FindFile(path.GetString());
    while (bFound)
    {
        bFound = hFile.FindNextFile();
        if (!hFile.IsDots() && !hFile.IsDirectory())
        {
            filename = hFile.GetFileName();
            hFile.GetLastWriteTime(time);
            sscanf_s(filename.GetString(), "frame%d.bmp", &frame_num);

            if (time>latest_time)
            {
                last_frame_num = frame_num;
                latest_time = time;
            }
        }
    }

    _next_replay_frame = last_frame_num + 1;
    if (_next_replay_frame >= prefs.replay_max_frames())
        _next_replay_frame = 0;
}

CReplayFrame::~CReplayFrame(void)
{
}

void CReplayFrame::CreateReplayFrame(void)
{
	CString			path;
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
		if (!prefs.disable_msgbox())
			MessageBox(NULL, "Not enough disk space to create replay-frame.", "ERROR", 0);

		return;
	}

	// Get current time
	time(&ltime);
	localtime_s(&now_time, &ltime);
	strftime(now_time_str, 100, "%Y-%m-%d %H:%M:%S", &now_time);

	// Get exclusive access to CScraper and CSymbols variables
	// (Wait for scrape/symbol cycle to finish before saving frame)
	EnterCriticalSection(&p_heartbeat_thread->cs_update_in_progress);
	CreateBitMapFile();

	// Create HTML file
	path.Format("%s\\replay\\session_%lu\\frame%06d.htm", _startup_path, p_sessioncounter->session_id(), _next_replay_frame);
	if (fopen_s(&fp, path.GetString(), "w")==0)
	{
		// HTML header
		fprintf(fp, "<html>\n");
		fprintf(fp, "<style>\n");
		fprintf(fp, "td {text-align:right;}\n");
		fprintf(fp, "</style>\n");
		fprintf(fp, "<body>\n");
		fprintf(fp, "<font face=courier>\n");

		// Bitmap image
		fprintf(fp, "<img src=\"frame%06d.bmp\">\n", _next_replay_frame);
		fprintf(fp, "<br>\n");

		// Table title
		fprintf(fp, "%s", p_scraper->title());
		fprintf(fp, "\n");

		// Links forwards and backwards to the next frames
		fprintf(fp, "%s", GetLinksToPrevAndNextFile());

		// Session, frame number and time
		fprintf(fp, " [Session %lu]", p_sessioncounter->session_id());
		fprintf(fp, " [Frame: %06d]", _next_replay_frame);
		fprintf(fp, " [%s]<br>\n", now_time_str);
		fprintf(fp, "<br>\n");

		// Header of main table for smaller data tables
		fprintf(fp, "<table>\n");
		fprintf(fp, "<tr>\n");
		fprintf(fp, "<td>\n");

		// Data tables
		fprintf(fp, "%s", GetPlayerInfoAsHTML());
		fprintf(fp, "%s", GetButtonStatesAsHTML());
		fprintf(fp, "%s", GetBlindInfoAsHTML());
		fprintf(fp, "%s", GetCommonCardsAsHTML());
		fprintf(fp, "%s", GetPotsAsHTML());

		// Footer of main table
		fprintf(fp, "</td>\n");
		fprintf(fp, "</tr>\n");
		fprintf(fp, "</table>\n");
		fprintf(fp, "</body></html>\n");

		fclose(fp);
	}	

	LeaveCriticalSection(&p_heartbeat_thread->cs_update_in_progress);
}

CString CReplayFrame::GetCardHtml(unsigned int card)
{
	CString suit, color, rank, final;

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


CString CReplayFrame::GetPlayerInfoAsHTML()
{
	CString player_info, text;
	
	// Table header for: SFABD (seated, friend, active, button, dealt, playing),
	// hand, bet, balance, name
	player_info += "<table border=4 cellpadding=1 cellspacing=1>\n";
	player_info += "<tr>\n";
	player_info += "<th>#</th>\n";
	player_info += "<th>SFABDP</th>\n";  // seated, friend, active, button, dealt, playing
	player_info += "<th>hand</th>\n";
	player_info += "<th>bet</th>\n";
	player_info += "<th>balance</th>\n";
	player_info += "<th>name</th>\n";
	player_info += "</tr>\n";

	// One table row per player...
	for (int i=0; i<p_tablemap->nchairs(); i++)
	{
		player_info += "<tr>\n";
		// Chair number
		player_info += "<td>%d</td>", i;   
		// seated, friend, active, button, dealt, playing
		text.Format("%s%s%s%s%s%s",
			(int) (p_symbols->sym()->playersseatedbits) & (1<<i) ? "s" : "-",
			p_symbols->sym()->userchair == i ? "f" : "-",
			(int) (p_symbols->sym()->playersactivebits) & (1<<i) ? "a" : "-",
			p_symbols->sym()->dealerchair== i ? "b" : "-",
			(int) (p_symbols->sym()->playersdealtbits) & (1<<i) ? "d" : "-",
			(int) (p_symbols->sym()->playersplayingbits) & (1<<i) ? "p" : "-");
		player_info += "<td>%s</td>", text.GetString();  
		// Cards
		player_info += "<td>%s%s</td>",
			GetCardHtml(p_scraper->card_player(i, 0)),
			GetCardHtml(p_scraper->card_player(i, 1));  
		// Bet
		player_info += "<td>%11.2f</td>", p_scraper->player_bet(i); 
		// Balance
		player_info += "<td>%11.2f</td>", p_scraper->player_balance(i);  
		// Name
		player_info += "<td>%-15s</td>\n", p_scraper->player_name(i).GetString();  
		player_info += "</tr>\n";
	}
	// Table footer for explanations
	player_info += "<tfoot>\n";
	player_info += "<tr colspan=6>\n";
	player_info += "SFABDP = seated / friend / active / button / dealt / playing\n";
	player_info += "</tr>\n";
	player_info += "</tfoot>\n";
	// End of table
	player_info += "</table>\n";
	player_info += "</td>\n";
	return player_info;
}

CString CReplayFrame::GetButtonStatesAsHTML()
{
	CString button_states, fckra_seen;

	// Table header for: FCKRA
	button_states += "<td>\n";
	button_states += "<table align=center border=4 cellpadding=1 cellspacing=1>\n";
	button_states += "<tr><th>FCRA</th></tr>\n";
	button_states += "<tr>\n";
	// Buttons
	int sym_myturnbits = (int) p_symbols->sym()->myturnbits;
	fckra_seen.Format("%s%s%s%s%s",
		sym_myturnbits&0x01 ? "F" : ".",
		sym_myturnbits&0x02 ? "C" : ".",
		// Check button out of order to stay consistent
		// with button order in manual mode.
		sym_myturnbits&0x10 ? "K" : ".",
		sym_myturnbits&0x04 ? "R" : ".",
		sym_myturnbits&0x08 ? "A" : ".");
	// Table footer
	button_states += "<td>%s</td>\n", fckra_seen.GetString();
	button_states += "</tr>\n";
	button_states += "</table>\n";
	return button_states;
}

CString CReplayFrame::GetBlindInfoAsHTML()
{
	CString blind_info;
	// Table for: sb, bb, BB
	blind_info += "<table align=center border=4 cellpadding=1 cellspacing=1>\n";
	blind_info += "<tr><th>sb</th><td>%11.2f</td></tr>\n", p_tablelimits->sblind();
	blind_info += "<tr><th>bb</th><td>%11.2f</td></tr>\n", p_tablelimits->bblind();
	blind_info += "<tr><th>BB</th><td>%11.2f</td></tr>\n", p_tablelimits->bbet();
	blind_info += "</table>\n";
	return blind_info;
}

CString CReplayFrame::GetCommonCardsAsHTML()
{
	CString common_cards;

	// Table header for: common cards
	common_cards += "<table align=center border=4 cellpadding=1 cellspacing=1>\n";
	common_cards += "<tr><th>commoncard</th></tr>\n";
	common_cards += "<tr>\n";
	// Common cards
	common_cards += "<td>%s%s%s%s%s</td>\n",
		GetCardHtml(p_scraper->card_common(0)),
		GetCardHtml(p_scraper->card_common(1)),
		GetCardHtml(p_scraper->card_common(2)),
		GetCardHtml(p_scraper->card_common(3)),
		GetCardHtml(p_scraper->card_common(4));
	// Table footer
	common_cards += "</tr>\n";
	common_cards += "</table>\n";

	return common_cards;
}

CString CReplayFrame::GetPotsAsHTML()
{
	CString pots;

	// Table header for: pots
	pots += "<table align=center border=4 cellpadding=1 cellspacing=1>\n";
	pots += "<tr><th>#</th><th>pot</th></tr>\n";
	pots += "<tr>\n";
	// Main pot
	pots += "<td>0</td><td>%11.2f</td>\n", p_scraper->pot(0);
	pots += "</tr>\n";
	// Side pots
	for (int i=1; i<k_max_number_of_pots; i++)
	{
		if (p_scraper->pot(i))
		{
			pots += "<tr>\n";
			pots += "<td>%d</td><td>%11.2f</td>\n", i, p_scraper->pot(i);
			pots += "</tr>\n";
		}
		else
		{
			break;
		}
	}
	pots += "</table>\n";
	return pots;
}

void CReplayFrame::CreateBitMapFile()
{
	CString path;

	// Create replay/session dir if it does not exist
	path.Format("%s\\replay\\session_%lu\\", _startup_path, p_sessioncounter->session_id());
	if (GetFileAttributes(path.GetString()) == INVALID_FILE_ATTRIBUTES)
		SHCreateDirectoryEx(NULL, path.GetString(), NULL);

	// Create bitmap file
	path.Format("%s\\replay\\session_%lu\\frame%06d.bmp", _startup_path, p_sessioncounter->session_id(), _next_replay_frame);
	CreateBMPFile(path.GetString(), p_scraper->entire_window_cur());
}

CString CReplayFrame::GetLinksToPrevAndNextFile()
{
	CString links;
	links += "<a href=\"frame%06d.htm\">PREV</a>\n",
		_next_replay_frame-1 >= 0 ? _next_replay_frame-1 : prefs.replay_max_frames();
	links += "<a href=\"frame%06d.htm\">NEXT</a>\n",
		_next_replay_frame+1 < prefs.replay_max_frames() ? _next_replay_frame+1 : 0;
	return links;
}