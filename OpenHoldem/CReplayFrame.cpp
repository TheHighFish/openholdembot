//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#include "StdAfx.h"
#include "CReplayFrame.h"

#include "CFilenames.h"
#include "CHeartbeatThread.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CSessionCounter.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineDealerchair.h"
#include "CSymbolEngineUserchair.h"
#include "CSymbolEngineTableLimits.h"
#include "CTableState.h"
#include "..\CTablemap\CTablemap.h"
#include "OH_MessageBox.h"
#include "OpenHoldem.h"

//  Sanity check: enough disk-space for a replay frame?
//	We assume, 10 MB are enough
//	  - a large table of 1000x1000 pixels a 4 byte
//	  - some KB for the HTML file.
//	  - some space for other processes	
const unsigned int FREE_SPACE_NEEDED_FOR_REPLAYFRAME = 10000000;  

// We always start with 0 amd overwrite existing frames.
// That's more simple and we usually are not interested
// in old frames of bygone sessions.
int CReplayFrame::_next_replay_frame = 0;

CReplayFrame::CReplayFrame(void) {
}

CReplayFrame::~CReplayFrame(void) {
  // As we create a new CReplayFrame for each frame
  // we simply have to increment the global counter once we are finished.
	++_next_replay_frame;
  if (_next_replay_frame >= preferences.replay_max_frames()) {
    // Once we reach the maximum overwrite old frames
    // in cyclic order, starting again from 0.
    _next_replay_frame = 0;
  }
}

void CReplayFrame::CreateReplayFrame(void){
	FILE	*fp = NULL;
	ULARGE_INTEGER	free_bytes_for_user_on_disk = {0}, 
	total_bytes_on_disk = {0}, 
	free_bytes_total_on_disk = {0};
	//  Sanity check: Enough disk-space for replay frame?	
	GetDiskFreeSpaceEx(
		p_filenames->OpenHoldemDirectory(),  //  Directory on disk of interest
		&free_bytes_for_user_on_disk,  
		&total_bytes_on_disk,	
		&free_bytes_total_on_disk);
	if (free_bytes_for_user_on_disk.QuadPart < FREE_SPACE_NEEDED_FOR_REPLAYFRAME) {
		write_log(preferences.debug_replayframes(), "[CReplayFrame] Not enough disk-space\n");
		OH_MessageBox_Error_Warning("Not enough disk space to create replay-frame.");
    return;
	}
	// Get exclusive access to CScraper and CSymbols variables
	// (Wait for scrape/symbol cycle to finish before saving frame)
	EnterCriticalSection(&p_heartbeat_thread->cs_update_in_progress);
	CreateBitMapFile();
  // Create HTML file
	CString path = p_filenames->ReplayHTMLFilename(_next_replay_frame);
	if (fopen_s(&fp, path.GetString(), "w")==0) {
		write_log(preferences.debug_replayframes(), "[CReplayFrame] Creating HTML file: $s\n", path);
		// First line has to be the "title" of the table.
		// This is no longer valid HTML, but the way Ray.E.Bornert did it
		// for WinHoldem and WinScrape.
		fprintf(fp, "%s\n", p_table_state->_title);
    fprintf(fp, "<br>\n");
		// HTML header
		fprintf(fp, "<html>\n");
		fprintf(fp, "  <head>\n");
		fprintf(fp, "    <title>%s</title>\n", p_table_state->_title);
		fprintf(fp, "  </head>");
		fprintf(fp, "<style>\n");
		fprintf(fp, "td {text-align:right;}\n");
		fprintf(fp, "</style>\n");
		fprintf(fp, "<body>\n");
		fprintf(fp, "<font face=courier>\n");
    // Bitmap image
		fprintf(fp, "<img src=\"frame%06d.bmp\">\n", _next_replay_frame);
		fprintf(fp, "<br>\n");
    // Title, data, frame-number, OH-version
    fprintf(fp, "%s", GeneralInfo());
    // Links forwards and backwards to the next frames
		fprintf(fp, "%s", LPCSTR(GetLinksToPrevAndNextFile()));
		fprintf(fp, "<br>\n");
		fprintf(fp, "<br>\n");
    // Header of main table for smaller data tables
		fprintf(fp, "<table>\n");
		fprintf(fp, "<tr>\n");
		fprintf(fp, "<td>\n");
    // Data tables
		fprintf(fp, "%s", LPCSTR(GetPlayerInfoAsHTML()));
		fprintf(fp, "/<td>\n");
		fprintf(fp, "<td>\n");
		fprintf(fp, "%s", LPCSTR(GetButtonStatesAsHTML()));
		fprintf(fp, "%s", LPCSTR(GetBlindInfoAsHTML()));
		fprintf(fp, "%s", LPCSTR(GetCommonCardsAsHTML()));
		fprintf(fp, "%s", LPCSTR(GetPotsAsHTML()));
		fprintf(fp, "</td>\n");
    // Footer of main table
		fprintf(fp, "</tr>\n");
		fprintf(fp, "</table>\n");
    // End of HTML
		fprintf(fp, "</body></html>\n");
    fclose(fp);
	}	
  LeaveCriticalSection(&p_heartbeat_thread->cs_update_in_progress);
}

