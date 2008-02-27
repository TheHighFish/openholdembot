// MainFrm.h : interface of the CMainFrame class
//
#pragma once
#include "structs_defines.h"

#define		BLINKER_TIMER				1

// Global variable to hold the startup path
extern char		startup_path[MAX_PATH];

// Global variable for holding table list by callback function
extern CArray <STableList, STableList>		g_tlist;

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewRefresh();
	afx_msg void OnViewShowregionboxes();
	afx_msg void OnEditUpdatehashes();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnUpdateViewShowregionboxes(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewCurrentwindowsize(CCmdUI *pCmdUI);
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

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

BOOL CALLBACK EnumProcTopLevelWindowList(HWND hwnd, LPARAM lparam);
