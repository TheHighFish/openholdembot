#ifndef INC_MAINFRM_H
#define INC_MAINFRM_H

#include "MyCToolBar.h"
#include "..\CTablemap\CTablemap.h"
#include "..\CCritSec\CCritSec.h"

#define		HWND_CHECK_TIMER				1
#define		ENABLE_BUTTONS_TIMER			2
#define		UPDATE_STATUS_BAR_TIMER			3
#define		ATTACH_WINDOW_TIMER				4

extern class CMainFrame : public CFrameWnd 
{
protected: // create from serialization only
	DECLARE_DYNCREATE(CMainFrame)
	DECLARE_MESSAGE_MAP()

	CMainFrame();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileOpen();
	afx_msg void OnEditFormula();
	afx_msg void OnEditForceuserchair();
	afx_msg void OnScraperOutput();
	afx_msg void OnViewShootreplayframe();
	afx_msg void OnEditPreferences();
	afx_msg void OnFileLoadOpenPPL();
	afx_msg void OnFileLoadTableMap();
	afx_msg void OnDllLoad();
	afx_msg void OnBnClickedRedCircle();
	afx_msg void OnBnClickedGreenCircle();
	afx_msg void OnClickedFlags();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateStatus(CCmdUI *pCmdUI);
	afx_msg void OnAutoplayer();
	afx_msg void OnValidator();
	afx_msg void OnMinMax();
	afx_msg void OnAttachTop();
	afx_msg void OnAttachBottom();
	afx_msg void OnLockBlinds();
	afx_msg void OnPokerproConnect();

	afx_msg void OnUpdateMenuFileNew(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuFileOpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuFileLoadProfile(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileConnect(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileDisconnect(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuDllLoad(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDllLoadspecificfile(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewMainToolbar(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewFlagsToolbar(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewStatusbar(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePokerproConnect(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewShootreplayframe(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditForceuserchair(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuPerlReloadFormula(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuPerlCheckSyntax(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuPerlEditMainFormula(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuPerlLoad(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuPerlLoadSpecificFormula(CCmdUI* pCmdUI);

	afx_msg void OnFormulaViewMainToolbar();
	afx_msg void OnFormulaViewFlagsToolbar();
	afx_msg void OnFormulaViewStatusbar();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDllLoadspecificfile();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg LRESULT OnConnectMessage(WPARAM, LPARAM hwnd);
	afx_msg LRESULT OnDisconnectMessage(WPARAM, LPARAM);
	afx_msg LRESULT OnConnectedHwndMessage(WPARAM, LPARAM);
	afx_msg LRESULT OnSetWindowText(WPARAM, LPARAM title);
	afx_msg void OnPerlLoadFormula();
	afx_msg void OnPerlLoadSpecificFormula();
	afx_msg void OnPerlEditMainFormula();
	afx_msg void OnPerlCheckSyntax();
	afx_msg void OnPerlReloadFormula();
	afx_msg void OnHelp();
	afx_msg void OnHelpWiki();
	afx_msg void OnHelpForums();

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual ~CMainFrame();
	virtual BOOL DestroyWindow();
	CMyToolBar		m_MainToolBar;

	void	SetMainWindowTitle(LPCSTR title);
	CString	_exec_filename;

public:
	// public accessors
	const bool flags(const int i) { if (i>=0 && i<=19) return _flags[i]; else return false; }
	const bool wait_cursor() { return _wait_cursor; }

public:
	void DisableButtonsOnConnect();
	void EnableButtonsOnDisconnect();
	void UnattachOHFromPokerWindow();
public:
	void UpdateWindowTitle();
	void StartTimer();
	void ResetDisplay();
	void KillTimer();

public:
#define ENT CSLock lock(m_critsec);
	// public mutators
	void set_flags(const int i, const bool b) { ENT if (i>=0 && i<=19) _flags[i] = b; }
	void set_wait_cursor(const bool b) { ENT _wait_cursor = b; }
#undef ENT

private:
	// private variables - use public accessors and public mutators to address these
	bool			_flags[20];		 // Flags button status
	bool			_wait_cursor;	 // Used if we need to display a wait cursor anywhere

private:
	// private functions and variables - not available via accessors or mutators
	int CreateMainToolbar(void);
	int CreateFlagsToolbar(void);
	void AlignToolbars(void);
	int CreateStatusBar(void);

	CStatusBar		_status_bar;
	CMyToolBar		_tool_bar;
	CString			_status_plcards, _status_comcards, _status_pokerhand, _status_prwin, _status_nopp;
	CString			_status_nit, _status_action, _status_handrank;
	RECT			_table_view_size;
	bool			_autoplay_pressed;
	RECT			_prev_att_rect, _prev_wrect;

	CCritSec		m_critsec;
} *p_mainframe;

// used by EnumProcTopLevelWindowList function
extern CArray <STableList, STableList>		g_tlist; 

BOOL CALLBACK EnumProcTopLevelWindowList(HWND hwnd, LPARAM lparam);
bool check_window_match(SWholeMap *map, HWND h, RECT r, CString title);

#endif /* INC_MAINFRM_H */
