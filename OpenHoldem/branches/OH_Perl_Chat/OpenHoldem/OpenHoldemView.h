#pragma once

#define DISPLAY_UPDATE_TIMER	111

// OpenHoldemView.h : interface of the COpenHoldemView class

class COpenHoldemView : public CView {
protected: // create from serialization only
	COpenHoldemView();
	DECLARE_DYNCREATE(COpenHoldemView)
	DECLARE_MESSAGE_MAP()

	void draw_center_info_box(void);
	void draw_button_indicators(void);
	void draw_specific_button_indicator(int button_num, char ch, int left, int top, int right, int bottom);
	void draw_seated_active_circle(int chair);
	void draw_card(unsigned int card, int left, int top, int right, int bottom, bool pl_card);
	void draw_dealer_button(int chair); 
	void draw_name_box(int chair); 
	void draw_balance_box(int chair); 
	void draw_player_bet(int chair);
	afx_msg void OnTimer(UINT nIDEvent);
	void update_display(bool update_all);

	CPen		black_pen, green_pen, red_pen, blue_pen, white_dot_pen, null_pen;
	CBrush		white_brush, gray_brush, red_brush, yellow_brush;
	LOGFONT		lf;
	RECT		m_rectDraw;


public:
	COpenHoldemDoc* GetDocument() const;
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual ~COpenHoldemView();
	virtual void OnInitialUpdate();
};

inline COpenHoldemDoc* COpenHoldemView::GetDocument() const
   { return reinterpret_cast<COpenHoldemDoc*>(m_pDocument); }


