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

// OpenHoldemView.cpp : implementation of the COpenHoldemView class
//

#include "stdafx.h"
#include "OpenHoldemView.h"

#include "CAutoplayerTrace.h"
#include "CEngineContainer.h"
#include "CHandresetDetector.h"
#include "CHeartbeatThread.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CScrapedActionInterface.h"
#include "CStringMatch.h"
#include "CSymbolengineAutoplayer.h"
#include "CSymbolengineChipAmounts.h"
#include "CSymbolengineColourCodes.h"
#include "CSymbolengineGameType.h"
#include "CSymbolEngineIsTournament.h"
#include "CSymbolEngineTableLimits.h"
#include "..\CTablemap\CTablemap.h"
#include "CTableState.h"
#include "CWhiteInfoBox.h"
#include "MagicNumbers.h"
#include "MainFrm.h"
#include "OpenHoldem.h"
#include "OpenHoldemDoc.h"

// Table layouts
int		cc[k_number_of_community_cards][2] = 
{ 
	{-(CARDSIZEX*2 + 3*2 + CARDSIZEX/2), -(CARDSIZEY/2)},	
	{-(CARDSIZEX*1 + 3*1 + CARDSIZEX/2), -(CARDSIZEY/2)},
	{-(CARDSIZEX*0 + 3*0 + CARDSIZEX/2), -(CARDSIZEY/2)},
	{+(CARDSIZEX*0 + 3*1 + CARDSIZEX/2), -(CARDSIZEY/2)},
	{+(CARDSIZEX*1 + 3*2 + CARDSIZEX/2), -(CARDSIZEY/2)}
};

