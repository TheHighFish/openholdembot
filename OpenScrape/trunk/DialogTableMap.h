#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "resource.h"
#include "StickyButton.h"

// CDlgTableMap dialog

class CDlgTableMap : public CDialog
{
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	void clear_bitmap_control(void);
	void draw_region_bitmap(void);
	void draw_image_bitmap(void);
	afx_msg void OnRegionChange();
	afx_msg void OnZoomChange();
	afx_msg void OnTvnSelchangedTablemapTree(NMHDR *pNMHDR, LRESULT *pResult);
	void disable_and_clear_all(void);
	void update_r$_display(bool dont_update_spinners);
	void update_t$_display();
	afx_msg void OnDeltaposLeftSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposTopSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposBottomSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposRightSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposRadiusSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedNew();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedEdit();
	afx_msg void OnBnClickedCreateImage();
	afx_msg void OnBnClickedCreateFont();
	afx_msg void OnBnClickedCreateHash();
	afx_msg void OnBnClickedFontplus();
	afx_msg void OnBnClickedFontminus();
	afx_msg void OnBnClickedNudgeTaller();
	afx_msg void OnBnClickedNudgeShorter();
	afx_msg void OnBnClickedNudgeWider();
	afx_msg void OnBnClickedNudgeNarrower();
	afx_msg void OnBnClickedNudgeBigger();
	afx_msg void OnBnClickedNudgeSmaller();
	afx_msg void OnBnClickedNudgeUpleft();
	afx_msg void OnBnClickedNudgeUp();
	afx_msg void OnBnClickedNudgeUpright();
	afx_msg void OnBnClickedNudgeRight();
	afx_msg void OnBnClickedNudgeDownright();
	afx_msg void OnBnClickedNudgeDown();
	afx_msg void OnBnClickedNudgeDownleft();
	afx_msg void OnBnClickedNudgeLeft();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT OnStickyButtonDown(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnStickyButtonUp(WPARAM wp, LPARAM lp);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnToolTipText(UINT id, NMHDR* pTTTStruct, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSizing(UINT nSide, LPRECT lpRect);
	COLORREF get_color_under_mouse(UINT *nFlags, CPoint *point);

	CStatic				m_BitmapFrame;
	CStickyButton		m_Picker;
	CSpinButtonCtrl		m_LeftSpin, m_TopSpin, m_BottomSpin, m_RightSpin, m_RadiusSpin;
	CComboBox			m_Transform, m_Zoom, m_TrackerFontSet, m_TrackerFontNum, m_TrackerCardNum;
	CEdit				m_Alpha, m_Red, m_Green, m_Blue, m_RedAvg, m_GreenAvg, m_BlueAvg, m_Radius, m_Result, m_PixelSeparation;
	CButton				m_New, m_Delete, m_Edit, m_CreateHash, m_CreateImage, m_CreateFont, m_FontPlus, m_FontMinus;
	CButton				m_NudgeTaller, m_NudgeShorter, m_NudgeWider, m_NudgeNarrower, m_NudgeBigger, m_NudgeSmaller;
	CButton				m_NudgeUpLeft, m_NudgeUp, m_NudgeUpRight, m_NudgeRight, m_NudgeDownRight, m_NudgeDown;
	CButton				m_NudgeDownLeft, m_NudgeLeft;
	CPen				black_pen, green_pen, red_pen, blue_pen, white_dot_pen, null_pen;
	CBrush				white_brush, lt_gray_brush, gray_brush, red_brush, yellow_brush;
	LOGFONT				lf_fixed;
	CFont				separation_font, nudge_font;
	int					separation_font_size;
	CBitmap				picker_bitmap, drawrect_bitmap;
	HBITMAP				h_picker_bitmap, h_drawrect_bitmap;
	bool				picker_cursor;
	HCURSOR				hCurPicker, hCurStandard;
	bool				ignore_changes;
	CStatusBarCtrl		m_status;

public:
	virtual BOOL DestroyWindow();
	CDlgTableMap(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTableMap();
	enum { IDD = IDD_TABLEMAP };
	void create_tree(void);
	void update_display(void);
	void UpdateStatus(void);
	HTREEITEM update_tree(CString node_text);

	CTreeCtrl			m_TableMapTree;
	CEdit				m_Left, m_Top, m_Bottom, m_Right;
	CStickyButton		m_DrawRect;

	DECLARE_MESSAGE_MAP()
};


