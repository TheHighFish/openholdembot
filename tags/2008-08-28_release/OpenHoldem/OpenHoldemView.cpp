// OpenHoldemView.cpp : implementation of the COpenHoldemView class
//

#include "stdafx.h"
#include "OpenHoldemDoc.h"
#include "OpenHoldemView.h"
#include "OpenHoldem.h"

#include "CScraper.h"
#include "CSymbols.h"
#include "CHeartbeatThread.h"
#include "CGlobal.h"


// Table layouts
int		cc[5][2] = 
{ 
	{-(CARDSIZEX*2 + 3*2 + CARDSIZEX/2), -(CARDSIZEY/2)},	// absolutes
    {-(CARDSIZEX*1 + 3*1 + CARDSIZEX/2), -(CARDSIZEY/2)},
    {-(CARDSIZEX*0 + 3*0 + CARDSIZEX/2), -(CARDSIZEY/2)},
    {+(CARDSIZEX*0 + 3*1 + CARDSIZEX/2), -(CARDSIZEY/2)},
    {+(CARDSIZEX*1 + 3*2 + CARDSIZEX/2), -(CARDSIZEY/2)}
};

// Player locations as a percentage of width/height
// [nplayers][chairnum][x/y]
double	pc[11][10][2] = 
{
    { {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },	// 0 players
    { {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },	// 1 player
    { {.95,.47}, {.05,.47}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },	// 2 players
    { {.95,.47}, {.50,.83}, {.05,.47}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },	// 3 players
    { {.89,.25}, {.89,.69}, {.11,.69}, {.11,.25}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },	// 4 players
    { {.89,.25}, {.89,.69}, {.50,.83}, {.11,.69}, {.11,.25}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0} },	// 5 players
    { {.72,.11}, {.95,.47}, {.72,.83}, {.28,.83}, {.05,.47}, {.28,.11}, {0,0}, {0,0}, {0,0}, {0,0} },	// 6 players
    { {.72,.11}, {.95,.47}, {.72,.83}, {.50,.83}, {.28,.83}, {.05,.47}, {.28,.11}, {0,0}, {0,0}, {0,0} },	// 7 players
    { {.72,.11}, {.89,.25}, {.89,.69}, {.72,.83}, {.28,.83}, {.11,.69}, {.11,.25}, {.28,.11}, {0,0}, {0,0} },	// 8 players
    { {.72,.11}, {.89,.25}, {.89,.69}, {.72,.83}, {.50,.83}, {.28,.83}, {.11,.69}, {.11,.25}, {.28,.11}, {0,0} },	// 9 players
    { {.72,.11}, {.85,.21}, {.95,.47}, {.85,.73}, {.72,.83}, {.28,.83}, {.15,.73}, {.05,.47}, {.15,.21}, {.28,.11} }	// 10 players
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
COpenHoldemView::COpenHoldemView() 
{
    __SEH_SET_EXCEPTION_HANDLER

    __SEH_HEADER

	black_pen.CreatePen(PS_SOLID, 1, COLOR_BLACK);
    green_pen.CreatePen(PS_SOLID, 1, COLOR_GREEN);
    red_pen.CreatePen(PS_SOLID, 1, COLOR_RED);
    blue_pen.CreatePen(PS_SOLID, 1, COLOR_BLUE);
    white_dot_pen.CreatePen(PS_DOT, 1, COLOR_WHITE);
    null_pen.CreatePen(PS_NULL, 0, COLOR_BLACK);

    white_brush.CreateSolidBrush(COLOR_WHITE);
    gray_brush.CreateSolidBrush(COLOR_GRAY);
    red_brush.CreateSolidBrush(COLOR_RED);
    yellow_brush.CreateSolidBrush(COLOR_YELLOW);

    lf.lfWidth = 0;
    lf.lfEscapement = 0;
    lf.lfOrientation = 0;
    lf.lfItalic = 0;
    lf.lfUnderline = 0;
    lf.lfStrikeOut = 0;
    lf.lfCharSet = 0;
    lf.lfOutPrecision = 0;
    lf.lfClipPrecision = 0;
    lf.lfQuality = PROOF_QUALITY;
    lf.lfPitchAndFamily = 0;
    strcpy(lf.lfFaceName, "Times New Roman");


    __SEH_LOGFATAL("COpenHoldemView::constructor\n");
}

COpenHoldemView::~COpenHoldemView() 
{
    __SEH_HEADER

    __SEH_LOGFATAL("COpenHoldemView::Destructor\n");
}

BOOL COpenHoldemView::PreCreateWindow(CREATESTRUCT& cs) 
{
    __SEH_HEADER

    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs
    return CView::PreCreateWindow(cs);

    __SEH_LOGFATAL("COpenHoldemView::PreCreateWindow\n");
}

void COpenHoldemView::OnInitialUpdate() 
{
    __SEH_HEADER

    CView::OnInitialUpdate();

    // Timer to check for display updates
    SetTimer(DISPLAY_UPDATE_TIMER, 103, 0);

    __SEH_LOGFATAL("COpenHoldemView::OnInitialUpdate \n");
}

// COpenHoldemView drawing
void COpenHoldemView::OnDraw(CDC* pDC) 
{
    __SEH_HEADER

    update_display(true);

    __SEH_LOGFATAL("COpenHoldemView::OnDraw \n");
}

void COpenHoldemView::OnTimer(UINT nIDEvent) 
{
    __SEH_HEADER

    if (nIDEvent == DISPLAY_UPDATE_TIMER) 
	{
		// Only do this if we are not in the middle of a scraper/symbol update
		if (TryEnterCriticalSection(&p_heartbeat_thread->cs_update_in_progress))
		{
			update_display(false);
			LeaveCriticalSection(&p_heartbeat_thread->cs_update_in_progress);
		}
	}

    CView::OnTimer(nIDEvent);

    __SEH_LOGFATAL("COpenHoldemView::OnTimer :\n");
}

void COpenHoldemView::update_display(bool update_all) 
{
    __SEH_HEADER

    int						i;

    static double			handnumber_last, sblind_last, bblind_last, lim_last;
    static double			ante_last, pot_last;
    static unsigned int		iterator_thread_progress_last;
    static unsigned int		card_common_last[5];
    static CString			seated_last[10], active_last[10];
    static unsigned int		card_player_last[10][2];
    static bool				dealer_last[10], istournament_last;
    static CString			playername_last[10];
    static double			playerbalance_last[10];
    static double			playerbet_last[10];

    bool					update_it;
    RECT					cr;
    CDC						*pDC = GetDC();

	double sym_handnumber		= p_symbols->sym()->handnumber;
	double sym_bblind		= p_symbols->sym()->bblind;
	double sym_sblind		= p_symbols->sym()->sblind;
	int sym_lim				= (int) p_symbols->sym()->lim;
	bool sym_istournament	= (bool) p_symbols->sym()->istournament;
	double sym_ante			= p_symbols->sym()->ante;
	double sym_pot			= p_symbols->sym()->pot;

    // Get size of current client window
    GetClientRect(&cr);

    // Set background color (light gray)
    if (update_all) 
	{
        CBrush backBrush(COLOR_GRAY);
        CBrush* pOldBrush = pDC->SelectObject(&backBrush);
        pDC->PatBlt(cr.left, cr.top, cr.right-cr.left, cr.bottom-cr.top, PATCOPY);
        pDC->SelectObject(pOldBrush);
    }

	// Draw center info box
	update_it = false;
	if (handnumber_last != sym_handnumber) 
	{
		handnumber_last = sym_handnumber;
		update_it = true;
	}
	if (sblind_last != sym_sblind) 
	{
		sblind_last = sym_sblind;
		update_it = true;
	}
	if (bblind_last != sym_bblind) 
	{
		bblind_last = sym_bblind;
		update_it = true;
	}
	if (lim_last != sym_lim) 
	{
		lim_last = sym_lim;
		update_it = true;
	}
	if (istournament_last != sym_istournament) 
	{
		istournament_last = sym_istournament;
		update_it = true;
	}
	if (ante_last != sym_ante != 0) 
	{
		ante_last = sym_ante;
		update_it = true;
	}
	if (pot_last != sym_pot) 
	{
		pot_last = sym_pot;
		update_it = true;
	}

	if (update_it || update_all) 
	{
		draw_center_info_box();
	}

	// Draw button state indicators
	draw_button_indicators();

	// Draw common cards
	for (i=0; i<5; i++) 
	{
		if (card_common_last[i] != p_scraper->card_common(i) || update_all) 
		{
			card_common_last[i] = p_scraper->card_common(i);

			draw_card(p_scraper->card_common(i),
					  cr.right/2 + cc[i][0], cr.bottom/2 + cc[i][1],
					  cr.right/2 + cc[i][0] + CARDSIZEX, cr.bottom/2 + cc[i][1] + CARDSIZEY,
					  false);
		}
	}

	// Draw collection of player info
	for (i=0; i<p_global->trans.map.num_chairs; i++) 
	{

		// Figure out if we need to redraw this seat
		update_it = false;
		if (seated_last[i] != p_scraper->seated(i) ||
			active_last[i] != p_scraper->active(i)) 
		{
			seated_last[i] = p_scraper->seated(i);
			active_last[i] = p_scraper->active(i);
			update_it = true;
		}
		if (card_player_last[i][0] != p_scraper->card_player(i, 0) ||
			card_player_last[i][1] != p_scraper->card_player(i, 1)) 
		{
			card_player_last[i][0] = p_scraper->card_player(i, 0);
			card_player_last[i][1] = p_scraper->card_player(i, 1);
			update_it = true;
		}
		if (dealer_last[i] != p_scraper->dealer(i)) 
		{
			dealer_last[i] = p_scraper->dealer(i);
			update_it = true;
		}
		if (playername_last[i] != p_scraper->player_name(i)) 
		{
			playername_last[i] = p_scraper->player_name(i);
			update_it = true;
		}
		if (playerbalance_last[i] != p_scraper->player_balance(i)) 
		{
			playerbalance_last[i] = p_scraper->player_balance(i);
			update_it = true;
		}
		if (playerbet_last[i] != p_scraper->player_bet(i)) 
		{
			playerbet_last[i] = p_scraper->player_bet(i);
			update_it = true;
		}

		if (update_it || update_all) 
		{
			// Draw active circle
			if (p_scraper->IsStringSeated(p_scraper->seated(i))) 
				draw_seated_active_circle(i);

			// Draw player cards
			draw_card(p_scraper->card_player(i, 0),
					  cr.right * pc[p_global->trans.map.num_chairs][i][0] - CARDSIZEX - 2,
					  cr.bottom * pc[p_global->trans.map.num_chairs][i][1] - CARDSIZEY/2,
					  cr.right * pc[p_global->trans.map.num_chairs][i][0] - 2,
					  cr.bottom * pc[p_global->trans.map.num_chairs][i][1] + CARDSIZEY/2 - 1,
					  true);
			draw_card(p_scraper->card_player(i, 1),
					  cr.right * pc[p_global->trans.map.num_chairs][i][0] + 1,
					  cr.bottom * pc[p_global->trans.map.num_chairs][i][1] - CARDSIZEY/2,
					  cr.right * pc[p_global->trans.map.num_chairs][i][0] + CARDSIZEX + 1,
					  cr.bottom * pc[p_global->trans.map.num_chairs][i][1] + CARDSIZEY/2 - 1,
					  true);

			// Draw dealer button
			if (p_scraper->dealer(i))
				draw_dealer_button(i);

			// Draw name and balance boxes
			draw_name_box(i);
			draw_balance_box(i);

			// Draw player bet
			draw_player_bet(i);
		}
	}

	ReleaseDC(pDC);

    __SEH_LOGFATAL("COpenHoldemView::update_display :\n");
}

void COpenHoldemView::draw_center_info_box(void) 
{
    __SEH_HEADER

    CPen		*pTempPen, oldpen;
    CBrush		*pTempBrush, oldbrush;
    RECT		rect;
    CString		t, s;
    CFont		*oldfont, cFont;
    RECT		cr;
    int			left, top, right, bottom;
    CDC			*pDC = GetDC();
	int			height = 80;
	
	double sym_handnumber	= p_symbols->sym()->handnumber;
	double sym_bblind		= p_symbols->sym()->bblind;
	double sym_sblind		= p_symbols->sym()->sblind;
	int sym_lim				= (int) p_symbols->sym()->lim;
	bool sym_istournament	= (bool) p_symbols->sym()->istournament;
	double sym_ante			= p_symbols->sym()->ante;
	double sym_pot			= p_symbols->sym()->pot;
	bool sym_playing		= (bool) p_symbols->sym()->playing;

	if (p_global->preferences.LogSymbol_enabled)
		height += 40;

    // Get size of current client window
    GetClientRect(&cr);

    // Figure placement of box
    left = cr.right/2-70;
    top = 4;
    right = cr.right/2+70;
    bottom = top+height;

    pTempPen = (CPen*)pDC->SelectObject(&black_pen);
    oldpen.FromHandle((HPEN)pTempPen);					// Save old pen
    pTempBrush = (CBrush*)pDC->SelectObject(&white_brush);
    oldbrush.FromHandle((HBRUSH)pTempBrush);			// Save old brush

    pDC->SetBkMode(OPAQUE);
    pDC->Rectangle(left, top, right, bottom);

    // Set font basics
    lf.lfHeight = -12;
    lf.lfWeight = FW_NORMAL;
    cFont.CreateFontIndirect(&lf);
    oldfont = pDC->SelectObject(&cFont);
    pDC->SetTextColor(COLOR_BLACK);

    // Set rectangle
    rect.left = left;
    rect.top = top;
    rect.right = right;
    rect.bottom = bottom;

	t = "";
	// handnumber
	if (sym_handnumber != 0) 
	{
		s.Format("  Hand #: %0f\n", sym_handnumber);
	}
	else 
	{
		s.Format("  Hand #: -\n");
	}
	t.Append(s);

	// blinds/type
	if ((int) sym_sblind != sym_sblind || (int) sym_bblind != sym_bblind) 
	{
		s.Format("  %s%s %.2f/%.2f/%.2f\n",
				 (sym_lim == LIMIT_NL ? "NL" : sym_lim == LIMIT_PL ? "PL" :
				  sym_lim == LIMIT_FL ? "FL" : "?L"),
				 (sym_istournament ? "T" : ""),
				 sym_sblind, sym_bblind, p_symbols->bigbet());
	}
	else 
	{
		s.Format("  %s%s %.0f/%.0f/%.0f\n",
				 (sym_lim == LIMIT_NL ? "NL" : sym_lim == LIMIT_PL ? "PL" :
				  sym_lim == LIMIT_FL ? "FL" : "?L"),
				 (sym_istournament ? "T" : ""),
				 sym_sblind, sym_bblind, p_symbols->bigbet());
	}
	t.Append(s);

	// ante
	if (sym_ante != 0) 
	{
		if ((int) sym_ante != sym_ante) 
		{
			s.Format("  Ante: %.2f\n", sym_ante);
		}
		else 
		{
			s.Format("  Ante: %.0f\n", sym_ante);
		}
		t.Append(s);
	}

	// Pot
	if ((int) sym_pot != sym_pot) 
		s.Format("  Pot: %.2f\n", sym_pot);

	else 
		s.Format("  Pot: %.0f\n", sym_pot);

	t.Append(s);

	if (p_symbols->user_chair_confirmed() && sym_playing) 
	{
		for (int i=0; i<min(5, p_symbols->logsymbols_collection()->GetCount()); i++)
		{
			s.Format("  Log: %s\n", p_symbols->logsymbols_collection()[i]);
			t.Append(s);
		}
	}

	// Draw it
    pDC->SetBkMode(TRANSPARENT);
    pDC->DrawText(t.GetString(), t.GetLength(), &rect, NULL);

    // Restore original pen, brush and font
    pDC->SelectObject(oldpen);
    pDC->SelectObject(oldbrush);
    pDC->SelectObject(oldfont);
    cFont.DeleteObject();
    ReleaseDC(pDC);

    __SEH_LOGFATAL("COpenHoldemView::draw_center_info_box\n");
}

void COpenHoldemView::draw_button_indicators(void) 
{
    __SEH_HEADER

    int			i;
    bool		fold_drawn, call_drawn, check_drawn, raise_drawn, allin_drawn;
    RECT		cr;


	// Get size of current client window
    GetClientRect(&cr);

    fold_drawn = call_drawn = check_drawn = raise_drawn = allin_drawn = false;

    for (i=0; i<=9; i++) 
	{
		// Draw "on" buttons
		if (p_scraper->GetButtonState(i)) 
		{
            if (p_scraper->IsStringFold(p_scraper->button_label(i))) 
			{
                draw_specific_button_indicator(i, 'F', cr.right-84, cr.bottom-16, cr.right-70, cr.bottom-2);
                fold_drawn = true;
            }
            else if (p_scraper->IsStringCall(p_scraper->button_label(i))) 
			{
                draw_specific_button_indicator(i, 'C', cr.right-67, cr.bottom-16, cr.right-53, cr.bottom-2);
                call_drawn = true;
            }
            else if (p_scraper->IsStringCheck(p_scraper->button_label(i))) 
			{
                draw_specific_button_indicator(i, 'K', cr.right-50, cr.bottom-16, cr.right-36, cr.bottom-2);
                check_drawn = true;
            }
            else if (p_scraper->IsStringRaise(p_scraper->button_label(i))) 
			{
                draw_specific_button_indicator(i, 'R', cr.right-33, cr.bottom-16, cr.right-19, cr.bottom-2);
                raise_drawn = true;
            }
            else if (p_scraper->IsStringAllin(p_scraper->button_label(i))) 
			{
                draw_specific_button_indicator(i, 'A', cr.right-16, cr.bottom-16, cr.right-2, cr.bottom-2);
                allin_drawn = true;
            }
        }
    }

	// Draw "off" buttons
    if (!fold_drawn) 
        draw_specific_button_indicator(-1, 'F', cr.right-84, cr.bottom-16, cr.right-70, cr.bottom-2);

	if (!call_drawn) 
        draw_specific_button_indicator(-1, 'C', cr.right-67, cr.bottom-16, cr.right-53, cr.bottom-2);

	if (!check_drawn)
        draw_specific_button_indicator(-1, 'K', cr.right-50, cr.bottom-16, cr.right-36, cr.bottom-2);

	if (!raise_drawn)
        draw_specific_button_indicator(-1, 'R', cr.right-33, cr.bottom-16, cr.right-19, cr.bottom-2);

	if (!allin_drawn)
        draw_specific_button_indicator(-1, 'A', cr.right-16, cr.bottom-16, cr.right-2, cr.bottom-2);

    __SEH_LOGFATAL("COpenHoldemView::draw_specific_button_indicators\n");
}

void COpenHoldemView::draw_specific_button_indicator(int button_num, char ch, int left, int top, int right, int bottom) 
{
    __SEH_HEADER

    CPen		*pTempPen, oldpen;
    CBrush		*pTempBrush, oldbrush;
    RECT		rect;
    CFont		*oldfont, cFont;
    CString		t;
    CDC			*pDC = GetDC();

	// Set font basics
    lf.lfHeight = -12;
    lf.lfWeight = FW_NORMAL;
    cFont.CreateFontIndirect(&lf);
    oldfont = pDC->SelectObject(&cFont);

    // Background color
    pDC->SetBkColor(COLOR_GRAY);

    if (button_num == -1) 
	{
        pTempPen = (CPen*)pDC->SelectObject(&white_dot_pen);
        pTempBrush = (CBrush*)pDC->SelectObject(&gray_brush);
        pDC->SetTextColor(COLOR_WHITE);
    }
    else 
	{
        if (p_scraper->GetButtonState(button_num)) 
		{
            if (ch=='F') 
			{
                pTempPen = (CPen*)pDC->SelectObject(&red_pen);
                pTempBrush = (CBrush*)pDC->SelectObject(&white_brush);
                pDC->SetTextColor(COLOR_RED);
            }
            else if (ch=='C') 
			{
                pTempPen = (CPen*)pDC->SelectObject(&blue_pen);
                pTempBrush = (CBrush*)pDC->SelectObject(&white_brush);
                pDC->SetTextColor(COLOR_BLUE);
            }
            else if (ch=='K') 
			{
                pTempPen = (CPen*)pDC->SelectObject(&blue_pen);
                pTempBrush = (CBrush*)pDC->SelectObject(&white_brush);
                pDC->SetTextColor(COLOR_BLUE);
            }
            else if (ch=='R') 
			{
                pTempPen = (CPen*)pDC->SelectObject(&green_pen);
                pTempBrush = (CBrush*)pDC->SelectObject(&white_brush);
                pDC->SetTextColor(COLOR_GREEN);
            }
            else if (ch=='A') 
			{
                pTempPen = (CPen*)pDC->SelectObject(&black_pen);
                pTempBrush = (CBrush*)pDC->SelectObject(&white_brush);
                pDC->SetTextColor(COLOR_BLACK);
            }
        }
        else 
		{
            pTempPen = (CPen*)pDC->SelectObject(&white_dot_pen);
            pTempBrush = (CBrush*)pDC->SelectObject(&gray_brush);
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

    __SEH_LOGFATAL("COpenHoldemView::draw_specific_button_indicator\n");

}

void COpenHoldemView::draw_seated_active_circle(int chair) 
{
    __SEH_HEADER

    CPen		*pTempPen, oldpen;
    CBrush		*pTempBrush, oldbrush;
    RECT		cr;
    int			left, top, right, bottom;
    CDC			*pDC = GetDC();

    // Get size of current client window
    GetClientRect(&cr);

    // Background color
    pDC->SetBkColor(COLOR_GRAY);

    // Figure placement of circle
    left = cr.right * pc[p_global->trans.map.num_chairs][chair][0] - CARDSIZEX - 4;
    top = cr.bottom * pc[p_global->trans.map.num_chairs][chair][1] - CARDSIZEX - 3;
    right = cr.right * pc[p_global->trans.map.num_chairs][chair][0] + CARDSIZEX + 3;
    bottom = cr.bottom * pc[p_global->trans.map.num_chairs][chair][1] + CARDSIZEX + 3;

    pTempPen = (CPen*)pDC->SelectObject(&black_pen);
    oldpen.FromHandle((HPEN)pTempPen);					// Save old pen

    if (p_scraper->IsStringActive(p_scraper->active(chair))) 
	{
        pTempBrush = (CBrush*)pDC->SelectObject(&white_brush);
    }
    else 
	{
        pTempBrush = (CBrush*)pDC->SelectObject(&gray_brush);
    }
    oldbrush.FromHandle((HBRUSH)pTempBrush);			// Save old brush

    pDC->Ellipse(left, top, right, bottom);

    // Restore original pen and brush
    pDC->SelectObject(oldpen);
    pDC->SelectObject(oldbrush);
    ReleaseDC(pDC);

    __SEH_LOGFATAL("COpenHoldemView::draw_seated_active_circle\n");

}

void COpenHoldemView::draw_dealer_button(int chair) 
{
    __SEH_HEADER

    CPen		*pTempPen, oldpen;
    CBrush		*pTempBrush, oldbrush;
    RECT		cr;
    int			left, top, right, bottom;
    CDC			*pDC = GetDC();

    // Get size of current client window
    GetClientRect(&cr);

    // Background color
    pDC->SetBkColor(COLOR_GRAY);

    // Figure placement of circle
    left = cr.right * pc[p_global->trans.map.num_chairs][chair][0] - 8;
    top = cr.bottom * pc[p_global->trans.map.num_chairs][chair][1] - 8;
    right = cr.right * pc[p_global->trans.map.num_chairs][chair][0] + 8;
    bottom = cr.bottom * pc[p_global->trans.map.num_chairs][chair][1] + 8;


    pTempPen = (CPen*)pDC->SelectObject(&black_pen);
    oldpen.FromHandle((HPEN)pTempPen);					// Save old pen
    pTempBrush = (CBrush*)pDC->SelectObject(&red_brush);
    oldbrush.FromHandle((HBRUSH)pTempBrush);			// Save old brush

    pDC->Ellipse(left, top, right, bottom);

    // Restore original pen and brush
    pDC->SelectObject(oldpen);
    pDC->SelectObject(oldbrush);
    ReleaseDC(pDC);

    __SEH_LOGFATAL("COpenHoldemView::draw_dealer_button\n");

}

void COpenHoldemView::draw_card(unsigned int card, int left, int top, int right, int bottom, bool pl_card) 
{
    __SEH_HEADER

    CPen		*pTempPen, oldpen;
    CBrush		*pTempBrush, oldbrush;
    RECT		rrect, srect;
    CFont		*oldfont, cFont;
    CDC			*pDC = GetDC();

    // Set font basics
    lf.lfHeight = -24;
    lf.lfWeight = FW_BOLD;
    cFont.CreateFontIndirect(&lf);
    oldfont = pDC->SelectObject(&cFont);

    // Background color
    pDC->SetBkColor(COLOR_GRAY);

    // CARD BACK
    if (card == CARD_BACK) 
	{
        pTempPen = (CPen*)pDC->SelectObject(&black_pen);
        oldpen.FromHandle((HPEN)pTempPen);					// Save old pen
        pTempBrush = (CBrush*)pDC->SelectObject(&yellow_brush);
        oldbrush.FromHandle((HBRUSH)pTempBrush);			// Save old brush

        pDC->SetBkMode(OPAQUE);
        pDC->RoundRect(left, top, right, bottom, 5, 5);

        // Restore original pen and brush
        pDC->SelectObject(oldpen);
        pDC->SelectObject(oldbrush);
    }

    // NO CARD
    else if (card == CARD_NOCARD) 
	{
        pTempPen = (CPen*)pDC->SelectObject(&white_dot_pen);
        oldpen.FromHandle((HPEN)pTempPen);					// Save old pen
        pTempBrush = (CBrush*)pDC->SelectObject(&gray_brush);
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
        pTempBrush = (CBrush*)pDC->SelectObject(&white_brush);
        oldbrush.FromHandle((HBRUSH)pTempBrush);			// Save old brush

        // Set colors
        switch (StdDeck_SUIT(card)) 
		{
			case Suit_CLUBS:
				pDC->SetTextColor(COLOR_GREEN);
				pTempPen = (CPen*)pDC->SelectObject(&green_pen);
				break;
			case Suit_DIAMONDS:
				pDC->SetTextColor(COLOR_BLUE);
				pTempPen = (CPen*)pDC->SelectObject(&blue_pen);
				break;
			case Suit_HEARTS:
				pDC->SetTextColor(COLOR_RED);
				pTempPen = (CPen*)pDC->SelectObject(&red_pen);
				break;
			case Suit_SPADES:
				pDC->SetTextColor(COLOR_BLACK);
				pTempPen = (CPen*)pDC->SelectObject(&black_pen);
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
        switch (StdDeck_SUIT(card)) 
		{
			case Suit_CLUBS:
				pDC->DrawText("C", -1, &srect, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
				break;
			case Suit_DIAMONDS:
				pDC->DrawText("D", -1, &srect, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
				break;
			case Suit_HEARTS:
				pDC->DrawText("H", -1, &srect, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
				break;
			case Suit_SPADES:
				pDC->DrawText("S", -1, &srect, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
				break;
        }


        // Draw card rank
        switch (StdDeck_RANK(card)) 
		{
			case Rank_2:
				pDC->DrawText("2", -1, &rrect, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
				break;
			case Rank_3:
				pDC->DrawText("3", -1, &rrect, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
				break;
			case Rank_4:
				pDC->DrawText("4", -1, &rrect, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
				break;
			case Rank_5:
				pDC->DrawText("5", -1, &rrect, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
				break;
			case Rank_6:
				pDC->DrawText("6", -1, &rrect, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
				break;
			case Rank_7:
				pDC->DrawText("7", -1, &rrect, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
				break;
			case Rank_8:
				pDC->DrawText("8", -1, &rrect, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
				break;
			case Rank_9:
				pDC->DrawText("9", -1, &rrect, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
				break;
			case Rank_TEN:
				pDC->DrawText("T", -1, &rrect, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
				break;
			case Rank_JACK:
				pDC->DrawText("J", -1, &rrect, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
				break;
			case Rank_QUEEN:
				pDC->DrawText("Q", -1, &rrect, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
				break;
			case Rank_KING:
				pDC->DrawText("K", -1, &rrect, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
				break;
			case Rank_ACE:
				pDC->DrawText("A", -1, &rrect, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
				break;
        }

        // Restore original pen and brush
        pDC->SelectObject(oldpen);
        pDC->SelectObject(oldbrush);
    }

    pDC->SelectObject(oldfont);
    cFont.DeleteObject();
    ReleaseDC(pDC);

    __SEH_LOGFATAL("COpenHoldemView::draw_card\n");

}


void COpenHoldemView::draw_name_box(int chair) 
{
    __SEH_HEADER

    CPen		*pTempPen, oldpen;
    CBrush		*pTempBrush, oldbrush;
    RECT		textrect, drawrect;
    CFont		*oldfont, cFont;
    RECT		cr;
    int			left, top, right, bottom;
    CDC			*pDC = GetDC();
    static RECT	name_rect_last[10];

	// Get size of current client window
    GetClientRect(&cr);

    // Background color
    pDC->SetBkColor(COLOR_GRAY);

    // Figure placement of box
    left = cr.right * pc[p_global->trans.map.num_chairs][chair][0] - 36;
    top = cr.bottom * pc[p_global->trans.map.num_chairs][chair][1] + 15;
    right = cr.right * pc[p_global->trans.map.num_chairs][chair][0] + 35;
    bottom = cr.bottom * pc[p_global->trans.map.num_chairs][chair][1] + 30;

    // Set font basics
    lf.lfHeight = -12;
    lf.lfWeight = FW_NORMAL;
    cFont.CreateFontIndirect(&lf);
    oldfont = pDC->SelectObject(&cFont);
    pDC->SetTextColor(COLOR_BLACK);

    if (p_scraper->IsStringSeated(p_scraper->seated(chair)) || 
		p_scraper->IsStringActive(p_scraper->active(chair)) /*||
        (p_global->trans.map.r$pXseated_index[chair] == -1 && p_global->trans.map.r$uXseated_index[chair] == -1 &&
         p_global->trans.map.r$pXactive_index[chair] == -1 && p_global->trans.map.r$uXactive_index[chair] == -1)*/ ) 
	{

        pTempPen = (CPen*)pDC->SelectObject(&black_pen);
        oldpen.FromHandle((HPEN)pTempPen);					// Save old pen
        pTempBrush = (CBrush*)pDC->SelectObject(&white_brush);
        oldbrush.FromHandle((HBRUSH)pTempBrush);			// Save old brush

        // Calc rectangle size for text
        textrect.left = 0;
        textrect.top = 0;
        textrect.right = 0;
        textrect.bottom = 0;
        pDC->DrawText(p_scraper->player_name(chair).GetString(), p_scraper->player_name(chair).GetLength(), &textrect, DT_CALCRECT);

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
        pDC->DrawText(p_scraper->player_name(chair).GetString(), p_scraper->player_name(chair).GetLength(), &drawrect,
                      DT_CENTER | DT_SINGLELINE | DT_VCENTER);
        name_rect_last[chair].left = drawrect.left;
        name_rect_last[chair].top = drawrect.top;
        name_rect_last[chair].right = drawrect.right;
        name_rect_last[chair].bottom = drawrect.bottom;
    }
    else 
	{
        pTempPen = (CPen*)pDC->SelectObject(&white_dot_pen);
        oldpen.FromHandle((HPEN)pTempPen);					// Save old pen
        pTempBrush = (CBrush*)pDC->SelectObject(&gray_brush);
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

    __SEH_LOGFATAL("COpenHoldemView::draw_name_box\n");

}

void COpenHoldemView::draw_balance_box(int chair) 
{
    __SEH_HEADER

    CPen		*pTempPen, oldpen;
    CBrush		*pTempBrush, oldbrush;
    RECT		textrect, drawrect;
    CFont		*oldfont, cFont;
    CString		t;
    RECT		cr;
    int			left, top, right, bottom;
    CDC			*pDC = GetDC();
    static RECT	balance_rect_last[10];

	// Get size of current client window
    GetClientRect(&cr);

    // Background color
    pDC->SetBkColor(COLOR_GRAY);

    // Figure placement of box
    left = cr.right * pc[p_global->trans.map.num_chairs][chair][0] - 26;
    top = cr.bottom * pc[p_global->trans.map.num_chairs][chair][1] + 30;
    right = cr.right * pc[p_global->trans.map.num_chairs][chair][0] + 25;
    bottom = cr.bottom * pc[p_global->trans.map.num_chairs][chair][1] + 45;

    // Set font basics
    lf.lfHeight = -12;
    lf.lfWeight = FW_NORMAL;
    cFont.CreateFontIndirect(&lf);
    oldfont = pDC->SelectObject(&cFont);
    pDC->SetTextColor(COLOR_BLACK);

    if (p_scraper->IsStringSeated(p_scraper->seated(chair)) || 
		p_scraper->IsStringActive(p_scraper->active(chair)) /*||
        (p_global->trans.map.r$pXseated_index[chair] == -1 && p_global->trans.map.r$uXseated_index[chair] == -1 &&
         p_global->trans.map.r$pXactive_index[chair] == -1 && p_global->trans.map.r$uXactive_index[chair] == -1)*/ ) 
	{

        pTempPen = (CPen*)pDC->SelectObject(&black_pen);
        oldpen.FromHandle((HPEN)pTempPen);					// Save old pen
        pTempBrush = (CBrush*)pDC->SelectObject(&white_brush);
        oldbrush.FromHandle((HBRUSH)pTempBrush);			// Save old brush

        // Format Text
        if (!p_scraper->sitting_out(chair)) 
		{
            if ((int) p_scraper->player_balance(chair) != p_scraper->player_balance(chair)) 
			{
                t.Format("%.2f", p_scraper->player_balance(chair));
            }
            else 
			{
                t.Format("%d", (int) p_scraper->player_balance(chair));
            }
        }
        else 
		{
            if ((int) p_scraper->player_balance(chair) != p_scraper->player_balance(chair)) 
			{
                t.Format("Out (%.2f)", p_scraper->player_balance(chair));
            }
            else 
			{
                t.Format("Out (%d)", (int) p_scraper->player_balance(chair));
            }
        }
    }
    else 
	{
        pTempPen = (CPen*)pDC->SelectObject(&white_dot_pen);
        oldpen.FromHandle((HPEN)pTempPen);					// Save old pen
        pTempBrush = (CBrush*)pDC->SelectObject(&gray_brush);
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
    if (balance_rect_last[chair].right - balance_rect_last[chair].left != drawrect.right - drawrect.left) 
	{
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

    __SEH_LOGFATAL("COpenHoldemView::draw_balance_box\n");

}


void COpenHoldemView::draw_player_bet(int chair) 
{
    __SEH_HEADER

    CPen		*pTempPen, oldpen;
    CBrush		*pTempBrush, oldbrush;
    RECT		textrect, drawrect;
    CFont		*oldfont, cFont;
    CString		t;
    RECT		cr;
    static RECT	bet_rect_last[10];
    int			xcenter, ycenter, xadj, yadj;
    CDC			*pDC = GetDC();

	// Get size of current client window
    GetClientRect(&cr);

    // Draw background colored rectangle over position of previous bet to erase it
    pTempPen = (CPen*)pDC->SelectObject(&null_pen);
    oldpen.FromHandle((HPEN)pTempPen);					// Save old pen
    pTempBrush = (CBrush*)pDC->SelectObject(&gray_brush);
    oldbrush.FromHandle((HBRUSH)pTempBrush);			// Save old brush
    pDC->Rectangle(bet_rect_last[chair].left, bet_rect_last[chair].top,
                   bet_rect_last[chair].right, bet_rect_last[chair].bottom);
    pDC->SelectObject(oldpen);
    pDC->SelectObject(oldbrush);

    // Background color
    pDC->SetBkColor(COLOR_GRAY);

    // Figure placement
    xcenter = cr.right * pc[p_global->trans.map.num_chairs][chair][0];
    ycenter = cr.bottom * pc[p_global->trans.map.num_chairs][chair][1];
    xadj = pcbet[p_global->trans.map.num_chairs][chair][0];
    yadj = pcbet[p_global->trans.map.num_chairs][chair][1];

    // Set font basics
    lf.lfHeight = -12;
    lf.lfWeight = FW_NORMAL;
    cFont.CreateFontIndirect(&lf);
    oldfont = pDC->SelectObject(&cFont);
    pDC->SetTextColor(COLOR_WHITE);

    // Format text
    if (p_scraper->player_bet(chair) != 0) 
	{
        if ((int) p_scraper->player_bet(chair) != p_scraper->player_bet(chair)) 
		{
            t.Format("%.2f", p_scraper->player_bet(chair));
        }
        else 
		{
            t.Format("%d", (int) p_scraper->player_bet(chair));
        }
    }
    else 
	{
        t = "";
    }

    // Calc rectangle size for text
    textrect.left = 0;
    textrect.top = 0;
    textrect.right = 0;
    textrect.bottom = 0;
    pDC->DrawText(t.GetString(), t.GetLength(), &textrect, DT_CALCRECT);

    // Figure out placement of rectangle
    if (xadj<0) 
	{
        drawrect.left = xcenter + xadj - textrect.right;
        drawrect.top = ycenter + yadj - textrect.bottom/2;
        drawrect.right = xcenter + xadj;
        drawrect.bottom = ycenter + yadj + textrect.bottom/2;
    }
    else if (xadj>0) 
	{
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

    __SEH_LOGFATAL("COpenHoldemView::draw_player_bet\n");

}

