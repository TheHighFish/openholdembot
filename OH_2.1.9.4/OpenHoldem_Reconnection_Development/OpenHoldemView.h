#ifndef INC_OPENHOLDEMVIEW_H
#define INC_OPENHOLDEMVIEW_H

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
	void DrawCenterInfoBox(void);
	void DrawButtonIndicators(void);
	void DrawSpecificButtonIndicator(const int button_num, const char ch, const int left, const int top, const int right, const int bottom);
	void DrawSeatedActiveCircle(const int chair);
	void DrawCard(const unsigned int card, const int left, const int top, const int right, const int bottom, const bool pl_card);
	void DrawDealerButton(const int chair); 
	void DrawNameBox(const int chair); 
	void DrawBalanceBox(const int chair); 
	void DrawPlayerBet(const int chair);

	double			_handnumber_last, _sblind_last, _bblind_last, _lim_last, _ante_last, _pot_last;
	unsigned int	_iterator_thread_progress_last, _card_common_last[5], _card_player_last[10][2];
	CString			_seated_last[10], _active_last[10], _playername_last[10];
	bool			_dealer_last[10], _istournament_last;
	double			_playerbalance_last[10], _playerbet_last[10];
	LOGFONT			_logfont;
	CPen			_black_pen, _green_pen, _red_pen, _blue_pen, _white_dot_pen, _null_pen;
	CBrush			_white_brush, _gray_brush, _red_brush, _yellow_brush;

};

inline COpenHoldemDoc* COpenHoldemView::GetDocument() const
   { return reinterpret_cast<COpenHoldemDoc*>(m_pDocument); }


#endif //INC_OPENHOLDEMVIEW_H