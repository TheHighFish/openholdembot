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

#ifndef INC_OPENHOLDEMVIEW_H
#define INC_OPENHOLDEMVIEW_H

#include "OpenHoldemDoc.h"

#define DISPLAY_UPDATE_TIMER	111

// OpenHoldemView.h : interface of the COpenHoldemView class

class COpenHoldemView : public CView 
{
protected: // create from serialization only
	COpenHoldemView();
	DECLARE_DYNCREATE(COpenHoldemView)
	DECLARE_MESSAGE_MAP()

	afx_msg void OnTimer(UINT nIDEvent);

public:
	COpenHoldemDoc* GetDocument() const;
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual ~COpenHoldemView();
	virtual void OnInitialUpdate();

private:
	// private functions and variables - not available via accessors or mutators
	void UpdateDisplay(const bool update_all);
	void DrawCenterInfoBox();
	void DrawButtonIndicators();
	void DrawSpecificButtonIndicator(const int button_num, const char ch, const int left, const int top, const int right, const int bottom);
	void DrawSeatedActiveCircle(const int chair);
	void DrawCard(const unsigned int card, const int left, const int top, const int right, const int bottom, const bool pl_card);
	void DrawDealerButton(const int chair); 
	void DrawNameBox(const int chair); 
	void DrawBalanceBox(const int chair); 
	void DrawPlayerBet(const int chair);
	void DrawPlayerCards(const int chair);

	CString			_handnumber_last;
	double			_sblind_last, _bblind_last, _lim_last, _ante_last, _pot_last;
	unsigned int	_iterator_thread_progress_last;
	unsigned int	_card_common_last[k_number_of_community_cards];
	unsigned int	_card_player_last[k_max_number_of_players][k_number_of_cards_per_player];
	CString			_seated_last[k_max_number_of_players];
	CString			_active_last[k_max_number_of_players];
	CString			_playername_last[k_max_number_of_players];
	bool			_dealer_last[k_max_number_of_players]; 
	bool			_istournament_last; 
	double			_playerbalance_last[k_max_number_of_players]; 
	double			_playerbet_last[k_max_number_of_players];
	LOGFONT			_logfont;
	CPen			_black_pen, _green_pen, _red_pen, _blue_pen, _white_dot_pen, _null_pen;
	CBrush			_white_brush, _gray_brush, _red_brush, _yellow_brush;
	RECT			_client_rect;
};

inline COpenHoldemDoc* COpenHoldemView::GetDocument() const
   { return reinterpret_cast<COpenHoldemDoc*>(m_pDocument); }


#endif //INC_OPENHOLDEMVIEW_H