CString CReplayFrame::GeneralInfo() {
  // Get current time
  time_t    ltime = 0;
	tm        now_time = {0};
	char			now_time_str[100] = {0};
	time(&ltime);
	localtime_s(&now_time, &ltime);
	strftime(now_time_str, 100, "%Y-%m-%d %H:%M:%S", &now_time);

  CString result;
  result += "<table border=4 cellpadding=1 cellspacing=1>\n";
  // Table title
  result += "<tr><td>\n";
	result += p_table_state->_title;
  result += "</td></tr>\n";
	// Session, 
  result += "<tr><td>\n";
  result += "Session: ";
	result += Number2CString(p_sessioncounter->session_id(), 0);
  result += "</td></tr>\n";
  // Frame number 
  result += "<tr><td>\n";
  CString next_frame;
  next_frame.Format("Frame: %06d", _next_replay_frame);
	result += next_frame;
  result += "</td></tr>\n";
  // Time
  result += "<tr><td>\n";
	result += now_time_str;
  result += "</td></tr>\n";
  // Version
  result += "<tr><td>\n";
	result += "OpenHoldem ";
  result += VERSION_TEXT;
  result += "</td></tr>\n";
  // Finish table
  result += "</table>\n";
  return result;
}

CString CReplayFrame::GetCardHtml(unsigned int card) {
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

CString CReplayFrame::GetPlayerInfoAsHTML() {
	CString player_info, text;
	// Table header for: SFABD (seated, active, button, dealt, playing),
	// hand, bet, balance, name
	player_info += "<table border=4 cellpadding=1 cellspacing=1>\n";
	player_info += "  <thead>\n";
	player_info += "    <tr>\n";
	player_info += "      <th>#</th>\n";
	player_info += "      <th>SABDP</th>\n";  // seated, active, button, dealt, playing
	player_info += "      <th>hand</th>\n";
	player_info += "      <th>bet</th>\n";
	player_info += "      <th>balance</th>\n";
	player_info += "      <th>name</th>\n";
	player_info += "    </tr>\n";
	player_info += "  </thead>\n";
	// Table footer for explanations
	player_info += "  <tfoot>\n";
	player_info += "    <tr\n";
	player_info += "      <td colspan=5>SABDP = seated / active / button / dealt / playing</td>\n";
	player_info += "    </tr>\n";
	player_info += "  </tfoot>\n";
	// Table body
	player_info += "  <tbody>\n";
	// One table row per player...
	for (int i=0; i<p_tablemap->nchairs(); i++) {
		player_info += "    <tr>\n";
		// Chair number
		text.Format("      <td>%d</td>\n", i);   
		player_info += text;
		// seated, active, button, dealt, playing
		text = "      <td>";
		text +=	(p_symbol_engine_active_dealt_playing->playersseatedbits() & (1<<i)) ? "s" : "-";
		text +=	(p_symbol_engine_active_dealt_playing->playersactivebits() & (1<<i)) ? "a" : "-";
		text +=	(DEALER_CHAIR == i) ? "b" : "-";
		text +=	(p_symbol_engine_active_dealt_playing->playersdealtbits() & (1<<i)) ? "d" : "-";
		text +=	(p_symbol_engine_active_dealt_playing->playersplayingbits() & (1<<i)) ? "p" : "-";
		text += "</td>\n";
		player_info += text;  
		// Cards
		text.Format("      <td>%s%s</td>\n",
      GetCardHtml(p_table_state->_players[i]._hole_cards[0].GetValue()),
      GetCardHtml(p_table_state->_players[i]._hole_cards[1].GetValue()));
		player_info += text;  
		// Bet
		text.Format("      <td>%11.2f</td>\n", p_table_state->_players[i]._bet);
		player_info += text; 
		// Balance
		text.Format("      <td>%11.2f</td>\n", p_table_state->_players[i]._balance);
		player_info += text;  
		// Name
		text.Format("      <td>%-15s</td>\n", p_table_state->_players[i]._name.GetString());
		player_info += text;
		player_info += "    </tr>\n";
	}
	// End of table body
	player_info += "  </tbody>\n";
	// End of table
	player_info += "</table>\n";
	return player_info;
}

CString CReplayFrame::GetButtonStatesAsHTML() {
	CString button_states;
  // Table header for: FCKRA
	button_states += "<table align=center border=4 cellpadding=1 cellspacing=1>\n";
	button_states += "  <tr><th>FCKRA</th></tr>\n";
	button_states += "  <tr>\n";
	// Buttons
	CString fckra_seen = p_symbol_engine_autoplayer->GetFCKRAString();
	// Table footer
	button_states += "    <td>";
	button_states += fckra_seen.GetString();
	button_states += "</td>\n",
	button_states += "  </tr>\n";
	button_states += "</table>\n";
	return button_states;
}

CString CReplayFrame::GetBlindInfoAsHTML() {
	CString blind_info, text;
	// Table for: sb, bb, BB
	blind_info += "<table align=center border=4 cellpadding=1 cellspacing=1>\n";
	text.Format("  <tr><th>sb</th><td>%11.2f</td></tr>\n", p_symbol_engine_tablelimits->sblind());
	blind_info += text;
	text.Format("  <tr><th>bb</th><td>%11.2f</td></tr>\n", p_symbol_engine_tablelimits->bblind());
	blind_info += text;
	text.Format("  <tr><th>BB</th><td>%11.2f</td></tr>\n", p_symbol_engine_tablelimits->bigbet());
	blind_info += text;
	text.Format("</table>\n");
	blind_info += text;
	return blind_info;
}

CString CReplayFrame::GetCommonCardsAsHTML() {
	CString common_cards, text;
  // Table header for: common cards
	common_cards += "<table align=center border=4 cellpadding=1 cellspacing=1>\n";
	common_cards += "<tr><th>commoncard</th></tr>\n";
	common_cards += "<tr>\n";
	// Common cards
	text.Format("<td>%s%s%s%s%s</td>\n",
    GetCardHtml(p_table_state->_common_cards[0].GetValue()),
    GetCardHtml(p_table_state->_common_cards[1].GetValue()),
    GetCardHtml(p_table_state->_common_cards[2].GetValue()),
    GetCardHtml(p_table_state->_common_cards[3].GetValue()),
    GetCardHtml(p_table_state->_common_cards[4].GetValue()));
	common_cards += text;
	// Table footer
	common_cards += "</tr>\n";
	common_cards += "</table>\n";
  return common_cards;
}

CString CReplayFrame::GetPotsAsHTML() {
	CString pots, text;
  // Table header for: pots
	pots += "<table align=center border=4 cellpadding=1 cellspacing=1>\n";
	pots += "  <tr><th>#</th><th>pot</th></tr>\n";
	pots += "  <tr>\n";
	// Main pot
	text.Format("    <td>0</td><td>%11.2f</td>\n", p_table_state->_pot[0]);
	pots += text;
	pots += "  </tr>\n";
	// Side pots
	for (int i=1; i<k_max_number_of_pots; i++) {
		if (p_table_state->_pot[i]) {
			pots += "  <tr>\n";
			text.Format("    <td>%d</td><td>%11.2f</td>\n", i,p_table_state->_pot[i]);
			pots += text;
			pots += "  </tr>\n";
		}	else {
			break;
		}
	}
	pots += "</table>\n";
	return pots;
}

void CReplayFrame::CreateReplaySessionDirectoryIfNecessary() {
	CString path = p_filenames->ReplaySessionDirectory();
	write_log(preferences.debug_replayframes(), "[CReplayFrame] Creating bitmap file %s\n", path);
	if (GetFileAttributes(path.GetString()) == INVALID_FILE_ATTRIBUTES)	{
		SHCreateDirectoryEx(NULL, path.GetString(), NULL);
	}
}

void CReplayFrame::CreateBitMapFile() {
	CString path;
  CreateReplaySessionDirectoryIfNecessary();
	CreateBMPFile(p_filenames->ReplayBitmapFilename(_next_replay_frame), 
		p_scraper->entire_window_cur());
}

CString CReplayFrame::GetLinksToPrevAndNextFile() {
	CString links, text;
	text.Format("<a href=\"frame%06d.htm\">PREV</a>\n",
		_next_replay_frame-1 >= 0 ? _next_replay_frame-1 : preferences.replay_max_frames());
	links += text;
	text.Format("<a href=\"frame%06d.htm\">NEXT</a>\n",
		_next_replay_frame+1 < preferences.replay_max_frames() ? _next_replay_frame+1 : 0);
	links += text;
	return links;
}