// Player locations as a percentage of width/height
// [nplayers][chairnum][x/y]
double	pc[k_max_number_of_players+1][k_max_number_of_players][k_number_of_cards_per_player] = {
  // 0 chairs
	{ {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },	
  // 1 chair
	{ {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },	
  // 2 chairs
	{ {.95,.47}, {.05,.47}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },	
  // 3 chairs
	{ {.95,.47}, {.50,.83}, {.05,.47}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },	
  // 4 chairs
	{ {.89,.25}, {.89,.69}, {.11,.69}, {.11,.25}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },	
  // 5 chairs
	{ {.89,.25}, {.89,.69}, {.50,.83}, {.11,.69}, {.11,.25}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },
  // 6 chairs
	{ {.72,.11}, {.95,.47}, {.72,.83}, {.28,.83}, {.05,.47}, {.28,.11}, {0,0}, {0,0}, {0,0}, {0,0} },	
  // 7 chairs
	{ {.72,.11}, {.95,.47}, {.72,.83}, {.50,.83}, {.28,.83}, {.05,.47}, {.28,.11}, {0,0}, {0,0}, {0,0} },	
  // 8 chairs
	{ {.72,.11}, {.89,.25}, {.89,.69}, {.72,.83}, {.28,.83}, {.11,.69}, {.11,.25}, {.28,.11}, {0,0}, {0,0} },	
  // 9 chairs
	{ {.72,.11}, {.89,.25}, {.89,.69}, {.72,.83}, {.50,.83}, {.28,.83}, {.11,.69}, {.11,.25}, {.28,.11}, {0,0} },	
  // 10 chairs
	{ {.72,.11}, {.85,.21}, {.95,.47}, {.85,.73}, {.72,.83}, {.28,.83}, {.15,.73}, {.05,.47}, {.15,.21}, {.28,.11} }	
};

// Player bet locations relative to player locations above
// numbers are in pixel units
// [nplayers][chairnum][x/y]
int pcbet[11][10][2] = 
{
	{ {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },	// 0 players
	{ {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },	// 1 player
	{ {-40,+0}, {+40,+0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },	// 2 players
	{ {-40,+0}, {+0,-45}, {+40,+0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0} }, // 3 players
	{ {-40,+37}, {-40,-20}, {+40,-20}, {+40,+37}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0} }, // 4 players
	{ {-40,+37}, {-40,-20}, {+0,-45}, {+40,-20}, {+40,+37},  {0,0}, {0,0}, {0,0}, {0,0}, {0,0} }, // 5 players
	{ {-40,+40}, {-40,+0}, {-40,+40}, {+40,-40}, {+40,+0}, {+40,+40}, {0,0}, {0,0}, {0,0}, {0,0} },	// 6 players
	{ {-40,+40}, {-40,+0}, {-40,+40}, {+0,-45}, {+40,-40}, {+40,+0}, {+40,+40}, {0,0}, {0,0}, {0,0} }, // 7 players
	{ {-40,+40}, {-40,+37}, {-40,-20}, {-40,-40}, {+40,-40}, {+40,-20}, {+40,+37}, {+40,+40}, {0,0}, {0,0} }, // 8 players
	{ {-40,+58}, {-40,+37}, {-40,-20}, {-40,-40}, {+0,-45}, {+40,-40}, {+40,-20}, {+40,+37}, {+40,+58}, {0,0} }, // 9 players
	{ {-40,+40}, {-40,+37}, {-40,+0}, {-40,-20}, {-40,-40}, {+40,-40}, {+40,-20}, {+40,+0}, {+40,+37}, {+40,+40} } // 10 players
};

// COpenHoldemView
IMPLEMENT_DYNCREATE(COpenHoldemView, CView)

BEGIN_MESSAGE_MAP(COpenHoldemView, CView)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// COpenHoldemView construction/destruction
COpenHoldemView::COpenHoldemView() {
	_black_pen.CreatePen(PS_SOLID, 1, COLOR_BLACK);
	_green_pen.CreatePen(PS_SOLID, 1, COLOR_GREEN);
	_red_pen.CreatePen(PS_SOLID, 1, COLOR_RED);
	_blue_pen.CreatePen(PS_SOLID, 1, COLOR_BLUE);
	_white_dot_pen.CreatePen(PS_DOT, 1, COLOR_WHITE);
	_null_pen.CreatePen(PS_NULL, 0, COLOR_BLACK);

	_white_brush.CreateSolidBrush(COLOR_WHITE);
	_gray_brush.CreateSolidBrush(COLOR_GRAY);
	_red_brush.CreateSolidBrush(COLOR_RED);
	_yellow_brush.CreateSolidBrush(COLOR_YELLOW);

	_logfont.lfWidth = 0;
	_logfont.lfEscapement = 0;
	_logfont.lfOrientation = 0;
	_logfont.lfItalic = 0;
	_logfont.lfUnderline = 0;
	_logfont.lfStrikeOut = 0;
	_logfont.lfCharSet = 0;
	_logfont.lfOutPrecision = 0;
	_logfont.lfClipPrecision = 0;
	_logfont.lfQuality = PROOF_QUALITY;
	_logfont.lfPitchAndFamily = 0;
	strcpy_s(_logfont.lfFaceName, 32, "Times New Roman");

	_handnumber_last = "";
	_sblind_last = _bblind_last = _lim_last = _ante_last = _pot_last = 0.;
	_iterator_thread_progress_last = 0;
	
	for (int i = 0; i<k_number_of_community_cards; i++)
		_card_common_last[i] = CARD_UNDEFINED;

	for (int i = 0; i<k_max_number_of_players ; i++)
	{
		_seated_last[i] = false; 
    _active_last[i] = false;
    _playername_last[i] = "";
		_dealer_last[i] = false;
		_playerbalance_last[i] = _playerbet_last[i] = 0.;
		for (int j=0; j<k_number_of_cards_per_player; j++)
		{
			_card_player_last[i][j] = CARD_NOCARD;
		}
	}
		
	_istournament_last = false;
}

COpenHoldemView::~COpenHoldemView() {
}

BOOL COpenHoldemView::PreCreateWindow(CREATESTRUCT& cs) {
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CView::PreCreateWindow(cs);
}

void COpenHoldemView::OnInitialUpdate() {
	CView::OnInitialUpdate();

	// Timer to check for display updates
	SetTimer(DISPLAY_UPDATE_TIMER, 103, 0);
}

// COpenHoldemView drawing
void COpenHoldemView::OnDraw(CDC* pDC) {
	UpdateDisplay(true);
}

void COpenHoldemView::OnTimer(UINT nIDEvent) {
	if (nIDEvent == DISPLAY_UPDATE_TIMER) 
	{
		// Only do this if we are not in the middle of a scraper/symbol update
		if (TryEnterCriticalSection(&p_heartbeat_thread->cs_update_in_progress))
		{
			UpdateDisplay(false);
			LeaveCriticalSection(&p_heartbeat_thread->cs_update_in_progress);
		}
	}
	// Otherwise: continue with parent class
	CView::OnTimer(nIDEvent);
}

void COpenHoldemView::UpdateDisplay(const bool update_all) {
	bool		update_it = false;
	CDC			*pDC = GetDC();

	CString sym_handnumber = SYM->p_handreset_detector()->GetHandNumber();
	double  sym_bblind = SYM->p_symbol_engine_tablelimits()->bblind();
	double  sym_sblind = SYM->p_symbol_engine_tablelimits()->sblind();
	double  sym_ante = SYM->p_symbol_engine_tablelimits()->ante();
	int     sym_lim = SYM->p_symbol_engine_gametype()->gametype();
	bool    sym_istournament = SYM->p_symbol_engine_istournament()->istournament();
	double  sym_pot = SYM->p_symbol_engine_chip_amounts()->pot();

	// Get size of current client window
	GetClientRect(&_client_rect);

	// Set background color (light gray)
	if (update_all) 
	{
		CBrush backBrush(COLOR_GRAY);
		CBrush* pOldBrush = pDC->SelectObject(&backBrush);
		pDC->PatBlt(_client_rect.left, _client_rect.top, _client_rect.right-_client_rect.left, _client_rect.bottom-_client_rect.top, PATCOPY);
		pDC->SelectObject(pOldBrush);
	}

	// Draw center info box
	update_it = false;
	if (_handnumber_last != sym_handnumber) 
	{
		_handnumber_last = sym_handnumber;
		update_it = true;
	}
	if (_sblind_last != sym_sblind) 
	{
		_sblind_last = sym_sblind;
		update_it = true;
	}
	if (_bblind_last != sym_bblind) 
	{
		_bblind_last = sym_bblind;
		update_it = true;
	}
	if (_lim_last != sym_lim) 
	{
		_lim_last = sym_lim;
		update_it = true;
	}
	if (_istournament_last != sym_istournament) 
	{
		_istournament_last = sym_istournament;
		update_it = true;
	}
	if (_ante_last != sym_ante != 0) 
	{
		_ante_last = sym_ante;
		update_it = true;
	}
	if (_pot_last != sym_pot) 
	{
		_pot_last = sym_pot;
		update_it = true;
	}

  if ((SYM->p_symbol_engine_autoplayer()->ismyturn()) || update_it || update_all) 
	{
		assert(GUI->p_white_info_box()-> != NULL);
    GUI->p_white_info_box()->Draw(_client_rect, _logfont, pDC,
      &_black_pen, &_white_brush);
    ReleaseDC(pDC);
	}

	// Draw button state indicators
	DrawButtonIndicators();

	// Draw common cards
	for (int i=0; i<k_number_of_community_cards; i++) 
	{
    Card *p_card = &p_table_state->_common_cards[i];
    int card_value = p_table_state->_common_cards[i].GetValue();
		if (_card_common_last[i] != card_value || update_all) 
		{
			_card_common_last[i] = card_value;
			write_log(MAIN->p_preferences()->debug_gui(), "[GUI] COpenHoldemView::UpdateDisplay() Drawing common card %i: [%s]\n",
        i, p_card->ToString());
			DrawCard(p_card,
					  _client_rect.right/2 + cc[i][0], _client_rect.bottom/2 + cc[i][1],
					  _client_rect.right/2 + cc[i][0] + CARDSIZEX, _client_rect.bottom/2 + cc[i][1] + CARDSIZEY,
					  false);
		}
	}
  // Draw collection of player info
	for (int i=0; i<p_tablemap->nchairs(); i++) 	{
		write_log(MAIN->p_preferences()->debug_gui(), "[GUI] COpenHoldemView::UpdateDisplay() checking changes for chair %i\n", i);
		// Figure out if we need to redraw this seat
		update_it = false;
		if (_seated_last[i] != p_table_state->_players[i].seated() 
        || _active_last[i] != p_table_state->_players[i].active()) 	{
			_seated_last[i] = p_table_state->_players[i].seated();
			_active_last[i] = p_table_state->_players[i].active();
			update_it = true;
		}
    if (_card_player_last[i][0] != p_table_state->_players[i]._hole_cards[0].GetValue()
        || _card_player_last[i][1] != p_table_state->_players[i]._hole_cards[1].GetValue()) 		{
			_card_player_last[i][0] = p_table_state->_players[i]._hole_cards[0].GetValue();
			_card_player_last[i][1] = p_table_state->_players[i]._hole_cards[1].GetValue();
			update_it = true;
		}
		if (_dealer_last[i] != p_table_state->_players[i].dealer()) {
			_dealer_last[i] = p_table_state->_players[i].dealer();
			update_it = true;
		}
		if (_playername_last[i] != p_table_state->_players[i]._name) {
			_playername_last[i] = p_table_state->_players[i]._name;
			update_it = true;
		}
		if (_playerbalance_last[i] != p_table_state->_players[i]._balance) {
			_playerbalance_last[i] = p_table_state->_players[i]._balance;
			update_it = true;
		}
		if (_playerbet_last[i] != p_table_state->_players[i]._bet) 
		{
			_playerbet_last[i] = p_table_state->_players[i]._bet;
			update_it = true;
		}

		if (update_it || update_all) {
			write_log(MAIN->p_preferences()->debug_gui(), "[GUI] COpenHoldemView::UpdateDisplay() updating chair %i\n", i);
			// Draw active circle
			if (p_table_state->_players[i].seated()) 	{
				DrawSeatedActiveCircle(i);
				// Draw cards first, because we want the name 
				// to occlude the cards and not the other way.
				DrawPlayerCards(i);
				DrawNameBox(i);
				DrawBalanceBox(i);
        DrawColourCodes(i);
			}
			// Drawing a bet, even if no player seated.
			// The player might have left the table, 
			// but depending on casinos potmethod a bet might still be there.
			DrawPlayerBet(i);
		}
		// Draw dealer button
		// At some casinos the dealer can be at an empty seat.
		// Therefore we draw the dealer-button anyway, inependent of "seated" and "active".
		// Draw it at the very last, as we want to have it at the top of the cards.
		if (p_table_state->_players[i].dealer()) {
			DrawDealerButton(i);
		}
	}
	write_log(MAIN->p_preferences()->debug_gui(), "[GUI] COpenHoldemView::UpdateDisplay() Update finished\n");
	ReleaseDC(pDC);
	write_log(MAIN->p_preferences()->debug_gui(), "[GUI] COpenHoldemView::UpdateDisplay() DC released\n");
}

void COpenHoldemView::DrawButtonIndicators(void) {
	bool		fold_drawn, call_drawn, check_drawn, raise_drawn, allin_drawn;
	bool		autopost_drawn, sitin_drawn, sitout_drawn, leave_drawn, prefold_drawn = false;

	autopost_drawn = sitin_drawn = sitout_drawn = leave_drawn = prefold_drawn = false;
	fold_drawn = call_drawn = check_drawn = raise_drawn = allin_drawn = false;

	for (int i=0; i<k_max_number_of_players; i++) 
	{
		// Draw "on" buttons
		if (p_scraper->GetButtonState(i)) 
		{
			if (CStringMatch::IsStringFold(p_table_state->_SCI._button_label[i])) 
			{
				DrawSpecificButtonIndicator(i, 'F', _client_rect.right-84, _client_rect.bottom-16, _client_rect.right-70, _client_rect.bottom-2);
				fold_drawn = true;
			}
			else if (CStringMatch::IsStringCall(p_table_state->_SCI._button_label[i])) 
			{
				DrawSpecificButtonIndicator(i, 'C', _client_rect.right-67, _client_rect.bottom-16, _client_rect.right-53, _client_rect.bottom-2);
				call_drawn = true;
			}
			else if (CStringMatch::IsStringCheck(p_table_state->_SCI._button_label[i])) 
			{
				DrawSpecificButtonIndicator(i, 'K', _client_rect.right-50, _client_rect.bottom-16, _client_rect.right-36, _client_rect.bottom-2);
				check_drawn = true;
			}
			else if (CStringMatch::IsStringRaise(p_table_state->_SCI._button_label[i])) 
			{
				DrawSpecificButtonIndicator(i, 'R', _client_rect.right-33, _client_rect.bottom-16, _client_rect.right-19, _client_rect.bottom-2);
				raise_drawn = true;
			}
			else if (CStringMatch::IsStringAllin(p_table_state->_SCI._button_label[i])) 
			{
				DrawSpecificButtonIndicator(i, 'A', _client_rect.right-16, _client_rect.bottom-16, _client_rect.right-2, _client_rect.bottom-2);
				allin_drawn = true;
			}
			else if (CStringMatch::IsStringAutopost(p_table_state->_SCI._button_label[i])) 
			{
				DrawSpecificButtonIndicator(i, 'T', _client_rect.left+2,  _client_rect.bottom-16, _client_rect.left+16, _client_rect.bottom-2);
				autopost_drawn = true;
			}
			else if (CStringMatch::IsStringSitin(p_table_state->_SCI._button_label[i])) 
			{
				DrawSpecificButtonIndicator(i, 'I', _client_rect.left+19, _client_rect.bottom-16, _client_rect.left+33, _client_rect.bottom-2);
				sitin_drawn = true;
			}
			else if (CStringMatch::IsStringSitout(p_table_state->_SCI._button_label[i])) 
			{
				DrawSpecificButtonIndicator(i, 'O', _client_rect.left+36, _client_rect.bottom-16, _client_rect.left+50, _client_rect.bottom-2);
				sitout_drawn = true;
			}
			else if (CStringMatch::IsStringLeave(p_table_state->_SCI._button_label[i])) 
			{
				DrawSpecificButtonIndicator(i, 'L', _client_rect.left+53, _client_rect.bottom-16, _client_rect.left+67, _client_rect.bottom-2);
				leave_drawn = true;
			}
			else if (CStringMatch::IsStringPrefold(p_table_state->_SCI._button_label[i])) 
			{
				DrawSpecificButtonIndicator(i, 'P', _client_rect.left+70, _client_rect.bottom-16, _client_rect.left+84, _client_rect.bottom-2);
				prefold_drawn = true;
			}
		}
	}

	// Draw "off" buttons
	if (!fold_drawn) 
		DrawSpecificButtonIndicator(-1, 'F', _client_rect.right-84, _client_rect.bottom-16, _client_rect.right-70, _client_rect.bottom-2);

	if (!call_drawn) 
		DrawSpecificButtonIndicator(-1, 'C', _client_rect.right-67, _client_rect.bottom-16, _client_rect.right-53, _client_rect.bottom-2);

	if (!check_drawn)
		DrawSpecificButtonIndicator(-1, 'K', _client_rect.right-50, _client_rect.bottom-16, _client_rect.right-36, _client_rect.bottom-2);

	if (!raise_drawn)
		DrawSpecificButtonIndicator(-1, 'R', _client_rect.right-33, _client_rect.bottom-16, _client_rect.right-19, _client_rect.bottom-2);

	if (!allin_drawn)
		DrawSpecificButtonIndicator(-1, 'A', _client_rect.right-16, _client_rect.bottom-16, _client_rect.right-2, _client_rect.bottom-2);

	if (!autopost_drawn) 
		DrawSpecificButtonIndicator(-1, 'T', _client_rect.left+2,  _client_rect.bottom-16, _client_rect.left+16, _client_rect.bottom-2);

	if (!sitin_drawn) 
		DrawSpecificButtonIndicator(-1, 'I', _client_rect.left+19, _client_rect.bottom-16, _client_rect.left+33, _client_rect.bottom-2);

	if (!sitout_drawn)
		DrawSpecificButtonIndicator(-1, 'O', _client_rect.left+36, _client_rect.bottom-16, _client_rect.left+50, _client_rect.bottom-2);

	if (!leave_drawn)
		DrawSpecificButtonIndicator(-1, 'L', _client_rect.left+53, _client_rect.bottom-16, _client_rect.left+67, _client_rect.bottom-2);

	if (!prefold_drawn)
		DrawSpecificButtonIndicator(-1, 'P', _client_rect.left+70, _client_rect.bottom-16, _client_rect.left+84, _client_rect.bottom-2);
}

void COpenHoldemView::DrawSpecificButtonIndicator(const int button_num, const char ch, const int left, 
												  const int top, const int right, const int bottom) {
	CPen		*pTempPen = NULL, oldpen;
	CBrush		*pTempBrush = NULL, oldbrush;
	RECT		rect = {0};
	CFont		*oldfont = NULL, cFont;
	CString		t = "";
	CDC			*pDC = GetDC();

	// Set font basics
	_logfont.lfHeight = -12;
	_logfont.lfWeight = FW_NORMAL;
	cFont.CreateFontIndirect(&_logfont);
	oldfont = pDC->SelectObject(&cFont);

	// Background color
	pDC->SetBkColor(COLOR_GRAY);

	if (button_num == k_undefined) 
	{
		pTempPen = (CPen*)pDC->SelectObject(&_white_dot_pen);
		pTempBrush = (CBrush*)pDC->SelectObject(&_gray_brush);
		pDC->SetTextColor(COLOR_WHITE);
	}
	else 
	{
		if (p_scraper->GetButtonState(button_num)) 
		{
			if (ch=='F') 
			{
				pTempPen = (CPen*)pDC->SelectObject(&_red_pen);
				pTempBrush = (CBrush*)pDC->SelectObject(&_white_brush);
				pDC->SetTextColor(COLOR_RED);
			}
			else if (ch=='C') 
			{
				pTempPen = (CPen*)pDC->SelectObject(&_blue_pen);
				pTempBrush = (CBrush*)pDC->SelectObject(&_white_brush);
				pDC->SetTextColor(COLOR_BLUE);
			}
			else if (ch=='K') 
			{
				pTempPen = (CPen*)pDC->SelectObject(&_blue_pen);
				pTempBrush = (CBrush*)pDC->SelectObject(&_white_brush);
				pDC->SetTextColor(COLOR_BLUE);
			}
			else if (ch=='R') 
			{
				pTempPen = (CPen*)pDC->SelectObject(&_green_pen);
				pTempBrush = (CBrush*)pDC->SelectObject(&_white_brush);
				pDC->SetTextColor(COLOR_GREEN);
			}
			else if (ch=='A') 
			{
				pTempPen = (CPen*)pDC->SelectObject(&_black_pen);
				pTempBrush = (CBrush*)pDC->SelectObject(&_white_brush);
				pDC->SetTextColor(COLOR_BLACK);
			}
			else if (ch=='T') 
			{
				pTempPen = (CPen*)pDC->SelectObject(&_black_pen);
				pTempBrush = (CBrush*)pDC->SelectObject(&_white_brush);
				pDC->SetTextColor(COLOR_BLUE);
			}
			else if (ch=='I') 
			{
				pTempPen = (CPen*)pDC->SelectObject(&_black_pen);
				pTempBrush = (CBrush*)pDC->SelectObject(&_white_brush);
				pDC->SetTextColor(COLOR_GREEN);
			}
			else if (ch=='O') 
			{
				pTempPen = (CPen*)pDC->SelectObject(&_black_pen);
				pTempBrush = (CBrush*)pDC->SelectObject(&_white_brush);
				pDC->SetTextColor(COLOR_YELLOW);
			}
			else if (ch=='L') 
			{
				pTempPen = (CPen*)pDC->SelectObject(&_black_pen);
				pTempBrush = (CBrush*)pDC->SelectObject(&_white_brush);
				pDC->SetTextColor(COLOR_RED);
			}
			else if (ch=='P') 
			{
				pTempPen = (CPen*)pDC->SelectObject(&_black_pen);
				pTempBrush = (CBrush*)pDC->SelectObject(&_white_brush);
				pDC->SetTextColor(COLOR_BLACK);
			}
		}
		else 
		{
			pTempPen = (CPen*)pDC->SelectObject(&_white_dot_pen);
			pTempBrush = (CBrush*)pDC->SelectObject(&_gray_brush);
			pDC->SetTextColor(COLOR_WHITE);
		}
	}

	oldpen.FromHandle((HPEN)pTempPen);					// Save old pen
	oldbrush.FromHandle((HBRUSH)pTempBrush);			// Save old brush

	pDC->SetBkMode(OPAQUE);
	pDC->Rectangle(left, top, right, bottom);

	// Set rectangle
	rect.left = left;
	rect.top = top;
	rect.right = right;
	rect.bottom = bottom;

	t.Format("%c", ch);
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(t.GetString(), t.GetLength(), &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	// Restore original pen and brush
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
	pDC->SelectObject(oldfont);
	cFont.DeleteObject();
	ReleaseDC(pDC);
}

void COpenHoldemView::DrawSeatedActiveCircle(const int chair) {
	CPen		*pTempPen = NULL, oldpen;
	CBrush	*pTempBrush = NULL, oldbrush;
	int			left = 0, top = 0, right = 0, bottom = 0;
	CDC			*pDC = GetDC();

	// Background color
	pDC->SetBkColor(COLOR_GRAY);

	// Figure placement of circle
	left = _client_rect.right * pc[p_tablemap->nchairs()][chair][0] - CARDSIZEX - 6;
	top = _client_rect.bottom * pc[p_tablemap->nchairs()][chair][1] - CARDSIZEX - 5;
	right = _client_rect.right * pc[p_tablemap->nchairs()][chair][0] + CARDSIZEX + 5;
	bottom = _client_rect.bottom * pc[p_tablemap->nchairs()][chair][1] + CARDSIZEX + 5;

	pTempPen = (CPen*)pDC->SelectObject(&_black_pen);
	oldpen.FromHandle((HPEN)pTempPen);					// Save old pen

  if (p_table_state->_players[chair].active())	{
		pTempBrush = (CBrush*)pDC->SelectObject(&_white_brush);
	}	else 	{
		pTempBrush = (CBrush*)pDC->SelectObject(&_gray_brush);
	}
	oldbrush.FromHandle((HBRUSH)pTempBrush);			// Save old brush

	pDC->Ellipse(left, top, right, bottom);

	// Restore original pen and brush
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
	ReleaseDC(pDC);
}

void COpenHoldemView::DrawDealerButton(const int chair) {
	CPen		*pTempPen = NULL, oldpen;
	CBrush	*pTempBrush = NULL, oldbrush;
	int			left = 0, top = 0, right = 0, bottom = 0;
	CDC			*pDC = GetDC();

	// Background color
	pDC->SetBkColor(COLOR_GRAY);

	// Figure placement of circle
	left = _client_rect.right * pc[p_tablemap->nchairs()][chair][0] - 8;
	top = _client_rect.bottom * pc[p_tablemap->nchairs()][chair][1] - 8;
	right = _client_rect.right * pc[p_tablemap->nchairs()][chair][0] + 8;
	bottom = _client_rect.bottom * pc[p_tablemap->nchairs()][chair][1] + 8;

  pTempPen = (CPen*)pDC->SelectObject(&_black_pen);
	oldpen.FromHandle((HPEN)pTempPen);					// Save old pen
	pTempBrush = (CBrush*)pDC->SelectObject(&_red_brush);
	oldbrush.FromHandle((HBRUSH)pTempBrush);			// Save old brush

	pDC->Ellipse(left, top, right, bottom);

	// Restore original pen and brush
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
	ReleaseDC(pDC);
}

void COpenHoldemView::DrawCard(Card *card, const int left, const int top, 
							   const int right, const int bottom, const bool pl_card) {
	CPen		*pTempPen = NULL, oldpen;
	CBrush	*pTempBrush = NULL, oldbrush;
	RECT		rrect = {0}, srect = {0};
	CFont		*oldfont = NULL, cFont;
	CDC			*pDC = GetDC();

	// Set font basics
	_logfont.lfHeight = -24;
	_logfont.lfWeight = FW_BOLD;
	cFont.CreateFontIndirect(&_logfont);
	oldfont = pDC->SelectObject(&cFont);

	// Background color
	pDC->SetBkColor(COLOR_GRAY);

	// CARD BACK
  if (card->IsCardBack()) 
	{
		pTempPen = (CPen*)pDC->SelectObject(&_black_pen);
		oldpen.FromHandle((HPEN)pTempPen);					// Save old pen
		pTempBrush = (CBrush*)pDC->SelectObject(&_yellow_brush);
		oldbrush.FromHandle((HBRUSH)pTempBrush);			// Save old brush

		pDC->SetBkMode(OPAQUE);
		pDC->RoundRect(left, top, right, bottom, 5, 5);

		// Restore original pen and brush
		pDC->SelectObject(oldpen);
		pDC->SelectObject(oldbrush);
	}

	// NO CARD
  else if (card->IsNoCard())
	{
		pTempPen = (CPen*)pDC->SelectObject(&_white_dot_pen);
		oldpen.FromHandle((HPEN)pTempPen);					// Save old pen
		pTempBrush = (CBrush*)pDC->SelectObject(&_gray_brush);
		oldbrush.FromHandle((HBRUSH)pTempBrush);			// Save old brush

		pDC->SetBkMode(OPAQUE);
		pDC->RoundRect(left, top, right, bottom, 5, 5);

		// Restore original pen and brush
		pDC->SelectObject(oldpen);
		pDC->SelectObject(oldbrush);
	}

	// NORMAL CARD
	else 
	{
		// Rect for rank and suit location
		rrect.left = left;
		rrect.top = top;
		rrect.right = right;
		rrect.bottom = top + ((bottom-top)/2);
		srect.left = left;
		srect.top = top + ((bottom-top)/2) + 1 - (pl_card ? 7 : 0);
		srect.right = right;
		srect.bottom = bottom - (pl_card ? 7 : 0);

		// Draw card rect and suit
		pTempBrush = (CBrush*)pDC->SelectObject(&_white_brush);
		oldbrush.FromHandle((HBRUSH)pTempBrush);			// Save old brush

		// Set colors
    switch (card->GetSuit())
		{
			case Suit_CLUBS:
				pDC->SetTextColor(COLOR_GREEN);
				pTempPen = (CPen*)pDC->SelectObject(&_green_pen);
				break;
			case Suit_DIAMONDS:
				pDC->SetTextColor(COLOR_BLUE);
				pTempPen = (CPen*)pDC->SelectObject(&_blue_pen);
				break;
			case Suit_HEARTS:
				pDC->SetTextColor(COLOR_RED);
				pTempPen = (CPen*)pDC->SelectObject(&_red_pen);
				break;
			case Suit_SPADES:
				pDC->SetTextColor(COLOR_BLACK);
				pTempPen = (CPen*)pDC->SelectObject(&_black_pen);
				break;
			default:
				pTempPen = (CPen*)pDC->GetCurrentPen();
				break;
		}
		oldpen.FromHandle((HPEN)pTempPen);					// Save old pen

		// Draw card and suit
		pDC->SetBkMode(OPAQUE);
		pDC->RoundRect(left, top, right, bottom, 5, 5);
		pDC->SetBkMode(TRANSPARENT);
    // Draw card rank and suit
    pDC->DrawText(CString(card->GetSuitCharacter(true)), 
      -1, &srect, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
    pDC->DrawText(CString(card->GetRankCharacter()), 
      -1, &rrect, DT_CENTER | DT_SINGLELINE | DT_VCENTER );

		// Restore original pen and brush
		pDC->SelectObject(oldpen);
		pDC->SelectObject(oldbrush);
	}

	pDC->SelectObject(oldfont);
	cFont.DeleteObject();
	ReleaseDC(pDC);
}

void COpenHoldemView::DrawNameBox(const int chair) {
	CPen		*pTempPen = NULL, oldpen;
	CBrush		*pTempBrush = NULL, oldbrush;
	RECT		textrect = {0}, drawrect = {0};
	CFont		*oldfont = NULL, cFont;
	int			left = 0, top = 0, right = 0, bottom = 0;
	CDC			*pDC = GetDC();
	static RECT	name_rect_last[10] = {0};

	// Background color
	pDC->SetBkColor(COLOR_GRAY);

	// Figure placement of box
	left = _client_rect.right * pc[p_tablemap->nchairs()][chair][0] - 36;
	top = _client_rect.bottom * pc[p_tablemap->nchairs()][chair][1] + 15;
	right = _client_rect.right * pc[p_tablemap->nchairs()][chair][0] + 35;
	bottom = _client_rect.bottom * pc[p_tablemap->nchairs()][chair][1] + 30;

	// Set font basics
	_logfont.lfHeight = -12;
	_logfont.lfWeight = FW_NORMAL;
	cFont.CreateFontIndirect(&_logfont);
	oldfont = pDC->SelectObject(&cFont);
	pDC->SetTextColor(COLOR_BLACK);

	if (p_table_state->_players[chair].seated() 
		|| p_table_state->_players[chair].active()) 
		/*|| (p_tablemap->r$pXseated_index[chair] == k_undefined && p_tablemap->r$uXseated_index[chair] == k_undefined &&
		|| p_tablemap->r$pXactive_index[chair] == k_undefined && p_tablemap->r$uXactive_index[chair] == k_undefined)*/  
	{

		pTempPen = (CPen*)pDC->SelectObject(&_black_pen);
		oldpen.FromHandle((HPEN)pTempPen);					// Save old pen
		pTempBrush = (CBrush*)pDC->SelectObject(&_white_brush);
		oldbrush.FromHandle((HBRUSH)pTempBrush);			// Save old brush

		// Calc rectangle size for text
		textrect.left = 0;
		textrect.top = 0;
		textrect.right = 0;
		textrect.bottom = 0;
		pDC->DrawText(p_table_state->_players[chair]._name.GetString(), 
      p_table_state->_players[chair]._name.GetLength(), &textrect, DT_CALCRECT);

		// Figure out placement of rectangle
		drawrect.left = left < (left+(right-left)/2)-textrect.right/2-3 ? left : (left+(right-left)/2)-textrect.right/2-3;
		drawrect.top = top;
		drawrect.right = right > (left+(right-left)/2)+textrect.right/2+3 ? right : (left+(right-left)/2)+textrect.right/2+3;
		drawrect.bottom = bottom;

		// Invalidate everything if the name has decreased in width
		if (name_rect_last[chair].right - name_rect_last[chair].left != drawrect.right - drawrect.left) 
		{
			InvalidateRect(NULL, true);
		}

		// Draw it
		pDC->SetBkMode(OPAQUE);
		pDC->Rectangle(drawrect.left, drawrect.top, drawrect.right, drawrect.bottom);
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(p_table_state->_players[chair]._name.GetString(), 
      p_table_state->_players[chair]._name.GetLength(), &drawrect,
			DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		name_rect_last[chair].left = drawrect.left;
		name_rect_last[chair].top = drawrect.top;
		name_rect_last[chair].right = drawrect.right;
		name_rect_last[chair].bottom = drawrect.bottom;
	}
	else 
	{
		pTempPen = (CPen*)pDC->SelectObject(&_white_dot_pen);
		oldpen.FromHandle((HPEN)pTempPen);					// Save old pen
		pTempBrush = (CBrush*)pDC->SelectObject(&_gray_brush);
		oldbrush.FromHandle((HBRUSH)pTempBrush);			// Save old brush

		pDC->SetBkMode(OPAQUE);
		pDC->Rectangle(left, top, right, bottom);
	}

	// Restore original pen and brush
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
	pDC->SelectObject(oldfont);
	cFont.DeleteObject();
	ReleaseDC(pDC);
}

void COpenHoldemView::DrawBalanceBox(const int chair) {
	CPen		*pTempPen = NULL, oldpen;
	CBrush	*pTempBrush = NULL, oldbrush;
	RECT		textrect = {0}, drawrect = {0};
	CFont		*oldfont = NULL, cFont;
	CString	t = "";
	int			left = 0, top = 0, right = 0, bottom = 0;
	CDC			*pDC = GetDC();
	static RECT	balance_rect_last[10] = {0};
  // Background color
	pDC->SetBkColor(COLOR_GRAY);
  // Figure placement of box
	left = _client_rect.right * pc[p_tablemap->nchairs()][chair][0] - 26;
	top = _client_rect.bottom * pc[p_tablemap->nchairs()][chair][1] + 30;
	right = _client_rect.right * pc[p_tablemap->nchairs()][chair][0] + 25;
	bottom = _client_rect.bottom * pc[p_tablemap->nchairs()][chair][1] + 45;
  // Set font basics
	_logfont.lfHeight = -12;
	_logfont.lfWeight = FW_NORMAL;
	cFont.CreateFontIndirect(&_logfont);
	oldfont = pDC->SelectObject(&cFont);
	pDC->SetTextColor(COLOR_BLACK);
  if (p_table_state->_players[chair].seated() 
		  || p_table_state->_players[chair].active()) 	{
    pTempPen = (CPen*)pDC->SelectObject(&_black_pen);
		oldpen.FromHandle((HPEN)pTempPen);					// Save old pen
		pTempBrush = (CBrush*)pDC->SelectObject(&_white_brush);
		oldbrush.FromHandle((HBRUSH)pTempBrush);			// Save old brush

		// Format Text
		if (p_table_state->_players[chair].active()) 	{
			t = Number2CString(p_table_state->_players[chair]._balance);
		}	else {
			t.Format("Out (%s)", Number2CString(p_table_state->_players[chair]._balance));
		}
	}	else {
		pTempPen = (CPen*)pDC->SelectObject(&_white_dot_pen);
		oldpen.FromHandle((HPEN)pTempPen);					// Save old pen
		pTempBrush = (CBrush*)pDC->SelectObject(&_gray_brush);
		oldbrush.FromHandle((HBRUSH)pTempBrush);			// Save old brush
    t = "";
	}
  // Calc rectangle size for text
	textrect.left = 0;
	textrect.top = 0;
	textrect.right = 0;
	textrect.bottom = 0;
	pDC->DrawText(t.GetString(), t.GetLength(), &textrect, DT_CALCRECT);
  // Figure out placement of rectangle
	drawrect.left = left < (left+(right-left)/2)-textrect.right/2-3 ? left : (left+(right-left)/2)-textrect.right/2-3;
	drawrect.top = top;
	drawrect.right = right > (left+(right-left)/2)+textrect.right/2+3 ? right : (left+(right-left)/2)+textrect.right/2+3;
	drawrect.bottom = bottom;
  // Invalidate everything if the balance has decreased in width
	if (balance_rect_last[chair].right - balance_rect_last[chair].left != drawrect.right - drawrect.left) {
		InvalidateRect(NULL, true);
	}
  // Draw it
	pDC->SetBkMode(OPAQUE);
	pDC->Rectangle(drawrect.left, drawrect.top, drawrect.right, drawrect.bottom);
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(t.GetString(), t.GetLength(), &drawrect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	balance_rect_last[chair].left = drawrect.left;
	balance_rect_last[chair].top = drawrect.top;
	balance_rect_last[chair].right = drawrect.right;
	balance_rect_last[chair].bottom = drawrect.bottom;

	// Restore original pen and brush
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
	pDC->SelectObject(oldfont);
	cFont.DeleteObject();
	ReleaseDC(pDC);
}

void COpenHoldemView::DrawPlayerBet(const int chair) {
	CPen		*pTempPen = NULL, oldpen;
	CBrush	*pTempBrush = NULL, oldbrush;
	RECT		textrect = {0}, drawrect = {0};
	CFont		*oldfont = NULL, cFont;
	CString	t = "";
	static  RECT	bet_rect_last[10] = {0};
	int		  xcenter = 0, ycenter = 0, xadj = 0, yadj = 0;
	CDC		 *pDC = GetDC();
  // Draw background colored rectangle over position of previous bet to erase it
	pTempPen = (CPen*)pDC->SelectObject(&_null_pen);
	oldpen.FromHandle((HPEN)pTempPen);					// Save old pen
	pTempBrush = (CBrush*)pDC->SelectObject(&_gray_brush);
	oldbrush.FromHandle((HBRUSH)pTempBrush);			// Save old brush
	pDC->Rectangle(bet_rect_last[chair].left, bet_rect_last[chair].top,
				   bet_rect_last[chair].right, bet_rect_last[chair].bottom);
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
  // Background color
	pDC->SetBkColor(COLOR_GRAY);
  // Figure placement
	xcenter = _client_rect.right * pc[p_tablemap->nchairs()][chair][0];
	ycenter = _client_rect.bottom * pc[p_tablemap->nchairs()][chair][1];
	xadj = pcbet[p_tablemap->nchairs()][chair][0];
	yadj = pcbet[p_tablemap->nchairs()][chair][1];
  // Set font basics
	_logfont.lfHeight = -12;
	_logfont.lfWeight = FW_NORMAL;
	cFont.CreateFontIndirect(&_logfont);
	oldfont = pDC->SelectObject(&cFont);
	pDC->SetTextColor(COLOR_BLACK);
  // Format text
	if (p_table_state->_players[chair]._bet != 0) 
	{
		t = Number2CString(p_table_state->_players[chair]._bet);
	}
	else 	{
		t = "";
	}
  // Calc rectangle size for text
	textrect.left = 0;
	textrect.top = 0;
	textrect.right = 0;
	textrect.bottom = 0;
	pDC->DrawText(t.GetString(), t.GetLength(), &textrect, DT_CALCRECT);
  // Figure out placement of rectangle
	if (xadj<0) {
		drawrect.left = xcenter + xadj - textrect.right;
		drawrect.top = ycenter + yadj - textrect.bottom/2;
		drawrect.right = xcenter + xadj;
		drawrect.bottom = ycenter + yadj + textrect.bottom/2;
	}	else if (xadj>0) {
		drawrect.left = xcenter + xadj;
		drawrect.top = ycenter + yadj - textrect.bottom/2;
		drawrect.right = xcenter + xadj + textrect.right;
		drawrect.bottom = ycenter + yadj + textrect.bottom/2;
	}
	else	// xadj==0
	{  
		drawrect.left = xcenter + xadj - textrect.right/2;
		drawrect.top = ycenter + yadj - textrect.bottom/2;
		drawrect.right = xcenter + xadj + textrect.right/2;
		drawrect.bottom = ycenter + yadj + textrect.bottom/2;
	}
  // Draw it
	pDC->SetBkMode(OPAQUE);
	pDC->DrawText(t.GetString(), t.GetLength(), &drawrect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	bet_rect_last[chair].left = drawrect.left;
	bet_rect_last[chair].top = drawrect.top;
	bet_rect_last[chair].right = drawrect.right;
	bet_rect_last[chair].bottom = drawrect.bottom;

	cFont.DeleteObject();
	ReleaseDC(pDC);
}

void COpenHoldemView::DrawPlayerCards(const int chair) {
	if (!p_table_state->_players[chair].active())	{
		// Forget about inactive players, they have no cards.
		// Don't draw them to point out the mistake faster
		// for newbies with bad tablemaps.
		return;
	}
	// Get size of current client window
	GetClientRect(&_client_rect);
	// Draw player cards (first)
  Card *player_card_0 = &p_table_state->_players[chair]._hole_cards[0];
	write_log(MAIN->p_preferences()->debug_gui(), "[GUI] COpenHoldemView::UpdateDisplay() Drawing card 0 of player %i: [%s]\n",
    chair, player_card_0->ToString());
  int pos_x_right  = _client_rect.right * pc[p_tablemap->nchairs()][chair][0] + 5;
  int pos_x_left   = pos_x_right - CARDSIZEX;
  int pos_y_top    = _client_rect.bottom * pc[p_tablemap->nchairs()][chair][1] - CARDSIZEY/2;
  int pos_y_bottom = pos_y_top + CARDSIZEY - 1;
	DrawCard(player_card_0, pos_x_left, pos_y_top, pos_x_right, pos_y_bottom,	true);
  // Draw player cards (second)
  Card *player_card_1 = &p_table_state->_players[chair]._hole_cards[1];
  pos_x_right = pos_x_right + CARDSIZEX - 9; 
  pos_x_left  = pos_x_right - CARDSIZEX;
	DrawCard(player_card_1, pos_x_left, pos_y_top, pos_x_right, pos_y_bottom,	true);
}

void COpenHoldemView::DrawColourCodes(const int chair) {
  if (SYM->p_symbol_engine_colourcodes() == NULL) {
    // Not yet initialized
    return;
  }
  // Figure placement of box
  int right = 0;
  int bottom = _client_rect.bottom * pc[p_tablemap->nchairs()][chair][1] + 16;
  int top    = bottom - 10;
  if (chair >= (p_tablemap->nchairs() / 2)) {
    right = _client_rect.right * pc[p_tablemap->nchairs()][chair][0] - 20;
  } else {
    right = _client_rect.right * pc[p_tablemap->nchairs()][chair][0] + 31;
  }
  int left = right - 10;
  // Draw it
  CDC	*pDC = GetDC();
	CPen		*pTempPen = NULL, oldpen;
	CBrush	*pTempBrush = NULL, oldbrush;
  pTempPen = (CPen*)pDC->SelectObject(&_black_pen);
	oldpen.FromHandle((HPEN)pTempPen);					// Save old pen
  COLORREF colour_code = SYM->p_symbol_engine_colourcodes()->ColourCodeToDisplay(chair);
  CBrush mybrush(colour_code);
	pTempBrush = (CBrush*)pDC->SelectObject(mybrush); 
	oldbrush.FromHandle((HBRUSH)pTempBrush);			// Save old brush
	pDC->Rectangle(left, top, right, bottom);
	// Restore original pen and brush
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
	ReleaseDC(pDC);
}