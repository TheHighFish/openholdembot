//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*****************************************************************************
//
// Purpose:
//
//*****************************************************************************


// MainFrm.h : interface of the CMainFrame class
//
#pragma once
#include "OpenScrapeDoc.h"

#define		BLINKER_TIMER				1

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewRefresh();
	afx_msg void OnViewPrev();
	afx_msg void OnViewNext();
	afx_msg void OnToolsCloneRegions();
	afx_msg void OnViewShowregionboxes();
	afx_msg void OnEditUpdatehashes();
	afx_msg void OnEditDuplicateregion();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnGroupregionsBytype();
	afx_msg void OnGroupregionsByname();
	afx_msg void OnViewUngroupregions();
	afx_msg void OnUpdateViewShowregionboxes(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewCurrentwindowsize(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditDuplicateregion(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGroupregionsBytype(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGroupregionsByname(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewUngroupregions(CCmdUI *pCmdUI);
	void SaveBmpPbits(void);
	DECLARE_MESSAGE_MAP()

	CStatusBar		m_wndStatusBar;
	CToolBar		m_wndToolBar;

public:
	virtual BOOL DestroyWindow();
	afx_msg void OnViewConnecttowindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual ~CMainFrame();

	// Flag indicating whether red regions are shown or not
	bool show_regions;

private:
	bool CreateToolbar();
	bool CreateStatusBar();
	void ForceRedraw();
	void ResizeWindow(COpenScrapeDoc *pDoc);
	void BringOpenScrapeBackToFront();
	void SetTablemapSizeIfUnknown(int size_x, int size_y);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

// used by EnumProcTopLevelWindowList function
extern CArray <STableList, STableList>		g_tlist; 

BOOL CALLBACK EnumProcTopLevelWindowList(HWND hwnd, LPARAM lparam);
